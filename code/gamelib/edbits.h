#ifndef EDBITS_H
#define EDBITS_H

#include "../types.h"
#include "../nu.h"

// Ed random seed.
extern u32 edqseed;

// SFX volume.
extern u32 edbitsSfxVol;

// SFX name global tab.
extern char* edSfxGlobalTab;

// SFX name level tab.
extern char* edSfxLevelTab;

// SFX name global count.
extern s32 edSfxGlobalCount;

// SFX name total count.
extern u32 edSfxAllCount;

extern pSFX* edSfxLevelTab;

extern char edbits_what_game;

// Base scene.
extern nugscn_s* edbits_base_scene;

/*
  80084750 0000ec 80084750  4 edbitsLookupSoundFX 	Global
  80084844 000118 80084844  4 edbitsSoundPlay 	Global
  8008495c 00008c 8008495c  4 edbitsLookupInstance 	Global
*/


// Size: 0x30
struct
{
    char name[16];
    unsigned short pitch;
    unsigned short volume;
    unsigned char buzz;
    unsigned char rumble;
    unsigned char delay;
    unsigned char wait;
    char* path;
    unsigned short frequency;
    unsigned short stream;
    signed char type;
    char pad1;
    unsigned short id;
    nuvec_s Pos;
}pSFX;

// Get a random number.
u32 edqrand();

// Register the SFX tabs.
void edbitsRegisterSfx(char* sfxGlobalTab, char* sfxLevelTab, u32 sfxGlobalCount, u32 sfxAllCount);

// Register the base scene.
void edbitsRegisterBaseScene(NuScene* s);

// Set the SFX volume.
void edbitsSetSoundFxVolume(u32 vol);

#endif // !EDBITS_H