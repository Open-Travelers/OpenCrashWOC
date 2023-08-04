#ifndef DXFRAME_H
#define DXFRAME_H

#include "../types.h"
#include "nuraster/nurastertypes.h"
//#include <d3d11.h>

/*
  800c1c64 000038 800c1c64  4 ResetFwGlobals__Fv 	Global
  800c1d20 0000bc 800c1d20  4 CreateEnvironment 	Global
  800c1d20 0000bc 800c1d20  4 CreateEnvironment__FP6HWND__ 	Global
  800c1ddc 000034 800c1ddc  4 GetFullscreenBuffers 	Global
  800c1ddc 000034 800c1ddc  4 GetFullscreenBuffers__FP9D3DDevice 	Global
  800c1e10 0000f4 800c1e10  4 CreateDirect3D 	Global
  800c1e10 0000f4 800c1e10  4 CreateDirect3D__FP6HWND__ 	Global
  800c1f04 0000a8 800c1f04  4 InitRenderTargets 	Global
  800c1f04 0000a8 800c1f04  4 InitRenderTargets__Fv 	Global
  800c1fac 000008 800c1fac  4 SetRenderTarget 	Global
  800c1fac 000008 800c1fac  4 SetRenderTarget__FUl 	Global
  800c1fb4 000008 800c1fb4  4 NudxFw_SetRenderTargetSurface 	Global
  800c1fbc 000040 800c1fbc  4 NudxFw_BeginScene 	Global
  800c1ffc 000028 800c1ffc  4 NudxFw_Clear 	Global
  800c2024 000054 800c2024  4 NudxFw_FlipScreen 	Global
  800c2078 000024 800c2078  4 NudxFw_SetRenderState 	Global
  800c209c 000024 800c209c  4 NudxFw_SetTextureState 	Global
  800c20c0 000008 800c20c0  4 NudxFw_GetBackBuffer 	Global
  800c20c8 000008 800c20c8  4 NudxFw_GetZBuffer 	Global
*/

// If the backbuffer has been grabbed this frame.
extern s32 backbuffer_grabbed_this_frame;

// Back buffer TID.
extern s32 backbuffer_tid;

static int g_dwCurrentRT;

// Reset the globals.
void ResetFwGlobals(); // TODO: IMPLEMENT THIS!!!

// Destroy the back buffer copy.
void NudxFw_DestroyBackBufferCopy();

// Make a copy of the backbuffer. Set force to 1 to copy regardless if a backbuffer has been grabbed this frame.
void NudxFw_MakeBackBufferCopy(s32 force);

// Get the back buffer copy TID.
s32 NudxFw_GetBackBufferCopyTID();

// Set if the back buffer has been copied or not.
void NudxFw_SetBackBufferCopied(s32 copied);

// Init graphics, returns 0 if successful, -1 otherwise.
s32 NudxFw_Init();

// Create the graphics environment.
//s32 CreateEnvironment(HWND* hwnd); // TODO: IMPLEMENT THIS!!!
//s32 CreateEnvironment(struct HWND__ *hwnd);

// TODO FUNCTIONS!!!

#endif // !NUFPAR_H
