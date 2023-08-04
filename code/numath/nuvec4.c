#include "nuvec4.h"
#include "numtx.h"

void NuVec4Scale(f32 scale, struct Vec4* dest, struct Vec4* v)
{
	dest->x = v->x * scale;
	dest->y = v->y * scale;
	dest->z = v->z * scale;
	dest->w = v->w * scale;
}

void NuVec4MtxTransform(struct Vec4* dest, struct Vec4* a, struct Mtx* b)
{
	dest->x = a->x * b->m11 + a->y * b->m21 + a->z * b->m31 + b->m41;
	dest->y = a->x * b->m12 + a->y * b->m22 + a->z * b->m32 + b->m42;
	dest->z = a->x * b->m13 + a->y * b->m23 + a->z * b->m33 + b->m43;
	dest->w = a->x * b->m14 + a->y * b->m24 + a->z * b->m34 + b->m44;
}

void NuVec4MtxInvTransform(struct Vec4* dest, struct Vec4* a, struct Mtx* b)
{
	struct Mtx tmp;
	NuMtxInv(&tmp, b);
	NuVec4MtxTransform(dest, a, &tmp);
}
