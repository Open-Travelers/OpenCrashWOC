float CLOUDRNG;
float MAXCLOUDSIZE;
float PARTICLESIZE;
struct nugobj_s* cloudGobj;
struct numtl_s* cloudmtl;
static s32 disable_clouds;
struct nuvec_s groff[20];
struct nuivec_s grphase[20];
struct nuivec_s grphaserate[20];
struct nuvec_s grscale;
struct nuvec_s grvel;

//NGC MATCH
void cloudInit(union variptr_u *buffer,union variptr_u *buffend) {
  s32 i = 0;
  s32 stride;
  struct nuvtx_ps_s *vtx;
  struct nutex_s *cloudtex;
  float rndm;
  s32 diffuse;
    
  diffuse = -1;
  if (cloudmtl == NULL) {
    cloudtex = NuTexReadBitmap("gfx\\cloudslo.bmp");
    cloudmtl = NuMtlCreate(1);
    if (cloudmtl != NULL) {
      (cloudmtl->diffuse).r = 1.0f;
      (cloudmtl->diffuse).g = 1.0f;
      (cloudmtl->diffuse).b = 1.0f;
      cloudmtl->alpha_sort = 0;
      //cloudmtl->attrib = (cloudmtl->attrib & 0xc00ffff | 0x81668000);
      cloudmtl->attrib.cull = 2;
      cloudmtl->attrib.zmode = 1;
      cloudmtl->attrib.filter = 0;
      cloudmtl->attrib.lighting = 2;
      cloudmtl->attrib.alpha = 2;
      cloudmtl->attrib.colour = 1;
      cloudmtl->alpha = 0.7f;
      cloudmtl->attrib.utc = 1;
      cloudmtl->attrib.vtc = 1;
      cloudmtl->tid = NuTexCreate(cloudtex);
      cloudmtl->fxid = 0x61;
      (cloudmtl->fx1).f32 = CLOUDRNG;
      (cloudmtl->fx2).f32 = PARTICLESIZE;
      NuMtlUpdate(cloudmtl);
      NuMemFree(cloudtex->bits);
      NuMemFree(cloudtex);
    }
  }
  if (buffer != NULL) {
    NuMemSetExternal(buffer,buffend);
  }
  cloudGobj = NuGobjCreate();
  cloudGobj->ngobjs = 1;
  cloudGobj->type = NUGOBJ_MESH;
  cloudGobj->geom = NuGeomCreate();
  cloudGobj->geom->prims = NuPrimCreate(MAXCLOUDSIZE,NUPT_POINT);
  NuGeomCreateVB(cloudGobj->geom,(s32)MAXCLOUDSIZE,NUVT_PS,0);
  cloudGobj->geom->mtl = cloudmtl;
  if (buffer != NULL) {
    NuMemSetExternal(NULL,NULL);
  }
  stride = NuVtxStride(cloudGobj->geom->vtxtype);
  vtx = (struct nuvtx_ps_s *)cloudGobj->geom->hVB;
  if (vtx == NULL) {
    NuErrorProlog("C:/source/crashwoc/code/gamecode/cloudfx.c",0x4a)("cloudInit : Lock VB failed!");
  }
  
  for(i = 0; i < MAXCLOUDSIZE; i++, (s32)vtx += stride) {
      rndm = NuRandFloat();
      (vtx->pnt).x = (rndm * CLOUDRNG + rndm * CLOUDRNG) - CLOUDRNG;
      rndm = NuRandFloat();
      (vtx->pnt).y = (rndm * CLOUDRNG + rndm * CLOUDRNG) - CLOUDRNG;
      rndm = NuRandFloat();
      rndm = (rndm * CLOUDRNG + rndm * CLOUDRNG) - CLOUDRNG;
      vtx->diffuse = diffuse;
      (vtx->pnt).z = rndm;
  }
  NuGobjCalcDims(cloudGobj);
  return;
}

//NGC MATCH
void cloudRender(struct nuvec_s *target,struct nuvec_s *cloudoff) {
  struct numtx_s m;
  struct nuvec_s camoff;
  struct nuvec_s off;
  
  NuVecScale(&off,cloudoff,CLOUDRNG);
  camoff.x = target->x + off.x;
  camoff.y = target->y + off.y;
  camoff.z = target->z + off.z;
  camoff.x = camoff.x - ((2.0f * CLOUDRNG) * (s32)(camoff.x / (2.0f * CLOUDRNG)));
  camoff.y = camoff.y - ((2.0f * CLOUDRNG) * (s32)(camoff.y / (2.0f * CLOUDRNG)));
  camoff.z = camoff.z - ((2.0f * CLOUDRNG) * (s32)(camoff.z / (2.0f * CLOUDRNG)));
  NuMtxSetIdentity(&m);
  m._30 = camoff.x;
  m._31 = camoff.y;
  m._32 = camoff.z;
  NuRndrGobj(cloudGobj,&m,NULL);
  return;
}

//NGC MATCH
void InitClouds(union variptr_u *buffer,union variptr_u *buffend) {
  s32 n;
  
  srand(0x11);
  cloudInit(buffer,buffend);
  for(n = 0; n < 0x14; n++) {
    groff[n].x = NuRandFloat();
    groff[n].y = NuRandFloat();
    groff[n].z = NuRandFloat();
    grphase[n].x = rand() << 1;
    grphase[n].y = rand() << 1;
    grphase[n].z = rand() << 1;
    grphaserate[n].x = (rand() & 0xff) + 0x80;
    grphaserate[n].y = (rand() & 0xff) + 0x80;
    grphaserate[n].z = (rand() & 0xff) + 0x80;
  }
  return;
}

//NGC MATCH
void CloseClouds() {
  if (cloudGobj != NULL) {
    NuGobjDestroy(cloudGobj);
    cloudGobj = NULL;
  }
  cloudmtl = NULL;
  return;
}

//NGC MATCH
void DoClouds(s32 paused) {
  struct numtx_s *m;
  s32 i;
  
  if (disable_clouds == 0) {
    NuCameraEnableClipping(0);
    for(i = 0; i < 0x14; i++) {
      if (paused == 0) {
        grphase[i].x += grphaserate[i].x;
        grphase[i].y += grphaserate[i].y;
        grphase[i].z += grphaserate[i].z;
        groff[i].x += NuTrigTable[grphase[i].x & 0xFFFF] * grscale.x + grvel.x;
        groff[i].y += NuTrigTable[grphase[i].y & 0xFFFF] * grscale.y + grvel.y;
        groff[i].z += NuTrigTable[grphase[i].z & 0xFFFF] * grscale.z + grvel.z;
      }
      while (1.0f < groff[i].x) {
          groff[i].x = groff[i].x - 2.0f;
      }
      while (1.0f < groff[i].y) {
          groff[i].y = groff[i].y - 2.0f;
      }
      while (1.0f < groff[i].z) {
          groff[i].z = groff[i].z - 2.0f;
      }
      while (groff[i].x < -1.0f) {
          groff[i].x = groff[i].x + 2.0f;
      }
      while (groff[i].y < -1.0f) {
          groff[i].y = groff[i].y + 2.0f;
      }
      while (groff[i].z < -1.0f) {
          groff[i].z = groff[i].z + 2.0f;
      }
      m = NuCameraGetMtx();
      cloudRender((struct nuvec_s *)&m->_30,&groff[i]);
    }
    NuCameraEnableClipping(1);
  }
  return;
}