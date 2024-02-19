#ifndef TYPES_H
#define TYPES_H

#include "elf_systypes.h"
#include<stdbool.h>

#define NuError(msg) \
    error_func e = NuErrorProlog(__FILE__, __LINE__); \
    e(msg);

typedef int UNKWORD;
typedef void UNKTYPE;

// Unsigned byte.
typedef unsigned char u8;

// Signed byte.
typedef signed char s8;

// Unsigned short.
typedef unsigned short u16;

// Signed short.
typedef signed short s16;

// Unsigned word.
typedef unsigned int u32;

// Signed word.
typedef signed int s32;

// Unsigned  double word.
typedef unsigned long long u64;

// Signed double word.
typedef signed long long s64;

// 32-bit float.
typedef float f32;

// 64-bit float.
typedef double f64;

// Angle from [0x0, 0x10000). 0x0 represents 0 where 0x10000 is 2pi, so an increment is 2pi/0x10000.
typedef unsigned short angle;

// Fixed point value.
typedef signed int fxi;

// Error function.
typedef void error_func(char *msg,...);

// File handle.
typedef s32 fileHandle;

unsigned long long LBIT;

typedef volatile u16 vu16;

union variptr_u superbuffer_ptr;
union variptr_u superbuffer_base;
union variptr_u superbuffer_end;


// Size: 0x4
struct nunrand_s
{
    long idum; // Offset: 0x0
};

// Size: 0xC
struct nucolour3_s
{
	f32 r;
	f32 g;
	f32 b;
};

// Size: 0x18
struct _XINPUT_STATE
{
    unsigned int dwPacketNumber; // Offset: 0x0
};

// Size: 0x44
struct _XINPUT_FEEDBACK_HEADER
{
    unsigned int dwStatus; // Offset: 0x0
    void* hEvent; // Offset: 0x4
    unsigned char Reserved[58]; // Offset: 0x8
};

// Size: 0x48
struct _XINPUT_FEEDBACK
{
    struct _XINPUT_FEEDBACK_HEADER Header; // Offset: 0x0
};

// Size: 0xF4
struct nupad_s
{
    void* padhandle; // Offset: 0x0
    struct _XINPUT_STATE xinputs; // Offset: 0x4
    struct _XINPUT_STATE old_xinputs; // Offset: 0x1C
    struct _XINPUT_FEEDBACK feedback; // Offset: 0x34
    s32 port; // Offset: 0x7C
    s32 slot; // Offset: 0x80
    s32 stage; // Offset: 0x84
    s32 id; // Offset: 0x88
    s32 ldx; // Offset: 0x8C
    s32 ldy; // Offset: 0x90
    s32 rdx; // Offset: 0x94
    s32 rdy; // Offset: 0x98
    float l_nx; // Offset: 0x9C
    float l_ny; // Offset: 0xA0
    float l_rx; // Offset: 0xA4
    float l_ry; // Offset: 0xA8
    float l_dx; // Offset: 0xAC
    float l_dy; // Offset: 0xB0
    float r_nx; // Offset: 0xB4
    float r_ny; // Offset: 0xB8
    float r_rx; // Offset: 0xBC
    float r_ry; // Offset: 0xC0
    float r_dx; // Offset: 0xC4
    float r_dy; // Offset: 0xC8
    unsigned int paddata; // Offset: 0xCC
    unsigned int oldpaddata; // Offset: 0xD0
    unsigned int paddata_db; // Offset: 0xD4
    int analogue_sticks : 1; // Offset: 0xD8, Bit Offset: 0, Bit Size: 1
    int analogue_buttons : 1; // Offset: 0xD8, Bit Offset: 1, Bit Size: 1
    int valid : 1; // Offset: 0xD8, Bit Offset: 2, Bit Size: 1
    int deadzone : 1; // Offset: 0xD8, Bit Offset: 3, Bit Size: 1
    unsigned int padvalue; // Offset: 0xDC
    unsigned char ok; // Offset: 0xE0
    unsigned char read_id; // Offset: 0xE1
    unsigned char buttons_hi; // Offset: 0xE2
    unsigned char buttons_lo; // Offset: 0xE3
    unsigned char r_alg_x; // Offset: 0xE4
    unsigned char r_alg_y; // Offset: 0xE5
    unsigned char l_alg_x; // Offset: 0xE6
    unsigned char l_alg_y; // Offset: 0xE7
    unsigned char l_algpad_r; // Offset: 0xE8
    unsigned char l_algpad_l; // Offset: 0xE9
    unsigned char l_algpad_u; // Offset: 0xEA
    unsigned char l_algpad_d; // Offset: 0xEB
    unsigned char r_algpad_u; // Offset: 0xEC
    unsigned char r_algpad_r; // Offset: 0xED
    unsigned char r_algpad_d; // Offset: 0xEE
    unsigned char r_algpad_l; // Offset: 0xEF
    unsigned char l1_alg; // Offset: 0xF0
    unsigned char r1_alg; // Offset: 0xF1
    unsigned char l2_alg; // Offset: 0xF2
    unsigned char r2_alg; // Offset: 0xF3
};

struct nupad_s* Pad[2];
s32 nuvideo_global_vbcnt;

#include "numath/numathtypes.h"
#endif // !TYPES_H
