#ifndef NUFPAR_H
#define NUFPAR_H

#include "types.h"
#include "nucore/nucoretypes.h"

/*
  800be18c 000104 800be18c  4 NuFParGetLine 	Global
  800be3cc 0000bc 800be3cc  4 NuFParInterpretWord 	Global
*/

// Old line position.
extern s32 old_line_pos;

// Read a char from a text file.
char NuGetChar(struct nufpar_s* fPar);

// GetLine.

// Read a word from the text buffer into the word buffer, and return its length.
s32 NuFParGetWord(struct nufpar_s* fPar);

// Get an int from a text file.
s32 NuFParGetInt(struct nufpar_s* fPar);

// Push a command. Returns the index of the next command.
s32 NuFParPushCom(struct nufpar_s* fPar, struct nufpcomjmp_s* jmp);

// Close a text file.
void NuFParClose(struct nufpar_s* fPar);

// Open a text file.
struct nufpar_s* NuFParOpen(s32 handle);

// Destroy a text file.
void NuFParDestroy(struct nufpar_s* fPar);

// Create a text file from a file.
struct nufpar_s* NuFParCreate(char* filename);
#endif // !NUFPAR_H
