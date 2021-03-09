#include "edbits.h"

u32 edqseed = 0x3039;
u32 edbitsSfxVol = 0x64;

u32 edqrand()
{
	edqseed = (edqseed * 0x24cd + 1) & 0xffff;
	return edqseed;
}

void edbitsRegisterSfx(char* sfxGlobalTab, char* sfxLevelTab, u32 sfxGlobalCount, u32 sfxAllCount)
{
	edSfxGlobalTab = sfxGlobalTab;
	edSfxLevelTab = sfxLevelTab;
	edSfxGlobalCount = sfxGlobalCount;
	edSfxAllCount = sfxAllCount;
}

void edbitsRegisterBaseScene(NuScene* s)
{
	edbits_base_scene = s;
}

void edbitsSetSoundFxVolume(u32 vol)
{
	edbitsSfxVol = vol;
}