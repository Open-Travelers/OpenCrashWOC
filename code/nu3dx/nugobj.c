#include <nu3dx/nugobj.h>

error_func e;
u32 GS_BufferSize;
static s32 sysinit;
s32 Paused;
s32 _timer;
u32 BufferTypes[4];


void NuGobjInit(void) {

    if ((s32) sysinit != 0) {
        NuGobjClose();
    }
    sysgobj = NULL;
    sysinit = 1; //sysinit is a bool


    //NuLightFog(1.0, 1.0, 0, 0, 0);

    return;
}

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


void NuGobjDestroy(struct nugobj_s* obj)

{
    struct nugobj_s* next_gobj;
    struct nugeom_s* next;
    struct nugeom_s* prev;

    next_gobj = obj->next_gobj;
    prev = obj->geom;
    while (prev != NULL) {
        next = prev->next;
        NuGeomDestroy(prev);
        prev = next;
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


void NuGobjAddGeom(struct nugobj_s* gobj, struct nugeom_s* geom)
{
  if (gobj->geom == NULL) {
    gobj->geom = geom;
    return;
  }

  struct nugeom_s* last = gobj->geom;
  while (last->next != NULL) {
    last = last->next;
  }
  last->next = geom;
  return;
}

/*void NuGobjAddGeom(struct nugobj_s* gobj, struct nugeom_s* geom) {
    struct nugeom_s* g = gobj->geoms;
    struct nugeom_s* last = NULL;

    if (g != NULL) {
        do {
            last = g;
            g = last->next;
        } while (g != NULL);
    }
    if (last != NULL) {
        last->next = geom;
        return;
    }
    gobj->geoms = geom;
}
}*/

/*void NuGobjAddFaceOnGeom(struct nugobj_s* gobj, struct nufaceongeom_s* geom) {
  if (gobj->faceon_geom == NULL) {
    gobj->faceon_geom = geom;
    return;
  }

  struct nufaceongeom_s* last = gobj->faceon_geom;
  while (last->next != NULL) {
    last = last->next;
  }
  last->next = geom;
  return;
}*/

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
}

void NuGobjCalcFaceOnDims(struct nugobj_s* gobj) {
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  struct nuvec_s *pnt;
  //struct nufaceon_s *faceon;
  s32 iVar5;
  s32 iVar6;
  struct nufaceongeom_s *face1;
  double dVar7;
  float fVar8;
  float fVar9;
  struct nuvec_s v;
  float fvar10;
  struct nufaceongeom_s* fgeom;
  float pointX;

  gobj->bounding_box_min.z = __FLT_MAX__;
  gobj->bounding_box_max.z = __FLT_MIN__;
  gobj->bounding_rsq_from_origin = 0.0;
  gobj->bounding_box_min.x = __FLT_MAX__;
  gobj->bounding_box_min.y = __FLT_MAX__;
  gobj->bounding_box_max.x = __FLT_MIN__;
  gobj->bounding_box_max.y = __FLT_MIN__;
  for (fgeom = gobj->faceon_geom; fgeom != NULL; fgeom = fgeom->next) {
    int count = 0;
    if (fgeom->nfaceons > 0) {
      // Seems to be used as an index for an array? Increments by 24 each iteration.
      int size = 0;
      do {
        struct nufaceon_s* faceon = fgeom->faceons;
        float fonHeight = *(float *)((int)&faceon->height + size);
        float fonWidth = *(float *)((int)&faceon->width + size);
	// Clamp width to height.
        if (fonWidth < fonHeight) {
          fonWidth = fonHeight;
        }
        fonHeight = *(float *)((int)&faceon->point.x + size) - fonWidth;
        if (fonHeight < gobj->bounding_box_min.x) {
          gobj->bounding_box_min.x = fonHeight;
        }
        fonHeight = *(float *)((int)&faceon->point.y + size) - fonWidth;
        if (fonHeight < gobj->bounding_box_min.y) {
          gobj->bounding_box_min.y = fonHeight;
        }
        fonHeight = *(float *)((int)&faceon->point.z + size) - fonWidth;
        if (fonHeight < gobj->bounding_box_min.z) {
          gobj->bounding_box_min.z = fonHeight;
        }
        fonHeight = *(float *)((int)&faceon->point.x + size) + fonWidth;
        if (gobj->bounding_box_max.x < fonHeight) {
          gobj->bounding_box_max.x = fonHeight;
        }
        fonHeight = *(float *)((int)&faceon->point.y + size) + fonWidth;
        if (gobj->bounding_box_max.y < fonHeight) {
          gobj->bounding_box_max.y = fonHeight;
        }
        fonWidth = *(float *)((int)&faceon->point.z + size) + fonWidth;
        if (gobj->bounding_box_max.z < fonWidth) {
          gobj->bounding_box_max.z = fonWidth;
        }
        fonHeight = *(float *)((int)&faceon->point.y + size);
        pointX = *(float *)((int)&faceon->point.x + size);
        fonWidth = *(float *)((int)&faceon->point.z + size);
	// z^2 + x^2 + y^2
        fonHeight = fonWidth * fonWidth + pointX * pointX + fonHeight * fonHeight;
	// Set the radius squared(?) from the origin point, if it has changed.
        if (gobj->bounding_rsq_from_origin < fonHeight) {
          gobj->bounding_rsq_from_origin = fonHeight;
        }
        count = count + 1;
        size = size + 24;
      } while (count < fgeom->nfaceons);
    }
  }
  fVar8 = sqrt(gobj->bounding_rsq_from_origin);
  fVar9 = (gobj->bounding_box_max).x;
  fvar10 = (gobj->bounding_box_min).x;
  fVar4 = (gobj->bounding_box_max).y;
  fVar1 = (gobj->bounding_box_min).y;
  fVar2 = (gobj->bounding_box_min).z;
  fVar3 = (gobj->bounding_box_max).z;
  face1 = gobj->faceon_geom;
  gobj->bounding_radius_from_origin = fVar8;
  (gobj->bounding_box_center).x = (fvar10 + fVar9) * 0.5;
  (gobj->bounding_box_center).y = (fVar1 + fVar4) * 0.5;
  (gobj->bounding_box_center).z = (fVar2 + fVar3) * 0.5;
  gobj->bounding_rsq_from_center = (float)0;
  for (; face1 != NULL; face1 = face1->next) {
    iVar6 = 0;
    if (0 < face1->nfaceons) {
      iVar5 = 0;
      do {
        pnt = (struct nuvec_s *)((int)&(face1->faceons->point).x + iVar5);
        dVar7 = (double)pnt[1].x;
        if ((double)pnt[1].x < (double)pnt[1].y) {
          dVar7 = (double)pnt[1].y;
        }
        NuVecSub(&v,pnt,&gobj->bounding_box_center);
        if (v.x < (float)0) {
          v.x = -v.x;
        }
        if (v.y < (float)0) {
          v.y = -v.y;
        }
        if (v.z < (float)0) {
          v.z = -v.z;
        }
        fVar9 = (float)((double)v.z + dVar7) * (float)((double)v.z + dVar7) +
                (float)((double)v.x + dVar7) * (float)((double)v.x + dVar7) +
                (float)((double)v.y + dVar7) * (float)((double)v.y + dVar7);
        if (gobj->bounding_rsq_from_center < fVar9) {
          gobj->bounding_rsq_from_center = fVar9;
        }
        iVar6 = iVar6 + 1;
        iVar5 = iVar5 + 0x18;
      } while (iVar6 < face1->nfaceons);
    }
  }
  fVar9 = sqrt(gobj->bounding_rsq_from_center);
  gobj->bounding_radius_from_center = fVar9;
  return;
}


void NuGobjCalcDims(struct nugobj_s* gobj)
{
  if (gobj->culltype == 1) {
    NuGobjCalcFaceOnDims(gobj);
    return;
  }
  gobj->bounding_box_min.x = __FLT_MAX__;
  gobj->bounding_box_min.y = __FLT_MAX__;
  gobj->bounding_box_min.z = __FLT_MAX__;
  gobj->bounding_box_max.x = __FLT_MIN__;
  gobj->bounding_box_max.y = __FLT_MIN__;
  gobj->bounding_box_max.z = __FLT_MIN__;
  gobj->bounding_rsq_from_origin = 0.0;
  for (struct nugeom_s* geom = gobj->geom; geom != NULL; geom = geom->next) {
    s32 stride = NuVtxStride(geom->vtxtype);
    s8* vertex_raw = geom->hVB;
    if (vertex_raw == NULL) {
	    //error_func e = NuErrorProlog("C:/source/crashwoc/code/nu3dx/nugobj.c",0x13b);
		//e("NuGobjCalcDims : Lock VB failed!");
    }
    s8* end = vertex_raw + stride * geom->vtxcnt;
    for (; vertex_raw < end; vertex_raw += stride) {
      struct nuvec_s* vertex = (struct nuvec_s*)vertex_raw;
      if (vertex->x < gobj->bounding_box_min.x) {
        gobj->bounding_box_min.x = vertex->x;
      }
      if (vertex->y < gobj->bounding_box_min.y) {
        gobj->bounding_box_min.y = vertex->y;
      }
      if (vertex->z < gobj->bounding_box_min.z) {
        gobj->bounding_box_min.z = vertex->z;
      }
      if (gobj->bounding_box_max.x < vertex->x) {
        gobj->bounding_box_max.x = vertex->x;
      }
      if (gobj->bounding_box_max.y < vertex->y) {
        gobj->bounding_box_max.y = vertex->y;
      }
      if (gobj->bounding_box_max.z < vertex->z) {
        gobj->bounding_box_max.z = vertex->z;
      }
      f32 rsq = vertex->z * vertex->z + vertex->x * vertex->x + vertex->y * vertex->y;
      if (gobj->bounding_rsq_from_origin < rsq) {
        gobj->bounding_rsq_from_origin = rsq;
      }
    }
  }
  gobj->bounding_radius_from_origin = (f32)sqrt((double)gobj->bounding_rsq_from_origin);

  gobj->bounding_box_center.x = (gobj->bounding_box_min.x + gobj->bounding_box_max.x) * 0.5;
  gobj->bounding_box_center.y = (gobj->bounding_box_min.y + gobj->bounding_box_max.y) * 0.5;
  gobj->bounding_box_center.z = (gobj->bounding_box_min.z + gobj->bounding_box_max.z) * 0.5;

  gobj->bounding_rsq_from_center = 0.0;
  for (struct nugeom_s* geom = gobj->geom; geom != NULL; geom = geom->next) {
    s32 stride = NuVtxStride(geom->vtxtype);
    s8* vertex_raw = geom->hVB;
    if (vertex_raw == NULL) {
	  //error_func e = NuErrorProlog("C:/source/crashwoc/code/nu3dx/nugobj.c",0x157);
		//e("NuGobjCalcDims : Lock VB failed!");
    }
    s8* end = vertex_raw + stride * geom->vtxcnt;
    for (; vertex_raw < end; vertex_raw += stride) {
      struct nuvec_s* vertex = (struct nuvec_s*)vertex_raw;
      struct nuvec_s diff;
      NuVecSub(&diff, vertex, gobj->bounding_box_center);
      f32 rsq = diff.z * diff.z + diff.x * diff.x + diff.y * diff.y;
      if (gobj->bounding_rsq_from_center < rsq) {
        gobj->bounding_rsq_from_center = rsq;
      }
    }
  }
  gobj->bounding_radius_from_center = (f32)sqrt((double)gobj->bounding_rsq_from_center);
  return;
}



/**********************************************NUGEOM STUFF*************************************/

struct nugeom_s* NuGeomCreate(void)
{
    struct nugeom_s* geom;

    geom = (struct nugeom_s*)NuMemAlloc(0x30);
    memset(geom,0, 0x30);
    return geom;
}

struct nufaceongeom_s* NuFaceOnGeomCreate(void)
{
  struct nufaceongeom_s* ret = (struct nufaceongeom_s*)NuMemAlloc(sizeof(struct nufaceongeom_s)); //0x30
  memset(ret, 0, sizeof(struct nufaceongeom_s));
  return ret;
}



void NuGeomDestroy(struct nugeom_s *geom)
{
  NuGeomDestroyVB(geom);
  for (struct nuprim_s* prim = geom->prim; prim != NULL; prim = prim->next) {
    NuPrimDestroy(prim);
  }


  if (geom->blendgeom != NULL) {
    int buffer;
    if (buffer = geom->blendgeom->hVB, buffer != 0) {
      GS_DeleteBuffer(buffer);
    }
  }

  return;
}


// Create geometry vertex buffer
void NuGeomCreateVB(struct nugeom_s* geom, u32 vtxCount, enum nuvtxtype_e vtxType, s32 dynamic)
{
        // Boolean argument is unused
    #pragma unused(dynamic);

    u32 vtxSize;
    struct GS_Buffer* vtxBuffer;

    switch(vtxType) {

			//determining the vertex buffer size (vertex count * sizeof(buffer_type_element)
    case NUVT_PS:
        vtxSize = vtxCount * 0x10;
        break;
    case NUVT_TC1:
        vtxSize = vtxCount * 0x24;
        break;
    case NUVT_LC1:
        vtxSize = vtxCount * 0x18;
        break;
    case NUVT_TLTC1:
        vtxSize = vtxCount * 0x1C;
        break;
    case NUVT_SK3TC1:
        vtxSize = vtxCount * 0x38;
        break;
    default:
        //"NuGeomCreateVB : Unknown vertex type!"
        e = NuErrorProlog("OpenCrashWOC/code/nu3dx/nugobj.c", 441);
        e("assert");
    }

    if (geom->hVB != 0)
    {
        //NuAssert(geom->vertex_buffer == NULL, "NuGeomCreateVB : geom already has VB");
        e = NuErrorProlog("OpenCrashWOC/code/nu3dx/nugobj.c", 448);
        e("assert");
    }


    // Second argument is some vertex type
    vtxBuffer = GS_CreateBuffer(vtxSize, 1);


    geom->vtxmax = vtxCount;
    geom->hVB = (int)vtxBuffer;
    geom->vtxtype = vtxType;
    geom->vtxcnt = vtxCount;
    return;
}

void NuGeomDestroyVB(struct nugeom_s *geom)

{
	if(geom->hVB != NULL){
		GS_DeleteBuffer(geom->hVB);
        geom->hVB = 0;
		}
	return;
}

// Append prim to geom
void NuGeomAddPrim(struct nugeom_s* geom, struct nuprim_s* prim)
{
    struct nuprim_s *head;
    struct nuprim_s *tail;
    struct nuprim_s *iter;

    head = geom->prim;
    tail = NULL;

    // When iter is NULL, tail will be the last non-NULL prim (list tail)
    for (iter = head; iter != NULL; iter = iter->next)
    {
        tail = iter;
    }

    // I thought prims was the list head???
    // Not sure what's going on here
    if (tail == NULL) {
        geom->prim = prim;
        return;
    }

    // Append prim (NULL tail = empty list)
    tail->next = prim;
    return;
}


// Append skin to geom
void NuGeomAddSkin(struct nugeom_s* geom, struct nuskin_s* skin)
{
    struct nuskin_s *head;
    struct nuskin_s *tail;
    struct nuskin_s *iter;

    head = geom->skin;
    tail = NULL;

    // When iter is NULL, tail will be the last non-NULL skin (list tail)
    for (iter = head; iter != NULL; iter = iter->next)
    {
        tail = iter;
    }


     // I thought skins was the list head???
    // Not sure what's going on here
    if (tail == NULL)
    {
        geom->skin = skin;
        return;

    }

    // Append skin (NULL tail = empty list)
     tail->next = skin;
     return;
}


/**********************************************NUPRIM STUFF*************************************/

struct nuprim_s* NuPrimCreate(int amount, enum nuprimtype_e type) {
    u16* data;
    struct nuprim_s *prim;

    prim = NuMemAlloc(0x3C);
    memset(prim,0,0x3C);
    prim->type = type;
    prim->vertexCount = (u16)amount;
    prim->max = (u16)amount;
    if (type > NUPT_TRI) {
        data = (u16*)(amount * 2);
        prim->vid = NuMemAlloc((s32)data);
        prim->idxbuff = (int)GS_CreateBuffer((u32)data, 2);
    }
    return prim;
}


void NuPrimDestroy(struct nuprim_s* prim) {
    if ((prim != NULL) && ((s32)prim->idxbuff != 0)) {
        GS_DeleteBuffer((void*)prim->idxbuff);
        prim->idxbuff = 0;
    }
	return;
}

//BufferTypes is int[4], GS_BufferSize is uint
void* GS_CreateBuffer (u32 bufsize, s32 bufferType){
    struct GS_Buffer *bufptr;

    bufptr = (struct GS_Buffer*)malloc(bufsize + 8);
	GS_BufferSize = GS_BufferSize + bufsize;
	BufferTypes[bufferType] = BufferTypes[bufferType] + bufsize;
    bufptr->size = bufsize;
    bufptr->type = bufferType;
	return bufptr + 1;
}

void GS_DeleteBuffer(void* ptr)
{
    struct GS_Buffer* bufptr = (struct GS_Buffer*)((int)ptr - 2);
    GS_BufferSize -= bufptr->size;
    BufferTypes[bufptr->type] -= bufptr->size;
    free(bufptr);
}


// Vertex stride = size of 1 vertex element
int NuVtxStride(enum nuvtxtype_e type)
{
    switch (type)
    {
    case NUVT_PS:
        return 0x10;
    case NUVT_LC1:
        return 0x18;
    case NUVT_TLTC1:
        return 0x1C;
    case NUVT_SK3TC1:
        return 0x38;
    case NUVT_TC1:
        return 0x24;
    }

    NuErrorProlog("NuVtxStride: Unknown vertex type", 441);
}

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

