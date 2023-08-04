#include <nu3dx/nucamera.h>
#include "numtx.h"
#include "nu_asm.h"
#include "nutrig.h"
#include "nuvec.h"
#include "nufloat.h"

struct Mtx mident = {
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0
};

struct Mtx gm = {
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0
};

void NuMtxSetIdentity(struct Mtx* m)
{
	m->m11 = mident.m11;
	m->m12 = mident.m12;
	m->m13 = mident.m13;
	m->m14 = mident.m14;
	m->m21 = mident.m21;
	m->m22 = mident.m22;
	m->m23 = mident.m23;
	m->m24 = mident.m24;
	m->m31 = mident.m31;
	m->m32 = mident.m32;
	m->m33 = mident.m33;
	m->m34 = mident.m34;
	m->m41 = mident.m41;
	m->m42 = mident.m42;
	m->m43 = mident.m43;
	m->m44 = mident.m44;
}

void NuMtxSetTranslation(struct Mtx* m, struct Vec* v)
{
	m->m11 = 1.0;
	m->m12 = 0.0;
	m->m13 = 0.0;
	m->m14 = 0.0;
	m->m21 = 0.0;
	m->m22 = 1.0;
	m->m23 = 0.0;
	m->m24 = 0.0;
	m->m31 = 0.0;
	m->m32 = 0.0;
	m->m33 = 1.0;
	m->m34 = 0.0;
	m->m41 = v->x;
	m->m42 = v->y;
	m->m43 = v->z;
	m->m44 = 1.0;
}

void NuMtxSetScale(struct Mtx* m, struct Vec* v)
{
	m->m11 = v->x;
	m->m12 = 0.0;
	m->m13 = 0.0;
	m->m14 = 0.0;
	m->m21 = 0.0;
	m->m22 = v->y;
	m->m23 = 0.0;
	m->m24 = 0.0;
	m->m31 = 0.0;
	m->m32 = 0.0;
	m->m33 = v->z;
	m->m34 = 0.0;
	m->m41 = 0.0;
	m->m42 = 0.0;
	m->m43 = 0.0;
	m->m44 = 1.0;
}

void NuMtxSetRotationX(struct Mtx* m, s32 angle)
{
	f32 s = ANG_SIN(angle);
	f32 c = ANG_COS(angle);
	m->m11 = 1.0;
	m->m12 = 0.0;
	m->m13 = 0.0;
	m->m14 = 0.0;
	m->m21 = 0.0;
	m->m22 = c;
	m->m23 = s;
	m->m24 = 0.0;
	m->m31 = 0.0;
	m->m32 = -s;
	m->m33 = c;
	m->m34 = 0.0;
	m->m41 = 0.0;
	m->m42 = 0.0;
	m->m43 = 0.0;
	m->m44 = 1.0;
}

void NuMtxSetRotationY(struct Mtx* m, s32 angle)
{
	f32 s = ANG_SIN(angle);
	f32 c = ANG_COS(angle);
	m->m11 = c;
	m->m12 = 0.0;
	m->m13 = -s;
	m->m14 = 0.0;
	m->m21 = 0.0;
	m->m22 = 1.0;
	m->m23 = 0.0;
	m->m24 = 0.0;
	m->m31 = s;
	m->m32 = 0.0;
	m->m33 = c;
	m->m34 = 0.0;
	m->m41 = 0.0;
	m->m42 = 0.0;
	m->m43 = 0.0;
	m->m44 = 1.0;
}

void NuMtxSetRotationZ(struct Mtx* m, s32 a)
{
	f32 s = ANG_SIN(a);
	f32 c = ANG_COS(a);
	m->m11 = c;
	m->m12 = s;
	m->m13 = 0.0;
	m->m14 = 0.0;
	m->m21 = -s;
	m->m22 = c;
	m->m23 = 0.0;
	m->m24 = 0.0;
	m->m31 = 0.0;
	m->m32 = 0.0;
	m->m33 = 1.0;
	m->m34 = 0.0;
	m->m41 = 0.0;
	m->m42 = 0.0;
	m->m43 = 0.0;
	m->m44 = 1.0;
}

void NuMtxTranslate(struct Mtx* m, struct Vec* v)
{
	m->m41 = m->m41 + v->x;
	m->m42 = m->m42 + v->y;
	m->m43 = m->m43 + v->z;
}

void NuMtxPreTranslate(struct Mtx* m, struct Vec* v)
{
	m->m41 = v->x * m->m11 + v->y * m->m21 + m->m41 + v->z * m->m31;
	m->m42 = v->x * m->m12 + v->y * m->m22 + m->m42 + v->z * m->m32;
	m->m43 = v->x * m->m13 + v->y * m->m23 + m->m43 + v->z * m->m33;
}

void NuMtxScale(struct Mtx* m, struct Vec* v)
{
	m->m11 = m->m11 * v->x;
	m->m12 = m->m12 * v->y;
	m->m13 = m->m13 * v->z;
	m->m21 = m->m21 * v->x;
	m->m22 = m->m22 * v->y;
	m->m23 = m->m23 * v->z;
	m->m31 = m->m31 * v->x;
	m->m32 = m->m32 * v->y;
	m->m33 = m->m33 * v->z;
	m->m41 = m->m41 * v->x;
	m->m42 = m->m42 * v->y;
	m->m43 = m->m43 * v->z;
}

void NuMtxGetScale(struct Vec* dest, struct Mtx* m)
{
	dest->x = NuFsqrt(m->m11 * m->m11 + m->m12 * m->m12 + m->m13 * m->m13);
	dest->y = NuFsqrt(m->m21 * m->m21 + m->m22 * m->m22 + m->m23 * m->m23);
	dest->z = NuFsqrt(m->m31 * m->m31 + m->m32 * m->m32 + m->m33 * m->m33);
}

void NuMtxPreScale(struct Mtx* m, struct Vec* v)
{
	m->m11 = m->m11 * v->x;
	m->m12 = m->m12 * v->x;
	m->m13 = m->m13 * v->x;
	m->m21 = m->m21 * v->y;
	m->m22 = m->m22 * v->y;
	m->m23 = m->m23 * v->y;
	m->m31 = m->m31 * v->z;
	m->m32 = m->m32 * v->z;
	m->m33 = m->m33 * v->z;
}

void NuMtxRotateX(struct Mtx* m, s32 a)
{
	f32 m13 = m->m13;
	f32 m23 = m->m23;
	f32 m33 = m->m33;
	f32 m43 = m->m43;
	f32 s = ANG_SIN(a);
	f32 c = ANG_COS(a);
	m->m13 = m->m12 * s + m13 * c;
	m->m43 = m->m42 * s + m43 * c;
	m->m23 = m->m22 * s + m23 * c;
	m->m33 = m->m32 * s + m33 * c;
	m->m12 = m->m12 * c - m13 * s;
	m->m22 = m->m22 * c - m23 * s;
	m->m32 = m->m32 * c - m33 * s;
	m->m42 = m->m42 * c - m43 * s;
}

void NuMtxPreRotateX(struct Mtx* m, s32 a)
{
	f32 m31 = m->m31;
	f32 m32 = m->m32;
	f32 m33 = m->m33;
	f32 s = ANG_SIN(a);
	f32 c = ANG_COS(a);
	m->m33 = c * m33 - s * m->m23;
	m->m31 = c * m31 - s * m->m21;
	m->m32 = c * m32 - s * m->m22;
	m->m21 = c * m->m21 + s * m31;
	m->m22 = c * m->m22 + s * m32;
	m->m23 = c * m->m23 + s * m33;
}

void NuMtxRotateY(struct Mtx* m, s32 a)
{
	f32 m13 = m->m13;
	f32 m23 = m->m23;
	f32 m33 = m->m33;
	f32 m43 = m->m43;
	f32 s = ANG_SIN(a);
	f32 c = ANG_COS(a);
	m->m43 = m43 * c - m->m41 * s;
	m->m13 = m13 * c - m->m11 * s;
	m->m23 = m23 * c - m->m21 * s;
	m->m33 = m33 * c - m->m31 * s;
	m->m11 = m->m11 * c + m13 * s;
	m->m21 = m->m21 * c + m23 * s;
	m->m31 = m->m31 * c + m33 * s;
	m->m41 = m->m41 * c + m43 * s;
}

void NuMtxPreRotateY(struct Mtx* m, s32 a)
{
	f32 m31 = m->m31;
	f32 m32 = m->m32;
	f32 m33 = m->m33;
	f32 s = ANG_SIN(a);
	f32 c = ANG_COS(a);
	m->m31 = s * m->m11 + c * m31;
	m->m33 = s * m->m13 + c * m33;
	m->m32 = s * m->m12 + c * m32;
	m->m11 = c * m->m11 - s * m31;
	m->m12 = c * m->m12 - s * m32;
	m->m13 = c * m->m13 - s * m33;
}

void NuMtxRotateZ(struct Mtx* m, s32 a)
{
	f32 m12 = m->m12;
	f32 m22 = m->m22;
	f32 m32 = m->m32;
	f32 m42 = m->m42;
	f32 s = ANG_SIN(a);
	f32 c = ANG_COS(a);
	m->m12 = m->m11 * s + m12 * c;
	m->m42 = m->m41 * s + m42 * c;
	m->m22 = m->m21 * s + m22 * c;
	m->m32 = m->m31 * s + m32 * c;
	m->m11 = m->m11 * c - m12 * s;
	m->m21 = m->m21 * c - m22 * s;
	m->m31 = m->m31 * c - m32 * s;
	m->m41 = m->m41 * c - m42 * s;
}

void NuMtxPreRotateZ(struct Mtx* m, s32 a)
{
	f32 m21 = m->m21;
	f32 m22 = m->m22;
	f32 m23 = m->m23;
	f32 s = ANG_SIN(a);
	f32 c = ANG_COS(a);
	m->m23 = c * m23 - s * m->m13;
	m->m21 = c * m21 - s * m->m11;
	m->m22 = c * m22 - s * m->m12;
	m->m11 = c * m->m11 + s * m21;
	m->m12 = c * m->m12 + s * m22;
	m->m13 = c * m->m13 + s * m23;
}

void NuMtxSetRotateXYZ(struct Mtx* m, s32 a[3])
{
	f32 sx = ANG_SIN(a[0]);
	f32 cx = ANG_COS(a[0]);
	f32 sy = ANG_SIN(a[1]);
	f32 cy = ANG_COS(a[1]);
	f32 sz = ANG_SIN(a[2]);
	f32 cz = ANG_COS(a[2]);
	m->m11 = cy * cz;
	m->m12 = cy * sz;
	m->m13 = -sy;
	m->m14 = 0.00000000;
	m->m21 = sx * sy * cz - cx * sz;
	m->m22 = sx * sy * sz + cx * cz;
	m->m23 = sx * cy;
	m->m24 = 0.00000000;
	m->m31 = cx * sy * cz + sx * sz;
	m->m32 = cx * sy * sz - sx * cz;
	m->m33 = cx * cy;
	m->m34 = 0.00000000;
	m->m41 = 0.00000000;
	m->m42 = 0.00000000;
	m->m43 = 0.00000000;
	m->m44 = 1.00000000;
}

void NuMtxMul(struct Mtx* dest, struct Mtx* a, struct Mtx* b)
{
	gm.m11 = a->m13 * b->m31 + a->m11 * b->m11 + a->m12 * b->m21;
	gm.m12 = a->m13 * b->m32 + a->m11 * b->m12 + a->m12 * b->m22;
	gm.m13 = a->m13 * b->m33 + a->m11 * b->m13 + a->m12 * b->m23;
	gm.m14 = 0.00000000;
	gm.m21 = a->m23 * b->m31 + a->m21 * b->m11 + a->m22 * b->m21;
	gm.m22 = a->m23 * b->m32 + a->m21 * b->m12 + a->m22 * b->m22;
	gm.m23 = a->m23 * b->m33 + a->m21 * b->m13 + a->m22 * b->m23;
	gm.m24 = 0.00000000;
	gm.m31 = a->m33 * b->m31 + a->m31 * b->m11 + a->m32 * b->m21;
	gm.m32 = a->m33 * b->m32 + a->m31 * b->m12 + a->m32 * b->m22;
	gm.m33 = a->m33 * b->m33 + a->m31 * b->m13 + a->m32 * b->m23;
	gm.m34 = 0.00000000;
	gm.m41 = a->m43 * b->m31 + a->m41 * b->m11 + a->m42 * b->m21 + b->m41;
	gm.m42 = a->m43 * b->m32 + a->m41 * b->m12 + a->m42 * b->m22 + b->m42;
	gm.m43 = a->m43 * b->m33 + a->m41 * b->m13 + a->m42 * b->m23 + b->m43;
	gm.m44 = 1.00000000;
	dest->m11 = gm.m11;
	dest->m12 = gm.m12;
	dest->m13 = gm.m13;
	dest->m14 = gm.m14;
	dest->m21 = gm.m21;
	dest->m22 = gm.m22;
	dest->m23 = gm.m23;
	dest->m24 = gm.m24;
	dest->m31 = gm.m31;
	dest->m32 = gm.m32;
	dest->m33 = gm.m33;
	dest->m34 = gm.m34;
	dest->m41 = gm.m41;
	dest->m42 = gm.m42;
	dest->m43 = gm.m43;
	dest->m44 = gm.m44;
}

void NuMtxMulH(struct Mtx* dest, struct Mtx* a, struct Mtx* b)
{
	gm.m11 = a->m14 * b->m41 + a->m13 * b->m31 + a->m11 * b->m11 + a->m12 * b->m21;
	gm.m12 = a->m14 * b->m42 + a->m13 * b->m32 + a->m11 * b->m12 + a->m12 * b->m22;
	gm.m13 = a->m14 * b->m43 + a->m13 * b->m33 + a->m11 * b->m13 + a->m12 * b->m23;
	gm.m14 = a->m14 * b->m44 + a->m13 * b->m34 + a->m11 * b->m14 + a->m12 * b->m24;
	gm.m21 = a->m24 * b->m41 + a->m23 * b->m31 + a->m21 * b->m11 + a->m22 * b->m21;
	gm.m22 = a->m24 * b->m42 + a->m23 * b->m32 + a->m21 * b->m12 + a->m22 * b->m22;
	gm.m23 = a->m24 * b->m43 + a->m23 * b->m33 + a->m21 * b->m13 + a->m22 * b->m23;
	gm.m24 = a->m24 * b->m44 + a->m23 * b->m34 + a->m21 * b->m14 + a->m22 * b->m24;
	gm.m31 = a->m34 * b->m41 + a->m33 * b->m31 + a->m31 * b->m11 + a->m32 * b->m21;
	gm.m32 = a->m34 * b->m42 + a->m33 * b->m32 + a->m31 * b->m12 + a->m32 * b->m22;
	gm.m33 = a->m34 * b->m43 + a->m33 * b->m33 + a->m31 * b->m13 + a->m32 * b->m23;
	gm.m34 = a->m34 * b->m44 + a->m33 * b->m34 + a->m31 * b->m14 + a->m32 * b->m24;
	gm.m41 = a->m44 * b->m41 + a->m43 * b->m31 + a->m41 * b->m11 + a->m42 * b->m21;
	gm.m42 = a->m44 * b->m42 + a->m43 * b->m32 + a->m41 * b->m12 + a->m42 * b->m22;
	gm.m43 = a->m44 * b->m43 + a->m43 * b->m33 + a->m41 * b->m13 + a->m42 * b->m23;
	gm.m44 = a->m44 * b->m44 + a->m43 * b->m34 + a->m41 * b->m14 + a->m42 * b->m24;
	dest->m11 = gm.m11;
	dest->m12 = gm.m12;
	dest->m13 = gm.m13;
	dest->m14 = gm.m14;
	dest->m21 = gm.m21;
	dest->m22 = gm.m22;
	dest->m23 = gm.m23;
	dest->m24 = gm.m24;
	dest->m31 = gm.m31;
	dest->m32 = gm.m32;
	dest->m33 = gm.m33;
	dest->m34 = gm.m34;
	dest->m41 = gm.m41;
	dest->m42 = gm.m42;
	dest->m43 = gm.m43;
	dest->m44 = gm.m44;
}

void NuMtxMulR(struct Mtx* dest, struct Mtx* a, struct Mtx* b)
{
	gm.m11 = a->m13 * b->m31 + a->m11 * b->m11 + a->m12 * b->m21;
	gm.m12 = a->m13 * b->m32 + a->m11 * b->m12 + a->m12 * b->m22;
	gm.m13 = a->m13 * b->m33 + a->m11 * b->m13 + a->m12 * b->m23;
	gm.m14 = 0.00000000;
	gm.m21 = a->m23 * b->m31 + a->m21 * b->m11 + a->m22 * b->m21;
	gm.m22 = a->m23 * b->m32 + a->m21 * b->m12 + a->m22 * b->m22;
	gm.m23 = a->m23 * b->m33 + a->m21 * b->m13 + a->m22 * b->m23;
	gm.m24 = 0.00000000;
	gm.m31 = a->m33 * b->m31 + a->m31 * b->m11 + a->m32 * b->m21;
	gm.m32 = a->m33 * b->m32 + a->m31 * b->m12 + a->m32 * b->m22;
	gm.m33 = a->m33 * b->m33 + a->m31 * b->m13 + a->m32 * b->m23;
	gm.m34 = 0.00000000;
	gm.m41 = 0.00000000;
	gm.m43 = 0.00000000;
	gm.m42 = 0.00000000;
	gm.m44 = 1.00000000;
	dest->m11 = gm.m11;
	dest->m12 = gm.m12;
	dest->m13 = gm.m13;
	dest->m14 = gm.m14;
	dest->m21 = gm.m21;
	dest->m22 = gm.m22;
	dest->m23 = gm.m23;
	dest->m24 = gm.m24;
	dest->m31 = gm.m31;
	dest->m32 = gm.m32;
	dest->m33 = gm.m33;
	dest->m34 = gm.m34;
	dest->m41 = gm.m41;
	dest->m42 = gm.m42;
	dest->m43 = gm.m43;
	dest->m44 = gm.m44;
}

void NuMtxTranspose(struct Mtx* dest, struct Mtx* m)
{
	f32 tmp = m->m21;
	dest->m21 = m->m12;
	dest->m12 = tmp;
	tmp = m->m31;
	dest->m31 = m->m13;
	dest->m13 = tmp;
	tmp = m->m41;
	dest->m41 = m->m14;
	dest->m14 = tmp;
	tmp = m->m32;
	dest->m32 = m->m23;
	dest->m23 = tmp;
	tmp = m->m42;
	dest->m42 = m->m24;
	dest->m24 = tmp;
	tmp = m->m34;
	dest->m34 = m->m43;
	dest->m43 = tmp;
	dest->m11 = m->m11;
	dest->m22 = m->m22;
	dest->m33 = m->m33;
	dest->m44 = m->m44;
}

void NuMtxInv(struct Mtx* dest, struct Mtx* m)
{
	f32 m12 = m->m12;
	f32 m21 = m->m21;
	f32 m42 = m->m42;
	f32 m43 = m->m43;
	f32 m41 = -m->m41;
	dest->m21 = m12;
	m42 = -m42;
	dest->m12 = m21;
	m43 = -m43;
	f32 m13 = m->m13;
	f32 m31 = m->m31;
	dest->m31 = m13;
	dest->m13 = m31;
	f32 m32 = m->m32;
	f32 m23 = m->m23;
	dest->m23 = m32;
	dest->m32 = m23;
	f32 m11 = m->m11;
	dest->m11 = m11;
	f32 m22 = m->m22;
	dest->m22 = m22;
	f32 m33 = m->m33;
	dest->m14 = 0.00000000;
	dest->m24 = 0.00000000;
	dest->m33 = m33;
	dest->m34 = 0.00000000;
	dest->m41 = m43 * m13 + m41 * m11 + m42 * m12;
	dest->m42 = m43 * m23 + m41 * m21 + m42 * m22;
	dest->m43 = m43 * m33 + m41 * m31 + m42 * m32;
	dest->m44 = 1.00000000;
}

void NuMtxInvR(struct Mtx* dest, struct Mtx* m)
{
	f32 tmp = m->m21;
	dest->m21 = m->m12;
	dest->m12 = tmp;
	tmp = m->m31;
	dest->m31 = m->m13;
	dest->m13 = tmp;
	tmp = m->m23;
	dest->m23 = m->m32;
	dest->m32 = tmp;
	dest->m11 = m->m11;
	dest->m22 = m->m22;
	tmp = m->m33;
	dest->m14 = 0.00000000;
	dest->m24 = 0.00000000;
	dest->m33 = tmp;
	dest->m34 = 0.00000000;
	dest->m41 = 0.00000000;
	dest->m42 = 0.00000000;
	dest->m43 = 0.00000000;
	dest->m44 = 1.00000000;
}

void NuMtxInvRSS(struct Mtx* dest, struct Mtx* m)
{
	f32 diff = m->m22 * m->m33 - m->m23 * m->m32;
	f32 scale = 1.0 / (m->m13 * (m->m21 * m->m32 - m->m22 * m->m31) + (m->m11 * diff - m->m12 * (m->m21 * m->m33 - m->m23 * m->m31)));
	dest->m11 = diff * scale;
	diff = -scale;
	dest->m21 = (m->m21 * m->m33 - m->m23 * m->m31) * diff;
	dest->m31 = (m->m21 * m->m32 - m->m22 * m->m31) * scale;
	dest->m12 = (m->m12 * m->m33 - m->m13 * m->m32) * diff;
	dest->m22 = (m->m11 * m->m33 - m->m13 * m->m31) * scale;
	dest->m32 = (m->m11 * m->m32 - m->m12 * m->m31) * diff;
	dest->m13 = (m->m12 * m->m23 - m->m13 * m->m22) * scale;
	dest->m23 = (m->m11 * m->m23 - m->m13 * m->m21) * diff;
	diff = m->m21;
	f32 m12 = m->m12;
	f32 m11 = m->m11;
	f32 m22 = m->m22;
	dest->m43 = 0.0;
	dest->m44 = 1.0;
	dest->m14 = 0.0;
	dest->m33 = (m11 * m22 - m12 * diff) * scale;
	dest->m24 = 0.0;
	dest->m34 = 0.0;
	dest->m41 = 0.0;
	dest->m42 = 0.0;
}

// TODO: Clean this function up
void NuMtxAlignZ(struct Mtx* dest, struct Mtx* m)
{
	float tmp1;
	float tmp2;
	float *tmp3;
	float *tmp4;
	double tmp5;
	double tmp6;
	double tmp7;

    tmp2 = m->m13 * m->m13 + m->m11 * m->m11 + m->m12 * m->m12;
    tmp1 = 0.0;
    tmp6 = dest->m13 * dest->m13 + dest->m11 * dest->m11 + dest->m12 * dest->m12;
    tmp7 = dest->m23 * dest->m23 + dest->m21 * dest->m21 + dest->m22 * dest->m22;
	if (tmp2 != 0.0)
	{
        tmp5 = sqrt((dest->m33 * dest->m33 + dest->m31 * dest->m31 + dest->m32 * dest->m32) / tmp2);
        tmp1 = tmp5;
	}
    tmp3 = &dest->m21;
    tmp4 = &dest->m31;
	dest->m31 = tmp1 * m->m11;
	dest->m32 = tmp1 * m->m12;
	dest->m33 = tmp1 * m->m13;
	NuVecDot(tmp3, tmp4);
    tmp5 = NuFabs(tmp5);
	if (tmp5 <= 0.8660253882408142)
	{
		NuVecCross(dest, tmp3, tmp4);
        tmp1 = 0.0;
        tmp5 = dest->m13 * dest->m13 + dest->m11 * dest->m11 + dest->m12 * dest->m12;
		if (tmp5 != 0.0)
		{
            tmp6 = sqrt(tmp6 / tmp5);
            tmp1 = tmp6;
		}
		dest->m11 = dest->m11 * tmp1;
		dest->m12 = dest->m12 * tmp1;
		dest->m13 = dest->m13 * tmp1;
		NuVecCross(tmp3, tmp4, dest);
        tmp1 = 0.0;
        tmp6 = dest->m23 * dest->m23 + dest->m21 * dest->m21 + dest->m22 * dest->m22;
		if (tmp6 != 0.0)
		{
            tmp6 = sqrt(tmp7 / tmp6);
            tmp1 = tmp6;
		}
		dest->m21 = dest->m21 * tmp1;
		dest->m22 = dest->m22 * tmp1;
		dest->m23 = dest->m23 * tmp1;
	}
	else
	{
		NuVecCross(tmp3, tmp4, dest);
        tmp1 = 0.0;
        tmp5 = dest->m23 * dest->m23 + dest->m21 * dest->m21 + dest->m22 * dest->m22;
		if (tmp5 != 0.0)
		{
            tmp7 = sqrt(tmp7 / tmp5);
            tmp1 = tmp7;
		}
		dest->m21 = dest->m21 * tmp1;
		dest->m22 = dest->m22 * tmp1;
		dest->m23 = dest->m23 * tmp1;
		NuVecCross(dest, tmp3, tmp4);
        tmp1 = 0.0;
        tmp7 = dest->m13 * dest->m13 + dest->m11 * dest->m11 + dest->m12 * dest->m12;
		if (tmp7 != 0.0)
		{
            tmp6 = sqrt(tmp6 / tmp7);
            tmp1 = tmp6;
		}
		dest->m11 = dest->m11 * tmp1;
		dest->m12 = dest->m12 * tmp1;
		dest->m13 = dest->m13 * tmp1;
	}
}

void NuMtxLookAtZ(struct Mtx* dest, struct Vec* v)
{
	struct Vec tmp;
	tmp.x = v->x - dest->m41;
	tmp.y = v->y - dest->m42;
	tmp.z = v->z - dest->m43;
	NuVecNorm(&tmp, &tmp);
	NuMtxAlignZ(dest, &tmp);
}

void NuMtxAddR(struct Mtx* dest, struct Mtx* a, struct Mtx* b)
{
	dest->m11 = a->m11 + b->m11;
	dest->m12 = a->m12 + b->m12;
	f32 tmp2 = b->m13;
	f32 tmp1 = a->m13;
	dest->m14 = 0.0;
	dest->m13 = tmp1 + tmp2;
	dest->m21 = a->m21 + b->m21;
	dest->m22 = a->m22 + b->m22;
	tmp2 = b->m23;
	tmp1 = a->m23;
	dest->m24 = 0.0;
	dest->m23 = tmp1 + tmp2;
	dest->m31 = a->m31 + b->m31;
	dest->m32 = a->m32 + b->m32;
	tmp2 = a->m33;
	tmp1 = b->m33;
	dest->m34 = 0.0;
	dest->m41 = 0.0;
	dest->m33 = tmp2 + tmp1;
	dest->m42 = 0.0;
	dest->m43 = 0.0;
	dest->m44 = 1.0;
}

void NuMtxSkewSymmetric(struct Mtx* m, struct Vec* v)
{
	m->m11 = 0.0;
	m->m12 = -v->z;
	f32 tmp = v->y;
	m->m14 = 0.0;
	m->m13 = tmp;
	tmp = v->z;
	m->m22 = 0.0;
	m->m21 = tmp;
	tmp = v->x;
	m->m24 = 0.0;
	m->m23 = -tmp;
	m->m31 = -v->y;
	tmp = v->x;
	m->m44 = 1.0;
	m->m32 = tmp;
	m->m43 = 0.0;
	m->m33 = 0.0;
	m->m34 = 0.0;
	m->m41 = 0.0;
	m->m42 = 0.0;
}

void NuMtxOrth(struct Mtx* m)
{
	f32 mag = NuFsqrt(m->m13 * m->m13 + m->m11 * m->m11 + m->m12 * m->m12);
	f32 t5 = 1.0;
	f32 t3 = 1.0 / mag;
	m->m13 = m->m13 * t3;
	m->m11 = m->m11 * t3;
	m->m12 = m->m12 * t3;
	mag = NuFsqrt(m->m23 * m->m23 + m->m21 * m->m21 + m->m22 * m->m22);
	t3 = t5 / mag;
	f32 m12 = m->m12;
	f32 m13 = m->m13;
	f32 m11 = m->m11;
	f32 t1 = m->m21 * t3;
	f32 t2 = m->m22 * t3;
	t3 = m->m23 * t3;
	f32 t4 = m11 * t2 - m12 * t1;
	t2 = m12 * t3 - m13 * t2;
	m->m33 = t4;
	t3 = m13 * t1 - m11 * t3;
	m->m31 = t2;
	m->m32 = t3;
	m->m23 = t2 * m12 - t3 * m11;
	m->m22 = t4 * m11 - t2 * m13;
	m->m21 = t3 * m13 - t4 * m12;
}

void NuMtxCalcCheapFaceOn(struct Mtx* dest, struct Vec* v)
{
	struct Mtx* view = NuCameraGetViewMtx();
	dest->m11 = -view->m11;
	dest->m21 = view->m12;
	dest->m31 = -view->m13;
	dest->m12 = -view->m21;
	dest->m22 = view->m22;
	dest->m32 = -view->m23;
	dest->m13 = -view->m31;
	dest->m23 = view->m32;
	f32 tmp = view->m33;
	dest->m14 = 0.0;
	dest->m44 = 1.0;
	dest->m33 = -tmp;
	dest->m34 = 0.0;
	dest->m24 = 0.0;
	dest->m41 = v->x;
	dest->m42 = v->y;
	dest->m43 = v->z;
}

void NuMtxCalcDebrisFaceOn(struct Mtx* m)
{
  struct Mtx* view = NuCameraGetViewMtx();
  m->m11 = -view->m11;
  m->m21 = view->m12;
  m->m31 = -view->m13;
  m->m12 = -view->m21;
  m->m22 = view->m22;
  m->m32 = -view->m23;
  m->m13 = -view->m31;
  m->m23 = view->m32;
  f32 tmp = view->m33;
  m->m44 = 1.0;
  m->m14 = 0.0;
  m->m33 = -tmp;
  m->m34 = 0.0;
  m->m24 = 0.0;
}


/*
void NuMtxInvH(void) //TODO
{

}

*/