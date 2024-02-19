#include "nufpar.h"
#include "nufile.h"

#define LF 0xA	// '\n'
#define CR 0xD	// '\r'
// 0x3b ';'


s32 old_line_pos;

//96% NGC
char NuGetChar(struct nufpar_s* fPar) {
    s32 bufferEndPos = fPar->buffend;
    char ret;
    s32 size;
    s32 tmp;
    
    if (bufferEndPos < 0) {
        bufferEndPos = 0;
    }
    if (fPar->cpos > fPar->buffend) {
        if (bufferEndPos + 1 < fPar->size) {
            size = fPar->size - bufferEndPos;
            if (size > 0x1000) {
                size = 0x1000;
            }
            tmp = NuFileRead(fPar->fh, fPar->fbuff, size);
            fPar->buffstart = fPar->buffend + 1;
            fPar->buffend = fPar->buffend + tmp;
            if (tmp == 0) {
                return 0;
            }
        } else{
           return 0;
        }
    }
    ret = fPar->fbuff[fPar->cpos - fPar->buffstart];
    fPar->cpos++;
    return ret;
}

//MATCH NGC
s32 NuFParGetWord(struct nufpar_s* fPar) {
    s32 len = 0;
    s32 inquotes = 0;
    char chr;
    
    old_line_pos = fPar->line_pos;
    while (fPar->lbuff[(fPar->line_pos) & 0xFF] != 0) {
        chr = fPar->lbuff[fPar->line_pos];
        switch (chr) {
        case 9:
        case 0x20:
        case 0x2c:
            if (inquotes == 0) {
                if (len != 0) {
                    (fPar->wbuff)[len & 0xFF] = 0;
                    return len;
                }
              break;
            }
        default:
                if (chr == 0x22) {
                    inquotes = 1 - inquotes;
                    break;
                } else {
                    fPar->wbuff[len & 0xFF] = chr;
                    len++;
                    break;
                }
            break;
        }
        fPar->line_pos++;
    }
    fPar->wbuff[len & 0xFF] = 0;
    return len;
}

s32 NuFParGetInt(struct nufpar_s* fPar)
{
    NuFParGetWord(fPar);
    s32 ret = 0;
    if (fPar->wbuff[1] != 0)
    {
        ret = atoi((char*)(fPar->wbuff + 1));
    }
    return ret;
}

s32 NuFParPushCom(struct nufpar_s* fPar, struct nufpcomjmp_s* jmp)
{
    s32 ind = fPar->compos;
    if (ind < 7)
    {
        fPar->compos = ind + 1;
        fPar->comstack[ind] = jmp;
        return fPar->compos;
    }
    return -1;
}

void NuFParClose(struct nufpar_s* fPar)
{
    NuMemFree(fPar);
}

struct nufpar_s* NuFParOpen(s32 handle)
{
    struct nufpar_s* fPar = NuMemAlloc(sizeof(struct nufpar_s));	//size: 0x1244
    if (fPar != NULL)
    {
        memset(fPar, 0, sizeof(struct nufpar_s));
        fPar->handle = handle;
        fPar->compos = -1;
        fPar->buffend = -1;
        fPar->line_num = -1;
        s32 originalPos = NuFilePos(handle);
        NuFileSeek(handle, 0, 2);
        fPar->size = NuFilePos(handle);	// = filelength (global var s32)
        NuFileSeek(handle, originalPos, 0);
    }

    return fPar;
}

void NuFParDestroy(struct nufpar_s* fPar)
{
    fileHandle handle = fPar->handle;
    NuFParClose(fPar);
    NuFileClose(handle);
}

struct nufpar_s* NuFParCreate(char* filename)
{
    s32 handle = NuFileOpen(filename, 0); //0= NUFILE_READ
    if (handle != NULL)
    {
        struct nufpar_s* fPar = NuFParOpen(handle);
        if (fPar != NULL)
        {
            return fPar;
        }
        NuFileClose(handle);
    }
    return NULL;
}

s32 NuFParGetLine(struct nufpar_s* fPar) {
    s32 i;
    char ch;
    char* textBuffer_ptr;

    i = 0;
    fPar->line_pos = 0;

    char inc_f2_flag = 1;
    while ((ch = NuGetChar(fPar)) != 0) {
        if (inc_f2_flag) {
            fPar->line_num += 1;
            inc_f2_flag = 0;
        }

        if ((ch == CR) || (ch == LF)) {
            if (ch == CR) {
                ch = NuGetChar(fPar);
            }
            if (i == 0) {
                inc_f2_flag = 1;
            }
            else {
                break;
            }

        }
        else {
            if (ch == 0x3B) {
                if (i == 0) {
                    do {
                        ch = NuGetChar(fPar);
                    } while (!((ch == LF) || (ch == CR) || (ch == 0)));
                    if (ch == CR) {
                        ch = NuGetChar(fPar);
                    }
                    i = 0;
                    fPar->line_pos = 0;
                    inc_f2_flag = 1;
                    continue;
                }
            }
            fPar->lbuff[i] = ch;
            i += 1;
        }
    }
    fPar->lbuff[i] = 0;
    return i;
}

// 79% NGC
s32 NuFParInterpretWord(struct nufpar_s* fPar) {
    s32 i;
    if (fPar->compos >= 0 && fPar->comstack[fPar->compos]->fname != NULL) {
        for (i = 0;fPar->comstack[fPar->compos]->fname != NULL; i++) {
            if (strcasecmp((fPar->comstack[fPar->compos]->fname), fPar->wbuff) == 0) {
                fPar->comstack[fPar->compos][i].func(fPar);
                return 1;
            }
        }
    }
    return 0;
}
