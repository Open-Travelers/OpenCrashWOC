#include "system/skinning.h"

struct _GS_VERTEXTL* TVertices; //xform.c
struct _GS_VERTEXPSTL* TVertices2;//pointspr.c

void GS_SetVertexSource(float *vertexptr) {
    GS_SkinVertexSource = (struct _GS_VERTEXSKIN *)vertexptr;
    return;
}

void GS_SetBlendSource(struct _GS_VECTOR3 *vertexptr) {
    GS_BlendSource = vertexptr;
    return;
}

//vtx --> _GS_VERTEXTL/_GS_VERTEXNORM

//NGC MATCH
void VecMatMulAndWeight1(_GS_VERTEXNORM *vtx,_GS_VECTOR3 *source,_GSMATRIX *mtx) {
    asm ("psq_l 0,0(4),0,0");
    asm ("psq_l 2, 0(5), 0, 0");
    asm ("psq_l 1, 8(4), 1, 0");
    asm ("ps_mul 4, 2, 0");
    asm ("psq_l 3,8(5),0,0");
    asm ("ps_madd 5, 3, 1, 4");
    asm ("psq_l 8,16(5),0,0");
    asm("ps_sum0 6, 5, 6, 5");
    asm ("psq_l 9,24(5),0,0");
    asm ("ps_mul 10, 8, 0");
    asm ("psq_st 6, 0(3), 1, 0");
    asm ("ps_madd 11, 9, 1, 10");
    asm ("psq_l 2,32(5),0,0");
    asm ("ps_sum0 12, 11, 12, 11");
    asm ("psq_l 3,40(5),0,0");
    asm ("ps_mul 4, 2, 0");
    asm ("psq_st 12, 4(3), 1, 0");
    asm ("ps_madd 5, 3, 1, 4");
    asm ("ps_sum0 6, 5, 6, 5");
    asm ("psq_st 6, 8(3), 1, 0");
    asm("blr");
    asm ("psq_l 13,0(6),1,0");
    asm ("psq_l 0,0(4),0,0");
    asm ("psq_l 2,0(5),0,0");
    asm ("psq_l 1,8(4),1,0");
    asm ("ps_mul 4, 2, 0");
    asm ("psq_l 3,8(5),0,0");
    asm ("ps_madd 5, 3, 1, 4");
    asm ("psq_l 8,16(5),0,0");
    asm ("ps_sum0 6, 5, 6, 5");
    asm ("psq_l 9,24(5),0,0");
    asm ("ps_mul 10, 8, 0");
    asm ("psq_l 14,0(3),1,0");
    asm ("ps_madd 6, 6, 13, 14");
    asm ("psq_st 6, 0(3), 1, 0");
    asm ("ps_madd 11, 9, 1, 10");
    asm ("psq_l 2,32(5),0,0");
    asm ("ps_sum0 12, 11, 12, 11");
    asm ("psq_l 3,40(5),0,0");
    asm ("ps_mul 4, 2, 0");
    asm ("psq_l 14,4(3),1,0");
    asm ("ps_madd 12, 12, 13, 14");
    asm ("psq_st 12, 4(3), 1, 0");
    asm ("ps_madd 5, 3, 1, 4");
    asm ("ps_sum0 6, 5, 6, 5");
    asm ("psq_l 14,8(3),1,0");
    asm ("ps_madd 6, 6, 13, 14");
    asm ("psq_st 6, 8(3), 1, 0");
}

//arg0 --> _GS_VERTEXTL/_GS_VERTEXNORM
void VecMatMulAndWeight3(struct nuvec_s *arg0,float *inputvert,_GSMATRIX *cvskinmtx1,
                        _GSMATRIX *cvskinmtx2,_GSMATRIX *cvskinmtx3,float *weights,float *c_one) {

    asm ("psq_l 13,0(8),1,0");
    asm ("lfs 20,0(9)");
    asm ("psq_l 0, 0(4), 0, 0");
    asm ("lfs 21,0(8)");
    asm ("psq_l 2, 0(5), 0, 0");
    asm ("psq_l 1, 8(4), 1, 0");
    asm ("ps_mul 4, 2, 0");
    asm ("psq_l 3,8(5),0,0");
    asm ("ps_madd 5, 3, 1, 4");
    asm ("psq_l 8,16(5),0,0");
    asm ("ps_sum0 6, 5, 6, 5");
    asm ("psq_l 9,24(5),0,0");
    asm ("ps_mul 10, 8, 0");
    asm ("ps_mul 14, 6, 13");
    asm ("ps_madd 11, 9, 1, 10");
    asm ("psq_l 2,32(5),0,0");
    asm ("ps_sum0 12, 11, 12, 11");
    asm ("psq_l 3,40(5),0,0");
    asm ("ps_mul 4, 2, 0");
    asm ("ps_mul 15, 12, 13");
    asm ("ps_madd 5, 3, 1, 4");
    asm ("ps_sum0 6, 5, 6, 5");
    asm ("ps_mul 16, 6, 13");
    asm ("psq_l 13,4(8),1,0");
    asm ("lfs 22,4(8)");
    asm ("psq_l 2,0(6),0,0");
    asm ("fadds 21,21,22");
    asm ("ps_mul 4, 2, 0");
    asm ("psq_l 3,8(6),0,0");
    asm ("ps_madd 5, 3, 1, 4");
    asm ("psq_l 8,16(6),0,0");
    asm ("ps_sum0 6, 5, 6, 5");
    asm ("psq_l 9,24(6),0,0");
    asm ("ps_mul 10, 8, 0");
    asm ("ps_madd 14, 6, 13, 14");
    asm ("ps_madd 11, 9, 1, 10");
    asm ("psq_l 2,32(6),0,0");
    asm ("ps_sum0 12, 11, 12, 11");
    asm ("psq_l 3,40(6),0,0");
    asm ("ps_mul 4, 2, 0");
    asm ("ps_madd 15, 12, 13, 15");
    asm ("ps_madd 5, 3, 1, 4");
    asm ("ps_sum0 6, 5, 6, 5");
    asm ("ps_madd 16, 6, 13, 16");
    asm ("fsubs 13,20,21");
    asm ("psq_l 2,0(7),0,0");
    asm ("ps_mul 4, 2, 0");
    asm ("psq_l 3,8(7),0,0");
    asm ("ps_madd 5, 3, 1, 4");
    asm ("psq_l 8,16(7),0,0");
    asm ("ps_sum0 6, 5, 6, 5");
    asm ("psq_l 9,24(7),0,0");
    asm ("ps_mul 10, 8, 0");   
    asm ("ps_madd 14, 6, 13, 14");
    asm ("psq_st 14, 0(3), 1, 0");
    asm ("ps_madd 11, 9, 1, 10");
    asm ("psq_l 2,32(7),0,0");
    asm ("ps_sum0 12, 11, 12, 11");
    asm ("psq_l 3,40(7),0,0");
    asm ("ps_mul 4, 2, 0");
    asm ("ps_madd 15, 12, 13, 15");
    asm ("psq_st 15, 4(3), 1, 0");
    asm ("ps_madd 5, 3, 1, 4");
    asm ("ps_sum0 6, 5, 6, 5");
    asm ("ps_madd 16, 6, 13, 16");
    asm ("psq_st 16, 8(3), 1, 0");
    asm ("blr");
    asm ("psq_l 13,0(7),1,0");
    asm ("lfs 20,0(8)");
    asm ("lfs 21,0(7)");
    asm ("psq_l 0,0(4),0,0");
    asm ("psq_l 2,0(5),0,0");
    asm ("psq_l 1,8(4),1,0");
    asm ("ps_mul 4, 2, 0");
    asm ("psq_l 3,8(5),0,0");
    asm ("ps_madd 5, 3, 1, 4");
    asm ("psq_l 8,16(5),0,0");
    asm ("ps_sum0 6, 5, 6, 5");
    asm ("psq_l 9,24(5),0,0");
    asm ("ps_mul 10, 8, 0");
    asm ("ps_mul 14, 6, 13");
    asm ("ps_madd 11, 9, 1, 10");
    asm ("psq_l 2,32(5),0,0");
    asm ("ps_sum0 12, 11, 12, 11");
    asm ("psq_l 3,40(5),0,0");
    asm ("ps_mul 4, 2, 0");
    asm ("ps_mul 15, 12, 13");
    asm ("ps_madd 5, 3, 1, 4");
    asm ("ps_sum0 6, 5, 6, 5");
    asm ("ps_mul 16, 6, 13");
    asm ("fsubs 13,20,21");
    asm ("psq_l 2,0(6),0,0");
    asm ("ps_mul 4, 2, 0");
    asm ("psq_l 3,8(6),0,0");
    asm ("ps_madd 5, 3, 1, 4");
    asm ("psq_l 8,16(6),0,0"); 
    asm ("ps_sum0 6, 5, 6, 5");
    asm ("psq_l 9,24(6),0,0");
    asm ("ps_mul 10, 8, 0");
    asm ("ps_madd 14, 6, 13, 14");
    asm ("psq_st 14, 0(3), 1, 0");
    asm ("ps_madd 11, 9, 1, 10");
    asm ("psq_l 2,32(6),0,0"); 
    asm ("ps_sum0 12, 11, 12, 11");
    asm ("psq_l 3,40(6),0,0");
    asm ("ps_mul 4, 2, 0");
    asm ("ps_madd 15, 12, 13, 15");
    asm ("psq_st 15, 4(3), 1, 0");
    asm ("ps_madd 5, 3, 1, 4");
    asm ("ps_sum0 6, 5, 6, 5");    
    asm ("ps_madd 16, 6, 13, 16");
    asm ("psq_st 16, 8(3), 1, 0");
}

void SkinnedShader(int VertexCount,short *pIndexData)
{
    struct _GS_VERTEXSKIN *inputvert;
    short *IndexPtr;
    int i;
    struct _GS_VERTEXNORM *outputvert;
      char pad[6];

    outputvert = (struct _GS_VERTEXNORM *)TVertices;
    DBTimerStart(0x1b);
    IndexPtr = pIndexData;
    if (VertexCount > MaxSkinVerts) {
        MaxSkinVerts = VertexCount;
    }
        for(i = 0;  i < VertexCount; i++, outputvert++, IndexPtr++) {
            inputvert = &GS_SkinVertexSource[(int)*IndexPtr];
            if (inputvert->weights[0] == 1.0f) {
                    //ASM FUNCTION
                //VecMatMulAndWeight1(outputvert,inputvert,
                //                   &CV_SKINMTX[(int)inputvert->indexes[0]]);
            }
            else {
                    //ASM FUNCTION
                //VecMatMulAndWeight3(outputvert,inputvert,
                   //                 &CV_SKINMTX[(int)inputvert->indexes[0]],
                    //                &CV_SKINMTX[(int)inputvert->indexes[1]],
                    //                &CV_SKINMTX[(int)inputvert->indexes[2]],
                     //               inputvert->weights,&c_one);
            }
        }
    DBTimerEnd(0x1b);
    SkinLights = 1;
    GS_DrawTriListTSkin(TVertices,VertexCount,GS_SkinVertexSource,pIndexData);
    SkinLights = 0;
    return;
}



void SkinnedShaderBlend(int param_1,short *param_2)
{
  struct _GS_VERTEXSKIN *inputvert;
  short *IndexPtr;
  int i;
  struct _GS_VERTEXNORM* outputvert;
    char pad[6];

  outputvert = (struct _GS_VERTEXNORM*)TVertices;
  DBTimerStart(0x1c);
  IndexPtr = param_2;
  if (param_1 > MaxSkinVerts) {
    MaxSkinVerts = param_1;
  }
        for(i = 0;  i < param_1; i++, outputvert++, IndexPtr++) {
              inputvert = GS_SkinVertexSource + *IndexPtr;
            if (inputvert->weights[0] == 1.0f) {
                    //ASM FUNCTION
               // VecMatMulAndWeight1(outputvert,(struct _GS_VERTEXSKIN*)&GS_BlendSource[*IndexPtr],
                 //                   &CV_SKINMTX[(int)inputvert->indexes[0]]);
            }
            else {
                //ASM FUNCTION
               // VecMatMulAndWeight3(outputvert,(struct _GS_VERTEXSKIN*)&GS_BlendSource[*IndexPtr],
                     //               &CV_SKINMTX[(int)inputvert->indexes[0]],
                      //              &CV_SKINMTX[(int)inputvert->indexes[1]],
                     //               &CV_SKINMTX[(int)inputvert->indexes[2]],
                      //              inputvert->weights,&c_one);
            }

        }
  DBTimerEnd(0x1c);
  SkinLights = 1;
  GS_DrawTriListTSkin(TVertices,param_1,GS_SkinVertexSource,param_2);
  SkinLights = 0;
  return;
}

