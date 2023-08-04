#include "../system.h"

void NuRndrInitEx(s32 streambuffersize)
{
  NuRndrInit();
  return;
}

void NuGScnRndr3(struct nugscn_s *scn)	//TODO
{

/*
DWARF

    s32 cnt;
    nuinstance_s* i;
    nuspecial_s* sp;
    nuinstanim_s* instanim; 
*/

}

struct nugspline_s * NuSplineFind(struct nugscn_s *scene,char *name)
{
  s32 cmp;
  s32 i;
  struct nugspline_s *spline;
  
  i = 0;
  spline = scene->splines;
  if (0 < scene->numsplines) {
    do {
      cmp = strcasecmp(name,spline->name);
      if (cmp == 0) {
        return spline;
      }
      i = i + 1;
      spline = spline + 1;
    } while (i < scene->numsplines);
  }
  return NULL;
}

s32 NuSpecialFind(struct nugscn_s *scene,struct nuhspecial_s *special,char *name)
{
  s32 cmp;
  s32 i;
  struct nuspecial_s *spec;
  
  i = 0;
  spec = scene->specials;
  if (0 < scene->numspecial) {
    do {
      cmp = strcasecmp(name,spec->name);
      if (cmp == 0) {
        special->special = spec;
        special->scene = scene;
        return 1;
      }
      i = i + 1;
      spec = spec + 1;
    } while (i < scene->numspecial);
  }
  return 0;
}

s32 NuRndrGScnObj(struct nugobj_s *gobj,struct numtx_s *wm)
{
  s32 ret;
  
  ret = NuRndrGobj(gobj,wm,0);
  return ret;
}

void * NuScratchAlloc32(s32 size)	//TODO
{
	/*
	DWARF

		s32 cnt;
		nuinstance_s* i;
		nuspecial_s* sp;
		nuinstanim_s* instanim; 
	*/
}

static int ps2_scratch_free;	// --> point to PS2_SCRATCH_BASE
unsigned char PS2_SCRATCH_BASE[16384];

void NuScratchRelease(void)
{
  ps2_scratch_free = (int *)ps2_scratch_free[-1];
  return;
}

union variptr_u vpdmatag_curr;

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

struct _sceDmaTag * CreateDmaParticleSet(void *buffer,s32 *size)
{
  s32 lp;
  struct debris_s *temp;
  union variptr_u buff;
  
  vpDmaTag_RetEx((variptr_u)&temp);
  lp = 0x20;
  do {
    temp->x = 1.0f;
    temp->y = 2.0f;
    temp->z = 3.0f;
    temp->mx = 4.0f;
    temp->my = 5.0f;
    temp->mz = 6.0f;
    temp->time = -1.0f;
    temp->etime = 128.0f;
    temp = temp + 1;
    lp = lp + -1;
  } while (lp != 0);
  vpDmaTag_Close((variptr_u)&buff);
  *size = (s32)buff.voidptr - (s32)buffer;
  return (struct _sceDmaTag *)buffer;
}

DmaDebTypePointer * CreateDmaPartEffectList(void *buffer,int *size)		//TODO
{
	/*
	DWARF
	
	variptr_u buff;
    variptr_u start; // 
    // Size: 0x28
    struct
    {
        nuvec_s vt[3]; // Offset: 0x0
        int colour; // Offset: 0x24
    }* PartData;
    // Size: 0xA48
    struct
    {
        s32 DmaHeader[4]; // Offset: 0x0
        float grav; // Offset: 0x10
        float gtime; // Offset: 0x14
        s32 DmaBody[4]; // Offset: 0x18
        float u0; // Offset: 0x28
        float v0; // Offset: 0x2C
        float u1; // Offset: 0x30
        float v1; // Offset: 0x34
        float u2; // Offset: 0x38
        float v2; // Offset: 0x3C
        float u3; // Offset: 0x40
        float v3; // Offset: 0x44
        // Size: 0x28
        struct
        {
            nuvec_s vt[3]; // Offset: 0x0
            s32 colour; // Offset: 0x24
        } Data[64]; // Offset: 0x48
    }* PartStart; // 
    s32 lp; // 
	
	*/
}
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

/*
void NuGScnUpdate (float unkparam, struct nugscn_s * gscn)	//TODO
{

}
*/

s32 frame_counter;
void NuInitFrameAdvance(void)
{
  QueryPerformanceFrequency(&timerfreq);
  frame_counter = 0;
  QueryPerformanceCounter(&timer_start);
  return;
}


s32 NuGetFrameAdvance(void)		//TODO
{

/* DWARF INFO

    s32 advance;
    struct _LARGE_INTEGER new_time;
    long delta_time;
    float time_percentage;

// Size: 0x8
struct _LARGE_INTEGER
{
    double QuadPart;
};

*/
/*
  s32 iVar1;
  double dVar2;
  double local_20 [2];
  double local_10;
  
  QueryPerformanceCounter(local_20);
  local_10 = (double)CONCAT44(0x43300000,(s32)(local_20[0] - timer_start) ^ 0x80000000);
  dVar2 = local_10 - 4503601774854144.0;
  local_10 = (double)CONCAT44(0x43300000,frame_counter ^ 0x80000000);
  iVar1 = (s32)(((float)dVar2 / (float)timerfreq) * 60.0 - (float)(local_10 - 4503601774854144.0) );
  local_10 = (double)(longlong)iVar1;
  if (iVar1 == 0) {
    iVar1 = 1;
  }
  frame_counter = frame_counter + iVar1;
  if ((0x3c < frame_counter) || (timerfreq < dVar2)) {
    frame_counter = 0;
    QueryPerformanceCounter(&timer_start);
  }
  if (6 < iVar1) {
    iVar1 = 6;
  }
  if (iVar1 < 1) {
    iVar1 = 1;
  }
  return iVar1;
  */
}


s32 randy(void)
{
s32 s;
s32 t;
s32 u;
  s = s + 1;
  s = rand();
  u = rand();
  t = (s & 0x7fff) << 0x10 | u & 0xffff;
  return t;
}

/*float randyfloat(void)	//TODO
{
  s32 uVar1;
  
  uVar1 = rand();
  return (float)((double)CONCAT44(0x43300000,uVar1 ^ 0x80000000) - 4503601774854144.0) *
         4.656613e-10;
}*/

void NuRndrRect2di(s32 a,s32 b,s32 SWIDTH,s32 SHEIGHT,s32 fadecol,struct numtl_s *mtl)
{
  GS_DrawFade(fadecol);
  return;
}

void NuRndrParticleGroup(rdata_s *data,setup_s *setup,numtl_s *mtl,float time,numtx_s *wm)
{
  s32 i;
  struct _sceDmaTag *data2;
  //double dVar1;
  bool check;
  
  //dVar1 = (double)time;
  setup->DmaHeader[3] = (s32)time;
  check = false;
  i = 0;
  do {
    if (check) {
      return;
    }
    data2 = (struct _sceDmaTag *)data->dmadata[1];
    if (data->dmadata[0] == 0) {
      if (data2 != NULL) {
        renderpsdma(0x20,data,setup,mtl,time,wm);
        data = (struct rdata_s *)data2;
      }
    }
    else {
      if (data->dmadata[0] == 1) {
        renderpsdma(0x20,data,setup,mtl,time,wm);
      }
      check = true;
    }
    i = i + 1;
  } while (i < 0x101);
  return;
}


struct numtx_s debmtx;

void DebMtxTransform(struct nuvec_s *v,struct nuvec_s *v0)
{
  v->x = v0->z * debmtx._20 + v0->x * debmtx._00 + v0->y * debmtx._10 + debmtx._30;
  v->y = v0->z * debmtx._21 + v0->x * debmtx._01 + v0->y * debmtx._11 + debmtx._31;
  v->z = v0->z * debmtx._22 + v0->x * debmtx._02 + v0->y * debmtx._12 + debmtx._32;
  return;
}


void * renderpsdma (int count, rdata_s * rdata, setup_s * setup, numtl_s * mtl, float time, numtx_s * wm)	//TODO
{

/*
DWARF

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

*/

}


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

s32 NuSpecialDrawAt(struct nuhspecial_s *sph,struct numtx_s *mtx)
{
  s32 rndr;
  
  rndr = NuRndrGScnObj(sph->scene->gobjs[sph->special->instance->objid],mtx);
  return rndr;
}

static s32 XboxEffectSystemInitialised;

void InitXboxEffectSystem(s32 Level)
{
  if (XboxEffectSystemInitialised == 0) {
    InitialiseShaders();
    NuHazeInit();
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
    CloseShaders();
    NuHazeClose();
    NuWaterClose();
    NuGlassClose();
    XboxEffectSystemInitialised = 0;
  }
  return;
}

void DrawStencilShadowQuad(void)

{
  return;
}


s32 NuGobjAverageTextureSpaceVerts(struct nugobj_s *gobj1,struct numtx_s *m1,struct nugobj_s *gobj2,struct numtx_s *m2)
{
/* DWARF INFO

    s32 averaged; // 
    nugeom_s* geom1; //
    nugeom_s* geom2; // 
    nugobj_s* g2;
*/
  s32 averaged;
  struct nugeom_s *geom1;
  
  do {
    if (gobj1 == NULL) {
      return 0;
    }
    geom1 = gobj1->geom;
    gobj1 = gobj1->next_gobj;
  } while (geom1 == NULL);
  do {
    geom1 = geom1->next;
  } while (geom1 != NULL);
  averaged = NuGobjAverageTextureSpaceVerts(gobj1,m1,gobj2,m2);
  return averaged;
}

void NuSceneAverageTextureSpaceVerts(struct nuscene_s *scene)	//TODO
{
/* DWARF INFO
    nugscn_s* gsc; // 
    s32 numinst; // 
    s32 ix1; // 
    s32 ix2; // 
    nugobj_s* gobj1; // 
    nugobj_s* gobj2; // 
    nuvec_s pos1; //
    nuvec_s pos2; //
    float dist; //
    s32 totalaveraged;
*/
	
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



