#pragma once
#include "system.h"

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
