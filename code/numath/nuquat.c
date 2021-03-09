#include "nuquat.h"
#include "nu_asm.h"

void NuMtxToQuat(Mtx* m, Quat* q)
{
	f32 qw2 = m->m11 + m->m22 + m->m33;
	s32 weight[3];
	weight[0] = 1;
	weight[1] = 2;
	weight[2] = 0;
	if (qw2 <= 0.0)
	{
		u32 mode = m->m11 < m->m22;
		if ((&m->m11)[mode * 5] < m->m33)
		{
			mode = 2;
		}
		s32 p1 = weight[mode];
		s32 p2 = weight[p1];
		f32 qw2P2 = NuFsqrt((((&m->m11)[mode * 5] - ((&m->m11)[p1 * 5] + (&m->m11)[p2 * 5])) + 1.0));
		qw2 = qw2P2;
		Quat tmp;
		(&tmp.v.x)[mode] = qw2 * 0.5;
		if (qw2 != 0.0)
		{
			qw2 = 0.5 / qw2;
		}
		f32 f1 = (&m->m11)[p2 * 4 + mode];
		f32 f2 = (&m->m11)[mode * 4 + p2];
		tmp.w = ((&m->m11)[p1 * 4 + p2] - (&m->m11)[p2 * 4 + p1]) * qw2;
		(&tmp.v.x)[p1] = ((&m->m11)[mode * 4 + p1] + (&m->m11)[p1 * 4 + mode]) * qw2;
		(&tmp.v.x)[p2] = (f2 + f1) * qw2;
		(q->v).x = tmp.v.x;
		q->w = tmp.w;
		(q->v).y = tmp.v.y;
		(q->v).z = tmp.v.z;
	}
	else
	{
		f32 qw2P2 = sqrt((qw2 + 1.0));
		qw2 = 0.5 / qw2P2;
		q->w = qw2P2 * 0.5;
		(q->v).x = (m->m23 - m->m32) * qw2;
		(q->v).y = (m->m31 - m->m13) * qw2;
		(q->v).z = (m->m12 - m->m21) * qw2;
	}
}

void NuQuatToMtx(Quat* q, Mtx* m)
{
	f32 x = q->v.x;
	f32 y = q->v.y;
	f32 z = q->v.z;
	f32 w = q->w;
	m->m44 = 1.0;
	f32 y2 = y * y;
	f32 diff = w * w - x * x;
	m->m34 = 0.0;
	m->m41 = 0.0;
	m->m42 = 0.0;
	m->m43 = 0.0;
	m->m14 = 0.0;
	m->m24 = 0.0;
	f32 xy = x * y + x * y;
	f32 xz = x * z + x * z;
	f32 xw = x * w + x * w;
	f32 yz = y * z + y * z;
	f32 yw = y * w + y * w;
	y = z * w + z * w;
	z = z * z;
	m->m12 = xy + y;
	m->m13 = xz - yw;
	m->m23 = yz + xw;
	m->m33 = (diff - y2) + z;
	m->m11 = ((w * w + x * x) - y2) - z;
	m->m22 = (diff + y2) - z;
	m->m21 = xy - y;
	m->m31 = xz + yw;
	m->m32 = yz - xw;
}

void NuQuatMul(Quat* dest, Quat* a, Quat* b)
{
	dest->w = ((a->w * b->w - a->v.x * b->v.x) - a->v.y * b->v.y) - a->v.z * b->v.z;
	dest->v.x = (a->v.y * b->v.z + a->w * b->v.x + a->v.x * b->w) - a->v.z * b->v.y;
	dest->v.y = (a->v.z * b->v.x + a->w * b->v.y + a->v.y * b->w) - a->v.x * b->v.z;
	dest->v.z = (a->v.x * b->v.y + a->w * b->v.z + a->v.z * b->w) - a->v.y * b->v.x;
}

void NuQuatNormalise(Quat* dest, Quat* q)
{
	f32 mag = q->v.x * q->v.x + q->v.y * q->v.y + q->v.z * q->v.z + q->w * q->w;
	if (mag <= 0)
	{
		dest->v.x = q->v.x;
		dest->v.y = q->v.y;
		dest->v.z = q->v.z;
		dest->w = q->w;
	}
	else
	{
		mag = NuFsqrt(mag);
		f32 scale = 1.0 / mag;
		dest->v.x = q->v.x * scale;
		dest->v.y = q->v.y * scale;
		dest->v.z = q->v.z * scale;
		dest->w = q->w * scale;
	}
}

void NuQuatSlerp(f32 alpha, Quat* dest, Quat* a, Quat* b)
{
	f32 mag = a->v.x * a->v.x + a->v.y * a->v.y + a->v.z * a->v.z + a->w * a->w;
	f32 x = b->v.x;
	f32 y = b->v.y;
	f32 z = b->v.z;
	f32 w = b->w;
	if (mag < 0.0)
	{
		x = -x;
		y = -y;
		z = -z;
		w = -w;
		mag = -mag;
	}
	f32 rot = 1.0;
	if (1.0 - mag <= 0.0)
	{
		mag = 1.0 - alpha;
	}
	else
	{
		f32 ac = acos(mag);
		f32 s = sin(ac);
		mag = sin((rot - alpha) * ac);
		mag = mag / s;
		rot = sin(alpha * ac);
		alpha = rot / s;
	}
	dest->v.x = mag * a->v.x + alpha * x;
	dest->v.y = mag * a->v.y + alpha * y;
	dest->v.z = mag * a->v.z + alpha * z;
	dest->w = mag * a->w + alpha * w;
}