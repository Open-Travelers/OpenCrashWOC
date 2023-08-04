#ifndef NUGLASS_H
#define NUGLASS_H

#include "../types.h"
#include "nu3dxtypes.h"

/*
  800aed6c 000068 800aed6c  4 InitSpecular 	Global
  800aedd4 000008 800aedd4  4 GetGlassSpecularTexId 	Global
  800aeddc 0001bc 800aeddc  4 InitOverrideMtl 	Global
  800aef98 000064 800aef98  4 isGlassInstance 	Global
  800aeffc 0002bc 800aeffc  4 NuGlassProcessScene 	Global
  800af2b8 000064 800af2b8  4 NuGlassLoad 	Global
  800af31c 00002c 800af31c  4 NuGlassInit 	Global
  800af348 0000a0 800af348  4 NuGlassClose 	Global
  800af3e8 000198 800af3e8  4 ProcessGlass 	Global
  800af580 0000a0 800af580  4 DrawGlassCreatures 	Global
  800af620 0000e0 800af620  4 NuGlassRenderStatic 	Global
*/


static int spectid;

numtl_s* glass_mtl_blendskin;

numtl_s* glass_mtl_blendskin2;

static int num_glass_inst;

static nugobj_s* glassGobj[256];

static nuinstance_s* glass_inst[256]; 

static int num_glass_inst;

int SKELETALCRASH;

float glass_mix;

float glass_mix_speed;

float WATERBOSSGLASSMIX;

float MAXGLASSMIX;

int glass_enabled; 

float ANGELGLASSMIX;

float glass_col_mix;

int glass_col_enabled;

float glass_col_mix_speed;

nucolour3_s glass_col[2];

int GLASSPLAYERFADE;

int GLASSPLAYER;

int glass_phase;

int glass_draw;

int force_glass_screencopy_enable;

float glass_strength;

float plr_invisibility_time;


#endif // !NUGLASS_H