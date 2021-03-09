#ifndef NUFLOAT_H
#define NUFLOAT_H

#include "../types.h"

// Absolute value.
f32 NuFabs(f32 f);

// 1 for positive, -1 for negative.
f32 NuFsign(f32 f);

// Divide dividend by divisor. 0 if undefined.
f32 NuFpDiv(f32 dividend, f32 divisor);

#endif // !NUFLOAT_H