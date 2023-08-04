#include <nucore/numem.h>
#include "nucamera.h"
#include "numath/numtx.h"

s32 clip_enable = 1;
struct Vec cam_axes = { 1.0f, 1.0f, 1.0f };

struct nucamera_s global_camera;
struct numtx_s* vmtx;
struct numtx_s pmtx;
struct numtx_s vpcsmtx;
static struct numtx_s vpmtx;
static struct Vec4 frustrumplanes[6];
static float zx;
static float zy;
static enum nucamfxmode_s camfx;
static struct nureflect_s global_reflect;

struct nucamera_s * NuCameraCreate(void)
{
  struct nucamera_s *cam = (struct nucamera_s *)NuMemAlloc(sizeof(struct nucamera_s));
  NuMtxSetIdentity(&cam->mtx);
  cam->nearclip = 0.3;
  cam->portalnearclip = 0;
  cam->farclip = 1000.0;
  cam->aspect = 0.75;
  (cam->scale).x = 1.0;
  cam->fov = 0.75;
  (cam->scale).z = 1.0;
  (cam->scale).y = 1.0;
  return cam;
}

struct numtx_s* NuCameraGetMtx()
{
    return &global_camera.mtx;
}

struct numtx_s* NuCameraGetViewMtx()
{
    return vmtx;
}

struct numtx_s* NuCameraGetProjectionMtx()
{
    return &pmtx;
}

struct numtx_s* NuCameraGetVPCSMtx()
{
    return &vpcsmtx;
}

f32 NuCameraDistSqr(struct Vec* point)
{
    return NuVecDistSqr(point, (struct Vec*)&global_camera.mtx._30, NULL);
}

void FixAxes(struct Mtx* m)
{
    m->m11 *= cam_axes.x;
    m->m12 *= cam_axes.x;
    m->m13 *= cam_axes.x;
    m->m14 *= cam_axes.x;
    m->m21 *= cam_axes.y;
    m->m22 *= cam_axes.y;
    m->m23 *= cam_axes.y;
    m->m24 *= cam_axes.y;
    m->m31 *= cam_axes.z;
    m->m32 *= cam_axes.z;
    m->m33 *= cam_axes.z;
    m->m34 *= cam_axes.z;
}

void NuCameraEnableClipping(s32 enable)
{
    clip_enable = enable;
}



void NuCameraCalcFrustrumPlanes(void)	//CHECK

{
  double div;
  float fVar1;

  frustrumplanes[2].y = vpmtx._13 - vpmtx._10;
  frustrumplanes[2].x = vpmtx._03 - vpmtx._00;
  frustrumplanes[2].z = vpmtx._23 - vpmtx._20;
  frustrumplanes[2].w = vpmtx._33 - vpmtx._30;
    fVar1 = NuFsqrt(frustrumplanes[2].z * frustrumplanes[2].z +
                  frustrumplanes[2].x * frustrumplanes[2].x +
                  frustrumplanes[2].y * frustrumplanes[2].y);
  div = (double)fVar1;
  frustrumplanes[2].x = NuFpDiv(frustrumplanes[2].x,fVar1);
  frustrumplanes[2].y = NuFpDiv(frustrumplanes[2].y,(float)div);
  frustrumplanes[2].z = NuFpDiv(frustrumplanes[2].z,(float)div);
  frustrumplanes[2].w = NuFpDiv(frustrumplanes[2].w,(float)div);
  frustrumplanes[3].y = vpmtx._13 + vpmtx._10;
  frustrumplanes[3].x = vpmtx._03 + vpmtx._00;
  frustrumplanes[3].z = vpmtx._23 + vpmtx._20;
  frustrumplanes[3].w = vpmtx._33 + vpmtx._30;
  fVar1 = NuFsqrt(frustrumplanes[3].z * frustrumplanes[3].z +
                  frustrumplanes[3].x * frustrumplanes[3].x +
                  frustrumplanes[3].y * frustrumplanes[3].y);
  div = (double)fVar1;
  frustrumplanes[3].x = NuFpDiv(frustrumplanes[3].x,fVar1);
  frustrumplanes[3].y = NuFpDiv(frustrumplanes[3].y,(float)div);
  frustrumplanes[3].z = NuFpDiv(frustrumplanes[3].z,(float)div);
  frustrumplanes[3].w = NuFpDiv(frustrumplanes[3].w,(float)div);
  frustrumplanes[5].y = vpmtx._13 - vpmtx._11;
  frustrumplanes[5].x = vpmtx._03 - vpmtx._01;
  frustrumplanes[5].z = vpmtx._23 - vpmtx._21;
  frustrumplanes[5].w = vpmtx._33 - vpmtx._31;
  fVar1 = NuFsqrt(frustrumplanes[5].z * frustrumplanes[5].z +
                  frustrumplanes[5].x * frustrumplanes[5].x +
                  frustrumplanes[5].y * frustrumplanes[5].y);
  div = (double)fVar1;
  frustrumplanes[5].x = NuFpDiv(frustrumplanes[5].x,fVar1);
  frustrumplanes[5].y = NuFpDiv(frustrumplanes[5].y,(float)div);
  frustrumplanes[5].z = NuFpDiv(frustrumplanes[5].z,(float)div);
  frustrumplanes[5].w = NuFpDiv(frustrumplanes[5].w,(float)div);
  frustrumplanes[4].y = vpmtx._13 + vpmtx._11;
  frustrumplanes[4].x = vpmtx._03 + vpmtx._01;
  frustrumplanes[4].z = vpmtx._23 + vpmtx._21;
  frustrumplanes[4].w = vpmtx._33 + vpmtx._31;
  fVar1 = NuFsqrt(frustrumplanes[4].z * frustrumplanes[4].z +
                  frustrumplanes[4].x * frustrumplanes[4].x +
                  frustrumplanes[4].y * frustrumplanes[4].y);
  div = (double)fVar1;
  frustrumplanes[4].x = NuFpDiv(frustrumplanes[4].x,fVar1);
  frustrumplanes[4].y = NuFpDiv(frustrumplanes[4].y,(float)div);
  frustrumplanes[4].z = NuFpDiv(frustrumplanes[4].z,(float)div);
  frustrumplanes[4].w = NuFpDiv(frustrumplanes[4].w,(float)div);
  frustrumplanes[1].y = vpmtx._13 - vpmtx._12;
  frustrumplanes[1].x = vpmtx._03 - vpmtx._02;
  frustrumplanes[1].z = vpmtx._23 - vpmtx._22;
  frustrumplanes[1].w = vpmtx._33 - vpmtx._32;
  fVar1 = NuFsqrt(frustrumplanes[1].z * frustrumplanes[1].z +
                  frustrumplanes[1].x * frustrumplanes[1].x +
                  frustrumplanes[1].y * frustrumplanes[1].y);
  div = (double)fVar1;
  frustrumplanes[1].x = NuFpDiv(frustrumplanes[1].x,fVar1);
  frustrumplanes[1].y = NuFpDiv(frustrumplanes[1].y,(float)div);
  frustrumplanes[1].z = NuFpDiv(frustrumplanes[1].z,(float)div);
  frustrumplanes[1].w = NuFpDiv(frustrumplanes[1].w,(float)div);
  frustrumplanes[0].y = vpmtx._13 + vpmtx._12;
  frustrumplanes[0].x = vpmtx._03 + vpmtx._02;
  frustrumplanes[0].z = vpmtx._23 + vpmtx._22;
  frustrumplanes[0].w = vpmtx._33 + vpmtx._32;
  fVar1 = NuFsqrt(frustrumplanes[0].z * frustrumplanes[0].z +
                  frustrumplanes[0].x * frustrumplanes[0].x +
                  frustrumplanes[0].y * frustrumplanes[0].y);
  div = (double)fVar1;
  frustrumplanes[0].x = NuFpDiv(frustrumplanes[0].x,fVar1);
  frustrumplanes[0].y = NuFpDiv(frustrumplanes[0].y,(float)div);
  frustrumplanes[0].z = NuFpDiv(frustrumplanes[0].z,(float)div);
  frustrumplanes[0].w = NuFpDiv(frustrumplanes[0].w,(float)div);
  return;
}

void NuCameraSet(struct nucamera_s* camera) //CHECK
{
    struct numtx_s sp88;
    struct numtx_s sp48;
    struct numtx_s sp8;
    f32 temp_f1;
    f32 temp_r0;
    f32 temp_r0_2;
    f32 temp_r0_3;
    f32 temp_r11;
    s32 cnt;
    struct nucamera_s* cam;
    struct nucamera_s* glob_cam;
    struct numtx_s* temp_r3;
    struct numtx_s* var_r10;
    struct numtx_s* var_r11_3;
    struct numtx_s* var_r29;
    struct numtx_s* var_r8;
    struct numtx_s* vTmp;
    struct numtx_s* var_r28;

    cam = camera;
    cnt = 0x60;
    glob_cam = &global_camera;
    do {
        cnt -= 0x18;
        glob_cam->mtx._00 = cam->mtx._00;
        glob_cam->mtx._01 = cam->mtx._01;
        glob_cam->mtx._02 = cam->mtx._02;
        glob_cam->mtx._03 = cam->mtx._03;
        glob_cam->mtx._10 = cam->mtx._10;
        temp_r0 = cam->mtx._11;
        cam += 0x18;
        glob_cam->mtx._11 = temp_r0;
        glob_cam += 0x18;
    } while (cnt != 0);
    FixAxes(&global_camera.mtx);
    if (camfx == NUCAMFX_NONE) {
        NuMtxInv(&vmtx, &global_camera.mtx);
        NuMtxScale(&vmtx - 0x32BC, &global_camera.scale);
    } else {
        var_r29 = &sp48;
        NuMtxSetIdentity(&sp88);
        sp88._11 = -1.0;
        NuMtxInv(&sp8, &global_camera.mtx);
        NuMtxInv(var_r29, &global_reflect.mtx);
        vTmp = &vmtx;
        cnt = 0x30;
        do {
            cnt -= 0x18;
            vTmp->_00 = var_r29->_00;
            vTmp->_01 = var_r29->_01;
            vTmp->_02 = var_r29->_02;
            vTmp->_03 = var_r29->_03;
            vTmp->_10 = var_r29->_10;
            temp_r11 = var_r29->_11;
            var_r29 += 0x18;
            vTmp->_11 = temp_r11;
            vTmp += 0x18;
        } while (cnt != 0);
        vTmp->_00 = var_r29->_00;
        vTmp->_01 = var_r29->_01;
        vTmp->_02 = var_r29->_02;
        vTmp->_03 = var_r29->_03;
        if ((enum nucamfxmode_s) camfx == NUCAMFX_REFLECT) {
            NuMtxMul(&vmtx, &vmtx, &sp88);
        }
        NuMtxMul(&vmtx, &vmtx, &global_reflect.mtx);
        temp_r3 = &vmtx - 0x32BC;
        NuMtxMul(temp_r3, temp_r3, &sp8);
    }
    SetProjectionMatrix(&pmtx, global_camera.fov, global_camera.aspect, global_camera.nearclip, global_camera.farclip);
    //NuVpGetClippingMtx(&cmtx);
    //NuVpGetScalingMtx(&smtx);
    NuMtxMulH(&vpmtx, &vmtx, &pmtx - 0x327C);
    NuMtxMulH(&vpcmtx,&vpmtx,&cmtx);
    NuMtxMulH(&vpcsmtx,&vpcmtx,&smtx);
    NuMtxMulH(&csmtx,&cmtx,&smtx);
    //NuMtxInvH(&icsmtx, &csmtx);       TODO
    //NuMtxInvH(&ivpcsmtx, &vpcsmtx);   TODO
    var_r28 = &vpcmtx;
    cnt = 0x30;
    var_r10 = &global_reflect.uvmtx;
    do {
        cnt -= 0x18;
        var_r10->_00 = var_r28->_00;
        var_r10->_01 = var_r28->_01;
        var_r10->_02 = var_r28->_02;
        var_r10->_03 = var_r28->_03;
        var_r10->_10 = var_r28->_10;
        temp_r0_2 = var_r28->_11;
        var_r28 += 0x18;
        var_r10->_11 = temp_r0_2;
        var_r10 += 0x18;
    } while (cnt != 0);
    var_r8 = &vpc_vport_mtx;
    var_r10->_00 = var_r28->_00;
    var_r11_3 = &vpcmtx;
    cnt = 0x30;
    var_r10->_01 = var_r28->_00;
    var_r10->_02 = var_r28->_02;
    var_r10->_03 = var_r28->_03;
    do {
        cnt -= 0x18;
        var_r8->_00 = var_r11_3->_00;
        var_r8->_01 = var_r11_3->_01;
        var_r8->_02 = var_r11_3->_02;
        var_r8->_03 = var_r11_3->_03;
        var_r8->_10 = var_r11_3->_10;
        temp_r0_3 = var_r11_3->_11;
        var_r11_3 += 0x18;
        var_r8->_11 = temp_r0_3;
        var_r8 += 0x18;
    } while (cnt != 0);
    var_r8->_00 = var_r11_3->_00;
    var_r8->_01 = var_r11_3->_01;
    var_r8->_02 = var_r11_3->_02;
    var_r8->_03 = var_r11_3->_03;
    zx = (f32) tan(global_camera.fov * 0.5) / global_camera.aspect;
    zy = tan(global_camera.fov * 0.5);
    //GS_SetViewMatrix(&vmtx);
    //GS_SetProjectionMatrix(&pmtx);
    NuCameraCalcFrustrumPlanes();
}

void NuCameraTransformView(struct nuvec_s *dest,struct nuvec_s *src,int n,struct numtx_s *w)	//CHECK

{
  struct nuvec_s *pnVar1;
  struct nuvec_s *pnVar2;
  struct nuvec_s *pfVar3;
  int size;
  struct nuvec_s *pfVar4;
  struct nuvec_s *end;
  struct numtx_s m;

  end = src + n;
  if (w == NULL) {
    size = 0x30;
    pnVar1 = (struct nuvec_s *)&vmtx;
    pnVar2 = (struct nuvec_s *)&m;
    do {
      pfVar4 = pnVar2;
      pfVar3 = pnVar1;
      size = size + -0x18;
      pfVar4->x = pfVar3->x;
      pfVar4->y = pfVar3->y;
      pfVar4->z = pfVar3->z;
      pfVar4[1].x = pfVar3[1].x;
      pfVar4[1].y = pfVar3[1].y;
      pfVar4[1].z = pfVar3[1].z;
      pnVar1 = pfVar3 + 2;
      pnVar2 = pfVar4 + 2;
    } while (size != 0);
    pfVar4[2].x = pfVar3[2].x;
    pfVar4[2].y = pfVar3[2].y;
    pfVar4[2].z = pfVar3[2].z;
    pfVar4[3].x = pfVar3[3].x;
  }
  else {
    NuMtxMulH(&m,w,&vmtx);
  }
  for (; src < end; src = src + 1) {
    NuVecMtxTransformH(dest,src,&m);
    dest = dest + 1;
  }
  return;
}



void NuCameraTransformClip(struct nuvec_s *v,struct nuvec_s *b,int param_3,struct numtx_s *a)	//TODO
{
  float *pfVar1;
  float *pfVar2;
  float *pfVar3;
  int size;
  float *pfVar4;
  struct nuvec_s *pnVar5;
  struct numtx_s dest;

  pnVar5 = b + param_3;
  if (a == NULL) {
    size = 0x30;
    pfVar1 = (float *)&vpcmtx;
    pfVar2 = (float *)&dest;
    do {
      pfVar4 = pfVar2;
      pfVar3 = pfVar1;
      size = size + -0x18;
      *pfVar4 = *pfVar3;
      pfVar4[1] = pfVar3[1];
      pfVar4[2] = pfVar3[2];
      pfVar4[3] = pfVar3[3];
      (*(float (*) [4])(pfVar4 + 4))[0] = (*(float (*) [4])(pfVar3 + 4))[0];
      pfVar4[5] = pfVar3[5];
      pfVar1 = pfVar3 + 6;
      pfVar2 = pfVar4 + 6;
    } while (size != 0);
    pfVar4[6] = pfVar3[6];
    pfVar4[7] = pfVar3[7];
    pfVar4[8] = pfVar3[8];
    pfVar4[9] = pfVar3[9];
  }
  else {
    NuMtxMulH(&dest,a,&vpcmtx);
  }
  for (; b < pnVar5; b = b + 1) {
    NuVecMtxTransformH(v,b,&dest);
    v = v + 1;
  }
  return;
}




void NuCameraTransformScreenClip(struct nuvec_s* dest, struct nuvec_s* src, s32 n, struct numtx_s* w) {
    struct numtx_s m;
    f32 temp_r0;
    s32 cnt;
    struct nuvec_s* end;
    struct numtx_s* mTmp;
    struct numtx_s* vpcTmp;


    if (w != NULL) {
        NuMtxMulH(&m, w, &vpc_vport_mtx);
    } else {
        mTmp = &m;
        vpcTmp = &vpc_vport_mtx;
        cnt = 0x30;
        do {
            cnt -= 0x18;
            mTmp->_00 = vpcTmp->_00;
            mTmp->_01 = vpcTmp->_01;
            mTmp->_02 = vpcTmp->_02;
            mTmp->_03 = vpcTmp->_03;
            mTmp->_10 = vpcTmp->_10;
            temp_r0 = vpcTmp->_11;
            vpcTmp += 0x18;
            mTmp->_11 = temp_r0;
            mTmp += 0x18;
        } while (cnt != 0);
        mTmp->_00 = vpcTmp->_00;
        mTmp->_01 = vpcTmp->_01;
        mTmp->_02 = vpcTmp->_02;
        mTmp->_03 = vpcTmp->_03;
    }
loop_6:
    if (src < &src[n]) {
        end = src;
        src += 0xC;
        NuVecMtxTransformH(dest, end, &m);
        dest += 0xC;
        goto loop_6;
    }
    return;
}

int NuCameraClipTestExtents(struct nuvec_s *min,struct nuvec_s *max,struct numtx_s *wm)		//TODO

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  struct nuvec_s *pnVar5;
  struct nuvec_s **ppnVar6;
  int iVar7;
  int iVar8;
  struct nuvec_s local_f0 [8];
  struct nuvec_s local_90 [8];
  struct nuvec_s *local_30 [2];
  char local_28 [4];
  char local_24;
  char local_23;
  char local_22;
  char local_21;

  if (clip_enable == 0) {
    iVar1 = 1;
  }
  else {
    local_30[0] = min;
    local_30[1] = max;
    iVar4 = 0;
    iVar1 = 0;
    do {
      iVar2 = iVar1 + 1;
      iVar8 = 0;
      do {
        iVar7 = iVar4 * 0xc;
        iVar3 = iVar8 + 1;
        ppnVar6 = local_30;
        do {
          iVar4 = iVar4 + 1;
          *(float *)((int)&local_f0[0].x + iVar7) = local_30[iVar1]->x;
          *(float *)((int)&local_f0[0].y + iVar7) = local_30[iVar8]->y;
          pnVar5 = *ppnVar6;
          ppnVar6 = ppnVar6 + 1;
          *(float *)((int)&local_f0[0].z + iVar7) = pnVar5->z;
          iVar7 = iVar7 + 0xc;
        } while ((int)ppnVar6 <= (int)(local_30 + 1));
        iVar8 = iVar3;
      } while (iVar3 < 2);
      iVar1 = iVar2;
    } while (iVar2 < 2);
    NuCameraTransformView(local_90,local_f0,8,wm);
    memset(local_28,0,0x8);
    iVar8 = 8;
    iVar1 = 0;
    iVar4 = 0;
    do {
      if (*(float *)((int)&local_90[0].z + iVar4) < global_camera.nearclip) {
        local_28[iVar1] = local_28[iVar1] | 1;
      }
      if (global_camera.farclip < *(float *)((int)&local_90[0].z + iVar4)) {
        local_28[iVar1] = local_28[iVar1] | 2;
      }
      if (*(float *)((int)&local_90[0].x + iVar4) < -*(float *)((int)&local_90[0].z + iVar4) * zx)  {
        local_28[iVar1] = local_28[iVar1] | 4;
      }
      if (*(float *)((int)&local_90[0].z + iVar4) * zx < *(float *)((int)&local_90[0].x + iVar4))  {
        local_28[iVar1] = local_28[iVar1] | 8;
      }
      if (*(float *)((int)&local_90[0].y + iVar4) < -*(float *)((int)&local_90[0].z + iVar4) * zy)  {
        local_28[iVar1] = local_28[iVar1] | 0x20;
      }
      if (*(float *)((int)&local_90[0].z + iVar4) * zy < *(float *)((int)&local_90[0].y + iVar4))  {
        local_28[iVar1] = local_28[iVar1] | 0x10;
      }
      iVar4 = iVar4 + 0xc;
      iVar1 = iVar1 + 1;
      iVar8 = iVar8 + -1;
    } while (iVar8 != 0);
    if ((local_21 &
        local_22 & local_23 & local_24 & local_28[3] & local_28[2] & local_28[0] & local_28[1]) ==  0
       ) {
      iVar1 = 1;
      if ((char)(local_21 |
                local_22 |
                local_23 | local_24 | local_28[3] | local_28[2] | local_28[0] | local_28[1]) != 0)  {
        iVar1 = 2;
      }
    }
    else {
      iVar1 = 0;
    }
  }
  return iVar1;
}


int NuCameraClipTestBoundingSphere(struct nuvec_s *gobj_centre,float *radius,struct numtx_s *wm)		//CHECK

{
  struct Vec4 *frp;
  int distanceFromPlane;
  struct nuvec_s world_centre;

  if (clip_enable != 0) {
    if (wm == NULL) {
      world_centre.x = gobj_centre->x;
      world_centre.z = gobj_centre->z;
      world_centre.y = gobj_centre->y;
    }
    else {
      NuVecMtxTransform(&world_centre,gobj_centre,wm);
    }
    distanceFromPlane = 6;
    frp = frustrumplanes;
    do {
      if (frp->z * world_centre.z + frp->x * world_centre.x + frp->y * world_centre.y + frp->w +
          *radius < 0.0) {
        return 0;
      }
      frp = frp + 1;
      distanceFromPlane = distanceFromPlane + -1;
    } while (distanceFromPlane != 0);
  }
  return 1;
}


s32 NuCameraClipTestPoints(struct nuvec_s* pnts, s32 cnt, struct numtx_s* wm) 	//CHECK
{
    struct nuvec_s v;
    struct numtx_s m;
    f32 temp_f10;
    f32 temp_f13;
    f32 temp_r0;
    s32 var_r0;
    s32 var_r10;
    s32 var_r29;
    s32 i;
    struct numtx_s* mTmp;
    struct numtx_s* Vtmp;

    i = cnt;
    if (wm != NULL) {
        NuMtxMulH(&m, wm, &vmtx);
    } else {
        mTmp = &m;
        Vtmp = &vmtx;
        var_r10 = 0x30;
        do {
            var_r10 -= 0x18;
            mTmp->_00 = Vtmp->_00;
            mTmp->_01 = Vtmp->_01;
            mTmp->_02 = Vtmp->_02;
            mTmp->_03 = Vtmp->_03;
            mTmp->_10 = Vtmp->_10;
            temp_r0 = Vtmp->_11;
            Vtmp += 0x18;
            mTmp->_11 = temp_r0;
            mTmp += 0x18;
        } while (var_r10 != 0);
        mTmp->_00 = Vtmp->_00;
        mTmp->_01 = Vtmp->_01;
        mTmp->_02 = Vtmp->_02;
        mTmp->_03 = Vtmp->_03;
    }
    var_r29 = -1;
    if (i > 0) {
        do {
            NuVecMtxTransform(&v, pnts, &m);
            var_r0 = 0;
            if (v.z < global_camera.nearclip) {
                var_r0 = 1;
            }
            if (v.z > global_camera.farclip) {
                var_r0 |= 2;
            }
            temp_f10 = -v.z;
            //temp_f13 = (bitwise f32) v;
            if (temp_f13 < (temp_f10 * zx)) {
                var_r0 |= 4;
            }
            if (temp_f13 > (v.z * zx)) {
                var_r0 |= 8;
            }
            if (v.y < (temp_f10 * zy)) {
                var_r0 |= 0x20;
            }
            if (v.y > (v.z * zy)) {
                var_r0 |= 0x10;
            }
            var_r29 &= var_r0;
            pnts += 0xC;
            i -= 1;
        } while (i != 0);
    }
    return var_r29;
}


void SetProjectionMatrix(struct numtx_s* mtx, f32 fFOV, f32 fAspect, f32 fNearPlane, f32 fFarPlane) {
    f32 temp_f0;
    f32 temp_f27;
    f32 dist;
    f32 temp_f29_2;
    f32 temp_f30_2;
    f32 temp_f31_2;
    f64 temp_f30;
    f64 temp_f31;

    dist = fFarPlane - fNearPlane;
    temp_f27 = 0.01;
    if (!(NuFabs(dist) > temp_f27)) {
        //NuErrorProlog("C:/source/crashwoc/code/nu3dx/nucamera.c", 0x2A2)("assert");
    }
    temp_f0 = fFOV * 0.5;
    if (!(NuFabs((f32) sin(temp_f0)) > temp_f27)) {
        //NuErrorProlog("C:/source/crashwoc/code/nu3dx/nucamera.c", 0x2A3)("assert");
    }
    temp_f29_2 = fFarPlane / dist;
    temp_f31 = cos(temp_f0);
    temp_f31_2 = fAspect * (f32) (temp_f31 / sin(temp_f0));
    temp_f30 = cos(temp_f0);
    temp_f30_2 = (f32) (temp_f30 / sin(temp_f0));
    memset(mtx, 0, 0x40);
    mtx->_32 = -temp_f29_2 * fNearPlane;
    mtx->_00 = temp_f31_2;
    mtx->_11 = temp_f30_2;
    mtx->_23 = 1.0;
    mtx->_22 = temp_f29_2;
}
