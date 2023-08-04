#ifndef NUMTL_H
#define NUMTL_H

#include "../types.h"
#include "nu3dxtypes.h"

/*
  800b2aac 000074 800b2aac  4 NuMtlCreateDefault 	Global
  800b2b20 000040 800b2b20  4 NuMtlInit 	Global
  800b2b60 000058 800b2b60  4 NuMtlClose 	Global
  800b2bb8 0000a4 800b2bb8  4 NuMtlCreate 	Global
  800b2c5c 000080 800b2c5c  4 NuMtlDestroy 	Global
  800b2cdc 000008 800b2cdc  4 NuMtlNum 	Global
  800b2ce4 000034 800b2ce4  4 NuMtlGet 	Global
  800b2d18 000048 800b2d18  4 NuMtlRead 	Global
  800b2d60 00008c 800b2d60  4 NuMtlInsert 	Global
  800b2dec 000038 800b2dec  4 NuMtlRemove 	Global
  800b2e24 000004 800b2e24  4 NuMtlUpdate 	Global
  800b2e28 000008 800b2e28  4 NuMtlSetStencilRender 	Global
  800b2e30 000190 800b2e30  4 NuMtlAddRndrItem 	Global
  800b2fc0 000108 800b2fc0  4 NuMtlAddFaceonItem 	Global
  800b30c8 00008c 800b30c8  4 NuMtlOTInsert 	Global
  800b3154 00013c 800b3154  4 NuMtlGet2dBuffer 	Global
  800b3290 000138 800b3290  4 NuMtlGet3dBuffer 	Global
  800b33c8 000174 800b33c8  4 NuMtlRender 	Global
  800b353c 0000b0 800b353c  4 NuMtlRender3d 	Global
  800b35ec 000164 800b35ec  4 NuMtlRenderDynamic2d3d 	Global
  800b3750 0000f4 800b3750  4 NuMtlRenderOT 	Global
  800b3844 0003c8 800b3844  4 NuMtlRenderFaceOn 	Global
  800b3c0c 000090 800b3c0c  4 NuMtlRenderGlass 	Global
  800b3c9c 000094 800b3c9c  4 NuMtlRenderWater 	Global
  800b3d30 000160 800b3d30  4 NuMtlRenderSten 	Global
  800b3e90 0000b0 800b3e90  4 NuMtlAddGlassItem 	Global
  800b3f40 00003c 800b3f40  4 NuMtlClearGlassList 	Global
  800b3f7c 000040 800b3f7c  4 NuMtlRenderUpd 	Global
  800b3fbc 0002c8 800b3fbc  4 NuMtlSetRenderStates 	Global
  800b4284 00006c 800b4284  4 NuMtlAnimate 	Global
  800b42f0 000160 800b42f0  4 NuMtlUVAnimation 	Global
  800b4450 000034 800b4450  4 NuMtlClearOt 	Global
*/

/**********************************************************/
// Prototypes
/**********************************************************/
struct numtl_s* NuMtlCreate(s32 mode);
void NuMtlUVAnimation(struct nugobj_s* gobj);
/**********************************************************/

// Size: 0x10
struct _D3DCOLORVALUE
{
    f32 r;
    f32 g;
    f32 b;
    f32 a;
};

// Size: 0x44
struct _D3DMATERIAL8
{
    struct _D3DCOLORVALUE Diffuse;
    struct _D3DCOLORVALUE Ambient;
    struct _D3DCOLORVALUE Specular;
    struct _D3DCOLORVALUE Emissive;
    float Power;
};

// Size: 0x10
struct nuotitem_s
{
    struct nuotitem_s* next;
    struct nurndritem_s* hdr;
    float dist;
    struct nusysmtl_s* mtl;
};

// Size: 0xC
struct nuwateritem_s
{
    struct nuwateritem_s* next;
    struct nurndritem_s* hdr;
    struct nusysmtl_s* mtl;
};

// Size: 0x10
struct nustenitem_s
{
    struct nustenitem_s* next;
    struct nurndritem_s* hdr;
    struct nusysmtl_s* mtl;
    s32 type;
};

enum nustencilmode_e
{
    NUSTENCIL_REPLACE_NODRAW = 1,
    NUSTENCIL_NOSTENCIL = 0
};



struct nuotitem_s otitem[2024];
struct nuotitem_s* ot[257];
enum nustencilmode_e stencil_mode;
struct nustenitem_s stenitem[512];
struct nufaceonitem_s* faceonmtllist[50];
struct nuotitem_s dynamic_glass_items[64];
struct nuwateritem_s wateritem[512];
extern f32 sinetime_246;


#endif // !NUMTL_H
