#include "gstex.h"

static unsigned int GS_TexInitFlag;
unsigned int GS_TexAllocs;
static unsigned int GS_NumTextures;
s32 TexStages[4];
s32 iss3cmp;
static struct _GS_TEXTURE* GS_TexList;
enum _GXTexWrapMode GS_TexWrapMode_t [4];
enum _GXTexWrapMode GS_TexWrapMode_s [4];
enum _GXTevStageID ShadowBodge = GX_TEVSTAGE0;
enum _GXTevStageID maxstage_189 = GX_TEVSTAGE15 | GX_MAX_TEVSTAGE | 0; //GX_TEVSTAGE15 | GX_MAX_TEVSTAGE | FFFFFFE0h

//NGC MATCH
void GS_TexInit(void) {
    s32 i;

    if (GS_TexInitFlag == 0) {
        for (i = 0; i < 4; i++) {
            TexStages[4] = 0;
            GS_TexWrapMode_s[i] = GS_TexWrapMode_t[i] = 1;
        }
        GS_TexList = (struct _GS_TEXTURE *)malloc(0x13000);
        memset(GS_TexList,0,0x13000);
        GS_NumTextures = 0;
        GS_TexInitFlag = 1;
    }
    return;
}


// Re-initialize textures.	//NGC MATCH
void GS_TexReInit(void) {
    s32 i;
    struct _GS_TEXTURE *GSTex;


    if (GS_TexInitFlag != 0) {
        GSTex = GS_TexList;
        for (i = 0; i < 4; i++) {
           TexStages[4] = 0;
           GS_TexWrapMode_s[i] = GS_TexWrapMode_t[i] = 1;
        }

        for (i = 0; i < 0x400; i++) {
            if (GSTex->Flags == -1) {
                free((void *)GSTex->TexBits);
            }
            GSTex = GSTex + 1;
        }
    }
    memset(GS_TexList,0,0x13000);
    GS_NumTextures = 0;
    GS_TexAllocs = 0;
    return;
}

/*
    int x;
    int y; // r6
    int w; // r4
    int h; // r29
    unsigned int * SrcBuf;
    int nBlkWidth; // r28
    int nBlkHeight; // r25
    int nOutByte; // r12

    // Blocks
    /* anonymous block*/ //  {
        // Range: 0x800CC868 -> 0x800CC8E4
      //  int nBlockY; // r0
        /* anonymous block */ //{
            /* Range: 0x800CC884 -> 0x800CC8E4
            int nBlockX;
            unsigned int dwCol; // r9
            unsigned short rgb; // r10
        }
    }
*/
//MATCH GCN
void GS_TexSwizzleRGB5A3(s32 nWidth,s32 nHeight,s32 *TxtBuf,char *DstBuf) {
    s32 w;
    s32 fixedH;
    s32 h;
    s32 iVar4;
    s32 iVar7;
    u32 uVar8;
    u32 uVar9;
    s32 iVar10;
    s32 iVar11;
    s32 iVar12;

    s32 fixedW = nWidth;
    if (fixedW < 0) {
        fixedW = fixedW + 3;
    }
    w = fixedW >> 2;

    fixedH = nHeight;
    if (fixedH < 0) {
        fixedH = fixedH + 3;
    }
    h = fixedH >> 2;

    iVar4 = 0;
    for (iVar11 = 0; iVar11 < h; iVar11++) {
        for(iVar10 = 0; iVar10 < w; iVar10++) {
            for(iVar7 = 0; iVar7 < 4; iVar7++) {
                for(iVar12 = 0; iVar12 < 4; iVar12++) {
                    uVar9 = TxtBuf[(iVar10*4) + (iVar11 * 4 + iVar7) * nWidth + iVar12];
                    if ((uVar9 >> 29) == 7) {
                        uVar8 = 0x8000 | ((uVar9 >> 19) & 0x1f) | ((uVar9 >> 6) & 0x3e0) |
                                ((uVar9 & 0xf8) << 7);
                    }
                    else {
                        uVar8 = (uVar9 >> 20) & 0xF | (uVar9 >> 8) & 0xf0 |
                                ((uVar9 & 0xf0) << 4) | (uVar9 >> 17) & 0x7000;
                    }
                    DstBuf[iVar4++] = uVar8 >> 8;
                    DstBuf[iVar4++] = uVar8;
                }
            }
        }
    }
    return;
}

//NGC MATCH
void GS_TexCreateNU(enum nutextype_e Format,u32 width,u32 height,u8 *bits,u32 MipLevels,u32 RTFlag, s32 theirid) {
    char *newbits;
    s32 i;
    struct _GS_TEXTURE* pTex;

    pTex = GS_TexList;
    if (iss3cmp != 0) {
        newbits = (char *)malloc(iss3cmp);
        GS_TexAllocs = GS_TexAllocs + iss3cmp;
        memcpy(newbits,(char *)(bits + 0xc),iss3cmp);
        DCFlushRange(newbits,iss3cmp);
        for (i = 0; i < 0x400; i++, pTex++) {
            if (pTex->Flags != 0) {
                pTex->Flags = -1;
                pTex->Pad = 0xe;
                pTex->Format = Format;
                pTex->NUID = theirid;
                pTex->Width = width;
                pTex->Height = height;
                pTex->TexBits = (u32)newbits;
                GXInitTexObj(&pTex->Tex,newbits,width,height,0xE,0,0,'\0');
                break;
            }
        }
    } else if (Format == 0x80) {
        newbits = (char *)malloc(MipLevels);
        GS_TexAllocs = GS_TexAllocs + MipLevels;
        memcpy(newbits,bits,MipLevels);
        DCFlushRange(newbits,MipLevels);
        for (i = 0; i < 0x400; i++, pTex++) {
            if (pTex->Flags == 0) {
                pTex->Flags = -1;
                pTex->Pad = 0xe;
                pTex->Format = Format;
                pTex->NUID = theirid;
                pTex->Width = width;
                pTex->Height = height;
                pTex->TexBits = (u32)newbits;
                GXInitTexObj(&pTex->Tex,newbits,width,height,0xE,0,0,'\0');
                break;
            }
        }
    } else if (Format == 0x81) {
        s32 size = width * height * 2;
        newbits = (char *)malloc(size);
        GS_TexAllocs = GS_TexAllocs + size;
        DCFlushRange(bits,size);
        memcpy(newbits,bits,size);
        DCFlushRange(newbits,size);
        for (i = 0; i < 0x400; i++, pTex++) {
            if (pTex->Flags == 0) {
                pTex->Flags = -1;
                pTex->Pad = 5;
                pTex->Format = Format;
                pTex->NUID = theirid;
                pTex->Width = width;
                pTex->Height = height;
                pTex->TexBits = (u32)newbits;
                GXInitTexObj(&pTex->Tex,newbits,width,height,5,0,0,'\0');
                break;
            }
        }
    } else if (Format == 0x82) {
        s32 size = width * height * 4;
        newbits = (char *)malloc(size);
        GS_TexAllocs = GS_TexAllocs + size;
        DCFlushRange(bits,size);
        memcpy(newbits,bits,size);
        DCFlushRange(newbits, size);
        for (i = 0; i < 0x400; i++, pTex++) {
            if (pTex->Flags == 0) {
                pTex->Flags = -1;
                pTex->Pad = 6;
                pTex->Format = Format;
                pTex->NUID = theirid;
                pTex->Width = width;
                pTex->Height = height;
                pTex->TexBits = (u32)newbits;
                GXInitTexObj(&pTex->Tex, newbits, width, height, 6, 0, 0, '\0');
                break;
            }
        }
    } else {
        s32 size = width * height * 2;
        newbits = (char *)malloc(size);
        GS_TexAllocs = GS_TexAllocs + size;
        DCFlushRange(bits,width * height * 4);
        GS_TexSwizzleRGB5A3(width,height,(s32 *)bits,newbits);
        DCFlushRange(newbits,size);
        for (i = 0; i < 0x400; i++, pTex++) {
            if (pTex->Flags == 0) {
                pTex->Flags = -1;
                pTex->Pad = 5;
                pTex->Format = Format;
                pTex->Width = width;
                pTex->NUID = theirid;
                pTex->Height = height;
                pTex->TexBits = (u32)newbits;
                GXInitTexObj(&pTex->Tex,newbits,width,height,5,0, 0,'\0');
                break;
            }
        }
    }
    GS_NumTextures++;
    return;
}

void DCFlushRange (void* arg0, u32 arg1)
{
    return;
}

/*

typedef unsigned int u32;
typedef signed int s32;

void DCFlushRange(s32 arg0, u32 arg1) {
    u32 var_ctr;
    u32 var_r4;

    var_r4 = arg1;
    if (var_r4 > 0U) {
        if (arg0 & 0x1F) {
            var_r4 += 0x20;
        }
        var_ctr = (u32) (var_r4 + 0x1F) >> 5U;
        do {
            __dcbz((void*)var_r4, 0);   //dataCacheBlockFlush
            var_ctr -= 1;
        } while (var_ctr != 0);
    }
    asm {
        sc
    }
}

*/

void GXInitTexObj(struct _GXTexObj * obj, void* image_ptr, u16 width, u16 height, enum _GXTexFmt texFormat, enum _GXTexWrapMode wrapModeS, enum _GXTexWrapMode wrapModeT, char mipmap)
{
   return;
   /*
  int iVar1;
  int iVar2;
  
  memset(obj,0,0x20);
  obj->mode0 = obj->mode0 & 0xfffffffc | wrap_s;
  obj->mode0 = obj->mode0 & 0xfffffff3 | wrap_t << 2;
  obj->mode0 = obj->mode0 & 0xffffffef | 0x10;
  if (mipmap == '\0') {
    obj->mode0 = obj->mode0 & 0xffffff1f | 0x80;
  }
  else {
    obj->flags = obj->flags | 1;
    if (format + 0xfffffff8 < 3) {
      obj->mode0 = obj->mode0 & 0xffffff1f | 0xa0;
    }
    else {
      obj->mode0 = obj->mode0 & 0xffffff1f | 0xc0;
    }
    if ((height & 0xffff) < (uint)width) {
      iVar1 = countLeadingZeros((uint)width);
    }
    else {
      iVar1 = countLeadingZeros(height & 0xffff);
    }
    obj->mode1 = ((int)(@164 * (float)((double)CONCAT44(0x43300000,0x1f - iVar1) - @166)) & 0xffU)
                 << 8 | obj->mode1 & 0xffff00ff;
  }
  obj->fmt = format;
  obj->image0 = obj->image0 & 0xfffffc00 | width - 1;
  obj->image0 = obj->image0 & 0xfff003ff | ((height & 0xffff) - 1) * 0x400;
  obj->image0 = (format & 0xf) << 0x14 | obj->image0 & 0xff0fffff;
  obj->image3 = obj->image3 & 0xffe00000 | (uint)image_ptr >> 5 & 0x1ffffff;
  if (false) {
switchD_800fa714_caseD_7:
    obj->loadFmt = '\x02';
    iVar1 = 2;
    iVar2 = 2;
  }
  else {
    switch(format & 0xf) {
    case GX_TF_I4:
    case 8:
      obj->loadFmt = '\x01';
      iVar1 = 3;
      iVar2 = 3;
      break;
    case GX_TF_I8:
    case GX_TF_IA4:
    case 9:
      obj->loadFmt = '\x02';
      iVar1 = 3;
      iVar2 = 2;
      break;
    case GX_TF_IA8:
    case GX_TF_RGB565:
    case GX_TF_RGB5A3:
    case 10:
      obj->loadFmt = '\x02';
      iVar1 = 2;
      iVar2 = 2;
      break;
    case GX_TF_RGBA8:
      obj->loadFmt = '\x03';
      iVar1 = 2;
      iVar2 = 2;
      break;
    default:
      goto switchD_800fa714_caseD_7;
    case GX_TF_CMPR:
      obj->loadFmt = '\0';
      iVar1 = 3;
      iVar2 = 3;
    }
  }
  obj->loadCnt = (short)((int)((uint)width + (1 << iVar1) + -1) >> iVar1) *
                 (short)((int)((height & 0xffff) + (1 << iVar2) + -1) >> iVar2) & 0x7fff;
  obj->flags = obj->flags | 2;
  return;
   */
}

//MATCH NGC
void GS_ChangeTextureStates(int id) {
  u32 i;
  s32 st;
  struct _GS_TEXTURE *texlist;
  
  st = TexStages[id];
  st--;
  texlist = GS_TexList;
  for(i = 0; i < GS_NumTextures; i++) {
      if (texlist->NUID == st) {
        GXInitTexObjWrapMode(&texlist->Tex,GS_TexWrapMode_s[id],GS_TexWrapMode_t[id]);
        GXLoadTexObj(&texlist->Tex,id);
        return;
      }
      texlist++;
  }
  return;
}

//MATCH NGC
void GS_TexSetWrapModes(int id,enum _GXTexWrapMode mode) {

  if (id < 4) {
    GS_TexWrapMode_s[id] = mode;
    GS_ChangeTextureStates(id);
  }
  return;
}

//MATCH NGC
void GS_TexSetWrapModet(int id,enum _GXTexWrapMode mode) {
  if (id < 4) {
    GS_TexWrapMode_t[id]= mode;
    GS_ChangeTextureStates(id);
  }
  return;
}

//NGC MATCH
void GS_TexSelect(enum _GXTevStageID stage,s32 NUID) {
  s32 iVar1;
  s32 i;
  struct _GS_TEXTURE *pTex;

  if (stage == GX_TEVSTAGE0) {
    ShadowBodge = 0;
  }
  if (3 < (s32)stage) {
    DisplayErrorAndLockup("C:/source/crashwoc/code/system/gc/gstex.c",0x21c,"GS_TexSelect1");
  }
  TexStages[stage] = NUID;
  if ((NUID == 0) || (NUID == 9999)) {
    GXSetNumTevStages(1);
    GXSetTevOrder(stage,GX_TEXCOORD_NULL,GX_TEXMAP_NULL,GX_COLOR0A0);
    GXSetTevOp(stage,GX_PASSCLR);
  }
  else {
    if (NUID == ShadowMatBodge) {
      ShadowBodge = 1;
    }
    pTex = GS_TexList;
    if ((s32)stage > (s32)maxstage_189) {
      maxstage_189 = stage;
    }
    if (stage == GX_TEVSTAGE0) {
      maxstage_189 = stage;
    }
    if (NUID == 0x270e) {
      GS_SetFBCopyTexturePause();
    }
    GXSetNumTexGens((char)maxstage_189 + 1);
    GXSetNumTevStages((char)maxstage_189 + 1);
    GXSetTexCoordGen2(stage,GX_TG_MTX2x4,GX_TG_TEX0,0x3c,0,0x7d);
    GXSetTevOrder(stage,stage,stage,GX_COLOR0A0);
    iVar1 = 0;
    if (stage == GX_TEVSTAGE0) {
      iVar1 = 10;
    }
    GXSetTevColorIn(stage,0xf,8,iVar1,0xf);
    GXSetTevColorOp(stage,0,0,0,1,0);
    iVar1 = 0;
    if (stage == GX_TEVSTAGE0) {
      iVar1 = 5;
    }
    GXSetTevColorIn(stage,7,4,iVar1,7);
    GXSetTevAlphaOp(stage,0,0,0,1,0);
    if (NUID - 0x270eU > 1) {
    NUID--;
    for(i = 0; i < GS_NumTextures; i++, pTex++) {
          if (pTex->NUID == NUID) {
            GXInitTexObjWrapMode(&pTex->Tex,GS_TexWrapMode_s[stage],GS_TexWrapMode_t[stage]);
            GXLoadTexObj(&pTex->Tex,stage);
            return;
          }
    }
      
      DisplayErrorAndLockup("C:/source/crashwoc/code/system/gc/gstex.c",0x281,"GS_TexSelect2");
      GXLoadTexObj(&GS_TexList->Tex,stage);
    }
  }
  return;
}

void GS_SetTevBlend(enum _GXTevStageID id)

{
  //GXSetTevColorIn(id,8,0xf,0xf,10);
  //GXSetTevColorOp(id,0,0,0,1,0);
  return;
}

void GS_SetTevModulate(enum _GXTevStageID id)

{
  //GXSetTevOp(id,GX_MODULATE);
  return;
}

void GS_SetTextureStageState(void)

{
  return;
}
