#ifndef NU3DXTYPES_H
#define NU3DXTYPES_H

#include "../types.h"
#include "numath.h"

// Texture format
enum NuTexFormat
{
	UNKNOWN,
	BPP16,
	BPP24,
	BPP32,
	BPP4,
	RGB8MAYBE
};

// Camera.
typedef struct
{
	Mtx mtx;
	f32 v17;
	f32 v18;
	f32 v19;
	f32 v20;
	u32 v21;
	f32 v22;
	f32 v23;
	f32 v24;
} NuCamera; // 0x60.

// Light.
typedef struct
{
	u8 tmp;
} NuLight; // 0x19.

// Light collection.
typedef struct
{
	u8 tmp;
} NuLightCollection; //0x12C - 300.

// Texture.
typedef struct
{
	u32 format;
	u32 width;
	u32 height;
	u32 mode;
	u32 data;
	u32 palette;
	u16 decal;
} NuTexData;

// Surface.
typedef struct
{
	u8 tmp;
} NuSurface; // 0x18.

// Texture info.
typedef struct
{
	NuTexData tex;
	NuSurface* surface;
	u32 refCount;
	u32 d10;
} NuTex; // 0x28.

typedef struct
{
	u8 tmp;
} NuMtl; // 0x6C.

// Scene.
typedef struct
{
	u8 tmp;
} NuScene; // 0x80. This might be something else, I'm seeing 0x3C for scenes.

// Render data.
typedef struct
{
	u8 tmp;
} NuRndr; // 0x12. I am unsure of this.

#endif // !NU3DXTYPES_H