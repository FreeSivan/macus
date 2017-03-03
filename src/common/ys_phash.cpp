#include "ys_phash.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
namespace ys {

phash::phash() {
    first = new uint16[F_LENGTH];
    second = new uint32[S_LENGTH];
    third = new uint64[T_LENGTH];
    foffset = soffset = toffset = lastKey = 0;
}

phash::~phash() {
    delete[] first;
    delete[] second;
    delete[] third;
}

long phash::getpHash(uint32 key) {
    int i1 = key >> 21;
    int i2 = key >> 6;
    int i3 = key;
    return first[i3]+second[i2]+third[i1];
}

void phash::addphKey(uint32 key, int length) {
    if (lastKey+1 < key) {
        uint32 i = lastKey+1;
        for (; i < key; ++i) {
            generate(i);
        }    
    }
    generate(key);
    foffset += length;
    lastKey = key;
}

void phash::addOver() {
    for (int i = lastKey+1; i < F_LENGTH; ++i) {
        addphKey(i, 0);
    }
}

void phash::generate(uint32 key) {
    if (lastKey >> 21 != key >> 21) {
        toffset += soffset;
        third[key>>21] = toffset;
        soffset = 0;
    }
    if (lastKey >> 6 != key >> 6) {
        soffset += foffset;
        second[key>>6] = soffset;
        foffset = 0;
    }
    first[key] = foffset;
}

}

