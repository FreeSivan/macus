#ifndef _MERGE_H_
#define _MERGE_H_

namespace ys {

struct mergeMeta {
    int key;
    FILE* meta;
    FILE* data;
    bool operator<(const mergeMeta& v) {
        return key < v.key;
    }
};

class merge {
public:
    merge();
    ~merge();
public:
    void mergeTask();
private:
    char* rawPath;
    char* dstPath;
private:
    merge(const& merge);
    merge& operator=(const& merge);
    void mergeIndex();
    void mergePosit();
};

}

#endif


