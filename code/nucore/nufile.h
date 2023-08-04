#ifndef NUFILE_H
#define NUFILE_H

#include "types.h"
#include "nucoretypes.h"
#include "numem.h"
//#include <string.h>
#include <stdio.h>
//#include <memory.h>

#define MAX_FILES 10
#define MAX_MEM_FILES 20
#define MAX_DAT_FILES 20

#ifndef FIRST
// Number of blocks.
extern s32 blkcnt;

// Block info.
extern struct BlockInfo blkinfo[0x400];


// Current file buffer.
extern void* filebuffer;

// Some kind of file pointer.
extern void* bpointer;

// Load screen fade direction.
extern u32 loadscreenfadedir;
#endif
// If the game disk is bad.
extern s32 badGameDisk;

// Current file pointer.
extern fileHandle currentpointer;

// Bytes left to read.
extern s32 bytesleft;

// Bytes read.
extern s32 thisbytesread;

// File pointers.
extern FILE* fpointers[MAX_FILES];

// Memory files.
extern struct numemfile_s memfiles[MAX_MEM_FILES];

// Data files.
extern struct nudatfile_s datfiles[MAX_MEM_FILES];

// Loading screen.
extern s32 loadscreen;

// Load screen %.
extern s32 datacounter;

// If the load screen is done.
extern s32 datafull;

// Number of bytes read.
extern s32 totalbytesread;

// If the game disk is bad.
s32 NuFileGetBadGameDisc();

// Initialize files and the file buffer.
void NuFileInitEx(int deviceid, int rebootiop);

// Check if a file exists.
s32 NuFileExists(char* filename);

// For sanity sake.
s32 checkmemfile(char* name);

// For sanity sake.
s32 checkdiskfile(char* name);

// Open a file with a given name, and FileMode. Returns the file handle if successful, 0 otherwise.
fileHandle NuFileOpen(char* filename, enum nufilemode_e mode);

// Close a file with fileIndex - 1.
void NuFileClose(fileHandle handle);

// Open a file in memory with a given buffer, size, and mode.
fileHandle NuMemFileOpen(void* buffer, u32 size, u32 mode);

// Data file position.
s32 NuDatFilePos(fileHandle handle);

// Read from a data file.
s32 NuDatFileRead(s32 fh, void* data, s32 size);

// Seek from a data file.
s32 NuDatFileSeek(s32 fh, s32 offset, s32 origin);

// Close a data file.
void NuDatFileClose(fileHandle handle);

// Close a memory file.
void NuMemFileClose(fileHandle handle);

// Get the memory file position.
s32 NuMemFilePos(fileHandle handle);

// Read a memory file.
s32 NuMemFileRead(fileHandle handle, void* data, s32 size);

// Seek in a data file.
s32 NuMemFileSeek(fileHandle handle, s32 offset, s32 origin);

// Get the file position.
s32 NuFilePos(fileHandle handle);

// Seek a file.
s32 NuFileSeek(fileHandle handle, s32 offset, s32 origin);

// Get the size of a file.
s32 NuFileSize(char* fileName);

// Load an entire file into memory.
void* NuFileLoad(char* fileName);

// Load a file into a buffer.
s32 NuFileLoadBuffer(char* fileName, void* dest, s32 maxSize);

// Read a file into a destination.
s32 NuFileRead(fileHandle handle, void* data, s32 size);

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
s32 NuFileBeginBlkRead(fileHandle handle, s32 blkType);

// Stop reading a block.
s32 NuFileEndBlkRead(fileHandle handle);

// Close a dat file.
void NuDatClose(struct nudathdr_s* ndh);

#endif // !NUFILE_H
