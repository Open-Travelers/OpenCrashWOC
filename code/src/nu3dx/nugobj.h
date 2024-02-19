#ifndef NUGOBJ_H
#define NUGOBJ_H

#include "../types.h"
#include "numath/numathtypes.h"
#include "nu3dxtypes.h"
#include "../nucore/nuerror.h"
/**********************************************************/
// Prototypes
/**********************************************************/
/*void NuGobjInit(void);
void NuGobjClose(void);
struct nugobj_s* NuGobjCreate(void);
void NuGobjDestroy(struct nugobj_s* obj);
void NuGobjAddGeom(struct nugobj_s* gobj, struct nugeom_s* geom);
void NuGobjAddFaceOnGeom(struct nugobj_s* gobj, struct nufaceongeom_s* Fgeom);
struct nugeom_s* NuGeomCreate(void);
struct nufaceongeom_s* NuFaceOnGeomCreate(void);
void NuGeomDestroy(struct nugeom_s* geom);
void NuGeomCreateVB(struct nugeom_s* geom, u32 vtxCount, enum nuvtxtype_e vtxType, s32 dynamic);
void NuGeomDestroyVB(struct nugeom_s* geom);
void NuGeomAddPrim(struct nugeom_s* geom, struct nuprim_s* prim);
void NuGeomAddSkin(struct nugeom_s* geom, struct nuskin_s* skin);
struct nuprim_s* NuPrimCreate(int amount, enum nuprimtype_e type);
void NuPrimDestroy(struct nuprim_s* prim);
void* GS_CreateBuffer(u32 bufsize, s32 bufferType);
void GS_DeleteBuffer(void* ptr);
int NuVtxStride(enum nuvtxtype_e type);
void NuAnimUV(void);*/
void NuGobjCalcDims(struct nugobj_s* gobj);
/**********************************************************/
// Variables
/**********************************************************/
static s32 sysinit;
extern s32 Paused;
extern s32 _timer;
extern u32 GS_BufferSize;
extern u32 BufferTypes[4];
static struct nugobj_s* sysgobj;
/**************************************************************/

#endif // !NUGOBJ_H
