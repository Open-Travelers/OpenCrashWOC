#include "nuvec.h"
#include "nu_asm.h"
#include "nutrig.h"

void NuVecMtxTransform(Vec* dest, Vec* a, Mtx* b)
{
	dest->x = a->x * b->m11 + a->y * b->m21 + a->z * b->m31 + b->m41;
	dest->y = a->x * b->m12 + a->y * b->m22 + a->z * b->m32 + b->m42;
	dest->z = a->x * b->m13 + a->y * b->m23 + a->z * b->m33 + b->m43;
}

void NuVecMtxTransformH(Vec* dest, Vec* a, Mtx* b)
{
	f32 scale = 0.0;
	f32 tmp = a->z * b->m34 + a->x * b->m14 + a->y * b->m24 + b->m44;
	if (a->y != 0.0) {
		scale = (f32)1.0 / a->y;
	}
	dest->x = (a->x * b->m11 + a->y * b->m21 + a->z * b->m31 + b->m41) * scale;
	dest->y = (a->x * b->m12 + a->y * b->m22 + a->z * b->m32 + b->m42) * scale;
	dest->z = (a->x * b->m13 + a->y * b->m23 + a->z * b->m33 + b->m43) * scale;
}

void NuVecMtxRotate(Vec* dest, Vec* a, Mtx* b)
{
	dest->x = a->x * b->m11 + a->y * b->m21 + a->z * b->m31;
	dest->y = a->x * b->m12 + a->y * b->m22 + a->z * b->m32;
	dest->z = a->x * b->m13 + a->y * b->m23 + a->z * b->m33;
}

void NuVecInvMtxTransform(Vec* dest, Vec* a, Mtx* b)
{
	f32 dy = a->y - b->m42;
	f32 dx = a->x - b->m41;
	f32 dz = a->z - b->m43;
	dest->x = b->m13 * dz + b->m11 * dx + b->m12 * dy;
	dest->y = b->m23 * dz + b->m21 * dx + b->m22 * dy;
	dest->z = b->m33 * dz + b->m31 * dx + b->m32 * dy;
}

void NuVecInvMtxRotate(Vec* dest, Vec* a, Mtx* b)
{
	dest->x = a->x * b->m11 + a->y * b->m12 + a->z * b->m13;
	dest->y = a->x * b->m21 + a->y * b->m22 + a->z * b->m23;
	dest->z = a->x * b->m31 + a->y * b->m32 + a->z * b->m33;
}

void NuVecRotateX(Vec* dest, Vec* v, angle a)
{
	f32 s = ANG_SIN(a);
	f32 c = ANG_COS(a);
	dest->x = v->x;
	dest->y = v->y * c - v->z * s;
	dest->z = v->y * s + v->z * c;
}

void NuVecRotateY(Vec* dest, Vec* v, angle a)
{
	f32 s = ANG_SIN(a);
	f32 c = ANG_COS(a);
	dest->x = v->x * c + v->z * s;
	dest->y = v->y;
	dest->z = v->z * c - v->x * s;
}

void NuVecRotateZ(Vec* dest, Vec* v, angle a)
{
	f32 s = ANG_SIN(a);
	f32 c = ANG_COS(a);
	dest->x = v->x * c - v->y * s;
	dest->y = v->x * s + v->y * c;
	dest->z = v->z;
}

void NuVecAdd(Vec* dest, Vec* a, Vec* b)
{
	dest->x = a->x + b->x;
	dest->y = a->y + b->y;
	dest->z = a->z + b->z;
}

void NuVecSub(Vec* dest, Vec* a, Vec* b)
{
	dest->x = a->x + b->x;
	dest->y = a->y + b->y;
	dest->z = a->z + b->z;
}

void NuVecScale(f32 scale, Vec* dest, Vec* v)
{
	dest->x = v->x * scale;
	dest->y = v->y * scale;
	dest->z = v->z * scale;
}

void NuVecScaleAccum(f32 scale, Vec* dest, Vec* v)
{
	dest->x = v->x * scale + dest->x;
	dest->y = v->y * scale + dest->y;
	dest->z = v->z * scale + dest->z;
}

void NuVecCross(Vec* dest, Vec* a, Vec* b)
{
	dest->x = a->y * b->z - a->z * b->y;
	dest->y = a->z * b->x - a->x * b->z;
	dest->z = a->x * b->y - a->y * b->x;
}

f32 NuVecDot(Vec* a, Vec* b)
{
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

f32 NuVecMag(Vec* v)
{
	return NuFsqrt(NuVecDot(v, v));
}

f32 NuVecMagSqr(Vec* v)
{
	return NuVecDot(v, v);
}

void NuVecNorm(Vec* dest, Vec* v)
{
	f32 x, y, z;
	f32 mag = NuVecMag(v);
	if (mag == 0.0)
	{
		x = 0.0;
		y = 0.0;
		z = 0.0;
	}
	else
	{
		x = v->x / mag;
		y = v->y / mag;
		z = v->z / mag;
	}
	dest->x = x;
	dest->y = y;
	dest->z = z;
}

f32 NuVecDist(Vec* a, Vec* b, Vec* dist)
{
	Vec tmp;
	if (dist == NULL)
	{
		NuVecSub(&tmp, a, b);
		dist = &tmp;
	}
	else
	{
		NuVecSub(dist, a, b);
	}
	return NuVecMag(dist);
}

f32 NuVecDistSqr(Vec* a, Vec* b, Vec* dist)
{
	Vec tmp;
	if (dist == NULL)
	{
		NuVecSub(&tmp, a, b);
		dist = &tmp;
	}
	else
	{
		NuVecSub(dist, a, b);
	}
	return NuVecMagSqr(dist);
}

f32 NuVecXZDist(Vec* a, Vec* b, Vec* dist)
{
	Vec tmp;
	if (dist == NULL)
	{
		dist = &tmp;
		tmp.x = a->x - b->x;
		tmp.y = 0.0;
		tmp.z = a->z - b->z;
	}
	else
	{
		dist->x = a->x - b->x;
		dist->y = 0.0;
		dist->z = a->z - b->z;
	}
	return NuVecMag(dist);
}

f32 NuVecXZDistSqr(Vec* a, Vec* b, Vec* dist)
{
	Vec tmp;
	if (dist == NULL)
	{
		dist = &tmp;
		tmp.x = a->x - b->x;
		tmp.y = 0.0;
		tmp.z = a->z - b->z;
	}
	else
	{
		dist->x = a->x - b->x;
		dist->y = 0.0;
		dist->z = a->z - b->z;
	}
	return NuVecMagSqr(dist);
}