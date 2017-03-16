#include "../build/ys_build.h"
#include "../common/ys_datapool.h"

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>

using namespace ys;

void help() {
    printf ("usage :\n\t-a:afpPath\n\t-b:buildPath\n");
}

int main(int argc, char **argv) {
    char *afpPath = 0;
    char *buildPath = 0;
    int ch = -1;

    while (-1 != (ch=getopt(argc, argv, "a:b:"))) {
        switch(ch) {
        case 'a': {
            afpPath = optarg;
            break;
        }
        case 'b': {
            buildPath = optarg;
            break;
        }
        default: {
            help();
            break;
        }
        }
    }
    if (!afpPath || !buildPath) {
        help();
        return -1;
    }
    singleBuild builder(buildPath);
    DIR *dp;
    struct dirent *dirp;
    char fileName[256];
    if (!(dp = opendir(afpPath))) {
        return -1;
    }
    while (dirp = readdir(dp)) {
        if (!strcmp(".", dirp->d_name) ||
            !strcmp("..", dirp->d_name)) {
            continue;
        }
        sprintf (fileName, "%s/%s", afpPath, dirp->d_name);
        FILE *file = fopen(fileName, "r");
        fseek(file, 0, SEEK_END);
        int len = ftell(file);
        fseek(file, 0, SEEK_SET);
        char *content = new char[len];
        fread(content, len, 1, file);
        int orgid = atoi(dirp->d_name);
        builder.addDoc(content, len, orgid);
        delete content;
    }
    builder.writeFlush();
    return 0; 
}

