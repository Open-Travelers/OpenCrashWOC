#include "gsbuffer.h"

//MATCH GCN
void* GS_CreateBuffer (u32 length, s32 btype){
    struct _GS_BUFFER *bufptr;

    bufptr = (struct _GS_BUFFER*)malloc(length + 8);
    GS_BufferSize += length;
    bufptr->length = length;
    bufptr->type = btype;
    BufferTypes[btype] = BufferTypes[btype] + length;

	return bufptr + 1;
}

//MATCH GCN
void GS_DeleteBuffer(void* ptr)
{
    struct _GS_BUFFER* bufptr = (struct _GS_BUFFER*)((int)ptr - 8);
    GS_BufferSize -= bufptr->length;
    BufferTypes[bufptr->type] -= bufptr->length;
    free(bufptr);
}
