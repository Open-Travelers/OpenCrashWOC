#include "nurand.h"

u32 fseed = 0;
u32 global_rand = 1;

void NuRand(u32* rand)
{
	if (rand == NULL)
	{
		rand = &global_rand;
	}
	u32 val = (*rand ^ 0x75bd924) * 0x41a7 + ((s32)(*rand ^ 0x75bd924) / 0x31e5) * -0xccbbc77;
	*rand = val;
	if ((s32)val < 0)
	{
		*rand = val + 0x7fffffff;
	}
	*rand = *rand ^ 0x75bd924;
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