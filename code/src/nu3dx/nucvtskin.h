#ifndef NUCVTSKN_H
#define NUCVTSKN_H

#include "../types.h"
#include "nu3dxtypes.h"
#include "nugobj.h"


/*
  800ad8e8 00003c 800ad8e8  4 InitSkinning 	Global
  800ad924 000038 800ad924  4 CloseSkinning 	Global
  800ad95c 0003d8 800ad95c  4 CreateSkinGeom 	Global
  800add34 00010c 800add34  4 FillFreeMatrixSlots 	Global
  800ade40 0009a4 800ade40  4 SortPrimdefs 	Global
  800ae7e4 000064 800ae7e4  4 AddMtxToPrimDef 	Global
  800ae848 00013c 800ae848  4 SetVtxSkinData 	Global
  800ae984 000124 800ae984  4 SetVtxSkinData2 	Global
  800aeaa8 000288 800aeaa8  4 NuPs2CreateSkinNorm 	Global
  800aed30 00003c 800aed30  4 NuPs2CreateSkin 	Global
*/

struct primdef_s* primdefs_sorted;
struct primdef_s* primdefs;
extern s32 stritot;
extern s32 totaldupes;
extern s32 totalpts;
extern s32 tritot;
union variptr_u skinbuffer_free;
extern s32 stats[15];

static char mtxused[256];

// Size: 0x18
struct nuvtx_lc1_s
{
    struct nuvec_s pnt;
    int diffuse;
    float tc[2];
};


// Size: 0x10
struct nuvtx_ps_s
{
    struct nuvec_s pnt;
    int diffuse;
};

// Size: 0x38
struct nuvtx_sk3tc1_s
{
    struct nuvec_s pnt;
    float weights[2];
    float indexes[3];
    struct nuvec_s nrm;
    int diffuse;
    float tc[2];
};

// Size: 0x24
struct nuvtx_tc1_s
{
    struct nuvec_s pnt; // Offset: 0x0
    struct nuvec_s nrm; // Offset: 0xC
    int diffuse; // Offset: 0x18
    float tc[2]; // Offset: 0x1C
};

// Size: 0x1C
struct nuvtx_tltc1_s
{
    struct nuvec_s pnt;
    float rhw;
    int diffuse;
    float tc[2];
};

// Size: 0x18
struct nuvtx_ts_s
{
    struct nuvec_s vTangent;
    struct nuvec_s vNormal;
};


// Size: 0x174
struct primdef_s
{
    struct nuvtx_tc1_s vrts[3];
    int vid[3];
    float weights[3][15]; //dwarf orginal: [15][3]
    int mtxid[15];
    int nummtx;
    int sorted;
    int baseid;
};

#endif // !NUCVTSKN_H
