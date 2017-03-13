#include "build.h"
#include "../common/ys_datapool.h"

uint32_t build::gen_key(uint32_t v) {
    return v >> 1;
}

uint32_t build::gen_key2(uint32_t x, uint32_t y) {
    return ((y >> 16) << 15) + ((x << 16) >> 17);
}

void singleBuild::addDoc(char* buf, int len, int orgId) {
    int docId = docIndex++;
    dataPool<indexData> iMetaPool;
    dataPool<positMeta> pMetaPool;
    dataPool<positData> pDataPool;
    for (int i = 0; i < len; i += 4) {
        if (pDataPool.getCurLen() > MAXLEN) {
            writeIndex(iMetaPool);
            writePositMeta(pMetaPool);
            writePositData(pDataPool);
            docIndex++;
        }
        // create indexMeta
        indexData iData;
        iData.key = gen_key(*((int*)(buf+i)));
        iData.docId = docId;
        iData.offset = i;
        iDataPool.push_back(iData);
        // create positMeta
        positMeta pMeta;
        pMeta.docId = docId;
        pMeta.orgId = orgId;
        pMeta.offset = pDataPool.getCurLen();
        pMeta.length = len;
        pMetaPool.push_back(pMeta);
        // create positData
        pDataPool.push_back(buf, len);
    }
}

void singleBuild::writeIndex(dataPool<indexData>& iDataPool) {
    char filename[128] = {0};
    sprintf (filename, "/home/xiwen.yxw/index/%s.in", docIndex);
    FILE* file1 = open(filename, "rw");
    sprintf (filename, "/home/xiwen.yxw/index/%s.iv", docIndex);
    FILE* file2 = open(filename, "rw");
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
        }
        // docId|offset
        fwrite(&iDataPool[i].docId, sizeof(int), 1, file2);
        fwrite(&iDataPool[i].offset, sizeof(long), 1, file2);
        curlength ++;
    }
    fclose(file1);
    fclose(file2);
}

void singleBuild::writePMeta(dataPool<positMeta>& pMetaPool) {
    char filename[128] = {0};
    sprintf (filename, "/home/xiwen.yxw/index/%s.po", docIndex);
    FILE* file = open(filename, "rw")
    for (int i = 0; i < pMetaPool.getCurLen(); ++i) {
        // docId|orgid|offset|length
        fwrite(&pMetaPool[i].docId, sizeof(int), 1, file);
        fwrite(&pMetaPool[i].orgId, sizeof(int), 1, file);
        fwrite(&pMetaPool[i].offset, sizeof(long), 1, file);
        fwrite(&pMetaPool[i].length, sizeof(int), 1, file);
    }
    fclose(file);
}


void singleBuild::writePData(dataPool<positData>& pDataPool) {
    char filename[128] = {0};
    sprintf (filename, "/home/xiwen.yxw/index/%s.po",docIndex);
    FILE* file = open(filename, "rw");
    for (int i = 0; i < pMetaPool.getCurLen(); ++i) {
        fwrite(&pDataPool[i], sizeofpositData(, 1, file));
    }
    fclose(file);
}

