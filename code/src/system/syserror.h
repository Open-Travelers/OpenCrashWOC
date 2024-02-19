#ifndef SYSERROR_H
#define SYSERROR_H

#include "gs.h"
#include "system/gs.h"

// Size: 0xC
struct __va_list_tag
{
    unsigned char gpr; // Offset: 0x0
    unsigned char fpr; // Offset: 0x1
    void* overflow_arg_area; // Offset: 0x4
    void* reg_save_area; // Offset: 0x8
};

char Lockupbuffer1[256];
char Lockupbuffer2[256];
char Lockupbuffer3[256];

#endif // !SYSERROR_H
