/**
 * author : sivan yue
 * data : 2017-03-15
 */
#ifndef _PQUEUE_H_
#define _PQUEUE_H_

#include <malloc.h>
#include <new>
using namespace std;

namespace ys {

#define L_SON(_POS) (2*_POS+1)
#define R_SON(_POS) (2*_POS+2)
#define P_SON(_POS) ((_POS-1)/2)

template <class T>
class baseHeap {
public:
    baseHeap(int length = 10);
    ~baseHeap();
public:
    bool pop(T& v);
    bool peek(T& v);
    bool push(const T& v);
    int getCurlength()const;
protected:
    void adjustUp(int pos);
    void adjustDown(int pos);
    virtual bool match(const T& l, const T& r) = 0;
protected:
    T* data;
    int curlength;
    int maxlength;
private:
    baseHeap(const baseHeap&);
    baseHeap& operator=(const baseHeap&);
};

template <class T>
class maxHeap : public baseHeap<T> {
protected:
    virtual bool match(const T& l, const T& r);
};

template <class T>
class minHeap : public baseHeap<T> {
protected:
    virtual bool match(const T& l, const T& r);
};

template <class T>
inline baseHeap<T>::baseHeap(int length) {
    maxlength = length;
    curlength = 0;
    data = (T*)malloc(sizeof(T)*maxlength);
}

template <class T>
inline baseHeap<T>::~baseHeap() {
    for (int i = 0; i < curlength; ++i) {
        (data+i)->~T();
    }
    free(data);
}

template <class T>
inline bool baseHeap<T>::pop(T& v) {
    if (!curlength) {
        return 0;
    }
    v = data[0];
    data[0] = data[curlength - 1];
    (data+curlength-1)->~T();
    curlength --;
    adjustDown(0);
    return 1;
}

tmplate <class T>
inline bool baseHeap<T>::peek(T& v) {
    if (!curlength) {
        return 0;
    }
    v = data[0];
}

template <class T>
inline bool baseHeap<T>::push(const T& v) {
    if (curlength >= maxlength) {
        return 0;
    }
    new (data+curlength)T(v);
    curlength ++;
    adjustUp(curlength-1);
    return 1;
}

template <class T>
inline int baseHeap<T>::getCurlength()const {
    return curlength;
}

template <class T>
inline void baseHeap<T>::adjustDown(int pos) {
    int lson = L_SON(pos); 
    int rson = R_SON(pos);
    if (lson >= curlength) {
        return;
    }
    if (lson == curlength - 1) {
        if (match(data[lson], data[pos])) {
            T tmp = data[pos];
            data[pos] = data[lson];
            data[lson] = tmp;
        }
        return;
    }
    int tmpPos;
    if (match(data[lson], data[rson])) {
        tmpPos = lson;
    }
    else {
        tmpPos = rson;
    }
    if (match(data[tmpPos], data[pos])) {
        T tmp = data[pos];
        data[pos] = data[tmpPos];
        data[tmpPos] = tmp;      
        adjustDown(tmpPos);
    }
}

template <class T>
inline void baseHeap<T>::adjustUp(int pos) {
    if (!pos) {
        return;
    }
    int pPos = P_SON(pos);
    if (match(data[pPos], data[pos])) {
        return;
    }
    T tmp = data[pPos];
    data[pPos] = data[pos];
    data[pos] = tmp;
    adjustUp(pPos);
}

template <class T>
inline bool maxHeap<T>::match(const T& l, const T& r) {
    return l > r;
}

template <class T>
inline bool minHeap<T>::match(const T& l, const T& r) {
    return l < r;
}

}

#endif

