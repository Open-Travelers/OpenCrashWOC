#include "../system.h"

void AddQuad3DrotXYZ(struct nuvec_s *pos,struct nuvec_s *shape,struct numtl_s *mtl,struct nuangvec_s *a,float *uvs,u32 colou r)
{
  s32 outcode;
  struct numtx_s mtx;
  struct nuvtx_tc1_s vtx [4];
  
  vtx[0].diffuse =
       (colour & 0xff000000) + (colour & 0xff) * 0x10000 + (colour & 0xff00) +
       (colour >> 0x10 & 0xff);
  outcode = NuCameraClipTestPoints(pos,1,NULL);
  if (outcode == 0) {
    NuMtxSetRotateXYZ(&mtx,a);
    NuMtxTranslate(&mtx,pos);
    vtx[0].pnt.x = shape->x;
    vtx[0].pnt.y = shape->y;
    vtx[0].pnt.z = shape->z;
    vtx[0].tc[1] = uvs[1];
    vtx[1].pnt.x = shape[1].x;
    vtx[1].pnt.y = shape[1].y;
    vtx[1].pnt.z = shape[1].z;
    vtx[1].tc[0] = uvs[2];
    vtx[1].tc[1] = uvs[3];
    vtx[2].pnt.x = shape[2].x;
    vtx[0].tc[0] = *uvs;
    vtx[2].pnt.y = shape[2].y;
    vtx[0].nrm.x = 1.0f;
    vtx[0].nrm.y = 0.0f;
    vtx[0].nrm.z = 0.0f;
    vtx[1].nrm.x = 1.0f;
    vtx[1].nrm.y = 0.0f;
    vtx[1].nrm.z = 0.0f;
    vtx[3].pnt.z = shape[3].z;
    vtx[3].tc[1] = uvs[7];
    vtx[2].pnt.z = shape[2].z;
    vtx[2].tc[0] = uvs[4];
    vtx[2].tc[1] = uvs[5];
    vtx[3].pnt.x = shape[3].x;
    vtx[3].pnt.y = shape[3].y;
    vtx[3].tc[0] = uvs[6];
    vtx[3].nrm.x = 1.0f;
    vtx[3].nrm.z = 0.0f;
    vtx[2].nrm.x = 1.0f;
    vtx[2].nrm.y = 0.0f;
    vtx[2].nrm.z = 0.0f;
    vtx[3].nrm.y = 0.0f;
    vtx[1].diffuse = vtx[0].diffuse;
    vtx[2].diffuse = vtx[0].diffuse;
    vtx[3].diffuse = vtx[0].diffuse;
    NuRndrTri3d(vtx,mtl,&mtx);
    NuRndrTri3d(vtx + 1,mtl,&mtx);
  }
  return;
}

void DrawMask(struct mask *mask)
{
/*	DWARF INFO

	CharacterModel *model; //
    numtx_s mM; //
    numtx_s mS; //
    float** dwa; //
    int i; 
*/
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

void LoadWaterCausticTextures(void)
{
  void *mem;
  struct nutex_s *tex;
  s32 i;
  s32 *causticTx;
  s32 n;
  char bmpname [24];
  
  mem = malloc_x(0x1000);
  tex = (struct nutex_s *)malloc_x(0x1c);
  tex->height = 0x40;
  tex->decal = 0;
  tex->mmcnt = 1;
  causticTx = CausticTextures;
  tex->pal = NULL;
  tex->type = NUTEX_RGB24;
  tex->width = 0x40;
  tex->bits = mem;
  iss3cmp = 0x80c;
  i = 0;
  do {
    n = i + 1;
    sprintf(bmpname,"gfx\\caust%d.s3",i);
    NuFileLoadBuffer(bmpname,mem,0x80c);
    i = NuTexCreate(tex);
    *causticTx = i;
    causticTx = causticTx + 1;
    i = n;
  } while (n < 0x20);
  iss3cmp = 0;
  free_x(tex);
  free_x(mem);
  return;
}

void RemoveWaterCausticTextures(void)
{
  int tid;
  int *i;
  
  i = CausticTextures;
  do {
    tid = *i;
    i = i + 1;
    NuTexDestroy(tid);
  } while ((int)i < sizeof(CausticTextures));
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


