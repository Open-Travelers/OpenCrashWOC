#ifndef NU_ASM_H
#define NU_ASM_H

#include "../types.h"
#include "numathtypes.h"
#include <numath.h>

// Take square root of value.
f32 NuFsqrt(f32 f);

// Divide dividend by divisor. 0 if undefined. NOTE: Is this different from the other one?
f32 NuFdiv(f32 f);

// Multiply a vector by a scalar, and place it into the destination.
void NuVec4MtxTransformVU0(struct nuvec4_s* dest, struct nuvec4_s* a, struct Mtx* b);

#endif // !NU_ASM_H
