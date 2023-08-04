#ifndef NULIGHT_H
#define NULIGHT_H

#include "../types.h"
#include "nu3dxtypes.h"
#include "nu3dx/numtl.h"

/*
  800b14dc 0000cc 800b14dc  4 NuLightInit 	Global
  800b15b8 0000b0 800b15b8  4 NuLightCreate 	Global
  800b1668 000160 800b1668  4 NuLightUpdate 	Global
  800b17c8 000028 800b17c8  4 NuLightGetLight 	Global
  800b17f0 000078 800b17f0  4 NuLightSetAmbientLight 	Global
  800b1868 000218 800b1868  4 NuLightSetDirectionalLights 	Global
  800b1a80 000148 800b1a80  4 NuLightStoreCurrentLights 	Global
  800b1bc8 000200 800b1bc8  4 NuLightSetStoredLights 	Global
  800b1dc8 000010 800b1dc8  4 NuLightClearStoredLights 	Global
  800b1dd8 000084 800b1dd8  4 NuLightFog 	Global
  800b1e5c 000020 800b1e5c  4 NuLightAddSpotXSpanFade 	Global
  800b1e7c 000090 800b1e7c  4 NuLightMatInit 	Global
  800b1f0c 00001c 800b1f0c  4 NuLgtRand 	Global
  800b1f28 000144 800b1f28  4 NuLgtArcLaser 	Global
  800b206c 000a28 800b206c  4 NuLgtArcLaserDraw 	Global
  800b2a94 000018 800b2a94  4 NuLgtSetArcMat 	Global
*/

// If the lights are initialised.
//extern u32 initialised;

// Maximum amount of lights allowed.
extern s32 maxlights;

// Number of lights currently.
extern s32 numlights;

static s32 freelight;

static s32 alloclight;

static s32 current_lights_stored;
static s32 num_stored_lights;

// Size: 0x58
struct nulight_s
{
    struct nucolour3_s ambient;
    struct nucolour3_s diffuse;
    struct numtx_s mtx;
};

// Size: 0x64
struct nusyslight_s
{
    struct nulight_s light;
    s32 index;
    s32 next;
    s32 last;
};

// Size: 0x108
struct nulights_s
{
    struct nulight_s light[3];
};

// Size: 0x40
struct LgtArcLaserData
{
    struct nuvec_s start;
    struct nuvec_s target;
    struct nuvec_s lasdir;
    f32 sizew;
    f32 sizel;
    f32 sizewob;
    f32 arcsize;
    u8 type;
    u8 p1;
    u8 p2;
    u8 p3;
    s32 col;
    s32 seed;
};

struct nusyslight_s light[3];
struct nulights_s StoredLights[1000];
struct nulight_s* currentlight1;
struct nulight_s* currentlight2;
struct nulight_s* currentlight3;
float buglight_distance;
struct numtl_s* NuLightAddMat;
s32 NuLgtSeed;
struct numtl_s* NuLgtArcMtl;
float NuLgtArcU0;
float NuLgtArcV0;
float NuLgtArcU1;
float NuLgtArcV1;
s32 NuLgtArcLaserFrame;
s32 NuLgtArcLaserOldCnt;
s32 NuLgtArcLaserCnt;
struct LgtArcLaserData NuLgtArcLaserData[16];



extern s32 HazeValue;
extern f32 NuRndrFogNear;
extern f32 NuRndrFogFar;
extern u32 NuRndrFogBlur;
extern u32 NuRndrFogCol;

/**********************************************************/
// D3D and GS var
/**********************************************************/

// Size: 0x4
enum _D3DLIGHTTYPE
{
    D3DLIGHT_FORCE_DWORD = 2147483647,
    D3DLIGHT_DIRECTIONAL = 3,
    D3DLIGHT_SPOT = 2,
    D3DLIGHT_POINT = 1
};


struct _D3DVECTOR
{
    f32 x;
    f32 y;
    f32 z;
};


// Size: 0x68
struct _D3DLIGHT8
{
    enum _D3DLIGHTTYPE Type;
    struct _D3DCOLORVALUE Diffuse;
    struct _D3DCOLORVALUE Specular;
    struct _D3DCOLORVALUE Ambient;
    struct _D3DVECTOR Position;
    struct _D3DVECTOR Direction;
    f32 range;
    f32 falloff;
    f32 attenuation0;
    f32 attenuation1;
    f32 attenuation2;
    f32 Theta;
    f32 Phi;
};


// Size: 0x6C
struct _LIGHTLIST
{
    int EnableLight;
    struct _D3DLIGHT8 Light;
};

struct _LIGHTLIST GS_LightList[3];



/**********************************************************/
// END D3D\GS Var
/**********************************************************/


// NuLightInit

// Close the lights.
void NuLightClose();

void GS_SetupFog(int type, float startz, float endz, u32 colour);


// NuLightCreate

// NuLightUpdate

// NuLightGetLight

// NuLightSetAmbientLight

// NuLightSetDirectionalLights

// NuLightStoreCurrentLights

// NuLightSetStoredLights

// NuLightClearStoredLights

void NuLightFog(f32 pnear, f32 pfar, u32 colour, s32 blur, s32 haze);

// NuLightAddSpotXSpanFade

// NuLightMatInit

// NuLgtRand

// NuLgtArcLaser

// NuLgtArcLaserDraw

// NuLgtSetArcMat

#endif // !NULIGHT_H
