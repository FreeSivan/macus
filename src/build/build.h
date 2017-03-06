#ifndef _BUILD_H_
#define _BUILD_H_

namespace ys {

struct positMeta {
    int docId;                // document id
    int orgId;                // origin id
    long offset;              // the offset in positData
    int length;               // the length of this file
};

struct indexData {
    int docId;                // document id
    int offset;               // the offset in origin file
};

struct indexMeta {
    int offset;               // the offset in indexData
    int length;               // the number of index
};

typedef char positData;       // posit data unit type

class build {
public:
    build();
    virtual ~build();
public:
    virtual void addDoc(char* buf, int len, int orgId) = 0;
protected:
    int docIndex;
private:
    build(const build&);
    build& operator=(const build&);
};

class singleBuild : public build {
public:
    singleBuild();
    ~singleBuild();
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

