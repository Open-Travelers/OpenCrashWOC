#include "nufloat.h"

f32 NuFabs(f32 f)
{
	return (f32)((u32)f & 0x7fffffff);
}

f32 NuFsign(f32 f)
{
	return f < 0.0 ? -1.0 : 1.0;
}

f32 NuFpDiv(f32 dividend, f32 divisor)
{
	if (divisor != 0.0) {
		return dividend / divisor;
	}
	return 0.0;
}