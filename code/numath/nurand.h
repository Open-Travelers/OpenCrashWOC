#ifndef NURAND_H
#define NURAND_H

#include "../types.h"

// Float random seed.
u32 fseed;

// Current random number.
u32 global_rand;

// Get a random number.
void NuRand(u32* rand);

// Set the float seed.
void NuRandSeed(u32 seed);

// Fetch a random float.
f32 NuRandFloat();

#endif // !NURAND_H