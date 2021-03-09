#ifndef NUMATHTYPES_H
#define NUMATHTYPES_H

#include "../types.h"

// 3D vector.
typedef struct
{
	f32 x;
	f32 y;
	f32 z;
} Vec;

// Vector with 4 slots.
typedef struct
{
	f32 x;
	f32 y;
	f32 z;
	f32 w;
} Vec4;

// 4x4 matrix.
typedef struct
{
	f32 m11, m12, m13, m14;
	f32 m21, m22, m23, m24;
	f32 m31, m32, m33, m34;
	f32 m41, m42, m43, m44;
} Mtx;

// Quaternion.
typedef struct
{
	Vec v;
	f32 w;
} Quat;

#endif // !NUMATHTYPES_H