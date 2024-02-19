#ifndef ELFSYSTYPES_H
#define ELFSYSTYPES_H

#include "types.h"


struct __sbuf //0x8
{
    unsigned char* _base;
    int _size;
};


// Size: 0x18
struct _Bigint
{
    struct _Bigint* _next; // Offset: 0x0, DWARF: 0xFD28
    int _k; // Offset: 0x4, DWARF: 0xFD55
    int _maxwds; // Offset: 0x8, DWARF: 0xFD7A
    int _sign; // Offset: 0xC, DWARF: 0xFDA4
    int _wds; // Offset: 0x10, DWARF: 0xFDCC
    unsigned int _x[1]; // Offset: 0x14, DWARF: 0xFDF3
};

// Size: 0x24
struct tm
{
    int tm_sec; // Offset: 0x0, DWARF: 0xF152
    int tm_min; // Offset: 0x4, DWARF: 0xF17B
    int tm_hour; // Offset: 0x8, DWARF: 0xF1A4
    int tm_mday; // Offset: 0xC, DWARF: 0xF1CE
    int tm_mon; // Offset: 0x10, DWARF: 0xF1F8
    int tm_year; // Offset: 0x14, DWARF: 0xF221
    int tm_wday; // Offset: 0x18, DWARF: 0xF24B
    int tm_yday; // Offset: 0x1C, DWARF: 0xF275
    int tm_isdst; // Offset: 0x20, DWARF: 0xF29F
};

// Size: 0xC
struct _glue
{
    struct _glue* _next; // Offset: 0x0, DWARF: 0xF300
    int _niobs; // Offset: 0x4, DWARF: 0xF32D
    struct __sFILE* _iobs; // Offset: 0x8, DWARF: 0xF356
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

// Size: 0x88
struct _atexit
{
    struct _atexit* _next;
    int _ind;
    void(*_fns[32])();
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


// Size: 0x58
struct __sFILE
{
    unsigned char* _p; // Offset: 0x0, DWARF: 0xF3A3
    int _r; // Offset: 0x4, DWARF: 0xF3CB
    int _w; // Offset: 0x8, DWARF: 0xF3F0
    short _flags; // Offset: 0xC, DWARF: 0xF415
    short _file; // Offset: 0xE, DWARF: 0xF43E
    struct __sbuf _bf; // Offset: 0x10, DWARF: 0xF466
    int _lbfsize; // Offset: 0x18, DWARF: 0xF48E
    void* _cookie; // Offset: 0x1C, DWARF: 0xF4B9
    int(*_read)(void*, char*, int); // Offset: 0x20, DWARF: 0xF4E6
    int(*_write)(void*, const char*, int); // Offset: 0x24, DWARF: 0xF513
    int(*_seek)(void*, int, int); // Offset: 0x28, DWARF: 0xF541
    int(*_close)(void*); // Offset: 0x2C, DWARF: 0xF56E
    struct __sbuf _ub; // Offset: 0x30, DWARF: 0xF59C
    unsigned char* _up; // Offset: 0x38, DWARF: 0xF5C4
    int _ur; // Offset: 0x3C, DWARF: 0xF5ED
    unsigned char _ubuf[3]; // Offset: 0x40, DWARF: 0xF613
    unsigned char _nbuf[1]; // Offset: 0x43, DWARF: 0xF63D
    struct __sbuf _lb; // Offset: 0x44, DWARF: 0xF667
    int _blksize; // Offset: 0x4C, DWARF: 0xF68F
    int _offset; // Offset: 0x50, DWARF: 0xF6BA
    struct _reent* _data; // Offset: 0x54, DWARF: 0xF6E4
};

// Size: 0x2EC
struct _reent
{
    int _errno; // Offset: 0x0, DWARF: 0xF8D1
    struct __sFILE* _stdin; // Offset: 0x4, DWARF: 0xF8FA
    struct __sFILE* _stdout; // Offset: 0x8, DWARF: 0xF928
    struct __sFILE* _stderr; // Offset: 0xC, DWARF: 0xF957
    int _inc; // Offset: 0x10, DWARF: 0xF986
    char _emergency[25]; // Offset: 0x14, DWARF: 0xF9AD
    int _current_category; // Offset: 0x30, DWARF: 0xF9DC
    const char* _current_locale; // Offset: 0x34, DWARF: 0xFA10
    int __sdidinit; // Offset: 0x38, DWARF: 0xFA46
    void(*__cleanup)(struct _reent*); // Offset: 0x3C, DWARF: 0xFA73
    struct _Bigint* _result; // Offset: 0x40, DWARF: 0xFAA4
    int _result_k; // Offset: 0x44, DWARF: 0xFAD3
    struct _Bigint* _p5s; // Offset: 0x48, DWARF: 0xFAFF
    struct _Bigint** _freelist; // Offset: 0x4C, DWARF: 0xFB2B
    int _cvtlen; // Offset: 0x50, DWARF: 0xFB5D
    char* _cvtbuf; // Offset: 0x54, DWARF: 0xFB87
    // Size: 0xF0, DWARF: 0xFE1E
    union
    {
        // Size: 0x4C, DWARF: 0xFF8D
        struct
        {
            unsigned int _rand_next; // Offset: 0x0, DWARF: 0xFF9F
            char* _strtok_last; // Offset: 0x4, DWARF: 0xFFCC
            char _asctime_buf[26]; // Offset: 0x8, DWARF: 0xFFFE
            struct tm _localtime_buf; // Offset: 0x24, DWARF: 0x1002F
            int _gamma_signgam; // Offset: 0x48, DWARF: 0x10062
        } _reent; // Offset: 0x0, DWARF: 0xFE30
        // Size: 0xF0, DWARF: 0x10097
        struct
        {
            unsigned char* _nextf[30]; // Offset: 0x0, DWARF: 0x100A9
            unsigned int _nmalloc[30]; // Offset: 0x78, DWARF: 0x100D4
        } _unused; // Offset: 0x0, DWARF: 0xFE5B
    } _new; // Offset: 0x58, DWARF: 0xFBB4
    struct _atexit* _atexit; // Offset: 0x148 //???
    struct _atexit _atexit0; // Offset: 0x14C //???
    void(**_sig_func)(int); // Offset: 0x1D4, DWARF: 0xFC39
    struct _glue __sglue; // Offset: 0x1D8, DWARF: 0xFC6B
    struct __sFILE __sf[3]; // Offset: 0x1E4, DWARF: 0xFC97
};


// Size: 0xC
struct D3DVertexBuffer
{
    unsigned int Common;
    unsigned int Data;
    unsigned int Lock;
};

#endif // !ELFSYSTYPES_H
