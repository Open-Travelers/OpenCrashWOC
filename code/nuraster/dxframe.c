// This file is original in C++, but I'll try and make it in C for consistency.
#include "dxframe.h"

// If the backbuffer has been grabbed this frame.
u32 backbuffer_grabbed_this_frame = 0;

// Back buffer TID.
u32 backbuffer_tid = 0x207F;

void ResetFwGlobals()
{
	//g_pddsZBuffer = (void*)0x0;
	//g_dwRenderWidth = 0;
	//g_dwRenderHeight = 0;
	//g_pd3dDevice = 0;
	//g_pddsBackBuffer = (void*)0x0;
	//InitRenderTargets();
}

void NudxFw_DestroyBackBufferCopy()
{
}

void NudxFw_MakeBackBufferCopy(u32 force)
{
	if (!backbuffer_grabbed_this_frame || force)
	{
		//GS_CopyTextureFromFB(); This is a stub function. Is it supposed to do something?
		NudxFw_SetBackBufferCopied(1);
	}
}

u32 NudxFw_GetBackBufferCopyTID()
{
	return backbuffer_tid;
}

void NudxFw_SetBackBufferCopied(u32 grabbed)
{
	backbuffer_grabbed_this_frame = grabbed;
}

s32 NudxFw_Init()
{
	ResetFwGlobals();
	s32 code = CreateEnvironment(NULL);
	if (code > -1)
	{
		code = 0;
	}
	return code;
}

//s32 CreateEnvironment(HWND* hwnd)
//{
//	// TODO!!!
//}