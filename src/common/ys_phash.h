#ifndef _PERFACT_HASH_H_
#define _PERFACT_HASH_H_

namespace ys {

#define T_LENGTH (1 << 10)
#define S_LENGTH (1 << 25)
#define F_LENGTH ((unsigned int)1 << 31)

class phash {
public:
    typedef unsigned int uint32;
    typedef unsigned short uint16;
    typedef unsigned long uint64;
public:
    phash();
    ~phash();
public:
    void addOver();
    long getpHash(uint32 key);
    void addphKey(uint32 key, int length);
private:
    unsigned short *first;
    unsigned int *second;
    unsigned long *third;
    unsigned short foffset;
    unsigned int soffset;
    unsigned long toffset;
    unsigned int lastKey;
private:
    phash(const phash&);
    phash& operator=(const phash&);
    void generate(uint32 key);
};

}

#endif

