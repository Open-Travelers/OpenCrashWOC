void cloudInit(variptr_u *buffer,variptr_u *buffend)

{
  nutex_s *cloudtex;
  numtl_s *mtl;
  nugobj_s *gobj;
  nugeom_s *g;
  nuprim_s *prim;
  NUERRORFUNC *e;
  numtlattrib_s attr;
  uint i;
  nuvtx_ps_s *vtx;
  double ptr;
  double n;
  float maxsize;
  float ptlSize;
  int stride;
  
  if (cloudmtl == (numtl_s *)0x0) {
    cloudtex = NuTexReadBitmap("gfx\\cloudslo.bmp");
    mtl = NuMtlCreate(1);
    cloudmtl = mtl;
    if (mtl != (numtl_s *)0x0) {
      attr = mtl->attrib;
      (mtl->diffuse).b = 1.0;
      mtl->alpha = 0.7;
      (mtl->diffuse).r = 1.0;
      (mtl->diffuse).g = 1.0;
      mtl->alpha_sort = 0;
      mtl->attrib = (numtlattrib_s)((uint)attr & 0xc00ffff | 0x81668000);
      stride = NuTexCreate(cloudtex);
      mtl = cloudmtl;
      cloudmtl->fxid = 'a';
      mtl->tid = stride;
      mtl = cloudmtl;
      ptlSize = PARTICLESIZE;
      cloudmtl->fx1 = (nufx_u)CLOUDRNG;
      mtl->fx2 = (nufx_u)ptlSize;
      NuMtlUpdate(mtl);
      NuMemFree(cloudtex->bits);
      NuMemFree(cloudtex);
    }
  }
  if (buffer != (variptr_u *)0x0) {
    NuMemSetExternal(buffer,buffend);
  }
  gobj = NuGobjCreate();
  cloudGobj = gobj;
  gobj->type = NUGOBJ_MESH;
  gobj->ngobjs = 1;
  g = NuGeomCreate();
  stride = (int)MAXCLOUDSIZE;
  cloudGobj->geom = g;
  prim = NuPrimCreate(stride,NUPT_POINT);
  gobj = cloudGobj;
  stride = (int)MAXCLOUDSIZE;
  cloudGobj->geom->prim = prim;
  NuGeomCreateVB(gobj->geom,stride,NUVT_PS,0);
  cloudGobj->geom->mtls = cloudmtl;
  if (buffer != (variptr_u *)0x0) {
    NuMemSetExternal((variptr_u *)0x0,(variptr_u *)0x0);
  }
  stride = NuVtxStride(cloudGobj->geom->vertex_type);
  vtx = (nuvtx_ps_s *)cloudGobj->geom->hVB;
  if (vtx == (nuvtx_ps_s *)0x0) {
    e = NuErrorProlog("C:/source/crashwoc/code/gamecode/cloudfx.c",0x4a);
    (*e)("cloudInit : Lock VB failed!");
  }
  i = 0;
  ptr = 4503601774854144.0;
  if (0.0 < MAXCLOUDSIZE) {
    do {
      ptlSize = NuRandFloat();
      i = i + 1;
      (vtx->pnt).x = (ptlSize * CLOUDRNG + ptlSize * CLOUDRNG) - CLOUDRNG;
      ptlSize = NuRandFloat();
      (vtx->pnt).y = (ptlSize * CLOUDRNG + ptlSize * CLOUDRNG) - CLOUDRNG;
      ptlSize = NuRandFloat();
      maxsize = MAXCLOUDSIZE;
      n = (double)CONCAT44(0x43300000,i ^ 0x80000000);
      ptlSize = (ptlSize * CLOUDRNG + ptlSize * CLOUDRNG) - CLOUDRNG;
      vtx->diffuse = -1;
      (vtx->pnt).z = ptlSize;
      vtx = (nuvtx_ps_s *)((int)&(vtx->pnt).x + stride);
    } while ((float)(n - ptr) < maxsize);
  }
  NuGobjCalcDims(cloudGobj);
  return;
}


void cloudRender(nuvec_s *target,nuvec_s *cloudoff)

{
  numtx_s m;
  nuvec_s camoff;
  nuvec_s off;
  double local_10;
  float RNG;
  
  NuVecScale(&off,cloudoff,CLOUDRNG);
  off.x = target->x + off.x;
  RNG = CLOUDRNG + CLOUDRNG;
  off.y = target->y + off.y;
  off.z = target->z + off.z;
  local_10 = (double)CONCAT44(0x43300000,(int)(off.x / RNG) ^ 0x80000000);
  camoff.x = off.x - RNG * (float)(local_10 - 4503601774854144.0);
  local_10 = (double)CONCAT44(0x43300000,(int)(off.y / RNG) ^ 0x80000000);
  camoff.y = off.y - RNG * (float)(local_10 - 4503601774854144.0);
  local_10 = (double)CONCAT44(0x43300000,(int)(off.z / RNG) ^ 0x80000000);
  camoff.z = off.z - RNG * (float)(local_10 - 4503601774854144.0);
  NuMtxSetIdentity(&m);
  m._30 = camoff.x;
  m._31 = camoff.y;
  m._32 = camoff.z;
  NuRndrGobj(cloudGobj,&m,(float **)0x0);
  return;
}


void InitClouds(void *supbuf_ptr,variptr_u *superbuf_end)

{
  int iVar1;
  uint uVar2;
  int i;
  int n;
  float *offZ;
  int *serateZ;
  int *haseZ;
  float fVar3;
  
  n = 0;
  srand(0x11);
  i = 0x14;
  cloudInit((variptr_u *)supbuf_ptr,superbuf_end);
  haseZ = &grphase[0].z;
  offZ = &groff[0].z;
  serateZ = &grphaserate[0].z;
  do {
    fVar3 = NuRandFloat();
    *(float *)((int)&groff[0].x + n) = fVar3;
    fVar3 = NuRandFloat();
    offZ[-1] = fVar3;
    fVar3 = NuRandFloat();
    *offZ = fVar3;
    iVar1 = rand();
    offZ = offZ + 3;
    *(int *)((int)&grphase[0].x + n) = iVar1 << 1;
    iVar1 = rand();
    haseZ[-1] = iVar1 << 1;
    iVar1 = rand();
    *haseZ = iVar1 << 1;
    uVar2 = rand();
    haseZ = haseZ + 3;
    *(uint *)((int)&grphaserate[0].x + n) = (uVar2 & 0xff) + 0x80;
    uVar2 = rand();
    n = n + 0xc;
    serateZ[-1] = (uVar2 & 0xff) + 0x80;
    uVar2 = rand();
    i = i + -1;
    *serateZ = (uVar2 & 0xff) + 0x80;
    serateZ = serateZ + 3;
  } while (i != 0);
  return;
}


void CloseClouds(void)

{
  if (cloudGobj != NULL) {
    NuGobjDestroy(cloudGobj);
    cloudGobj = NULL;
  }
  cloudmtl = NULL;
  return;
}


void DoClouds(int paused)

{
  float fVar1;
  float fVar2;
  float fVar3;
  numtx_s *m;
  int i;
  int iVar4;
  int iVar5;
  int n;
  double dVar6;
  double dVar7;
  float tmp;
  
  if (disable_clouds == 0) {
    NuCameraEnableClipping(0);
    dVar6 = 1.0;
    dVar7 = -1.0;
    i = 0;
    do {
      if (paused == 0) {
        n = grphase[i].x + grphaserate[i].x;
        iVar4 = grphase[i].y + grphaserate[i].y;
        iVar5 = grphase[i].z + grphaserate[i].z;
        fVar2 = *(float *)((int)NuTrigTable + (iVar4 * 4 & 0x3fffcU)) * grscale.y + grvel.y;
        tmp = groff[i].y;
        fVar1 = groff[i].z;
        fVar3 = *(float *)((int)NuTrigTable + (iVar5 * 4 & 0x3fffcU)) * grscale.z + grvel.z;
        groff[i].x = groff[i].x +
                     *(float *)((int)NuTrigTable + (n * 4 & 0x3fffcU)) * grscale.x + grvel.x;
        groff[i].y = tmp + fVar2;
        groff[i].z = fVar1 + fVar3;
        grphase[i].x = n;
        grphase[i].y = iVar4;
        grphase[i].z = iVar5;
      }
      n = i + 1;
      if (dVar6 < (double)groff[i].x) {
        do {
          tmp = groff[i].x - 2.0;
          groff[i].x = tmp;
        } while (1.0 < tmp);
      }
      if (dVar6 < (double)groff[i].y) {
        do {
          tmp = groff[i].y - 2.0;
          groff[i].y = tmp;
        } while (1.0 < tmp);
      }
      if (dVar6 < (double)groff[i].z) {
        do {
          tmp = groff[i].z - 2.0;
          groff[i].z = tmp;
        } while (1.0 < tmp);
      }
      if ((double)groff[i].x < dVar7) {
        do {
          tmp = groff[i].x + 2.0;
          groff[i].x = tmp;
        } while (tmp < -1.0);
      }
      if ((double)groff[i].y < dVar7) {
        do {
          tmp = groff[i].y + 2.0;
          groff[i].y = tmp;
        } while (tmp < -1.0);
      }
      if ((double)groff[i].z < dVar7) {
        do {
          tmp = groff[i].z + 2.0;
          groff[i].z = tmp;
        } while (tmp < -1.0);
      }
      m = NuCameraGetMtx();
      cloudRender((nuvec_s *)&m->_30,groff + i);
      i = n;
    } while (n < 0x14);
    NuCameraEnableClipping(1);
  }
  return;
}
