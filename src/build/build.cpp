#include "build.h"
#include "../common/ys_datapool.h"

#include <stdio.h>

namespace ys {

build::build(char* buildPath) {
    path = buildPath;
    docIndex = 0;
    fIndex = 0;
}

singleBuild::singleBuild(char* buildPath)
    : build(buildPath) {
}

unsigned build::gen_key(unsigned v) {
    return v >> 1;
}

unsigned build::gen_key2(unsigned x, unsigned y) {
    return ((y >> 16) << 15) + ((x << 16) >> 17);
}

void singleBuild::addDoc(char* buf, int len, int orgId) {
    int docId = docIndex++;
    for (int i = 0; i < len; i += 4) {
        unsigned int k = *((unsigned int*)(buf+i));
        if (!(k & 1)) {
            continue;
        }
        indexData iData;
        iData.key = gen_key(*((int*)(buf+i)));
        iData.docId = docId;
        iData.offset = i;
        iPool.push_back(iData);
    }
    positMeta pMeta;
    pMeta.docId = docId;
    pMeta.orgId = orgId;
    pMeta.offset = pDPool.getCurLen();
    pMeta.length = len;
    pMPool.push_back(pMeta);
    pDPool.push_back(buf, len);
    if (pDPool.getCurLen() > MAXLEN) {
        writeIndexData();
        writePositMeta();
        writePositData();
        fIndex++;
    }
}

void singleBuild::writeIndexData() {
    char filename[128] = {0};
    sprintf (filename, "%s/%d.in", path, fIndex);
    FILE* file1 = fopen(filename, "wb");
    sprintf (filename, "%s/%d.iv", path, fIndex);
    FILE* file2 = fopen(filename, "wb");
    iPool.sort();
    int curkey = iPool[0].key;
    int curoffset = 0;
    int curlength = 0;
    for (int i = 0; i < iPool.getCurLen(); ++i) {
        if (iPool[i].key != curkey) {
            fwrite(&iPool[i].key, sizeof(int), 1, file1);
            fwrite(&curoffset, sizeof(long), 1, file1);
            fwrite(&curlength, sizeof(int), 1, file1);
            curoffset += curlength;
            curlength = 0;
            curkey = iPool[i].key;
        }
        fwrite(&iPool[i].docId, sizeof(int), 1, file2);
        fwrite(&iPool[i].offset, sizeof(long), 1, file2);
        curlength ++;
    }
    iPool.reset();
    fclose(file1);
    fclose(file2);
}

void singleBuild::writePositMeta() {
    char filename[128] = {0};
    sprintf (filename, "%s/%d.po", path, fIndex);
    FILE* file = fopen(filename, "wb");
    for (int i = 0; i < pMPool.getCurLen(); ++i) {
        fwrite(&pMPool[i].docId, sizeof(int), 1, file);
        fwrite(&pMPool[i].orgId, sizeof(int), 1, file);
        fwrite(&pMPool[i].offset, sizeof(long), 1, file);
        fwrite(&pMPool[i].length, sizeof(int), 1, file);
    }
    pMPool.reset();
    fclose(file);
}


void singleBuild::writePositData() {
    char filename[128] = {0};
    sprintf (filename, "%s/%d.pv", path, fIndex);
    FILE* file = fopen(filename, "wb");
    for (int i = 0; i < pDPool.getCurLen(); ++i) {
        fwrite(&pDPool[i], sizeof(positData), 1, file);
    }
    pDPool.reset();
    fclose(file);
}

void singleBuild::writeFlush() {
    writeIndexData();
    writePositMeta();
    writePositData();
    fIndex++;
}

}

using namespace ys;

#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
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
