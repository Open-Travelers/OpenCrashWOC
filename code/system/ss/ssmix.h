#ifndef SSMIX_H
#define SSMIX_H

#include "../../types.h"
#include "sstypes.h"

#define NUM_MIX_CHANNELS 64

// Mixer hannels.
MIX __MIXChannel[NUM_MIX_CHANNELS];

// Current attentuation.
u32 __MIXDvdStreamAttenCurrent;

// User attenuation.
u32 __MIXDvdStreamAttenUser;

// Sound mode.
u32 __MIXSoundMode;

/*
  800cd2bc 00003c 800cd2bc  4 __MIXGetVolume 	Global
  800cd320 0000b0 800cd320  4 __MIXResetChannel 	Global
  800cd3d0 000058 800cd3d0  4 __MIXSetPan 	Global
  800cd480 0004c4 800cd480  4 MIXInitChannel 	Global
  800cd944 00001c 800cd944  4 MIXReleaseChannel 	Global
  800cd960 000028 800cd960  4 MIXSetInput 	Global
*/

// Convert pan byte to pan value.
s32 __MIXPanTableL[128];

// Convert pan byte to pan value.
s32 __MIXPanTableR[128];

// Get the true pan value.
s32 __MIXGetPanL(s8 pan);

// Get the true pan value.
s32 __MIXGetPanR(s8 pan);

// Initialize all the mixing channels.
void MIXInit();

#endif // !SSMIX_H