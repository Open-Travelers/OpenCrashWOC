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
    //GS_DrawTriListTSkin(TVertices,VertexCount,GS_SkinVertexSource,pIndexData);
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
  //GS_DrawTriListTSkin(TVertices,param_1,GS_SkinVertexSource,param_2);
  SkinLights = 0;
  return;
}

