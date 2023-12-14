#include "nuglass.h"
#include "../system.h"

static s32 spectid;

//MATCH NGC
void InitSpecular() {
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

//MATCH NGC
s32 GetGlassSpecularTexId(void) {
    return spectid;
}

//WIP
void InitOverrideMtl(void) {
  int pad;

  pad = 0;
  glass_mtl = NuMtlCreate(1);
  glass_mtl->tid = NudxFw_GetBackBufferCopyTID();
  glass_mtl->special_id = '\x01';
  glass_mtl->fxid = '\x03';
    glass_mtl->attrib.zmode = 3;
    glass_mtl->attrib.alpha = 3;
    glass_mtl->attrib.atst = 1;
    glass_mtl->attrib.utc = 1;
    glass_mtl->attrib.colour = 3;
    glass_mtl->attrib.vtc = 1;
  //glass_mtl->attrib = (struct numtlattrib_s *)((uint)&glass_mtl->attrib & 0x3c30c7ff | 0x1408800);
  glass_mtl->diffuse.r = 0.9f;
  glass_mtl->diffuse.g = 0.9f;
  glass_mtl->diffuse.b = 0.9f;
  glass_mtl->fx1.f32 = 1.0f;
  glass_mtl->power = 1.0f;
  glass_mtl->alpha = 1.0f;

  NuMtlUpdate(glass_mtl);
  glass_mtl->next = NULL;
  glass_mtl_blendskin = NuMtlCreate(1);

   *glass_mtl_blendskin = *glass_mtl;


  ((struct nusysmtl_s *)glass_mtl_blendskin)->hShader = 0x11;
  NuMtlUpdate(glass_mtl_blendskin);
  glass_mtl_blendskin2 = NuMtlCreate(1);

    *glass_mtl_blendskin2 = *glass_mtl;
  pad = 8;
  ((struct nusysmtl_s *)glass_mtl_blendskin2)->hShader = 0x12;
  NuMtlUpdate(glass_mtl_blendskin2);
  return;
}

//MATCH NGC
static s32 isGlassInstance(struct nugscn_s *gsc,struct nuinstance_s *inst) {
  struct nugeom_s *geom;

  geom = gsc->gobjs[inst->objid]->geom;
        if (inst->flags.visible >= 0) {
              return 0;
           }
          while( geom != NULL  ) {
            if ((((geom->mtl)->fxid == '\x01') || ((geom->mtl)->fxid == '\x03')) || ((geom->mtl)->fxid == 'd')) return 1;
            geom = geom->next;
          }
  return 0;
}

//MATCH NGC
static void NuGlassProcessScene(struct nugscn_s* gsc) {
    s32 i;
    struct nugeom_s* geom;
    struct nugeom_s* prev;
    struct nugeom_s* next;
    struct nuinstance_s* inst;

    for (i = 0; i < gsc->numinstance; i++) {
        if (isGlassInstance(gsc, &gsc->instances[i]) != 0) {
            if (0xff < num_glass_inst) {
                NuErrorProlog("C:/source/crashwoc/code/nu3dx/nuglass.c", 0x9f)("assert");
            }
            glass_inst[num_glass_inst] = &gsc->instances[i];
            inst = &gsc->instances[i];

            glassMtx[num_glass_inst] = &inst->mtx;

            // Initiate glass gobj
            glassGobj[num_glass_inst] = NuGobjCreate();
            glassGobj[num_glass_inst]->ngobjs = 1;
            glassGobj[num_glass_inst]->type = NUGOBJ_MESH;
            glassGobj[num_glass_inst]->geom = NULL;
            glassGobj[num_glass_inst]->faceon_geom = NULL;
            glassGobj[num_glass_inst]->next_gobj = NULL;
            glassGobj[num_glass_inst]->bounding_box_min = gsc->gobjs[inst->objid]->bounding_box_min;
            glassGobj[num_glass_inst]->bounding_box_max = gsc->gobjs[inst->objid]->bounding_box_max;
            glassGobj[num_glass_inst]->bounding_radius_from_origin = gsc->gobjs[inst->objid]->bounding_radius_from_origin;
            glassGobj[num_glass_inst]->bounding_rsq_from_origin = gsc->gobjs[inst->objid]->bounding_rsq_from_origin;
            glassGobj[num_glass_inst]->bounding_box_center = gsc->gobjs[inst->objid]->bounding_box_center;
            glassGobj[num_glass_inst]->bounding_radius_from_center = gsc->gobjs[inst->objid]->bounding_radius_from_center;
            glassGobj[num_glass_inst]->bounding_rsq_from_center = gsc->gobjs[inst->objid]->bounding_rsq_from_center;

            geom = gsc->gobjs[inst->objid]->geom;
            prev = NULL;

            for (geom = gsc->gobjs[inst->objid]->geom; geom != NULL;) {
                next = geom->next;
                if ((geom->mtl->fxid == 1) || (geom->mtl->fxid == 3) || (geom->mtl->fxid == 100)) {
                    geom->next = glassGobj[num_glass_inst]->geom;
                    glassGobj[num_glass_inst]->geom = geom;
                    if (prev != NULL) {
                        prev->next = next;
                    } else {
                        gsc->gobjs[inst->objid]->geom = next;
                    }
                } else {
                    prev = geom;
                }
                geom = next;
            }
            num_glass_inst++;
        }
    }
    return;
}

//MATCH NGC
void NuGlassLoad(struct nugscn_s *gsc,union variptr_u *buffer,union variptr_u *buffend) {
  if (buffer != NULL) {
    NuMemSetExternal(buffer,buffend);
  }
  NuGlassProcessScene(gsc);
  if (buffer != NULL) {
    NuMemSetExternal(NULL,NULL);
  }
  return;
}

//MATCH NGC
void NuGlassInit() {
  num_glass_inst = 0;
  InitSpecular();
  InitOverrideMtl();
  return;
}

//MATCH NGC
void NuGlassClose(void) {
  s32 i;

    for(i = 0; i < num_glass_inst; i++)
    {
      glass_inst[i] = 0;
      glassMtx[i] = 0;
      if (glassGobj[i] != NULL) {
        NuGobjDestroy(glassGobj[i]);
        glassGobj[i] = 0;
      }
    }
  num_glass_inst = 0;
  if (glass_mtl != NULL) {
    NuMtlDestroy(glass_mtl);
  }
  glass_mtl = NULL;
  return;
}

//MATCH NGC
void NuGlassRenderStatic(void) {
    s32 i;

    i = 0;
    ProcessGlass(0);
    nurndr_forced_mtl = glass_mtl;
    DrawGlassCreatures(0);
    nurndr_forced_mtl = NULL;
    if ((SKELETALCRASH != 0) || (glass_mix != 1.0f)) {
        NuRndrEndScene();
        NudxFw_MakeBackBufferCopy(1);
        NuRndrBeginScene(1);
    }
    if (num_glass_inst != 0) {
            for (i = 0; i < num_glass_inst; i++) {
                if (glass_inst[i]->flags.visitest != 0) {
                    NuRndrGobj(glassGobj[i],glassMtx[i],NULL);
                }
            }
    }
    return;
}

//MATCH NGC
void ProcessGlass(s32 paused) {
    float mix;

    if (glass_mtl == NULL) {
        return;
    }
    if (paused == 0) {
        if (Level == 23) {
            mix = WATERBOSSGLASSMIX;
        } else if (player->used != 0 && (player->obj).dead == 3) {
            mix = ANGELGLASSMIX;
        } else if (glass_enabled) {
            mix = MAXGLASSMIX;
        } else {
            mix = 0.0f;
        }
        if (glass_mix > mix) {
            glass_mix -= glass_mix_speed;
            if (glass_mix < mix) {
                glass_mix = mix;
            }
        } else if (glass_mix < mix) {
            glass_mix += glass_mix_speed;
            if (glass_mix > mix) {
                glass_mix = mix;
            }
        }
    }
    if (glass_mix != 1.0f) {
        glass_mtl->alpha = glass_mix;
        glass_mtl->attrib.alpha = 1;
        glass_mtl_blendskin->alpha = glass_mix;
        glass_mtl_blendskin->attrib.alpha = 1;
        glass_mtl_blendskin2->alpha = glass_mix;
        glass_mtl_blendskin2->attrib.alpha = 1;
    } else {
        glass_mtl->attrib.alpha = 0;
        glass_mtl_blendskin->attrib.alpha = 0;
        glass_mtl_blendskin2->attrib.alpha = 0;
    }
    //NuShaderSetGlassMix(glass_mix);
    glass_mtl->diffuse = glass_col[0];
    NuMtlUpdate(glass_mtl);
    NuMtlUpdate(glass_mtl_blendskin);
    NuMtlUpdate(glass_mtl_blendskin2);
    return;
}

//MATCH NGC
void DrawGlassCreatures(s32 solid) {
    if (Level != 0x17) {
        if ((GLASSPLAYER != 0) && (plr_invisibility_time < 5.0f)) {
        glass_phase = 1;
        glass_draw = solid;
        //DrawCreatures(Character,1,1,0);
        force_glass_screencopy_enable = 0;
    }

    }
    else {
        glass_draw = solid;
        glass_phase = 1;
        //DrawCreatures(Character + 1,8,1,1);
    }
    return;
}
