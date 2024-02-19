#include <nucore/numem.h>
#include "nucamera.h"
#include "numath/numtx.h"

static s32 clip_enable = 1;
static struct nuvec_s cam_axes = { 1.0f, 1.0f, 1.0f };


//MATCH NGC
struct nucamera_s * NuCameraCreate(void) {
 struct nucamera_s *cam;

  cam = (struct nucamera_s *)NuMemAlloc(sizeof(struct nucamera_s));
  NuMtxSetIdentity(&cam->mtx);
  cam->nearclip = 0.3f;
  cam->portalnearclip = 0;
  cam->farclip = 1000.0f;
  cam->fov = 0.75f;
  cam->aspect = 0.75f;
  (cam->scale).z = 1.0f;
  (cam->scale).y = 1.0f;
  (cam->scale).x = 1.0f;
  return cam;
}

struct numtx_s* NuCameraGetMtx()
{
    return &global_camera.mtx;
}

struct numtx_s* NuCameraGetViewMtx()
{
    return &vmtx;
}

struct numtx_s* NuCameraGetProjectionMtx()
{
    return &pmtx;
}

struct numtx_s* NuCameraGetVPCSMtx()
{
    return &vpcsmtx;
}

f32 NuCameraDistSqr(struct nuvec_s* point)
{
    return NuVecDistSqr(point, (struct nuvec_s*)&global_camera.mtx._30, NULL);
}

void FixAxes(struct numtx_s* m)
{
    m->_00 *= cam_axes.x;
    m->_01 *= cam_axes.x;
    m->_02 *= cam_axes.x;
    m->_03 *= cam_axes.x;
    m->_10 *= cam_axes.y;
    m->_11 *= cam_axes.y;
    m->_12 *= cam_axes.y;
    m->_13 *= cam_axes.y;
    m->_20 *= cam_axes.z;
    m->_21 *= cam_axes.z;
    m->_22 *= cam_axes.z;
    m->_23 *= cam_axes.z;
    return;
}

void NuCameraEnableClipping(s32 enable)
{
    clip_enable = enable;
}

//MATCH NGC
void NuCameraCalcFrustrumPlanes(void) {
  float f;

  frustrumplanes[2].x = vpmtx._03 - vpmtx._00;
  frustrumplanes[2].y = vpmtx._13 - vpmtx._10;
  frustrumplanes[2].z = vpmtx._23 - vpmtx._20;
  frustrumplanes[2].w = vpmtx._33 - vpmtx._30;
  f = NuFsqrt(frustrumplanes[2].x * frustrumplanes[2].x + frustrumplanes[2].y * frustrumplanes[2].y + frustrumplanes[2].z * frustrumplanes[2].z);
  frustrumplanes[2].x = NuFpDiv(frustrumplanes[2].x,f);
  frustrumplanes[2].y = NuFpDiv(frustrumplanes[2].y,f);
  frustrumplanes[2].z = NuFpDiv(frustrumplanes[2].z,f);
  frustrumplanes[2].w = NuFpDiv(frustrumplanes[2].w,f);
  frustrumplanes[3].x = vpmtx._03 + vpmtx._00;
  frustrumplanes[3].y = vpmtx._13 + vpmtx._10;
  frustrumplanes[3].z = vpmtx._23 + vpmtx._20;
  frustrumplanes[3].w = vpmtx._33 + vpmtx._30;
  f = NuFsqrt(frustrumplanes[3].x * frustrumplanes[3].x + frustrumplanes[3].y * frustrumplanes[3].y + frustrumplanes[3].z * frustrumplanes[3].z);
  frustrumplanes[3].x = NuFpDiv(frustrumplanes[3].x,f);
  frustrumplanes[3].y = NuFpDiv(frustrumplanes[3].y,f);
  frustrumplanes[3].z = NuFpDiv(frustrumplanes[3].z,f);
  frustrumplanes[3].w = NuFpDiv(frustrumplanes[3].w,f);
  frustrumplanes[5].x = vpmtx._03 - vpmtx._01;
  frustrumplanes[5].y = vpmtx._13 - vpmtx._11;
  frustrumplanes[5].z = vpmtx._23 - vpmtx._21;
  frustrumplanes[5].w = vpmtx._33 - vpmtx._31;
  f = NuFsqrt(frustrumplanes[5].x * frustrumplanes[5].x + frustrumplanes[5].y * frustrumplanes[5].y + frustrumplanes[5].z * frustrumplanes[5].z);
  frustrumplanes[5].x = NuFpDiv(frustrumplanes[5].x,f);
  frustrumplanes[5].y = NuFpDiv(frustrumplanes[5].y,f);
  frustrumplanes[5].z = NuFpDiv(frustrumplanes[5].z,f);
  frustrumplanes[5].w = NuFpDiv(frustrumplanes[5].w,f);
  frustrumplanes[4].x = vpmtx._03 + vpmtx._01;
  frustrumplanes[4].y = vpmtx._13 + vpmtx._11;
  frustrumplanes[4].z = vpmtx._23 + vpmtx._21;
  frustrumplanes[4].w = vpmtx._33 + vpmtx._31;
  f = NuFsqrt(frustrumplanes[4].x * frustrumplanes[4].x + frustrumplanes[4].y * frustrumplanes[4].y + frustrumplanes[4].z * frustrumplanes[4].z);
  frustrumplanes[4].x = NuFpDiv(frustrumplanes[4].x,f);
  frustrumplanes[4].y = NuFpDiv(frustrumplanes[4].y,f);
  frustrumplanes[4].z = NuFpDiv(frustrumplanes[4].z,f);
  frustrumplanes[4].w = NuFpDiv(frustrumplanes[4].w,f);
  frustrumplanes[1].x = vpmtx._03 - vpmtx._02;
  frustrumplanes[1].y = vpmtx._13 - vpmtx._12;
  frustrumplanes[1].z = vpmtx._23 - vpmtx._22;
  frustrumplanes[1].w = vpmtx._33 - vpmtx._32;
  f = NuFsqrt(frustrumplanes[1].x * frustrumplanes[1].x + frustrumplanes[1].y * frustrumplanes[1].y + frustrumplanes[1].z * frustrumplanes[1].z);
  frustrumplanes[1].x = NuFpDiv(frustrumplanes[1].x,f);
  frustrumplanes[1].y = NuFpDiv(frustrumplanes[1].y,f);
  frustrumplanes[1].z = NuFpDiv(frustrumplanes[1].z,f);
  frustrumplanes[1].w = NuFpDiv(frustrumplanes[1].w,f);
  frustrumplanes[0].x = vpmtx._03 + vpmtx._02;
  frustrumplanes[0].y = vpmtx._13 + vpmtx._12;
  frustrumplanes[0].z = vpmtx._23 + vpmtx._22;
  frustrumplanes[0].w = vpmtx._33 + vpmtx._32;
  f = NuFsqrt(frustrumplanes[0].x * frustrumplanes[0].x + frustrumplanes[0].y * frustrumplanes[0].y + frustrumplanes[0].z * frustrumplanes[0].z);
  frustrumplanes[0].x = NuFpDiv(frustrumplanes[0].x,f);
  frustrumplanes[0].y = NuFpDiv(frustrumplanes[0].y,f);
  frustrumplanes[0].z = NuFpDiv(frustrumplanes[0].z,f);
  frustrumplanes[0].w = NuFpDiv(frustrumplanes[0].w,f);

  return;
}

//MATCH NGC
void NuCameraSet(struct nucamera_s *camera) {
    struct numtx_s mM;
    struct numtx_s minv;
    struct numtx_s mri;

    global_camera = *camera;
    FixAxes(&global_camera.mtx);
    if (camfx == NUCAMFX_NONE) {
        NuMtxInv(&vmtx,&global_camera.mtx);
        NuMtxScale(&vmtx,&global_camera.scale);
    }
    else {
        NuMtxSetIdentity(&mri);
        mri._11 = -1.0f;
        NuMtxInv(&mM,&global_camera.mtx);
        NuMtxInv(&minv,&global_reflect.mtx);
        vmtx = minv;
        if (camfx == NUCAMFX_REFLECT) {
            NuMtxMul(&vmtx,&vmtx,&mri);
        }
        NuMtxMul(&vmtx,&vmtx,&global_reflect.mtx);
        NuMtxMul(&vmtx,&vmtx,&mM);
    }
    SetProjectionMatrix(&pmtx,global_camera.fov,global_camera.aspect,global_camera.nearclip,global_camera.farclip);
    NuVpGetClippingMtx(&cmtx);
    NuVpGetScalingMtx(&smtx);
    NuMtxMulH(&vpmtx,&vmtx,&pmtx);
    NuMtxMulH(&vpcmtx,&vpmtx,&cmtx);
    NuMtxMulH(&vpcsmtx,&vpcmtx,&smtx);
    NuMtxMulH(&csmtx,&cmtx,&smtx);
    NuMtxInvH(&icsmtx,&csmtx);
    NuMtxInvH(&ivpcsmtx,&vpcsmtx);
    global_reflect.uvmtx = vpcmtx;
    vpc_vport_mtx = vpcmtx;
    zx = (float)tan(global_camera.fov * 0.5f) / global_camera.aspect;
    zy = (float)tan(global_camera.fov * 0.5f);
    GS_SetViewMatrix((struct _GSMATRIX *)&vmtx);
    GS_SetProjectionMatrix((struct _GSMATRIX *)&pmtx);
    NuCameraCalcFrustrumPlanes();
    return;
}

//NGC MATCH
void NuCameraTransformView(struct nuvec_s *dest,struct nuvec_s *src,s32 n, const struct numtx_s *w) {
    struct nuvec_s *end;
    struct numtx_s m;

    end = &src[n];
    if (w != NULL) {
        NuMtxMulH(&m,w,&vmtx);
    }
    else {
        m = vmtx;
    }
    for (; src < end; src++, dest++) {
        NuVecMtxTransformH(dest,src,&m);
    }
    return;
}

//NGC MATCH
void NuCameraTransformClip(struct nuvec_s *dest,struct nuvec_s *src,int n, const struct numtx_s *w) {
  struct nuvec_s *end;
  struct numtx_s m;

  end = &src[n];
  if (w != NULL) {
    NuMtxMulH(&m,w,&vpcmtx);
  }
  else {
      m = vpcmtx;
  }
  for (; src < end; src++, dest++) {
    NuVecMtxTransformH(dest,src,&m);
  }
  return;
}

//MATCH GCN
void NuCameraTransformScreenClip(struct nuvec_s *dest,struct nuvec_s *src,s32 n,struct numtx_s *w) {
    struct nuvec_s *end;
    struct numtx_s m;

    end = &src[n];
    if (w != NULL) {
        NuMtxMulH(&m,w,&vpc_vport_mtx);
    }
    else {
        m = vpc_vport_mtx;
    }
    for (; src < end; src++, dest++) {
        NuVecMtxTransformH(dest,src,&m);
    }
    return;
}


//MATCH GCN
s32 NuCameraClipTestExtents(struct nuvec_s* min, struct nuvec_s* max, const struct numtx_s* wm) {
    struct nuvec_s* ext[2];
    struct nuvec_s src[8];
    struct nuvec_s dest[8];
    s32 k;
    s32 iVar2;
    s32 iVar4;
    s32 iVar6;
    char oc[8];

    if (clip_enable == 0) {
        k = 1;
    } else {
        int j = 0;
        ext[0] = min;
        ext[1] = max;
        for (iVar2 = 0; iVar2 < 2; iVar2++) {
            for (iVar6 = 0; iVar6 < 2; iVar6++) {
                for (iVar4 = 0; iVar4 < 2; iVar4++) {
                    src[j].x = ext[iVar2]->x;
                    src[j].y = ext[iVar6]->y;
                    src[j].z = ext[iVar4]->z;
                    j++;
                }
            }
        }

        NuCameraTransformView(dest, src, 8, wm);
        memset(oc, 0, sizeof(oc));
        for (k = 0; k < 8; k++) {
            if (dest[k].z < global_camera.nearclip) {
                oc[k] = oc[k] | 1;
            }
            if (dest[k].z > global_camera.farclip) {
                oc[k] = oc[k] | 2;
            }
            if (dest[k].x < -dest[k].z * zx) {
                oc[k] = oc[k] | 4;
            }
            if (dest[k].x > dest[k].z * zx) {
                oc[k] = oc[k] | 8;
            }
            if (dest[k].y < -dest[k].z * zy) {
                oc[k] = oc[k] | 0x20;
            }
            if (dest[k].y > dest[k].z * zy) {
                oc[k] = oc[k] | 0x10;
            }
        }
        if ((oc[0] & oc[1] & oc[2] & oc[3] & oc[4] & oc[5] & oc[6] & oc[7]) == 0) {
            k = 1;
            if ((oc[0] | oc[1] | oc[2] | oc[3] | oc[4] | oc[5] | oc[6] | oc[7]) != 0) {
                k = 2;
            }
        } else {
            k = 0;
        }
    }
    return k;
}

//MATCH GCN
s32 NuCameraClipTestBoundingSphere(struct nuvec_s *gobj_centre,float *radius,struct numtx_s *wm) {
    s32 i;
    struct nuvec_s world_centre;
    float distanceFromPlane;

    if (clip_enable != 0) {
        if (wm != NULL) {
            NuVecMtxTransform(&world_centre,gobj_centre,wm);
        }
        else {
            world_centre = *gobj_centre;
        }
        for (i = 0; i < 6; i++) {
            distanceFromPlane = frustrumplanes[i].x * world_centre.x +  frustrumplanes[i].y * world_centre.y + frustrumplanes[i].z * world_centre.z + frustrumplanes[i].w  + *radius;
            if (distanceFromPlane < 0.0f) {
                return 0;
            }
        }
    }
    return 1;
}

//MATCH GCN
s32 NuCameraClipTestPoints(struct nuvec_s* pnts, s32 cnt, struct numtx_s* wm) {
    struct numtx_s m;
    struct nuvec_s v;
    s32 c;
    s32 in;
    s32 out;
    s32 i;

    in = cnt;
    if (wm != NULL) {
        NuMtxMulH(&m, wm, &vmtx);
    } else {
        m = vmtx;
    }
    out = -1;

        for (i = 0; i < in; i++) {
            NuVecMtxTransform(&v, pnts, &m);
            c = 0;
            if (v.z < global_camera.nearclip) {
                c = 1;
            }
            if (v.z > global_camera.farclip) {
                c |= 2;
            }
            if (v.x < (-v.z * zx)) {
                c |= 4;
            }
            if (v.x > (v.z * zx)) {
                c |= 8;
            }
            if (v.y < (-v.z * zy)) {
                c |= 0x20;
            }
            if (v.y > (v.z * zy)) {
                c |= 0x10;
            }
            out &= c;
            pnts++;
        }
    return out;
}

//MATCH GCN  //part of SetProjectionMatrix
float tan2(float x) {
    return cos(x) / sin(x);
}

//MATCH GCN
static void SetProjectionMatrix(struct numtx_s* mtx, float fFOV, float fAspect, float fNearPlane, float fFarPlane) {
    float dVar5;
    float h;
    float w;
    float Q;
    float fVar1;

    // Stack Padding
    u8 x[0x38];

    h = fFarPlane - fNearPlane;

    // Probably an assert macro
    if (NuFabs(h) > 0.01f) {
    } else {
        NuErrorProlog("C:/source/crashwoc/code/nu3dx/nucamera.c",0x2a2, "assert");

    }
    w = fFOV * 0.5f;
    if (NuFabs(sin(w)) > 0.01f) {
    } else {
        NuErrorProlog("C:/source/crashwoc/code/nu3dx/nucamera.c",0x2a3, "assert");
    }
    dVar5 = (fFarPlane / h);
    Q = fAspect * (float)(cos(w) / sin(w));
    fVar1 = (cos(w) / sin(w));
    memset(mtx,0,0x40);
    mtx->_00 = Q;
    mtx->_11 = fVar1;
    mtx->_22 = dVar5;
    mtx->_23 = 1.0f;
    mtx->_32 = (-dVar5 * fNearPlane);
    return;
}
