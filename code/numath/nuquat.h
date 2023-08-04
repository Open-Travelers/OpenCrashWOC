#ifndef NUQUAT_H
#define NUQUAT_H

#include "../types.h"
#include "numathtypes.h"

// Convert a matric into a quaternion.
void NuMtxToQuat(struct Mtx* m, struct Quat* q);

// Convert a quaternion to a matrix.
void NuQuatToMtx(struct Quat* q, struct Mtx* m);

// Multiply quaternions.
void NuQuatMul(struct Quat* dest, struct Quat* a, struct Quat* b);

// Normalize a quaternion.
void NuQuatNormalise(struct Quat* dest, struct Quat* q);

// Interpolate a quaternion given the start a, ending b, and the alpha value.
void NuQuatSlerp(f32 alpha, struct Quat* dest, struct Quat* a, struct Quat* b);

#endif // !NUQUAT_H
