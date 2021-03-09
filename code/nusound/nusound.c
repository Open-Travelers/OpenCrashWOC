#include "nusound.h"

u32 SS_TrackPlayingFlag = 0;
s32 SS_CurrentChannel = -1;

void NuSoundResumeSfx()
{
	//SS_ResumeSFX();
}

void NuSoundPlayChan(u32 track, u8 leftVol, u8 rightVol, u32 pitch, s32 channel)
{
	//PlayStream(track, leftVol, channel);
}

u32 NuSoundKeyStatus(s32 channel)
{
	if ((SS_CurrentChannel == channel) && (SS_TrackPlayingFlag != 0)) {
		return PLAYING;
	}
	return STOPPED;
}