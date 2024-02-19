#ifndef EDFILE_H
#define EDFILE_H

#include "../types.h"
#include <stdlib.h>

// Current ed file handle.
fileHandle edfile_handle;

// Open the ed file.
s32 EdFileOpen(char* filename);

// Close the ed file.
s32 EdFileClose();

// Read from an ed file.
void EdFileRead(void* dest, size_t size);

// Read a float from an ed file.
f32 EdFileReadFloat();

// Read an int from an ed file.
s32 EdFileReadInt();

// Read a short from an ed file.
s16 EdFileReadShort();

// Read a char from an ed file.
char EdFileReadChar();

#endif // !EDFILE_H