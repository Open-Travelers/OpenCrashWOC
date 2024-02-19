#ifndef NURAND_H
#define NURAND_H

#include "../types.h"

// Float random seed.
extern u32 fseed;

// Current random number.
static struct nunrand_s global_rand;

// Get a random number.
long NuRand(struct nunrand_s* rand);

// Set the float seed.
void NuRandSeed(u32 seed);

// Fetch a random float.
f32 NuRandFloat();


#endif // !NURAND_H
