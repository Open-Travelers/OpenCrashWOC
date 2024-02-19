#ifndef NUWATER_H
#define NUWATER_H

#include "../types.h"
#include "nu3dxtypes.h"
#include "gamecode/main.h"
#include "nu3dx/nurndr.h"

/*
  800bbd60 000058 800bbd60  4 NuDynamicWaterClose 	Global
  800bbdb8 000018 800bbdb8  4 dynamicWaterGetNormalMap 	Global
  800bbdd0 0003c0 800bbdd0  4 NuDynamicWaterInit 	Global
  800bc190 0000f0 800bc190  4 NuDynamicWaterRender2dRect 	Global
  800bc280 000188 800bc280  4 NuDynamicWaterCalcShaderUVOffsets 	Global
  800bc408 000004 800bc408  4 NuDynamicWaterSetVertexShaderUVOffsets 	Global
  800bc40c 00006c 800bc40c  4 NuDynamicWaterCycleTextures 	Global
  800bc478 000308 800bc478  4 NuDynamicWaterExcite 	Global
  800bc780 000460 800bc780  4 NuDynamicWaterUpdate 	Global
  800bcbe0 000020 800bcbe0  4 Powr2 	Global
  800bcc00 000078 800bcc00  4 isWaterInstance 	Global
  800bcc78 000084 800bcc78  4 initWaterGeom 	Global
  800bccfc 00016c 800bccfc  4 initWater 	Global
  800bce68 0000a4 800bce68  4 fixMaterials 	Global
  800bcf0c 00002c 800bcf0c  4 NuWaterLoad 	Global
  800bcf38 0000c8 800bcf38  4 NuWaterInit 	Global
  800bd000 000038 800bd000  4 NuWaterClose 	Global
  800bd038 0001dc 800bd038  4 animateWater 	Global
  800bd214 000010 800bd214  4 NuWaterSetup 	Global
  800bd224 000014 800bd224  4 NuWaterSpeed 	Global
  800bd238 000120 800bd238  4 NuWaterRender 	Global
*/

s32 watervisible;
static s32 nwinst;
static s32 nwater_mtl;
static struct nugscn_s* wgsc[256];
static struct nuinstance_s* winst[256];
static s32 dynamicWaterInitialised;
static s32 dynamicWaterEnabled;
static s32 dynamicWaterForceStepOneTex;
static s32 dynamicWaterTextureIds[6];
static struct nuvec4_s dynamicWaterUVOffsets[2][6];
struct numtl_s* water_copy_mtl;
s32 DropletTID;
struct nutex_s DropletTex;
static s32 dynamicWaterBlurTex;
static s32 dynamicWaterFlipState;
static s32 dynamicWaterForceTex;
static s32 dynamicWaterHeightSourceTex;
static s32 dynamicWaterHeightTargetTex;
static s32 dynamicWaterNormalTex;
static struct D3DSurface* dynamicWaterSurfaces[6];
static s32 dynamicWaterVelocitySourceTex;
static s32 dynamicWaterVelocityTargetTex;
struct D3DTexture* pDropletTex;
struct numtl_s* dynamicWaterMaterial;
static float dynamicWaterDropFrequency;
static float dynamicWaterBlurDist;
float dynamicWaterBlend;
float dynamicWaterScale;
static f32 dynamicWaterDropMaxScale;
static f32 dynamicWaterDropMinScale;
static s32 dynamicWaterTurbulenceFrequency;
static f32 dynamicWaterTurbulenceScale;
static s32 dynamicWaterTurbulenceStrength;
static float theta_step;
struct nuvec4_s waterUVOffset[32];

#endif // !NUWATER_H
