#include "glutils.h"


char* balls;
int iss3cmp;
int DoNotSiwzzle;

numtl_s * CreateAlphaBlendTexture256_32(char *fname,int uvmode,int alpha,int pri,int zmode)		//CHECK

{
  numtlattrib_s attr;
  numtl_s *mtl;
  int t;
  nutex_s tex;
  
  balls = (char *)malloc_x(0x40000);
  NuFileLoadBuffer(fname,balls,0x40000);
  tex.type = NUTEX_RGBA32;
  tex.height = 0x100;
  tex.mmcnt = 1;
  tex.width = 0x100;
  tex.bits = balls;
  mtl = NuMtlCreate(1);
  attr = mtl->attrib;
  (mtl->diffuse).b = 1.0;
  mtl->alpha = 0.999;
  attr = (numtlattrib_s)(alpha << 0x1e | (zmode & 3U) << 0x12 | (uint)attr & 0x3fc3ffff | 0x200000 );
  (mtl->diffuse).r = 1.0;
  (mtl->diffuse).g = 1.0;
  mtl->attrib = attr;
  if (uvmode != 0) {
    mtl->attrib = (numtlattrib_s)((uint)attr | 1);
  }
  t = NuTexCreate(&tex);
  mtl->tid = t;
  mtl->alpha_sort = (short)pri;
  mtl->attrib = (numtlattrib_s)((uint)mtl->attrib & 0xfffc7fff | 0x20000);
  NuMtlUpdate(mtl);
  free_x(balls);
  return mtl;
}


numtl_s * CreateAlphaBlendTexture64(char *fname,int uvmode,int alpha,int pri)		//CHECK

{
  numtlattrib_s attr;
  numtl_s *mtl;
  int t;
  nutex_s tex;
  
  balls = (char *)malloc_x(0x4000);
  NuFileLoadBuffer(fname,balls,0x4000);
  tex.height = 0x40;
  tex.type = NUTEX_RGBA32;
  tex.bits = balls;
  tex.width = 0x40;
  tex.mmcnt = 0;
  mtl = NuMtlCreate(1);
  attr = mtl->attrib;
  (mtl->diffuse).b = 1.0;
  mtl->alpha = 0.999;
  (mtl->diffuse).r = 1.0;
  attr = (numtlattrib_s)(alpha << 0x1e | (uint)attr & 0x3fc4ffff | 0x260000);
  (mtl->diffuse).g = 1.0;
  mtl->attrib = attr;
  if (uvmode != 0) {
    mtl->attrib = (numtlattrib_s)((uint)attr | 1);
  }
  t = NuTexCreate(&tex);
  mtl->tid = t;
  mtl->alpha_sort = (short)pri;
  mtl->attrib = (numtlattrib_s)((uint)mtl->attrib & 0xfffcffff);
  NuMtlUpdate(mtl);
  free_x(balls);
  return mtl;
}

numtl_s * CreateAlphaBlendTexture256128_32(char *fname,int uvmode,int alpha,int pri,int zmode)		//CHECK

{
  numtlattrib_s attr;
  numtl_s *mtl;
  int tpid;
  nutex_s tex;
  
  balls = (char *)malloc_x(0x1000c);
  NuFileLoadBuffer(fname,balls,0x1000c);
  iss3cmp = 0x200c;
  tex.type = NUTEX_RGBA32;
  tex.mmcnt = 1;
  DoNotSiwzzle = 1;
  tex.height = 0x80;
  tex.bits = balls;
  tex.width = 0x80;
  mtl = NuMtlCreate(1);
  attr = mtl->attrib;
  (mtl->diffuse).b = 1.0;
  mtl->alpha = 0.999;
  attr = (numtlattrib_s)(alpha << 0x1e | (zmode & 3U) << 0x12 | (uint)attr & 0x3fc3ffff | 0x200000 );
  (mtl->diffuse).r = 1.0;
  (mtl->diffuse).g = 1.0;
  mtl->attrib = attr;
  if (uvmode != 0) {
    mtl->attrib = (numtlattrib_s)((uint)attr | 1);
  }
  tpid = NuTexCreate(&tex);
  mtl->tid = tpid;
  mtl->alpha_sort = (short)pri;
  mtl->attrib = (numtlattrib_s)((uint)mtl->attrib & 0xfffc7fff | 0x20000);
  NuMtlUpdate(mtl);
  free_x(balls);
  iss3cmp = 0;
  DoNotSiwzzle = 0;
  return mtl;
}