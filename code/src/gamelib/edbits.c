#include "edbits.h"

u32 edqseed;
u32 edbitsSfxVol;
char* edSfxGlobalTab;
char* edSfxLevelTab;
u32 edSfxGlobalCount;
u32 edSfxAllCount;
nugscn_s* edbits_base_scene;

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


int edbitsLookupSoundFX(char *name)	//CHECK

{
  int cmp;
  int i;
  int j;
  
  if ((edSfxLevelTab != (pSFX *)0x0) && (j = 0, 0 < edSfxAllCount - edSfxGlobalCount)) {
    i = 0;
    do {
      cmp = strncmp(edSfxLevelTab->name + i,name,0xf);
      if (cmp == 0) {
        return j + edSfxGlobalCount;
      }
      j = j + 1;
      i = i + 0x30;
    } while (j < edSfxAllCount - edSfxGlobalCount);
  }
  if ((edSfxGlobalTab != 0) && (j = 0, 0 < edSfxGlobalCount)) {
    i = 0;
    do {
      cmp = strncmp((char *)(edSfxGlobalTab + i),name,0xf);
      if (cmp == 0) {
        return j;
      }
      j = j + 1;
      i = i + 0x30;
    } while (j < edSfxGlobalCount);
  }
  return -1;
}

void edbitsSoundPlay(nuvec_s *pos,int sid)		//CHECK

{
  pSFX *SFXTab;
  int tsid;
  
  if (edbits_what_game == '\x02') {
    if (sid < edSfxGlobalCount) {
      tsid = sid * 0x30;
      SFXTab = edSfxGlobalTab;
    }
    else {
      tsid = (sid - edSfxGlobalCount) * 0x30;
      SFXTab = edSfxLevelTab;
    }
    gamesfx_pitch = (int)*(ushort *)(SFXTab->name + tsid + 0x10);
    gamesfx_effect_volume = (int)*(ushort *)(SFXTab->name + tsid + 0x12);
    gamesfx_edbits = 1;
    GameSfx(sid,pos);
    debris_sfx = 0;
  }
  else if (sid < edSfxGlobalCount) {
    tsid = (int)((uint)edSfxGlobalTab[sid].volume * edbitsSfxVol) / 100;
    NuSoundPlay3d(pos,sid,tsid,tsid,(uint)edSfxGlobalTab[sid].pitch);
  }
  else {
    tsid = (int)((uint)edSfxLevelTab[sid - edSfxGlobalCount].volume * edbitsSfxVol) / 100;
    NuSoundPlay3d(pos,sid,tsid,tsid,(uint)edSfxLevelTab[sid - edSfxGlobalCount].pitch);
  }
  return;
}


int edbitsLookupInstance(char *name)		//CHECK

{
  int cmp;
  int i;
  int cnt;
  
  if ((edbits_base_scene != (nugscn_s *)0x0) && (cnt = 0, 0 < edbits_base_scene->numspecial)) {
    i = 0;
    do {
      cmp = strncmp(*(char **)((int)(&edbits_base_scene->specials->mtx + 1) + i + 4),name,0x13);
      if (cmp == 0) {
        return cnt;
      }
      cnt = cnt + 1;
      i = i + 0x50;
    } while (cnt < edbits_base_scene->numspecial);
  }
  return -1;
}