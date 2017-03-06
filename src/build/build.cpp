#include "build.h"
#include "../common/ys_datapool.h"

build::build() {
    docIndex = 0;
}

build::~build() {
}

singleBuild::singleBuild() {
}

singleBuild::~singleBuild() {
}

void singleBuild::addDoc(char* buf, int len, int orgId) {
    int docId = docIndex++;
    dataPool<indexData> iDataPool;
    dataPool<positMeta> pMetaPool;
    dataPool<positData> pDataPool;
    for (int i = 0; i < len; i += 4) {
        if (pDataPool.getCurLen() > MAXLEN) {
            writeIndex(iDataPool);
            writePositMeta(pMetaPool);
            writePositData(pDataPool);
        }
        indexMeta iData;
        positMeta pMeta;
        iData.docId = docId;
        iData.offset = i;
        iDataPool.push_back(iData);
        pMeta.docId = docId;
        pMeta.orgId = orgId;
        pMeta.offset = pDataPool.getCurLen();
        pMeta.length = len;
        pMetaPool.push_back(pMeta);
        pDataPool.push_back(buf, len);
    }
}

void singleBuild::writeIndex(dataPool<indexData>& iDataPool) {
    iDataPool.sort();
    int curDocId = iDataPool[0].docId;
    int curoffset = 0;
    int curlength = 0;
    for (int i = 0; i < iDataPool.getCurLen(); ++i) {
        if (iDataPool[i].docId != curDocId) {
            fwrite(&iDataPool[i].docId, sizeof(int), 1, file1);
            fwrite(&curoffset, sizeof(int), 1, file1);
            fwrite(&curlength, sizeof(int), 1, file1);
            curoffset += curlength;
            curlength = 0;
        }
        fwrite(&iDataPool[i].docId, sizeof(int), 1, file2);
        fwrite(&iDataPool[i].offset, sizeof(int), 1, file2);
        curlength ++;
    }
}

void singleBuild::writePMeta(dataPool<positMeta>& pMetaPool) {
    for (int i = 0; i < pMetaPool.getCurLen(); ++i) {
        fwrite(&pMetaPool[i].docId, sizeof(int), 1, file1);
        fwrite(&pMetaPool[i].orgId, sizeof(int), 1, file1);
        fwrite(&pMetaPool[i].offset, sizeof(int), 1, file1);
        fwrite(&pMetaPool[i].length, sizeof(int), 1, file1);
    }
    file1.close();
}


void singleBuild::writePData(dataPool<positData>& pDataPool) {
    for (int i = 0; i < pMetaPool.getCurLen(); ++i) {
        fwrite(&pDataPool[i], sizeofpositData(, 1, file));
    }
    file.close;
}

