#include "../system.h"
#include "nu3dx/nu3dxtypes.h"

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define ALIGN4(n) (((n) + 3) & ~3)

int maxblend_cntcnt;
int maxblend_cnt;
int globalix1;
int globalix2;
int gscn_dbg;

void NuRndrInitEx(s32 streambuffersize)
{
  NuRndrInit();
  return;
}

//MATCH GCN
void NuGScnRndr3(struct nugscn_s *scn) {
  s32 iVar3; //temp ?
  s32 cnt;
  struct nuinstance_s *i;
  struct nuspecial_s *sp;
  struct nuinstanim_s *instanim;


  cnt = scn->numinstance;
  i = &scn->instances[cnt];

  while (cnt != 0)
  {
    cnt--;
    i--;
    if ((i->flags.visible & i->flags.visitest))
    {
      if (i->anim != NULL) {
        iVar3 = i->objid;
        instanim = i->anim;
      }
      else {
        iVar3 = i->objid;
        instanim = i;
      }

       i->flags.onscreen = NuRndrGScnObj(scn->gobjs[iVar3],&instanim->mtx);
    }
  }
    cnt = scn->numexspecials;
    while (cnt != 0)
    {
      sp = &scn->exspecials[--cnt];
      if ((sp->flags.ext < 0) && ((sp->flags.ext_vis) != 0)) {
        sp->flags.ext_onscreen = NuRndrGScnObj(scn->gobjs[sp->instance->objid],&sp->mtx);
      }
    }
  return;
}

//MATCH GCN
struct nugspline_s * NuSplineFind(struct nugscn_s *scene,char *name) {
  s32 i;
  struct nugspline_s *sp;


  sp = scene->splines;
    for (i = 0; i < scene->numsplines; i++) {
      if (strcasecmp(name,sp->name) == 0) {
        return sp;
      }
      sp++;
    }
  return NULL;
}

//MATCH GCN
s32 NuSpecialFind(struct nugscn_s *scene,struct nuhspecial_s *special,char *name) {
  s32 i;
  struct nuspecial_s *spec;

  spec = scene->specials;
    for (i = 0; i < scene->numspecial; i++) {
      if (strcasecmp(name,spec->name) == 0) {
        special->scene = scene;
        special->special = spec;
        return 1;
      }
      spec++;
    }
  return 0;
}

void NuRndrRect2di(s32 a,s32 b,s32 SWIDTH,s32 SHEIGHT,s32 fadecol,struct numtl_s *mtl) {
  GS_DrawFade(fadecol);
  return;
}

//MATCH GCN
s32 NuRndrGScnObj(struct nugobj_s *gobj,struct numtx_s *wm) {
  return NuRndrGobj(gobj,wm,NULL);
}

static s32* ps2_scratch_free;	// --> point to PS2_SCRATCH_BASE
unsigned char PS2_SCRATCH_BASE[16384];

//MATCH NGC
void* NuScratchAlloc32(s32 size) {
s32 old;
s32 sizeAligned;
s32 addrAligned;

    old = ps2_scratch_free;
    sizeAligned = ALIGN4(size);
    addrAligned = ALIGN4(ps2_scratch_free);

    ps2_scratch_free = addrAligned + sizeAligned;
    *(s32 *)(addrAligned + sizeAligned) = old;
    ps2_scratch_free += 4;
    return (void *)addrAligned;
}

//MATCH NGC
void NuScratchRelease(void) {
  ps2_scratch_free = ps2_scratch_free[-1];
  return;
}



//NGC MATCH
float ** NuHGobjEvalDwa(int layer,void *bollox,struct nuanimdata_s *vtxanim,float vtxtime) {
    float *dwa;
    struct nuanimcurve_s *animcurve;
    float **array_dwa;
    int tblendcnt;
    int blend_cnt;
    int cnt;
    int i;
    struct nuanimtime_s atimeblendanim;
        char pad [12];

    cnt = layer + -1;
    array_dwa = NULL;
    blend_cnt = 0;
    if (vtxanim != NULL) {
        NuAnimDataCalcTime(vtxanim,vtxtime,&atimeblendanim);
        array_dwa = NuRndrCreateBlendShapeDWAPointers(1);
        if (((atimeblendanim.chunk != -1) && (array_dwa != NULL)) &&
           (vtxanim->chunks[atimeblendanim.chunk] != NULL)) {
            if (vtxanim->chunks[atimeblendanim.chunk]->animcurvesets[cnt] != NULL) {
                dwa = NuRndrCreateBlendShapeDeformerWeightsArray(vtxanim->chunks[atimeblendanim.chunk]->animcurvesets[cnt]->ncurves);
                *array_dwa = dwa;
                if (dwa != NULL) {
                    tblendcnt = vtxanim->chunks[atimeblendanim.chunk]->animcurvesets[cnt]->ncurves;
                        for (i = 0; i < tblendcnt; i++) {
                            dwa[i] = 0.0f;
                            animcurve = vtxanim->chunks[atimeblendanim.chunk]->animcurvesets[cnt]->set[i];
                            if (animcurve != NULL) {
                                blend_cnt++;
                                dwa[i] = NuAnimCurveCalcVal2(animcurve,&atimeblendanim);
                            }
                            else {

                                dwa[i] = vtxanim->chunks[atimeblendanim.chunk]->animcurvesets[cnt]->constants[i];
                                if (dwa[i] > 0.00000001) { //lbl_80121738
                                    blend_cnt++;
                                }

                            }
                            if (dwa[i] < 0.0f) {
                                (dwa[i]) = 0;
                            }
                        }
                    if (maxblend_cnt <= blend_cnt) {
                        maxblend_cnt = blend_cnt;
                        maxblend_cntcnt = tblendcnt;
                    }
                }
            }
        }
    }
    return array_dwa;
}


//NGC MATCH
float ** NuHGobjEvalDwaBlend(s32 nlayers,short *layers,struct nuanimdata_s *vtxanim1,float vtxtime1,
                            struct nuanimdata_s *vtxanim2,float vtxtime2,float blend) {
    float** array_dwa; //
    float* dwa; //
    struct nuanimtime_s atimeblendanim1; //
    struct nuanimtime_s atimeblendanim2; //
    int i; //
    int j; //
    int blend_cnt; //
    float val1; //
    float val2; //
    int ncurves1; //
    int ncurves2; //
    int layer; //
    int size; //
    char pad [18];

    s32 nweights;
    struct nuanimcurveset_s *cv_set;

    array_dwa = NULL;
    if (((vtxanim1 != NULL) || (vtxanim2 != NULL)) && (nlayers != 0) )
    {
        if (vtxanim1 != NULL) {
            NuAnimDataCalcTime(vtxanim1, vtxtime1, &atimeblendanim1);
        }

        if (vtxanim2 != NULL) {
            NuAnimDataCalcTime(vtxanim2, vtxtime2, &atimeblendanim2);
        }

        if (layers == NULL) {
            nlayers = 1;
        }

        array_dwa = NuRndrCreateBlendShapeDWAPointers(nlayers);
        if (array_dwa != NULL) {
            for(i = 0; i < nlayers; i++) {
                if (layers != NULL) {
                    layer = (int)layers[i];
                    if (layer < 0) {
                        continue;
                    }
                }
                else {
                    layer = 0;
                }

                if ((vtxanim1 != NULL) &&
                    (layer < vtxanim1->chunks[atimeblendanim1.chunk]->numnodes) &&
                    (vtxanim1->chunks[atimeblendanim1.chunk]->animcurvesets[layer] != NULL)
                    ) {
                    ncurves1 = vtxanim1->chunks[atimeblendanim1.chunk]->animcurvesets[layer]->ncurves;
                }
                else {
                    ncurves1 = 0;
                }
                if ((vtxanim2 != NULL) &&
                    (layer < vtxanim2->chunks[atimeblendanim2.chunk]->numnodes) &&
                    (vtxanim2->chunks[atimeblendanim2.chunk]->animcurvesets[layer] != NULL)
                    ) {
                    ncurves2 = vtxanim2->chunks[atimeblendanim2.chunk]->animcurvesets[layer]->ncurves;
                }
                else {
                    ncurves2 = 0;
                }

                nweights = MAX(ncurves1, ncurves2);

                dwa = NuRndrCreateBlendShapeDeformerWeightsArray(nweights);
                array_dwa[i] = dwa;

                if (dwa != NULL) {
                    for(j = 0; j < nweights; j++) {
                        if (j < ncurves1) {
                            cv_set = vtxanim1->chunks[atimeblendanim1.chunk]->animcurvesets[layer];
                            if (cv_set->set[j] != NULL) {
                                val1 = NuAnimCurveCalcVal2(cv_set->set[j], &atimeblendanim1);
                            }
                            else {
                                val1 = cv_set->constants[j];
                            }
                        }
                        else {
                            val1 = 0.0f;
                        }

                        if (j < ncurves2) {
                            cv_set = vtxanim2->chunks[atimeblendanim2.chunk]->animcurvesets[layer];
                            if (cv_set->set[j] != NULL) {
                                val2 = NuAnimCurveCalcVal2(cv_set->set[j], &atimeblendanim2);
                            }
                            else {
                                val2 = cv_set->constants[j];
                            }
                        }
                        else {
                            val2 = 0.0f;
                        }

                        dwa[j] = (val2 * blend + (val1 * (1.0f - blend)));
                    }
                }
            }
        }
    }
    return array_dwa;
}


//NGC MATCH
float ** NuHGobjEvalDwa2(int nlayers,short *layers,struct nuanimdata2_s *vtxanim,float vtxtime) {
    float** array_dwa; //
    float* dwa; //
    struct nuanimtime_s atimeblendanim; //
    int i; //
    int j; //
    int blend_cnt; //
    int layer; //
    int size; //
    struct nuanimcurve2_s* curves; //
    char* curveflags; //
    char pad [6];

    array_dwa = NULL;
    if ((vtxanim != NULL) && (nlayers != 0)) {
        NuAnimData2CalcTime(vtxanim, vtxtime, &atimeblendanim);

        if (layers == NULL) {
            nlayers = 1;
        }

        array_dwa = NuRndrCreateBlendShapeDWAPointers(nlayers);
        if (array_dwa != NULL) {
            for(i = 0; i < nlayers; i++) {
                if (layers != NULL) {
                    layer = (int)layers[i];
                    if (layer < 0) {
                        continue;
                    }

                    if (layer >= vtxanim->nnodes) {
                        dwa = NuRndrCreateBlendShapeDeformerWeightsArray(0);
                        array_dwa[i] = dwa;
                        continue;
                    }
                }
                else {
                    layer = 0;
                }

                blend_cnt = vtxanim->ncurves;
                dwa = NuRndrCreateBlendShapeDeformerWeightsArray(blend_cnt);
                array_dwa[i] = dwa;
                curves = &vtxanim->curves[vtxanim->ncurves * layer];
                curveflags = &vtxanim->curveflags[vtxanim->ncurves * layer];
                if ((dwa != NULL)) {
                    for(j = 0; j < blend_cnt; j++) {
                        dwa[j] = NuAnimCurve2CalcVal(&curves[j], &atimeblendanim, curveflags[j]);
                        if (dwa[j] < 0.0f) {
                            dwa[j] = 0.0f;
                        }
                    }
                }
            }
        }
    }

    return array_dwa;
}



union variptr_u vpdmatag_curr;
/*
union variptr_u vpDmaTag_RetEx(union variptr_u ptr)
{
  struct _sceDmaTag dmatag174;
  union variptr_u *in_r4;
  union variptr_u dtag;
  struct _sceDmaTag *next;

  next = dmatag174.next;
  dtag = (variptr_u)in_r4->dmatag;
  vpdmatag_curr = dtag;
  *dtag.u32 = dmatag174._0_4_;
  (dtag.dmatag)->next = next;
  *ptr.u32 = (uint)&(dtag.vec3)->z;
  return (variptr_u)ptr.voidptr;
}

union variptr_u vpDmaTag_Close(union variptr_u ptr)
{
  uint *in_r4;
  uint qwc;

  qwc = *in_r4;
  if (vpdmatag_curr.voidptr == NULL) {
    NuErrorProlog("C:/source/crashwoc/code/nuxbox/dummyfunc.c",0x23a)("Attempted to DmaTag_End without Begin or Add");
  }
  vpdmatag_curr.voidptr = NULL;
  *ptr.u32 = qwc;
  return (union variptr_u)ptr.voidptr;
}

//78% NGC
struct _sceDmaTag * CreateDmaParticleSet(void *buffer,s32 *size) {
    s32 lp;
    union variptr_u buff;
    struct debris_s *temp;

    buff.voidptr = buffer;
    vpDmaTag_RetEx(buff);
    //lp = 0x20;
    temp = (struct debris_s*)buff.voidptr;
    for (lp = 0; lp < 0x20; lp++, temp++){
        temp->x = 1.0f;
        temp->y = 2.0f;
        temp->z = 3.0f;
        temp->mx = 4.0f;
        temp->my = 5.0f;
        temp->mz = 6.0f;
        temp->time = -1.0f;
        temp->etime = 128.0f;
    }
    buff.voidptr = temp;
    *size = (s32)vpDmaTag_Close(buff).voidptr - (s32)buffer;
    return (struct _sceDmaTag *)buffer;
}

//MATCH NGC
struct DmaDebTypePointer * CreateDmaPartEffectList(void *buffer,int *size) {
    union variptr_u buff;
    union variptr_u start;
    struct pDat* PartData;
    struct DmaDebTypePointer* PartStart;
    s32 lp;

    start.voidptr = buffer;
    vpDmaTag_RetEx(start);
    ((struct DmaDebTypePointer *)buffer)->v0 = 72.0f;
    ((struct DmaDebTypePointer *)buffer)->grav = 0.0f;
    ((struct DmaDebTypePointer *)buffer)->u0 = 56.0f;
    ((struct DmaDebTypePointer *)buffer)->u1 = 200.0f;
    ((struct DmaDebTypePointer *)buffer)->v1 = 72.0f;
    PartData = &((struct DmaDebTypePointer *)buffer)->Data[0];
    for (lp = 0; lp < 64; lp++) {
        PartData++;
    }
    buff.voidptr = PartData;
    *size = (int)vpDmaTag_Close(buff).voidptr - (int)buffer;
    return (struct DmaDebTypePointer *)buffer;
}
*/

/*
struct nupad_s * NuPs2OpenPad(s32 port,s32 slot)
{
  struct nupad_s *pad;

  pad = XbGetWorkingController();
  return pad;
}

void NuPs2PadSetMotors(struct nupad_s *pad,s32 motor1,s32 motor2)	//TODO
{
}

UNKTYPE* NuPs2ReadPad (struct nupad_s * pad)	//TODO
{

}

//reverseendian32, reverseendian16 TODO
void InitPadPlayRecord(char *name,s32 mode,s32 size,void *buff)
{
  s32 size_LB;
  u8 *puVar1;
  s32 i;
  struct PadRecInfo *recinfo;

  if (PadRecInfo == (PadRecInfo *)0x0) {
    PadRecInfo = (PadRecInfo *)buff;
  }
  memset(PadRecInfo,0,0x1b658);
  PadDemoEnd = 0;
  if ((name != (char *)0x0) && (mode == 2)) {
    size_LB = NuFileLoadBuffer(name,PadRecInfo,0x1b658);
    if (size_LB == 0) {
      PadDemoEnd = 1;
    }
    else {
      i = 0x10;
      size_LB = 0x13ec;
      reverseendian32(PadRecInfo);
      reverseendian32(&PadRecInfo->padmode);
      reverseendian32(&PadRecInfo->padend);
      reverseendian32(&PadRecInfo->padsize);
      do {
        reverseendian16((short *)((s32)PadRecInfo->recdata + i + -0xc));
        reverseendian16((short *)((s32)PadRecInfo->recdata + i + -2));
        reverseendian16((short *)(&PadRecInfo->recdata[0].ok + i));
        reverseendian16((short *)(&PadRecInfo->recdata[0].buttons_hi + i));
        puVar1 = &PadRecInfo->recdata[0].r_alg_x + i;
        i = i + 0x16;
        reverseendian16((short *)puVar1);
        size_LB = size_LB + -1;
      } while (size_LB != 0);
    }
  }
  recinfo = PadRecInfo;
  PadRecInfo->padpointer = 1;
  recinfo->padmode = mode;
  recinfo->padsize = size + -2;
  return;
}

s32 NuPs2PadDemoEnd(void)

{
  return PadDemoEnd;
}
*/

s32 DeadZoneValue(s32 dx)

{
  if (dx < 1) {
    if (dx < -0x1f) {
      return ((dx + 0x20) * 0xff) / 0xdf;
    }
  }
  else if (0x1f < dx) {
    return ((dx + -0x20) * 0xff) / 0xdf;
  }
  return 0;
}

void NuPs2Init(void)
{
  initd3d();
  return;
}

//PS2
void NuGScnUpdate(float param_1,struct nugscn_s *gsc)
{
    int iVar6;
    int ani_ix;
    struct nuinstance_s *inst_;
    int ninst;
    float unaff_f20;
    struct nuanimtime_s atime;
    struct numtx_s mtx;
    struct nuinstanim_s* anim;
    struct nuanimdata_s* instanim;

    ninst = gsc->numinstance;
    inst_ = &gsc->instances[ninst];
    while (ninst != 0)
    {
        inst_--;
        ninst--;
        if ((gscn_dbg != 0) && (gscn_dbg != ninst)) {
            continue;
        }

        anim = inst_->anim;
        iVar6 = inst_->flags.visitest;
        if (inst_->flags.visible == 0) {
            iVar6 = 0;
        }

        if (iVar6 == 0) {
            continue;
        }

        if (inst_->anim == NULL) {
            continue;
        }

        if (gsc->instanimdata == NULL) {
            continue;
        }

        if (gsc->instanimdata[inst_->anim->anim_ix] == NULL) {
            continue;
        }


        instanim = gsc->instanimdata[inst_->anim->anim_ix];
        if (anim->playing) {
            anim->ltime += param_1 * anim->tfactor;
            if ((anim->waiting) && (anim->tfirst <= anim->ltime)) {
                anim->waiting = 0;
                anim->ltime -=  anim->tfactor - 1.0f;
            }

            if (!anim->waiting) {
                if ((instanim->time + anim->tinterval) <= anim->ltime) {
                    if (anim->repeating) {
                        while ((instanim->time + anim->tinterval) <= anim->ltime)
                        {
                            if (anim->oscillate) {
                                anim->backwards = !anim->backwards;
                            }
                            anim->ltime = (anim->ltime + 1.0f) - (instanim->time + anim->tinterval);
                        }
                        unaff_f20 = anim->ltime;
                    }
                    else {
                        anim->playing = 0;
                        unaff_f20 = anim->ltime = instanim->time;
                    }
                }
                else if (anim->ltime < unaff_f20) {
                    unaff_f20 = anim->ltime;
                }
                else {
                    unaff_f20 = instanim->time;
                }
                if (anim->backwards) {
                    unaff_f20 = (instanim->time + 1.0f) - unaff_f20;
                }
            }
        }
        else {
            unaff_f20 = anim->ltime;
        }

        NuAnimDataCalcTime(instanim, unaff_f20, &atime);
        NuAnimCurveSetApplyToMatrix(*instanim->chunks[atime.chunk]->animcurvesets, &atime, &mtx);
        memcpy(&anim->mtx, &mtx, sizeof(struct numtx_s));
        NuMtxTranslate(&anim->mtx, (struct NuVec *)&inst_->mtx._30);
    }
    return;
}
struct _LARGE_INTEGER_NGC timerfreq;
struct _LARGE_INTEGER_NGC timer_start;
s32 frame_counter;
void NuInitFrameAdvance(void)
{
  //QueryPerformanceFrequency(&timerfreq);
  frame_counter = 0;
  //QueryPerformanceCounter(&timer_start);
  return;
}

//MATCH GCN
s32 NuGetFrameAdvance(void) {
    struct _LARGE_INTEGER_NGC new_time;
    long delta_time;
    s32 advance;
    f32 time_percentage;

    //QueryPerformanceCounter_N1(&new_time);
    delta_time = (s32) (new_time.QuadPart - timer_start.QuadPart);
    time_percentage = ((f32) delta_time / (f32) timerfreq.QuadPart) * 60.0f;
    advance = (s32) (time_percentage - (f32) frame_counter);
    if (advance == 0) {
        advance = 1;
    }
    frame_counter = frame_counter + advance;
    if ((frame_counter > 0x3C) || ( delta_time > timerfreq.QuadPart)) {
        frame_counter = 0;
        //QueryPerformanceCounter_N1(&timer_start);
    }
    if (advance > 6) {
        advance = 6;
    }
    if (advance <= 0) {
        advance = 1;
    }
    return advance;
}


s32 s_223;
s32 t_224;
s32 u_225;

//NGC MATCH
s32 randy(void)
{
  s_223 = s_223 + 1;
  s_223 = rand();
  u_225 = rand();
  t_224 = (s_223 & 0x7fff) << 0x10 | u_225 & 0xffff;
  return t_224;
}

//NGC MATCH
float randyfloat(void)
{
  return rand() * 4.656613f; //4.6566129E-10
}

//NGC MATCH
void NuRndrRectUV2di(s32 x,s32 y,s32 w,s32 h,float tx,float ty,float tw,float th,s32 col,struct numtl_s *mtl) {

    struct nuvtx_tltc1_s vtx [7];

    GS_SetOrthMatrix();

    vtx[0].pnt.x = x;
    vtx[0].pnt.y = y;
    vtx[0].pnt.z = 0.000001f;
    vtx[0].rhw = 1.0f;
    vtx[0].diffuse = col;
    vtx[0].tc[0] = tx;
    vtx[0].tc[1] = ty;

    vtx[1].pnt.x = (x + w) - 1;
    vtx[1].pnt.y = vtx[0].pnt.y;
    vtx[1].pnt.z = 0.000001f;
    vtx[1].rhw = 1.0f;
    vtx[1].diffuse = col;
    vtx[1].tc[0] = tx + tw;
    vtx[1].tc[1] = ty;

    vtx[2].pnt.x = vtx[0].pnt.x;
    vtx[2].pnt.y = (y + h) - 1;
    vtx[2].pnt.z = 0.000001f;
    vtx[2].rhw = 1.0f;
    vtx[2].diffuse = col;
    vtx[2].tc[0] = tx;
    vtx[2].tc[1] = ty + th;

    vtx[3].pnt.x = vtx[1].pnt.x;
    vtx[3].pnt.y = vtx[0].pnt.y;
    vtx[3].pnt.z = 0.000001f;
    vtx[3].rhw = 1.0f;
    vtx[3].diffuse = col;
    vtx[3].tc[0] = vtx[1].tc[0];
    vtx[3].tc[1] = ty;

    vtx[4].pnt.x = vtx[1].pnt.x;
    vtx[4].pnt.y = vtx[2].pnt.y;
    vtx[4].pnt.z = 0.000001f;
    vtx[4].rhw = 1.0f;
    vtx[4].diffuse = col;
    vtx[4].tc[0] = vtx[1].tc[0];
    vtx[4].tc[1] = vtx[2].tc[1];

    vtx[5].pnt.x = vtx[0].pnt.x;
    vtx[5].pnt.y = vtx[2].pnt.y;
    vtx[5].pnt.z = 0.000001f;
    vtx[5].rhw = 1.0f;
    vtx[5].diffuse = col;
    vtx[5].tc[0] = tx;
    vtx[5].tc[1] = vtx[2].tc[1];

    //ResetShaders();
    NuTexSetTexture(0,0);
    NuMtlSetRenderStates(mtl);
    NuTexSetTextureStates(mtl);
    //SetVertexShader(0x144);
    NuTexSetTexture(0, mtl->tid);
    //GS_DrawTriListTTL(vtx, 6);
    return;
}

//NGC MATCH
void NuRndrParticleGroup(struct _sceDmaTag* data, struct PartHeader_testretail *setup,struct numtl_s *mtl,float time,struct numtx_s *wm) {
    s32 instruction; //
    s32 address; //
    s32 qcount; //
    void* retaddress; //
    struct rdata_s* rdat; //
    s32 s; //
    s32 t; //
    struct _sceDmaTag* data2; //
	//char pad[7];

    setup->gtime = time;
    rdat = data;
    instruction = 0;
    for(qcount = 0; qcount < 0x101; qcount++) {
        if (instruction != 0) {
            return;
        }
        retaddress = (void*)rdat->dmadata[0];
        data2 = (struct _sceDmaTag *)rdat->dmadata[1];
        switch (rdat->dmadata[0]) {
            case 0:
                if (data2 != NULL) {
                    //renderpsdma(0x20, rdat, setup, mtl, time, wm);
                    rdat = (struct rdata_s *)data2;
                }
                break;
            case 1:
                //renderpsdma(0x20, rdat, setup, mtl, time, wm);
                instruction = 1;
                break;
            default:
                instruction = 1;
                break;
        }
    }
    return;
}


struct numtx_s debmtx;

//NGC MATCH
void DebMtxTransform(struct nuvec_s *v,struct nuvec_s *v0) {
    v->x = v0->x * debmtx._00 + v0->y * debmtx._10 + v0->z * debmtx._20 + + debmtx._30;
    v->y = v0->x * debmtx._01 + v0->y * debmtx._11 + v0->z * debmtx._21 + debmtx._31;
    v->z = v0->x * debmtx._02 + v0->y * debmtx._12 + v0->z * debmtx._22 + debmtx._32;
    return;
}

/*
DWARF renderpsdma

// Size: 0x410
    struct
    {
        int dmadata[2]; // Offset: 0x0
        int unpackdata[2]; // Offset: 0x8
        // Size: 0x20
        struct
        {
            float x; // Offset: 0x0
            float y; // Offset: 0x4
            float z; // Offset: 0x8
            float time; // Offset: 0xC
            float mx; // Offset: 0x10
            float my; // Offset: 0x14
            float mz; // Offset: 0x18
            float etime; // Offset: 0x1C
        } debris[32]; // Offset: 0x10
    }* rdat; //
    // Size: 0x20
    struct
    {
        float x; // Offset: 0x0
        float y; // Offset: 0x4
        float z; // Offset: 0x8
        float time; // Offset: 0xC
        float mx; // Offset: 0x10
        float my; // Offset: 0x14
        float mz; // Offset: 0x18
        float etime; // Offset: 0x1C
    }* rdeb;
    int s;
    int index;
    // Size: 0x28
    struct
    {
        nuvec_s vt[3]; // Offset: 0x0
        int colour; // Offset: 0x24
    }* pdat;
    // Size: 0x28
    struct
    {
        nuvec_s vt[3]; // Offset: 0x0
        int colour; // Offset: 0x24
    }* hpdat;
    nuvec_s* pdatpt;
    float grav; //
    float elapsed; //
    float r; //
    float g; //
    float b; //
    float a; //
    float u1; //
    float v1; //
    float u2; //
    float v2; //
    nuvec_s pos1; //
    nuvec_s pos2; //
    nuvec_s pos3; //
    nuvec_s pos4; //
    int numverts; //
    int size; //
    int numprims; //

//88% NGC
void* renderpsdma(s32 count, struct rdata_s* rdata, struct PartHeader_testretail* setup, struct numtl_s* mtl, float time, struct numtx_s* wm) {
    s32 uVar1;
    s32 i;
    struct NuVec* pdatpt;
    // struct rdata_s* rdat;
    float elapsed;
    struct NuVec pos1;
    struct NuVec pos2;
    struct NuVec pos3;
    struct NuVec pos4;
    struct uv1deb* rdeb;
    float u1, v1, u2, v2;
    float grav = setup->grav;
    struct PartList_s* pdat;

    grav /= gravdiv;
    rdeb = (struct uv1deb*)&rdata->unpackdata[0];
    ResetShaders();
    GS_SetAlphaCompare(4, 0);
    NuTexSetTexture(0, mtl->tid);
    NuMtlSetRenderStates(mtl);
    NuTexSetTextureStates(mtl);
    if (mtl->attrib.alpha == 1) {
        GS_SetBlendSrc(1, 4, 5);
    } else {
        GS_SetBlendSrc(1, 4, 1);
    }
    GS_SetAlphaCompare(4, 0);
    GS_SetZCompare(1, 0, GX_LEQUAL);
    GS_EnableLighting(0);
    SetVertexShader(0x142);
    GS_LoadWorldMatrixIdentity();
    NuMtxCalcDebrisFaceOn(&debmtx);

    u1 = setup->u0;
    v2 = setup->v0;
    u2 = setup->u1;
    v1 = setup->v1;
    for (i = 0; i < 32; i++) {
        // rdeb = &rdata->debris[i];
        if (rdeb->etime < 0.0f) {
            elapsed = (time - rdeb->time);
            uVar1 = (rdeb->time * elapsed);
            if (uVar1 > 0x3FU) {
                rdeb->etime = -1.0f;
            } else {
                float f11 = grav * elapsed;
                pdat = &setup->Data[uVar1];
                pdatpt = pdat->vt;
                debmtx._30 = (rdeb->x + elapsed * rdeb->mx) + wm->_30;
                debmtx._32 = (rdeb->z + elapsed * rdeb->mz) + wm->_32;
                debmtx._31 = f11 * ((rdeb->my * elapsed + rdeb->y) + wm->_31) + elapsed;
                rdeb++;
                DebMtxTransform(&pos1, &pdatpt[0]);
                GS_SetQuadListRGBA(
                    pdat->colour >> 0x10 & 0xff, pdat->colour >> 8 & 0xff,
                    pdat->colour & 0xff, (pdat->colour >> 0x18) & 0xff
                );
                GS_DrawQuadListBeginBlock(4, 1);
                GS_DrawQuadListSetVert((struct _GS_VECTOR3*)&pos1, u1, v1);
                DebMtxTransform(&pos2, &pdatpt[1]);
                GS_DrawQuadListSetVert((struct _GS_VECTOR3*)&pos2, u1, v2);
                DebMtxTransform(&pos3, &pdatpt[2]);
                GS_DrawQuadListSetVert((struct _GS_VECTOR3*)&pos3, u2, v2);
                pos4.x = (pos2.x - pos3.x) + pos1.x;
                pos4.y = (pos2.y - pos3.y) + pos1.y;
                pos4.z = (pos2.z - pos3.z) + pos1.z;
                GS_DrawQuadListSetVert((struct _GS_VECTOR3*)&pos4, u2, v1);
                GS_DrawQuadListEndBlock();
            }
        }
    }
    GS_EnableLighting(1);
    return rdeb;
}
*/



void GenericDebinfoDmaTypeUpdate(struct debtab *debinfo)
{
	/* DWARF INFO
// Size: 0x28
    struct
    {
        nuvec_s vt[3]; // Offset: 0x0
        int colour; // Offset: 0x24
    }* PartData; //
    int i; //
    int j; //
    float time; //
    float dt; //
    float tt; //
    float vr; //
    float vg; //
    float vb; //
    float va; //
    float vw; //
    float vh; //
    float vrot; //
    float vjx; //
    float vjy; //
    float x0; //
    float y0; //
    float x1; //
    float y1; //
    float dx; //
    float dy; //

	*/
}

//MATCH NGC
s32 NuSpecialDrawAt(struct nuhspecial_s *sph,struct numtx_s *mtx) {
    return NuRndrGScnObj(sph->scene->gobjs[sph->special->instance->objid],mtx);
}

static s32 XboxEffectSystemInitialised;

void InitXboxEffectSystem(s32 Level)
{
  if (XboxEffectSystemInitialised == 0) {
    //InitialiseShaders();
    //NuHazeInit();
    NuWaterInit();
    NuGlassInit();
    XboxEffectSystemInitialised = 1;
  }
  return;
}

void InitXboxGSceneEffects(struct nugscn_s *gsc,union variptr_u *buffer,union variptr_u *buffend)
{
  if (gsc != NULL) {
    NuWaterLoad(gsc);
    NuGlassLoad(gsc,buffer,buffend);
  }
  return;
}

void CloseXboxEffectSystem(void)
{
  if (XboxEffectSystemInitialised != 0) {
    //CloseShaders();
    //NuHazeClose();
    NuWaterClose();
    NuGlassClose();
    XboxEffectSystemInitialised = 0;
  }
  return;
}

void DrawStencilShadowQuad(void) {
  return;
}

//MATCH NGC
s32 NuGobjAverageTextureSpaceVerts(struct nugobj_s *gobj1,struct numtx_s *m1,struct nugobj_s *gobj2,struct numtx_s *m2)
{
  struct nugeom_s *geom1;

    while (gobj1 != NULL) {
        geom1 = gobj1->geom;
        while (geom1 != NULL) {
            geom1 = geom1->next;
        }
        gobj1 = gobj1->next_gobj;
    }
    return 0;
}

//MATCH NGC
void NuSceneAverageTextureSpaceVerts(struct nuscene_s *scene) {
    s32 totalaveraged; //unused?
    s32 ix1;
    s32 numinst;
    s32 ix2;
    struct nugscn_s *gsc;
    struct nugobj_s *gobj1;
    struct nugobj_s *gobj2;
    float dist;
    struct nuvec_s pos1;
    struct nuvec_s pos2;

    gsc = scene->gscene;
    numinst = gsc->numinstance;
        for(ix1 = 0; ix1 < numinst; ix1++) {
                for(ix2 = 0; ix2 < numinst; ix2++) {
                    globalix1 = ix1;
                    gobj1 = gsc->gobjs[gsc->instances[ix1].objid];
                    gobj2 = gsc->gobjs[gsc->instances[ix2].objid];
                    globalix2 = ix2;
                    NuVecMtxTransform(&pos1,&gobj1->bounding_box_center,&gsc->instances[ix1].mtx);
                    NuVecMtxTransform(&pos2,&gobj2->bounding_box_center,&gsc->instances[ix2]);
                    dist = NuVecDist(&pos1,&pos2,NULL);
                    if (dist < ((gobj1->bounding_radius_from_center + gobj2->bounding_radius_from_center) + 0.5f)) {
                        NuGobjAverageTextureSpaceVerts
                                  (gobj1,&gsc->instances[ix1].mtx,gobj2,&gsc->instances[ix2]);
                    }
                }
        }
    return;
}


static unsigned char vibration;
static unsigned char music_volume;
static unsigned char sfx_volume;

/*

void storemenu1(void)

{
  vibration = Game.vibration;
  return;
}

void restoremenu1(void)

{
  Game.vibration = vibration;
  return;
}

void storemenu2(void)

{
  music_volume = Game.music_volume;
  sfx_volume = Game.sfx_volume;
  return;
}

void restoremenu2(void)

{
  Game.sfx_volume = sfx_volume;
  Game.music_volume = music_volume;
  return;
}

*/



