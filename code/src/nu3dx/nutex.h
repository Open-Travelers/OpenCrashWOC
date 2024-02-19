#ifndef NUTEX_H
#define NUTEX_H

#include "../types.h"
#include "../nucore.h"
#include "nu3dxtypes.h"
#include "nuraster/nurastertypes.h"

// Texture infos.
extern struct nusystex_s tinfo[0x400];

// If textures are initialized.
//s32 initialised;

// Current texture ID in tinfo list.
static s32 tpid;

// Number of textures.
static s32 tpid;

/*
  800b97e8 000068 800b97e8  4 NuTexClose 	Global
  800b9858 0000d4 800b9858  4 NuTexCreate 	Global
  800b992c 00001c 800b992c  4 NuTexGetDecalInfo 	Global
  800b9b2c 0001cc 800b9b2c  4 NuTexSetTextureStates 	Global
  800b9cf8 00003c 800b9cf8  4 NuTexSetTexture 	Global
  800b9d34 00038c 800b9d34  4 NuTexReadBitmapMM 	Global
*/


struct tagBITMAPFILEHEADER {
    u16 pad;
    u16 bfType;
    u32 bfSize;
    u16 bfReserved1;
    u16 bfReserved2;
    u32 bfOffBits;
};

struct tagBITMAPINFOHEADER {
    u32 biSize;
    u32 biWidth;
    u32 biHeight;
    u16 biPlanes;
    u16 biBitCount;
    u32 biCompression;
    u32 biSizeImage;
    u32 biXPelsPerMeter;
    u32 biYPelsPerMeter;
    u32 biClrUsed;
    u32 biClrImportant;
};

// Initialize the texture system.
void NuTexInit();

// Close the textures.
void NuTexClose(); // TODO: IMPLEMENT THIS!!!

// Get the current texture pointer ID.
s32 GetTPID();

// Create a texture from texture data.
s32 NuTexCreate(struct nutex_s *nutex); // TODO: IMPLEMENT THIS!!!

// Create a texture from a surface. Returns the texture ID.
s32 NuTexCreateFromSurface(struct nutex_s *tex, struct D3DTexture *surface);

// Destroy a texture given an ID.
void NuTexDestroy(s32 id);

// Remove a texture reference.
u32 NuTexUnRef(s32 id);

// Get the pixel size for a format.
s32 NuTexPixelSize(enum nutextype_e type);

// Get the size of an image.
s32 NuTexImgSize(enum nutextype_e type, s32 width, s32 height);

// Get the texture palette size.
s32 NuTexPalSize(enum nutextype_e type);

// Read bitmap data into a texture.
s32 NuTexReadBitmapMM(char* fileName, s32 mmlevel, struct nutex_s* tex); // TODO!!! FINISH!!!

// Read a texture from a BMP.
struct nutex_s * NuTexReadBitmap(char* fileName);

// This function was never implemented, DO NOT USE!
struct D3DTexture* NuTexLoadTextureFromDDSFile(char* fileName);

#endif // !NUTEX_H
