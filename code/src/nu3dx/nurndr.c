#include "nurndr.h"
#include "../system.h"

#define PI 3.1415927f
#define MAX_FIXED_POINT 65536
#define DEG_TO_FIXED_POINT (MAX_FIXED_POINT * (1 / (2 * PI)))

s32 GS_Parallax;
static struct nugeomitem_s geomitem[2048];
static s32 geomitem_cnt;
static s32 hgobj_enabled;
struct numtx_s mtx_array2HGobjRndrDwa[256];
struct numtx_s mtx_array2HGobj[256];
static struct nuvtx_tc1_s vtx_270[4];
static struct nuvtx_tc1_s* vtx2_271[4];

//MATCH GCN
void NuRndrInit(void) {
      s32 lp;

      lp = 0x20;
      NuRndrShadowCnt = 0;
      do {
        NuRndrWaterRipDat[lp - 1].timer = 0;
        lp--;
      } while (lp != 0);
      return;
}

//MATCH GCN
s32 NuRndrBeginScene(s32 hRT) {
  u32 bs;

  if (rndrmtx_cnt_max > rndrmtx_cnt) {
    rndrmtx_cnt_max = rndrmtx_cnt;
  }
  if (geomitem_cnt_max > geomitem_cnt) {
    geomitem_cnt_max = geomitem_cnt;
  }
  rndrmtx_cnt = 0x600;
  rndr_blend_shape_deformer_wt_cnt = 0x400;
  geomitem_cnt = 0x800;
  rndr_blend_shape_deformer_wt_ptrs_cnt = 0x800;
  NuMtlClearOt();
  bs = NudxFw_BeginScene(hRT);
  return ~bs >> 0x1f;
}

//MATCH GCN
void NuRndrEndScene(void) {
  if (GS_Parallax != 0) {
    GS_SetZCompare(0,0,GX_NEVER);
  }
  NuRndrFlush();
  GS_EndScene();
  return;
}

//MATCH GCN
void NuRndrFlush(void) {
    NuMtlRender();
    NuLightClearStoredLights();
}

//MATCH GCN
void NuRndrClear(s32 flags,s32 colour,f32 depth)
{
  s32 fwf;

    fwf = 0;
  if ((flags & 1U) != 0) {
    fwf = 0xf0;
  }
  if ((flags & 2U) != 0) {
    fwf = fwf | 1;
  }
  if ((flags & 8U) != 0) {
    fwf = fwf | 2;
  }
  NudxFw_Clear(fwf,colour,depth);
  return;
}

//MATCH GCN
void NuRndrSwapScreen(s32 hRT) {
  NudxFw_SetBackBufferCopied(1);
  if (Pad[0] != NULL) {
        if (Pad[0]->oldpaddata == 0) {
          padflag = 1;
        }
        if (padflag == 0) {
          NudxFw_FlipScreen(hRT,0);
            return;
        }
        if ((Pad[0]->oldpaddata & 1) != 0) {
            NudxFw_FlipScreen(hRT,1);
            return;
        }
  }
    NudxFw_FlipScreen(hRT,1);
    return;
}

//96% NGC
s32 NuRndrGobj(struct nugobj_s* gobj, struct numtx_s* wm, f32** blendvals) {
    struct nugeomitem_s* item;
    struct nufaceongeom_s* facegeom;
    struct nugeom_s* geom;
    struct numtx_s* mtx;
    struct numtx_s premtx;
    int outcode;
    int total_outcode;
    int split;
    struct nuvec_s min;
    struct nuvec_s max;
    int current_lights;

    total_outcode = -1;

    if (gobj->next_gobj != NULL) {
        split = 1;
    } else {
        split = 0;
    }

    current_lights = NuLightStoreCurrentLights();

    for(; gobj != NULL; gobj = gobj->next_gobj)
    {
        if (gobj->culltype == 0)
        {
            outcode = NuCameraClipTestBoundingSphere
                (&gobj->bounding_box_center, &gobj->bounding_radius_from_center, wm);
        }
        else
        {
            if ((((gobj->origin).x != 0.0f) || ((gobj->origin).y != 0.0f)) || ((gobj->origin).z != 0.0f))
            {
                min = gobj->bounding_box_min;
                max = gobj->bounding_box_max;
                NuVecAdd(&min, &gobj->bounding_box_min, &gobj->origin);
                NuVecAdd(&max, &gobj->bounding_box_max, &gobj->origin);
                outcode = NuCameraClipTestExtents(&min, &max, wm);
            }
            else {
                outcode = NuCameraClipTestExtents(&gobj->bounding_box_min, &gobj->bounding_box_max, wm);
            }
        }

        if (total_outcode == -1) {
            total_outcode = outcode;
        } else {
            if (((total_outcode == 1) && (outcode != 1)) || ((total_outcode == 0) && (outcode != 0))){
                total_outcode = 2;
            }
        }

        if (outcode != 0)  {
            rndrmtx_cnt--;
            if (rndrmtx_cnt < 0) {
                NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c", 0x282,"NuRndrGobj : No free matrix slots!");
            }

            mtx = rndrmtx;
            mtx[rndrmtx_cnt] = *wm;

            if (split != 0) {
                NuMtxSetIdentity(&premtx);
                premtx._30 = (gobj->origin).x;
                premtx._31 = (gobj->origin).y;
                premtx._32 = (gobj->origin).z;
                NuMtxMul(mtx, &premtx, mtx);
            }

            for(geom = gobj->geom; geom != NULL; geom = geom->next)
            {
                if (geomitem_cnt != 0) {
                    geomitem_cnt--;
                    item = &geomitem[geomitem_cnt];
                    item->hdr.type = NURNDRITEM_GEOM3D;
                    item->hdr.flags = 0;
                    if (outcode == 1) {
                        item->hdr.flags = 1;
                    }
                    item->hdr.lights_index = current_lights;
                    item->mtx = mtx;
                    item->geom = geom;
                    item->blendvals = blendvals;

                    item->hShader = NuShaderAssignShader(geom);
                    if ((nurndr_forced_mtl_table != NULL) && ((geom->mtl)->special_id != 0))
                    {
                        if (nurndr_forced_mtl_table[(geom->mtl)->special_id] != NULL)
                        {
                            NuMtlAddRndrItem(nurndr_forced_mtl_table[(geom->mtl)->special_id], &item->hdr);
                        }
                    }
                    else {
                        if (nurndr_forced_mtl != NULL)
                        {
                            NuMtlAddRndrItem(nurndr_forced_mtl, &item->hdr);
                        } else {
                            NuMtlAddRndrItem(geom->mtl, &item->hdr);
                        }
                    }
                }
                else {
                    NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c", 0x2ba,"NuRndrGobj : No free geom item slots!");
                }
            }

            facegeom = gobj->faceon_geom;
            if (facegeom != NULL) {
                if (geomitem_cnt != 0) {
                    geomitem_cnt--;
                    item = &geomitem[geomitem_cnt];
                    item->geom = (struct nugeom_s *)facegeom;
                    item->hdr.lights_index = NuLightStoreCurrentLights();
                    item->hdr.type = NURNDRITEM_GEOMFACE;
                    item->hdr.flags = 0;
                    if (outcode == 1) {
                        item->hdr.flags = 1;
                    }
                    item->mtx = mtx;
                    item->blendvals = blendvals;
                    NuMtlAddFaceonItem(facegeom->mtl, &item->hdr);
                }
                else {
                    NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c", 0x2dc,"NuRndrGobj : No free geom item slots!");
                }
            }
        }
    }

    return total_outcode;
}

//MATCH NGC
s32 NuRndrGrassGobj(struct nugobj_s *gobj,struct numtx_s *wm,float **blendvals) {
    float dy;
    float dx;
    float dz;
    float dist;
    struct nugeomitem_s* item;
    struct nugeom_s *geom;
    struct numtx_s *mtx;
    s32 total_outcode;
    s32 outcode;
    struct nuvec_s min;
    struct nuvec_s max;
    struct numtl_s* mtl;

    total_outcode = -1;

        for(; gobj != NULL; gobj = gobj->next_gobj) {
            min = gobj->bounding_box_min;
            max = gobj->bounding_box_max;
            NuVecAdd(&min,&gobj->bounding_box_min,&gobj->origin);
            NuVecAdd(&max,&gobj->bounding_box_max,&gobj->origin);
            dx = min.x - wm->_30;
            dy = min.y - wm->_31;
            dz = min.z - wm->_32;
            dist = dx * dx + dy * dy + dz * dz;
            if (dist > 100.0) {
                outcode = 0;
            }
            else {
                outcode = NuCameraClipTestBoundingSphere
                                   (&gobj->bounding_box_center,&gobj->bounding_radius_from_center, wm);
            }

        if (total_outcode == -1) {
            total_outcode = outcode;
        } else {
            if (((total_outcode == 1) && (outcode != 1)) || ((total_outcode == 0) && (outcode != 0))){
                total_outcode = 2;
            }
        }

            if (outcode != 0) {
                    rndrmtx_cnt--;
                    if (rndrmtx_cnt < 0) {
                        NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c",0x316,"NuRndrGobj : No free matrix slots!");
                    }

                    mtx = &rndrmtx[rndrmtx_cnt];
                    *mtx = *wm;

                        for(geom = gobj->geom; geom != NULL; geom = geom->next) {
                            if (geomitem_cnt != 0) {
                                geomitem_cnt--;
                                item = &geomitem[geomitem_cnt];
                                item->hdr.type = NURNDRITEM_GEOM3D;
                                item->hdr.flags = 0;
                                if (outcode == 1) {
                                    item->hdr.flags = 1;
                                }
                                item->hdr.lights_index = NuLightStoreCurrentLights();
                                item->mtx = mtx;
                                item->geom = geom;
                                item->blendvals = blendvals;

                                item->hShader = NuShaderAssignShader(geom);
                                if ((nurndr_forced_mtl_table != NULL) && (geom->mtl->special_id != 0))
                                {
                                    if (nurndr_forced_mtl_table[geom->mtl->special_id] != NULL)
                                    {
                                        NuMtlAddRndrItem(nurndr_forced_mtl_table[geom->mtl->special_id], &item->hdr);
                                    }
                                }
                                else {
                                    if (nurndr_forced_mtl != NULL)
                                    {
                                        NuMtlAddRndrItem(nurndr_forced_mtl, &item->hdr);
                                    } else {
                                        NuMtlAddRndrItem(geom->mtl, &item->hdr);
                                    }
                                }
                            }
                            else {
                                NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c",0x344,"NuRndrGobj : No free geom item slots!");
                            }
                        }
                    }
        }
    return total_outcode;
}


//MATCH NGC
s32 NuRndrGobjSkin2(struct nugobj_s *gobj, int nummtx, struct numtx_s *wm, float **blendvals)
{
    struct nugeomitem_s* item;
    struct nugeom_s *geom;
    struct numtx_s *mtx;
    s32 outcode;
    s32 i;


    if (gobj->culltype == 0) {
        outcode = NuCameraClipTestBoundingSphere(&gobj->bounding_box_center, &gobj->bounding_radius_from_center, wm);
    }
    else {
        outcode = NuCameraClipTestExtents(&gobj->bounding_box_min, &gobj->bounding_box_max, wm);
    }

    if (outcode != 0) {
        rndrmtx_cnt = rndrmtx_cnt - nummtx;
        if (rndrmtx_cnt < 0) {
            NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c", 0x3c3,"NuRndrGobjSkin : No free matrix slots!");
        }
        mtx = &rndrmtx[rndrmtx_cnt];

        for (i = 0; i < nummtx; i++) {
            mtx[i] = wm[i];
        }

        for(geom = gobj->geom; geom != NULL; geom = geom->next)
        {
            if (geomitem_cnt != 0) {
                geomitem_cnt--;
                item = &geomitem[geomitem_cnt];
                item->hdr.type = NURNDRITEM_SKIN3D2;
                item->hdr.flags = 0;
                if (outcode == 1) {
                    item->hdr.flags = 1;
                }

                item->hdr.lights_index = NuLightStoreCurrentLights();
                item->blendvals = blendvals;
                item->mtx = mtx;
                item->geom = geom;

                item->hShader = NuShaderAssignShader(geom);
                if ((nurndr_forced_mtl_table != NULL) && ((geom->mtl)->special_id != 0)) {
                    if (nurndr_forced_mtl_table[(geom->mtl)->special_id] != NULL) {
                        NuMtlAddRndrItem(nurndr_forced_mtl_table[(geom->mtl)->special_id], &item->hdr);
                    }
                }
                else {
                    if (nurndr_forced_mtl != NULL) {
                        NuMtlAddRndrItem(nurndr_forced_mtl, &item->hdr);
                    } else {
                        NuMtlAddRndrItem(geom->mtl, &item->hdr);
                    }
                }
            }
            else {
                NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c", 0x3f3,"NuRndrGobjSkin : No free geom item slots!");
            }
        }
    }
    return outcode;
}


s32 NuRndrTri2d(struct nuvtx_tltc1_s *vtx,struct numtl_s *mtl)
{
  s32 stride;
  struct nuvtx_tltc1_s *vb;
  s32 i;
  struct nugeom_s *geom;
  struct nuprim_s *prim;

  NuMtlGet2dBuffer(mtl,NUPT_NDXTRI,&geom,&prim,&superbuffer_ptr,&superbuffer_end);
  if (geom->vtxmax - geom->vtxcnt < 3) {
    NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c",0x5c0, "NuRndrTri2d : Vertex buffer full!");
  }

  if ((s32)((u32)prim->max - (u32)prim->cnt) < 3) {
    NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c",0x5c2, "NuRndrTri2d : Primitive buffer full!");
  }
  stride = NuVtxStride(geom->vtxtype);
  vb = (struct nuvtx_tltc1_s *)geom->hVB;
  if (vb == NULL) {
    NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c",0x5c9, "NuRndrTri2d : Lock VB failed!");
  }

  vb = (struct nuvtx_tltc1_s *)((s32)vb + stride * geom->vtxcnt);
  for(i = 0; i < 3; i++) {
    (vb->pnt).x = (vtx->pnt).x;
    (vb->pnt).y = (vtx->pnt).y;
    (vb->pnt).z = (vtx->pnt).z;
    vb->rhw = vtx->rhw;
    vb->diffuse = vtx->diffuse;
    vb->tc[0] = vtx->tc[0];
    vb->tc[1] = vtx->tc[1];
    vtx = vtx + 1;
    vb = (struct nuvtx_tltc1_s *)((s32)vb + stride);
    geom->vtxcnt++;
  }
  return 1;
}

//NGC MATCH
s32 NuRndrTri3d(struct nuvtx_tc1_s *vtx,struct numtl_s *mtl,struct numtx_s *wm) {
    s32 stride;
    s32 i;
    s32 ix;
    struct nuvtx_tc1_s *vb;
    struct nugeom_s *geom;
    struct nuprim_s *prim;


    NuMtlGet3dBuffer(mtl,NUPT_TRI,&geom,&prim,&superbuffer_ptr,&superbuffer_end);
    if (geom->vtxmax - geom->vtxcnt < 3) {
        NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c",0x5f0, "NuRndrTri3d : Vertex buffer full!");
    }
    if ((s32)((u32)prim->max - (u32)prim->cnt) < 3) {
        NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c",0x5f2, "NuRndrTri3d : Primitive buffer full!");
    }
    stride = NuVtxStride(geom->vtxtype);
    vb = (struct nuvtx_tltc1_s *)geom->hVB;
    if (vb == 0) {
        NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c",0x5f9, "NuRndrTri3d : Lock VB failed!");
    }
    vb = (struct nuvtx_tc1_s *)((s32)vb + stride * geom->vtxcnt);
    for(i = 0; i < 3; vtx++ , i++) {
        if (wm != NULL) {
            NuVecMtxTransform(&vb->pnt,&vtx->pnt,wm);
            NuVecMtxRotate(&vb->nrm,&vtx->nrm,wm);
        }
        else {
            vb->pnt = vtx->pnt;
            vb->nrm = vtx->nrm;
        }
        vb->diffuse = vtx->diffuse;
        vb->tc[0] = vtx->tc[0];
        vb->tc[1] = vtx->tc[1];
        vb = (struct nuvtx_tc1_s *)((s32)vb + stride);
    }
    geom->vtxcnt += 3;
    return 1;
}

//MATCH NGC
float NuRndrItemDist(struct nurndritem_s *item) {
  struct nuvec_s pnt;
  float dist;
  struct nugeomitem_s* geomitem;

  //geomitem->hdr = *item;
  dist = 0.0f;
  if (item->type == NURNDRITEM_GEOM3D) {
    NuCameraTransformClip(&pnt,&((struct nugeomitem_s*)item)->mtx->_30,1,NULL);
    dist = pnt.z;
  }
  return dist;
}

//MATCH NGC
s32 NuRndrStrip3d(struct nuvtx_tc1_s **vtx,struct numtl_s *mtl,struct numtx_s* wm,s32 pts) {
    
    struct nugeom_s *geom;
    struct nuprim_s *prim;
    struct nuvtx_tc1_s *vb;
    s32 stride;
    s32 i;
    s32 ix;
    s32 realpts;

    
    ix = (pts - 2);
    realpts = ix * 3;
    NuMtlGet3dBuffer(mtl, NUPT_TRI, &geom, &prim, &superbuffer_ptr, &superbuffer_end);
    if ((geom->vtxmax - geom->vtxcnt) < realpts) {
        NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c", 0x643,"NuRndrStrip3d : Vertex buffer full!");
    }
    stride = NuVtxStride(geom->vtxtype);
    vb = (struct nuvtx_tc1_s *)geom->hVB;
    if (vb == 0) {
        NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c", 0x64c,"NuRndrTri3d : Lock VB failed!");
    }
    
    vb = (struct nuvtx_tc1_s *) ((s32)vb + stride * geom->vtxcnt);
    
    for (i = 0; i < ix; i++) {
        memcpy(vb, *vtx, 0x24);
        (s32)vb += stride;
        vtx++;
        memcpy(vb, *vtx, 0x24);
        (s32)vb += stride;
        vtx++;
        memcpy(vb, *vtx, 0x24);
        (s32)vb += stride;
        vtx--;
    }
    
    geom->vtxcnt += realpts;
    return 1;
}

//NGC MATCH
static void NuRndr2dItem(struct nugeomitem_s *item) {
  struct nuprim_s *prim;

  if ((struct _GSMATRIX *)item->mtx != NULL) {
    GS_SetWorldMatrix((struct _GSMATRIX*)item->mtx);
  }
  else {
    GS_LoadWorldMatrixIdentity();
  }
  SetupShaders(item);
  for (prim = item->geom->prim; prim != NULL; prim = prim->next) {
    GS_DrawTriList(item->geom->vtxcnt,(float *)item->geom->hVB,NuVtxStride(item->geom->vtxtype));
  }
  return;
}

//NGC MATCH
void NuRndrFaceItem(struct nugeomitem_s* item) {

    struct nufaceongeom_s* fop;
    struct nuvtx_tc1_s vertices[4];
    struct nuvec_s centrepos;
    struct nuvec_s vtxpos;
    long colour;
    struct nufaceon_s* fo;
    struct numtx_s centre;
    struct numtx_s rotation;
    struct nuvec_s local;
    struct numtx_s identity;
    int nfo;
    int ct;
    struct numtx_s faceonmtx;
    float w;
    float h;

    DBTimerStart(0x1c);
    SetupShaders(item);
    NuMtxSetIdentity(&identity);
    NuMtxSetIdentity(&centre);

    centre._30 = item->mtx->_30;
    centre._31 = item->mtx->_31;
    centre._32 = item->mtx->_32;

    GS_LoadWorldMatrixIdentity();

    rotation = *item->mtx;
    rotation._30 = 0.0f;
    rotation._31 = 0.0f;
    rotation._32 = 0.0f;

    for (fop = (struct nufaceongeom_s*)item->geom; fop != NULL; fop = fop->next) {

        fo = fop->faceons;
        nfo = fop->nfaceons;

        for (ct = 0; ct < nfo; ct++, fo++)
        {
            colour = fo->colour;
            w = fo->width * 0.5f;
            h = fo->height * 0.5f;
            NuVecMtxRotate(&local, &fo->point, &rotation);
            NuVecAdd(&centrepos, &local, &item->mtx->_30); // ????
            NuMtxCalcCheapFaceOn(&faceonmtx, &centrepos);
            vtxpos.x = -w;
            vtxpos.y = h;
            vtxpos.z = 0.0f;
            NuVecMtxTransform(&vertices[0].pnt, &vtxpos, &faceonmtx);
            vertices[0].diffuse = colour;
            vertices[0].tc[0] = 0.0f;
            vertices[0].tc[1] = 0.0f;
            vertices[0].nrm.x = 1.0f;
            vertices[0].nrm.y = 0.0f;
            vertices[0].nrm.z = 0.0f;
            vtxpos.x = w;
            vtxpos.y = h;
            vtxpos.z = 0.0f;
            NuVecMtxTransform(&vertices[1].pnt, &vtxpos, &faceonmtx);
            vertices[1].diffuse = colour;
            vertices[1].tc[0] = 1.0f;
            vertices[1].tc[1] = 0.0f;
            vertices[1].nrm.x = 1.0f;
            vertices[1].nrm.y = 0.0f;
            vertices[1].nrm.z = 0.0f;
            vtxpos.x = w;
            vtxpos.y = -h;
            vtxpos.z = 0.0f;
            NuVecMtxTransform(&vertices[2].pnt, &vtxpos, &faceonmtx);
            vertices[2].diffuse = colour;
            vertices[2].tc[0] = 1.0f;
            vertices[2].tc[1] = 1.0f;
            vertices[2].nrm.x = 1.0f;
            vertices[2].nrm.y = 0.0f;
            vertices[2].nrm.z = 0.0f;
            vtxpos.x = -w;
            vtxpos.y = -h;
            vtxpos.z = 0.0f;
            NuVecMtxTransform(&vertices[3].pnt, &vtxpos, &faceonmtx);
            vertices[3].diffuse = colour;
            vertices[3].tc[0] = 0.0f;
            vertices[3].tc[1] = 1.0f;
            vertices[3].nrm.x = 1.0f;
            vertices[3].nrm.y = 0.0f;
            vertices[3].nrm.z = 0.0f;
            GS_DrawPrimitiveQuad(&vertices);
        }
    }
    DBTimerStart(0x1c);
    return;
}

//MATCH GCN
static void NuRndrGeomItem(struct nugeomitem_s *item) {
  struct nuprim_s *prim;

  DBTimerStart(6);
  if ((struct _GSMATRIX *)item->mtx != NULL) {
    GS_SetWorldMatrix((struct _GSMATRIX *)item->mtx);
  }
  else {
    GS_LoadWorldMatrixIdentity();
  }
  SetupShaders(item);
  for (prim = item->geom->prim; prim != NULL; prim = prim->next) {
          switch (prim->type) {
              case NUPT_POINT:
                    GS_DrawPointList(item->geom->vtxcnt, item->geom->hVB, NuVtxStride(item->geom->vtxtype)); //GS_DrawPointList --> EMPTY FUNCTION
                    break;
              case NUPT_LINE:
                  break;
                case NUPT_TRI:
                    GS_DrawTriList(item->geom->vtxcnt,(float *)item->geom->hVB,NuVtxStride(item->geom->vtxtype));
                    break;
                case NUPT_TRISTRIP:
                    GS_DrawTriStrip(item->geom->vtxcnt,(float *)item->geom->hVB,NuVtxStride(item->geom->vtxtype));
                    break;
                case NUPT_QUADLIST:
                    GS_DrawQuadList(item->geom->vtxcnt,(float *)item->geom->hVB,NuVtxStride(item->geom->vtxtype));
                    break;
                case NUPT_NDXTRI:
                    GS_DrawIndexedTriList(prim->cnt,(short *)prim->idxbuff,(float *)item->geom->hVB,NuVtxStride(item->geom->vtxtype));
                    break;
                case NUPT_NDXTRISTRIP:
                    GS_DrawIndexedTriStrip(prim->cnt,(short *)prim->idxbuff,(float *)item->geom->hVB,NuVtxStride(item->geom->vtxtype));
              break;
          }
  }
  DBTimerEnd(6);
  return;
}

//NGC MATCH
static void NuRndrBlendedSkinItem(struct nugeomitem_s* item) {
  float blendval;
  int j;
  struct nuvec_s** LCBlendOffsets;
  struct nugeom_s *geom;
  struct nuvtx_sk3tc1_s *srcverts;
  struct nuvec_s *destvb;
  int i;
  struct NUBLENDGEOM_s *blendgeom;
  struct nuprim_s *prim;

  blendgeom = item->geom->blendgeom;
  DBTimerStart(4);
  DBTimerStart(0xd);
  //PPCMtmmcr0(0x8b);
  //PPCMtmmcr1(0x78400000);
  geom = item->geom;
  srcverts = (struct nuvtx_sk3tc1_s *)geom->hVB;
  destvb = (struct nuvec_s *)geom->blendgeom->hVB;
    for (i = 0; i < item->geom->vtxcnt; i++) {
        destvb->x = srcverts->pnt.x;
        destvb->y = srcverts->pnt.y;
        destvb->z = srcverts->pnt.z;
        for (j = 0; j < blendgeom->nblends; j++) {
          LCBlendOffsets = blendgeom->blend_offsets[j];
            if (LCBlendOffsets != 0) {
            blendval = (*item->blendvals)[*(j + blendgeom->ix)];
            if (blendval != 0.0f) {
                destvb->x =  ((*(f32*)&LCBlendOffsets[i * 3] * blendval) + destvb->x);
                destvb->y =  (((i + (blendgeom->blend_offsets[j]))->y * blendval) + destvb->y); //(*LCBlendOffsets[i])
                destvb->z =  (((i + (blendgeom->blend_offsets[j]))->z * blendval) + destvb->z);
                }
            }
        }
      destvb = destvb + 1;
      srcverts = srcverts + 1;
    }
  //PPCMtmmcr1(0);
  //PPCMtmmcr0(0);
  DBTimerEnd(0xd);
  SetupShaders(item);
  GS_LoadWorldMatrixIdentity();
  GS_SetVertexSource((float *)item->geom->hVB);
  GS_SetBlendSource((struct _GS_VECTOR3 *)item->geom->blendgeom->hVB);
  for (prim = item->geom->prim; prim != NULL; prim = prim->next) {
    NuShaderSetSkinningConstants(item,prim);
    if (prim->type == NUPT_NDXTRI) {
      SkinnedShaderBlend(prim->cnt,prim->idxbuff);
    }
    else {
      NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c",0x1087,"NuRndrGeomItem : Unknown primitive type!");
    }
  }
  DBTimerEnd(4);
  return;
}

//NGC MATCH
void NuRndrSkinItem2(struct nugeomitem_s *item) {
    struct nuprim_s *prim;

    if ((item->blendvals != NULL) && (item->geom->blendgeom != NULL)) {
        NuRndrBlendedSkinItem(item);
        return;
    }
    DBTimerStart(5);
    SetupShaders(item);
    GS_LoadWorldMatrixIdentity();
    GS_SetVertexSource((float *)item->geom->hVB);
    for (prim = item->geom->prim; prim != NULL; prim = prim->next) {
        NuShaderSetSkinningConstants(item,prim);
        if (prim->type == NUPT_NDXTRI) {
            SkinnedShader((u32)prim->cnt,(short *)prim->idxbuff);
        }
        else if (prim->type != NUPT_NDXTRISTRIP) {
            NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c",0xf4e,"NuRndrGeomItem : Unknown primitive type!");
        }
    }
    DBTimerEnd(5);
    return;
}

//MATCH GCN
void NuRndrAnglesZX(struct nuvec_s *src,struct nuvec_s *rot) {
  struct nuvec_s v[2];

  rot->x = (float)NuAtan2D(src->z,src->y);
  NuVecRotateX(&v[0],src,(s32)-rot->x);
  rot->z = (float)-NuAtan2D(v[0].x,v[0].y);
  return;
}


//MATCH GCN
void NuRndrItem(struct nurndritem_s *item) {
//struct nugeomitem_s * geomitem;

    //geomitem->hdr = *item;
        switch (item->type) {
                    case NURNDRITEM_GEOM2D:
                        DBTimerStart(0x1e);
                        NuRndr2dItem((struct nugeomitem_s *)item);
                        DBTimerEnd(0x1e);
                    break;
                    case NURNDRITEM_GEOM3D:
                        DBTimerStart(0x1f);
                        NuRndrGeomItem((struct nugeomitem_s *)item);
                        DBTimerEnd(0x1f);
                        break;
                    case NURNDRITEM_SKIN3D2:
                        DBTimerStart(0x20);
                        NuRndrSkinItem2((struct nugeomitem_s *)item);
                        DBTimerEnd(0x20);
                        break;
                    case NURNDRITEM_GEOMFACE:
                        DBTimerStart(0x22);
                        NuRndrFaceItem((struct nugeomitem_s *)item);
                        DBTimerEnd(0x22);
                        break;
                    default:
                            NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c",0xbff,"NuRndrItem : Unknown render item type!");
                        break;
        }
    return;

}

//MATCH GCN
void NuRndrAddFootPrint(s32 rot,float sizex,float sizez,s32 brightness,struct nuvec_s *pos,struct nuvec_s *norm,s32 gfx,s32 unknown) {

    struct nuvec4_s tp [4];
    struct nuvec_s terrot;
    struct numtx_s m;
    s32 i;
    s32 i3;
    s32 i2;


    tp[0].x = NuTrigTable[((rot + 0x4000) & 0xffffU)] * sizex + NuTrigTable[rot & 0xffff] * sizez;
    tp[0].z = -NuTrigTable[rot & 0xffff] * sizex + NuTrigTable[((rot + 0x4000) & 0xffffU)] * sizez;

    tp[1].x = NuTrigTable[((rot + 0x4000) & 0xffffU)] * -sizex + NuTrigTable[rot & 0xffff] * sizez;
    tp[1].z = -NuTrigTable[rot & 0xffff] * -sizex + NuTrigTable[((rot + 0x4000) & 0xffffU)] * sizez;

    tp[2].x = NuTrigTable[((rot + 0x4000) & 0xffffU)] * sizex + NuTrigTable[rot & 0xffff] * -sizez;
    tp[2].z = -NuTrigTable[rot & 0xffff] * sizex + NuTrigTable[((rot + 0x4000) & 0xffffU)] * -sizez;

    tp[3].x = NuTrigTable[((rot + 0x4000) & 0xffffU)] * -sizex + NuTrigTable[rot & 0xffff] * -sizez;
    tp[3].z = -NuTrigTable[rot & 0xffff] * -sizex + NuTrigTable[((rot + 0x4000) & 0xffffU)] * -sizez;

    tp[3].y = 0.0f;
    tp[2].y = 0.0f;
    tp[1].y = 0.0f;
    tp[0].y = 0.0f;

    tp[3].w = 0.0f;
    tp[2].w = 0.0f;
    tp[1].w = 0.0f;
    tp[0].w = 0.0f;

    NuRndrAnglesZX(norm,&terrot);
    NuMtxSetIdentity(&m);
    NuMtxRotateZ(&m,(int)terrot.z);
    NuMtxRotateX(&m,(int)terrot.x);
    NuVec4MtxTransformVU0(tp,tp,&m);
    NuVec4MtxTransformVU0(&tp[1],&tp[1],&m);
    NuVec4MtxTransformVU0(&tp[2],&tp[2],&m);
    NuVec4MtxTransformVU0(&tp[3],&tp[3],&m);
    i2 = NuRndrFootFree & 0x3f;
    NuRndrFootData[i2].pnts[0].x = tp[0].x + pos->x;
    NuRndrFootData[i2].pnts[0].y = tp[0].y + pos->y;
    NuRndrFootData[i2].pnts[0].z = tp[0].z + pos->z;
    NuRndrFootData[i2].pnts[1].x = tp[1].x + pos->x;
    NuRndrFootData[i2].pnts[1].y = tp[1].y + pos->y;
    NuRndrFootData[i2].pnts[1].z = tp[1].z + pos->z;
    NuRndrFootData[i2].pnts[2].x = tp[2].x + pos->x;
    NuRndrFootData[i2].pnts[2].y = tp[2].y + pos->y;
    NuRndrFootData[i2].pnts[2].z = tp[2].z + pos->z;
    NuRndrFootData[i2].pnts[3].x = tp[3].x + pos->x;
    NuRndrFootData[i2].pnts[3].y = tp[3].y + pos->y;
    NuRndrFootData[i2].pnts[3].z = tp[3].z + pos->z;
    NuRndrFootData[i2].gfx = (char)(gfx << 2);
    NuRndrFootData[i2].timer = '\x10';
    NuRndrFootData[i2].brightness = (short)brightness;
    NuRndrFootFree = NuRndrFootFree + 1;
    for (i3 = NuRndrFootFree; i3 < NuRndrFootFree + 8; i3++) {
        i2 = i3 & 0x3f;
        if (NuRndrFootData[i2].timer > '\x0f') {
            NuRndrFootData[i2].timer = '\x0f';
        }
    }
    return;
}


//86% GCN
void NuRndrFootPrints(struct numtl_s *mtl,float *u,float *v) {
    s32 cnt;
    s32 lp, cnt2;
    struct nuvtx_tc1_s *vp [4];
    static struct nuvtx_tc1_s vtx_247[256][4]; //nuvtx_tc1_s vtx[2048];

    for (lp = 0, cnt = 0; lp < 0x40; lp++) {
        if (NuRndrFootData[lp].timer > 0) {
            if (NuRndrFootData[lp].timer < 16) {
                NuRndrFootData[lp].timer--;
            }
            cnt += 4;
            vtx_247[lp][0].pnt = NuRndrFootData[lp].pnts[0];
            vtx_247[lp][0].tc[0] = u[NuRndrFootData[lp].gfx];
            vtx_247[lp][0].tc[1] = v[NuRndrFootData[lp].gfx];

            vtx_247[lp][1].pnt = NuRndrFootData[lp].pnts[1];
            vtx_247[lp][1].tc[0] = u[NuRndrFootData[lp].gfx + 1];
            vtx_247[lp][1].tc[1] = v[NuRndrFootData[lp].gfx + 1];

            vtx_247[lp][2].pnt = NuRndrFootData[lp].pnts[2];
            vtx_247[lp][2].tc[0] = u[NuRndrFootData[lp].gfx + 2];
            vtx_247[lp][2].tc[1] = v[NuRndrFootData[lp].gfx + 2];

            vtx_247[lp][3].pnt = NuRndrFootData[lp].pnts[3];
            vtx_247[lp][3].tc[0] = u[NuRndrFootData[lp].gfx + 3];
            vtx_247[lp][3].tc[1] =  v[NuRndrFootData[lp].gfx + 3];

            vtx_247[lp][3].diffuse = (NuRndrFootData[lp].timer * NuRndrFootData[lp].brightness * 0x100000 & 0xff000000U) + 0x808080;
            vtx_247[lp][2].diffuse = (NuRndrFootData[lp].timer * NuRndrFootData[lp].brightness * 0x100000 & 0xff000000U) + 0x808080;
            vtx_247[lp][1].diffuse = (NuRndrFootData[lp].timer * NuRndrFootData[lp].brightness * 0x100000 & 0xff000000U) + 0x808080;
            vtx_247[lp][0].diffuse = (NuRndrFootData[lp].timer * NuRndrFootData[lp].brightness * 0x100000 & 0xff000000U) + 0x808080;
        }
    }
    if (cnt != 0) {
        for (lp = 0, cnt2=0; cnt2 < cnt; lp++, cnt2+=4) {
            vp[0] = &vtx_247[lp][0];
            vp[1] = &vtx_247[lp][1];
            vp[2] = &vtx_247[lp][2];
            vp[3] = &vtx_247[lp][3];
            NuRndrStrip3d(vp,mtl,0,4);
        }
    }
}

//MATCH GCN
float * NuRndrCreateBlendShapeDeformerWeightsArray(s32 nweights) {

  rndr_blend_shape_deformer_wt_cnt -= nweights;
  if (rndr_blend_shape_deformer_wt_cnt < 0) {
    NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c",0x11f4,"No free blend shape deformer weights");
  }
  else {
    return &rndr_blend_shape_deformer_wts[rndr_blend_shape_deformer_wt_cnt];
  }
  return NULL;
}

//MATCH GCN
float ** NuRndrCreateBlendShapeDWAPointers(s32 size) {

  rndr_blend_shape_deformer_wt_ptrs_cnt -= size;
  if (rndr_blend_shape_deformer_wt_cnt < 0) {
    NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c",0x1206,"No free blend shape deformer weights");
  }
  else {
    return &rndr_blend_shape_deformer_wt_ptrs[rndr_blend_shape_deformer_wt_ptrs_cnt];
  }
  return NULL;
}

//MATCH GCN
void NuRndrInitWorld(void) {
    s32 lp;

    lp = 0x40;
    NuRndrShadMaskCount = 0;
    do {
        NuRndrFootData[lp - 1].timer = 0;
        lp--;
    } while (lp != 0);
    return;
}

//MATCH GCN
void NuRndrFx(s32 paused,struct nuvec_s *playerpos) {
  //NuLgtArcLaserDraw(paused);
  return;
}

static struct nuvtx_tc1_s vtx_263[2048];

//40% GCN
void NuRndrTrail(s32 trailpt,struct trail_s *trail,s32 TRAILCOUNT) {
    s32 uVar2;
    s32 uVar5;
    s32 iVar6;
    s32 unaff_r15;
    s32 iVar8;
    s32 uVar9;
    s32 iVar10;
    s32 iVar11;
    s32 iVar12;
    s32 iVar13;

    s32 cnt;
    s32 loop;
    struct nuvtx_tc1_s *vp [4];


    //loop = (trailpt + 1) - TRAILCOUNT;
    iVar8 = 0;
        //uVar7 = TRAILCOUNT - 1;
        iVar12 = 0x6c;
        iVar13 = 0x48;
        iVar10 = 0x24;
        iVar11 = 0;
        uVar2 = -1;
        for (cnt = 0; cnt < (trailpt + 1) - TRAILCOUNT; cnt++) {
            uVar9 = trailpt & TRAILCOUNT - 1;
            if (trail[uVar9].pos1.x != -10000.0f) {
                uVar9 = -1;
            }
            else if (uVar2 == -1) {
                uVar5 = 0;
                unaff_r15 = 1;
            }
            else {
                uVar5 = trail[uVar9].intensity;
                if ((trail[cnt - 2U & TRAILCOUNT - 1].pos1.x == -10000.0f) ||
                   ((s32)(cnt - 1U) <= (cnt + 1) - TRAILCOUNT)) {
                    uVar5 = 0;
                }
                else if ((trail[cnt - 1U & TRAILCOUNT - 1].pos1.x == -10000.0f) || (unaff_r15 != 0)) {
                    uVar5 = ((uVar5 >> 0x18) - (uVar5 >> 0x1f)) * 0x800000 & 0xff000000;
                }
                vtx_263[iVar11].pnt = trail[uVar2].pos1;
                //unaff_r15 = 0;

                (vtx_263[cnt].pnt) = trail[uVar9].pos1;

                vtx_263[iVar13].pnt.x = (trail[uVar2].pos2.x - trail[uVar2].pos1.x) * 0.25f + trail[uVar2].pos1.x;

                vtx_263[iVar13].pnt.y = (trail[uVar2].pos2.y - trail[uVar2].pos1.y) * 0.25f + trail[uVar2].pos1.y;

                vtx_263[iVar13].pnt.z = (trail[uVar2].pos2.z - trail[uVar2].pos1.z) * 0.25f + trail[uVar2].pos1.z;

                 vtx_263[iVar12].nrm.x = (trail[uVar9].pos2.x - trail[uVar9].pos1.x) * 0.25f + trail[uVar9].pos1.x;

                vtx_263[iVar12].nrm.y = (trail[uVar9].pos2.y - trail[uVar9].pos1.y) * 0.25f + trail[uVar9].pos1.y;

                vtx_263[iVar12].nrm.z = (trail[uVar9].pos2.z - trail[uVar9].pos1.z) * 0.25f + trail[uVar9].pos1.z;


                if (-1 > uVar5) {
                    vtx_263[iVar11].diffuse = -uVar5;
                    vtx_263[iVar13].diffuse = 0;
                }
                else {
                    vtx_263[iVar11].diffuse = uVar5 + 0xffffff;
                    vtx_263[iVar13].diffuse = 0xffffff;
                }
                if (uVar5 < 0) {
                    vtx_263[iVar10].diffuse = -uVar5;
                    vtx_263[iVar12].diffuse = 0;
                }
                else {
                    vtx_263[iVar10].diffuse = uVar5 + 0xffffff;
                    vtx_263[iVar12].diffuse = 0xffffff;
                }
                vtx_263[iVar11++].pnt = trail[uVar2].pos2;
                vtx_263[iVar11++].pnt = trail[uVar9].pos2;
                vtx_263[iVar13].pnt.x =
                     (trail[uVar2].pos1.x - trail[uVar2].pos2.x) * 0.25f + trail[uVar2].pos2.x;
                vtx_263[iVar13].pnt.y =
                     (trail[uVar2].pos1.y - trail[uVar2].pos2.y) * 0.25f + trail[uVar2].pos2.y;
                vtx_263[iVar13].pnt.z =
                     (trail[uVar2].pos1.z - trail[uVar2].pos2.z) * 0.25f + trail[uVar2].pos2.z;
                vtx_263[iVar12].nrm.x =
                     (trail[uVar9].pos1.x - trail[uVar9].pos2.x) * 0.25f + trail[uVar9].pos2.x;
                vtx_263[iVar12].nrm.y =
                     (trail[uVar9].pos1.y - trail[uVar9].pos2.y) * 0.25f + trail[uVar9].pos2.y;
                vtx_263[iVar12].nrm.z =
                     (trail[uVar9].pos1.z - trail[uVar9].pos2.z) * 0.25f + trail[uVar9].pos2.z;
                if (-1 > (s32)uVar5) {
                    vtx_263[iVar11].diffuse = 0xfffff - uVar5;
                }
                else {
                    vtx_263[iVar11].diffuse = uVar5;
                }
                if (-1 > (s32)uVar5) {
                    vtx_263[iVar10].diffuse = 0xffffff - uVar5;
                }
                else {
                    vtx_263[iVar10].diffuse = uVar5;
                }
                iVar12 = iVar12 + 0x120;
                iVar13 = iVar13 + 0x120;
                iVar10 = iVar10 + 0x120;
                iVar11 = iVar11 + 0x120;
                iVar8 = iVar8 + 4;
            }
            uVar2 = uVar9;
        }
    if (iVar8 != 0) {
            for (iVar6 = 0; iVar6 <= iVar8; iVar6++) {
                vp[iVar6] = &vtx_263[iVar6];
                NuRndrStrip3d(&vp[iVar6],NuLightAddMat,0,4);
            }
    }
    return;
}

//MATCH GCN
void NuRndrWaterRip(struct numtl_s *mtl) {
    int outcode;
    struct nuvtx_tc1_s *tmp1;
    int ind;
    struct WaterDat *dat;
    float size;
    struct numtx_s* cmtx;
    struct nuvec_s pos;
    int lp;

    cmtx = NuCameraGetMtx();
    NuRndrWaterRipCnt = 0;
    for (ind = 0; ind < 0x20; ind++) {
        if ((NuRndrWaterRipDat[ind].timer != 0) &&
           (outcode = NuCameraClipTestPoints(&NuRndrWaterRipDat[ind].pos,1,NULL) == 0)) {
            indexlist_272[NuRndrWaterRipCnt] = (short)ind;
            NuRndrWaterRipCnt++;
        }
    }
    if (NuRndrWaterRipCnt != 0) {
        for(lp = 0; lp < 4; lp++) {
            vtx_270[lp].nrm.x = 1.0f;
            vtx_270[lp].nrm.y = 0.0f;
            vtx_270[lp].nrm.z = 0.0f;
        }

        vtx2_271[0] = &vtx_270[0];
        vtx2_271[1] = &vtx_270[1];
        vtx2_271[2] = &vtx_270[2];
        vtx2_271[3] = &vtx_270[3];
        for(ind = 0; ind < NuRndrWaterRipCnt; ind++) {
            pos = NuRndrWaterRipDat[indexlist_272[ind]].pos;
            size = NuRndrWaterRipDat[indexlist_272[ind]].cursize;
            vtx_270[0].diffuse = NuRndrWaterRipDat[indexlist_272[ind]].curshade + 0x80808080;
            vtx_270[1].diffuse = NuRndrWaterRipDat[indexlist_272[ind]].curshade + 0x80808080;
            vtx_270[2].diffuse = NuRndrWaterRipDat[indexlist_272[ind]].curshade + 0x80808080;
            vtx_270[3].diffuse = NuRndrWaterRipDat[indexlist_272[ind]].curshade + 0x80808080;

            vtx_270[0].pnt.x = pos.x - size;
            vtx_270[0].pnt.y = pos.y - 0.1;
            vtx_270[0].pnt.z = pos.z - size;
            vtx_270[0].tc[0] = 0.75f;
            vtx_270[0].tc[1] = 0.25f;


            vtx_270[1].pnt.x = pos.x + size;
            vtx_270[1].pnt.y = pos.y - 0.1;
            vtx_270[1].pnt.z = pos.z - size;
            vtx_270[1].tc[0] = 1.0f;
            vtx_270[1].tc[1] = 0.25f;

            vtx_270[2].pnt.x = pos.x - size;
            vtx_270[2].pnt.y = pos.y - 0.1;
            vtx_270[2].pnt.z = pos.z + size;
            vtx_270[2].tc[0] = 0.75f;
            vtx_270[2].tc[1] = 0.5f;

            vtx_270[3].pnt.x = pos.x + size;
            vtx_270[3].pnt.y = pos.y - 0.1;
            vtx_270[3].pnt.z = pos.z + size;
            vtx_270[3].tc[0] = 1.0f;
            vtx_270[3].tc[1] = 0.5f;

            NuRndrStrip3d(vtx2_271,mtl,0,4);
        }
    }
    return;
}

//87% NGC
void NuRndrShadPolys(struct numtl_s *mtl) {
    s32 s;
    s32 ind;
    float size;
    struct nuvec_s pos;
    s32 lp;
    u32 colour;
    static struct nuvtx_tc1_s vtx[4];
    static struct nuvtx_tc1_s* vtx2[4];

    if (NuRndrShadowCnt != 0) {
        for (lp = 0; lp < 4; lp++) {
            vtx[lp].nrm.x = 0.0f;
            vtx[lp].nrm.y = 0.0f;
            vtx[lp].nrm.z = 0.0f;
        }

        vtx2[0] = &vtx[0];
        vtx2[1] = &vtx[1];
        vtx2[2] = &vtx[2];
        vtx2[3] = &vtx[3];

        for (ind = 0; ind < NuRndrShadowCnt; ind++) {

            pos = NuRndrShadPolDat[ind].pos;
            size = NuRndrShadPolDat[ind].size;

            vtx[0].diffuse = colour;
            vtx[1].diffuse = colour;
            vtx[2].diffuse = colour;
            vtx[3].diffuse = colour;
            colour = 0xFF000000;

            vtx[0].pnt.x = pos.x - size;
            vtx[0].pnt.y = pos.y + 0.01f;
            vtx[0].pnt.z = pos.z - size;
            vtx[1].pnt.x = pos.x + size;
            vtx[1].pnt.y = pos.y + 0.01f;
            vtx[1].pnt.z = pos.z - size;
            vtx[2].pnt.x = pos.x - size;
            vtx[2].pnt.y = pos.y + 0.01f;
            vtx[2].pnt.z = pos.z + size;
            vtx[3].pnt.x = pos.x + size;
            vtx[3].pnt.y = pos.y + 0.01f;
            vtx[3].pnt.z = pos.z + size;

            vtx[0].tc[0] = 0.0f;
            vtx[0].tc[1] = 0.0f;
            vtx[1].tc[0] = 1.0f;
            vtx[1].tc[1] = 0.0f;
            vtx[2].tc[0] = 0.0f;
            vtx[2].tc[1] = 1.0f;
            vtx[3].tc[0] = 1.0f;
            vtx[3].tc[1] = 1.0f;

            NuRndrStrip3d(vtx2, mtl, 0, 4);
        }
        NuRndrShadowCnt = 0;
    }
    return;
}


//MATCH GCN
void NuRndrWaterRippleUpdate(s32 count) {
    float t;
    s32 lp;

    for(lp = 0; lp < 0x20; lp++) {
        if (NuRndrWaterRipDat[lp].timer != 0) {
            NuRndrWaterRipDat[lp].timer = NuRndrWaterRipDat[lp].timer - (short)count;
            if (NuRndrWaterRipDat[lp].timer < 1) {
                NuRndrWaterRipDat[lp].timer = 0;
            }
            else {
                t = (float)NuRndrWaterRipDat[lp].timer / (float) NuRndrWaterRipDat[lp].otimer;
                NuRndrWaterRipDat[lp].cursize = (NuRndrWaterRipDat[lp].size - NuRndrWaterRipDat[lp].endsize) * t + NuRndrWaterRipDat[lp].endsize;
                if (t > 0.75f) {
                   t = (1.0f - t) * 4.0f;
                }
                else {
                    t = t * 1.333333f;
                }
                NuRndrWaterRipDat[lp].curshade =
                     (s32)((NuRndrWaterRipDat[lp].shade >> 0x18) * t) * 0x1000000 +
                     (NuRndrWaterRipDat[lp].shade & 0xffffff);
            }
        }
    }
    return;
}


//NGC MATCH
void NuRndrAddWaterRipple(struct nuvec_s *pos,float size,float endsize,s32 duration,s32 shade) {
  s32 lp;

  for (lp = 0; lp < 0x20; lp++) {
        if (NuRndrWaterRipDat[lp].timer == 0) {

          pos->y = pos->y + 0.01f;
          NuRndrWaterRipDat[lp].pos = *pos;
          NuRndrWaterRipDat[lp].size = size;
          NuRndrWaterRipDat[lp].cursize = size;
          NuRndrWaterRipDat[lp].endsize = endsize;
          NuRndrWaterRipDat[lp].shade = shade;
          NuRndrWaterRipDat[lp].curshade = 0;
          NuRndrWaterRipDat[lp].timer = (short)duration;
          NuRndrWaterRipDat[lp].otimer = (short)duration;
          return;
        }
  }
  return;
}

//NGC MATCH
void NuRndrAddShadow(struct nuvec_s* v, f32 scale, s16 shade, s16 xrot,  s16 yrot, s16 zrot) {

    if ((NuCameraClipTestPoints(v, 1, NULL) == 0) && ((s32) NuRndrShadowCnt < 0x80)) {
        v->y += 0.01f;
        NuRndrShadPolDat[NuRndrShadowCnt].pos = *v;
        NuRndrShadPolDat[NuRndrShadowCnt].size = scale;
        NuRndrShadPolDat[NuRndrShadowCnt].shade = shade;
        NuRndrShadPolDat[NuRndrShadowCnt].xrot = xrot;
        NuRndrShadPolDat[NuRndrShadowCnt].yrot = yrot;
        NuRndrShadPolDat[NuRndrShadowCnt].zrot = zrot;
        NuRndrShadowCnt += 1;
    }
	return;
}


//DONE 93% (regswap)
int NuHGobjRndr(struct NUHGOBJ_s *hgobj,struct numtx_s *wm,int nlayers,short *layers)
{
    struct numtx_s *T;
    int layer;
    int i;
    int flags = 0;
    struct numtx_s *parent_T;
    struct numtx_s* WT;
    char pad[73];

    layer = nlayers + -1;
    if (layer >= hgobj->num_layers) {
        NuErrorProlog("C:/source/crashwoc/code/nu3dx/nurndr.c",0x8a8,"assert");
    }

    for (i = 0; i < hgobj->num_joints; i++) {
        T = &hgobj->T[i];
        WT = &mtx_arrayHGobj[i];
        if (hgobj->joints[i].parent_ix == -1) {
            NuMtxMul(WT,T,wm);
        }
        else {
            NuMtxMul(WT,T,&mtx_arrayHGobj[hgobj->joints[i].parent_ix]);
        }
    }

    if (hgobj->layers[layer].gobjs != NULL && !(flags & 2)) {
        for (i = 0; i < hgobj->num_joints; i++) {
            if (hgobj->layers[layer].gobjs[i] != NULL) {
                NuRndrGobj(hgobj->layers[layer].gobjs[i],&mtx_arrayHGobj[i],NULL);
            }
        }
    }

    if (hgobj->layers[layer].blend_gobjs != NULL && !(flags & 2))  {
        for (i = 0; i < hgobj->num_joints; i++) {
            if (hgobj->layers[layer].blend_gobjs[i] != NULL) {
                NuRndrGobj(hgobj->layers[layer].blend_gobjs[i],&mtx_arrayHGobj[i],NULL);
            }
        }
    }

    for (i = 0; i < hgobj->num_joints; i++) {
        NuMtxMul(&mtx_array2HGobj[i],&hgobj->INV_WT[i],&mtx_arrayHGobj[i]);
    }

    if (hgobj->layers[layer].skin_gobj != NULL && !(flags & 1)) {
        NuRndrGobjSkin2(hgobj->layers[layer].skin_gobj,hgobj->num_joints,mtx_array2HGobj,NULL);
    }

    if (hgobj->layers[layer].blend_skin_gobj != NULL && !(flags & 1)) {
        NuRndrGobjSkin2(hgobj->layers[layer].blend_skin_gobj,hgobj->num_joints,mtx_array2HGobj,NULL);
    }
    return 1;
}


//GCN MATCH
s32 NuHGobjRndrMtxDwa(struct NUHGOBJ_s *hgobj,struct numtx_s *wm,int nlayers,short *layers,struct numtx_s *mtx_array,float **dwa) {
  s32 layer;
  s32 i;
  s32 j;
  s32 rv;
  struct numtx_s mtx;

  rv = 0;
  if (hgobj_enabled == 0) {
       return 1;
  }
  else {
      for(j = 0; j < nlayers; j++) {
        if (layers != NULL) {
          layer = layers[j];
          if ((layer >= 0) && (layer >= hgobj->num_layers)) goto LAB_800b6930;
        }
        else {
          nlayers = 1;
          layer = 0;
LAB_800b6930:
          if ((layer >= 0) && (layer <= hgobj->num_layers)) {
            if ((hgobj->layers[layer].gobjs != NULL))  {
              for(i = 0; i < hgobj->num_joints; i++) {

                if (hgobj->layers[layer].gobjs[i] != 0) {
                  NuMtxMul(&mtx,&mtx_array[i],wm);
                  rv = rv | NuRndrGobj(hgobj->layers[layer].gobjs[i],&mtx,NULL);
                }
              }
            }
            if ((hgobj->layers[layer].blend_gobjs != NULL)) {
              for(i = 0; i < hgobj->num_joints; i++) {

                if (hgobj->layers[layer].blend_gobjs[i] != 0) {
                  NuMtxMul(&mtx,&mtx_array[i],wm);
                  rv = rv | NuRndrGobj(hgobj->layers[layer].blend_gobjs[i],&mtx,NULL);
                }
              }
            }
            if ((hgobj->layers[layer].skin_gobj != NULL) || (hgobj->layers[layer].blend_skin_gobj != NULL)) {
                for(i = 0; i < hgobj->num_joints; i++) {
                  NuMtxMul(&mtx_array2HGobjRndrDwa[i],&hgobj->INV_WT[i],&mtx_array[i]);
                }
                for(i = 0; i < hgobj->num_joints; i++) {
                  NuMtxMul(&mtx_array2HGobjRndrDwa[i],&mtx_array2HGobjRndrDwa[i],wm);
                }
              if (hgobj->layers[layer].skin_gobj != NULL) {
                rv = rv | NuRndrGobjSkin2(hgobj->layers[layer].skin_gobj,hgobj->num_joints,mtx_array2HGobjRndrDwa,NULL);
              }
              if (hgobj->layers[layer].blend_skin_gobj != NULL) {
                rv = rv | NuRndrGobjSkin2(hgobj->layers[layer].blend_skin_gobj,hgobj->num_joints,mtx_array2HGobjRndrDwa,dwa);
              }
            }
          }
        }
      }
  }
  return rv;
}

//GCN MATCH
s32 NuHGobjRndrMtx(struct NUHGOBJ_s *hgobj,struct numtx_s *wm,s32 nlayers,short *layers,struct numtx_s *mtx_array) {
  return NuHGobjRndrMtxDwa(hgobj,wm,nlayers,layers,mtx_array,NULL);
}


//PS2 Match
void NuHGobjEvalAnim(struct NUHGOBJ_s *hgobj,struct nuanimdata_s *animdata,float time,int njanims,struct NUJOINTANIM_s *janim,
                    struct numtx_s *mtx_array)
{
    u8 i;
    u8 joint_ix;
    struct numtx_s *parent_T;
    struct numtx_s *WT;
    struct numtx_s *T;
    struct numtx_s LOCAL_T;
    struct nuvec_s VtxBuffer[256];
    struct nuanimtime_s atime;
    struct nuanimdatachunk_s *chunk;
    struct NUJOINTANIM_s *offset;
    struct NUJOINTANIM_s *janim_array[256];
    u8 cindex;

    // -----------------------------
    struct numtx_s *pnVar1;
    struct nuanimcurveset_s *animcurveset;
    struct NUJOINTDATA_s *jointdata;


    memset(janim_array, 0, sizeof(janim_array));
    VtxBuffer[255].x = 1.0f;
    VtxBuffer[255].y = 1.0f;
    VtxBuffer[255].z = 1.0f;
    NuAnimDataCalcTime(animdata, time, &atime);
    chunk = animdata->chunks[atime.chunk];
    if (njanims != 0) {
        for (i = 0; i < njanims; i++) {
            if (janim[i].joint_id < hgobj->num_joint_ixs){
                joint_ix = hgobj->joint_ixs[janim[i].joint_id];
                if (joint_ix != 0xff) {
                    janim_array[joint_ix] = &janim[i];
                }
            }
        }
    }


    for (i = 0; i < hgobj->num_joints; i++) {
        parent_T = &mtx_array[i];
        if (njanims != 0) {
            offset = janim_array[i];
        }
        else {
            offset = NULL;
        }

        animcurveset = chunk->animcurvesets[i];
        if (animcurveset != NULL) {
            if (((animcurveset->flags & 0x1A) != 0) || ((hgobj->joints[i].parent_ix & 8) != 0)) { //parent_ix_1
                NuAnimCurveSetApplyToJoint2(animcurveset, &atime, &hgobj->joints[i], &VtxBuffer[i],
                &VtxBuffer[hgobj->joints[i].parent_ix], &LOCAL_T, offset);
            }
            else {
                NuAnimCurveSetApplyToJointBasic(animcurveset, &atime, &hgobj->joints[i], &VtxBuffer[i],
                &VtxBuffer[hgobj->joints[i].parent_ix], &LOCAL_T, offset);
            }
            T = &LOCAL_T;
        }
        else {
            T = &hgobj->T[i];
        }

        if (hgobj->joints[i].parent_ix == 0xff) {
            *parent_T = *T;
        }
        else {
            NuMtxMul(parent_T, T, &mtx_array[hgobj->joints[i].parent_ix]);
        }
    }
    return;
}




//NUJOINTANIM_s* janim_arrayHGobjEval2[256]; //global var

//PS2
void NuHGobjEvalAnim2(struct NUHGOBJ_s *hgobj,struct nuanimdata2_s *animdata,float time,s32 njanims,struct NUJOINTANIM_s *janim,struct numtx_s *mtx_array)
{
  u8 numJoints;
  u8 joint_ix;
  char* curveflags;
  struct nuanimcurve2_s* curves;
  struct NUJOINTANIM_s *offset;
  u8 i;
  struct numtx_s *parent_T;
  struct numtx_s* WT;
  struct numtx_s LOCAL_T;
  struct nuvec_s scale_arrayHGobjEval2 [256];

  struct nuanimtime_s atime;
  struct NUJOINTANIM_s *janim_arrayHGobjEval2 [256];
  s32 curvesetflags;

  memset(janim_arrayHGobjEval2,0,sizeof(janim_arrayHGobjEval2));
  scale_arrayHGobjEval2[255].x = 1.0f;
  scale_arrayHGobjEval2[255].y = 1.0f;
  scale_arrayHGobjEval2[255].z = 1.0f;
  NuAnimData2CalcTime(animdata,time,&atime);
  if (njanims != 0) {
        for (i = 0; i < njanims; i++) {
            if (janim[i].joint_id < hgobj->num_joint_ixs){
                joint_ix = hgobj->joint_ixs[janim[i].joint_id];
                if (joint_ix != 0xff) {
                    janim_arrayHGobjEval2[joint_ix] = &janim[i];
                }
            }
        }
    numJoints = hgobj->num_joints;
  }
  else {
    numJoints = hgobj->num_joints;
  }
    for (i = 0; i < hgobj->num_joints; i++) {
      parent_T = &mtx_array[i];

      if (njanims != 0) {
        offset = janim_arrayHGobjEval2[i];
      }
      else {
        offset = 0;
      }
      curvesetflags = animdata->curvesetflags[i];
          curves = &animdata->curves[animdata->ncurves * i];
        curveflags = &animdata->curveflags[animdata->ncurves * i];
        if ((curvesetflags & 0x1a) || (hgobj->joints[i].parent_ix & 8)) { //parent_ix_1
            NuAnimCurve2SetApplyToJoint(curves, curveflags, curvesetflags, &atime,
                &hgobj->joints[i], &scale_arrayHGobjEval2[i],
                &scale_arrayHGobjEval2[hgobj->joints[i].parent_ix], &LOCAL_T, offset);
        } else {
            NuAnimCurve2SetApplyToJointBasic(curves, curveflags, curvesetflags, &atime,
                &hgobj->joints[i], &scale_arrayHGobjEval2[i],
                &scale_arrayHGobjEval2[hgobj->joints[i].parent_ix], &LOCAL_T, offset);
        }
        WT = &LOCAL_T;
      if (hgobj->joints[i].parent_ix == 0xff) {
          *parent_T = *WT;
      }
      else {
        NuMtxMul(parent_T,WT,&mtx_array[hgobj->joints[i].parent_ix]);
      }
    }

  return;
}


//PS2
void NuHGobjEvalAnimBlend (struct NUHGOBJ_s * hgobj, struct nuanimdata_s * animdata1, float time1,
struct nuanimdata_s * animdata2, float time2, float blend, s32 njanims, struct NUJOINTANIM_s * janim,
struct numtx_s * mtx_array)
{
  struct numtx_s *T;
  struct numtx_s* parent_T;
  struct NUJOINTANIM_s *offset;
  u8 i;
  u8 joint_ix;
  struct numtx_s LOCAL_T;
  struct nuvec_s scale_array0 [512];
  struct nuanimtime_s atime1;
  struct nuanimtime_s atime2;
  struct NUJOINTANIM_s *janim_array [256];
  struct nuanimdatachunk_s *chunk1;
  struct nuanimdatachunk_s *chunk2;

  struct nuanimcurveset_s *animcurveset1;

  memset(janim_array,0,0x400);
  scale_array0[255].x = 1.0f;
  scale_array0[255].y = 1.0f;
  scale_array0[255].z = 1.0f;
  //scale_array0[511].x = 1.0f;
  //scale_array0[511].y = 1.0f;
  //scale_array0[511].z = 1.0f;
  NuAnimDataCalcTime(animdata1,time1,&atime1);
  chunk1 = animdata1->chunks[atime1.chunk];
  NuAnimDataCalcTime(animdata2,time2,&atime2);
  chunk2 = animdata2->chunks[atime2.chunk];

          if (njanims != 0) {
        for (i = 0; i < njanims; i++) {
            if (janim[i].joint_id < hgobj->num_joint_ixs){
                joint_ix = hgobj->joint_ixs[janim[i].joint_id];
                if (joint_ix != 0xff) {
                    janim_array[joint_ix] = &janim[i];
                }
            }
        }
    }

    for (i = 0; i < hgobj->num_joints; i++) {
        parent_T = &mtx_array[i];
        if (njanims != 0) {
            offset = janim_array[i];
        }
        else {
            offset = NULL;
        }

        if (chunk1->animcurvesets[i] != NULL) {
        animcurveset1 = chunk1->animcurvesets[i];
        if (chunk2->animcurvesets[i] == NULL) {
           T = &hgobj->T[i];
        }
            else
        {
        NuAnimCurveSetApplyBlendToJoint2
                  (chunk1->animcurvesets[i],&atime1,chunk2->animcurvesets[i],&atime2,
                   blend,&hgobj->joints[i],&scale_array0[i],
                   &scale_array0[hgobj->joints[i].parent_ix],&LOCAL_T,offset);
        T = &LOCAL_T;
        }

      }
      else {

         T = &hgobj->T[i];
      }

        if (hgobj->joints[i].parent_ix == 0xff) {
            *parent_T = *T;
        }
        else {
            NuMtxMul(parent_T, T, &mtx_array[hgobj->joints[i].parent_ix]); //NuMtxMulVU0
        }
    }
  return;
}

//PS2 MATCH
void NuHGobjEval(struct NUHGOBJ_s *hgobj, s32 njanims, struct NUJOINTANIM_s *janim, struct numtx_s *mtx_array)
{
    u8 i;
    u8 joint_ix;
    struct NUJOINTANIM_s *janimptr;
    struct numtx_s *m;
    struct numtx_s *m_00;
    struct nuvec_s scale_arrayHGobjEval2 [256];
    struct NUJOINTANIM_s *janim_array [256];
    struct nuangvec_s rf;
    struct numtx_s auStack_130;
    struct numtx_s NStack_f0;
    struct numtx_s *t3;
    struct NUJOINTANIM_s *offset;

    memset(janim_array, 0, sizeof(janim_array));

    scale_arrayHGobjEval2[255].x = 1.0f;
    scale_arrayHGobjEval2[255].y = 1.0f;
    scale_arrayHGobjEval2[255].z = 1.0f;
    if (njanims != 0) {
        for (i = 0; i < njanims; i++) {
            if (janim[i].joint_id < hgobj->num_joint_ixs){
                joint_ix = hgobj->joint_ixs[janim[i].joint_id];
                if (joint_ix != 0xff) {
                    janim_array[joint_ix] = &janim[i];
                }
            }
        }
    }

    for (i = 0; i < hgobj->num_joints; i++) {
        m_00 = &mtx_array[i];
        if (njanims != 0) {
            offset = janim_array[i];
        }
        else {
            offset = NULL;
        }

        if (offset != NULL) {
            m = &auStack_130;
            auStack_130 = hgobj->T[i];
            rf.x = (s32)(offset->rx * DEG_TO_FIXED_POINT);
            rf.y = (s32)(offset->ry * DEG_TO_FIXED_POINT);
            rf.z = (s32)(offset->rz * DEG_TO_FIXED_POINT);
            NuMtxSetRotateXYZ(&NStack_f0, &rf); //NuMtxSetRotateXYZVU0
            NuMtxTranslate(&NStack_f0, &offset->tx);
            NuMtxMul(m, &NStack_f0, m); //NuMtxMulVU0
        }
        else {
            m = &hgobj->T[i];
        }

        if (hgobj->joints[i].parent_ix == 0xff) {
            *m_00 = *m;
        }
        else {
            NuMtxMul(m_00, m, &mtx_array[hgobj->joints[i].parent_ix]); //NuMtxMulVU0
        }
    }
    return;
}
