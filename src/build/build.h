#ifndef _BUILD_H_
#define _BUILD_H_

namespace ys {

#define MAXLEN (1024*1024)

struct indexData {
    int key;                  // sub finger
    int docId;                // document id
    int offset;               // the offset in origin file
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
    typedef unsigned int uint32_t;
public:
    build(){ docIndex = 0 };
    virtual ~build() {};
public:
    virtual void addDoc(char* buf, int len, int orgId) = 0;
protected:
    int docIndex;
protected:
    uint32_t gen_key(uint32_t v);
    uint32_t gen_key2(uint32_t x, uint32_t y);
private:
    build(const build&);
    build& operator=(const build&);
};

class singleBuild : public build {
public:
    singleBuild() {};
    ~singleBuild() {};
public:
    virtual void addDoc(char* buf, int len, int orgId);
private:
    void writeIndex(dataPool<indexData> iDataPool);
    void writePMeta(dataPool<positMeta> pMetaPool);
    void writePData(dataPool<positData> pDataPool);
};

class mutilBuild : public build {
public:
    mutilBuild();
    ~mutilBuild();
public:
    virtual void addDoc(char* buf, int len, int orgId);
};

}

#endif

