#ifndef _DATA_POOL_H_
#define _DATA_POOL_H_

#include <stdio.h>
#include <string.h>

namespace ys {

template <typename T>
class dataPool {
public:
    dataPool();
    dataPool(int bsize, int blen);
    ~dataPool();
public:
    void push_back(T val);
    void push_back(T *val, int size);
    T& operator[](long index);
    long getCurLen() const;
    void sort();
private:
    T** data;                                   
    long curLen;                   
    int bunchSize;               
    int bunchLen;
private:
    dataPool(const dataPool&);
    dataPool& operator=(const dataPool&);
    void judgeSize();
    void resize();
    void sort(long left, long right);
    long partition(long left, long right);
};

template <typename T>
inline dataPool<T>::dataPool() {
    curLen = 0;
    bunchLen = 0x400;
    bunchSize = 10;
    data = new T*[bunchSize];
    for (int i = 0; i < bunchSize; ++i) {
        data[i] = 0;
    }
}

template <typename T>
inline dataPool<T>::dataPool(int bsize, int blen) {
    curLen = 0;
    bunchLen = blen;
    bunchSize = bsize;
    data = new T*[bunchSize];
    for (int i = 0; i < bunchSize; ++i) {
        data[i] = 0;
    }
}

template <typename T>
inline dataPool<T>::~dataPool() {
    for (int i = 0; i < bunchSize; ++i) {
        delete[] data[i];
    }
    delete[] data;
}

template <typename T>
inline void dataPool<T>::push_back(T val) {
    judgeSize();
    data[curLen/bunchLen][curLen%bunchLen] = val;
    curLen ++;
}

template <typename T>
inline void dataPool<T>::push_back(T *val, int size) {
    int pos = 0;
    while (pos < size) {
        judgeSize();
        int offset = curLen % bunchLen;
        int index = curLen / bunchLen;
        int len1 = size - pos;
        int len2 = bunchLen - offset;
        int len = len1 < len2 ? len1 : len2;
        memcpy(data[index]+offset, val+pos, sizeof(T)*len);
        curLen += len;
        pos += len;
    }
}

template <typename T>
inline T& dataPool<T>::operator[](long index) {
    if (index < 0 || index >= curLen) {
        throw index;
    }
    int iex = index / bunchLen;
    int off = index % bunchLen;
    return data[iex][off];
}

template <typename T>
inline void dataPool<T>::judgeSize() {
    if (curLen == bunchLen * bunchSize) {
        resize();
    }
    if (!(curLen % bunchLen)) {
        data[curLen/bunchLen] = new T[bunchLen];
    }
}

template <typename T>
inline void dataPool<T>::resize() {
    T** tmpData = new T*[bunchSize+10];
    int i = 0;
    while (i < bunchSize) {
        tmpData[i] = data[i++];
    }
    while (i < bunchSize + 10) {
        tmpData[i++] = 0;
    }
    bunchSize += 10;
    delete[] data;
    data = tmpData;
}

template <typename T>
inline long dataPool<T>::getCurLen() const{
    return curLen;
}

template <typename T>
inline void dataPool<T>::sort() {
    sort(0, curLen-1);
}

template <typename T>
inline void dataPool<T>::sort(long left, long right) {
    if (left >= right) {
        return;
    }
    int pos = partition(left, right);
    sort(left, pos-1);
    sort(pos+1, right);
}

template <typename T>
inline long dataPool<T>::partition(long left, long right) {
    int i, j = left;
    for (i = left; i < right; ++i) {
        if ((*this)[i] < (*this)[right]){
            T tmp = (*this)[i];
            (*this)[i] = (*this)[j];
            (*this)[j++] = tmp;
        }
    }
    T tmp = (*this)[right];
    (*this)[right] = (*this)[j];
    (*this)[j] = tmp;
    return j;
}

}

#endif
