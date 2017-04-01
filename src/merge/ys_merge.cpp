#include "ys_merge.h"
#include "../build/builder.h"

merge::merge(){}

merge::~merge(){}

void merge::mergeTask() {
}

bool merge::mergeIndex() {
    FILE *segment = fopen("", "rb");
    char fileName[256];
    FILE *fp;
    int fileNum, curKey = -1;
    long offset, curOffset;
    int length, curLength;
    minHeap<mergeMeta> mHeap; 
    mergeMeta mMeta;
    dataPool<iValueData> ivPool;

    while (fread(&fileNum, sizeof(int), 1, segment)) {
        sprintf (fileName, "%s%d", "", fileNum);
        file = fopen(fileName, "rb");
        if (!fread(&mMeta.meta, sizeof(int), 1, meta)) {
            goto Error;
        }
        mMeta.meta = file;
        sprintf (fileName, "%s%d", "", fileNum);
        data = fopen(fileName, "rb");
        mMeta.data = file;
        mHeap.push(mMeta);
    }
    mMeta.peek(mMeta);
    curKey = mMeta.key;
    offset = curOffset = 0;
    length = curLength = 0;
    while (mHeap.pop(mMeta)) {
        if (curKey != pMeta.key) {
            for (int i = 0; i < ivPool.getCurLen(); ++i) {
                fwrite(&ivPool[i], sizeof(iValueData),1, fp);
            }
            fwrite(&curKey, sizeof(curKey), 1, fp1);
            fwrite(&offset, sizeof(offset), 1, fp1);
            fwrite(&length, sizeof(length), 1, fp1);
            ivPool.reset();
            curKey = pMeta.key;
            offset += length;
            length = 0;
        }
        else {
            fread(&curOffset, sizeof(curOffset), 1, mMeta.meta);
            fread(&curLength, sizeof(curLength), 1, mMeta.meta);
            iValueData* vdataArr = new iValueData[curLength];
            fread(vdataArr, sizeof(iValueData), curLength, mMeta.data);
            ivPool.add(vdataArr, curLength);
            length += curLength;
            delete[] vdataArr;
        }
        if (fread(&mMeta.key, sizeof(curKey), 1, mMeta.meta)) {
            mHeap.push(mMeta);
        }
        else {
            fclose (mMeta.meta);
            fclose (mMeta.data);
        }
    }

    return true;
Error:
    return false;
}

void merge::mergePosit() {
}


