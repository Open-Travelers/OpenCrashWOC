#pragma once

// Size: 0x20
struct DVDDiskID
{
    char gameName[4]; // Offset: 0x0, DWARF: 0xC18836
    char company[2]; // Offset: 0x4, DWARF: 0xC18863
    unsigned char diskNumber; // Offset: 0x6, DWARF: 0xC1888F
    unsigned char gameVersion; // Offset: 0x7, DWARF: 0xC188BC
    unsigned char streaming; // Offset: 0x8, DWARF: 0xC188EA
    unsigned char streamingBufSize; // Offset: 0x9, DWARF: 0xC18916
    unsigned char padding[22]; // Offset: 0xA, DWARF: 0xC18949
};

// Size: 0x30
struct DVDCommandBlock
{
    struct DVDCommandBlock* next; // Offset: 0x0, DWARF: 0xC18431
    struct DVDCommandBlock* prev; // Offset: 0x4, DWARF: 0xC1845D
    unsigned int command; // Offset: 0x8, DWARF: 0xC18489
    int state; // Offset: 0xC, DWARF: 0xC184B3
    unsigned int offset; // Offset: 0x10, DWARF: 0xC184DB
    unsigned int length; // Offset: 0x14, DWARF: 0xC18504
    void* addr; // Offset: 0x18, DWARF: 0xC1852D
    unsigned int currTransferSize; // Offset: 0x1C, DWARF: 0xC18557
    unsigned int transferredSize; // Offset: 0x20, DWARF: 0xC1858A
    struct DVDDiskID* id; // Offset: 0x24, DWARF: 0xC185BC
    void(*callback)(int, struct DVDCommandBlock*); // Offset: 0x28, DWARF: 0xC185E6
    void* userData; // Offset: 0x2C, DWARF: 0xC18616
};

// Size: 0x3C
struct DVDFileInfo
{
    struct DVDCommandBlock cb; // Offset: 0x0, DWARF: 0xC1813F
    unsigned int startAddr; // Offset: 0x30, DWARF: 0xC18166
    unsigned int length; // Offset: 0x34, DWARF: 0xC18192
    void(*callback)(int, struct DVDFileInfo*); // Offset: 0x38, DWARF: 0xC181BB
};

// Size: 0x8
struct __sbuf
{
    unsigned char* _base; // Offset: 0x0, DWARF: 0x470FA
    int _size; // Offset: 0x4, DWARF: 0x47125
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
     //_atexit* _atexit; // Offset: 0x148 //???
    //_atexit _atexit0; // Offset: 0x14C //???
    void(**_sig_func)(int); // Offset: 0x1D4, DWARF: 0xFC39
    struct _glue __sglue; // Offset: 0x1D8, DWARF: 0xFC6B
    struct __sFILE __sf[3]; // Offset: 0x1E4, DWARF: 0xFC97
};

struct DVDFileInfo MAHFiles[20];
struct DVDFileInfo MAHFile; 
u32 filelength;
u32 fileoffset;
u32 seekoffset;
s32 FSStart;
u32 amread;
u32 currentfd;
char namesave[128];
char temp[64];
