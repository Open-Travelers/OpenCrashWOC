#ifndef NUCAMERA_H
#define NUCAMERA_H

#include "../types.h"
#include "numath.h"
#include "nu3dx/nu3dxtypes.h"

/*
  800ac788 0003b4 800ac788  4 NuCameraCalcFrustrumPlanes 	Global          See https://github.com/microsoft/DirectX-Graphics-Samples/blob/master/MiniEngine/Core/Math/Frustum.h
  800acb3c 0003a4 800acb3c  4 NuCameraSet 	Global
  800acf10 0000e4 800acf10  4 NuCameraTransformView 	Global
  800acff4 0000e4 800acff4  4 NuCameraTransformClip 	Global
  800ad0d8 000274 800ad0d8  4 NuCameraClipTestExtents 	Global
  800ad34c 0000d0 800ad34c  4 NuCameraClipTestBoundingSphere 	Global
  800ad41c 00016c 800ad41c  4 NuCameraClipTestPoints 	Global
  800ad680 00017c 800ad680  4 SetProjectionMatrix 	Global
  800ad804 0000e4 800ad804  4 NuCameraTransformScreenClip 	Global
*/


enum nucamfxmode_s
{
    NUCAMFX_REFLECT = 1,
    NUCAMFX_NONE = 0
};

// Global camera.
extern struct nucamera_s global_camera;

// View matrix.
static struct numtx_s vmtx;

// Projection matrix.
extern struct numtx_s pmtx;

static struct numtx_s vpmtx;

// VPCS matrix.
extern struct numtx_s vpcsmtx;

static struct numtx_s vpcmtx;

static struct numtx_s ivpcsmtx;

// Camera clipping.
static s32 clip_enable;

struct nucamera_s global_camera;
struct numtx_s pmtx;
struct numtx_s vpcsmtx;
static struct numtx_s vpmtx;
static struct nuvec4_s frustrumplanes[6];
static float zx;
static float zy;
static enum nucamfxmode_s camfx;
static struct nureflect_s global_reflect;
static struct nuvec_s cam_axes;

// Camera axes.
extern struct nuvec_s cam_axes;

static struct nuvec4_s frustrumplanes[6];

static struct numtx_s vpc_vport_mtx;

static struct numtx_s cmtx;

static struct numtx_s smtx;

static struct numtx_s csmtx;

static struct numtx_s icsmtx;

static struct numtx_s* glassMtx[256];

static struct numtx_s smtx;

static struct nuvec4_s frustrumplanes[6];


// Create a new camera.
struct nucamera_s* NuCameraCreate();

// NuCameraCalcFrustrumPlanes

// NuCameraSet

// Get the camera matrix.
struct numtx_s* NuCameraGetMtx();

// Get the view matrix.
struct numtx_s* NuCameraGetViewMtx();

// Get the projection matrix.
struct numtx_s* NuCameraGetProjectionMtx();

// Get the VPCS matrix.
struct numtx_s* NuCameraGetVPCSMtx();

// NuCameraTransformView

// NuCameraTransformClip

// NuCameraClipTestExtents

// NuCameraClipTestBoundingSphere

// NuCameraClipTestPoints

// Get the squared distance from the camera to the point.
f32 NuCameraDistSqr(struct nuvec_s* point);

// Fix the axes of a matrix.
void FixAxes(struct numtx_s* m);

static void SetProjectionMatrix(struct numtx_s* mtx, float fFOV, float fAspect, float fNearPlane, float fFarPlane);

// Enable or disable camera clipping.
void NuCameraEnableClipping(s32 enable);

// NuCameraTransformScreenClip

#endif // !NUCAMERA_H
