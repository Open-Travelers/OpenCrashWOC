#include <nu3dx/nugobj.h>

#define FLT_MAX 3.402823e+38
#define FLT_MIN -3.402823e+38

static s32 sysinit;
s32 Paused;
s32 _timer;

//MATCH GCN
void NuGobjInit(void) {

    if ((s32) sysinit != 0) {
        NuGobjClose();
    }
    sysgobj = NULL;
    sysinit = 1; //sysinit is a bool
    NuLightFog(1.0f, 1.0f, 0, 0, 0);

    return;
}

//MATCH GCN
void NuGobjClose(void) {

    struct nugobj_s* nextobj;

    if ((sysinit != 0) && (sysgobj != NULL))
    {
        do {
            nextobj = sysgobj->sysnext;
            NuGobjDestroy(sysgobj);
            sysgobj = nextobj;
        } while (nextobj != NULL);
    }
    return;
}

//MATCH GCN
struct nugobj_s* NuGobjCreate(void)
{
    struct nugobj_s* gobj = (struct nugobj_s*)NuMemAlloc(sizeof(struct nugobj_s)); //0x64;
    memset(gobj, 0, sizeof(struct nugobj_s));


    if (sysgobj != NULL) {
        sysgobj->syslast = gobj;
    }
    gobj->syslast = NULL;
    gobj->sysnext = sysgobj;
    gobj->culltype = 1;
    sysgobj = gobj;
    return gobj;
}

//MATCH GCN
void NuGobjDestroy(struct nugobj_s* obj) {

    struct nugobj_s* next_gobj;
    struct nugeom_s* next;
    struct nugeom_s* geom;

    next_gobj = obj->next_gobj;
    geom = obj->geom;
    while (geom != NULL) {
        next = geom->next;
        NuGeomDestroy(geom);
        geom = next;
    }
    if (obj->syslast != NULL) {
        obj->syslast->sysnext = obj->sysnext;
    }
    if (obj->sysnext != NULL) {
        obj->sysnext->syslast = obj->syslast;
    }
    if (next_gobj != NULL) {
        NuGobjDestroy(next_gobj);
    }
    return;
}

//MATCH GCN
void NuGobjAddGeom(struct nugobj_s* gobj, struct nugeom_s* geom)
{
    struct nugeom_s* last;
    struct nugeom_s* next;

    last = gobj->geom;
    next = NULL;
    while (last != NULL) {
        next = last;
        last = last->next;
    }
    if (next != NULL) {
        next->next = geom;
        return;
    }

    gobj->geom = geom;
    return;
}

//MATCH GCN
void NuGobjAddFaceOnGeom(struct nugobj_s* gobj, struct nufaceongeom_s* Fgeom) {
    struct nufaceongeom_s* face = gobj->faceon_geom;
    struct nufaceongeom_s* last = NULL;

    while (face != NULL) {
         last = face;
         face = last->next;
    }
    if (last != NULL) {
        last->next = Fgeom;
        return;
    }
    gobj->faceon_geom = Fgeom;
    return;
}

//MATCH GCN
void NuGobjCalcFaceOnDims(struct nugobj_s *gobj)
{
  int i;
  float r2;
  float fonWidth;
  struct nuvec_s v;
  struct nufaceongeom_s *fgeom;
  struct nuvec_s *pnt;

  struct nufaceon_s* faceon;
  float fonHeight;
     char pad[6];

  gobj->bounding_box_min.x = FLT_MAX;
  gobj->bounding_box_min.y = FLT_MAX;
  gobj->bounding_box_min.z = FLT_MAX;
  gobj->bounding_box_max.x = FLT_MIN;
  gobj->bounding_box_max.y = FLT_MIN;
  gobj->bounding_box_max.z = FLT_MIN;
  gobj->bounding_rsq_from_origin = 0.0f;
  for (fgeom = gobj->faceon_geom; fgeom != NULL; fgeom = fgeom->next) {
      for( i = 0; i < fgeom->nfaceons; i++) {

        faceon = &fgeom->faceons[i];
        pnt = &faceon->point;
        fonHeight = faceon->height;
        fonWidth = faceon->width;
	    // Clamp width to height.
        if (faceon->height > fonWidth) {
          fonWidth = fonHeight;
        }
       fonHeight = (pnt->x) - fonWidth;
        if (fonHeight < gobj->bounding_box_min.x) {
          gobj->bounding_box_min.x = fonHeight;
        }
        fonHeight = (pnt->y ) - fonWidth;
        if (fonHeight < gobj->bounding_box_min.y) {
          gobj->bounding_box_min.y = fonHeight;
        }
        fonHeight = (pnt->z ) - fonWidth;
        if (fonHeight < gobj->bounding_box_min.z) {
          gobj->bounding_box_min.z = fonHeight;
        }
        fonHeight = (pnt->x ) + fonWidth;
        if (fonHeight > gobj->bounding_box_max.x) {
          gobj->bounding_box_max.x = fonHeight;
        }
        fonHeight = (pnt->y) + fonWidth;
        if (fonHeight > gobj->bounding_box_max.y) {
          gobj->bounding_box_max.y = fonHeight;
        }
        fonHeight = (pnt->z) + fonWidth;
        if (fonHeight > gobj->bounding_box_max.z) {
          gobj->bounding_box_max.z = fonHeight;
        }
	// z^2 + x^2 + y^2
        fonHeight = pnt->x * pnt->x + pnt->y * pnt->y + pnt->z * pnt->z;
	// Set the radius squared(?) from the origin point, to
        if (fonHeight > gobj->bounding_rsq_from_origin) {
          gobj->bounding_rsq_from_origin = fonHeight;
        }
      }
  }
  r2 = sqrt(gobj->bounding_rsq_from_origin);
  gobj->bounding_radius_from_origin = r2;
  (gobj->bounding_box_center).x = ((gobj->bounding_box_min).x + (gobj->bounding_box_max).x) * 0.5f;
  (gobj->bounding_box_center).y = ((gobj->bounding_box_min).y + (gobj->bounding_box_max).y) * 0.5f;
  (gobj->bounding_box_center).z = ((gobj->bounding_box_min).z + (gobj->bounding_box_max).z) * 0.5f;
  gobj->bounding_rsq_from_center = 0.0f;
  for (fgeom = gobj->faceon_geom; fgeom != NULL; fgeom = fgeom->next) {
      for (i = 0; i < fgeom->nfaceons; i++) {

        pnt = &fgeom->faceons[i].point;
        fonWidth = pnt[1].x;
        if (pnt[1].y > pnt[1].x) {
          fonWidth = pnt[1].y;
        }
        NuVecSub(&v,pnt,&gobj->bounding_box_center);
        if (v.x < 0.0f) {
          v.x = -v.x;
        }
        if (v.y < 0.0f) {
          v.y = -v.y;
        }
        if (v.z < 0.0f) {
          v.z = -v.z;
        }
        fonHeight =  (v.x + fonWidth) * (v.x + fonWidth) + (v.y + fonWidth) * (v.y + fonWidth) + (v.z + fonWidth) * (v.z + fonWidth);
        if (fonHeight > gobj->bounding_rsq_from_center) {
          gobj->bounding_rsq_from_center = fonHeight;
        }
      }
  }
  r2 = sqrt(gobj->bounding_rsq_from_center);
  gobj->bounding_radius_from_center = r2;
  return;
}

//MATCH GCN
void NuGobjCalcDims(struct nugobj_s *gobj) {

    struct nugeom_s *geom;
    struct nuvec_s v;
    struct nuvtx_tc1_s *ptr;
    char *end;
    s32 stride;
    float r2;
    float rsq;

    geom = gobj->geom;
    if (gobj->type == NUGOBJ_FACEON) {
        NuGobjCalcFaceOnDims(gobj);
    }
    else {
        gobj->bounding_box_min.x = FLT_MAX;
        gobj->bounding_box_min.y = FLT_MAX;
        gobj->bounding_box_min.z = FLT_MAX;
        gobj->bounding_box_max.x = FLT_MIN;
        gobj->bounding_box_max.y = FLT_MIN;
        gobj->bounding_box_max.z = FLT_MIN;
        gobj->bounding_rsq_from_origin = 0.0f;
        for (; geom != NULL; geom = geom->next) {
            stride = NuVtxStride(geom->vtxtype);
            ptr = (struct nuvtx_tc1_s *)geom->hVB;
            if (ptr == NULL) {
                NuErrorProlog("C:/source/crashwoc/code/nu3dx/nugobj.c",0x13b,"NuGobjCalcDims : Lock VB failed!");
            }
            end = (char*)ptr + stride * geom->vtxcnt;
            for (; (char*)ptr < end; ptr = (struct nuvtx_tc1_s *)((int)ptr + stride)) {
                if ((ptr->pnt).x < (gobj->bounding_box_min).x) {
                    (gobj->bounding_box_min).x = (ptr->pnt).x;
                }
                if ((ptr->pnt).y < (gobj->bounding_box_min).y) {
                    (gobj->bounding_box_min).y = (ptr->pnt).y;
                }
                if ((ptr->pnt).z < (gobj->bounding_box_min).z) {
                    (gobj->bounding_box_min).z = (ptr->pnt).z;
                }
                if ((ptr->pnt).x > (gobj->bounding_box_max).x) {
                    (gobj->bounding_box_max).x = (ptr->pnt).x;
                }
                if ((ptr->pnt).y > (gobj->bounding_box_max).y) {
                    (gobj->bounding_box_max).y = (ptr->pnt).y;
                }
                if ((ptr->pnt).z > (gobj->bounding_box_max).z) {
                    (gobj->bounding_box_max).z = (ptr->pnt).z;
                }
                rsq =  (ptr->pnt).x * (ptr->pnt).x + (ptr->pnt).y * (ptr->pnt).y
                       + ((ptr->pnt).z * (ptr->pnt).z);
                if (rsq > gobj->bounding_rsq_from_origin) {
                    gobj->bounding_rsq_from_origin = rsq;
                }
            }
        }
        r2 = sqrt(gobj->bounding_rsq_from_origin);
        geom = gobj->geom;
        gobj->bounding_radius_from_origin = r2;
        (gobj->bounding_box_center).x = ((gobj->bounding_box_min).x + (gobj->bounding_box_max).x) * 0.5f;
        (gobj->bounding_box_center).y = ((gobj->bounding_box_min).y + (gobj->bounding_box_max).y) * 0.5f;
        (gobj->bounding_box_center).z = ((gobj->bounding_box_min).z + (gobj->bounding_box_max).z) * 0.5f;
        gobj->bounding_rsq_from_center = 0.0f;
        for (; geom != NULL; geom = geom->next) {
            stride = NuVtxStride(geom->vtxtype);
            ptr = (struct nuvtx_tc1_s *)geom->hVB;
            if (ptr == NULL) {
                NuErrorProlog("C:/source/crashwoc/code/nu3dx/nugobj.c",0x157,"NuGobjCalcDims : Lock VB failed!");
            }
            end = (char*)ptr + stride * geom->vtxcnt;
            for (; (char*)ptr < end; ptr = (struct nuvtx_tc1_s *)((int)ptr + stride)) {
                NuVecSub(&v,&ptr->pnt,&gobj->bounding_box_center);
                rsq = v.x * v.x + v.y * v.y + v.z * v.z;
                if (rsq > gobj->bounding_rsq_from_center) {
                    gobj->bounding_rsq_from_center = rsq;
                }
            }
        }
        gobj->bounding_radius_from_center = sqrt(gobj->bounding_rsq_from_center);
    }
    return;
}

//MATCH GCN
struct nugeom_s* NuGeomCreate(void)
{
    struct nugeom_s* geom;

    geom = (struct nugeom_s*)NuMemAlloc(0x30);
    memset(geom,0, 0x30);
    return geom;
}

//MATCH GCN
struct nufaceongeom_s* NuFaceOnGeomCreate(void)
{
  struct nufaceongeom_s* ret = (struct nufaceongeom_s*)NuMemAlloc(sizeof(struct nufaceongeom_s)); //0x30
  memset(ret, 0, sizeof(struct nufaceongeom_s));
  return ret;
}

//MATCH GCN
void NuGeomDestroy(struct nugeom_s *geom) {
    struct nuprim_s* prim;
    struct nuprim_s* next;

  prim = geom->prim;
  NuGeomDestroyVB(geom);
  while (prim != NULL) {
        next = prim->next;
        NuPrimDestroy(prim);
        prim = next;
  }
  if (geom->blendgeom != NULL) {
    if (geom->blendgeom->hVB != 0) {
      GS_DeleteBuffer((struct GS_Buffer*)geom->blendgeom->hVB);
    }
  }
  return;
}


//MATCH GCN
// Create geometry vertex buffer
void NuGeomCreateVB(struct nugeom_s* geom, s32 vtxCount, enum nuvtxtype_e vtxType, s32 dynamic)
{
        // Boolean argument is unused
    #pragma unused(dynamic);

    u32 nuvbdesc_FVF;
    s32 vtxBuffer;

    switch(vtxType) {
			//determining the vertex buffer size (vertex count * sizeof(buffer_type_element)
    case NUVT_TC1:
        nuvbdesc_FVF = vtxCount * 0x24;
        break;
    case NUVT_LC1:
        nuvbdesc_FVF = vtxCount * 0x18;
        break;
    case NUVT_SK3TC1:
        nuvbdesc_FVF = vtxCount * 0x38;
        break;
    case NUVT_TLTC1:
        nuvbdesc_FVF = vtxCount * 0x1C;
        break;
    case NUVT_PS:
        nuvbdesc_FVF = vtxCount * 0x10;
        break;
    default:
        //"NuGeomCreateVB : Unknown vertex type!"
        NuErrorProlog("OpenCrashWOC/code/nu3dx/nugobj.c", 441, "assert");
    }

    if (geom->hVB != 0)
    {
        //NuAssert(geom->vertex_buffer == NULL, "NuGeomCreateVB : geom already has VB");
        NuErrorProlog("OpenCrashWOC/code/nu3dx/nugobj.c", 448, "assert");
    }


    // Second argument is some vertex type
    vtxBuffer = GS_CreateBuffer(nuvbdesc_FVF, 1);


    geom->vtxcnt = vtxCount;
    geom->hVB = (int)vtxBuffer;
    geom->vtxtype = vtxType;
    geom->vtxmax = vtxCount;
    return;
}

//MATCH GCN
void NuGeomDestroyVB(struct nugeom_s *geom) {
	if(geom->hVB != NULL){
		GS_DeleteBuffer((struct GS_Buffer*)geom->hVB);
        geom->hVB = 0;
		}
	return;
}

//MATCH GCN
// Append prim to geom
void NuGeomAddPrim(struct nugeom_s* geom, struct nuprim_s* prim)
{
    struct nuprim_s *last;
    struct nuprim_s *next;

    last = NULL;

    // When next is NULL, last will be the last non-NULL prim (list last)
    for (next = geom->prim; next != NULL; next = next->next)
    {
        last = next;
    }


    if (last != NULL) {
        last->next = prim;
        return;
    }

    geom->prim = prim;
    return;
}

//MATCH GCN
// Append skin to geom
void NuGeomAddSkin(struct nugeom_s* geom, struct nuskin_s* skin)
{
    struct nuskin_s *last;
    struct nuskin_s *next;

    last = NULL;

    // When next is NULL, last will be the last non-NULL skin (list last)
    for (next = geom->skin; next != NULL; next = next->next)
    {
        last = next;
    }

    if (last != NULL)
    {
        last->next = skin;
        return;

    }

     geom->skin = skin;
     return;
}


//MATCH GCN
struct nuprim_s* NuPrimCreate(int cnt, enum nuprimtype_e type) {
    struct nuprim_s *prim;

    prim = NuMemAlloc(0x3C);
    memset(prim,0,0x3C);
    prim->type = type;
    prim->cnt = (u16)cnt;
    prim->max = (u16)cnt;
    if (type > NUPT_TRI) {
        prim->vid = NuMemAlloc(cnt * 2);
        prim->idxbuff = GS_CreateBuffer(cnt * 2, 2);
    }
    return prim;
}

//MATCH GCN
void NuPrimDestroy(struct nuprim_s* prim) {
    if ((prim != NULL) && (prim->idxbuff != 0)) {
        GS_DeleteBuffer((void*)prim->idxbuff);
        prim->idxbuff = 0;
    }
	return;
}

//MATCH GCN
// Vertex stride = size of 1 vertex element
int NuVtxStride(enum nuvtxtype_e type)
{
    switch (type)
    {
    case NUVT_TC1:
        return 0x24;
    case NUVT_SK3TC1:
        return 0x38;
    case NUVT_TLTC1:
        return 0x1C;
    case NUVT_LC1:
        return 0x18;
    case NUVT_PS:
        return 0x10;
    default:
        NuErrorProlog("NuVtxStride: Unknown vertex type", 0x708)("NuVtxStride: Unknown vertex type!");
        return 0;
    }
}

//MATCH GCN
// UV animation for all gobjs
void NuAnimUV(void)
{
    struct nugobj_s* current;

    _timer++;

    // Equivalent to (timer % 2 == 0). 30 fps animation?
    if ((_timer & 1) == 0)
    {
        if (!Paused && sysinit)
        {
            // Animate all gobjs
            for (current = sysgobj; current != NULL; current = current->sysnext)
            {
                //NuMtlUVAnimation(current);
            }
        }
    }
    return;
}
