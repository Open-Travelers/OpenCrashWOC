#ifndef NUTRIG_H
#define NUTRIG_H

#include "../types.h"

#define ANGLE_RANGE 0x10000
#define NU_TRIG_TABLE_SIZE ANGLE_RANGE
#define ANGLE_0 0x0000
#define ANGLE_45 0x2000
#define ANGLE_90 0x4000
#define ANGLE_135 0x6000
#define ANGLE_180 0x8000
#define ANGLE_225 0xA000
#define ANGLE_270 0xC000
#define ANGLE_315 0xE000
#define PI ANGLE_180
#define PIOV2 ANGLE_90
#define PI3OV2 ANGLE_270
#define UNIT_CIRCLE_PRECISION 513

#define ANG_SIN(a) NuTrigTable[a]
#define ANG_COS(a) NuTrigTable[(a + PIOV2)]

// Why do expensive trig when we can store it here?
extern f32 NuTrigTable[NU_TRIG_TABLE_SIZE];

// Lookup table for converting unit length floats to angles.
extern angle ang[UNIT_CIRCLE_PRECISION];

// Setup the sin lookup table.
void NuTrigInit();

// If fixed-point x and y represent a unit vector of magnitude <= 1 in the unit circle, return the angle it produces.
angle xy(fxi x, fxi y);

// Return the tan-1 with x and y fixed-point sizes.
angle NuAtani(fxi x, fxi y);

// Get the float tan-1 with x and y sizes.
f32 NuAtan2(f32 x, f32 y);

// If x and y represent a unit vector of magnitude <= 1 in the unit circle, return the angle it produces.
angle fxyd(f32 x, f32 y);

// Return the tan-1 with x and y sizes.
angle NuAtan2D(f32 x, f32 y);

// Return a - b.
angle NuAngleSub(angle a, angle b);

#endif // !NUTRIG_H
