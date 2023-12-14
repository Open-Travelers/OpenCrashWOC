//#define ALIGN_ADDRESS(addr, al) (((u32)(addr) + ((u32)al - 1)) & ~((u32)al - 1))
#define ASSIGN_IF_SET(a, b) a = (a == NULL) ? NULL : b
#define ALIGN_ADDRESS(addr, al) (((u32)(addr) + (al - 1)) & ~(al - 1))
#define GET_IF_B_SET(a, b, c) a = (b == NULL) ? NULL : c

// this one is a common known-integer-divisor optimization, where instead
// of using DIV we multiply by some magic constant and then shift by 
// some amount, this particular combination should be equivalent to x/20,
// but the compiler doesn't feel like doing the optimization itself??
#define FAST_DIV_20(num) (((iVar5 - iVar4) * (s32)0xCCCCCCCD) >> 4)

#define PI 3.1415927finstNuCGutCharSysCreate
#define TAU 6.2831855f
#define MAX_FIXED_POINT 65536
#define DEG_TO_FIXED_POINT (MAX_FIXED_POINT * (1 / (2 * PI)))
short RO_640460[8];

struct NUGCUTLOCFNDATA_s* locatorfns;
static struct NUGCUTLOCFNDATA_s cutscene_locatorfns[2];
static struct instNUGCUTSCENE_s* active_cutscene_instances;

//PS2
void NuGCutSceneSysInit(struct NUGCUTLOCFNDATA_s *locfndat)
{
  locatorfns = locfndat;
  active_cutscene_instances = NULL;
  return;
}

//PS2
void NuGCutSceneSysUpdate(s32 paused)
{
    struct instNUGCUTSCENE_s* p;
    
    p = active_cutscene_instances;
    while( p != NULL ) {
        if (!p->been_updated_this_frame) {
            instNuGCutSceneUpdate(p, paused);
            p->been_updated_this_frame = 1;
        }
        p = p->next;
    }
    return;
}

//PS2
void NuGCutSceneSysRender(void)
{
    struct instNUGCUTSCENE_s *icutscene;
    
    for(icutscene = active_cutscene_instances; icutscene != NULL; icutscene = icutscene->next) { 
        instNuGCutSceneRender(icutscene);
        icutscene->been_updated_this_frame = 0;
    }
    
    return;
}


//PS2
struct NUGCUTSCENE_s * NuGCutSceneLoad(char *fname,union variptr_u *buff,union variptr_u *endbuff)
{
    struct NUGCUTSCENE_s *cutscene;
    s32 fsize;
    
    buff->voidptr = (void*)(((s32)buff->voidptr + 0xf) & 0xfffffff0);
    fsize = NuFileLoadBuffer(fname,buff->voidptr, (s32)endbuff->voidptr - (s32)buff->voidptr);
    if (fsize == 0) {
     NuErrorProlog("..\\nu2.ps2\\gamelib\\gcutscn.c", 0x149)(
         "Could not read from cutscene <fname>, either file is not there or the buffer may be full");
    }
    
    cutscene = (struct NUGCUTSCENE_s *)buff->voidptr;
    buff->voidptr = (void *)((s32)&cutscene->version + fsize);
    if (cutscene->version != 1) {
        NuErrorProlog("..\\nu2.ps2\\gamelib\\gcutscn.c", 0x14e)
            ("Out of date version of cutscene <%s>, you have version <%d>", fname, cutscene->version);
    }
    
    cutscene->address_offset = (s32)cutscene - cutscene->address_offset;
    
    ASSIGN_IF_SET(cutscene->name_table, cutscene->name_table + cutscene->address_offset);
    
    if (cutscene->cameras != NULL) {
        NuGCutCamsSysFixPtrs(cutscene, cutscene->address_offset);
    }
    
    if (cutscene->locators != NULL) {
        NuGCutLocatorSysFixPtrs(cutscene, cutscene->address_offset);
    }
    
    if (cutscene->rigids != NULL) {
        NuGCutRigidSysFixPtrs(cutscene, cutscene->address_offset);
    }
    
    if (cutscene->chars != NULL) {
        NuGCutCharSysFixPtrs(cutscene, cutscene->address_offset);
    }
    
    if (cutscene->triggersys != NULL) {
        NuGCutTriggerSysFixPtrs(cutscene, cutscene->address_offset);
    }

    ASSIGN_IF_SET(cutscene->bbox, (struct NUGCUTBBOX_s*)((s32)cutscene->bbox + cutscene->address_offset));
    
    return cutscene;
}


//PS2
void NuGCutSceneFixUp(struct NUGCUTSCENE_s *cutscene,struct nugscn_s *scene,struct NUTRIGGERSYS_s *triggersys)
{
  if (cutscene != NULL) {
    if (cutscene->rigids != NULL) {
      NuGCutRigidSysFixUp(cutscene,scene);
    }
    if (cutscene->chars != NULL) {
      NuGCutCharSysFixUp(cutscene);
    }
    if (cutscene->locators != NULL) {
      NuGCutLocatorSysFixUp(cutscene->locators);
    }
    if ((cutscene->triggersys != NULL) && (triggersys != NULL)
       ) {
      NuGCutTriggerSysFixUp(cutscene,triggersys);
    }
  }
  return;
}


//PS2
struct instNUGCUTSCENE_s *instNuGCutSceneCreate (struct NUGCUTSCENE_s *cutscene, struct nugscn_s *gscene,
                                                struct instNUTRIGGERSYS_s *itriggersys, char *name, union variptr_u *buff)
{
    struct instNUGCUTSCENE_s *icutscene;

    icutscene = 0;
    if (cutscene != NULL)
    {
        icutscene = buff->voidptr = ALIGN_ADDRESS(buff->voidptr, 0x10);
        buff->voidptr = &icutscene[1];
        
        memset(icutscene, 0, sizeof(struct instNUGCUTSCENE_s));
        
        icutscene->next = active_cutscene_instances;
        if (active_cutscene_instances != NULL) {
            active_cutscene_instances->prev = icutscene;
        }
        
        active_cutscene_instances = icutscene;
        icutscene->cutscene = cutscene;
        if (name != NULL) {
            sprintf(icutscene->name, name);
        }
        
        if (cutscene->cameras != NULL) {
            icutscene->icameras = instNuCGutCamSysCreate(cutscene->cameras, buff);
        }
        
        if (cutscene->rigids != NULL) {
            icutscene->irigids = instNuCGutRigidSysCreate(cutscene, gscene, buff);
        }
        
        if (cutscene->chars != NULL) {
            icutscene->ichars = instNuCGutCharSysCreate(cutscene, buff);
        }
        
        if (cutscene->locators != NULL) {
            icutscene->ilocators = NuCGutLocatorSysCreateInst(cutscene->locators, buff);
        }
        
        if ((cutscene->triggersys != NULL) && (itriggersys != NULL)) {
            icutscene->itriggersys = instNuCGutTriggerSysCreate(cutscene, itriggersys, buff);
        }
        
        instNuGCutSceneCalculateCentre(icutscene, 0);
        icutscene->rate = 1.0f;
    }
    
    return icutscene;
}

void instNuGCutSceneDestroy(instNUGCUTSCENE_s *icutscene)

{
  if ((*(uint *)&icutscene->field_0x6c & 0x40000000) != 0) //param_1[0x1b] & 0x40000000U
  {
    instNuGCutSceneEnd(icutscene);
  }
  if (icutscene->next != (instNUGCUTSCENE_s *)0x0) {
    icutscene->next->prev = icutscene->prev;
  }
  if (icutscene->prev == (instNUGCUTSCENE_s *)0x0) {
    active_cutscene_instances = icutscene->next;
  }
  else {
    icutscene->prev->next = icutscene->next;
  }
  return;
}

//PS2
inline static void instNuGCutSceneStart(struct instNUGCUTSCENE_s* icutscene)
{
    icutscene->cframe = 1.0f;
    
    icutscene->is_playing = 1;
    icutscene->played_first_frame = 0;
    
    if (icutscene->icameras != NULL) {
        instNuGCutCamSysStart(icutscene->icameras, icutscene->cutscene->cameras);
    }
    
    if (icutscene->irigids != NULL) {
        instNuGCutRigidSysStart(icutscene->irigids, icutscene->cutscene->rigids);
    }
    
    if (icutscene->ilocators != NULL) {
        instNuGCutLocatorSysStart(icutscene->ilocators, icutscene->cutscene->locators);
    }
    
    if (icutscene->itriggersys != NULL) {
        instNuGCutTriggerSysStart(icutscene);
    }
    return;
}

//PS2
inline static void instNuGCutSceneEnd(struct instNUGCUTSCENE_s *icutscene)
{
    struct NUGCUTSCENE_s* cutscene;
    float nframes;
    
    cutscene = icutscene->cutscene;
    nframes = cutscene->nframes;
    
    icutscene->is_playing = 0;
    icutscene->cframe = nframes + 1.0f;

//PS2 part
   /* if ( cutscene->rigids != NULL) {
        instNuGCutRigidSysEnd(icutscene->cframe , icutscene);
    }
    
    if (icutscene->ichars != NULL) {
        instNuGCutCharSysEnd(icutscene->cframe, icutscene);
    }
    
    if (icutscene->ilocators != NULL) {
        instNuGCutLocatorSysEnd(icutscene->cframe, icutscene->ilocators, cutscene->locators);
    }*/
    
    return;
}


//PS2
void instNuGCutSceneSetEndCallback(struct instNUGCUTSCENE_s *icutscene,void(*fn)(void*))
{
  icutscene->endfn = fn;
  return;
}


inline static void instNuGCutSceneClipTest(struct instNUGCUTSCENE_s *icutscene)
{
    struct NUGCUTBBOX_s *bbox; //temp
    struct numtx_s *wm; //temp
    
    struct NUGCUTSCENE_s *cutscene;
    float distsqr;
    int r;

    cutscene = icutscene->cutscene;
    icutscene->is_visible = 1;
    
    if (cutscene->bbox == NULL) {
        return;
    }

    if ((icutscene->checkmaxdist)) {
        if((NuCameraDistSqr(&icutscene->centre) > icutscene->maxdistsqr)) {
            icutscene->is_visible = 0;
            return;
        }
    }
    
    if ((icutscene->checkbboxclip))
    {
        if (icutscene->has_mtx)
        {
            bbox = cutscene->bbox;
            wm = &icutscene->mtx;
        }
        else {
            bbox = cutscene->bbox;
            wm = &numtx_identity;
        }
        
        if (NuCameraClipTestExtents(&bbox->min, &bbox->max, wm) == 0) {
            icutscene->is_visible = 0;
        }
    }
    return;
}

//PS2
static void instNuGCutSceneUpdate(struct instNUGCUTSCENE_s *icutscene,s32 paused)
{
    /*
    NUGCUTSCENE_s* cutscene; //
    float dt; //
    void(*endfn)(void*);
    */
    struct NUGCUTSCENE_s *cutscene;
    float dt;
    float dt1;
    float dt2;
    
    cutscene = icutscene->cutscene;
    if (icutscene->is_disabled)
    {
        return;
    }
    
    if (!icutscene->played_first_frame && icutscene->autostart)
    {
        instNuGCutSceneStart(icutscene);
    }
    
    if ((icutscene->is_playing) == 0)
    {
        return;
    }
    instNuGCutSceneClipTest(icutscene);
    
    if ((icutscene->is_visible) == 0)
    {
        return;
    }

    if ((icutscene->played_first_frame) != 0)
    {
        if (paused == 0) {
            dt = icutscene->cframe + icutscene->rate;
            icutscene->cframe = dt;
            if (1.0f > dt) {
                icutscene->cframe = 1.0f;
            } else {
                if (dt >= (cutscene->nframes + 1.0f) - 1.0f) 
                {
                    if (icutscene->endfn != NULL) {
                        void(*old)(void*) = icutscene->endfn;
                        icutscene->endfn = NULL;
                        old(icutscene);
                    }
                    
                    if (icutscene->next_to_play != NULL) {
                        dt1 = icutscene->cframe -((cutscene->nframes + 1.0f) - 1.0f);
                        if (cutscene->nframes < dt1) {
                            dt1 = 0.0f;
                        }
                        
                        instNuGCutSceneEnd(icutscene);
                        instNuGCutSceneStart(icutscene->next_to_play);
                        icutscene->next_to_play->cframe = icutscene->next_to_play->cframe + dt1;
                        
                        icutscene->next_to_play->played_first_frame = 1;
                        instNuGCutSceneClipTest(icutscene->next_to_play);
                        icutscene->next_to_play->been_updated_this_frame = 1;
                        icutscene->next_to_play = NULL;
                        return;
                    }
                    
                    if ((icutscene->looping) != 0) //0x20
                    {
                        dt2 = (icutscene->cframe - ((cutscene->nframes + 1.0f) - 1.0f));
                        if (cutscene->nframes < dt2) {
                            dt2 = 0.0f;
                        }
                        instNuGCutSceneStart(icutscene);
                        icutscene->cframe = icutscene->cframe + dt2;
                        icutscene->played_first_frame = 1;
                    } else {
                        instNuGCutSceneEnd(icutscene);
                    }
                }
            }
        }
    }
    else{
        icutscene->played_first_frame = 1;
    }

    if ((do_cameras != 0) && (icutscene->icameras != NULL)) {
        instNuGCutCamSysUpdate(icutscene, icutscene->cframe);
    }
    if (icutscene->itriggersys != NULL) {
        instNuGCutTriggerSysUpdate(icutscene, icutscene->cframe);
    }
    if ((do_rigids != 0) && (icutscene->irigids != NULL)) {
        instNuGCutRigidSysUpdate(icutscene, icutscene->cframe);
    }
    
    return;
}

//PS2 (change bifields from s32 to s16 type)
static void instNuGCutSceneRender(struct instNUGCUTSCENE_s *icutscene)
{
    
    if (icutscene->is_disabled) {
        return;
    }
    
    if (!icutscene->is_playing) {
        return;
    }
    
    if (!icutscene->is_visible){
        return;
    }
    
    if ((do_rigids != 0) && (icutscene->irigids != NULL)) {
        instNuGCutRigidSysRender(icutscene, icutscene->cframe);
    }
    if ((do_chars != 0) && (icutscene->ichars != NULL)) {
        instNuGCutCharSysRender(icutscene, icutscene->cframe);
    }
    if ((do_locator != 0) && (icutscene->ilocators != NULL)) {
        instNuGCutLocatorSysUpdate(icutscene, icutscene->cframe);
    }
}

//PS2
static void NuGCutCamsSysFixPtrs(struct NUGCUTSCENE_s *cutscene, s32 address_offset)
{
    struct NUGCUTCAMSYS_s *camsys;
    
    if (cutscene->cameras != NULL) {
        camsys = (struct NUGCUTCAMSYS_s *)((s32)cutscene->cameras + address_offset);
        cutscene->cameras = camsys;
        ASSIGN_IF_SET(camsys->cutcams, (struct NUGCUTCAM_s *)((s32)camsys->cutcams + address_offset));
        
        if (camsys->camanim != NULL) {
            camsys->camanim = NuAnimData2FixPtrs(camsys->camanim, address_offset);
        }
        camsys->camchanges = StateAnimFixPtrs(camsys->camchanges, address_offset);
    }
    return;
}

//PS2
static void instNuGCutSceneCalculateCentre(struct instNUGCUTSCENE_s *icutscene,struct numtx_s *mtx)
{
  struct NUGCUTSCENE_s *cutscene;
  
  cutscene = icutscene->cutscene;
  if (cutscene->bbox != NULL) {
    (icutscene->centre).x = ((cutscene->bbox->max).x + (cutscene->bbox->min).x) * 0.5f;
    (icutscene->centre).y = ((cutscene->bbox->max).y + (cutscene->bbox->min).y) * 0.5f;
    (icutscene->centre).z = ((cutscene->bbox->max).z + (cutscene->bbox->min).z) * 0.5f;
  }
  else {
    (icutscene->centre).x = 0.0f;
    (icutscene->centre).y = 0.0f;
    (icutscene->centre).z = 0.0f;
  }
  if (mtx != NULL) {
    NuVecMtxTransform(&icutscene->centre,&icutscene->centre,mtx);
  }
  return;
}

//PS2
struct instNUGCUTCAMSYS_s * instNuCGutCamSysCreate(struct NUGCUTCAMSYS_s *cameras,union variptr_u *buff)
{
  struct instNUGCUTCAMSYS_s *icameras;
  
  icameras = NULL;
  if (((cameras != NULL) && (cameras->ncutcams != 0)) && (buff != NULL ))
  {
    icameras = (struct instNUGCUTCAMSYS_s *)(((int)buff->voidptr + 0xf) & 0xfffffff0);
    buff->voidptr = icameras + 1;
    memset(icameras,0,0x10);
    buff->intaddr = ((u32)buff->voidptr + 0xf) & 0xfffffff0;
    icameras->icutcams = (struct instNUGCUTCAM_s *)buff->intaddr;
    buff->voidptr = (void *)((u32)buff->voidptr + cameras->ncutcams * 4);
    memset(icameras->icutcams,0,cameras->ncutcams << 2);
  }
  return icameras;
}


//PS2
static void instNuGCutCamSysUpdate(struct instNUGCUTSCENE_s *icutscene, float current_frame)
{
    struct instNUGCUTCAMSYS_s *icamsys;
    struct NUGCUTCAMSYS_s *camsys;
    struct nuanimtime_s atime;
    struct NUGCUTCAM_s *cutcam;
    struct instNUGCUTCAM_s *icutcam;
    struct instNUGCUTCAM_s *icutcam2;
    struct NUGCUTCAMTGT_s *cutcamtgt;
    struct numtx_s aim_mtx;
    float lerp_factor;
    float nframes;
    struct nuquat_s q_from;
    struct nuquat_s q_to;
    struct nuquat_s q_result;
    s32 lerp_out;
    struct NuVec campos;
    u8 camchange;
    struct nuanimcurve2_s* curves;
    s8* curveflags;
    s32 curvesetflags;
    //////////////////////////////////////////////////
    s32 curve_ix;

    
    camsys = icutscene->cutscene->cameras;
    icamsys = icutscene->icameras;
    if ((camsys->camchanges != NULL) && (StateAnimEvaluate(camsys->camchanges, &icamsys->next_switch, &camchange, current_frame) != 0)) {
        icamsys->current_camera = camchange;
    }
    
    if (icamsys->next_tgt_ix < icamsys->ntgts) {
        
        for(; icamsys->tgts[icamsys->next_tgt_ix].frame <= current_frame; )
        {
            cutcamtgt = &icamsys->tgts[icamsys->next_tgt_ix];
            icutcam = &icamsys->icutcams[camsys->remap_table[cutcamtgt->camid]];
            icutcam->flags |= 2;
            icutcam->tgt_ix = icamsys->next_tgt_ix;
            icamsys->next_tgt_ix++;
            if (icamsys->ntgts <= icamsys->next_tgt_ix) break;
        } 
    }

    
    if (icamsys->next_tgt_ix != 0){
        for(; current_frame < icamsys->tgts[icamsys->next_tgt_ix].frame;)
        {
            icamsys->next_tgt_ix--;
            cutcamtgt = &icamsys->tgts[icamsys->next_tgt_ix];
            icutcam = &icamsys->icutcams[camsys->remap_table[cutcamtgt->camid]];
            icutcam->flags |= 2;
            icutcam->tgt_ix = icamsys->next_tgt_ix;
            if (icamsys->next_tgt_ix == 0) break;
        }
    }
    
    if (icamsys->current_camera < 0) {
        return;
    }
    
    if (icamsys->current_camera >= camsys->ncutcams) {
        NuErrorProlog("..\\nu2.ps2\\gamelib\\gcutscn.c", 0x378)("instNuGCutCamSysUpdate: internal error");
    }
    
    icutcam2 = &icamsys->icutcams[icamsys->current_camera];
    cutcam = camsys->cutcams + icamsys->current_camera;
    if (((cutcam->flags & 1) != 0) && (cutcam->anim_ix < camsys->camanim->nnodes)) {
        NuAnimData2CalcTime(camsys->camanim, current_frame, &atime);
        curve_ix = (int)camsys->camanim->ncurves * (u32)cutcam->anim_ix;
        curves = &camsys->camanim->curves[curve_ix];
        curveflags = &camsys->camanim->curveflags[curve_ix];
        curvesetflags = camsys->camanim->curvesetflags[cutcam->anim_ix];
        NuAnimCurve2SetApplyToMatrix(curves, curveflags, curvesetflags, &atime, &cutscenecammtx);
    } else {
        memcpy(&cutscenecammtx, &cutcam->mtx, sizeof(struct numtx_s));    
    }
    
    if ((icutscene->has_mtx) != 0) {
        NuMtxMul(&cutscenecammtx, &cutscenecammtx, &icutscene->mtx);
    }
    
    set_cutscenecammtx = 1;
    if ((icutcam2->flags & 2) != 0) {
        lerp_out = 0;
        cutcamtgt = icamsys->tgts + icutcam2->tgt_ix;
        lerp_factor = NuFabs(cutcamtgt->nframes);
        
        if ((NuFsign(cutcamtgt->nframes) < 0.0f)){
            lerp_out = 1;
        }

        if ((lerp_out) && ((current_frame >= cutcamtgt->frame + lerp_factor))){
            icutcam2->flags = icutcam2->flags & 0xFD;
            return;
        }
        
        memcpy(&campos, &cutscenecammtx._30, sizeof(struct NuVec));
        memcpy(&aim_mtx, &cutscenecammtx, sizeof(struct numtx_s));
        
        NuMtxLookAtZ(&aim_mtx, cutcamtgt->tgt);
        if (0.01f < lerp_factor) {
            lerp_factor = (current_frame - cutcamtgt->frame) / lerp_factor;
            if (1.0f < lerp_factor) {
                lerp_out = 1;
            }
            else if (lerp_factor < 0.0f) {
                lerp_out = 0;
            }
        }
        else {
            lerp_factor = 1.0f;
        }
        if (lerp_out) {
            lerp_factor = 1.0f - lerp_factor;
        }
        
        NuMtxToQuat(&aim_mtx, &q_to);
        NuMtxToQuat(&cutscenecammtx, &q_from);
        NuQuatSlerp(&q_result, &q_from, &q_to, lerp_factor);
        NuQuatToMtx(&q_result, &cutscenecammtx);
        memcpy(&cutscenecammtx._30, &campos, sizeof(struct NuVec));
    }
    return;
}

//PS2
void instNuGCutCamSysStart(struct instNUGCUTCAMSYS_s *icamsys,struct NUGCUTCAMSYS_s *camsys)
{
  struct instNUGCUTCAM_s *icutcam;
  u32 i;
  
  icamsys->next_switch = '\0';
  icamsys->current_camera = camsys->initial_camid;
  icamsys->next_tgt_ix = '\0';
    for (i = 0; camsys->ncutcams > i; i++)
    {
      icutcam = &icamsys->icutcams[i];
      icutcam->tgt_ix = '\0';
      icutcam->flags = icutcam->flags & 0xfd;
    }
  return;
}


//PS2
static void NuGCutLocatorSysFixPtrs(struct NUGCUTSCENE_s *cutscene,s32 address_offset)
{
  struct NUGCUTLOCATOR_s *locator;
  struct NUGCUTLOCATORTYPE_s *loctype;
  struct NUGCUTLOCATORSYS_s *locatorsys;
  s32 i;
  
  if (cutscene->locators != NULL) {
    locatorsys = (char*)cutscene->locators + address_offset;
    cutscene->locators = locatorsys;
      
    ASSIGN_IF_SET(locatorsys->locators, (struct NUGCUTLOCATOR_s *)((s32)locatorsys->locators + address_offset));
    if ((locatorsys->locators != NULL)) {
      for(i = 0; i < (s32)locatorsys->nlocators; i++)
      {
        locator = &locatorsys->locators[i];
        if (locator->anim != NULL) {
          locator->anim =  NuAnimData2FixPtrs(locator->anim,address_offset);
        }
      }
    }
    ASSIGN_IF_SET(locatorsys->locator_types, (struct NUGCUTLOCATOR_s *)((s32)locatorsys->locator_types + address_offset));
    if ((locatorsys->locator_types != NULL)) {
      for(i = 0; i < (s32)locatorsys->ntypes; i++)
      {
        loctype = &locatorsys->locator_types[i];
        loctype->name = &cutscene->name_table[(s32)(loctype->name)] - 1;
      }
    }
  }
  return;
}

//PS2
void NuGCutLocatorCalcMtx
               (struct NUGCUTLOCATOR_s *locator,float current_frame, struct numtx_s *mtx,struct nuanimtime_s *atime)
{
  char curvesetflags;
  struct nuanimcurve2_s *curves;
  char *curveflags;
  //struct numtx_s* in_t0_lo;
  struct NuVec t;
  struct NuVec r;
  struct nuangvec_s rf;
  

  if (locator->anim != NULL) {
    curves = locator->anim->curves;
    curveflags = locator->anim->curveflags;
    curvesetflags = *locator->anim->curvesetflags;
    if ((curvesetflags & 1U) != 0) {
      r.x = NuAnimCurve2CalcVal(curves + 3,atime,(int)curveflags[3]);
      r.y = NuAnimCurve2CalcVal(curves + 4,atime,(int)curveflags[4]);
      r.z = NuAnimCurve2CalcVal(curves + 5,atime,(int)curveflags[5]);
      rf.x = (s32)(r.x * 10430.378f);
      rf.y = (s32)(r.y * 10430.378f);
      rf.z = (s32)(r.z * 10430.378f);
      NuMtxSetRotateXYZ(mtx,&rf);
    }
    else {
      NuMtxSetIdentity(mtx);
    }
      curvesetflags = *curveflags;
    t.x = NuAnimCurve2CalcVal(curves,atime,(int)curvesetflags);
    t.y = NuAnimCurve2CalcVal(curves + 1,atime,(int)curveflags[1]);
    t.z = NuAnimCurve2CalcVal(curves + 2,atime,(int)curveflags[2]);
    NuMtxTranslate(mtx,&t);
    mtx->_02 = -mtx->_02;
    mtx->_12 = -mtx->_12;
    mtx->_20 = -mtx->_20;
    mtx->_21 = -mtx->_21;
    mtx->_23 = -mtx->_23;
    mtx->_32 = -mtx->_32;
    NuMtxTranslate(mtx,(struct NuVec *)&(locator->mtx)._30);
  }
  else {
     //memcpy

      //*mtx = locator->mtx;
      memcpy(mtx, &locator->mtx, sizeof(struct numtx_s));
  }
  return;
}


//PS2
s32 NuGCutLocatorIsVisble
              (struct NUGCUTLOCATOR_s *locator,float current_frame,struct nuanimtime_s *atime,float *rate)
{
  s32 is_visible;
  char *curveflags;
  struct nuanimcurve2_s *curves;
  

  if (locator->anim != NULL) {
    curveflags = locator->anim->curveflags;
    curves = locator->anim->curves;
    is_visible = (s32)NuAnimCurve2CalcVal((struct nuanimcurve2_s *)(curves + 7),atime,(s32)curveflags[7 ]);;
    if ((is_visible != 0) && (rate != NULL)) {
      *rate = NuAnimCurve2CalcVal((struct nuanimcurve2_s *)(curves + 6),atime,
                                  (s32)curveflags[6]);
    }
  }
  else {
    is_visible = locator->flags & 8;
    if (((locator->flags & 8) != 0) && (rate != NULL)) {
      *rate = locator->rate;
    }
  }
  return is_visible;
}


//PS2
struct instNUGCUTLOCATORSYS_s * NuCGutLocatorSysCreateInst(struct NUGCUTLOCATORSYS_s * locators, union variptr_u *buff)
{
  struct instNUGCUTLOCATORSYS_s *iloc;
  
  iloc = NULL;

  if (((locators != NULL) &&
      (iloc = NULL, locators->nlocators != '\0')) &&
     (buff != NULL)) {
    iloc = (struct instNUGCUTLOCATORSYS_s *)(((s32)buff->voidptr + 0xf) & 0xfffffff0);
    buff->voidptr = iloc + 1;
    iloc->ilocators = NULL;
    buff->intaddr = (u32)(((u32)buff->voidptr + 0xf) & 0xfffffff0);
    iloc->ilocators = (struct instNUGCUTLOCATOR_s *) buff->intaddr;
    buff->voidptr = (void *)((u32)buff->voidptr + (u32)locators->nlocators * 8);
    memset(iloc->ilocators,0,((u32)locators->nlocators << 3));
  }
  return iloc;
}

//PS2
static void instNuGCutLocatorSysUpdate(struct instNUGCUTSCENE_s *icutscene,float current_frame)
{
  struct NUGCUTLOCATORSYS_s *locatorsys;
  struct instNUGCUTLOCATORSYS_s *ilocatorsys;
  struct NUGCUTLOCATOR_s *locator;
  //struct NUGCUTSCENE_s* cutscene;
  u32 i;
    
  locatorsys = icutscene->cutscene->locators;    //locatorsys = cutscene->locators;
  ilocatorsys = icutscene->ilocators;
    for (i = 0; i < locatorsys->nlocators; i++)
    {
      locator = &locatorsys->locators[i];
      if ((locator->flags & 1) == 0) {
        if ((locator->flags & 2) == 0) {
            if (icutscene->has_mtx & 0x10) {
            instNuGCutLocatorUpdate
                      (icutscene,locatorsys,ilocatorsys->ilocators + i,locator,current_frame,
                       &icutscene->mtx);
          }
          else {
            instNuGCutLocatorUpdate
                      (icutscene,locatorsys,ilocatorsys->ilocators + i,locator,current_frame,0);
          }
        }
      }
    }
  return;
}

//PS2
void instNuGCutLocatorSysStart(struct instNUGCUTLOCATORSYS_s *ilocatorsys,struct NUGCUTLOCATORSYS_s *locatorsys )
{
  struct instNUGCUTLOCATOR_s *ilocator;
  u32 i;

    for(i = 0; (u32)locatorsys->nlocators > i; i++)
    {
      ilocator = &ilocatorsys->ilocators[i];
      ilocator->timer = 0.0f;
    }
  return;
}

//PS2
inline static s32 LookupLocatorFn(char* name)
{
    s32 ix = 0;
      if (locatorfns != NULL) {
          while (locatorfns[ix].name) {
              if (strcasecmp(name,locatorfns[ix].name) == 0) {
                  return ix;
              }
              ix++;
          }
      }
  return -1;
}

//PS2
static void NuGCutLocatorSysFixUp(struct NUGCUTLOCATORSYS_s *locatorsys) 
{
  u32 i;
  struct NUGCUTLOCATORTYPE_s *loctype;

    for(i = 0; i < locatorsys->ntypes; i++) {
          
      loctype = &locatorsys->locator_types[i];
      if ((loctype->flags & 1) != 0) {
        loctype->ix = LookupDebrisEffect(loctype->name);
      }
      else {
        if ((loctype->flags & 2) != 0) {
          loctype->ix = LookupLocatorFn(loctype->name);
        }
      }
      if (loctype->ix == 0xFFFF) {
        NuDebugMsgProlog("..\\nu2.ps2\\gamelib\\gcutscn.c", 0x4FB)
            ("NuGCutLocatorSysFixUp: cannot fixup locator <%s> in cutscene", loctype->name);
      }
    }
  return;
}

//PS2
void instNuGCutLocatorUpdate
               (struct instNUGCUTSCENE_s *icutscene,struct NUGCUTLOCATORSYS_s *locatorsys,
               struct instNUGCUTLOCATOR_s *ilocator,struct NUGCUTLOCATOR_s *locator,
                float current_frame,struct numtx_s * wm)
{
    struct NUGCUTLOCATORTYPE_s* loctype;
    
    loctype = &locatorsys->locator_types[locator->locator_type];
    
    if ((loctype->flags & 1U) != 0) {
        instNuGCutDebrisLocatorUpdate(locatorsys, ilocator, locator, current_frame, wm);
    }
    else if (((loctype->flags & 2U) != 0) && (loctype->ix != 0xffff)) {
        (locatorfns[loctype->ix].fn)(icutscene, locatorsys, ilocator, locator, current_frame, wm);
    }
    
    return;
}


//NGC MATCH
static void instNuGCutDebrisLocatorUpdate(struct NUGCUTLOCATORSYS_s* locatorsys, struct instNUGCUTLOCATOR_s* ilocator, struct NUGCUTLOCATOR_s* locator, float current_frame, struct numtx_s* wm)
{
    struct numtx_s mtx;
    float rate;
    s32 num_shots;
    struct NUGCUTLOCATORTYPE_s* loctypes;
    u8 flag;
    s16 roty;
    s16 rotz;
    struct nuanimtime_s atime;
    struct NuVec dir;
    float fVar3; //temp?
    s32 local_34; //temp?

     char pad [5];
    
    loctypes = locatorsys->locator_types; //loctypes->flags
    flag = loctypes[locator->locator_type].ix;
    if (flag != 0xff) { //0x7 ?? --> ix
        if (locator->anim != NULL) {
            NuAnimData2CalcTime(locator->anim,current_frame,&atime);
        }
        if (NuGCutLocatorIsVisble(locator,current_frame,&atime,&rate) != 0) {
            if (rate > 0.0f) {
                if (rate < 0.0f) {
                    rate = 0.0f;
                }
                num_shots = 0;
                for (ilocator->timer += rate; 0.0f < ilocator->timer; ilocator->timer -= 1.0f) {
                        num_shots++;
                }
                if (num_shots != 0) {
                    NuGCutLocatorCalcMtx(locator,current_frame,&mtx,&atime);
                    if ((locator->flags & 4) != 0) {
                        NuMtxPreTranslate(&mtx,&locator->pivot);
                    }
                    if (wm != NULL) {
                        NuMtxMul(&mtx,&mtx,wm);
                    }
                    CalculateLocatorDirection(locator->direction,&mtx,&dir);
                    fVar3 = NuAtan2(dir.x,dir.z);
                    roty = NuAngSub((fVar3 * 10430.378f),0x4000);
                    fVar3 = NuFsqrt(dir.x * dir.x + dir.z * dir.z);
                    fVar3 = NuAtan2(fVar3,-dir.y);
                    rotz = (s16)(fVar3 * 10430.378f);
                    if ((locator->flags & 0x10) != 0) {
                        local_34 = -1;
                        AddFiniteShotDebrisEffect(&local_34,flag,(struct NuVec *)&mtx._30,num_shots);
                        DebrisEmitterOrientation(local_34,rotz,roty);
                    }
                    else {
                        if ((((flag == 0x23) || (flag == 10)) || (flag == 0x24)) ||
                           (((flag == 9 || (flag == 0x19)) || (flag == 0x25)))) {
                            num_shots = num_shots / 2;
                        }
                        AddVariableShotDebrisEffect(flag,(struct NuVec *)&mtx._30,num_shots,rotz,roty );
                    }
                }
            }
            else {
                ilocator->timer = 0.0f;
            }
        }
    }
    return;
}


//PS2
static void NuGCutRigidSysFixPtrs(struct NUGCUTSCENE_s* cutscene, s32 address_offset)
{

    struct NUGCUTRIGIDSYS_s *rigidsys;
    struct NUGCUTRIGID_s *rigid;
    s32 i;

  
    if (cutscene->rigids != NULL) {
        rigidsys = (struct NUGCUTRIGIDSYS_s *)((int)cutscene->rigids + address_offset);
        cutscene->rigids = rigidsys;
        ASSIGN_IF_SET(rigidsys->rigids, (struct NUGCUTRIGID_s *)((int)(rigidsys->rigids) + address_offset)); //rigidsys[address_offset].rigids
        
        if ((rigidsys->rigids != NULL)) {
            for(i = 0; i < rigidsys->nrigids; i++)
            {
                rigid = &rigidsys->rigids[i];
                if (rigid->name != NULL) {
                    rigid->name = &cutscene->name_table[(u32)rigid->name] - 1; // ????
                }

                if (rigid->anim != NULL) {
                    rigid->anim = NuAnimData2FixPtrs(rigid->anim, address_offset);
                }
                rigid->visibiltyanim = StateAnimFixPtrs(rigid->visibiltyanim, address_offset);
            }
        }
    }
    return;
}

//PS2
static void NuGCutRigidSysFixUp(struct NUGCUTSCENE_s *cutscene,struct nugscn_s *scene)
{
    struct NUGCUTRIGIDSYS_s *rigidsys;
    struct NUGCUTLOCATORSYS_s *locator;
    struct NUGCUTRIGID_s *rigid;
    s32 i;
    
    
    rigidsys = cutscene->rigids;
    locator = cutscene->locators;
    if (scene != NULL) {
        for(i = 0; i < rigidsys->nrigids; i++)
        {
            rigid = &rigidsys->rigids[i];
            
            if (NuSpecialFind(scene, &rigid->special, rigid->name) != 0) {
                rigid->flags |= 4;
            } else {
                NuDebugMsgProlog("..\\nu2.ps2\\gamelib\\gcutscn.c", 0x5d6)
                    ("NuGCutRigidSysFixUp: cannot fixup rigid object <%s> in cutscene", rigid->name);
            }
            
            if (rigid->nlocators == 0) {
                rigid->first_locator = 0xff;
            }
            else if ((s32)rigid->locators < 0xff) {
                rigid->first_locator = rigid->locators;
                rigid->locators = &locator->locators[(s32)rigid->locators];
            }
            else {
                rigid->first_locator = 0xff;
            }
        }
    }
    return;
}

//PS2
static struct instNUGCUTRIGIDSYS_s *
instNuCGutRigidSysCreate(struct NUGCUTSCENE_s *cutscene,struct nugscn_s *gscene,union variptr_u *buff)
{
    
    struct NUGCUTRIGIDSYS_s *rigids;
    struct instNUGCUTRIGIDSYS_s *irigids;
    struct NUGCUTRIGID_s *cutrigid;
    struct instNUGCUTRIGID_s *icutrigid;
    s32 i;
    s32 iVar4;
    s32 iVar5;
    
    irigids = NULL;
    rigids = cutscene->rigids;
    if ((rigids != NULL) && (rigids->nrigids != 0) && (buff != NULL))
    {
        irigids = buff->voidptr = (struct instNUGCUTRIGIDSYS_s *)ALIGN_ADDRESS(buff->voidptr, 0x10);
        buff->voidptr = &irigids[1];
        
        irigids->irigids = NULL;
        icutrigid = buff->voidptr = (struct instNUGCUTRIGID_s *)ALIGN_ADDRESS(buff->voidptr, 0x10);
        irigids->irigids = icutrigid;
        
        buff->voidptr = &buff->voidptr[rigids->nrigids * sizeof(struct instNUGCUTRIGID_s)];
        memset(irigids->irigids, 0, (rigids->nrigids * sizeof(struct instNUGCUTRIGID_s)));
        
        for (i = 0; i < rigids->nrigids; i++)
        {
            cutrigid = &rigids->rigids[i];
            icutrigid = &irigids->irigids[i];
            if ((gscene != NULL) && (gscene != cutrigid->special.scene)) {
                if (cutrigid->special.special != NULL) {
                    icutrigid->special.scene = gscene;
                    iVar4 = (s32)cutrigid->special.scene->specials;
                    iVar5 = (s32)cutrigid->special.special;
                    icutrigid->special.special = &gscene->specials[FAST_DIV_20(iVar5 - iVar4)];
                } else {
                    NuDebugMsgProlog("..\\nu2.ps2\\gamelib\\gcutscn.c", 0x606)
                        ("instNuCGutRigidSysCreate: cannot reference rigid object <%s>, object was not fixed up", cutrigid->name);
                }
            }
            else {
                icutrigid->special = cutrigid->special;
            }
        }
    }
    return irigids;
}



//PS2 Match
static void NuGCutRigidCalcMtx(struct NUGCUTRIGID_s* rigid, float current_frame, struct numtx_s* mtx) 
{
  struct nuanimcurve2_s *curves;
  struct nuanimtime_s atime;
  struct NuVec t;
  struct NuVec r;
  struct nuangvec_s rf;
  struct NuVec scale;
  char *curveflags;
  char curvesetflags;
  
  if (rigid->anim != NULL) {
    curves = rigid->anim->curves;
    curvesetflags = *rigid->anim->curvesetflags;
    curveflags = rigid->anim->curveflags;
    NuAnimData2CalcTime(rigid->anim,current_frame,&atime);
    if ((curvesetflags & 1) != 0) {
      r.x = NuAnimCurve2CalcVal(curves + 3,&atime,(s32)curveflags[3]);
      r.y = NuAnimCurve2CalcVal(curves + 4,&atime,(s32)curveflags[4]);
      r.z = NuAnimCurve2CalcVal(curves + 5,&atime,(s32)curveflags[5]);
      rf.z = (s32)(r.z * 10430.378f);
      rf.x = (s32)(r.x * 10430.378f);
      rf.y = (s32)(r.y * 10430.378f);
      NuMtxSetRotateXYZ(mtx,&rf);
    }
    else {
      NuMtxSetIdentity(mtx);
    }
    if ((curvesetflags & 8) != 0) {
      scale.x = NuAnimCurve2CalcVal(curves + 6,&atime,(s32)curveflags[6]);
      scale.y = NuAnimCurve2CalcVal(curves + 7,&atime,(s32)curveflags[7]);
      scale.z = NuAnimCurve2CalcVal(curves + 8,&atime,(s32)curveflags[8]);
      NuMtxPreScale(mtx,&scale);
    }
    t.x = NuAnimCurve2CalcVal(curves,&atime,(s32)*curveflags);
    t.y = NuAnimCurve2CalcVal(curves + 1,&atime,(s32)curveflags[1]);
    t.z = NuAnimCurve2CalcVal(curves + 2,&atime,(s32)curveflags[2]);
    NuMtxTranslate(mtx,&t);
    mtx->_02 = -mtx->_02;
    mtx->_12 = -mtx->_12;
    mtx->_20 = -mtx->_20;
    mtx->_21 = -mtx->_21;
    mtx->_23 = -mtx->_23;
    mtx->_32 = -mtx->_32;
    NuMtxTranslate(mtx,(struct NuVec*)&(rigid->mtx)._30);	//CHECK param2!
  }
  else {
    //memcpy
      *mtx = rigid->mtx;
  }
  return;
}

//PS2
static void instNuGCutRigidSysStart(struct instNUGCUTRIGIDSYS_s *irigidsys,struct NUGCUTRIGIDSYS_s *rigidsys)
{
  u32 i;
    
  for(i = 0; i < rigidsys->nrigids; i++) {
      irigidsys->irigids[i].visibleframeix = '\0';
  }
}

//PS2
static void instNuGCutRigidSysUpdate(struct instNUGCUTSCENE_s* icutscene, float current_frame)
{
    struct NUGCUTRIGID_s *rigid; 
    struct instNUGCUTRIGID_s *irigid;
    s32 i;
    s32 j;
    struct numtx_s mtx;
    struct NUGCUTSCENE_s *cutscene;
    struct NUGCUTRIGIDSYS_s *rigidsys;
    struct instNUGCUTRIGIDSYS_s *irigidsys;
    s8 newvisibilitystate;
    struct NUGCUTLOCATORSYS_s *locatorsys;
    struct instNUGCUTLOCATORSYS_s *ilocatorsys;
    s32 locix;
    // ------------------------------------------
    struct NUGCUTLOCATOR_s *locator;
    struct instNUGCUTLOCATOR_s *iloctemp;
    struct NUGCUTLOCATORTYPE_s* loctype;

    
    cutscene = icutscene->cutscene;
    irigidsys = icutscene->irigids;
    rigidsys = cutscene->rigids;
    
    for(i = 0; i < rigidsys->nrigids; i++)
    {
        rigid = &rigidsys->rigids[i];
        irigid = &irigidsys->irigids[i];

        if ((rigid->flags & 2)) {
            continue;
        }
        
        if ((rigid->flags & 4) == 0) {
            continue;
        }
        
        if ((rigid->visibiltyanim != NULL) &&
        (StateAnimEvaluate(rigid->visibiltyanim,&irigid->visibleframeix, (u8 *)&newvisibilitystate,current_frame) != 0)) {
            if (newvisibilitystate != '\0') {
                ((irigid->special).special)->instance->flags.visible = 1;
            }
            else {
                ((irigid->special).special)->instance->flags.visible = 0;
            }
        }
        
        if ( ((irigid->special).special)->instance->flags.visible != 0) // & 1
        {
            NuGCutRigidCalcMtx(rigid, current_frame, &mtx);
            
            if ((icutscene->has_mtx) != 0) //& 0x10 
            {
                NuMtxMul(&mtx, &mtx, &icutscene->mtx);
            }
            
            (irigid->special.special)->instance->mtx = mtx;

            if (((((irigid->special).special)->instance->flags.visible) != 0) //& 1
            && (rigid->first_locator != 0xff)) {
                locatorsys = cutscene->locators;
                ilocatorsys = icutscene->ilocators;
                
                for (j = 0; j < rigid->nlocators; j++)
                {
                    iloctemp = &ilocatorsys->ilocators[(u32)rigid->first_locator + j];
                    locator = &locatorsys->locators[(u32)rigid->first_locator + j];
                    loctype = &locatorsys->locator_types[locator->locator_type];
                    
                    if ((loctype->flags & 1) != 0) {
                        instNuGCutDebrisLocatorUpdate(locatorsys, iloctemp, locator, current_frame, &mtx);
                    }
                    else {
                        if ((loctype->flags & 2) != 0) {
                            locix = locatorsys->locator_types[locator->locator_type].ix;
                            if (locix != 0xffff) {
                                (locatorfns[locix].fn)(icutscene, locatorsys, iloctemp, locator, current_frame, &mtx);
                            }
                        }
                    }
                }
            }
        }
    }
    return;
}

//PS2
static void instNuGCutRigidSysRender(struct instNUGCUTSCENE_s *icutscene,float current_frame)
{
    s32 i;
    s32 j;
    struct numtx_s mtx;
    struct NUGCUTSCENE_s *cutscene;
    struct NUGCUTRIGIDSYS_s *rigidsys;
    struct instNUGCUTRIGIDSYS_s *irigidsys;
    char newvisibilitystate;
    struct NUGCUTLOCATORSYS_s *locatorsys;
    struct instNUGCUTLOCATORSYS_s *ilocatorsys;
    s32 locix;

    // --------------------------------------
    struct NUGCUTRIGID_s *rigidtemp;
    struct NUGCUTLOCATOR_s *loctemp1;
    struct instNUGCUTLOCATOR_s *iloctemp2;
    struct instNUGCUTRIGID_s *irigidtemp;
    struct NUGCUTLOCATORTYPE_s* loctype;
    s32 local_cc;
    
    cutscene = icutscene->cutscene;
    irigidsys = icutscene->irigids;
    rigidsys = cutscene->rigids;
    
    for(i = 0; i < rigidsys->nrigids; i++)
    {
        rigidtemp = &rigidsys->rigids[i];
        irigidtemp = &irigidsys->irigids[i];

        if ((rigidtemp->flags & 2) == 0)
        {
            continue;
        }
        
        if ((rigidtemp->flags & 4) == 0) {
            continue;
        }
        
        if ((rigidtemp->visibiltyanim != NULL) &&
            (StateAnimEvaluate(rigidtemp->visibiltyanim, &irigidtemp->visibleframeix, &newvisibilitystate, current_frame) != 0)) {
                if (newvisibilitystate != '\0') {
                    ((irigidtemp->special).special)->instance->flags.visible = 1;
                }
                else {
                    ((irigidtemp->special).special)->instance->flags.visible = 0;
                }
            }
        
        if ((((irigidtemp->special).special)->instance->flags.visible))
        {
            NuGCutRigidCalcMtx(rigidtemp, current_frame, &mtx);
            
            if (icutscene->has_mtx != 0)
            {
                NuMtxMul(&mtx, &mtx, &icutscene->mtx);
            }
            
            if (local_cc != 0) {
                NuSpecialDrawAt(irigidtemp, &mtx);
            }
            
            if (NuCutSceneRigidCollisionCheck != NULL) {
                (*NuCutSceneRigidCollisionCheck)(rigidtemp, &mtx);
            }
            
            if (rigidtemp->first_locator != 0xff) 
            {
                locatorsys = cutscene->locators;
                ilocatorsys = icutscene->ilocators;
                
                for (j = 0; j < rigidtemp->nlocators; j++)
                {
                    iloctemp2 = &ilocatorsys->ilocators[(u32)rigidtemp->first_locator + j];
                    loctemp1 = &locatorsys->locators[(u32)rigidtemp->first_locator + j];
                    loctype = &locatorsys->locator_types[loctemp1->locator_type];
                    
                    if ((loctype->flags & 1) != 0) {
                        instNuGCutDebrisLocatorUpdate(locatorsys, iloctemp2, loctemp1, current_frame, &mtx);
                    }
                    else { 
                        if ((loctype->flags & 2) != 0) {
                            locix = loctype->ix;
                            if (locix != 0xffff) {
                                (locatorfns[locix].fn)(icutscene, locatorsys, iloctemp2, loctemp1, current_frame, &mtx);
                            }
                        }
                    }
                }
            }
        }
    }
    return;
}


//PS2
static void NuGCutCharSysFixPtrs(struct NUGCUTSCENE_s* cutscene, s32 address_offset)
{
    struct NUGCUTCHAR_s *cutchar;
    struct NUGCUTCHARSYS_s *charsys;
    s32 i;
    
    if (cutscene->chars != NULL) {
        charsys = (struct NUGCUTCHARSYS_s *)((s32) cutscene->chars + address_offset);
        cutscene->chars = charsys;

        ASSIGN_IF_SET(charsys->chars, (struct NUGCUTCHAR_s* )((s32)charsys->chars + address_offset));
        
        if (charsys->chars != NULL)
        {
            for(i = 0; i < charsys->nchars; i++) {
                cutchar = &charsys->chars[i];
                if (cutchar->name != NULL) {
                    cutchar->name = &cutscene->name_table[(s32)cutchar->name] - 1;
                }
                
                if (cutchar->char_anim != NULL) {
                    cutchar->char_anim = NuAnimData2FixPtrs(cutchar->char_anim, address_offset);
                }
                
                if (cutchar->joint_anim != NULL) {
                    cutchar->joint_anim = NuAnimData2FixPtrs(cutchar->joint_anim, address_offset);
                }
                
                if (cutchar->face_anim != NULL) {
                    cutchar->face_anim = NuAnimData2FixPtrs(cutchar->face_anim, address_offset);
                }
            }
        }
    }
    return;
}

typedef void(*NuCutScnFindCharacters)(struct NUGCUTSCENE_s*);
NuCutScnFindCharacters NuCutSceneFindCharacters;

static void NuGCutCharSysFixUp(struct NUGCUTSCENE_s* cutscene) 
{
  if (NuCutSceneFindCharacters != NULL) {
    (*NuCutSceneFindCharacters)(cutscene);
  }
  return;
}


//PS2
static struct instNUGCUTCHARSYS_s * instNuCGutCharSysCreate(struct NUGCUTSCENE_s *cutscene,union variptr_u *buff)
{
    
    struct NUGCUTCHARSYS_s *chars;
    struct instNUGCUTCHARSYS_s *ichars;
    struct NUGCUTCHAR_s *cutchar;
    struct instNUGCUTCHAR_s *icutchar;
    s32 i;
    
    ichars = NULL;
    chars = cutscene->chars;
    if (((chars != NULL) && (chars->nchars != 0)) && (buff != NULL))
    {
        ichars = buff->voidptr = ALIGN_ADDRESS(buff->voidptr, 0x10);
        buff->voidptr = &ichars[1];
        
        ichars->ichars = NULL;
        
        icutchar = buff->voidptr = ALIGN_ADDRESS(buff->voidptr, 0x10);
        ichars->ichars = icutchar;
        
        buff->voidptr = buff->voidptr + (chars->nchars * sizeof(struct instNUGCUTCHAR_s));
        memset(ichars->ichars, 0, chars->nchars * sizeof(struct instNUGCUTCHAR_s));
        
        for (i = 0; i < chars->nchars; i++)
        {
            cutchar = &chars->chars[i];
            icutchar = &ichars->ichars[i];
            icutchar->current_animix = -1;
            icutchar->prev_animix = -1;
            if (cutchar->flags & 2)
            {
                if (NuCutSceneCharacterCreateData != NULL)
                {
                    (*NuCutSceneCharacterCreateData)(cutchar, icutchar, buff);
                }
            }
            else {
                icutchar->character = cutchar->character;
            }
        }
    }
    
    return ichars;
}

//PS2
void instNuGCutCharSysRender(struct instNUGCUTSCENE_s *icutscene,float current_frame)
{
  struct NUGCUTSCENE_s *cutscene;
  struct instNUGCUTCHARSYS_s *icharsys;
  struct NUGCUTCHARSYS_s *charsys;
  struct instNUGCUTCHAR_s *icutchar;
  struct NUGCUTCHAR_s* cutchar;
  s32 i;
  
  cutscene = icutscene->cutscene;
  icharsys = icutscene->ichars;
  charsys = cutscene->chars;
  if ((NuCutSceneCharacterRender != NULL))
  {
    for(i = 0; i < (s32)charsys->nchars; i++)
    {
      cutchar = &charsys->chars[i];
      icutchar = &icharsys->ichars[i];
      if (icutchar->character != NULL) {
        (*NuCutSceneCharacterRender)
                  (icutscene,cutscene,icutchar,cutchar, current_frame);
      }
    }
  }
  return;
}


//PS2
void NuGCutCharAnimProcess
               (struct NUGCUTCHAR_s *cutchar,float current_frame,struct numtx_s *mtx,s32 *is_visible,
                u32 *animix,float *animrate,float *blendframes)
{
    
    struct nuanimtime_s atime;
    struct NuVec t;
    struct NuVec r;
    struct nuangvec_s rf;
    float fanimix;
    struct nuanimcurve2_s *curves;
    char *curveflags;
    s32 curvesetflags;

    if (cutchar->char_anim != NULL) {
        NuAnimData2CalcTime(cutchar->char_anim, current_frame, &atime);
        curves = cutchar->char_anim->curves;
        curveflags = cutchar->char_anim->curveflags;
        curvesetflags = *cutchar->char_anim->curvesetflags;
        if (cutchar->char_anim->ncurves >= 7) {
            *is_visible = NuAnimCurve2CalcVal(&curves[6], &atime, (int)curveflags[6]);
        }
        else {
            *is_visible = cutchar->flags & 1;
        }
        
        if (animix != 0) {
            if (cutchar->char_anim->ncurves >= 8) {
                fanimix = NuAnimCurve2CalcVal(&curves[7], &atime, (int)curveflags[7]);
                if (fanimix < 0.0f) {
                    *animix = 0xFF;
                } else {
                    *animix = fanimix;
                }
            }
            else {
                *animix = cutchar->animix;
            }
        }
        
        if (*is_visible == 0) {
            return;
        }
        
        if (curvesetflags & 1) {
            r.x = NuAnimCurve2CalcVal(&curves[3], &atime, (int)curveflags[3]);
            r.y = NuAnimCurve2CalcVal(&curves[4], &atime, (int)curveflags[4]);
            r.z = NuAnimCurve2CalcVal(&curves[5], &atime, (int)curveflags[5]);
            rf.z = (s32)(r.z * DEG_TO_FIXED_POINT);
            rf.x = (s32)(r.x * DEG_TO_FIXED_POINT);
            rf.y = (s32)(r.y * DEG_TO_FIXED_POINT);
            NuMtxSetRotateXYZ(mtx, &rf);
        }
        else {
            NuMtxSetIdentity(mtx);
        }
        
        t.x = NuAnimCurve2CalcVal(&curves[0], &atime, (s32)curveflags[0]);
        t.y = NuAnimCurve2CalcVal(&curves[1], &atime, (s32)curveflags[1]);
        t.z = NuAnimCurve2CalcVal(&curves[2], &atime, (s32)curveflags[2]);
        NuMtxTranslate(mtx, &t);
        mtx->_02 = -mtx->_02;
        mtx->_12 = -mtx->_12;
        mtx->_20 = -mtx->_20;
        mtx->_21 = -mtx->_21;
        mtx->_23 = -mtx->_23;
        mtx->_32 = -mtx->_32;
        
        if (animrate != 0) {
            if (cutchar->char_anim->ncurves >= 10)
            {
                *animrate = NuAnimCurve2CalcVal(&curves[9], &atime, (s32)curveflags[9]);
            }
            else {
                *animrate = cutchar->animrate;
            }
        }
        
        if (blendframes == NULL) {
            return;
        }
        
        if (8 < cutchar->char_anim->ncurves) {
            *blendframes = NuAnimCurve2CalcVal(&curves[8], &atime, (s32)curveflags[8]);
            return;
        }
    } 
    else {
        *is_visible = (s32)cutchar->flags & 1;
        if (animix != 0) {
            *animix = cutchar->animix;
        }
        if (is_visible == NULL) {
            return;
        }
        
        *mtx = cutchar->mtx;  
        
        if (animrate != 0) {
            *animrate = cutchar->animrate;
        }
        if (blendframes == NULL) {
            return;
        }
    }
    
    *blendframes = cutchar->blendframes;
    return;
}

//PS2
static void NuGCutTriggerSysFixPtrs(struct NUGCUTSCENE_s *cutscene,s32 address_offset)
{
    struct NUGCUTTRIGGERSYS_s *triggersys;
    struct NUGCUTTRIGGER_s *trigger;
    s32 i;
    
    if (cutscene->triggersys != NULL) {
        triggersys = (struct NUGCUTTRIGGERSYS_s *)((int)cutscene->triggersys + address_offset);
        cutscene->triggersys = triggersys;
        
        ASSIGN_IF_SET(triggersys->triggers, (struct NUGCUTTRIGGER_s *)((s32)triggersys->triggers + address_offset));
        
        if ((triggersys->triggers != NULL))
        {
            for(i = 0; i < triggersys->ntriggers; i++)  {
                trigger = &triggersys->triggers[i];
                
                ASSIGN_IF_SET(trigger->triggername, (s8 *)trigger->triggername + address_offset);
                
                trigger->enableflag1anim = StateAnimFixPtrs(trigger->enableflag1anim, address_offset);
            }
        }
    }
    return;
}

//PS2
static void NuGCutTriggerSysFixUp(struct NUGCUTSCENE_s *cutscene,struct NUTRIGGERSYS_s *triggersys)
{
  s32 i;
  struct NUGCUTTRIGGERSYS_s *cuttriggersys;
  struct NUGCUTTRIGGER_s *cuttrigger;
  s32 j;
  
  cuttriggersys = cutscene->triggersys;
    for (i = 0; i < cuttriggersys->ntriggers; i++){
          cuttrigger = &cuttriggersys->triggers[i];
          for (j = 0; j < triggersys->ntriggers; j++) {
            cuttrigger->ix = -1;
            if (strcasecmp(cuttrigger->triggername,(&triggersys->triggers->triggername)[j * 0xd]) == 0)
            {
              cuttrigger->ix = (short)j;
              break;
            }
          }
          if (cuttrigger->ix == -1) {
            NuErrorProlog("..\\nu2.ps2\\gamelib\\gcutscn.c",0x8a5)
                ("NuGCutTriggerSysFixUp: Unable to find trigger <%s>",cuttrigger->triggername);
          }
    }
    return;
}

//PS2
struct instNUGCUTTRIGGERSYS_s *instNuCGutTriggerSysCreate(struct NUGCUTSCENE_s *cutscene,
struct instNUTRIGGERSYS_s *itriggersys,union variptr_u *buff)
{
  s32 ncuttrigs;
  struct instNUGCUTTRIGGERSYS_s *icuttrigsys;
  
  icuttrigsys = (struct instNUGCUTTRIGGERSYS_s *)(((s32)buff->voidptr + 0xf) & 0xfffffff0);
  ncuttrigs = cutscene->triggersys->ntriggers;
  buff->voidptr = icuttrigsys + 1;
  memset(icuttrigsys,0,8);
  icuttrigsys->itriggersys = itriggersys;
  icuttrigsys->itriggers = (struct instNUGCUTTRIGGER_s *)buff->voidptr;
  buff->voidptr = (void *)((s32)buff->voidptr + ncuttrigs * 4);
  memset(icuttrigsys->itriggers,0,ncuttrigs * 4);
  return icuttrigsys;
}

//PS2
void instNuGCutTriggerSysUpdate(struct instNUGCUTSCENE_s *icutscene,float current_frame)
{
    s32 i;
    struct instNUGCUTTRIGGERSYS_s *icuttrigsys;
    struct NUGCUTTRIGGERSYS_s *cuttrigsys;
    struct NUGCUTTRIGGER_s *cuttrig;
    struct instNUGCUTTRIGGER_s* icuttrig;
    u8 value;
    
    cuttrigsys = icutscene->cutscene->triggersys;
    icuttrigsys = icutscene->itriggersys;
    for (i = 0; i < cuttrigsys->ntriggers; i++)
    {
        icuttrig = icuttrigsys->itriggers;
        cuttrig = &cuttrigsys->triggers[i];
        if (cuttrig->enableflag1anim != NULL) {
            if (StateAnimEvaluate(cuttrig->enableflag1anim, (u8 *)(&icuttrigsys->itriggers[i]), &value, current_frame) != 0) {
                if (value != '\0') {
                    icuttrigsys->itriggersys->itriggers[cuttrig->ix].enableflags = icuttrigsys->itriggersys->itriggers[cuttrig->ix].enableflags | 1;
                }
                else {
                    icuttrigsys->itriggersys->itriggers[cuttrig->ix].enableflags = icuttrigsys->itriggersys->itriggers[cuttrig->ix].enableflags & 0xfe;
                }
            }
        }
    }
    return;
}


//PS2
static void instNuGCutTriggerSysStart(struct instNUGCUTSCENE_s *icutscene)
{
  struct instNUGCUTTRIGGERSYS_s *icuttrigsys;
  struct NUGCUTTRIGGERSYS_s *cuttrigsys;
  struct instNUGCUTTRIGGER_s *icuttrig;
  s32 i;
  
  icuttrigsys = icutscene->itriggersys;
  cuttrigsys = icutscene->cutscene->triggersys;
    for (i = 0; i < cuttrigsys->ntriggers; i++) //while( i < cuttrigsys->ntriggers)
    {
      icuttrig = &icuttrigsys->itriggers[i];
      icuttrig->next_ix = '\0';
    }
  return;
}

//PS2 87%
void CalculateLocatorDirection(enum locdir ldir, struct numtx_s* mtx, struct NuVec* dir) {
    int calc_rots;

    calc_rots = 0;
    switch (ldir) {
    case LOCATOR_NO_DIRECTION:
    case LOCATOR_UP:
        dir->x = 0.0f;
        dir->y = 1.0f; //var_f0
        dir->z = 0.0f;
        break;
    case LOCATOR_DOWN:
        dir->x = 0.0f;
        dir->y = -1.0f; //var_f0
        dir->z = 0.0f;
        break;
    case LOCATOR_X:
        dir->x = -1.0f; //var_f0_2
        dir->y = 0.0f;
        dir->z = 0.0f;
        calc_rots = 1;
        break;
    case LOCATOR_NEGX:
        dir->x = 1.0f; //var_f0_2
        dir->y = 0.0f;
        dir->z = 0.0f;
        calc_rots = 1;
        break;
    case LOCATOR_Y:
        dir->x = 0.0f;
        dir->y = -1.0f;
        dir->z = 0.0f;
        calc_rots = 1;
        break;
    case LOCATOR_NEGY:
        dir->x = 0.0f;
        dir->y = 1.0f;
        dir->z = 0.0f;
        calc_rots = 1;
        break;
    case LOCATOR_Z:
        dir->x = 0.0f;
        dir->y = 0.0f;
        dir->z = 1.0f; //var_f0_3
        calc_rots = 1;
        break;
    case LOCATOR_NEGZ:
        dir->x = 0.0f;
        dir->y = 0.0f;
        dir->z = -1.0f;
        calc_rots = 1;
        break;
    }
    if (calc_rots != 0) {
        NuVecMtxRotate(dir, dir, mtx);
    }
}

//PS2
static int StateAnimEvaluate(struct NUSTATEANIM_s* stateanim, u8* lastix, u8* newstate, float frame)
{
    s32 statechanged;
    u8 ix;
    
    statechanged = 0;
    if (*lastix < stateanim->nchanges) {
        do 
        {
            if (stateanim->frames[*lastix] <= frame) {
                *newstate = stateanim->states[*lastix];
                statechanged = 1;
                *lastix += 1;
            } else {
                break;
            }
        } while (*lastix < stateanim->nchanges);
    }
    
    if (statechanged == 0 && *lastix > 0) {
        do
        {
            if (stateanim->frames[*lastix-1] > frame) {
                *lastix -= 1;
                *newstate = stateanim->states[*lastix];
                statechanged = 1;
            } else {
                break;
            }
        } while (*lastix > 0);
    }
    return statechanged;
}

//PS2
static struct NUSTATEANIM_s * StateAnimFixPtrs(struct NUSTATEANIM_s *sanim, s32 address_offset)
{
    struct NUSTATEANIM_s *rv;

    GET_IF_B_SET(rv, sanim, (s32)&sanim->nchanges + address_offset);
    
    if (rv != NULL) {
        ASSIGN_IF_SET(rv->frames, (s32)rv->frames + address_offset);
        ASSIGN_IF_SET(rv->states, (s32)rv->states + address_offset);
    }
    
    return rv;
}

typedef void(*NuCutScnCharacterRndr)(struct instNUGCUTSCENE_s*, struct NUGCUTSCENE_s*, 
                                    struct instNUGCUTCHAR_s*, struct NUGCUTCHAR_s*, float);
NuCutScnCharacterRndr NuCutSceneCharacterRender;

void NuSetCutSceneCharacterRenderFn(NuCutScnCharacterRndr fn)
{
    NuCutSceneCharacterRender = fn;
    return;
}

typedef void(*NuCutScnFindCharacters)(struct NUGCUTSCENE_s*);
NuCutScnFindCharacters NuCutSceneFindCharacters;

void NuSetCutSceneFindCharactersFn(NuCutScnFindCharacters fn)
{
  NuCutSceneFindCharacters = fn;
  return;
}