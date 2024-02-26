#include "../system.h"
#include "nu.h"
#include "gamecode/creature.h"
#include "gamecode/main.h"
#include "gamecode/game.h"

s32 VEHICLECONTROL; //vehicle.c

//NGC MATCH
void AddQuad3DrotXYZ(struct nuvec_s *pos,struct nuvec_s *shape,struct numtl_s *mtl,struct nuangvec_s *a,float *uvs,u32 colour)
{
  s32 outcode;
  struct numtx_s mtx;
  u32 col;
  struct nuvtx_tc1_s pt[4];

  col = (colour & 0xff000000) + (colour & 0xff) * 0x10000 + (colour & 0xff00) + (colour >> 0x10 & 0xff);
  outcode = NuCameraClipTestPoints(pos,1,NULL);
  if (outcode == 0) {
    NuMtxSetRotateXYZ(&mtx,a);
    NuMtxTranslate(&mtx,pos);
    pt[0].pnt.x = shape->x;
    pt[0].pnt.y = shape->y;
    pt[0].pnt.z = shape->z;
    pt[0].nrm.x = 1.0f;
    pt[0].nrm.y = 0.0f;
    pt[0].nrm.z = 0.0f;
    pt[0].tc[0] = *uvs;
    pt[0].tc[1] = uvs[1];
    pt[1].pnt.x = shape[1].x;
    pt[1].pnt.y = shape[1].y;
    pt[1].pnt.z = shape[1].z;
    pt[1].tc[0] = uvs[2];
    pt[1].tc[1] = uvs[3];
    pt[1].nrm.x = 1.0f;
    pt[1].nrm.y = 0.0f;
    pt[1].nrm.z = 0.0f;
    pt[0].diffuse = col;
    pt[1].diffuse = pt[0].diffuse;
    pt[2].pnt.x = shape[2].x;
    pt[2].pnt.y = shape[2].y;
    pt[2].pnt.z = shape[2].z;
    pt[2].tc[0] = uvs[4];
    pt[2].tc[1] = uvs[5];
    pt[2].diffuse = pt[0].diffuse;
    pt[2].nrm.x = 1.0f;
    pt[2].nrm.y = 0.0f;
    pt[2].nrm.z = 0.0f;
    pt[3].pnt.x = shape[3].x;
    pt[3].pnt.y = shape[3].y;
    pt[3].pnt.z = shape[3].z;
    pt[3].diffuse = pt[0].diffuse;
    pt[3].tc[0] = uvs[6];
    pt[3].tc[1] = uvs[7];
    pt[3].nrm.x = 1.0f;
    pt[3].nrm.y = 0.0f;
    pt[3].nrm.z = 0.0f;
    NuRndrTri3d(pt,mtl,&mtx);
    NuRndrTri3d(&pt[1],mtl,&mtx);
  }
  return;
}

extern s32 LIGHTMASK; //game.c
extern s32 USELIGHTS; //main.c
extern signed char CRemap[191]; //creature.c
extern struct CharacterModel CModel[49]; //creature.c
extern struct creature_s* player; //creature.c
extern struct numtx_s tmtx[256];

//NGC MATCH
void DrawMask(struct mask_s *mask) {
    struct CharacterModel *model;
    struct numtx_s mM;
    struct numtx_s mS;
    float **dwa;
    s32 i;

    if (mask->active != 0) {
        i = (s32)CRemap[mask->character];
        if (i != -1) {
            model = &CModel[i];
            mM = mask->mM;
            mS = mask->mS;
            EvalModelAnim(model,&mask->anim,&mM,tmtx,&dwa,NULL);
            if ((USELIGHTS != 0) && (LIGHTMASK != 0)) {
                SetLights(&(mask->lights).pDir1st->Colour, &(mask->lights).pDir1st->Direction,
                          &(mask->lights).pDir2nd->Colour,&(mask->lights).pDir2nd->Direction,
                          &(mask->lights).pDir3rd->Colour,
                          &(mask->lights).pDir3rd->Direction,&(mask->lights).AmbCol);
            }
            NuHGobjRndrMtxDwa(model->hobj,&mM,1,NULL,tmtx,dwa);
            if (mask->shadow != 2000000.0f) {
                if (mask->reflect != '\0') {
                    mM._01 = -mM._01;
                    mM._11 = -mM._11;
                    mM._21 = -mM._21;
                    mM._31 = mask->shadow - (mM._31 - mask->shadow);
                    NuHGobjRndrMtxDwa(model->hobj,&mM,1,NULL,tmtx,dwa);
                }
                if (((((LDATA->flags & 0x1000) == 0) && (VEHICLECONTROL != 2)) &&
                    ((VEHICLECONTROL != 1 || ((player->obj).vehicle != 0x20)))) &&
                   (model->shadhdr == 0)) {
                    NuMtlSetStencilRender(NUSTENCIL_REPLACE_NODRAW);
                    NuHGobjRndrMtxDwa(model->hobj,&mS,1,NULL,tmtx,dwa);
                    NuMtlSetStencilRender(NUSTENCIL_NOSTENCIL);
                }
            }
            if ((USELIGHTS != 0) && (LIGHTMASK != 0)) {
                //SetLevelLights();
            }
        }
    }
    return;
}

struct nuvec_s tempvec;
struct nuvec4_s tempvec4;

void* makenuvec(float x,float y,float z)
{
  tempvec.x = x;
  tempvec.y = y;
  tempvec.z = z;
  return &tempvec;
}

void * makenuvec4(float x,float y,float z,float w)
{
  tempvec4.x = x;
  tempvec4.y = y;
  tempvec4.z = z;
  tempvec4.w = w;
  return &tempvec4;
}

s32 CausticTextures[32];

//NGC MATCH
void LoadWaterCausticTextures(void) {
  s32 i;
  char bmpname [24];
  struct nutex_s *tex;
  char *gerbils;
  
  gerbils = (char *) malloc_x(0x1000);
  tex = (struct nutex_s *)malloc_x(0x1c);
  tex->width = 0x40;
  tex->height = 0x40;
  tex->bits = gerbils;
  tex->decal = 0;
  tex->mmcnt = 1;
  tex->pal = NULL;
  tex->type = NUTEX_RGB24;
    
  iss3cmp = 0x80c;
  for (i = 0; i < 32; i++) {
    sprintf(bmpname,"gfx\\caust%d.s3",i);
    NuFileLoadBuffer(bmpname,gerbils,0x80c);
    CausticTextures[i] = NuTexCreate(tex);
  }
  iss3cmp = 0;
    
  free_x(tex);
  free_x(gerbils);
  return;
}

//NGC MATCH
void RemoveWaterCausticTextures(void) {
  s32 i;

  for (i = 0; i < 32; i++) {
    NuTexDestroy(CausticTextures[i]);
  }
  return;
}


struct nuscene_s * NuSceneReader(union variptr_u *buffer,union variptr_u *buffend,char *filename)
{
  struct nuscene_s *data;

  if (buffer != NULL) {
    NuMemSetExternal(buffer,buffend);
  }
  data = InstSceneLoad(filename);
  if (buffer != NULL) {
    NuMemSetExternal(NULL,NULL);
  }
  return data;
}

struct nuscene_s * NuSceneReader2(union variptr_u *buffer,union variptr_u *buffend,char *file)
{
  struct nuscene_s *scene;

  if (buffer != NULL) {
    NuMemSetExternal(buffer,buffend);
  }
  scene = NuSceneLoad(file);
  if (buffer != NULL) {
    NuMemSetExternal(NULL,NULL);
  }
  return scene;
}


