#ifndef NUCAMERA_H
#define NUCAMERA_H

#include "../types.h"
#include "nu3dxtypes.h"
#include "numath.h"

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

// Global camera.
NuCamera global_camera;

// View matrix.
Mtx* vmtx;

// Projection matrix.
Mtx pmtx;

// VPCS matrix.
Mtx vpcsmtx;

// Camera clipping.
u32 clip_enable;

// Camera axes.
Vec cam_axes;

// Create a new camera.
NuCamera* NuCameraCreate();

// NuCameraCalcFrustrumPlanes

// NuCameraSet

// Get the camera matrix.
Mtx* NuCameraGetMtx();

// Get the view matrix.
Mtx* NuCameraGetViewMtx();

// Get the projection matrix.
Mtx* NuCameraGetProjectionMtx();

// Get the VPCS matrix.
Mtx* NuCameraGetVPCSMtx();

// NuCameraTransformView

// NuCameraTransformClip

// NuCameraClipTestExtents

// NuCameraClipTestBoundingSphere

// NuCameraClipTestPoints

// Get the squared distance from the camera to the point.
f32 NuCameraDistSqr(Vec* point);

// Fix the axes of a matrix.
void FixAxes(Mtx* m);

// SetProjectionMatrix

// Enable or disable camera clipping.
void NuCameraEnableClipping(u32 enable);

// NuCameraTransformScreenClip

#endif // !NUCAMERA_H