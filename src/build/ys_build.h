#ifndef _BUILD_H_
#define _BUILD_H_
#include "../common/ys_datapool.h"

namespace ys {

#define MAXLEN (30*1024*1024)

struct indexData {
    int key;                  // sub finger
    int docId;                // document id
    int offset;               // the offset in origin file
    bool operator<(const indexData& v) {
        return key < v.key;
    }
};

struct positMeta {
    int docId;                // document id
    int orgId;                // origin id
    long offset;              // the offset in positData
    int length;               // the length of this file
};

typedef char positData;       // posit data unit type

class build {
public:
    build(char* buildPath);
    virtual ~build() {};
public:
    virtual void addDoc(char* buf, int len, int orgId) = 0;
protected:
    int docIndex;
    int fIndex;
    char* path;
protected:
    unsigned gen_key(unsigned v);
    unsigned gen_key2(unsigned x, unsigned y);
private:
    build(const build&);
    build& operator=(const build&);
};

class singleBuild : public build {
public:
    singleBuild(char* buildPath) ;
    ~singleBuild() {};
public:
    virtual void addDoc(char* buf, int len, int orgId);
    void writeFlush();
private:
    void writeIndexData();
    void writePositMeta();
    void writePositData();
private:
    dataPool<indexData> iPool;
    dataPool<positMeta> pMPool;
    dataPool<positData> pDPool;
};

class mutilBuild : public build {
public:
    mutilBuild(char* buildPath);
    ~mutilBuild();
public:
    virtual void addDoc(char* buf, int len, int orgId);
};

}

#endif

