#pragma once

/*
  800c9c24 000274 800c9c24  4 GS_SetMaterial 	Global
  800c9e98 00006c 800c9e98  4 GS_SetLight 	Global
  800c9f04 000018 800c9f04  4 GS_LightEnable 	Global
  800c9f1c 000094 800c9f1c  4 GS_XFormLightVec 	Global
  800c9fb0 000124 800c9fb0  4 GS_SetLightingNone 	Global
  800ca0d4 00000c 800ca0d4  4 GS_SetPointLighting 	Global
  800ca0e0 000008 800ca0e0  4 GS_EnableLighting 	Global
  800ca0e8 00001c 800ca0e8  4 GS_SetMaterialSourceAmbient 	Global
  800ca104 000008 800ca104  4 GS_SetMaterialSourceEmissive 	Global
  800ca10c 000008 800ca10c  4 GS_EnableColorVertex 	Global
  800ca114 000008 800ca114  4 GS_EnableSpecular 	Global
  800ca11c 0004f0 800ca11c  4 GS_Set3Lights 	Global
*/

enum _D3DLIGHTTYPE
{
    D3DLIGHT_FORCE_DWORD = 2147483647,
    D3DLIGHT_DIRECTIONAL = 3,
    D3DLIGHT_SPOT = 2,
    D3DLIGHT_POINT = 1
};

// Size: 0x10
struct _D3DCOLORVALUE
{
    float r; // Offset: 0x0
    float g; // Offset: 0x4
    float b; // Offset: 0x8
    float a; // Offset: 0xC
};

// Size: 0xC
struct _D3DVECTOR
{
    float x; // Offset: 0x0
    float y; // Offset: 0x4
    float z; // Offset: 0x8
};

// Size: 0x68
struct _D3DLIGHT8
{
    enum _D3DLIGHTTYPE Type; // Offset: 0x0
    struct _D3DCOLORVALUE Diffuse; // Offset: 0x4
    struct _D3DCOLORVALUE Specular; // Offset: 0x14
    struct _D3DCOLORVALUE Ambient; // Offset: 0x24
    struct _D3DVECTOR Position; // Offset: 0x34
    struct _D3DVECTOR Direction; // Offset: 0x40
    float Range; // Offset: 0x4C
    float Falloff; // Offset: 0x50
    float Attenuation0; // Offset: 0x54
    float Attenuation1; // Offset: 0x58
    float Attenuation2; // Offset: 0x5C
    float Theta; // Offset: 0x60
    float Phi; // Offset: 0x64
};

// Size: 0x6C
struct _LIGHTLIST
{
    s32 EnableLight; // Offset: 0x0
    struct _D3DLIGHT8 Light; // Offset: 0x4
};


static struct _LIGHTLIST GS_LightList[3];