#include "build.h"
#include "../common/ys_datapool.h"

#include <stdio.h>
#include <stdlib.h>

namespace ys {

unsigned build::gen_key(unsigned v) {
    return v >> 1;
}

unsigned build::gen_key2(unsigned x, unsigned y) {
    return ((y >> 16) << 15) + ((x << 16) >> 17);
}

void singleBuild::addDoc(char* buf, int len, int orgId) {
    printf ("2\n");
    int docId = docIndex++;
    printf ("orgid = %d docId = %d len = %d\n", orgId, docIndex, len);
    for (int i = 0; i < len; i += 4) {
        // create indexMeta
        unsigned int k = *((unsigned int*)(buf+i));
        if (k & 1 == 0) {
            continue;
        }
        indexData iData;
        iData.key = gen_key(*((int*)(buf+i)));
        iData.docId = docId;
        iData.offset = i;
        iDataPool.push_back(iData);
    }
    // create positMeta
    positMeta pMeta;
    pMeta.docId = docId;
    pMeta.orgId = orgId;
    pMeta.offset = pDataPool.getCurLen();
    pMeta.length = len;
    pMetaPool.push_back(pMeta);
    // create positData
    pDataPool.push_back(buf, len);
    if (pDataPool.getCurLen() > MAXLEN) {
        writeIndexData();
        writePositMeta();
        writePositData();
        iDataPool.reset();
        pMetaPool.reset();
        pDataPool.reset();
        fIndex++;
    }
}

void singleBuild::writeIndexData() {
    char filename[128] = {0};
    sprintf (filename, "/home/xiwen.yxw/index/%d.in", fIndex);
    FILE* file1 = fopen(filename, "wb");
    sprintf (filename, "/home/xiwen.yxw/index/%d.iv", fIndex);
    FILE* file2 = fopen(filename, "wb");
    iDataPool.sort();
    int curkey = iDataPool[0].key;
    int curoffset = 0;
    int curlength = 0;
    for (int i = 0; i < iDataPool.getCurLen(); ++i) {
        // key|offset|length
        if (iDataPool[i].key != curkey) {
            fwrite(&iDataPool[i].key, sizeof(int), 1, file1);
            fwrite(&curoffset, sizeof(long), 1, file1);
            fwrite(&curlength, sizeof(int), 1, file1);
            curoffset += curlength;
            curlength = 0;
            curkey = iDataPool[i].key;
        }
        // docId|offset
        fwrite(&iDataPool[i].docId, sizeof(int), 1, file2);
        fwrite(&iDataPool[i].offset, sizeof(long), 1, file2);
        curlength ++;
    }
    fclose(file1);
    fclose(file2);
}

void singleBuild::writePositMeta() {
    char filename[128] = {0};
    sprintf (filename, "/home/xiwen.yxw/index/%d.po", fIndex);
    FILE* file = fopen(filename, "wb");
    for (int i = 0; i < pMetaPool.getCurLen(); ++i) {
        // docId|orgid|offset|length
        fwrite(&pMetaPool[i].docId, sizeof(int), 1, file);
        fwrite(&pMetaPool[i].orgId, sizeof(int), 1, file);
        fwrite(&pMetaPool[i].offset, sizeof(long), 1, file);
        fwrite(&pMetaPool[i].length, sizeof(int), 1, file);
    }
    fclose(file);
}


void singleBuild::writePositData() {
    char filename[128] = {0};
    sprintf (filename, "/home/xiwen.yxw/index/%d.pv", fIndex);
    FILE* file = fopen(filename, "wb");
    for (int i = 0; i < pDataPool.getCurLen(); ++i) {
        fwrite(&pDataPool[i], sizeof(positData), 1, file);
    }
    fclose(file);
}

}

using namespace ys;

#include <dirent.h>


int main(int argc, char ** argv) {
    printf ("0.1\n");
    singleBuild builder;
    printf ("0.2\n");
    DIR *dp;
    struct dirent *dirp;
    char fileName[256];
    if (!(dp = opendir("/home/xiwen.yxw/afp/"))) {
        return -1;
    }
    while (dirp = readdir(dp)) {
        if (!strcmp(".", dirp->d_name) ||
            !strcmp("..", dirp->d_name)) {
            continue;
        }
        sprintf (fileName, "%s/%s", "/home/xiwen.yxw/afp/", dirp->d_name);
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

    return 0;
}
