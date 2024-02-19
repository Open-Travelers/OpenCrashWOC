#ifndef NUSCENE_H
#define NUSCENE_H

#include "../types.h"
#include<nucore/nufile.h>
#include "nu3dx/nugobj.h"
#include <stdlib.h>
#include "nu3dxtypes.h"
#include "nu3dx/nutxanm.h"


void ReadNuIFFTextureSet(fileHandle handle, struct nuscene_s *scene);
void ReadNuIFFMaterialSet(fileHandle fh,struct nuscene_s *sc);
void NuSceneMtlUpdate(struct nuscene_s *nus);
void ReadNuIFFAnimationLibrary(fileHandle handle, struct nugscn_s* scene);
static void ReadNuIFFGSplineSet(s32 fh, struct nugscn_s *gsc);
void ReadNuIFFGobjSet(fileHandle handle, struct nuscene_s* nus);
struct nugobj_s* ReadNuIFFGeom(s32 handle, struct numtl_s** mtls);
void ReadNuIFFGeomDef(fileHandle handle, struct nuscene_s* nus);
void ReadNuIFFGeomVtx(fileHandle handle, struct nugeom_s* geom);
void ReadNuIFFGeomCntrl(fileHandle handle, struct nugeom_s* geom);
static void ReadNuIFFFaceOnGeom(s32 handle,struct nufaceongeom_s *face);
static void ReadNuIFFGeomPrim(s32 fh, struct nugeom_s* geom);
void ReadNuIFFGeomTri(fileHandle handle, struct nugeom_s* geom, enum nuprimtype_e type);
static void ReadNuIFFGeomSkin(s32 handle,struct nugeom_s *geom);
static void ReadNuIFFBlendShape(s32 fh,struct nugeom_s *geom);
void ReadNuIFFUnknown(s32 fh, s32 blk);
s32 ReadNuIFFInstSet(fileHandle fh, struct nuinstance_s** instances, struct nuinstanim_s** instanims);
static void ReadNuIFFSpecialObjects(s32 fh,struct nugscn_s *gsc);
static void NuSceneCalcCulling(struct nugscn_s *scene);
void ReadNuIFFGScene(fileHandle handle,struct nugscn_s *gscene);
struct nuscene_s * NuSceneLoad(char *filename);
s8* ReadNuIFFNameTable(s32 handle);

#endif // !NUSCENE_H
