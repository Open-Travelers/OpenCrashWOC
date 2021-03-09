#ifndef NUQUAT_H
#define NUQUAT_H

#include "../types.h"
#include "numathtypes.h"

// Convert a matric into a quaternion.
void NuMtxToQuat(Mtx* m, Quat* q);

// Convert a quaternion to a matrix.
void NuQuatToMtx(Quat* q, Mtx* m);

// Multiply quaternions.
void NuQuatMul(Quat* dest, Quat* a, Quat* b);

// Normalize a quaternion.
void NuQuatNormalise(Quat* dest, Quat* q);

// Interpolate a quaternion given the start a, ending b, and the alpha value.
void NuQuatSlerp(f32 alpha, Quat* dest, Quat* a, Quat* b);

#endif // !NUQUAT_H