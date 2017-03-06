#ifndef _BUILD_H_
#define _BUILD_H_

namespace ys {

struct positMeta {
    int docId;
    int orgId;
    long offset;
};

struct indexData {
    int docId;
    int offset;
};

struct indexMeta {
    int offset;
    int length;
};

typedef char positData;

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

