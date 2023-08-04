#include "nuglass.h"
#include "../system.h"

static s32 spectid;

void InitSpecular(void)

{
  struct nutex_s tex;
  
  spectid = 0;
  NuTexReadBitmapMM("gfx\\spectxtf.bmp",0,&tex);
  spectid = NuTexCreate(&tex);
  if (tex.bits != NULL) {
    NuMemFree(tex.bits);
  }
  if (tex.pal != NULL) {
    NuMemFree(tex.pal);
  }
  return;
}


s32 GetGlassSpecularTexId(void)

{
  return spectid;
}



/*

void InitOverrideMtl(void)	//TODO
{
  numtlattrib_s attrib;
  int BBtid;
  numtl_s *pnVar1;
  numtl_s *a;
  numtl_s *bs_mtl;
  numtl_s *bs_mtl_1;
  numtl_s *mtl;
  
  glass_mtl = NuMtlCreate(1);
  BBtid = NudxFw_GetBackBufferCopyTID();
  mtl = glass_mtl;
  glass_mtl->special_id = '\x01';
  mtl->tid = BBtid;
  mtl = glass_mtl;
  attrib = glass_mtl->attrib;
  glass_mtl->fxid = '\x03';
  bs_mtl = glass_mtl;
  (mtl->diffuse).b = 0.9;
  (mtl->diffuse).r = 0.9;
  (mtl->diffuse).g = 0.9;
  mtl->alpha = 1.0;
  mtl->attrib = (numtlattrib_s)((uint)attrib & 0x3c30c7ff | 0x1408800);
  mtl->power = 1.0;
  bs_mtl->fx1 = (nufx_u)0x3f800000;
  NuMtlUpdate(bs_mtl);
  glass_mtl->next = (numtl_s *)0x0;
  glass_mtl_blendskin = NuMtlCreate(1);
  BBtid = 0x48;
  mtl = glass_mtl;
  bs_mtl = glass_mtl_blendskin;
  do {
    a = bs_mtl;
    pnVar1 = mtl;
    BBtid = BBtid + -0x18;
    a->next = pnVar1->next;
    a->attrib = pnVar1->attrib;
    (a->ambient).r = (pnVar1->ambient).r;
    (a->ambient).g = (pnVar1->ambient).g;
    (a->ambient).b = (pnVar1->ambient).b;
    mtl = (numtl_s *)&(pnVar1->diffuse).g;
    (a->diffuse).r = (pnVar1->diffuse).r;
    bs_mtl_1 = glass_mtl_blendskin;
    bs_mtl = (numtl_s *)&(a->diffuse).g;
  } while (BBtid != 0);
  *(float *)bs_mtl = *(float *)mtl;
  (a->diffuse).b = (pnVar1->diffuse).b;
  a->fx1 = pnVar1->fx1;
  bs_mtl_1[1].diffuse.r = 2.382207e-44;
  NuMtlUpdate(bs_mtl_1);
  glass_mtl_blendskin2 = NuMtlCreate(1);
  BBtid = 0x48;
  mtl = glass_mtl;
  bs_mtl = glass_mtl_blendskin2;
  do {
    a = bs_mtl;
    pnVar1 = mtl;
    BBtid = BBtid + -0x18;
    a->next = pnVar1->next;
    a->attrib = pnVar1->attrib;
    (a->ambient).r = (pnVar1->ambient).r;
    (a->ambient).g = (pnVar1->ambient).g;
    (a->ambient).b = (pnVar1->ambient).b;
    mtl = (numtl_s *)&(pnVar1->diffuse).g;
    (a->diffuse).r = (pnVar1->diffuse).r;
    bs_mtl_1 = glass_mtl_blendskin2;
    bs_mtl = (numtl_s *)&(a->diffuse).g;
  } while (BBtid != 0);
  *(float *)bs_mtl = *(float *)mtl;
  (a->diffuse).b = (pnVar1->diffuse).b;
  a->fx1 = pnVar1->fx1;
  bs_mtl_1[1].diffuse.r = 2.522337e-44;
  NuMtlUpdate(bs_mtl_1);
  return;
}



*/


s32 isGlassInstance(struct nugscn_s *gsc,struct nuinstance_s *inst)

{
  struct nugeom_s *geom;
  u8 fxid;
  
  geom = (&gsc->gobjs->sysnext)[inst->objid]->geom;
  if (-1 < (int)inst->flags) {
    return 0;
  }
  while( true ) {
    if (geom == NULL) {
      return 0;
    }
    fxid = geom->mtls->fxid;
    if (((fxid == '\x01') || (fxid == '\x03')) || (fxid == 'd')) break;
    geom = geom->next;
  }
  return 1;
}



void NuGlassProcessScene(nugscn_s *gsc)	//CHECK

{
  float z;
  int flag;
  nugobj_s *gobj;
  nugeom_s *next;
  nugobj_s *sysnext;
  float y;
  int i;
  int j;
  nuinstance_s *inst;
  uchar fxid;
  nugeom_s *g;
  nugeom_s *geom;
  nugeom_s *prev;
  
  i = 0;
  if (0 < gsc->numinstance) {
    do {
      j = i + 1;
      flag = isGlassInstance(gsc,gsc->instances + i);
      if (flag != 0) {
        if (0xff < num_glass_inst) {
          //e = NuErrorProlog("C:/source/crashwoc/code/nu3dx/nuglass.c",0x9f);
          //(*e)("assert");
        }
        flag = num_glass_inst;
        glass_inst[num_glass_inst] = gsc->instances + i;
        inst = gsc->instances + i;
        glassMtx[flag] = &inst->matrix;
        gobj = NuGobjCreate();
        i = num_glass_inst;
        glassGobj[num_glass_inst] = gobj;
        gobj->ngobjs = 1;
        glassGobj[i]->type = NUGOBJ_MESH;
        glassGobj[i]->geom = (nugeom_s *)0x0;
        glassGobj[i]->faceon_geom = (NuFaceOnGeom *)0x0;
        glassGobj[i]->next_gobj = (nugobj_s *)0x0;
        gobj = glassGobj[i];
        sysnext = (&gsc->gobjs->sysnext)[inst->objid];
        z = (sysnext->bounding_box_min).z;
        y = (sysnext->bounding_box_min).y;
        (gobj->bounding_box_min).x = (sysnext->bounding_box_min).x;
        (gobj->bounding_box_min).z = z;
        (gobj->bounding_box_min).y = y;
        gobj = glassGobj[i];
        sysnext = (&gsc->gobjs->sysnext)[inst->objid];
        z = (sysnext->bounding_box_max).z;
        y = (sysnext->bounding_box_max).y;
        (gobj->bounding_box_max).x = (sysnext->bounding_box_max).x;
        (gobj->bounding_box_max).z = z;
        (gobj->bounding_box_max).y = y;
        glassGobj[i]->bounding_radius_from_origin =
             (&gsc->gobjs->sysnext)[inst->objid]->bounding_radius_from_origin;
        glassGobj[i]->bounding_rsq_from_origin =
             (&gsc->gobjs->sysnext)[inst->objid]->bounding_rsq_from_origin;
        gobj = glassGobj[i];
        sysnext = (&gsc->gobjs->sysnext)[inst->objid];
        y = (sysnext->bounding_box_center).y;
        z = (sysnext->bounding_box_center).z;
        (gobj->bounding_box_center).x = (sysnext->bounding_box_center).x;
        (gobj->bounding_box_center).z = z;
        (gobj->bounding_box_center).y = y;
        glassGobj[i]->bounding_radius_from_center =
             (&gsc->gobjs->sysnext)[inst->objid]->bounding_radius_from_center;
        glassGobj[i]->bounding_rsq_from_center =
             (&gsc->gobjs->sysnext)[inst->objid]->bounding_rsq_from_center;
        g = (nugeom_s *)0x0;
        next = (&gsc->gobjs->sysnext)[inst->objid]->geom;
        while (geom = next, prev = g, geom != (nugeom_s *)0x0) {
          next = geom->next;
          fxid = geom->mtls->fxid;
          if (((fxid == '\x01') || (fxid == '\x03')) || (g = geom, fxid == 'd')) {
            geom->next = glassGobj[i]->geom;
            glassGobj[i]->geom = geom;
            g = prev;
            if (prev == (nugeom_s *)0x0) {
              (&gsc->gobjs->sysnext)[inst->objid]->geom = next;
            }
            else {
              prev->next = next;
            }
          }
        }
        num_glass_inst = num_glass_inst + 1;
      }
      i = j;
    } while (j < gsc->numinstance);
  }
  return;
}


void NuGlassLoad(nugscn_s *gsc,variptr_u *buffer,variptr_u *buffend)

{
  if (buffer != NULL) {
    NuMemSetExternal(buffer,buffend);
  }
  NuGlassProcessScene(gsc);
  if (buffer != NULL) {
    NuMemSetExternal(NULL,NULL);
  }
  return;
}

void NuGlassInit(void)

{
  num_glass_inst = 0;
  InitSpecular();
  InitOverrideMtl();
  return;
}

void NuGlassClose(void)	//CHECK

{
  int i;
  int j;
  
  i = 0;
  if (0 < num_glass_inst) {
    j = 0;
    do {
      j = i + 1;
      glass_inst[i] = NULL;
      glassMtx[i] = NULL;
      if (glassGobj[i] != NULL) {
        NuGobjDestroy(glassGobj[i]);
        glassGobj[i] = NULL;
      }
      i = j;
    } while (i < num_glass_inst);
  }
  num_glass_inst = 0;
  if (glass_mtl != NULL) {
    NuMtlDestroy(glass_mtl);
  }
  glass_mtl = NULL;
  return;
}

void NuGlassRenderStatic(void)	//CHECK
{
  int i;
  int iVar1;
  
  ProcessGlass(0);
  nurndr_forced_mtl = glass_mtl;
  DrawGlassCreatures(0);
  nurndr_forced_mtl = NULL;
  if ((SKELETALCRASH != 0) || (glass_mix != 1.0)) {
    NuRndrEndScene();
    NudxFw_MakeBackBufferCopy(1);
    NuRndrBeginScene(1);
  }
  if (num_glass_inst != 0) {
    i = 0;
    if (0 < num_glass_inst) {
      iVar1 = 0;
      do {
      if (((uint)glass_inst[iVar1]->flags & 0x20000000) != 0) 
	 {
          NuRndrGobj(glassGobj[iVar1],glassMtx[iVar1],NULL);
       }
        i = i + 1;
        iVar1 = iVar1 + 4;
      } while (i < num_glass_inst);
    }
  }
  return;
}


void ProcessGlass(int paused)	//TODO
{
  numtlattrib_s *attrib;
  numtl_s *bs;
  numtl_s *bs2;
  numtl_s *gmtl;
  float min;
  float mix;
  
  bs2 = glass_mtl_blendskin2;
  bs = glass_mtl_blendskin;
  gmtl = glass_mtl;
  if (glass_mtl == (numtl_s *)0x0) {
    return;
  }
  if (paused == 0) {
    mix = WATERBOSSGLASSMIX;
    if ((Level != 0x17) &&
       (((player->used == '\0' || (mix = ANGELGLASSMIX, (player->obj).dead != '\x03')) &&
        (mix = MAXGLASSMIX, glass_enabled == 0)))) {
      mix = 0.0;
    }
    if (glass_mix <= mix) {
      if ((mix <= glass_mix) || (glass_mix = glass_mix + glass_mix_speed, glass_mix <= mix))
      goto LAB_800af49c;
    }
    else {
      glass_mix = glass_mix - glass_mix_speed;
      if (mix <= glass_mix) goto LAB_800af49c;
    }
    glass_mix = mix;
  }
LAB_800af49c:
  mix = glass_mix;
  if (glass_mix == 1.0) {
    glass_mtl->attrib = (numtlattrib_s)((uint)glass_mtl->attrib & 0x3fffffff);
    bs->attrib = (numtlattrib_s)((uint)bs->attrib & 0x3fffffff);
    bs2->attrib = (numtlattrib_s)((uint)bs2->attrib & 0x3fffffff);
  }
  else {
    attrib = &glass_mtl->attrib;
    glass_mtl->alpha = glass_mix;
    gmtl->attrib = (numtlattrib_s)((uint)*attrib & 0x3fffffff | 0x40000000);
    bs->alpha = mix;
    bs->attrib = (numtlattrib_s)((uint)bs->attrib & 0x3fffffff | 0x40000000);
    bs2->alpha = mix;
    bs2->attrib = (numtlattrib_s)((uint)bs2->attrib & 0x3fffffff | 0x40000000);
  }
  NuShaderSetGlassMix(glass_mix);
  gmtl = glass_mtl;
  min = glass_col[0].b;
  mix = glass_col[0].g;
  (glass_mtl->diffuse).r = glass_col[0].r;
  (gmtl->diffuse).b = min;
  (gmtl->diffuse).g = mix;
  NuMtlUpdate(gmtl);
  NuMtlUpdate(glass_mtl_blendskin);
  NuMtlUpdate(glass_mtl_blendskin2);
  return;
}


void DrawGlassCreatures(int solid)

{
  if (Level == 0x17) {
    glass_phase = 1;
    glass_draw = solid;
    DrawCreatures(Character + 1,8,1,1);
  }
  else if ((GLASSPLAYER != 0) && (plr_invisibility_time < 5.0)) {
    glass_phase = 1;
    glass_draw = solid;
    DrawCreatures(Character,1,1,0);
    force_glass_screencopy_enable = 0;
  }
  return;
}