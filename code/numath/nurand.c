#include "nurand.h"

u32 fseed = 0;
//global_rand.idum = 1;

long NuRand(struct nunrand_s* nrand)
{
	if (nrand == NULL)
	{
		nrand = &global_rand;
	}
	u32 val = (nrand->idum ^ 0x75bd924) * 0x41a7 + ((s32)(nrand->idum ^ 0x75bd924) / 0x31e5) * -0xccbbc77;
	nrand->idum = val;
	if ((s32)val < 0)
	{
		nrand->idum = val + 0x7fffffff;
	}
	nrand->idum = nrand->idum ^ 0x75bd924;

	return nrand->idum;
}

void NuRandSeed(u32 seed)
{
	fseed = seed;
}

f32 NuRandFloat()
{
	fseed = fseed * 0x19660d + 0x3c6ef35f;
	return (f32)(fseed & 0x7fffff | 0x3f800000) - 1.0;
}
