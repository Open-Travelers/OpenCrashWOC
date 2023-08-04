#include "gstex.h"


static unsigned int GS_TexInitFlag;
unsigned int GS_TexAllocs;
static struct _GS_TEXTURE* GS_TexList;
static unsigned int GS_NumTextures;
s32 TexStages[4];
enum GXTexWrapMode GS_TexWrapMode_s;
enum GXTexWrapMode GS_TexWrapMode_t;
enum _GXTevStageID ShadowBodge = GX_TEVSTAGE0;
enum _GXTevStageID maxstage_189 = GX_TEVSTAGE15 | GX_MAX_TEVSTAGE | 0; //GX_TEVSTAGE15 | GX_MAX_TEVSTAGE | FFFFFFE0h

void GS_TexInit(void)
{
  int iVar1;
  int iVar2;

  if (GS_TexInitFlag == 0) {
    iVar2 = 4;
    iVar1 = 0;
    do {
      //*(undefined4 *)((int)&GS_TexWrapMode_t + iVar1) = 1;
      //*(undefined4 *)((int)&GS_TexWrapMode_s + iVar1) = 1;
      //DAT_80214b70 = 0;
      iVar1 = iVar1 + 4;
      iVar2 = iVar2 + -1;
    } while (iVar2 != 0);
    GS_TexList = (struct _GS_TEXTURE *)malloc(0x13000);
    memset(GS_TexList,0,0x13000);
    GS_NumTextures = 0;
    GS_TexInitFlag = 1;
  }
  return;
}


// Re-initialize textures.
void GS_TexReInit(void)		// TODO!!!

{
  int i;
  struct _GS_TEXTURE *GSTex;
  int j;

  GSTex = GS_TexList;
  if (GS_TexInitFlag != 0) {
    j = 4;
    i = 0;
    do {
      /* *(undefined4 *)((int)&GS_TexWrapMode_t + i) = 1;
      *(undefined4 *)((int)&GS_TexWrapMode_s + i) = 1;
      DAT_80214b70 = 0;*/
      i = i + 4;
      j = j + -1;
    } while (j != 0);
    i = 0x400;
    do {
      if (GSTex->Flags == -1) {
        free((void *)GSTex->TexBits);
      }
      //GSTex = GSTex + 1;
      i = i + -1;
    } while (i != 0);
  }
  memset(GS_TexList,0,0x13000);
  GS_TexAllocs = 0;
  GS_NumTextures = 0;
  return;
}


void GS_TexSwizzleRGB5A3(int width,int height,void *param_3,void *param_4)

{
  /*int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  u32 *puVar5;
  int iVar6;
  int iVar7;
  u16 uVar8;
  u32 uVar9;
  int iVar10;
  int iVar11;
  int iVar12;

  iVar2 = width;
  if (width < 0) {
    iVar2 = width + 3;
  }
  if (height < 0) {
    height = height + 3;
  }
  iVar4 = 0;
  iVar6 = 0;
  if (0 < height >> 2) {
    do {
      iVar10 = 0;
      iVar11 = iVar6 + 1;
      if (0 < iVar2 >> 2) {
        do {
          iVar1 = iVar10 * 4;
          iVar7 = 0;
          iVar10 = iVar10 + 1;
          do {
            iVar3 = iVar6 * 4 + iVar7;
            iVar7 = iVar7 + 1;
            iVar12 = 4;
            puVar5 = (uint *)((iVar1 + iVar3 * width) * 4 + (int)param_3);
            do {
              uVar9 = *puVar5;
              puVar5 = puVar5 + 1;
              uVar8 = (ushort)(uVar9 >> 0x10);
              if (uVar9 >> 0x1d == 7) {
                uVar8 = uVar8 >> 3 & 0x1f | (ushort)(uVar9 >> 6) & 0x3e0 | 0x8000 |
                        (ushort)((uVar9 & 0xf8) << 7);
              }
              else {
                uVar8 = uVar8 >> 4 & 0xf | (ushort)(uVar9 >> 8) & 0xf0 |
                        (ushort)((uVar9 & 0xf0) << 4) | uVar8 >> 1 & 0x7000;
              }
              *(char *)((int)param_4 + iVar4) = (char)(uVar8 >> 8);
              *(char *)((int)param_4 + iVar4 + 1) = (char)uVar8;
              iVar4 = iVar4 + 2;
              iVar12 = iVar12 + -1;
            } while (iVar12 != 0);
          } while (iVar7 < 4);
        } while (iVar10 < iVar2 >> 2);
      }
      iVar6 = iVar11;
    } while (iVar11 < height >> 2);
  }
  return;*/
}


s32 iss3cmp;

void GS_TexCreateNU(enum nutextype_e type,int width,int height,void *bits,int mmcnt,int rendertargetflag, int tpid)
{
  void *mmalloc;
  struct _GXTexObj *obj;
  enum _GXTexFmt format;
  int i;
  struct _GS_TEXTURE* texlist;
  size_t size;

  texlist = GS_TexList;
  if (iss3cmp == 0) {
    if (type == 0x80) {
      mmalloc = malloc(mmcnt);
      GS_TexAllocs = GS_TexAllocs + mmcnt;
      memcpy(mmalloc,bits,mmcnt);
      DCFlushRange(mmalloc,mmcnt);
      for (i = 0; i < 0x400; i = i + 1) {
        if (texlist->Flags == 0) {
          texlist->Pad = 0xe;
          obj = &texlist->Tex;
          texlist->Format = 0x80;
          texlist->NUID = tpid;
          texlist->Width = width;
          texlist->Height = height;
          format = GX_CTF_RA8;
          goto LAB_800cbc0c;
        }
        texlist = texlist + 1;
      }
    }
    else if (type == 0x81) {
      size = width * height * 2;
      mmalloc = malloc(size);
      GS_TexAllocs = GS_TexAllocs + size;
      DCFlushRange(bits,size);
      memcpy(mmalloc,bits,size);
      DCFlushRange(mmalloc,size);
      for (i = 0; i < 0x400; i = i + 1) {
        if (texlist->Flags == 0) {
          texlist->Pad = 5;
          obj = &texlist->Tex;
          texlist->Format = 0x81;
          texlist->NUID = tpid;
          texlist->Width = width;
          texlist->Height = height;
          format = GX_TF_RGB565;
          goto LAB_800cbc0c;
        }
        texlist = texlist + 1;
      }
    }
    else if (type == 0x82) {
      size = width * height * 4;
      mmalloc = malloc(size);
      GS_TexAllocs = GS_TexAllocs + size;
      DCFlushRange(bits,size);
      memcpy(mmalloc,bits,size);
      DCFlushRange(mmalloc,size);
      for (i = 0; i < 0x400; i = i + 1) {
        if (texlist->Flags == 0) {
          texlist->Pad = 6;
          obj = &texlist->Tex;
          texlist->Format = 0x82;
          texlist->NUID = tpid;
          texlist->Width = width;
          texlist->Height = height;
          format = GX_TF_RGB5A3;
          goto LAB_800cbc0c;
        }
        texlist = texlist + 1;
      }
    }
    else {
      size = width * height * 2;
      mmalloc = malloc(size);
      GS_TexAllocs = GS_TexAllocs + size;
      DCFlushRange(bits,width * height * 4);
      GS_TexSwizzleRGB5A3(width,height,bits,mmalloc);
      DCFlushRange(mmalloc,size);
      for (i = 0; i < 0x400; i = i + 1) {
        if (texlist->Flags == 0) {
          texlist->Pad = 5;
          texlist->Format = type;
          texlist->NUID = tpid;
          texlist->Width = width;
          texlist->Height = height;
          texlist->TexBits = (u32)mmalloc;
          texlist->Flags = -1;
          GXInitTexObj(&texlist->Tex,mmalloc,width & 0xffff,height & 0xffff,GX_TF_RGB565,GX_CLAMP, GX_CLAMP,'\0');
          break;
        }
        texlist = texlist + 1;
      }
    }
  }
  else {
    mmalloc = malloc(iss3cmp);
    GS_TexAllocs = GS_TexAllocs + iss3cmp;
    memcpy(mmalloc,(void *)((int)bits + 0xc),iss3cmp);
    DCFlushRange(mmalloc,iss3cmp);
    for (i = 0; i < 0x400; i = i + 1) {
      if (texlist->Flags == 0) {
        texlist->Pad = 0xe;
        obj = &texlist->Tex;
        texlist->Format = type;
        texlist->NUID = tpid;
        texlist->Width = width;
        texlist->Height = height;
        format = GX_CTF_RA8;
LAB_800cbc0c:
        texlist->TexBits = (u32)mmalloc;
        texlist->Flags = -1;
        GXInitTexObj(obj,mmalloc,width & 0xffff,height & 0xffff,format,GX_CLAMP,GX_CLAMP,'\0');
        break;
      }
      texlist = texlist + 1;
    }
  }
  GS_NumTextures = GS_NumTextures + 1;
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


UNKTYPE GXInitTexObj(struct _GXTexObj * obj, UNKTYPE *image, u16 width, u32 height, enum _GXTexFmt texFormat, enum GXTexWrapMode wrapModeS, enum GXTexWrapMode wrapModeT, char mipmap) //correct?
{
   return;
   /*
     int iVar1;
  int iVar2;

  memset(obj,0,0x20);
  obj->mFlags = obj->mFlags & 0xfffffffc | wrap_s;
  obj->mFlags = obj->mFlags & 0xfffffff3 | wrap_t << 2;
  obj->mFlags = obj->mFlags & 0xffffffef | 0x10;
  if (mipmap == '\0') {
    obj->mFlags = obj->mFlags & 0xffffff1f | 0x80;
  }
  else {
    *(byte *)((int)&obj->unk_0x1c + 3) = *(byte *)((int)&obj->unk_0x1c + 3) | 1;
    if (format + 0xfffffff8 < 3) {
      obj->mFlags = obj->mFlags & 0xffffff1f | 0xa0;
    }
    else {
      obj->mFlags = obj->mFlags & 0xffffff1f | 0xc0;
    }
    if ((height & 0xffff) < (uint)width) {
      iVar1 = countLeadingZeros((uint)width);
    }
    else {
      iVar1 = countLeadingZeros(height & 0xffff);
    }
    *(uint *)obj->UNK_0x4 =
         ((int)(@164 * (float)((double)CONCAT44(0x43300000,0x1f - iVar1) - @166)) & 0xffU) << 8 |
         *(uint *)obj->UNK_0x4 & 0xffff00ff;
  }
  obj->mFormat = format;
  obj->mDimensions = obj->mDimensions & 0xfffffc00 | width - 1;
  obj->mDimensions = obj->mDimensions & 0xfff003ff | ((height & 0xffff) - 1) * 0x400;
  obj->mDimensions = (format & 0xf) << 0x14 | obj->mDimensions & 0xff0fffff;
  *(uint *)obj->UNK_0xC = *(uint *)obj->UNK_0xC & 0xffe00000 | (uint)image_ptr >> 5 & 0x1ffffff;
  switch(format & 0xf) {
  case GX_TF_I4:
  case 8:
    *(undefined *)((int)&obj->unk_0x1c + 2) = 1;
    iVar1 = 3;
    iVar2 = 3;
    break;
  case GX_TF_I8:
  case GX_TF_IA4:
  case 9:
    *(undefined *)((int)&obj->unk_0x1c + 2) = 2;
    iVar1 = 3;
    iVar2 = 2;
    break;
  case GX_TF_IA8:
  case GX_TF_RGB565:
  case GX_TF_RGB5A3:
  case 10:
    *(undefined *)((int)&obj->unk_0x1c + 2) = 2;
    iVar1 = 2;
    iVar2 = 2;
    break;
  case GX_TF_RGBA8:
    *(undefined *)((int)&obj->unk_0x1c + 2) = 3;
    iVar1 = 2;
    iVar2 = 2;
    break;
  default:
    *(undefined *)((int)&obj->unk_0x1c + 2) = 2;
    iVar1 = 2;
    iVar2 = 2;
    break;
  case GX_TF_CMPR:
    *(undefined *)((int)&obj->unk_0x1c + 2) = 0;
    iVar1 = 3;
    iVar2 = 3;
  }
  *(ushort *)&obj->unk_0x1c =
       (short)((int)((uint)width + (1 << iVar1) + -1) >> iVar1) *
       (short)((int)((height & 0xffff) + (1 << iVar2) + -1) >> iVar2) & 0x7fff;
  *(byte *)((int)&obj->unk_0x1c + 3) = *(byte *)((int)&obj->unk_0x1c + 3) | 2;
  return;
   */
}


void GS_ChangeTextureStates(int id)

{
  u32 i;
  struct _GS_TEXTURE *texlist;

  i = 0;
  texlist = GS_TexList;
  if (GS_NumTextures != 0) {
    do {
      if (texlist->NUID == TexStages[id] - 1U) {
        //GXInitTexObjWrapMode(&texlist->Tex,(&GS_TexWrapMode_s)[id],(&GS_TexWrapMode_t)[id]);
        //GXLoadTexObj(&texlist->Tex,id);
        return;
      }
      i = i + 1;
      texlist = texlist + 1;
    } while (i < GS_NumTextures);
  }
  return;
}


void GS_TexSetWrapModes(int id,enum GXTexWrapMode mode)
{
  if (id < 4) {
    (&GS_TexWrapMode_s)[id] = mode;
    GS_ChangeTextureStates(id);
  }
  return;
}

void GS_TexSetWrapModet(int id,enum GXTexWrapMode mode)

{
  if (id < 4) {
    (&GS_TexWrapMode_t)[id] = mode;
    GS_ChangeTextureStates(id);
  }
  return;
}


void GS_TexSelect(enum _GXTevStageID stage,int NUID)

{
  int iVar1;
  u32 uVar2;
  struct _GS_TEXTURE *GSTex;
  bool check;

  check = stage == GX_TEVSTAGE0;
  if (check) {
    ShadowBodge = stage;
  }
  if (3 < (int)stage) {
    //DisplayErrorAndLockup("C:/source/crashwoc/code/system/gc/gstex.c",0x21c,"GS_TexSelect1");
  }
  TexStages[stage] = NUID;
  GSTex = GS_TexList;
  if ((NUID == 0) || (NUID == 9999)) {
    //GXSetNumTevStages('\x01');
    //GXSetTevOrder(stage,GX_TEXCOORD_NULL,GX_TEXMAP_NULL,GX_COLOR0A0);
    //GXSetTevOp(stage,GX_PASSCLR);
  }
  else {
    /*if (NUID == ShadowMatBodge) {
      ShadowBodge = GX_TEVSTAGE1;
    }*/
    if ((int)maxstage_189 < (int)stage) {
      maxstage_189 = stage;
    }
    if (check) {
      maxstage_189 = stage;
    }
    if (NUID == 0x270e) {
      GS_SetFBCopyTexturePause();
    }
    /*GXSetNumTexGens((char)maxstage_189 + '\x01');
    GXSetNumTevStages((char)maxstage_189 + '\x01');
    GXSetTexCoordGen2(stage,GX_TG_MTX2x4,GX_TG_TEX0,0x3c,'\0',0x7d);
    GXSetTevOrder(stage,stage,stage,GX_COLOR0A0);*/
    iVar1 = 0;
    if (check) {
      iVar1 = 10;
    }
    //GXSetTevColorIn(stage,0xf,8,iVar1,0xf);
    //GXSetTevColorOp(stage,0,0,0,1,0);
    iVar1 = 0;
    if (check) {
      iVar1 = 5;
    }
    //GXSetTevColorIn(stage,7,4,iVar1,7);
    //GXSetTevAlphaOp(stage,0,0,0,1,0);
    if (1 < NUID - 0x270eU) {
      uVar2 = 0;
      if (GS_NumTextures != 0) {
        do {
          if (GSTex->NUID == NUID - 1U) {
            //GXInitTexObjWrapMode(&GSTex->Tex,(&GS_TexWrapMode_s)[stage],(&GS_TexWrapMode_t)[stage] );
            //GXLoadTexObj(&GSTex->Tex,stage);
            return;
          }
          uVar2 = uVar2 + 1;
          GSTex = GSTex + 1;
        } while (uVar2 < GS_NumTextures);
      }
      //DisplayErrorAndLockup("C:/source/crashwoc/code/system/gc/gstex.c",0x281,"GS_TexSelect2");
      //GXLoadTexObj(&GS_TexList->Tex,stage);
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
