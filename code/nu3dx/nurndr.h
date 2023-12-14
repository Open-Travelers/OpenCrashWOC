#ifndef NURNDR_H
#define NURNDR_H

#include "../types.h"
#include "nu3dx/nu3dxtypes.h"
#include "nu3dx/nucvtskin.h"
#include "nu3dx/nuanim.h"
#include "nu3dx/nuhgobj.h"

/*
  800b4484 00002c 800b4484  4 NuRndrInit 	Global
  800b44b0 000080 800b44b0  4 NuRndrBeginScene 	Global
  800b4530 000040 800b4530  4 NuRndrEndScene 	Global
  800b4570 000024 800b4570  4 NuRndrFlush 	Global
  800b4594 00004c 800b4594  4 NuRndrClear 	Global
  800b45e0 000094 800b45e0  4 NuRndrSwapScreen 	Global
  800b4674 0003e4 800b4674  4 NuRndrGobj 	Global
  800b4a58 0002f8 800b4a58  4 NuRndrGrassGobj 	Global
  800b4d50 000240 800b4d50  4 NuRndrGobjSkin2 	Global
  800b4f90 00017c 800b4f90  4 NuRndrTri2d 	Global
  800b510c 0001d8 800b510c  4 NuRndrTri3d 	Global
  800b52e4 000254 800b52e4  4 NuRndrStrip3d 	Global
  800b5538 0002a0 800b5538  4 NuHGobjRndr 	Global
  800b57d8 00022c 800b57d8  4 NuHGobjEvalAnim2 	Global
  800b5a04 000264 800b5a04  4 NuHGobjEvalAnim 	Global
  800b5c68 000284 800b5c68  4 NuHGobjEvalAnimBlend 	Global
  800b5eec 00004c 800b5eec  4 NuRndrItemDist 	Global
  800b5f38 000108 800b5f38  4 NuRndrItem 	Global
  800b6040 000090 800b6040  4 NuRndr2dItem 	Global
  800b60d0 0002bc 800b60d0  4 NuRndrFaceItem 	Global
  800b638c 0001e4 800b638c  4 NuRndrGeomItem 	Global
  800b6570 0000f8 800b6570  4 NuRndrSkinItem2 	Global
  800b6668 000240 800b6668  4 NuRndrBlendedSkinItem 	Global
  800b68a8 0002dc 800b68a8  4 NuHGobjRndrMtxDwa 	Global
  800b6b84 000024 800b6b84  4 NuHGobjRndrMtx 	Global
  800b6ba8 000254 800b6ba8  4 NuHGobjEval 	Global
  800b6dfc 0000b4 800b6dfc  4 NuRndrAnglesZX 	Global
  800b6eb0 0002d0 800b6eb0  4 NuRndrAddFootPrint 	Global
  800b7180 0001c8 800b7180  4 NuRndrFootPrints 	Global
  800b7348 000070 800b7348  4 NuRndrCreateBlendShapeDeformerWeightsArray 	Global
  800b73b8 000074 800b73b8  4 NuRndrCreateBlendShapeDWAPointers 	Global
  800b742c 00002c 800b742c  4 NuRndrInitWorld 	Global
  800b7458 000020 800b7458  4 NuRndrFx 	Global
  800b7478 000400 800b7478  4 NuRndrTrail 	Global
  800b7878 000074 800b7878  4 NuRndrAddWaterRipple 	Global
  800b78ec 000268 800b78ec  4 NuRndrWaterRip 	Global
  800b7b54 000130 800b7b54  4 NuRndrWaterRippleUpdate 	Global
  800b7c84 0000c0 800b7c84  4 NuRndrAddShadow 	Global
  800b7d44 0001bc 800b7d44  4 NuRndrShadPolys 	Global
*/
s32 fog_enabled;

// Size: 0x34
struct FootData
{
    struct nuvec_s pnts[4];
    short brightness;
    char timer;
    char gfx;
};

// Size: 0x18
struct ShadPolDat
{
    struct nuvec_s pos;
    float size;
    short shade;
    short xrot;
    short yrot;
    short zrot;
};

// Size: 0x24
struct WaterDat
{
    struct nuvec_s pos;
    float size;
    float endsize;
    float cursize;
    unsigned int shade;
    unsigned int curshade;
    short timer;
    short otimer;
};

// Size: 0x20
struct trail
{
    struct nuvec_s pos1;
    struct nuvec_s pos2;
    int intensity;
    int RealIntensity;
};


/*************************NuRndrParticleGroup structs**************************/

/****VAR ARG*********/


// Size: 0x28
    struct setupDat
    {
        struct nuvec_s vt[3];
        int colour;
    };


// Size: 0xA48
struct setup_s
{
    int DmaHeader[4];
    float grav;
    float gtime;
    int DmaBody[4];
    float u0;
    float v0;
    float u1;
    float v1;
    float u2;
    float v2;
    float u3;
    float v3;
    struct setupDat Data[64];
};

/******************/

// Size: 0x410
 struct rdat
    {
        int dmadata[2];
        int unpackdata[2];
        // Size: 0x20
        struct
        {
            float x;
            float y;
            float z;
            float time;
            float mx;
            float my;
            float mz;
            float etime
        } debris[32];
    };
/************************************************************/



struct numtl_s* nurndr_forced_mtl;

struct numtl_s** nurndr_forced_mtl_table;

s32 NuRndrShadowCnt;

struct WaterDat NuRndrWaterRipDat[32];

static s32 rndrmtx_cnt;

s32 rndrmtx_cnt_max;

static s32 geomitem_cnt;

s32 padflag;

s32 geomitem_cnt_max;

static s32 rndr_blend_shape_deformer_wt_ptrs_cnt;

static s32 rndr_blend_shape_deformer_wt_cnt;

static float* rndr_blend_shape_deformer_wt_ptrs[2048];

static float rndr_blend_shape_deformer_wts[1024];

static struct numtx_s rndrmtx[1536];

struct FootData NuRndrFootData[64];

struct ShadPolDat NuRndrShadPolDat[128];

int NuRndrFootFree;

int NuRndrShadMaskCount;

static int fadecol;

short indexlist_272[32];

s32 NuRndrWaterRipCnt;

struct numtx_s mtx_arrayHGobj[256];


#endif // !NURNDR_H
