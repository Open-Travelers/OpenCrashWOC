#ifndef ELFSYSTYPES_H
#define ELFSYSTYPES_H

#include "types.h"


struct __sbuf //0x8
{
    unsigned char* _base;
    int _size;
};

struct tm //0x24
{
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
    int tm_yday;
    int tm_isdst;
};


/*
union
{
    // Size: 0x4C
    struct
    {
        unsigned int _rand_next;
        char* _strtok_last;
        char _asctime_buf[26];
        struct tm _localtime_buf;
        int _gamma_signgam;
    } _reent;
    // Size: 0xF0
    struct
    {
        unsigned char* _nextf[30];
        unsigned int _nmalloc[30];
    } _unused;
} _new; //0xF0
*/
// Size: 0x18
struct _Bigint
{
    struct _Bigint* _next;
    int _k;
    int _maxwds;
    int _sign;
    int _wds;
    unsigned int _x[1];
};

// Size: 0x88
struct _atexit
{
    struct _atexit* _next;
    int _ind;
    void(*_fns[32])();
};

// Size: 0xC
struct _glue
{
    struct _glue* _next;
    int _niobs;
    struct __sFILE* _iobs;
};

// Size: 0x10
struct _sceDmaTag
{
    unsigned short qwc;
    unsigned char mark;
    unsigned char id;
    struct _sceDmaTag* next;
    unsigned int p[2];
};

// Size: 0x10
struct giftag
{
    long long NLOOP : 15; // Offset: 0x0, Bit Offset: 0, Bit Size: 15
    long long EOP : 1; // Offset: 0x0, Bit Offset: 15, Bit Size: 1
    long long pad16 : 16; // Offset: 0x0, Bit Offset: 16, Bit Size: 16
    long long id : 14; // Offset: 0x0, Bit Offset: 32, Bit Size: 14
    long long PRE : 1; // Offset: 0x0, Bit Offset: 46, Bit Size: 1
    long long PRIM : 11; // Offset: 0x0, Bit Offset: 47, Bit Size: 11
    long long FLG : 2; // Offset: 0x0, Bit Offset: 58, Bit Size: 2
    long long NREG : 4; // Offset: 0x0, Bit Offset: 60, Bit Size: 4
    long long REGS0 : 4; // Offset: 0x8,  Bit Offset: 0, Bit Size: 4
    long long REGS1 : 4; // Offset: 0x8,  Bit Offset: 4, Bit Size: 4
    long long REGS2 : 4; // Offset: 0x8,  Bit Offset: 8, Bit Size: 4
    long long REGS3 : 4; // Offset: 0x8,  Bit Offset: 12, Bit Size: 4
    long long REGS4 : 4; // Offset: 0x8,  Bit Offset: 16, Bit Size: 4
    long long REGS5 : 4; // Offset: 0x8,  Bit Offset: 20, Bit Size: 4
    long long REGS6 : 4; // Offset: 0x8,  Bit Offset: 24, Bit Size: 4
    long long REGS7 : 4; // Offset: 0x8, Bit Offset: 28, Bit Size: 4
    long long REGS8 : 4; // Offset: 0x8, Bit Offset: 32, Bit Size: 4
    long long REGS9 : 4; // Offset: 0x8, Bit Offset: 36, Bit Size: 4
    long long REGS10 : 4; // Offset: 0x8, Bit Offset: 40, Bit Size: 4
    long long REGS11 : 4; // Offset: 0x8, Bit Offset: 44, Bit Size: 4
    long long REGS12 : 4; // Offset: 0x8, Bit Offset: 48, Bit Size: 4
    long long REGS13 : 4; // Offset: 0x8, Bit Offset: 52, Bit Size: 4
    long long REGS14 : 4; // Offset: 0x8, Bit Offset: 56, Bit Size: 4
    long long REGS15 : 4; // Offset: 0x8, Bit Offset: 60, Bit Size: 4
};

// Size: 0x4
union variptr_u
{
    void* voidptr;
    unsigned char* u8;
    unsigned short* u16;
    short* s16;
    unsigned int* u32;
    long long* u64;
    long long* u128;
    float* f32;
    struct nuvec_s* vec3;
    struct nuvec4_s* vec4;
    struct nuivec_s* ivec3;
    struct nuivec4_s* ivec4;
    struct numtx_s* mtx44;
    unsigned int* viftag;
    unsigned int intaddr;
    struct _sceDmaTag* dmatag;
    struct giftag* gifTag;

};

// Size: 0x8
struct _LARGE_INTEGER_NGC
{
    double QuadPart; // Offset: 0x0
};

// Size: 0x2EC
struct _reent
{
    int _errno;
    struct __sFILE* _stdin;
    struct __sFILE* _stdout;
    struct __sFILE* _stderr;
    int _inc;
    char _emergency[25];
    int _current_category;
    const char* _current_locale;
    int __sdidinit;
    void(*__cleanup)(struct _reent*);
    struct _Bigint* _result;
    int _result_k;
    struct _Bigint* _p5s;
    struct _Bigint** _freelist;
    int _cvtlen;
    char* _cvtbuf;
    //union _new;
    struct _atexit* _atexit;
    struct _atexit _atexit0;
    void(**_sig_func)(int);
    struct _glue __sglue;
    //struct __sFILE __sf[3];
};


// Size: 0x58
struct __sFILE
{
    unsigned char* _p;
    int _r;
    int _w;
    short _flags;
    short _file;
    struct __sbuf _bf;
    int _lbfsize;
    void* _cookie;
    int(*_read)(void*, char*, int);
    int(*_write)(void*, const char*, int);
    int(*_seek)(void*, int, int);
    int(*_close)(void*);
    struct __sbuf _ub;
    unsigned char* _up;
    int _ur;
    unsigned char _ubuf[3];
    unsigned char _nbuf[1];
    struct __sbuf _lb;
    int _blksize;
    int _offset;
    struct _reent* _data;
};


// Size: 0xC
struct D3DVertexBuffer
{
    unsigned int Common;
    unsigned int Data;
    unsigned int Lock;
};

#endif // !ELFSYSTYPES_H
