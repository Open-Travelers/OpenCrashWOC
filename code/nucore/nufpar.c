#include "nufpar.h"
#include "nufile.h"

char NuGetChar(FPar* fPar)
{
    s32 bufferEndPos = fPar->bufferEndPos;
    char ret;
    if (bufferEndPos < 0)
    {
        bufferEndPos = 0;
    }
    if (fPar->pos > fPar->bufferEndPos)
    {
        if (fPar->fileLength > bufferEndPos + 1)
        {
            size_t size = fPar->fileLength - bufferEndPos;
            if (size > 0x1000)
            {
                size = 0x1000;
            }
            size = NuFileRead(fPar->handle, fPar->buffer, size);
            bufferEndPos = fPar->bufferEndPos;
            fPar->bufferEndPos = bufferEndPos + size;
            fPar->bufferPos = bufferEndPos + 1;
            if (size != 0)
            {
                ret = fPar->buffer[fPar->pos - fPar->bufferPos];
                fPar->pos++;
                return ret;
            }
        }
        ret = 0;
    }
    else
    {
        ret = fPar->buffer[fPar->pos - fPar->bufferPos];
        fPar->pos++;
    }
    return ret;
}

u32 NuFParGetWord(FPar* fPar)
{
    u32 currLinePos = old_line_pos = fPar->linePos;
    u32 len = 0;
    u32 inQuotation = 0;
    while (fPar->textBuffer[currLinePos & 0xFF] != 0)
    {
        char currChar = fPar->textBuffer[fPar->linePos];
        switch (currChar)
        {
            case ' ':
            case ',':
            case '\t':
                if (inQuotation)
                {
                    (fPar->wordBuffer + 1)[len & 0xff] = currChar;
                    len++;
                }
                else if (len != 0)
                {
                    (fPar->wordBuffer + 1)[len & 0xFF] = 0;
                    return len;
                }
                break;
            case '"':
                inQuotation = 1 - inQuotation;
                break;
            default:
                (fPar->wordBuffer + 1)[len & 0xff] = currChar;
                len++;
                break;
        }
        currLinePos = fPar->linePos + 1;
        fPar->linePos = currLinePos;
    }
    fPar->wordBuffer[(len & 0xFF) + 1] = 0;
    return len;
}

s32 NuFParGetInt(FPar* fPar)
{
    NuFParGetWord(fPar);
    s32 ret = 0;
    if (fPar->wordBuffer[1] != 0)
    {
        ret = atoi((char*)(fPar->wordBuffer + 1));
    }
    return ret;
}

s32 NuFParPushCom(FPar* fPar, s32 command)
{
    s32 ind = fPar->commandStack[7];
    if (ind < 7)
    {
        fPar->commandStack[7] = ind + 1;
        fPar->commandStack[ind] = command;
        return fPar->commandStack[7];
    }
    return -1;
}

void NuFParClose(FPar* fPar)
{
    NuMemFree(fPar);
}

void NuFParDestroy(FPar* fPar)
{
    fileHandle handle = fPar->handle;
    NuFParClose(fPar);
    NuFileClose(handle);
}

FPar* NuFParCreate(char* filename)
{
    fileHandle handle = NuFileOpen(filename, 0);
    if (handle != NULL)
    {
        FPar* fPar = NuFParOpen(handle);
        if (fPar != NULL)
        {
            return fPar;
        }
        NuFileClose(handle);
    }
    return NULL;
}