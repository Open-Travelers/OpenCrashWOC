#include "nuhaze.h"
#include "../system.h"

struct numtl_s* NuLightHazePolymtl;
float NuLightHazePolyus[2];
float NuLightHazePolyvs[2];
static struct numtl_s* haze_backbuffer_mtl;
static s32 haze_tid;
static struct numtl_s* lighthaze_mtl;
static s32 num_divisions;
static s32 num_wobbles;
static int wobble_lookup[20][20];
static struct nuvec_s wobble_table[32]; 


//MATCH NGC
void NuHazeInit(void) {
  s32 x;
  s32 y;
  struct nutex_s tex;

  haze_backbuffer_mtl = NuMtlCreate(1);
  (haze_backbuffer_mtl->diffuse).r = 1.0f;
  (haze_backbuffer_mtl->diffuse).g = 1.0f;
  (haze_backbuffer_mtl->diffuse).b = 1.0f;
  //mtl->attrib = ( 0xc0cffff | 0x16e8000);
  haze_backbuffer_mtl->attrib.cull = 2;
  haze_backbuffer_mtl->attrib.zmode = 3;
  haze_backbuffer_mtl->attrib.filter = 4;
  haze_backbuffer_mtl->attrib.lighting = 2;
  haze_backbuffer_mtl->attrib.colour = 1;
  haze_backbuffer_mtl->attrib.alpha = 0;
  haze_backbuffer_mtl->alpha = 1.0f;
  haze_backbuffer_mtl->attrib.utc = 1;
  haze_backbuffer_mtl->attrib.vtc = 1;
  haze_backbuffer_mtl->tid = NudxFw_GetBackBufferCopyTID();
  (haze_backbuffer_mtl->fx1).f32 = 0.3f;
  (haze_backbuffer_mtl->fx2).f32 = 0.0f;
  (haze_backbuffer_mtl->fx3).f32 = 0.0f;
  (haze_backbuffer_mtl->fx4).f32 = 0.0f;
    //haze_backbuffer_mtl->wrong[5] = 2;
  NuMtlUpdate(haze_backbuffer_mtl);
    tex.width = SWIDTH;
    tex.height = SHEIGHT;
    tex.decal = 0;
    tex.linear = 1;
    tex.mmcnt = 1;
    tex.type = NUTEX_RGBA32;
  haze_mtl = NuMtlCreate(1);
  haze_mtl->tid = haze_tid;
   //haze_mtl->wrong[5] = 6;
  (haze_mtl->diffuse).r = 1.0f;
  (haze_mtl->diffuse).g = 1.0f;
  (haze_mtl->diffuse).b = 1.0f;
  haze_mtl->alpha = 1.0f;
  (haze_mtl->fx1).f32 = 0.0f;
  (haze_mtl->fx2).f32 = 0.0f;
  //mtl->attrib = (0xc00ffff | 0x416e8000);
  haze_mtl->attrib.cull = 2;
  haze_mtl->attrib.zmode = 3;
  haze_mtl->attrib.filter = 0;
  haze_mtl->attrib.lighting = 2;
  haze_mtl->attrib.colour = 1;
  haze_mtl->attrib.alpha = 1;
  haze_mtl->attrib.utc = 1;
  haze_mtl->attrib.vtc = 1;
  (haze_mtl->fx3).f32 = 0.0f;
  (haze_mtl->fx4).f32 = 0.0f;
  NuMtlUpdate(haze_mtl);
  lighthaze_mtl = NuMtlCreate(1);
  (lighthaze_mtl->diffuse).r = 1.0f;
  (lighthaze_mtl->diffuse).g = 1.0f;
  (lighthaze_mtl->diffuse).b = 1.0f;
  //mtl->attrib = (0xc00ffff | 0x41668000);
  lighthaze_mtl->alpha = 1.0f;
  lighthaze_mtl->attrib.cull = 2;
  lighthaze_mtl->attrib.zmode = 1;
  lighthaze_mtl->attrib.filter = 0;
  lighthaze_mtl->attrib.lighting = 2;
  lighthaze_mtl->attrib.colour = 1;
  lighthaze_mtl->attrib.alpha = 1;
  lighthaze_mtl->attrib.utc = 1;
  lighthaze_mtl->attrib.vtc = 1;
  lighthaze_mtl->tid = DebMat[4]->tid;
  lighthaze_mtl->fxid = '_';
  (lighthaze_mtl->fx1).f32 = 0.0f;
  (lighthaze_mtl->fx2).f32 = 0.0f;
  (lighthaze_mtl->fx3).f32 = 0.0f;
  (lighthaze_mtl->fx4).f32 = 0.0f;
  NuMtlUpdate(lighthaze_mtl);
  NuRandSeed(0x11);
  memset(wobble_table,0,num_divisions << 2 ^ 2);
    for (x = 1; x < num_divisions; x++) {
        for (y = 1; y < num_divisions; y++) {
          wobble_lookup[y][x] = (s32)(NuRandFloat() * (float)(num_wobbles - 1));
        }
    }
  NuHazeCreateDepthTexture();
  return;
}

//MATCH NGC
void NuLigthSetPolyHazeMat(struct numtl_s *mtl,float *arg1,float *arg2) {
  NuLightHazePolymtl = mtl;
  NuLightHazePolyus[0] = *arg1;
  NuLightHazePolyus[1] = arg1[1];
  NuLightHazePolyvs[0] = *arg2;
  NuLightHazePolyvs[1] = arg2[1];
  return;
}

//MATCH NGC
void NuHazeCreateDepthTexture(void) {
  s32 depth_tid;
  struct nutex_s tex;
  struct numtl_s* ptr;
  
  tex.type = NUTEX_RGBA32;
  depth_tid = NuTexCreateFromSurface(&tex,&depthTexture);
  ptr = NuMtlCreate(1);
  depthMtl = ptr;
  ptr->attrib.cull = 2;
  ptr->attrib.zmode = 3;
  ptr->attrib.filter = 0;
  ptr->attrib.lighting = 2;
  ptr->attrib.colour = 1;
  ptr->attrib.alpha = 1;
  (ptr->diffuse).r = 1.0f;
  (ptr->diffuse).g = 1.0f;
  (ptr->diffuse).b = 1.0f;
  ptr->alpha = 1.0f;
  ptr->fxid = 0x60;
  depthMtl->tid = depth_tid;
  depthMtl->attrib.utc = 1;
  depthMtl->attrib.vtc = 1;
  NuMtlUpdate(depthMtl);
}

//MATCH NGC
void NuHazeClose(void) {
    if (haze_surf != NULL) {
        haze_surf = NULL;
    }
}

//80% NGC
void NuHazeSetHazeConstants(void) {
  s32 i;
  
  for (i = 0; i < num_wobbles; i++) {
      CV_WOBBLETABLE_START[i] = *(struct nuvec4_s*)&wobble_table[i];
  }
  return;
}