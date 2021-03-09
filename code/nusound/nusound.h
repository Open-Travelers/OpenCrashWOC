#ifndef NUSOUND_H
#define NUSOUND_H

#include "../types.h"
#include "nusoundtypes.h"

/*
  800c20d0 000250 800c20d0  4 NuSoundApply3d 	Global
  800c2320 000080 800c2320  4 NuSoundFindLoopInfo 	Global
  800c23a0 000030 800c23a0  4 NuSoundInitLoopInfo 	Global
  800c23d0 000020 800c23d0  4 NuSoundInit 	Global
  800c23f0 000024 800c23f0  4 NuSoundPlay 	Global
  800c2414 0000b0 800c2414  4 NuSoundPlayLoop 	Global
  800c24c4 000060 800c24c4  4 NuSoundPlay3d 	Global
  800c2524 0000cc 800c2524  4 NuSoundPlay3dLoopSfx 	Global
  800c25f0 00002c 800c25f0  4 NuSoundPlayStereo 	Global
  800c261c 00002c 800c261c  4 NuSoundPlayStereo2 	Global
  800c2648 000028 800c2648  4 NuSoundStopStream 	Global
  800c2670 0000e8 800c2670  4 NuSoundUpdate 	Global
  800c2758 000030 800c2758  4 NuXboxSoundUpdate 	Global
  800c2788 000020 800c2788  4 NuSoundLocalSet 	Global
  800c27a8 0000cc 800c27a8  4 NuSoundSetLevelAmbience 	Global
  800c2874 00002c 800c2874  4 NuSoundKillAllAudio 	Global
  800c28a0 000020 800c28a0  4 NuSoundPauseSfx 	Global
*/

// If the track is currently playing.
u32 SS_TrackPlayingFlag;

// The current channel.
s32 SS_CurrentChannel;

// TODO: FUNCTIONS!!!

// Resume SFX.
void NuSoundResumeSfx();

// Play a sound in a channel.
void NuSoundPlayChan(u32 track, u8 leftVol, u8 rightVol, u32 pitch, s32 channel);

// If the given channel is currently playing.
u32 NuSoundKeyStatus(s32 channel);

#endif // !NUSOUND_H