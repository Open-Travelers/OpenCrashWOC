#include "gsprim.h"

#define AT_ADDRESS(xyz) __attribute__((address((xyz))))


volatile PPCWGPipe GXFIFO AT_ADDRESS(0xCC008000);


static inline void GXPosition3f32(const f32 x, const f32 y, const f32 z)
{
    GXFIFO.f32 = x;
    GXFIFO.f32 = y;
    GXFIFO.f32 = z;
}

static inline void GXNormal3f32(const f32 nx, const f32 ny, const f32 nz)
{
    GXFIFO.f32 = nx;
    GXFIFO.f32 = ny;
    GXFIFO.f32 = nz;
}

static inline void GXColor4u8(const u8 r,const u8 g,const u8 b,const u8 a)
{
	GXFIFO.u8 = r;
	GXFIFO.u8 = g;
	GXFIFO.u8 = b;
	GXFIFO.u8 = a;
}

static inline void GXTexCoord2f32(const f32 s, const f32 t)
{
    GXFIFO.f32 = s;
    GXFIFO.f32 = t;
}

static inline void GXNormal1x16(const u16 index)
{
    GXFIFO.u16 = index;
}

static inline void GXPosition1x16(const u16 index) {
    GXFIFO.u16 = index;
}

static inline void GXTexCoord1x16(const u16 index)
{
    GXFIFO.u16 = index;
}

static inline void GXColor1u32(const u32 clr)
{
    GXFIFO.u32 = clr;
}

static inline void GXColor1u16(const u16 clr) {
    GXFIFO.u16 = clr;
}

static inline void GXColor3f32(const f32 r, const f32 g, const f32 b) {
    GXFIFO.u8 = (u8)(r * 255.0);
    GXFIFO.u8 = (u8)(g * 255.0);
    GXFIFO.u8 = (u8)(b * 255.0);
}

/*
//NGC MATCH
void GS_DrawTriStrip(u32 nverts, f32* vert_list, s32 stride) {
    s32 diff;
    short i;

    if (stride == 0x1c) {
        if (GS_CurrentVertDesc != 0x81) {
            GS_CurrentVertDesc = 0x81;
            GXClearVtxDesc();
            GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
            GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
            GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        }
    } else if (GS_CurrentVertDesc != 0x83) {
        GS_CurrentVertDesc = 0x83;
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
        GXSetVtxDesc(GX_VA_NRM, GX_INDEX16);
        GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
    }
    
    GXSetArray(GX_VA_POS, vert_list, stride);
    if (stride != 0x1c) {
    ;
        GXSetArray(GX_VA_NRM, vert_list+3, stride);
    }
    GXSetArray(GX_VA_TEX0, vert_list + ((stride >> 2) -2), stride); 
    if (stride == 0x1c) {
        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT1, nverts);
    } else {
        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT2, nverts);
    }

    for (i = 0; i < nverts; i++) {
        if (stride == 0x1c) {
            GXPosition1x16(i);
            diff = ((struct _GS_VERTEXTL*)&vert_list[(((i * (stride >> 2)) + (stride >> 2)) - 7)])->diffuse;
            if ((IsStencil != 0) || (ShadowBodge != GX_TEVSTAGE0)) {
                        GXColor1u32(ShadowColour);
            } else {
                    if (GS_MaterialSourceEmissive != 0) {
                        GXColor1u32(diff);
                    } else {
                        GXColor1u32((u32)GS_CurrentMaterialEmissivergba);
                    }
            }
            GXColor1u16(i);
        } else {
          GXNormal1x16(i);
          GXPosition1x16(i);
            diff = ((struct _GS_VERTEXTL*)&vert_list[(((i * (stride >> 2)) + (stride >> 2)) - 7)])->diffuse;
            if ((IsStencil != 0) || (ShadowBodge != GX_TEVSTAGE0)) {
                        GXColor1u32(ShadowColour);
            } else {
                    if (GS_MaterialSourceEmissive != 0) {
                        GXColor1u32(diff);
                    } else {
                        GXColor1u32((u32)GS_CurrentMaterialEmissivergba);
                    }
            }
            GXColor1u16(i);
        }
    }
    return;
}

//NGC MATCH (stack problem)
void GS_DrawTriList(s32 nverts,float *vertlist,s32 stride) {
  Mtx44 GSmtx;
  Mtx44 GSmtx2;
  s32 i;
  s32 diff;
  f32* temp_vert;
  
  DBTimerStart(0x15);
  temp_vert = vertlist;
  if (stride == 0x1c) {
    C_MTXOrtho(GSmtx,0.0f,448.0f,0.0f,640.0f,0.0f,-1.0f);
    GXSetProjection(GSmtx,GX_ORTHOGRAPHIC);
    PSMTXIdentity(GSmtx2);
    GXLoadPosMtxImm(GSmtx2,0); 
    GXSetCurrentMtx(0);
    //iVar5 = stride >> 2;
    if (GS_CurrentVertDesc != 0x81) {
      GS_CurrentVertDesc = 0x81;
      GXClearVtxDesc();
      GXSetVtxDesc(GX_VA_POS,GX_DIRECT);
      GXSetVtxDesc(GX_VA_CLR0,GX_DIRECT);
      GXSetVtxDesc(GX_VA_TEX0,GX_DIRECT);
    }
    GXBegin(GX_TRIANGLES,GX_VTXFMT1,nverts);
    for (i = 0; i < nverts; i++) {
            GXPosition3f32(((struct _GS_VERTEXTL*)vertlist)[0].x,((struct _GS_VERTEXTL*)vertlist)[0].y,((struct _GS_VERTEXTL*)vertlist)[0].z);
            diff = *(u32*)&((struct _GS_VERTEXTL*)temp_vert)[0].v;
            if ((IsStencil != 0) || (ShadowBodge != GX_TEVSTAGE0)) {
                GXColor1u32(ShadowColour);
            } else {
                if (GS_MaterialSourceEmissive != 0) {
                   GXColor1u32(diff);
                } else {
                    GXColor1u32((u32)GS_CurrentMaterialEmissivergba);
                }
            }
        GXTexCoord2f32(((struct _GS_VERTEXTL*)vertlist)[0].u, ((struct _GS_VERTEXTL*)vertlist)[0].v);
        vertlist += 7;

        
    }
    GS_SetProjectionMatrix(&GSmtx2);
  }
  else {
    if (GS_CurrentVertDesc != 0x82) {
      GS_CurrentVertDesc = 0x82;
      GXClearVtxDesc();
      GXSetVtxDesc(GX_VA_POS,GX_DIRECT);
      GXSetVtxDesc(GX_VA_NRM,GX_DIRECT);
      GXSetVtxDesc(GX_VA_CLR0,GX_DIRECT);
      GXSetVtxDesc(GX_VA_TEX0,GX_DIRECT);
    }
    GXBegin(GX_TRIANGLES,GX_VTXFMT2,nverts);
    for (i = 0; i < nverts; i++) {
                
                GXPosition3f32(((struct _GS_VERTEX*)vertlist)[0].x,((struct _GS_VERTEX*)vertlist)[0].y,((struct _GS_VERTEX*)vertlist)[0].z); 
                GXNormal3f32(((struct _GS_VERTEX*)vertlist)[0].nx, ((struct _GS_VERTEX*)vertlist)[0].ny, ((struct _GS_VERTEX*)vertlist)[0].nz);
                diff = ((struct _GS_VERTEX*)temp_vert)->diffuse;
                if ((IsStencil != 0) || (ShadowBodge != GX_TEVSTAGE0)) {
                        GXColor1u32(ShadowColour);
                } else {
                if (GS_MaterialSourceEmissive != 0) {
                    GXColor1u32(diff);
                }else {
                    GXColor1u32((u32)GS_CurrentMaterialEmissivergba);
                }
            }
        GXTexCoord2f32(((struct _GS_VERTEX*)vertlist)[0].u, ((struct _GS_VERTEX*)vertlist)[0].v);
        vertlist += 9;
    }
  }
  DBTimerEnd(0x15);
  return;
}

//NGC MATCH
static inline void SetVertexStridedI(short index, struct _GS_VERTEX* vertlist, s32 stride,struct _GS_VERTEX * d) {
    u32 dd;
    GXPosition3f32(vertlist->x,vertlist->y, vertlist->z);
    GXNormal3f32(vertlist->nx,vertlist->ny, vertlist->nz);
    dd= d->diffuse;
            if ((IsStencil != 0) || (ShadowBodge != GX_TEVSTAGE0)) {
                GXColor1u32(ShadowColour);
            } else {
                if (GS_MaterialSourceEmissive != 0) {
                   GXColor1u32(dd);
                } else {
                    GXColor1u32((u32)GS_CurrentMaterialEmissivergba);
                }
            }
    GXTexCoord2f32(vertlist->u,vertlist->v);
}

//NGC MATCH
void GS_DrawQuadList(u32 nverts,struct _GS_VERTEX *vertlist, s32 stride) {
    s32 i;
    struct _GS_VERTEX *d = vertlist;
    
  if (GS_CurrentVertDesc != 0x82) {
    GS_CurrentVertDesc = 0x82;
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS,GX_DIRECT);
    GXSetVtxDesc(GX_VA_NRM,GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0,GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0,GX_DIRECT);
  }
  GXBegin(GX_QUADS,GX_VTXFMT2,(u16)nverts);
    for (i = 0; i < nverts; i++) {
        SetVertexStridedI(nverts,vertlist,stride, d);
        vertlist++;
    }
  return;
}

//NGC MATCH
static inline void SetVertexT (struct _GS_VERTEX *vertlist) {
    s32 diff;

    GXPosition3f32(vertlist->x,vertlist->y,vertlist->z);
    GXNormal3f32(vertlist->nx,vertlist->ny,vertlist->nz);
    diff = vertlist->diffuse;
            if ((IsStencil != 0) || (ShadowBodge != GX_TEVSTAGE0)) {
                GXColor1u32(ShadowColour);
            } else {
                if (GS_MaterialSourceEmissive != 0) {
                   GXColor1u32(diff);
                } else {
                    GXColor1u32((u32)GS_CurrentMaterialEmissivergba);
                }
            }
    GXTexCoord2f32(vertlist->u,vertlist->v);
}

//NGC MATCH
void GS_DrawPrimitiveQuad(struct _GS_VERTEX *vertlist) {
  if (GS_CurrentVertDesc != 0x82) {
    GS_CurrentVertDesc = 0x82;
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS,GX_DIRECT);
    GXSetVtxDesc(GX_VA_NRM,GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0,GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0,GX_DIRECT);
  }
  GXBegin(GX_QUADS,GX_VTXFMT2,4);
    SetVertexT(vertlist);
    SetVertexT(vertlist + 1);
    SetVertexT(vertlist + 2);
    SetVertexT(vertlist + 3);
  return;
}

//NGC MATCH
static inline void SetVertexIndex(s16* index, float * vertlist, s32 stride) {
    s32 diff;

        GXPosition3f32(((struct _GS_VERTEX*)vertlist)[*(s16*)index].x,((struct _GS_VERTEX*)vertlist)[*(s16*)index].y, ((struct _GS_VERTEX*)vertlist)[*(s16*)index].z);
        GXNormal3f32(((struct _GS_VERTEX*)vertlist)[*(s16*)index].nx,((struct _GS_VERTEX*)vertlist)[*(s16*)index].ny, ((struct _GS_VERTEX*)vertlist)[*(s16*)index].nz);
            diff = ((struct _GS_VERTEX*)vertlist)[*(s16*)index].diffuse;
            if ((IsStencil != 0) || (ShadowBodge != GX_TEVSTAGE0)) {
                    GXColor1u32(ShadowColour); 
            } else {
                if (GS_MaterialSourceEmissive != 0) {
                    GXColor1u32(diff); 
                } else {
                    GXColor1u32((u32)GS_CurrentMaterialEmissivergba);
                }
            }
            GXTexCoord2f32(((struct _GS_VERTEX*)vertlist)[*(s16*)index].u,((struct _GS_VERTEX*)vertlist)[*(s16*)index].v);
    
}

//NGC MATCH
void GS_DrawIndexedTriStrip(u32 VertexCount,short *pIndexData,float *vertlist,s32 stride) {
    s32 j;
    s32 i;

    i = VertexCount;
    DBTimerStart(0x19);
    if (GS_CurrentVertDesc != 0x82) {
        GS_CurrentVertDesc = 0x82;
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS,GX_DIRECT);
        GXSetVtxDesc(GX_VA_NRM,GX_DIRECT);
        GXSetVtxDesc(GX_VA_CLR0,GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0,GX_DIRECT);
    }
    for (i = VertexCount; i != 0; i--) {
            GXBegin(GX_TRIANGLESTRIP,GX_VTXFMT2,*pIndexData);
            for (j = *pIndexData++; j > 0; j--) {
                SetVertexIndex(pIndexData, vertlist, stride);
                pIndexData += 1;
                i -= 1;
            }
    }
    DBTimerEnd(0x19);
}

//NGC MATCH
void GS_DrawIndexedTriList(u32 nverts,short *indexlist,float *vertlist,s32 stride) {
    s32 i;

    DBTimerStart(0x18);
    if ((s32) GS_CurrentVertDesc != 0x82) {
        GS_CurrentVertDesc = 0x82;
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS,GX_DIRECT);
        GXSetVtxDesc(GX_VA_NRM,GX_DIRECT);
        GXSetVtxDesc(GX_VA_CLR0,GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0,GX_DIRECT);
    }
    GXBegin(GX_TRIANGLES,GX_VTXFMT2,nverts);
    
    for (i = 0; i < nverts; i++) {
            SetVertexIndex(indexlist, vertlist, stride);
            indexlist++;
    }
    DBTimerEnd(0x18);
}

//NGC MATCH
void TTLLights(void) {
  GXSetChanAmbColor(GX_COLOR0A0,GXWhite);
  GXSetChanMatColor(GX_COLOR0A0,GXWhite);
  GXSetChanCtrl(GX_COLOR0A0,0,GX_SRC_VTX,GX_SRC_REG,0,GX_DF_NONE,GX_AF_NONE);
  GXSetNumChans(1);
  return;
}

//NGC MATCH
static inline void SetVertexTTL(struct _GS_VERTEXTL * vert) {
    GXPosition3f32(vert->x, vert->y, vert->z);
            GXColor4u8(
                ((vert->diffuse >> 0x10) == 0) ? 0 : 1,
                ((vert->diffuse >> 0x8) == 0) ? 0 : 1,
                ((vert->diffuse) == 0) ? 0 : 1,
                ((vert->diffuse >> 0x18) == 0) ? 0 : 1
            );
          GXTexCoord2f32(vert->u, vert->v);    
}

//NGC MATCH
void GS_DrawTriStripTTL(struct _GS_VERTEXTL *vertlist,int nverts) {
  s32 i;
  
  if (GS_CurrentVertDesc != 0x81) {
    GS_CurrentVertDesc = 0x81;
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS,GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0,GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0,GX_DIRECT);
  }
  TTLLights();
  GXBegin(GX_TRIANGLESTRIP,GX_VTXFMT1,(u16)nverts);
  for (i = 0; i < nverts; i++) {
      SetVertexTTL(&vertlist[i]);
  }
  return;
}

//NGC MATCH
void GS_DrawTriListTTL(struct _GS_VERTEXTL* vertlist, s16 nverts) {
    s32 i;

    if (GS_CurrentVertDesc != 0x81) {
        GS_CurrentVertDesc = 0x81;
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    }
    TTLLights();
    GXBegin(GX_TRIANGLES, GX_VTXFMT1, nverts);
    for (i = 0; i < nverts; i++) {
        GXPosition3f32(vertlist[i].x, vertlist[i].y, vertlist[i].z);
        GXColor4u8(
            ((vertlist[i].diffuse >> 0x10) == 0) ? 0 : 1,
            ((vertlist[i].diffuse >> 0x8) == 0) ? 0 : 1,
            ((vertlist[i].diffuse >> 0x0) == 0) ? 0 : 1,
            ((vertlist[i].diffuse >> 0x18) == 0) ? 0 : 1
        );
        GXTexCoord2f32(vertlist[i].u, vertlist[i].v);
    }
}
*/

//NGC MATCH
void GS_DrawQuadListBeginBlock(int nverts,int arg1) {
  if (arg1 != 0) {
    GS_SetLightingNone();
  }
  else {
    TTLLights();
  }
  GXSetChanCtrl(GX_COLOR0A0,0,GX_SRC_REG,GX_SRC_VTX,0,GX_DF_NONE,GX_AF_NONE);
  QBlockverts = nverts;
  if (GS_CurrentVertDesc != 0x81) {
    GS_CurrentVertDesc = 0x81;
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS,GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0,GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0,GX_DIRECT);
  }
  GXBegin(GX_QUADS,GX_VTXFMT1,(u16)nverts);
  return;
}

//NGC MATCH
void GS_DrawQuadListSetVert(struct _GS_VECTOR3 *pos,float u,float v) {
  GXPosition3f32(pos->x,pos->y,pos->z);
  GXColor1u32(QuadListColour);
  GXTexCoord2f32(u,v);
  return;
}

void GS_DrawQuadListEndBlock(void)

{
  return;
}

u32 QuadListColour;

//NGC MATCH
void GS_SetQuadListRGBA(s32 r,s32 g,s32 b,s32 a) {
  QuadListColour = r << 0x18 | g << 0x10 | b << 8 | a;
  return;
}

//NGC MATCH
void GS_DrawTriListTSkin(struct _GS_VERTEXNORM *vertlist,s32 nverts,struct _GS_VERTEXSKIN *srcverts,short *pIndexData) {
  s32 i;
  
  DBTimerStart(0x1a);
  if (GS_EnableLightingFlag != 0) {
    if (GS_CurrentVertDesc != 0x82) {
      GS_CurrentVertDesc = 0x82;
      GXClearVtxDesc();
      GXSetVtxDesc(GX_VA_POS,GX_DIRECT);
      GXSetVtxDesc(GX_VA_NRM,GX_DIRECT);
      GXSetVtxDesc(GX_VA_CLR0,GX_DIRECT);
      GXSetVtxDesc(GX_VA_TEX0,GX_DIRECT);
    }
//  GXSetArray(10,&srcverts->nx,0x38); //alpha version
//  GXSetArray(0xd,&srcverts->u,0x38); //alpha version
    GXBegin(GX_TRIANGLES,GX_VTXFMT2,(u16)nverts);
      for (i = 0; i < nverts; i++) {
      GXPosition3f32(vertlist->x,vertlist->y,vertlist->z);
      GXNormal3f32(srcverts[pIndexData[i]].nx,srcverts[pIndexData[i]].ny,srcverts[pIndexData[i]].nz);
      GXColor1u32(srcverts[pIndexData[i]].diffuse);
      GXTexCoord2f32(srcverts[pIndexData[i]].u, srcverts[pIndexData[i]].v);
      vertlist++;
      }
  }
  else {
    if (GS_CurrentVertDesc != 0x80) {
      GS_CurrentVertDesc = 0x80;
      GXClearVtxDesc();
      GXSetVtxDesc(GX_VA_POS,GX_DIRECT);
      GXSetVtxDesc(GX_VA_CLR0,GX_DIRECT);
    }
    GXBegin(GX_TRIANGLES,GX_VTXFMT7,(u16)nverts);
      for (i = 0; i < nverts; i++) {
          GXPosition3f32(vertlist->x,vertlist->y,vertlist->z);
          GXColor1u32(ShadowColour);
          vertlist++;
      }
  }
  DBTimerEnd(0x1a);
  return;
}



/*********from melee decomp***************/

/*

void C_MTXPerspective(Mtx44 m, f32 fovY, f32 aspect, f32 n, f32 f)
{
    f32 temp_f3;
    f32 temp_f4;

    fovY = 0.5F * fovY;

    temp_f4 = 1.0F / tanf(0.017453292F * (fovY));
    temp_f3 = 1.0F / (f - n);

    m[0][0] = temp_f4 / aspect;
    m[0][1] = 0.0F;
    m[0][2] = 0.0F;
    m[0][3] = 0.0F;

    m[1][0] = 0.0F;
    m[1][1] = temp_f4;
    m[1][2] = 0.0F;
    m[1][3] = 0.0F;

    m[2][0] = 0.0F;
    m[2][1] = 0.0F;
    m[2][2] = -n * temp_f3;
    m[2][3] = temp_f3 * -(f * n);

    m[3][0] = 0.0F;
    m[3][1] = 0.0F;
    m[3][2] = -1.0F;
    m[3][3] = 0.0F;
}


void C_MTXOrtho(Mtx44 m, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f)
{
    f32 temp_f8;
    f32 temp_f10;
    f32 temp_f4;

    temp_f10 = 1.0F / (r - l);
    m[0][0] = 2.0F * temp_f10;
    m[0][1] = 0.0F;
    m[0][2] = 0.0F;
    m[0][3] = temp_f10 * -(r + l);

    temp_f8 = 1.0F / (t - b);
    m[1][0] = 0.0F;
    m[1][1] = 2.0F * temp_f8;
    m[1][2] = 0.0F;
    m[1][3] = temp_f8 * -(t + b);

    temp_f4 = 1.0F / (f - n);
    m[2][0] = 0.0F;
    m[2][1] = 0.0F;
    m[2][2] = -1.0F * temp_f4;
    m[2][3] = -f * temp_f4;

    m[3][0] = 0.0F;
    m[3][1] = 0.0F;
    m[3][2] = 0.0F;
    m[3][3] = 1.0F;
}
*/
