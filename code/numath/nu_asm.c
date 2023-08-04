#include "nu_asm.h"

f32 NuFsqrt(f32 f)
{
	return (f32)sqrt(f);
}


f32 NuFDiv(f32 dividend, f32 divisor)
{
	if (divisor != 0.0 && dividend != 0.0) {
		return dividend / divisor;
	}
	return 0.0;
}

void NuVec4MtxTransformVU0(struct Vec4* dest, struct Vec4* a, struct Mtx* b)
{
	dest->x = a->x * b->m11 + a->y * b->m21 + a->z * b->m31 + a->w * b->m41;
	dest->y = a->x * b->m12 + a->y * b->m22 + a->z * b->m32 + a->w * b->m42;
	dest->z = a->x * b->m13 + a->y * b->m23 + a->z * b->m33 + a->w * b->m43;
	dest->w = a->x * b->m14 + a->y * b->m24 + a->z * b->m34 + a->w * b->m44;
}
