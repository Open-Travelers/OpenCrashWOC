#include "gsinit.h"

struct _GS_VERTEXTL* TVertices;
struct _GSMATRIX* CV_SKINMTX;

void GS_InitXForm(void)

{
  if (TVertices == 0) {
    TVertices = (struct _GS_VERTEXTL *)malloc_x(0x2c300);
  }
  CV_SKINMTX = (struct _GSMATRIX *)malloc_x(0x400);
  return;
}

void GS_InitVertexDescriptors(void)

{
  /*
  NINTENDO SDK FUNCTIONS
  GXClearVtxDesc();
  GXSetVtxAttrFmt(GX_VTXFMT0,GX_VA_POS,GX_NRM_NBT,GX_RGBA4,'\0');
  GXSetVtxAttrFmt(GX_VTXFMT0,GX_VA_TEX0,GX_NRM_NBT,GX_RGBA4,'\x01');
  GXSetVtxAttrFmt(GX_VTXFMT1,GX_VA_POS,GX_NRM_NBT,GX_F32,'\0');
  GXSetVtxAttrFmt(GX_VTXFMT1,GX_VA_CLR0,GX_NRM_NBT,GX_RGBA8,'\0');
  GXSetVtxAttrFmt(GX_VTXFMT1,GX_VA_TEX0,GX_NRM_NBT,GX_F32,'\0');
  GXSetVtxAttrFmt(GX_VTXFMT2,GX_VA_POS,GX_NRM_NBT,GX_F32,'\0');
  GXSetVtxAttrFmt(GX_VTXFMT2,GX_VA_NRM,GX_CLR_RGB,GX_F32,'\0');
  GXSetVtxAttrFmt(GX_VTXFMT2,GX_VA_CLR0,GX_NRM_NBT,GX_RGBA8,'\0');
  GXSetVtxAttrFmt(GX_VTXFMT2,GX_VA_TEX0,GX_NRM_NBT,GX_F32,'\0');
  GXSetVtxAttrFmt(GX_VTXFMT7,GX_VA_POS,GX_NRM_NBT,GX_F32,'\0');
  GXSetVtxAttrFmt(GX_VTXFMT7,GX_VA_CLR0,GX_NRM_NBT,GX_RGBA8,'\0');*/
  return;
}
