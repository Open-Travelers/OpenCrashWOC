#pragma once
#include "system/gs.h"

struct _GS_VERTEXTL* TVertices;
struct _GSMATRIX* CV_SKINMTX;
struct _GS_VERTEXTL* TVertices; //xform.c
struct _GS_VERTEXPSTL* TVertices2;//pointspr.c
struct _GSMATRIX* CV_SKINMTX;
struct _GS_VERTEXSKIN* GS_SkinVertexSource;
struct _GS_VECTOR3* GS_BlendSource;
s32 MaxSkinVerts;
s32 SkinLights;
float c_one;
