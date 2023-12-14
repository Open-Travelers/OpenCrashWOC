#ifndef NUMATHTYPES_H
#define NUMATHTYPES_H

#include "types.h"
#include "stddef.h"

// Size: 0xC
struct nuangvec_s
{
    int x; // Offset: 0x0
    int y; // Offset: 0x4
    int z; // Offset: 0x8
};

// Size: 0xC // 3D vector.
struct Vec
{
	f32 x;
	f32 y;
	f32 z;
};


struct nuvec_s
{
	f32 x;
	f32 y;
	f32 z;
};

// Vector with 4 slots.
struct nuvec4_s
{
	f32 x;
	f32 y;
	f32 z;
	f32 w;
};

// Size: 0x10
struct nuivec4_s
{
	int x;
	int y;
	int z;
	int w;
};

// Size: 0xC
struct nuivec_s
{
	int x;
	int y;
	int z;
};

// 4x4 matrix.
struct Mtx
{
	f32 m11, m12, m13, m14;
	f32 m21, m22, m23, m24;
	f32 m31, m32, m33, m34;
	f32 m41, m42, m43, m44;
}; // Size: 0x40

// Size: 0x40
struct numtx_s
{
    f32 _00; // Offset: 0x0
    f32 _01; // Offset: 0x4
    f32 _02; // Offset: 0x8
    f32 _03; // Offset: 0xC
    f32 _10; // Offset: 0x10
    f32 _11; // Offset: 0x14
    f32 _12; // Offset: 0x18
    f32 _13; // Offset: 0x1C
    f32 _20; // Offset: 0x20
    f32 _21; // Offset: 0x24
    f32 _22; // Offset: 0x28
    f32 _23; // Offset: 0x2C
    f32 _30; // Offset: 0x30
    f32 _31; // Offset: 0x34
    f32 _32; // Offset: 0x38
    f32 _33; // Offset: 0x3C
};

// Quaternion.
struct Quat
{
	float x;
	float y;
	float z;
	float w;
};

#endif // !NUMATHTYPES_H
