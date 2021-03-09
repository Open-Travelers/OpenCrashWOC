#ifndef NUCORETYPES_H
#define NUCORETYPES_H

#include "../types.h"

enum FileMode {
	ReadBinary,
	WriteBinary
};

// Memory file.
typedef struct
{
	void* start;
	void* end;
	void* position;
	u32 unused;
	u32 open;
} MemFile;

// Data file, but this doesn't even need to exist.
typedef struct
{
	u32 v1;
	u32 v2;
	u32 v3;
	u32 v4;
	void* data;
	u32 v6;
	u32 v7;
	u32 fileId;
	fileHandle handle;
	u16 managedMem;
	u16 pad;
} DatFile;

typedef struct
{
	u32 magic;
	s32 size;
	u32 pos;
} BlockInfo;

typedef struct
{
	fileHandle handle;
	u8 buffer[0x1000];
	char textBuffer[0x100];
	char wordBuffer[0x104];
	u32 f2;
	u32 linePos;
	u32 pos;
	s32 bufferPos;
	s32 bufferEndPos;
	u32 f7;
	s32 commandStack[8]; // The last entry is the index of the last entry, where -1 is null.
	u32 fileLength;
} FPar;

#endif // !NUCORETYPES_H