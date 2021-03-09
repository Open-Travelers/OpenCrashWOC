#include "nulight.h"

u32 initialised = 0;
u32 maxlights = 3;
u32 numlights = 0;

void NuLightClose()
{
    initialised = 0;
    numlights = 0;
}