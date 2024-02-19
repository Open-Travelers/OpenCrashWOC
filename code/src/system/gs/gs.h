#pragma once
#include "stdio.h"
#include "nuraster/nurastertypes.h"
#include"system/gxtype.h"

/*
  800c8b14 00002c 800c8b14  4 GS_SetFBCopyTexturePause 	Global
  800c8b40 000040 800c8b40  4 GS_SetAlphaCompareForce 	Global
  800c8b80 000058 800c8b80  4 GS_SetAlphaCompare 	Global
  800c8bd8 000024 800c8bd8  4 GS_ARGBTORGBA 	Global
  800c8bfc 0000e0 800c8bfc  4 GS_BeginScene 	Global
  800c8cdc 000020 800c8cdc  4 GS_EndScene 	Global
  800c8cfc 0000fc 800c8cfc  4 GS_Init 	Global
  800c8df8 000088 800c8df8  4 GS_RenderClear 	Global
  800c8e80 000034 800c8e80  4 GS_RenderClearBlack 	Global
  800c8eb4 000030 800c8eb4  4 GS_SetOrthMatrix 	Global
  800c8ee4 000178 800c8ee4  4 GS_DrawFade 	Global
  800c905c 000254 800c905c  4 GS_FlipScreen 	Global
  800c92b0 00003c 800c92b0  4 GS_SetViewport 	Global
  800c92ec 00003c 800c92ec  4 GS_GetViewport 	Global
  800c9328 000090 800c9328  4 GS_SetupFog 	Global
  800c93b8 000004 800c93b8  4 GS_SetRenderState 	Global
  800c93bc 0000b4 800c93bc  4 GS_SetZCompare 	Global
  800c9470 0000bc 800c9470  4 GS_SetBlendSrc 	Global
  800c952c 000030 800c952c  4 GS_SetBlendSrc2 	Global
  800c955c 0000c0 800c955c  4 GS_SetProjectionMatrix 	Global
  800c961c 000118 800c961c  4 GS_SetLightingMatrix 	Global
  800c9734 000094 800c9734  4 GS_SetLightingMatrix2 	Global
  800c97c8 000078 800c97c8  4 GS_LoadMatrix 	Global
  800c9840 0000c4 800c9840  4 GS_SetViewMatrix 	Global
  800c9904 00003c 800c9904  4 GS_LoadWorldMatrixIdentity 	Global
  800c9940 00012c 800c9940  4 GS_SetWorldMatrix 	Global
  800c9a6c 000004 800c9a6c  4 GS_CopyTextureFromFB 	Global
  800c9a70 00008c 800c9a70  4 GS_CopyFBToPause 	Global
*/

void GS_SetRenderState(int state, int data);
void GS_SetBlendSrc(int enable,int src,int dest);

char DebugText[256];

struct nuviewport_s {
    u32 x;
    u32 y;
    u32 width;
    u32 height;
    float zmin;
    float zmax;
    float centre_x;
    float centre_y;
    float clipmin_x;
    float clipmin_y;
    float clipmax_x;
    float clipmax_y;
    float clip_w;
    float clip_h;
    float scis_w;
    float scis_h;
};



enum gs_texturetype_e
{
    GS_TEX_NUMTEXTURETYPES = 26,
    GS_TEX_RZT16 = 25,
    GS_TEX_RZT24 = 24,
    GS_TEX_RZT32 = 23,
    GS_TEX_RTT16 = 22,
    GS_TEX_RTT24 = 21,
    GS_TEX_RTT32 = 20,
    GS_TEX_RT16 = 19,
    GS_TEX_RT24 = 18,
    GS_TEX_RT32 = 17,
    GS_TEX_RT8H = 16,
    GS_TEX_RTT8 = 15,
    GS_TEX_RT8 = 14,
    GS_TEX_BB = 13,
    GS_TEX_DXT5 = 12,
    GS_TEX_DXT4 = 11,
    GS_TEX_DXT3 = 10,
    GS_TEX_DXT2 = 9,
    GS_TEX_DXT1 = 8,
    GS_TEX_PAL8_S = 7,
    GS_TEX_PAL4_S = 6,
    GS_TEX_PAL8 = 5,
    GS_TEX_PAL4 = 4,
    GS_TEX_RGBA32 = 3,
    GS_TEX_RGB24 = 2,
    GS_TEX_RGBA16 = 1,
    GS_TEX_RGB16 = 0
};

// Size: 0x40, DWARF: 0x8E9296
struct _GSMATRIX
{
    float _11; // Offset: 0x0, DWARF: 0x8E92B4
    float _12; // Offset: 0x4, DWARF: 0x8E92DA
    float _13; // Offset: 0x8, DWARF: 0x8E9300
    float _14; // Offset: 0xC, DWARF: 0x8E9326
    float _21; // Offset: 0x10, DWARF: 0x8E934C
    float _22; // Offset: 0x14, DWARF: 0x8E9372
    float _23; // Offset: 0x18, DWARF: 0x8E9398
    float _24; // Offset: 0x1C, DWARF: 0x8E93BE
    float _31; // Offset: 0x20, DWARF: 0x8E93E4
    float _32; // Offset: 0x24, DWARF: 0x8E940A
    float _33; // Offset: 0x28, DWARF: 0x8E9430
    float _34; // Offset: 0x2C, DWARF: 0x8E9456
    float _41; // Offset: 0x30, DWARF: 0x8E947C
    float _42; // Offset: 0x34, DWARF: 0x8E94A2
    float _43; // Offset: 0x38, DWARF: 0x8E94C8
    float _44; // Offset: 0x3C, DWARF: 0x8E94EE
};

// Size: 0xC, DWARF: 0x8E9114
struct _GS_VECTOR3
{
    float x; // Offset: 0x0, DWARF: 0x8E9134
    float y; // Offset: 0x4, DWARF: 0x8E9158
    float z; // Offset: 0x8, DWARF: 0x8E917C
};

// Size: 0x10, DWARF: 0x8E91C3
struct _GS_VECTOR4
{
    float x; // Offset: 0x0, DWARF: 0x8E91E3
    float y; // Offset: 0x4, DWARF: 0x8E9207
    float z; // Offset: 0x8, DWARF: 0x8E922B
    float w; // Offset: 0xC, DWARF: 0x8E924F
};

struct _GS_VIEWPORT {
    u64 X;
    u64 Y;
    u64 width;
    u64 height;
    float MinZ;
    float MaxZ;
};

struct _GSMATRIX33 {
    float _11;
    float _12;
    float _13;
    float _14;
    float _21;
    float _22;
    float _23;
    float _24;
    float _31;
    float _32;
    float _33;
    float _34;
};

// Size: 0x10, DWARF: 0x1B76
struct _D3DCOLORVALUE
{
    float r; // Offset: 0x0, DWARF: 0x1B99
    float g; // Offset: 0x4, DWARF: 0x1BBD
    float b; // Offset: 0x8, DWARF: 0x1BE1
    float a; // Offset: 0xC, DWARF: 0x1C05
};

// Size: 0x44, DWARF: 0x1A76
struct _D3DMATERIAL8
{
    struct _D3DCOLORVALUE Diffuse; // Offset: 0x0, DWARF: 0x1A98
    struct _D3DCOLORVALUE Ambient; // Offset: 0x10, DWARF: 0x1AC4
    struct _D3DCOLORVALUE Specular; // Offset: 0x20, DWARF: 0x1AF0
    struct _D3DCOLORVALUE Emissive; // Offset: 0x30, DWARF: 0x1B1D
    float Power; // Offset: 0x40, DWARF: 0x1B4A
};


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
    f32 theta;
    f32 phi;
};


// Size: 0x6C
struct _LIGHTLIST
{
    int EnableLight;
    struct _D3DLIGHT8 Light;
};

struct _LIGHTLIST GS_LightList[3];


// Size: 0x4C
struct _GS_TEXTURE
{
    u32 Flags; // Offset: 0x0
    enum gs_texturetype_e Format; // Offset: 0x4
    u32 Width; // Offset: 0x8
    u32 Height; // Offset: 0xC
    u32 TexBits; // Offset: 0x10
    u32 PalBits; // Offset: 0x14
    u32 NUID; // Offset: 0x18
    u32 Pad; // Offset: 0x1C
    struct _GXTexObj Tex; // Offset: 0x20
    struct _GXTlutObj Tlut; // Offset: 0x40
};


// Size: 0x24
struct _GS_VERTEX
{
    float x; // Offset: 0x0
    float y; // Offset: 0x4
    float z; // Offset: 0x8
    float nx; // Offset: 0xC
    float ny; // Offset: 0x10
    float nz; // Offset: 0x14
    u32 diffuse; // Offset: 0x18
    float u; // Offset: 0x1C
    float v; // Offset: 0x20
};

// Size: 0x38, DWARF: 0x8E9B5D
struct _GS_VERTEXSKIN
{
    float x; // Offset: 0x0, DWARF: 0x8E9B80
    float y; // Offset: 0x4, DWARF: 0x8E9BA4
    float z; // Offset: 0x8, DWARF: 0x8E9BC8
    float weights[2]; // Offset: 0xC, DWARF: 0x8E9BEC
    float indexes[3]; // Offset: 0x14, DWARF: 0x8E9C18
    float nx; // Offset: 0x20, DWARF: 0x8E9C44
    float ny; // Offset: 0x24, DWARF: 0x8E9C69
    float nz; // Offset: 0x28, DWARF: 0x8E9C8E
    int diffuse; // Offset: 0x2C, DWARF: 0x8E9CB3
    float u; // Offset: 0x30, DWARF: 0x8E9CDD
    float v; // Offset: 0x34, DWARF: 0x8E9D01
};

// Size: 0x18, DWARF: 0x8E98AB
struct _GS_VERTEXNORM
{
    float x; // Offset: 0x0, DWARF: 0x8E98CE
    float y; // Offset: 0x4, DWARF: 0x8E98F2
    float z; // Offset: 0x8, DWARF: 0x8E9916
    float nx; // Offset: 0xC, DWARF: 0x8E993A
    float ny; // Offset: 0x10, DWARF: 0x8E995F
    float nz; // Offset: 0x14, DWARF: 0x8E9984
};

// Size: 0x1C, DWARF: 0x8E9762
struct _GS_VERTEXTL
{
    float x; // Offset: 0x0, DWARF: 0x8E9783
    float y; // Offset: 0x4, DWARF: 0x8E97A7
    float z; // Offset: 0x8, DWARF: 0x8E97CB
    float rhw; // Offset: 0xC, DWARF: 0x8E97EF
    int diffuse; // Offset: 0x10, DWARF: 0x8E9815
    float u; // Offset: 0x14, DWARF: 0x8E983F
    float v; // Offset: 0x18, DWARF: 0x8E9863
};

// Size: 0x14, DWARF: 0x8E9F47
struct _GS_VERTEXPSTL
{
    float x; // Offset: 0x0, DWARF: 0x8E9F6A
    float y; // Offset: 0x4, DWARF: 0x8E9F8E
    float z; // Offset: 0x8, DWARF: 0x8E9FB2
    float w; // Offset: 0xC, DWARF: 0x8E9FD6
    int diffuse; // Offset: 0x10, DWARF: 0x8E9FFA
};

// Size: 0x8
struct _GS_BUFFER
{
    unsigned int length; // Offset: 0x0, DWARF: 0xD38868
    unsigned int type; // Offset: 0x4, DWARF: 0xD38891
};

// Size: 0x34
struct _GS_FrameBufferCopy
{
    GXTexObj tobj; // Offset: 0x0
    void * data; // Offset: 0x20
    s32 top; // Offset: 0x24
    s32 left; // Offset: 0x28
    s32 width; // Offset: 0x2c
    s32 height; // Offset: 0x30
};

struct _GS_FrameBufferCopy GS_FrameBufferCopyPause;
void* GS_FrameBufferCopydataptr;
u32 GS_FrameBufferCopySize;
static struct _GS_TEXTURE* GS_TexList;
static GXColor GS_BgColour;
struct _GXColor GS_BgColourBlack;
