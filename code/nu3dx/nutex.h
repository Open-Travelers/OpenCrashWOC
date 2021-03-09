#ifndef NUTEX_H
#define NUTEX_H

#include "../types.h"
#include "../nucore.h"
#include "nu3dxtypes.h"

// Texture infos.
NuTex tinfo[0x400];

// If textures are initialized.
u32 initialised;

// Current texture ID in tinfo list.
s32 tpid;

// Number of textures.
u32 ntex;

/*
  800b97e8 000068 800b97e8  4 NuTexClose 	Global
  800b9858 0000d4 800b9858  4 NuTexCreate 	Global
  800b992c 00001c 800b992c  4 NuTexGetDecalInfo 	Global
  800b9b2c 0001cc 800b9b2c  4 NuTexSetTextureStates 	Global
  800b9cf8 00003c 800b9cf8  4 NuTexSetTexture 	Global
  800b9d34 00038c 800b9d34  4 NuTexReadBitmapMM 	Global
*/

// Initialize the texture system.
void NuTexInit();

// Close the textures.
void NuTexClose(); // TODO: IMPLEMENT THIS!!!

// Get the current texture pointer ID.
s32 GetTPID();

// Create a texture from texture data.
NuTex* NuTexCreate(NuTexData* dat); // TODO: IMPLEMENT THIS!!!

// Create a texture from a surface. Returns the texture ID.
s32 NuTexCreateFromSurface(NuTexData* tex, NuSurface* surface);

// Destroy a texture given an ID.
void NuTexDestroy(s32 id);

// Remove a texture reference.
u32 NuTexUnRef(s32 id);

// Get the pixel size for a format.
u32 NuTexPixelSize(u32 format);

// Get the size of an image.
u32 NuTexImgSize(u32 format, u32 width, u32 height);

// Get the texture palette size.
u32 NuTexPalSize(u32 format);

// Read bitmap data into a texture.
s32 NuTexReadBitmapMM(char* fileName, u32 mode, NuTexData* tex); // TODO!!! FINISH!!!

// Read a texture from a BMP.
NuTexData* NuTexReadBitmap(char* fileName);

// This function was never implemented, DO NOT USE!
NuSurface* NuTexLoadTextureFromDDSFile(char* fileName);

#endif // !NUTEX_H