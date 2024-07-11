#include "glutils.h"


char* balls;
extern s32 iss3cmp; //main.c
s32 DoNotSiwzzle;

//NGC MATCH
struct numtl_s * CreateAlphaBlendTexture256_32(char *fname,s32 uvmode,s32 alpha,s32 pri,s32 zmode) {
  struct numtl_s *mtl;
  struct nutex_s tex;
  
  balls = (char *)malloc_x(0x40000);
  NuFileLoadBuffer(fname,balls,0x40000);
  tex.type = NUTEX_RGBA32;
  tex.bits = balls;
  tex.width = 0x100;
  tex.height = 0x100;
  tex.mmcnt = 1;
  mtl = NuMtlCreate(1);
  //(alpha << 0x1e | (zmode & 3U) << 0x12 | (u32)mtl->attrib & 0x3fc3ffff | 0x200000);
  mtl->attrib.cull = 2;
  mtl->attrib.zmode = zmode;
  mtl->diffuse.r = 1.0f;
  mtl->diffuse.g = 1.0f;
  mtl->diffuse.b = 1.0f;
  mtl->alpha = 0.999f;
  mtl->attrib.alpha = alpha;
  if (uvmode != 0) {
    mtl->attrib.uvmode = 1;
  }
  mtl->tid = NuTexCreate(&tex);
  mtl->alpha_sort = (short)pri;
  //((u32)mtl->attrib & 0xfffc7fff | 0x20000);
  mtl->attrib.lighting = 2;
  mtl->attrib.colour = 0;
  NuMtlUpdate(mtl);
  free_x(balls);
  tex.bits = 0;
  return mtl;
}

//NGC MATCH
struct numtl_s * CreateAlphaBlendTexture64(char *fname,s32 uvmode,s32 alpha,s32 pri) {
  struct numtl_s *mtl;
  struct nutex_s tex;
  
  balls = (char *)malloc_x(0x4000);
  NuFileLoadBuffer(fname,balls,0x4000);
  tex.type = NUTEX_RGBA32;
  tex.bits = balls;
  tex.width = 0x40;
  tex.height = 0x40;
  tex.mmcnt = 0;
  mtl = NuMtlCreate(1);
  //(alpha << 0x1e | (u32)mtl->attrib & 0x3fc4ffff | 0x260000);
  mtl->attrib.cull = 2;
  mtl->attrib.zmode = 1;
  mtl->attrib.lighting = 2;
  mtl->diffuse.r = 1.0f;
  mtl->diffuse.g = 1.0f;
  mtl->diffuse.b = 1.0f;
  mtl->alpha = 0.999f;
  mtl->attrib.alpha = alpha;
  if (uvmode != 0) {
    mtl->attrib.uvmode = 1;
  }
  mtl->tid = NuTexCreate(&tex);
  mtl->alpha_sort = (short)pri;
  //((u32)mtl->attrib & 0xfffcffff);
  mtl->attrib.lighting = 0;
  NuMtlUpdate(mtl);
  free_x(balls);
  return mtl;
}

//NGC MATCH
struct numtl_s * CreateAlphaBlendTexture256128_32(char *fname,s32 uvmode,s32 alpha,s32 pri,s32 zmode) {
  struct numtl_s *mtl;
  struct nutex_s tex;
  
  balls = (char *)malloc_x(0x1000c);
  NuFileLoadBuffer(fname,balls,0x1000c);
  iss3cmp = 0x200c;
  tex.type = NUTEX_RGBA32;
  DoNotSiwzzle = 1;
  tex.mmcnt = 1;
  tex.width = 0x80;
  tex.height = 0x80;
  tex.bits = balls;
  mtl = NuMtlCreate(1);
  //(alpha << 0x1e | (zmode & 3U) << 0x12 | (u32)mtl->attrib & 0x3fc3ffff | 0x200000);
  mtl->attrib.cull = 2;
  mtl->attrib.zmode = zmode;
  mtl->diffuse.r = 1.0f;
  mtl->diffuse.g = 1.0f;
  mtl->diffuse.b = 1.0f;
  mtl->alpha = 0.999f;
  mtl->attrib.alpha = alpha;
  if (uvmode != 0) {
      mtl->attrib.uvmode = 1;
  }
  mtl->tid = NuTexCreate(&tex);
  mtl->alpha_sort = (short)pri;
  //((u32)mtl->attrib & 0xfffc7fff | 0x20000);
  mtl->attrib.lighting = 2;
  mtl->attrib.colour = 0;
  NuMtlUpdate(mtl);
  free_x(balls);
  tex.bits = 0;
  DoNotSiwzzle = 0;
  iss3cmp = 0;
  return mtl;
}