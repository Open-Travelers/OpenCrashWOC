#include "numtl.h"

struct nusysmtl_s* smlist;
struct numtl_s* numtl_white;
s32 nummtls = 0;
s32 wateritem_cnt;
s32 stenitem_cnt;
s32 otitem_cnt;
s32 faceonmtl_cnt;
s32 faceonitem_cnt;
s32 dynamic_glass_item_cnt;
f32 sinetime_246;
struct nustenitem_s stenitem[512];
struct nufaceonitem_s* faceonmtllist[50];
struct nufaceonitem_s faceonitem[512];
struct nuotitem_s dynamic_glass_items[64];
struct nuwateritem_s wateritem[512];
s32 IsStencil = 0;

enum nustencilmode_e stencil_mode = NUSTENCIL_NOSTENCIL;


void NuMtlCreateDefault(void) {
    struct numtl_s* mtl;

    mtl = NuMtlCreate(1);
    numtl_white = mtl;
    mtl->L = 0;
    mtl->alpha = 1.0;
    //mtl->attrib._word = (union numtlattribs)((u32)mtl->attrib & 0x3fc3ffff | 0x200000);
    mtl->attrib._word = (0x3fc3ffff | 0x200000);    //should work this way
    mtl->K = 0;
    mtl->diffuse.r = 1.0;
    mtl->diffuse.g = 1.0;
    mtl->diffuse.b = 1.0;
    //NuMtlUpdate(numtl_white); empty
}

void NuMtlInit(void)

{
  if (initialised != 0) {
    NuMtlClose();
  }
  smlist = NULL;
  initialised = 1;
  NuMtlCreateDefault();
  return;
}

void NuMtlClose(void)

{
  struct numtl_s *list;
  struct numtl_s *next;

  if (initialised != 0) {
    list = &smlist->mtl;
    if (smlist != NULL) {
      do {
        next = list->next;
        NuMtlDestroy(list);
        list = next;
      } while (next != NULL);
    }
    initialised = 0;
  }
  return;
}


struct numtl_s* NuMtlCreate(s32 mode)
{
  struct nusysmtl_s *sm;
  struct nusysmtl_s *ret;
  f32 i;

  if (0 < mode) {
    i = 1.0;
    sm = NULL;
    do {
      ret = (struct nusysmtl_s *)malloc_x(0x6c);
      memset(ret,0,0x6c);
      (ret->mtl).alpha = i;
      //(ret->mtl).attrib = (struct numtlattrib_s)((u32)(ret->mtl).attrib & 0xcfffffff | 0x20000000);
      (ret->mtl).attrib._word = (0xcfffffff | 0x20000000);
      NuMtlInsert(ret);
      mode = mode + -1;
      (ret->mtl).next = &sm->mtl;
      nummtls = nummtls + 1;
      sm = ret;
    } while (mode != 0);
  }
  return &ret->mtl;
}



void NuMtlDestroy(struct numtl_s* mtl) //need test
{
    struct nusysmtl_s* sm = (struct nusysmtl_s*)mtl; // r31
    struct nusysmtl_s* next; //unused?

    if (sm->mtl.tid != 0) {
        NuTexUnRef(sm->mtl.tid);
    }
    if (sm->geom2d != NULL) {
        NuGeomDestroy(sm->geom2d->geom);
    }
    if (sm->geom3d != NULL) {
        NuGeomDestroy(sm->geom3d->geom);
    }
    NuMtlRemove(sm);
    free_x(mtl);
    nummtls -= 1;
}




int NuMtlNum(void)

{
  return nummtls;
}

struct numtl_s * NuMtlGet(int id)
{
  struct nusysmtl_s *ret;

  ret = NULL;
  if ((id < nummtls) && (ret = smlist, 0 < id)) {
    do {
      ret = ret->next;
      id = id + -1;
    } while (id != 0);
  }
  return &ret->mtl;
}


struct numtl_s * NuMtlRead(fileHandle handle)

{
  struct numtl_s *mtl;

  mtl = NuMtlCreate(1);
  NuFileRead(handle,mtl,0x54);
  return mtl;
}


void NuMtlInsert(struct nusysmtl_s *sm)    //need fix
{
  int sid;
  struct nusysmtl_s *last;
  struct nusysmtl_s *list;
  bool check;
  short lsid;


  list = smlist;
  lsid = (sm->mtl).alpha_sort;
  sid = (sm->mtl).tid;
  if(smlist != NULL)
  {
  sm->next = NULL;
  sm->last = NULL;
  last = NULL;
  while ((list->next != NULL && (lsid * 0x10000 + sid < (list->mtl).alpha_sort * 0x10000 + (list->mtl).tid)))  {
    check =
    last = list;
    list = list->next;
  }
  if (last == NULL) {
    sm->next = smlist;
    smlist = sm;
  }
  else {
    sm->last = last;
    sm->next = last->next;
    last->next = sm;
  }
  if (sm->next == NULL) {
      printf("sm next is null\n");
    return;
  }
  sm->next->last = sm;

  printf("smlist not null\n");
    return;
  }

  else

  {
    printf("smlist is empty\n");

  }
}


void NuMtlRemove(struct nusysmtl_s *sm)

{
  if (sm->last == NULL) {
    smlist = sm->next;
  }
  else {
    sm->last->next = sm->next;
  }
  if (sm->next == NULL) {
    return;
  }
  sm->next->last = sm->last;
  return;
}

void NuMtlUpdate(struct numtl_s *mtl)

{
  return;
}


void NuMtlSetStencilRender(enum nustencilmode_e mode)

{
  stencil_mode = mode;
  return;
}




void NuMtlAddRndrItem(struct nusysmtl_s *mtl,struct nugeomitem_s *item)

{
  struct nustenitem_s *steni;
  struct nuwateritem_s *wateri;
  float fVar1;
  bool check;
  int cnt;
  short hShader;
  enum nustencilmode_e stmode;
  int watercnt;

  stmode = stencil_mode;
  watercnt = wateritem_cnt;
  cnt = stenitem_cnt;
  if (stencil_mode == NUSTENCIL_NOSTENCIL) {
    hShader = item->hShader;
    if ((hShader == 3) || (hShader == 0x21)) {
      NuMtlAddGlassItem(&mtl->mtl,&item->hdr);
    }
    else {
      if ((hShader != 1) && (hShader != 0x1b)) {
        /*if (((u32)(mtl->mtl).attrib >> 0x1e == 0) && (item->geom->mtls->fxid != '\x04')) {
          (item->hdr).next = mtl->rndrlist;
          mtl->rndrlist = &item->hdr;
          return;
        }*/
        cnt = otitem_cnt + -1;
        otitem_cnt = cnt;
        otitem[cnt].mtl = mtl;
        otitem[cnt].hdr = &item->hdr;
        //fVar1 = NuRndrItemDist(&item->hdr);
        otitem[cnt].dist = fVar1;
        NuMtlOTInsert(otitem + cnt);
        return;
      }
      if (wateritem_cnt < 0x200) {
        wateri = wateritem + wateritem_cnt;
        wateritem[wateritem_cnt].hdr = &item->hdr;
        wateritem[watercnt].mtl = mtl;
      }
      else {
        //error = NuErrorProlog("C:/source/crashwoc/code/nu3dx/numtl.c",0x2a7);
        //(*error)("NuMtlAddRndrItem: Exceeded maximum number of water items in render queue!");
      }
      if (wateritem_cnt != 0) {
        wateritem[wateritem_cnt + -1].next = wateri;
      }
      wateritem_cnt = wateritem_cnt + 1;
    }
  }
  else {
    check = stenitem_cnt != 0;
    steni = stenitem + stenitem_cnt;
    stenitem[stenitem_cnt].hdr = &item->hdr;
    stenitem[cnt].mtl = mtl;
    stenitem[cnt].type = stmode;
    if (check) {
      stenitem[cnt + -1].next = steni;
    }
    stenitem_cnt = stenitem_cnt + 1;
  }
  return;
}


void NuMtlAddFaceonItem(struct numtl_s *mtl,struct nurndritem_s *item)

{
  struct nufaceonitem_s **flist;
  s32 cnt;
  s32 i;
  struct nufaceonitem_s *faceoni;
  bool check;
  struct nufaceonitem_s *face;

  face = faceonmtllist[0];
  flist = faceonmtllist;
  cnt = faceonitem_cnt + -1;
  i = 0;
  check = faceonmtllist[0] != NULL;
  faceoni = faceonitem + cnt;
  faceonitem_cnt = cnt;
  faceonitem[cnt].hdr = item;
  faceonitem[cnt].mtl = (struct nusysmtl_s *)mtl;
  if (check) {
    if (face->mtl != (struct nusysmtl_s *)mtl) {
      do {
        if (0x31 < i) {
          //e = NuErrorProlog("C:/source/crashwoc/code/nu3dx/numtl.c",0x2f5);
          //(*e)("Out of room for face-ons in the list");
        }
        flist = flist + 1;
        i = i + 1;
        if (*flist == NULL) goto LAB_800b3084;
      } while ((*flist)->mtl != faceonitem[cnt].mtl);
    }
    face = faceonmtllist[i];
    if (face != NULL) {
      faceonmtllist[i] = faceoni;
      faceoni->next = face;
      return;
    }
  }
LAB_800b3084:
  faceonmtllist[i] = faceoni;
  faceoni->next = NULL;
  faceonmtl_cnt = faceonmtl_cnt + 1;
  return;
}



void NuMtlOTInsert(struct nuotitem_s *oti)

{
  s32 ix;

  ix = (s32)(oti->mtl->mtl).alpha_sort;
  if (0x100 < ix) {
    //error = NuErrorProlog("C:/source/crashwoc/code/nu3dx/numtl.c",0x3e0);
    //("assert");
  }
  if (ot[ix] == NULL) {
    ot[ix] = oti;
    oti->next = NULL;
  }
  else {
    oti->next = ot[ix];
    ot[ix] = oti;
  }
  return;
}


void NuMtlGet2dBuffer(struct nusysmtl_s *mtl,enum nuprimtype_e pt,struct nugeom_s **geomptr,struct nuprim_s **primptr, union variptr_u *ptr,union variptr_u *end)

{
  enum nuprimtype_e type;
  struct nugeomitem_s *geomitem;
  struct nugeom_s *g;
  struct nuprim_s *prim;

  NuMemSetExternal(ptr,end);
  if (mtl->geom2d == NULL) {
    geomitem = (struct nugeomitem_s *)NuMemAlloc(0x24);
    mtl->geom2d = geomitem;
    memset(geomitem,0,0x24);
    (mtl->geom2d->hdr).type = NURNDRITEM_GEOM2D;
    (mtl->geom2d->hdr).flags = 0;
    g = NuGeomCreate();
    mtl->geom2d->geom = g;
    NuGeomCreateVB(mtl->geom2d->geom,0x400,NUVT_TLTC1,1);
    mtl->geom2d->geom->vtxcnt = 0;
  }
  g = mtl->geom2d->geom;
  prim = g->prim;
  if (prim != NULL) {
    type = prim->type;
    while (type != pt) {
      prim = prim->next;
      if (prim == NULL) goto LAB_800b3234;
      type = prim->type;
    }
    if (prim != NULL) goto LAB_800b325c;
  }
LAB_800b3234:
  prim = NuPrimCreate(0x400,pt);
  NuGeomAddPrim(g,prim);
  prim->vertexCount = 0;
LAB_800b325c:
  if (g != NULL) {
    *geomptr = g;
  }
  if (prim != NULL) {
    *primptr = prim;
  }
  NuMemSetExternal(NULL,NULL);
  return;
}



void NuMtlGet3dBuffer(struct nusysmtl_s *mtl,enum nuprimtype_e pt,struct nugeom_s **geomptr,struct nuprim_s **primptr,
                     union variptr_u *ptr,union variptr_u *end)

{
  enum nuprimtype_e type_;
  struct nugeomitem_s *geomitem;
  struct nugeom_s *geom;
  struct nuprim_s *prim;

  NuMemSetExternal(ptr,end);
  if (mtl->geom3d == NULL) {
    geomitem = (struct nugeomitem_s *)NuMemAlloc(0x24);
    mtl->geom3d = geomitem;
    memset(geomitem,0,0x24);
    (mtl->geom3d->hdr).type = NURNDRITEM_GEOM3D;
    (mtl->geom3d->hdr).flags = 0;
    geom = NuGeomCreate();
    mtl->geom3d->geom = geom;
    NuGeomCreateVB(mtl->geom3d->geom,0xc00,NUVT_TC1,1);
    mtl->geom3d->geom->vtxcnt = 0;
  }
  geom = mtl->geom3d->geom;
  prim = geom->prim;
  if (prim != NULL) {
    type_ = prim->type;
    while (type_ != pt) {
      prim = prim->next;
      if (prim == NULL) goto LAB_800b336c;
      type_ = prim->type;
    }
    if (prim != NULL) goto LAB_800b3394;
  }
LAB_800b336c:
  prim = NuPrimCreate(0xc00,pt);
  NuGeomAddPrim(geom,prim);
  prim->vertexCount = 0;
LAB_800b3394:
  if (geom != NULL) {
    *geomptr = geom;
  }
  if (prim != NULL) {
    *primptr = prim;
  }
  NuMemSetExternal(NULL,NULL);
  return;
}

void NuMtlRender(void)

{
  //DBTimerStart(0x13);
  NuMtlRender3d();
  //DBTimerEnd(0x13);
  //DBTimerStart(0x12);
  //DBTimerEnd(0x12);
  //DBTimerStart(0x11);
  //DBTimerEnd(0x11);
  //DBTimerStart(0x10);
  NuMtlRenderDynamic2d3d();
  //DBTimerEnd(0x10);
  //DBTimerStart(0xf);
  NuMtlRenderOT(0,10);
  //DBTimerEnd(0xf);
  //DBTimerStart(0xe);
  NuMtlRenderFaceOn();
  //DBTimerEnd(0xe);
  //DBTimerStart(0xc);
  //NuMtlRenderSten();
  //DBTimerEnd(0xc);
  //DBTimerStart(0xb);
  NuMtlRenderGlass();
  //DBTimerEnd(0xb);
  //DBTimerStart(10);
  NuMtlRenderWater();
  //DBTimerEnd(10);
  //DBTimerStart(9);
  NuMtlRenderOT(0xb,0x100);
  //DBTimerEnd(9);
  //DBTimerStart(8);
  //DrawStencilShadowQuad();
  //DBTimerEnd(8);
  //DBTimerStart(7);
  NuMtlRenderUpd();
  //DBTimerEnd(7);
  return;
}

void NuMtlRender3d(void)

{
  struct nusysmtl_s *sm;
  struct nurndritem_s *ri;

  //GS_SetAlphaCompare(7,0);
  for (sm = smlist; sm != NULL; sm = sm->next) {
    if (((sm->mtl).L == '\0') && (sm->rndrlist != NULL)) {
      NuTexSetTexture(0,(sm->mtl).tid);
      NuMtlSetRenderStates(&sm->mtl);
      NuTexSetTextureStates(&sm->mtl);
      for (ri = sm->rndrlist; ri != NULL; ri = ri->next) {
        //NuRndrItem(ri);
      }
      sm->rndrlist = NULL;
    }
  }
  //GS_SetAlphaCompare(7,0);
  return;
}


void NuMtlRenderDynamic2d3d(void)

{
  struct nuprim_s *prim;
  struct nusysmtl_s *sm;
  struct nugeomitem_s *item;

  //GS_SetAlphaCompare(7,0);
  sm = smlist;
  //GS_SetZCompare(1,0,GX_LEQUAL);
  for (; sm != NULL; sm = sm->next) {
    if ((sm->geom3d != NULL) && (sm->geom3d->geom->vtxcnt != 0)) {
      NuTexSetTexture(0,(sm->mtl).tid);
      NuMtlSetRenderStates(&sm->mtl);
      NuTexSetTextureStates(&sm->mtl);
      item = sm->geom3d;
      //NuRndrItem(&item->hdr);
      item->geom->vtxcnt = 0;
      for (prim = item->geom->prim; prim != NULL; prim = prim->next) {
        prim->vertexCount = 0;
      }
    }
    if ((sm->geom2d != NULL) && (sm->geom2d->geom->vtxcnt != 0)) {
      NuTexSetTexture(0,(sm->mtl).tid);
      NuMtlSetRenderStates(&sm->mtl);
      NuTexSetTextureStates(&sm->mtl);
      item = sm->geom2d;
      //GS_EnableLighting(0);
      //GS_SetZCompare(1,0,GX_LEQUAL);
      //GS_SetAlphaCompare(7,0);
      //NuRndrItem(&item->hdr);
      item->geom->vtxcnt = 0;
      for (prim = item->geom->prim; prim != NULL; prim = prim->next) {
        prim->vertexCount = 0;
      }
    }
  }
  return;
}


void NuMtlRenderOT(int begin,int end)

{
  struct nusysmtl_s *mtl;
  s32 i;
  struct nuotitem_s *oti;

  //GS_SetAlphaCompare(7,0);
  if (begin <= end) {
    do {
      i = begin + 1;
      for (oti = ot[begin]; oti != NULL; oti = oti->next) {
        mtl = oti->mtl;
        if ((mtl->mtl).L == '\0') {
          NuTexSetTexture(0,(mtl->mtl).tid);
          NuMtlSetRenderStates(&oti->mtl->mtl);
          NuTexSetTextureStates(&oti->mtl->mtl);
          //NuRndrItem(oti->hdr);
        }
        else {
          NuTexSetTexture(0,(mtl->mtl).tid);
          NuMtlSetRenderStates(&oti->mtl->mtl);
          NuTexSetTextureStates(&oti->mtl->mtl);
          //GS_SetZCompare(1,1,GX_LEQUAL);
          //GS_SetAlphaCompare(3,0xf7);
          //NuRndrItem(oti->hdr);
        }
      }
      ot[begin] = NULL;
      begin = i;
    } while (i <= end);
  }
  return;
}

void NuMtlRenderFaceOn() {

  u32 processed_count = 0; // count -> r31
  struct nufaceonitem_s* cur_list;
  struct nusysmtl_s* sys_mtl;
  struct numtx_s var_168;
  struct nuvec_s var_178;
  struct nuvec_s faceon_point_rotated;
  struct nuvec_s faceon_world_pos;
  struct nuvec_s* mtx_translation;
  struct numtx_s faceon_transform;
  struct nuvec_s vertex_corner;
  struct nuvec_s vertex_transformed;
  struct nugeomitem_s* list_geomitem;
  struct nufaceongeom_s* faceon_geom;
  struct numtx_s faceon_geom_xf;

  //ResetShaders();
  //SetVertexShader(0x142);
  //GS_LoadWorldMatrixIdentity();

  if (faceonmtl_cnt <= 0) {
    return;
  }
  do {
    cur_list = faceonmtllist[processed_count];

    sys_mtl = cur_list->mtl;
    if (sys_mtl->mtl.L) {
      NuTexSetTexture(0, sys_mtl->mtl.tid);
      NuMtlSetRenderStates(cur_list->mtl);
      NuTexSetTextureStates(cur_list->mtl);
      //GS_SetZCompare(1, 0, 3);
      //GS_SetAlphaCompare(3, 0xf7);
    } else {
      NuTexSetTexture(0, sys_mtl->mtl.tid);
      NuMtlSetRenderStates(cur_list->mtl);
      NuTexSetTextureStates(cur_list->mtl);
      //GS_SetAlphaCompare(4, 0);
      //GS_SetZCompare(1, 1, 3);
    }

    // nufaceonitem_s::hdr is probably a nugeomitem_s*
    // TODO: remove this cast
    var_168 = *((struct nugeomitem_s*)cur_list->hdr)->mtx;

    for (; cur_list != NULL; cur_list = cur_list->next) {
      faceon_geom_xf = *((struct nugeomitem_s*)cur_list->hdr)->mtx;
        var_178.x = 0.0f;
        var_178.y = 0.0f;
        var_178.z = 0.0f;
      list_geomitem = (struct nugeomitem_s*)cur_list->hdr;
      faceon_geom = (struct NuFaceOnGeom*)list_geomitem->geom;


      for (; faceon_geom != NULL; faceon_geom = faceon_geom->next) {
        s32 faceon_count = faceon_geom->nfaceons;
        struct nufaceon_s* faceon_list = faceon_geom->faceons;
        //GS_DrawQuadListBeginBlock(faceon_count * 4);

        for (; faceon_count > 0; faceon_count -= 1, faceon_list += 1) {
          const f32 right = faceon_list->width * 0.5f; // 30
          const f32 top = faceon_list->height * 0.5f; //31
          const f32 left = -right;
          const f32 bottom = -top;


          NuVecMtxRotate(&faceon_point_rotated, &faceon_list->point, &faceon_geom_xf);

          //NEED FIX!
        //mtx_translation = (struct nuvec_s*)((struct nugeomitem_s*)cur_list)->mtx->_30;
          NuVecAdd(&faceon_world_pos, &faceon_point_rotated, mtx_translation);


          NuMtxCalcCheapFaceOn(&faceon_transform, &faceon_world_pos);

          vertex_corner.x = left;
          vertex_corner.y = top;
          vertex_corner.z = 0.0f;

          vertex_transformed.x = left;
          vertex_transformed.y = top;
          vertex_transformed.z = 0.0f;
          NuVecMtxTransform(&vertex_transformed, &vertex_corner, &faceon_transform);

          // Color is defined ARGB?
          //GS_SetQuadListRGBA((faceon_list->colour >> 16) & 0xff,
                            // (faceon_list->colour >> 8) & 0xff,
                             //faceon_list->colour & 0xff,
                            // (faceon_list->colour >> 24) & 0xff);
          //GS_DrawQuadListSetVert(&vertex_transformed, 0.0f, 0.0f);

          vertex_corner.x = right;
          vertex_corner.y = top;
          vertex_corner.z = 0.0f;
          NuVecMtxTransform(&vertex_transformed, &vertex_corner, &faceon_transform);
          //GS_DrawQuadListSetVert(&vertex_transformed, 1.0f, 0.0f);

          vertex_corner.x = right;
          vertex_corner.y = bottom;
          vertex_corner.z = 0.0f;
          NuVecMtxTransform(&vertex_transformed, &vertex_corner, &faceon_transform);
          //GS_DrawQuadListSetVert(&vertex_transformed, 1.0f, 1.0f);

          vertex_corner.x = left;
          vertex_corner.y = bottom;
          vertex_corner.z = 0.0f;
          NuVecMtxTransform(&vertex_transformed, &vertex_corner, &faceon_transform);
          //GS_DrawQuadListSetVert(&vertex_transformed, 0.0f, 1.0f);
        }
      }
    }

    faceonmtllist[processed_count] = 0;
    processed_count += 1;
  } while (processed_count < faceonmtl_cnt);

return;
}

void NuMtlRenderGlass(void)

{
  s32 i;
  struct nuotitem_s *oti;

  oti = dynamic_glass_items;
  i = 0;
  if (0 < dynamic_glass_item_cnt) {
    NudxFw_MakeBackBufferCopy(force_glass_screencopy_enable);
    if (0 < dynamic_glass_item_cnt) {
      do {
        i = i + 1;
        NuTexSetTexture(0,(oti->mtl->mtl).tid);
        NuMtlSetRenderStates(&oti->mtl->mtl);
        NuTexSetTextureStates(&oti->mtl->mtl);
        //NuRndrItem(oti->hdr);
        oti = oti->next;
      } while (i < dynamic_glass_item_cnt);
    }
    NuMtlClearGlassList();
  }
  return;
}

void NuMtlRenderWater(void)

{
  s32 i;
  struct nustenitem_s *wateri;

  wateri = (struct nustenitem_s *)wateritem;
  if (0 < wateritem_cnt) {
    i = 0;
    NudxFw_MakeBackBufferCopy(1);
    if (0 < wateritem_cnt) {
      do {
        i = i + 1;
        NuTexSetTexture(0,(wateri->mtl->mtl).tid);
        NuMtlSetRenderStates(&wateri->mtl->mtl);
        NuTexSetTextureStates(&wateri->mtl->mtl);
        //NuRndrItem(wateri->hdr);
        wateri = wateri->next;
      } while (i < wateritem_cnt);
    }
  }
  wateritem_cnt = 0;
  return;
}


void NuMtlRenderSten(void)
{
  struct nustenitem_s *steni;
  s32 i;

   steni = stenitem;
  //GS_SetAlphaCompare(4,0);
  i = 0;
  NudxFw_SetRenderState(0x7d,1);
  NudxFw_SetRenderState(0x46,0x207);
  NudxFw_SetRenderState(0x44,0x1e00);
  NudxFw_SetRenderState(0x7e,0x1e00);
  NudxFw_SetRenderState(0x47,1);
  NudxFw_SetRenderState(0x48,0xffffffff);
  NudxFw_SetRenderState(0x49,0xffffffff);
  IsStencil = 1;
  if (0 < stenitem_cnt) {
    do {
      if (steni->type == 1) {
        //GS_EnableLighting(0);
        //GS_SetMaterialSourceEmissive(1);
        NuTexSetTexture(0,0);
        //GS_SetBlendSrc(1,1,0);
      }
      else {
        NuTexSetTexture(0,(steni->mtl->mtl).tid);
        NuMtlSetRenderStates(&steni->mtl->mtl);
        NuTexSetTextureStates(&steni->mtl->mtl);
      }
      if (steni->type == 1) {
        NudxFw_SetRenderState(0x45,0x1e01);
        //NuRndrItem(steni->hdr);
      }
      i = i + 1;
      steni = steni->next;
    } while (i < stenitem_cnt);
  }
  stenitem_cnt = 0;
  //GS_EnableLighting(1);
  NudxFw_SetRenderState(0x7d,0);
  IsStencil = 0;
  return;
}


void NuMtlAddGlassItem(struct numtl_s *mtl,struct nurndritem_s *item)

{
  struct nuotitem_s *tail;
  bool check;
  s32 cnt;

  if (0x40 < dynamic_glass_item_cnt) {
    NuErrorProlog("C:/source/crashwoc/code/nu3dx/numtl.c",0x980);
    //(*error)("Too many dynamic glass items");
  }
  cnt = dynamic_glass_item_cnt;
  if (dynamic_glass_item_cnt < 0x40) {
    check = 0 < dynamic_glass_item_cnt;
    tail = dynamic_glass_items + dynamic_glass_item_cnt;
    dynamic_glass_items[dynamic_glass_item_cnt].hdr = item;
    dynamic_glass_items[cnt].mtl = (struct nusysmtl_s *)mtl;
    dynamic_glass_items[cnt].dist = 0.0;
    if (check) {
      dynamic_glass_items[cnt + -1].next = tail;
    }
    dynamic_glass_item_cnt = dynamic_glass_item_cnt + 1;
  }
  return;
}

void NuMtlClearGlassList(void)

{
  memset(dynamic_glass_items,0,0x400);
  dynamic_glass_item_cnt = 0;
  return;
}

void NuMtlRenderUpd(void)

{
  faceonitem_cnt = 0x200;
  faceonmtl_cnt = 0;
  otitem_cnt = 0x7e8;
  NuTexSetTexture(0,0);
  return;
}


void NuMtlSetRenderStates(struct numtl_s *mtl)
{
  s32 ivar2;
  struct _D3DMATERIAL8 d3dmtl;
  u32 attrib;
  bool check;

  d3dmtl.Diffuse.a = mtl->alpha;
  d3dmtl.Diffuse.r = (mtl->diffuse).r;
  d3dmtl.Diffuse.g = (mtl->diffuse).g;
  d3dmtl.Diffuse.b = (mtl->diffuse).b;
  d3dmtl.Ambient.r = (mtl->ambient).r;
  d3dmtl.Ambient.g = (mtl->ambient).g;
  d3dmtl.Ambient.b = (mtl->ambient).b;
  d3dmtl.Emissive.r = d3dmtl.Ambient.r;
  d3dmtl.Emissive.g = d3dmtl.Ambient.g;
  d3dmtl.Emissive.b = d3dmtl.Ambient.b;
  /*if (((u32)mtl->attrib >> 0x10 & 3) == 2)  fix
  {
    d3dmtl.Emissive.r = d3dmtl.Diffuse.r;
    d3dmtl.Emissive.g = d3dmtl.Diffuse.g;
    d3dmtl.Emissive.b = d3dmtl.Diffuse.b;
  }*/
  d3dmtl.Power = mtl->power;
  d3dmtl.Ambient.a = d3dmtl.Diffuse.a;
  d3dmtl.Emissive.a = d3dmtl.Diffuse.a;
  //GS_SetMaterial(&d3dmtl);
  //attrib = (u32)mtl->attrib >> 0x1e;  fix
  if (attrib == 0) {
    //GS_SetBlendSrc(1,1,0);
    ivar2 = 7;
  }
  else {
    if (attrib == 1) {
      ivar2 = 5;
    }
    else {
      if (attrib != 2) {
        if (attrib == 3) {
          //GS_SetBlendSrc(1,0,3);
          //GS_SetAlphaCompare(4,0);
        }
        goto LAB_800b40dc;
      }
      ivar2 = 1;
    }
    //GS_SetBlendSrc(1,4,ivar2);
    ivar2 = 4;
  }
  //GS_SetAlphaCompare(ivar2,0);
LAB_800b40dc:
  //GS_SetAlphaCompare(4,0);
  NudxFw_SetRenderState(0x80,0);
  /*if (((u32)mtl->attrib & 0xc0000) == 0)
  {
    //GS_SetZCompare(1,1,GX_LEQUAL);
  }
  if (((u32)mtl->attrib >> 0x12 & 3) == 1) {
    //GS_SetZCompare(1,0,GX_LEQUAL);
  }
  if (((u32)mtl->attrib >> 0x12 & 3) == 2) {
    //GS_SetZCompare(1,1,GX_ALWAYS);
  }
  if (((u32)mtl->attrib >> 0x12 & 3) == 3) {
    //GS_SetZCompare(0,0,GX_ALWAYS);
  }
  IsObjLit = 0;
  check = ((u32)mtl->attrib & 0x30000) == 0;
  if (check) {
    //GS_EnableLighting(1);
    //GS_EnableSpecular(0);
  }
  IsObjLit = (u32)check;
  if (((u32)mtl->attrib >> 0x10 & 3) == 1) {
    //GS_EnableLighting(1);
    //GS_EnableSpecular(1);
    IsObjLit = 2;
  }
  if (((u32)mtl->attrib >> 0x10 & 3) == 2) {
    //GS_EnableLighting(0);
  }
  if (((u32)mtl->attrib & 0x8000) == 0) {
    //GS_EnableColorVertex(0);
    //GS_SetMaterialSourceAmbient(0);
    //GS_SetMaterialSourceEmissive(0);
  }
  if (((u32)mtl->attrib & 0x8000) != 0) {
    //GS_EnableColorVertex(1);
    //GS_SetMaterialSourceAmbient(1);
    if (((u32)mtl->attrib >> 0x10 & 3) == 2) {
      //GS_SetMaterialSourceEmissive(1);
    }
    else {
      //GS_SetMaterialSourceEmissive(0);
    }
  }*/
  return;
}

void NuMtlAnimate(float timestep)

{
  struct nusysmtl_s *sm;
  u32 unk;

  sinetime_246 = sinetime_246 + timestep;
  sm = smlist;
  if (smlist == NULL) {
    return;
  }
  do {
    unk = *(u32 *)&(sm->mtl).K;
    if ((unk & 0xff) != 0) {
      if ((unk & 0xf0) == 0x20) {
        (sm->mtl).du = (sm->mtl).su * timestep;
      }
      if ((*(u32 *)&(sm->mtl).K & 0xf) == 2) {
        (sm->mtl).dv = (sm->mtl).sv * timestep;
      }
    }
    sm = sm->next;
  } while (sm != NULL);
  return;
}

/*

void NuMtlUVAnimation(struct nugobj_s *gobj)

{
  s32 numvts;
  struct numtl_s *mtls;
  struct nuvtx_tc1_s* vt1;
  struct nuvtx_tc1_s *vt3;
  struct nuvtx_tc1_s *vt1;
  struct nuvtx_tc1_s *vt2;
  struct nugeom_s *geoms;
  f32 du;
  f32 dv;
  f32 fVar1;


  geoms = gobj->geom;
  if (geoms == NULL) {
    return;
  }
  do {
    mtls = geoms->mtls;
    if ((*(u32 *)&mtls->K & 0xf0) == 0x20) {
      du = mtls->du;
LAB_800b4334:
      dv = 0.0;
      if ((*(u32 *)&mtls->K & 0xf) == 2) {
        dv = mtls->dv;
      }
      if ((dv == 0.0) || (du == 0.0)) {
        if ((dv == 0.0) && (du != 0.0)) {
          if (geoms->vertex_type == NUVT_TC1) {
            numvts = geoms->vtxcount;
            vt1 = (struct nuvtx_tc1_s *)geoms->hVB;
            if (0 < numvts) {
              do {
                vt1->tc[0] = vt1->tc[0] + du + du;
                vt1 = vt1 + 1;
                numvts = numvts + -1;
              } while (numvts != 0);
            }
          }
        }
        else if (((dv != 0.0) && (du == 0.0)) && (geoms->vertex_type == NUVT_TC1)) {
          numvts = geoms->vtxcount;
          vt2 = (struct nuvtx_tc1_s *)geoms->hVB;
          if (0 < numvts) {
            do {
              vt2->tc[1] = vt2->tc[1] + dv + dv;
              vt2 = vt2 + 1;
              numvts = numvts + -1;
            } while (numvts != 0);
          }
        }
      }
      else if (geoms->vertex_type == NUVT_TC1) {
        numvts = geoms->vtxcount;
        vt3 = (struct nuvtx_tc1_s *)geoms->hVB;
        if (0 < numvts) {
          do {
            fVar1 = vt3->tc[1];
            vt3->tc[0] = vt3->tc[0] + du + du;
            vt3->tc[1] = fVar1 + dv + dv;
            vt3 = vt3 + 1;
            numvts = numvts + -1;
          } while (numvts != 0);
        }
      }
    }
    else {
      du = 0.0;
      if ((*(u32*)&mtls->K & 0xf) == 2) goto LAB_800b4334;
    }
    geoms = geoms->next;
    if (geoms == NULL) {
      return;
    }
  } while( true );
}
*/

void NuMtlClearOt(void)

{
  memset(ot,0,0x404);
  return;
}
