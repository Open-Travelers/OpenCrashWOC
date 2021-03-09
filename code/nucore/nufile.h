#ifndef NUFILE_H
#define NUFILE_H

#include "../types.h"
#include "nucoretypes.h"
#include "numem.h"
#include <string.h>
#include <stdio.h>
#include <memory.h>

#define MAX_FILES 10
#define MAX_MEM_FILES 20
#define MAX_DAT_FILES 20

// Number of blocks.
u32 blkcnt;

// Block info.
BlockInfo blkinfo[0x400];

// Current file buffer.
void* filebuffer;

// Some kind of file pointer.
void* bpointer;

// If the game disk is bad.
u32 badGameDisk;

// Current file pointer.
fileHandle currentpointer;

// Bytes left to read.
u32 bytesleft;

// Bytes read.
u32 thisbytesread;

// File pointers.
FILE* fpointers[MAX_FILES];

// Memory files.
MemFile memfiles[MAX_MEM_FILES];

// Data files.
MemFile datfiles[MAX_MEM_FILES];

// Loading screen.
s32 loadscreen;

// Load screen fade direction.
u32 loadscreenfadedir;

// Load screen %.
u32 datacounter;

// If the load screen is done.
u32 datafull;

// Number of bytes read.
u32 totalbytesread;

// If the game disk is bad.
u32 NuFileGetBadGameDisc();

// Initialize files and the file buffer.
void NuFileInitEx();

// Check if a file exists.
u32 NuFileExists(char* filename);

// For sanity sake.
s32 checkmemfile();

// For sanity sake.
s32 checkdiskfile();

// Open a file with a given name, and FileMode. Returns the file handle if successful, 0 otherwise.
fileHandle NuFileOpen(char* filename, u32 mode);

// Close a file with fileIndex - 1.
void NuFileClose(fileHandle handle);

// Open a file in memory with a given buffer, size, and mode.
fileHandle NuMemFileOpen(void* buffer, u32 size, u32 mode);

// Data file position.
s64 NuDatFilePos(fileHandle handle);

// Read from a data file.
size_t NuDatFileRead(fileHandle handle, void* dest, size_t size);

// Seek from a data file.
s32 NuDatFileSeek(fileHandle handle, s64 off, s32 whence);

// Close a data file.
void NuDatFileClose(fileHandle handle);

// Close a memory file.
void NuMemFileClose(fileHandle handle);

// Get the memory file position.
s64 NuMemFilePos(fileHandle handle);

// Read a memory file.
size_t NuMemFileRead(fileHandle handle, void* dest, size_t size);

// Seek in a data file.
s32 NuMemFileSeek(fileHandle handle, s64 off, s32 whence);

// Get the file position.
s64 NuFilePos(fileHandle handle);

// Seek a file.
s32 NuFileSeek(fileHandle handle, s64 off, s32 whence);

// Get the size of a file.
s32 NuFileSize(char* fileName);

// Load an entire file into memory.
void* NuFileLoad(char* fileName);

// Load a file into a buffer.
size_t NuFileLoadBuffer(char* fileName, void* dest, s32 maxSize);

// Read a file into a destination.
size_t NuFileRead(fileHandle handle, void* dest, size_t size);

// Start a loading screen.
void NuStartLoadScreen(s32 screen);

// Stop the loading screen.
void NuStopLoadScreen();

// Read a float from a file.
f32 NuFileReadFloat(fileHandle handle);

// Read an int from a file.
s32 NuFileReadInt(fileHandle handle);

// Read a short from a file.
s16 NuFileReadShort(fileHandle handle);

// Read a char from a file.
char NuFileReadChar(fileHandle handle);

// Begin reading a block. Magic is the expected value, can be 0 to not check. Returns the magic.
u32 NuFileBeginBlkRead(fileHandle handle, u32 magic);

// Stop reading a block.
void NuFileEndBlkRead(fileHandle handle);

// Close a dat file.
void NuDatClose(DatFile* datFile);

#endif // !NUFILE_H