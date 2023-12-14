#ifndef PORT_H
#define PORT_H

#include "gs.h"
#include "system/gs.h"

enum shadertypes_e
{
    NUM_SHADER_TYPES = 35,
    DUMMY = 34,
    XRAYGLASS = 33,
    CREATENORMALMAP = 32,
    BLURFILTER = 31,
    APPLYFORCEORVELOCITY = 30,
    CALCNEIGHBOURFORCE2 = 29,
    CALCNEIGHBOURFORCE = 28,
    DYNAMICWATER = 27,
    BLENDSKIN2POINTLIGHT = 26,
    BLENDSKINPOINTLIGHT = 25,
    POINTLIGHT = 24,
    ENVIRONMAP = 23,
    BUMPMAPPOINTLIGHT = 22,
    BUMPMAP = 21,
    BLENDSKINFUR2 = 20,
    BLENDSKINFUR = 19,
    BLENDSKINGLASS2 = 18,
    BLENDSKINGLASS = 17,
    BLENDSKIN2 = 16,
    BLENDSKIN = 15,
    VOLUMESHADOW_BLENDSKIN2 = 14,
    VOLUMESHADOW_BLENDSKIN = 13,
    VOLUMESHADOW = 12,
    LIGHTS = 11,
    BRDFGOLD = 10,
    PROJTEX = 9,
    LIGHTHAZE = 8,
    SPECULAR = 7,
    DEPTHBLEND = 6,
    SNOWCLOUD = 5,
    WATERCAUSTICS = 4,
    GLASS = 3,
    HEATHAZE = 2,
    WATER = 1,
    NO_SHADER = 0,
    RESET_SHADER = -1
};

enum fxidtypes_e
{
    ENVIRONMAPFX = 137,
    FURFX = 136,
    BUMPMAPFX = 135,
    XRAYFX = 100,
    HAZEFX = 99,
    CAUSTICSFX = 98,
    SNOWCLOUDFX = 97,
    DEPTHBLENDFX = 96,
    LIGHTHAZEFX = 95,
    PROJTEXFX = 94,
    BRDFFX = 93,
    DYNAMICWATERFX = 92,
    WATERFX = 4,
    SPECULARGLASSFX = 3,
    SPECULARFX = 2,
    GLASSFX = 1,
    NOFX = 0
};

// Size: 0x8
struct _SHADERTYPE
{
    s32 vertshader; // Offset: 0x0, DWARF: 0xD82019
    s32 pixelShaderHandle; // Offset: 0x4, DWARF: 0xD82046
};

s32 IsLoadingScreen;
struct _SHADERTYPE Shaders[35];
s32 ForceShader;
static float glassmix;
static s32 bypassEffectShaders;
s32 currentLevel;
enum shadertypes_e defaultShader;
static struct numtx_s identmtx;
s32 nobypass;
s32 NEW_CV_COLOR_TYPE;
s32 newCV_LIGHTS_OFF;
struct _GS_VECTOR4 CV_MTL_COLOR;
struct _GS_VECTOR4 CV_AMBIENT_COLOR;
struct _GS_VECTOR4 CV_LIGHT1_COLOR;
struct _GS_VECTOR4 CV_LIGHT1_POS;
struct _GS_VECTOR4 CV_LIGHT2_COLOR;
struct _GS_VECTOR4 CV_LIGHT2_POS;
struct _GS_VECTOR4 CV_LIGHT3_COLOR;
struct _GS_VECTOR4 CV_LIGHT3_POS;


#endif // !PORT_H
