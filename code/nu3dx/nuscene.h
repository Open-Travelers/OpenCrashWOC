#ifndef NUSCENE_H
#define NUSCENE_H

#include "../types.h"
#include<nucore/nufile.h>
#include "nu3dx/nugobj.h"
#include <stdlib.h>
#include "nu3dxtypes.h"


void ReadNuIFFTextureSet(fileHandle handle, struct nuscene_s *scene);
void ReadNuIFFMaterialSet(fileHandle fh,struct nuscene_s *sc);
void NuSceneMtlUpdate(struct nuscene_s *nus);
void ReadNuIFFAnimationLibrary(fileHandle handle, struct nugscn_s* scene);
void ReadNuIFFGSplineSet(fileHandle fh,struct nugscn_s *gsc);
void ReadNuIFFGobjSet(fileHandle handle, struct nuscene_s* nus);
struct nugobj_s * ReadNuIFFGeom(fileHandle handle,struct numtl_s **mtls);
void ReadNuIFFGeomDef(fileHandle handle, struct nuscene_s* nus);
void ReadNuIFFGeomVtx(fileHandle handle, struct nugeom_s* geom);
void ReadNuIFFGeomCntrl(fileHandle handle, struct nugeom_s* geom);
void ReadNuIFFFaceOnGeom(fileHandle handle,struct nufaceongeom_s *face);
void ReadNuIFFGeomPrim(fileHandle handle, struct nugeom_s* geom);
void ReadNuIFFGeomTri(fileHandle handle, struct nugeom_s* geom, enum nuprimtype_e type);
void ReadNuIFFGeomSkin(fileHandle handle,struct nugeom_s *geom);
void ReadNuIFFBlendShape(fileHandle handle,struct nugeom_s *geom);
void ReadNuIFFUnknown(fileHandle handle, u32 blockMagic);
s32 ReadNuIFFInstSet(fileHandle fh, struct nuinstance_s** instances, struct nuinstanim_s** instanims);
void ReadNuIFFSpecialObjects(fileHandle fh,struct nugscn_s *gsc);
void NuSceneCalcCulling(struct nugscn_s *scene);
void ReadNuIFFGScene(fileHandle handle,struct nugscn_s *gscene);
struct nuscene_s * NuSceneLoad(s8 *filename);
s32 ReadNuIFFNameTable(fileHandle handle);

#endif // !NUSCENE_H
