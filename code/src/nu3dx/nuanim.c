#include "nuanim.h"
#include "../system.h"

#define ASSIGN_IF_SET(a, b) a = (a == NULL) ? NULL : b

#define PI 3.1415927f
#define TAU 6.2831855f
#define FLOAT_MAX 3.4028235e38f
#define MAX_FIXED_POINT 65536
#define DEG_TO_FIXED_POINT (MAX_FIXED_POINT * (1 / (2 * PI)))

static unsigned char BitCountTable[256];
static int isBitCountTable;

void buildBitCountTable(void) //check NGC asm	//PS2
{
    s32 i, j;
     	for(i = 0; i < 256; i++){
            	BitCountTable[i] = 0;
            	for(j = 0; j < 8; j++) {
                	if (((i >> j) & 1) != 0) {
                    	BitCountTable[i]++;
                	}
            	}
        	}
        	isBitCountTable = 1;
}

void okdokey(void) {
  return;
}

//PS2 Match
struct nuanimdata_s * NuAnimDataFixPtrs(struct nuanimdata_s *animdata,s32 address_offset)
{
    s32 i;
    s32 j;
    s32 iNodes;
    s32 iChunks;
    s32 iCurves;    //nnodes counter related?
    struct nuanimcurve_s *curve;
    struct nuanimcurveset_s *curveset;
    struct nuanimdatachunk_s *datachunk;

    if (isBitCountTable == 0) {
	buildBitCountTable();
    }

    ASSIGN_IF_SET(animdata, (struct nuanimdata_s *)((s32)animdata + address_offset));
    ASSIGN_IF_SET(animdata->node_name, animdata->node_name + address_offset);
    ASSIGN_IF_SET(animdata->chunks, ((s32)animdata->chunks + address_offset));

    if (animdata->chunks != NULL) {
        for (iChunks = 0; iChunks < animdata->nchunks; iChunks++)
        {
            ASSIGN_IF_SET(animdata->chunks[iChunks], (struct nuanimdatachunk_s *)((s32)animdata->chunks[iChunks] + address_offset));
            datachunk = animdata->chunks[iChunks];
            if (datachunk == NULL) {
                continue;
            }

            ASSIGN_IF_SET(datachunk->animcurvesets, (struct nuanimcurveset_s **)((s32)datachunk->animcurvesets + address_offset));
            if (datachunk->animcurvesets == NULL) {
                continue;
            }

            for (iNodes = 0; iNodes < datachunk->numnodes; iNodes++)
            {
                ASSIGN_IF_SET(datachunk->animcurvesets[iNodes], (struct nuanimcurveset_s *)((s32)datachunk->animcurvesets[iNodes] + address_offset));
                curveset = datachunk->animcurvesets[iNodes];
                if (curveset == NULL) {
                    continue;
                }

                ASSIGN_IF_SET(curveset->constants, (float *)((s32)curveset->constants + address_offset));
                ASSIGN_IF_SET(curveset->set, (struct nuanimcurve_s **)((s32)curveset->set + address_offset));
                if (curveset->set == NULL) {
                    continue;
                }

                for(iCurves = 0; iCurves < curveset->ncurves; iCurves++)
                {
                    ASSIGN_IF_SET(curveset->set[iCurves], (struct nuanimcurve_s *)((s32)curveset->set[iCurves] + address_offset));
                    curve = curveset->set[iCurves];
                    if (curve != NULL) {
                        ASSIGN_IF_SET(curve->animkeys, (struct nuanimkey_s *)((s32)curve->animkeys + address_offset));
                    }
                }
            }
        }
    }
    return animdata;
}

//PS2 Match
struct nuanimdata_s * NuAnimDataLoadBuff(char *file,union variptr_u *buff,union variptr_u *endbuff)
{
  s32 addroff;
  s32 tmp;
  struct nuanimdata_s *data;
  void *mem;
  struct NUANIMDATAHDR_s *dathdr;

  mem = (((s32)buff->voidptr + 0xf) & 0xfffffff0);
  buff->voidptr = mem;
  addroff = NuFileLoadBuffer(file,mem,(s32)endbuff->voidptr - (s32)mem);
  if (addroff == 0) {
    NuErrorProlog("..\\nu2.ps2\\nu3d\\nuanim.c",0x2f1)("Could not read from <%s>, either file is not there or the buffer may be full",file);
  }

  dathdr = (struct NUANIMDATAHDR_s *)buff->voidptr;
  buff->voidptr = (void *)((s32)&dathdr->version + addroff);
  tmp = (s32)dathdr - dathdr->address_offset;
  dathdr->address_offset = tmp;
  data = NuAnimDataFixPtrs(dathdr->animdata,tmp);
  dathdr->animdata = data;
  return data;
}


//PS2 Match
struct nuanimdata2_s* NuAnimData2FixPtrs(struct nuanimdata2_s* animdata, s32 address_offset)
{
    s32 i;
    s32 j;
    s32 k;
    int totncurves;
    struct nuanimcurve2_s *curve;

    if (isBitCountTable == 0) {
        for(i = 0; i < 256; i++) {
            BitCountTable[i] = 0;
            for(j = 0; j < 8; j++) {
                if (((i >> j) & 1) != 0) {
                    BitCountTable[i]++;
                }
            }
        }
        isBitCountTable = 1;
    }

    ASSIGN_IF_SET(animdata, (struct nuanimdata2_s *)((s32)animdata + address_offset));

    if (animdata != NULL) {
        totncurves = animdata->nnodes * animdata->ncurves;

        ASSIGN_IF_SET(animdata->curves, ((s32)animdata->curves + address_offset));
        ASSIGN_IF_SET(animdata->curveflags, ((s32)animdata->curveflags + address_offset));
        ASSIGN_IF_SET(animdata->curvesetflags, ((s32)animdata->curvesetflags + address_offset));

        for (k = 0; k < totncurves; k++)
        {
            if (animdata->curveflags[k] != '\0') {
                curve = &animdata->curves[k];
                ASSIGN_IF_SET(curve->data.curvedata, (int)curve->data.curvedata + address_offset);
                ASSIGN_IF_SET(curve->data.curvedata->mask, (s32)curve->data.curvedata->mask + address_offset);
                ASSIGN_IF_SET(curve->data.curvedata->key_ixs, (s32)curve->data.curvedata->key_ixs + address_offset);
                ASSIGN_IF_SET(curve->data.curvedata->key_array, (s32)curve->data.curvedata->key_array + address_offset);
            }
        }
    }
    return animdata;
}


//PS2 Match
struct nuanimcurveset_s * NuAnimCurveSetCreate(s32 ncurves)
{
  struct nuanimcurveset_s *animcurveset;
  s32 nbytes;

  animcurveset = NULL;
  if (ncurves != 0) {
    nbytes = ncurves << 2;
    animcurveset = (struct nuanimcurveset_s *)NuMemAlloc(0x10);		//animcurveset = (struct nuanimcurveset_s *)NuMemAllocFn(0x10,"..\\nu2.ps2\\nu3d\\nuanim.c",0x531);
    memset(animcurveset,0,0x10);
    animcurveset->ncurves = (char)ncurves;
    animcurveset->set = (struct nuanimcurve_s **)NuMemAlloc(nbytes); 	//animcurveset->set = (struct nuanimcurve_s **)NuMemAllocFn(nbytes,"..\\nu2.ps2\\nu3d\\nuanim.c",0x538);
    memset(animcurveset->set,0,nbytes);
    animcurveset->constants = (float *)NuMemAlloc(nbytes);		//animcurveset->constants = (float *)NuMemAllocFn(nbytes,"..\\nu2.ps2\\nu3d\\nuanim.c",0x53d);
    memset(animcurveset->constants,0,nbytes);
  }
  return animcurveset;
}

//PS2 Match
struct nuanimdatachunk_s * NuAnimDataChunkCreate (s32 numnodes) {
  struct nuanimdatachunk_s *animdata;

  if (numnodes < 1) {
    NuErrorProlog("..\\nu2.ps2\\nu3d\\nuanim.c",0x275)("assert");
  }
  animdata = (struct nuanimdatachunk_s *)NuMemAlloc(0x14);	//animdata = (struct nuanimdatachunk_s *)NuMemAllocFn(0x14,"..\\nu2.ps2\\nu3d\\nuanim.c",0x276);

  memset(animdata,0,0x14);
  animdata->numnodes = numnodes;
  animdata->animcurvesets = (struct nuanimcurveset_s **)NuMemAlloc(numnodes << 2);

//animdata->animcurvesets = (struct nuanimcurveset_s **)NuMemAllocFn(numnodes << 2,"..\\nu2.ps2\\nu3d\\nuanim.c",0x27a);

  memset(animdata->animcurvesets,0,(long)(numnodes << 2));
  return animdata;
}

struct nuanimdata_s * NuAnimDataCreate(s32 nchunks)
{
  struct nuanimdata_s *animdata;

  animdata = (struct nuanimdata_s *)NuMemAlloc(0x10);
  memset(animdata,0,0x10);
  animdata->chunks = (struct nuanimdatachunk_s **)NuMemAlloc(nchunks << 2);
  memset(animdata->chunks,0,nchunks << 2);
  animdata->nchunks = nchunks;
  return animdata;
}

//PS2 Match
struct nuanimcurve_s * NuAnimCurveCreate(s32 numkeys)
{
  struct nuanimcurve_s *animcurve;

  if (numkeys < 1) {
     NuErrorProlog("..\\nu2.ps2\\nu3d\\nuanim.c",0x438)("assert");
  }
  animcurve = (struct nuanimcurve_s *)NuMemAlloc(0x10);		//animcurve = (struct nuanimcurve_s *)NuMemAllocFn(0x10,"..\\nu2.ps2\\nu3d\\nuanim.c",0x43a);
  memset(animcurve,0,0x10);
  animcurve->numkeys = numkeys;
  animcurve->animkeys = (struct nuanimkey_s *)NuMemAlloc(numkeys << 4);      //animcurve->animkeys = (struct nuanimkey_s *)NuMemAllocFn(numkeys << 4,"..\\nu2.ps2\\nu3d\\nuanim.c",0x43e);
  memset(animcurve->animkeys,0,(numkeys << 4));
  return animcurve;
}

//PS2 Match
struct nuanimdata_s* NuAnimDataRead(s32 fh)
{
    s32 numnodes;
    s32 i;
    s32 j;
    s32 k;
    s32 nKeys;
    s32 nCurves;
    s32 bytes;
    s32 nChunks;
    struct nuanimdata_s *animdata;
    struct nuanimkey_s *keyptr;
    struct nuanimcurve_s *curveptr;
    s32 length;
    char *node_name;
    struct nuanimdatachunk_s *chunk;
    float time;

    if (isBitCountTable == 0) {
         for(i = 0; i < 256; i++){
            BitCountTable[i] = 0;
            for(j = 0; j < 8; j++) {
                if (((i >> j) & 1) != 0) {
                    BitCountTable[i]++;
                }
            }
        }
        isBitCountTable = 1;
    }

    k = NuFileReadInt(fh);
    if (k != 0) {
        node_name = (char *)NuMemAlloc(k);	//node_name = (char *)NuMemAllocFn(k, "..\\nu2.ps2\\nu3d\\nuanim.c", 0x184);
        NuFileRead(fh, node_name, k);
    }
    else {
        node_name = NULL;
    }

    time = NuFileReadFloat(fh);
    animdata = NuAnimDataCreate(NuFileReadInt(fh));
    animdata->time = time;
    animdata->node_name = node_name;

    for (nChunks = 0; nChunks < animdata->nchunks; nChunks++) {
        numnodes = NuFileReadInt(fh);
        chunk = NuAnimDataChunkCreate(numnodes);
        animdata->chunks[nChunks] = chunk;
        chunk->numnodes = numnodes;
        bytes = NuFileReadInt(fh);
        if (bytes != 0) {
            chunk->keys = (struct nuanimkey_s *)NuMemAlloc(bytes << 4);		//chunk->keys = (struct nuanimkey_s *)NuMemAllocFn(bytes << 4, "..\\nu2.ps2\\nu3d\\nuanim.c", 0x19E);
            NuFileRead(fh, chunk->keys, bytes << 4);
        }
        else {
            chunk->keys = NULL;
        }
        keyptr = chunk->keys;
        bytes = NuFileReadInt(fh);
        if (bytes != 0) {
            chunk->curves = (struct nuanimcurve_s *)NuMemAlloc(bytes << 4);	//chunk->curves = (struct nuanimcurve_s *)NuMemAllocFn(bytes << 4, "..\\nu2.ps2\\nu3d\\nuanim.c", 0x1AB);
            NuFileRead(fh, chunk->curves, bytes << 4);
        }
        else {
            chunk->curves = NULL;
        }

        for (nKeys = 0; nKeys < numnodes; nKeys++) {
            length = NuFileReadChar(fh);
            if (length != 0) {
                chunk->animcurvesets[nKeys] = NuAnimCurveSetCreate(length);
                chunk->animcurvesets[nKeys]->flags = NuFileReadInt(fh);
                for (nCurves = 0; nCurves < chunk->animcurvesets[nKeys]->ncurves; nCurves++) {
                    chunk->animcurvesets[nKeys]->constants[nCurves] = NuFileReadFloat(fh);
                }
            }
        }

        curveptr = chunk->curves;
        for(nKeys = 0; nKeys < chunk->numnodes; nKeys++) {
            if (chunk->animcurvesets[nKeys] == NULL) {
                continue;
            }

            for (nCurves = 0; nCurves < chunk->animcurvesets[nKeys]->ncurves; nCurves++) {
                if (chunk->animcurvesets[nKeys]->constants[nCurves] == FLOAT_MAX) {
                    chunk->animcurvesets[nKeys]->set[nCurves] = curveptr;
                    chunk->num_valid_animcurvesets++;
                    curveptr->animkeys = keyptr;
                    keyptr += *&curveptr->numkeys;
                    curveptr++;
                }
            }
        }
    }
    return animdata;
}


void NuAnimCurveDestroy(struct nuanimcurve_s *curve)
{
  if (curve->animkeys != NULL) {
    NuMemFree(curve->animkeys);
  }
  NuMemFree(curve);
  return;
}

void NuAnimCurveSetDestroy(struct nuanimcurveset_s* animcurveset, s32 destroy_curves) //CHECK
{
    s32 var_r29;
    s32 i;
    struct nuanimcurve_s* curve;

  /*  if ((animcurveset != NULL) && ((struct nuanimcurve_s** ) animcurveset->set != NULL) && (destroy_curves != 0)) {
        i = 0;
        if ((s8) (u8) animcurveset->ncurves > 0) {
            var_r29 = 0;
            do {
                curve = *(animcurveset->set[var_r29]);
                if (curve != NULL) {
                    NuAnimCurveDestroy(curve);
                }
                i += 1;
                var_r29 += 4;
            } while (i < (s8) (u8) animcurveset->ncurves);
        }
    }*/
}

void NuAnimDataDestroy(struct nuanimdata_s *animdata)

{
  struct nuanimdatachunk_s **chunk;
  s32 i;

  if (((animdata != NULL) &&
      (chunk = animdata->chunks, chunk != NULL)) &&
     (i = 0, 0 < animdata->nchunks)) {
    while( true ) {
      NuAnimDataChunkDestroy(chunk[i]);
      if (animdata->nchunks <= i + 1) break;
      chunk = animdata->chunks;
      i = i + 1;
    }
  }
  return;
}

void NuAnimDataChunkDestroy(struct nuanimdatachunk_s *animdata)
{
  struct nuanimcurve_s *destroycurves;
  struct nuanimcurveset_s *animcurveset;
  s32 nnodes;
  s32 i;
/*
  destroycurves = animdata->curves;
  nnodes = animdata->numnodes;
  if (0 < nnodes) {
    do {
      i = 0;
      if (0 < nnodes) {
        nnodes = 0;
        do {
          animcurveset = *(struct nuanimcurveset_s **)(nnodes + (int)animdata->animcurveset);
          if (animcurveset != NULL) {
            NuAnimCurveSetDestroy(animcurveset,(u32)(destroycurves == NULL));
          }
          i = i + 1;
          nnodes = nnodes + 4;
        } while (i < animdata->numnodes);
      }
      nnodes = animdata->numnodes;
    } while (i + 1 < nnodes);
  }*/
  return;
}

//PS2 96%
void NuAnimDataCalcTime(struct nuanimdata_s *animdata,float time,struct nuanimtime_s *atime)
{
    s32 iVar1;
    s32 dVar2;
    s32 iVar3;

    if (animdata->time <= time) {
        if (animdata->time == 1.0f) {
          atime->time = 1.0f;
          atime->time_byte = 0;
          atime->time_mask = 1;
          atime->chunk = 0;
          return;
        }
        atime->time = animdata->time - 0.01f;
    }
    else if (time < 1.0f) {
        atime->time = 1.0f;
    }
    else {
        atime->time = time;
    }

    atime->chunk = floor((atime->time - 1.0) / 32.0);
    if (animdata->nchunks <= atime->chunk) {
        atime->chunk = animdata->nchunks - 1;
    }

    atime->time_offset = atime->time - atime->chunk * 32;
    iVar1 = floor(atime->time_offset);
    iVar1--;

    atime->time_byte = (u8)(iVar1 / 8);
    atime->time_mask = (u8)((1 << (iVar1 + (iVar1 / 8) * -8 + 1)) - 1);
    return;
}

//PS2
void NuAnimData2CalcTime(struct nuanimdata2_s *animdata,float time,struct nuanimtime_s *atime)
{
    s32 iVar1;

    if (animdata->endframe <= time) {
        if (animdata->endframe == 1.0f) {
            atime->time = 1.0f;
            atime->time_mask = 1;
            atime->time_byte = 0;
            atime->chunk = 0;
            return;
        }
        atime->time = animdata->endframe - 0.01f;
    }
    else if (time < 1.0f) {
        atime->time = 1.0f;
    }
    else {
        atime->time = time;
    }

    atime->chunk = floor((atime->time - 1.0) / 32.0);
    if (animdata->nchunks <= atime->chunk) {
        atime->chunk = animdata->nchunks - 1;
    }

    atime->time_offset = atime->time - atime->chunk * 32;
    iVar1 = floor(atime->time_offset);
    iVar1--;

    atime->time_byte = (u8)(iVar1 / 8);
    atime->time_mask = (u8)((1 << (iVar1 % 8) + 1) - 1);
    return;
}

//NGC 97%
float NuAnimCurve2CalcVal(struct nuanimcurve2_s* animcurve, struct nuanimtime_s* atime, enum NUANIMKEYTYPES_e keytype)
{
    u8 *mask;
    u32 chunk_start_ix;
    u32 offset;
    s32 poopoo;
    struct NUANIMKEYBIG_s *nextkey;
    struct NUANIMKEYBIG_s *key;
    float val;
    float dt;
    float fVar8;
    float fVar11;
    float time;
    struct NUANIMKEYINTEGER_s* inextkey;
    struct NUANIMKEYINTEGER_s* ikey;
    s32 frame;
    u32 *ixtmp;

    if (keytype == NUANIMKEYTYPE_NONE) {
        return animcurve->data.constant;
    }

    chunk_start_ix = atime->chunk;
    //mask = &animcurve->data.curvedata->mask[chunk_start_ix];
    mask = (u8*)&animcurve->data.curvedata->mask[chunk_start_ix];

    if (keytype == NUANIMKEYTYPE_BOOLEAN) {
        frame = floor(atime->time_offset); // gcc2_compiled__N108
        frame--;
        poopoo = 0;
        if ((*(s32*)&mask[0] != 0) && (1 << frame)) {
            poopoo = 1;
        }
        return poopoo;
    }

    switch (atime->time_byte) {
        case 0:
            offset =  BitCountTable[mask[0] & atime->time_mask];
            break;
        case 1:
            offset =  BitCountTable[mask[0]];
            offset += BitCountTable[mask[1] & atime->time_mask];
            break;
        case 2:
            offset =  BitCountTable[mask[0]];
            offset += BitCountTable[mask[1]];
            offset += BitCountTable[mask[2] & atime->time_mask];
            break;
        case 3:
            offset =  BitCountTable[mask[0]];
            offset += BitCountTable[mask[1]];
            offset += BitCountTable[mask[2]];
            offset += BitCountTable[mask[3] & atime->time_mask];
            break;
    }

    ixtmp = animcurve->data.curvedata->key_ixs[chunk_start_ix];
    switch(keytype) {
        case NUANIMKEYTYPE_NONE:
            NuErrorProlog("C:/source/crashwoc/code/nu3dx/nuanim.c",0xbbb)
                ("NuAnimCurve2CalcVal: should have already evaluated NUANIMKEYTYPE_NONE");
            break;
        case NUANIMKEYTYPE_BIG:
            key = &((struct NUANIMKEYBIG_s *)animcurve->data.curvedata->key_array)[(s32)ixtmp + offset - 1];
            nextkey = &key[1];
            if(atime->time > key->val  || atime->time < key->time){
                okdokey();
            }
            val = key->val - nextkey->val;
            fVar8 = key->grad * (nextkey->time - key->time);
            dt = nextkey->grad * (nextkey->time - key->time);
            time = (atime->time - key->time) * key->dtime;
            return time * (time * (((time * (val + val + fVar8 + dt) + val * -3.0f) - (fVar8 + fVar8)) - dt) + fVar8) + key->val;
        case NUANIMKEYTYPE_SMALL:
            NuErrorProlog("C:/source/crashwoc/code/nu3dx/nuanim.c",0xbdd)
                ("NuAnimCurve2CalcVal: not supporting NUANIMKEYTYPE_SMALL yet");
            break;
        case NUANIMKEYTYPE_INTEGER:
            ikey = &((struct NUANIMKEYINTEGER_s *)animcurve->data.curvedata->key_array)[(s32)ixtmp + offset - 1];
            inextkey = &ikey[1];
             if(atime->time > inextkey->time  || atime->time < ikey->time){
                okdokey();
            }
            return ikey->val;
        case NUANIMKEYTYPE_BOOLEAN:
            NuErrorProlog("C:/source/crashwoc/code/nu3dx/nuanim.c",0xbec)
                ("NuAnimCurve2CalcVal: should have already evaluated NUANIMKEYTYPE_BOOLEAN");
    }
    return 0.0f;
}


//PS2
void NuAnimCurve2SetApplyToJoint(struct nuanimcurve2_s* animcurveset, char* curveflags,
char curvesetflags, struct nuanimtime_s* atime, struct NUJOINTDATA_s* jointdata,
struct nuvec_s* scale, struct nuvec_s* parentscale, struct numtx_s* T, struct NUJOINTANIM_s* offset)
{
    char procanim_flags;
    struct nuvec_s t;
    struct nuvec_s r;
    struct nuvec_s lo;
    struct nuangvec_s rf;
    struct nuvec_s inv_scale;

    if (offset != NULL) {
        procanim_flags = offset->flags;
    }
    else {
        procanim_flags = 0;
    }

    if((curvesetflags & 1) || (procanim_flags & 1)) {
        if (curvesetflags & 1) {
            r.x = NuAnimCurve2CalcVal(&animcurveset[3], atime, (int)curveflags[3]);
            r.y = NuAnimCurve2CalcVal(&animcurveset[4], atime, (int)curveflags[4]);
            r.z = NuAnimCurve2CalcVal(&animcurveset[5], atime, (int)curveflags[5]);
        } else {
            r.x = r.y = r.z = 0.0f;
        }

        if (procanim_flags & 1U) {
            r.x += offset->rx;
            r.y += offset->ry;
            r.z += offset->rz;
            rf.x = (int)(r.x * DEG_TO_FIXED_POINT);
            rf.y = (int)(r.y * DEG_TO_FIXED_POINT);
            rf.z = (int)(r.z * DEG_TO_FIXED_POINT);
            if ((procanim_flags & 8U) != 0) {
                rf.x &= 0xFFFF;
                if (0x7fff < rf.x) {
                    rf.x -= 0x10000;
                }
                if (offset->max_rx < rf.x) {
                    rf.x = (int)offset->max_rx;
                }
                else if (rf.x < offset->min_rx) {
                    rf.x = (int)offset->min_rx;
                }
            }

            if ((procanim_flags & 0x10) != 0) {
                rf.y &= 0xFFFF;
                if (0x7fff < rf.y) {
                    rf.y -= 0x10000;
                }
                if (offset->max_ry < rf.y) {
                    rf.y = (int)offset->max_ry;
                }
                else if (rf.y < offset->min_ry) {
                    rf.y = (int)offset->min_ry;
                }
            }

            if ((procanim_flags & 0x20) != 0) {
                rf.z &= 0xFFFF;
                if (0x7fff < rf.z) {
                    rf.z -= 0x10000;
                }
                if (offset->max_rz < rf.z) {
                    rf.z = (int)offset->max_rz;
                }
                else if (rf.z < offset->min_rz) {
                    rf.z = (int)offset->min_rz;
                }
            }
        } else {
            rf.x = (int)(r.x * DEG_TO_FIXED_POINT);
            rf.y = (int)(r.y * DEG_TO_FIXED_POINT);
            rf.z = (int)(r.z * DEG_TO_FIXED_POINT);
        }
        NuMtxSetRotateXYZ(T, &rf); //NuMtxSetRotateXYZVU0
    }
    else
    {
        NuMtxSetIdentity(T);
    }

    if ((curvesetflags & 0x20U) != 0) {
        NuMtxMulR(T, T, &jointdata->orient); //NuMtxMulRVU0
    }

    if((curvesetflags & 8) || (procanim_flags & 4)) {
        if (curvesetflags & 8) {
            scale->x = NuAnimCurve2CalcVal(&animcurveset[6], atime, (int)curveflags[6]);
            scale->y = NuAnimCurve2CalcVal(&animcurveset[7], atime, (int)curveflags[7]);
            scale->z = NuAnimCurve2CalcVal(&animcurveset[8], atime, (int)curveflags[8]);
        } else {
            scale->x = scale->y = scale->z = 0.0f;
        }

        if ((procanim_flags & 4U)) {
            scale->x += offset->sx;
            scale->y += offset->sy;
            scale->z += offset->sz;
        }
        NuMtxPreScale(T, scale); //NuMtxPreScaleVU0
        scale->x *= parentscale->x;
        scale->y *= parentscale->y;
        scale->z *= parentscale->z;
    }
    else {
        *scale = *parentscale;
    }

    if ((curvesetflags & 0x10U)  && (parentscale != 0)) {
        inv_scale.x = 1.0f / parentscale->x;
        inv_scale.y = 1.0f / parentscale->y;
        inv_scale.z = 1.0f / parentscale->z;
        NuMtxScale(T, &inv_scale);
        scale->x *= inv_scale.x;
        scale->y *= inv_scale.y;
        scale->z *= inv_scale.z;
    }

    t.x = NuAnimCurve2CalcVal(&animcurveset[0], atime, (int)curveflags[0]);
    t.y = NuAnimCurve2CalcVal(&animcurveset[1], atime, (int)curveflags[1]);
    t.z = NuAnimCurve2CalcVal(&animcurveset[2], atime, (int)curveflags[2]);

    if ((procanim_flags & 2U) != 0) {
        t.x += offset->tx;
        t.y += offset->ty;
        t.z += offset->tz;
    }

    NuMtxTranslate(T, &t);
    if ((jointdata->flags & 8U) != 0) {
        NuMtxPreTranslate(T, &jointdata->locator_offset);
        lo.x = -(jointdata->locator_offset).x;
        lo.y = -(jointdata->locator_offset).y;
        lo.z = -(jointdata->locator_offset).z;
        NuMtxTranslate(T,&lo);
    }

    T->_02 = -T->_02;
    T->_12 = -T->_12;
    T->_20 = -T->_20;
    T->_21 = -T->_21;
    T->_23 = -T->_23;
    T->_32 = -T->_32;
    return;
}


//NGC MATCH
f32 NuAnimCurveCalcVal2(struct nuanimcurve_s* animcurve, struct nuanimtime_s* atime) {
    struct nuanimkey_s* key;
    struct nuanimkey_s* prevKey;
    float dt;
    float time;
    float f10;
    float fVar2;
    float fVar3;
    float fVar4;
    float fVar5;
    int offset;
    u8* test;

    test = (u8*)&animcurve->mask; // ???
    switch (atime->time_byte) {
        case 0:
            offset = BitCountTable[(animcurve->mask >> 0x18) & atime->time_mask];
            break;
        case 1:
            offset = BitCountTable[(animcurve->mask >> 0x18)];
            offset += BitCountTable[test[1] & atime->time_mask];
            // offset += BitCountTable[((animcurve->mask & 0x00FF0000) >> 0x10) & atime->time_mask];
            break;
        case 2:
            offset = BitCountTable[(animcurve->mask >> 0x18)];
            offset += BitCountTable[test[1]];
            offset += BitCountTable[test[2] & atime->time_mask];
            // offset += BitCountTable[((animcurve->mask >> 0x8) & 0x000000FF) & atime->time_mask];
            break;
        case 3:
            offset = BitCountTable[(animcurve->mask >> 0x18)];
            offset += BitCountTable[test[1]];
            offset += BitCountTable[test[2]];
            offset += BitCountTable[test[3] & atime->time_mask];
            // offset += BitCountTable[(animcurve->mask & 0xFF) & atime->time_mask];
            break;
        default:
            offset = 0;
            break;
    }
    prevKey = animcurve->animkeys + offset - 1;
    key = animcurve->animkeys + offset;
    fVar4 = (atime->time - prevKey->time) * prevKey->dtime;
    dt = key->time - prevKey->time;
    f10 = key->c * dt;
    fVar3 = prevKey->c * dt;
    fVar2 = prevKey->d - key->d;
    return fVar4
        * (fVar4 * (((fVar4 * (fVar2 + fVar2 + fVar3 + f10) + fVar2 * -3.0f) - (fVar3 + fVar3)) - f10) + fVar3)
        + prevKey->d;
}

//PS2 Version
void NuAnimCurveSetApplyToJointBasic (struct nuanimcurveset_s *animcurveset, struct nuanimtime_s *atime,
struct NUJOINTDATA_s *jointdata, struct nuvec_s *scale,struct nuvec_s *parentscale,
struct numtx_s *T, struct NUJOINTANIM_s *offset)
{
  //UNKTYPE *puVar1;
  //unsigned long *puVar2;
  //unsigned long uVar3;
  char procanim_flags;
  //float fVar4;
  //u32 uVar5;
  //float fVar6;
  //float fVar7;
  //u32 uvar;
  //u32 uVar1;
  struct nuvec_s r;
  struct nuangvec_s rf;
  //float fVar2;

  if (offset != NULL) {
    procanim_flags = offset->flags;
  }
  else {
    procanim_flags = '\0';
  }
  if (animcurveset->set[3] != NULL) {
      r.x = NuAnimCurveCalcVal2(animcurveset->set[3],atime);
  }
  else {
    r.x = animcurveset->constants[3];
  }
  if (animcurveset->set[4] != NULL) {
    r.y = NuAnimCurveCalcVal2(animcurveset->set[4],atime);
  }
  else {
      r.y = animcurveset->constants[4];
  }
  if (animcurveset->set[5] != NULL) {
    r.z = NuAnimCurveCalcVal2(animcurveset->set[5],atime);
  }
  else {
    r.z = animcurveset->constants[5];
  }
  if ((procanim_flags & 1) != 0)  {
    r.x += offset->rx;
    r.y += offset->ry;
    r.z += offset->rz;
      rf.x = (int)(r.x * 10430.378f);
      rf.y = (int)(r.y * 10430.378f);
      rf.z = (int)(r.z * 10430.378f);

    if ((procanim_flags & 8) != 0) {
        rf.x &= 0xFFFF;
      if (0x7fff < rf.x) {
        rf.x -= 0x10000;
      }
      if (offset->max_rx < rf.x) {
        rf.x = offset->max_rx;
      }
      else {
        //uvar = rf.x;
        if (rf.x < offset->min_rx) {
          rf.x = offset->min_rx;
        }
      }
    }
    //rf.x = uvar;
    //rf.y._0_2_ = (u16)uVar5;
    if ((procanim_flags & 0x10) != 0) {
      rf.y &= 0xffff;
      if (0x7fff < rf.y) {
        rf.y -= 0x10000;
      }
      if (offset->max_ry < rf.y) {
        rf.y = offset->max_ry;
      }
      else {
        //rf.y = rf.y;
        if (rf.y < offset->min_ry) {
          rf.y = offset->min_ry;
        }
      }
    }
    //rf.y = uVar5;
    if ((procanim_flags & 0x20) != 0) {
    //uvar = rf.z & 0xffff;
      rf.z &= 0xFFFF;
      if (0x7fff < rf.z) {
        rf.z -= 0x10000;
      }
      if ((long)offset->max_rz < (long)rf.z) {
        rf.z = (int)offset->max_rz;
      }
      else if ((long)rf.z < (long)offset->min_rz) {
        rf.z = (int)offset->min_rz;
      }
    }
  }
  else {                                        //if ((procanim_flags & 1) == 0)
    rf.x = (int)(r.x * 10430.378f);
    rf.y = (int)(r.y * 10430.378f);
    rf.z = (int)(r.z * 10430.378f);
  }
  NuMtxSetRotateXYZ(T,&rf);
  if ((animcurveset->flags & 0x20U) != 0) {
    NuMtxMulR(T,T,&jointdata->orient);
  }
  T->_30 = animcurveset->constants[0];
  T->_31 = animcurveset->constants[1];
  T->_32 = animcurveset->constants[2];
  if ((procanim_flags & 2) != 0) {
    T->_30 += offset->tx;
    T->_31 += offset->ty;
    T->_32 += offset->tz;
  }
    //memcpy
  *scale = *parentscale;    //or -->  memcpy ( parentscale, scale, sizeof(struct nuvec_s) );
  T->_02 = -T->_02;
  T->_12 = -T->_12;
  T->_20 = -T->_20;
  T->_21 = -T->_21;
  T->_23 = -T->_23;
  T->_32 = -T->_32;
  return;
}

//PS2 99,45% match
void NuAnimCurveSetApplyToJoint2
               (struct nuanimcurveset_s *animcurveset,struct nuanimtime_s *atime,
                struct NUJOINTDATA_s *jointdata,struct nuvec_s *scale ,
               struct nuvec_s *parentscale,struct numtx_s *T, struct NUJOINTANIM_s *offset)
{
    struct nuvec_s t;
    struct nuvec_s r;
    struct nuvec_s lo;
    struct nuangvec_s rf;
    char procanim_flags;
    struct nuvec_s inv_scale;

    if (offset != NULL) {
        procanim_flags = offset->flags;
    }
    else {
        procanim_flags = 0x00;
    }

    if ((animcurveset->flags & 1) || (procanim_flags & 1))
    {
        if ((animcurveset->flags & 1)) {
            if (animcurveset->set[3] != NULL) {
            r.x = NuAnimCurveCalcVal2(animcurveset->set[3], atime);
            }
            else {
                r.x = animcurveset->constants[3];
            }

            if (animcurveset->set[4] != NULL) {
                r.y = NuAnimCurveCalcVal2(animcurveset->set[4], atime);
            }
            else {
                r.y = animcurveset->constants[4];
            }

            if (animcurveset->set[5] != NULL) {
                r.z = NuAnimCurveCalcVal2(animcurveset->set[5], atime);
            }
            else {
                r.z = animcurveset->constants[5];
            }
        }
        else {
            r.x = r.y = r.z = 0.0f;
        }

        if(procanim_flags & 1) {
            r.x += offset->rx;
            r.y += offset->ry;
            r.z += offset->rz;
            rf.x = (int)(r.x * DEG_TO_FIXED_POINT);
            rf.y = (int)(r.y * DEG_TO_FIXED_POINT);
            rf.z = (int)(r.z * DEG_TO_FIXED_POINT);

            if ((procanim_flags & 8U) != 0) {
                rf.x &= 0xFFFF;
                if (0x7fff < rf.x) {
                    rf.x -= 0x10000;
                }
                if (offset->max_rx < rf.x) {
                    rf.x = (int)offset->max_rx;
                }
                else if (rf.x < offset->min_rx) {
                    rf.x = (int)offset->min_rx;
                }
            }

            if ((procanim_flags & 0x10) != 0) {
                rf.y &= 0xFFFF;
                if (0x7fff < rf.y) {
                    rf.y -= 0x10000;
                }
                if (offset->max_ry < rf.y) {
                    rf.y = (int)offset->max_ry;
                }
                else if (rf.y < offset->min_ry) {
                    rf.y = (int)offset->min_ry;
                }
            }

            if ((procanim_flags & 0x20) != 0) {
                rf.z &= 0xFFFF;
                if (0x7fff < rf.z) {
                    rf.z -= 0x10000;
                }
                if (offset->max_rz < rf.z) {
                    rf.z = (int)offset->max_rz;
                }
                else if (rf.z < offset->min_rz) {
                    rf.z = (int)offset->min_rz;
                }
            }
        } else {
            rf.x = (int)(r.x * DEG_TO_FIXED_POINT);
            rf.y = (int)(r.y * DEG_TO_FIXED_POINT);
            rf.z = (int)(r.z * DEG_TO_FIXED_POINT);
        }
        NuMtxSetRotateXYZ(T, &rf);//NuMtxSetRotateXYZVU0
    } else {
        NuMtxSetIdentity(T);
    }

    if ((animcurveset->flags & 0x20) != 0) {
        NuMtxMulR(T, T, &jointdata->orient); //NuMtxMulRVU0
    }

    if ((animcurveset->flags & 8) || (procanim_flags & 4)) {
        if((animcurveset->flags & 8)) {
            if (animcurveset->set[6] != NULL) {
                scale->x = NuAnimCurveCalcVal2(animcurveset->set[6], atime);
            }
            else {
                scale->x = animcurveset->constants[6];
            }
            if (animcurveset->set[7] != NULL) {
                scale->y = NuAnimCurveCalcVal2(animcurveset->set[7], atime);
            }
            else {
                scale->y = animcurveset->constants[7];
            }
            if (animcurveset->set[8] != NULL) {
                scale->z = NuAnimCurveCalcVal2(animcurveset->set[8], atime);
            }
            else {
                scale->z = animcurveset->constants[8];
            }
        } else {
            scale->x = scale->y = scale->z = 0.0f;
        }

        if ((procanim_flags & 4)) {
            scale->x += offset->sx;
            scale->y += offset->sy;
            scale->z += offset->sz;
        }
        NuMtxPreScale(T, scale); //NuMtxPreScaleVU0
        scale->x *= parentscale->x;
        scale->y *= parentscale->y;
        scale->z *= parentscale->z;
    } else {
        *scale = *parentscale;
    }

    if ((animcurveset->flags & 0x10U)  && (parentscale != 0)) {
        inv_scale.x = 1.0f / parentscale->x;
        inv_scale.y = 1.0f / parentscale->y;
        inv_scale.z = 1.0f / parentscale->z;
        NuMtxScale(T, &inv_scale); //NuMtxScaleVU0
        scale->x *= inv_scale.x;
        scale->y *= inv_scale.y;
        scale->z *= inv_scale.z;
    }

    if (animcurveset->set[0] != NULL) {
        t.x = NuAnimCurveCalcVal2(animcurveset->set[0], atime);
    }
    else {
        t.x = animcurveset->constants[0];
    }
    if (animcurveset->set[1] != NULL) {
        t.y = NuAnimCurveCalcVal2(animcurveset->set[1], atime);
    }
    else {
        t.y = animcurveset->constants[1];
    }
    if (animcurveset->set[2] != NULL) {
        t.z = NuAnimCurveCalcVal2(animcurveset->set[2], atime);
    }
    else {
        t.z = animcurveset->constants[2];
    }

    if ((procanim_flags & 2) != 0) {
        t.x = t.x + offset->tx;
        t.y = t.y + offset->ty;
        t.z = t.z + offset->tz;
    }

    NuMtxTranslate(T, &t);
    if ((jointdata->flags & 8) != 0) {
        NuMtxPreTranslate(T, &jointdata->locator_offset);
        lo.x = -(jointdata->locator_offset).x;
        lo.y = -(jointdata->locator_offset).y;
        lo.z = -(jointdata->locator_offset).z;
        NuMtxTranslate(T,&lo);
    }
    T->_02 = -T->_02;
    T->_12 = -T->_12;
    T->_20 = -T->_20;
    T->_21 = -T->_21;
    T->_23 = -T->_23;
    T->_32 = -T->_32;
    return;
}


//PS2 Match
void NuAnimCurveSetApplyBlendToJoint2
               (struct nuanimcurveset_s *animcurveset1,struct nuanimtime_s *atime1,
                struct nuanimcurveset_s *animcurveset2,
               struct nuanimtime_s *atime2,float blend,struct NUJOINTDATA_s *jointdata,struct nuvec_s *scale,
               struct nuvec_s *parentscale,struct numtx_s *T,struct NUJOINTANIM_s *offset)

{
    struct nuvec_s lo;
    struct nuangvec_s rf;
    float r[3];
    float r1[3];
    float r2[3];
    float s1[3];
    float s2[3];
    float t[3];
    float omblend;
    s32 i;
    char procanim_flags;
    struct nuvec_s inv_scale;


    omblend = 1.0f - blend;
    if (offset != NULL) {
        procanim_flags = offset->flags;
    }
    else {
        procanim_flags = 0;
    }

    if ((animcurveset1->flags & 1) || (animcurveset2->flags & 1) || (procanim_flags & 1))
    {
        for (i = 3; i < 6; i++) {

            if (animcurveset1->set[i] != NULL) {
                r1[i - 3] = NuAnimCurveCalcVal2(animcurveset1->set[i], atime1);
            }
            else {
                r1[i - 3] = animcurveset1->constants[i];
            }

            if (animcurveset2->set[i] != NULL) {
                r2[i - 3] = NuAnimCurveCalcVal2(animcurveset2->set[i], atime2);
            }
            else {
                r2[i - 3] = animcurveset2->constants[i];
            }
        }

        r1[0] -= (r1[0] - r2[0] > PI) ? TAU : 0.0f;
        r2[0] -= (r2[0] - r1[0] > PI) ? TAU : 0.0f;
        r1[1] -= (r1[1] - r2[1] > PI) ? TAU : 0.0f;
        r2[1] -= (r2[1] - r1[1] > PI) ? TAU : 0.0f;
        r1[2] -= (r1[2] - r2[2] > PI) ? TAU : 0.0f;
        r2[2] -= (r2[2] - r1[2] > PI) ? TAU : 0.0f;

        r[0] = (r1[0] * omblend) + (r2[0] * blend);
        r[1] = (r1[1] * omblend) + (r2[1] * blend);
        r[2] = (r1[2] * omblend) + (r2[2] * blend);
        if ((procanim_flags & 1) != 0) {
            r[0] += offset->rx;
            r[1] += offset->ry;
            r[2] += offset->rz;
            rf.x = (s32)(r[0] * DEG_TO_FIXED_POINT);
            rf.y = (s32)(r[1] * DEG_TO_FIXED_POINT);
            rf.z = (s32)(r[2] * DEG_TO_FIXED_POINT);
            if ((procanim_flags & 8U) != 0) {
                rf.x &= 0xFFFF;
                if (0x7fff < rf.x) {
                    rf.x -= 0x10000;
                }
                if (offset->max_rx < rf.x) {
                      rf.x = offset->max_rx;
                }
                else if (rf.x < offset->min_rx) {
                      rf.x = offset->min_rx;
                }
            }

            if ((procanim_flags & 0x10U) != 0) {
                rf.y &= 0xffff;
                if (0x7fff < rf.y) {
                    rf.y -= 0x10000;
                }
                if (offset->max_ry < rf.y) {
                    rf.y = offset->max_ry;
                }
                else if (rf.y < offset->min_ry) {
                    rf.y = offset->min_ry;
                }
            }

            if ((procanim_flags & 0x20U) != 0) {
                rf.z &= 0xFFFF;
                if (0x7fff < rf.z) {
                    rf.z -= 0x10000;
                }
                if (offset->max_rz < rf.z) {
                    rf.z = offset->max_rz;
                }
                else if (rf.z < offset->min_rz) {
                    rf.z = offset->min_rz;
                }
            }
        }
        else {
            rf.x = (s32)(r[0] * DEG_TO_FIXED_POINT);
            rf.y = (s32)(r[1] * DEG_TO_FIXED_POINT);
            rf.z = (s32)(r[2] * DEG_TO_FIXED_POINT);
        }

        NuMtxSetRotateXYZ(T, &rf); //NuMtxSetRotateXYZVU0
    }
    else {
        NuMtxSetIdentity(T);
    }

    if ((animcurveset1->flags & 0x20) || (animcurveset2->flags & 0x20)) {
        NuMtxMulR(T, T, &jointdata->orient); //NuMtxMulRVU0
    }

    if ((animcurveset1->flags & 8) || (animcurveset2->flags & 8) || (procanim_flags & 4)) {
        for (i = 6; i < 9; i++) {

            if (animcurveset1->set[i] != NULL) {
                s1[i - 6] = NuAnimCurveCalcVal2(animcurveset1->set[i], atime1);
            }
            else {
                s1[i - 6] = animcurveset1->constants[i];
            }

            if (animcurveset2->set[i] != NULL) {
                s2[i - 6] = NuAnimCurveCalcVal2(animcurveset2->set[i], atime2);
            }
            else {
                s2[i - 6] = animcurveset2->constants[i];
            }
        }

        scale->x = (s1[0] * blend) + (s2[0] * omblend);
        scale->y = (s1[1] * blend) + (s2[1] * omblend);
        scale->z = (s1[2] * blend) + (s2[2] * omblend);

        if ((procanim_flags & 4) != 0) {
            scale->x += offset->sx;
            scale->y += offset->sy;
            scale->z += offset->sz;
        }
        NuMtxPreScale(T,scale); //NuMtxPreScaleVU0
        scale->x *= parentscale->x;
        scale->y *= parentscale->y;
        scale->z *= parentscale->z;
    } else {
        *scale = *parentscale;
    }

    if (((animcurveset1->flags | animcurveset2->flags) & 0x10) != 0) {
        inv_scale.x = 1.0f / parentscale->x;
        inv_scale.y = 1.0f / parentscale->y;
        inv_scale.z = 1.0f / parentscale->z;
        NuMtxScale(T, &inv_scale); //NuMtxScaleVU0
        scale->x *= inv_scale.x;
        scale->y *= inv_scale.y;
        scale->z *= inv_scale.z;
    }

    for (i = 0; i < 3; i++) {
        if (animcurveset1->set[i] != NULL) {
            t[i] = omblend * NuAnimCurveCalcVal2(animcurveset1->set[i], atime1);
        }
        else {
            t[i] = omblend * animcurveset1->constants[i];
        }
        if (animcurveset2->set[i] != NULL) {
            t[i] += blend * NuAnimCurveCalcVal2(animcurveset2->set[i], atime2);
        }
        else {
            t[i] += blend * animcurveset2->constants[i];
        }
    }

    if ((procanim_flags & 2) != 0) {
        t[0] += offset->tx;
        t[1] += offset->ty;
        t[2] += offset->tz;
    }

    NuMtxTranslate(T, &t);
    if ((jointdata->flags & 8) != 0) {
        NuMtxPreTranslate(T, &jointdata->locator_offset);
        lo.x = -(jointdata->locator_offset).x;
        lo.y = -(jointdata->locator_offset).y;
        lo.z = -(jointdata->locator_offset).z;
        NuMtxTranslate(T, &lo);
    }

    T->_02 = -T->_02;
    T->_12 = -T->_12;
    T->_20 = -T->_20;
    T->_21 = -T->_21;
    T->_23 = -T->_23;
    T->_32 = -T->_32;
    return;
}


//PS2 Match
void NuAnimCurveSetApplyToMatrix (struct nuanimcurveset_s *animcurveset,struct nuanimtime_s *atime,struct numtx_s *T)
{
  s32 uVar1;
  struct nuvec_s local_80;
  struct nuvec_s local_70;
  struct nuangvec_s rf;
  struct nuvec_s local_50;


  if ((animcurveset->flags & 1U) != 0)  {
    if (animcurveset->set[3] != NULL) {
      local_70.x = NuAnimCurveCalcVal2(animcurveset->set[3],atime);
    }
    else {
        local_70.x = animcurveset->constants[3];
    }
    if (animcurveset->set[4] != NULL) {
      local_70.y = NuAnimCurveCalcVal2(animcurveset->set[4],atime);
    }
    else {
      local_70.y = animcurveset->constants[4];
    }
    if (animcurveset->set[5] != NULL) {
      local_70.z = NuAnimCurveCalcVal2(animcurveset->set[5],atime);
    }
    else {
      local_70.z = animcurveset->constants[5];
    }
    rf.x = (int)(local_70.x * 10430.378f);
    rf.y = (int)(local_70.y * 10430.378f);
    rf.z = (int)(local_70.z * 10430.378f);
    NuMtxSetRotateXYZ(T,&rf);
    //procanim_alt1:
  }
  else{
    NuMtxSetIdentity(T);
  }

  if ((animcurveset->flags & 8) != 0) {
    if (animcurveset->set[6] != NULL) {
      local_50.x = NuAnimCurveCalcVal2(animcurveset->set[6],atime);
    }
    else {
      local_50.x = animcurveset->constants[6];
    }
    if (animcurveset->set[7] != NULL) {
      local_50.y = NuAnimCurveCalcVal2(animcurveset->set[7],atime);
    }
    else {
      local_50.y = animcurveset->constants[7];
    }
    if (animcurveset->set[8] != NULL) {
      local_50.z = NuAnimCurveCalcVal2(animcurveset->set[8],atime);
    }
    else {
      local_50.z = animcurveset->constants[8];
    }
    NuMtxPreScale(T,&local_50);
  }
  if (*animcurveset->set != NULL) {
    local_80.x = NuAnimCurveCalcVal2(*animcurveset->set,atime);
  }
  else {
    local_80.x = *animcurveset->constants;
  }
  if (animcurveset->set[1] != NULL) {
    local_80.y = NuAnimCurveCalcVal2(animcurveset->set[1],atime);
  }
  else {
    local_80.y = animcurveset->constants[1];
  }
  if (animcurveset->set[2] != NULL) {
    local_80.z = NuAnimCurveCalcVal2(animcurveset->set[2],atime);
  }
  else {
    local_80.z = animcurveset->constants[2];
  }
  NuMtxTranslate(T,&local_80);
  T->_02 = -T->_02;
  T->_12 = -T->_12;
  T->_20 = -T->_20;
  T->_21 = -T->_21;
  T->_23 = -T->_23;
  T->_32 = -T->_32;
  return;
}

//PS2 Match
void NuAnimCurve2SetApplyToMatrix (struct nuanimcurve2_s *animcurveset,char *curveflags,char curvesetflags,
struct nuanimtime_s *atime, struct numtx_s *T)
{
  struct nuvec_s local_a0;
  struct nuvec_s local_90;
  struct nuangvec_s rf;
  struct nuvec_s local_70;

  if ((curvesetflags & 1U) != 0) {
    local_90.x = NuAnimCurve2CalcVal(animcurveset + 3,atime,(int)curveflags[3]);
    local_90.y = NuAnimCurve2CalcVal(animcurveset + 4,atime,(int)curveflags[4]);
    local_90.z = NuAnimCurve2CalcVal(animcurveset + 5,atime,(int)curveflags[5]);
    rf.z = (s32)(local_90.z * 10430.378f);
    rf.x = (s32)(local_90.x * 10430.378f);
    rf.y = (s32)(local_90.y * 10430.378f);
    NuMtxSetRotateXYZ(T,&rf);
  }
  else {
      NuMtxSetIdentity(T);
  }
  if ((curvesetflags & 8U) != 0) {
    local_70.x = NuAnimCurve2CalcVal(animcurveset + 6,atime,(int)curveflags[6]);
    local_70.y = NuAnimCurve2CalcVal(animcurveset + 7,atime,(int)curveflags[7]);
    local_70.z = NuAnimCurve2CalcVal(animcurveset + 8,atime,(int)curveflags[8]);
    NuMtxPreScale(T,&local_70);
  }
  local_a0.x = NuAnimCurve2CalcVal(animcurveset,atime,(int)*curveflags);
  local_a0.y = NuAnimCurve2CalcVal(animcurveset + 1,atime,(int)curveflags[1]);
  local_a0.z = NuAnimCurve2CalcVal(animcurveset + 2,atime,(int)curveflags[2]);
  NuMtxTranslate(T,&local_a0);
  T->_02 = -T->_02;
  T->_12 = -T->_12;
  T->_20 = -T->_20;
  T->_21 = -T->_21;
  T->_23 = -T->_23;
  T->_32 = -T->_32;
  return;
}


//PS2 Match
void NuAnimCurve2SetApplyToJointBasic(struct nuanimcurve2_s *animcurveset, char* curveflags, char curvesetflags,
struct nuanimtime_s *atime, struct NUJOINTDATA_s *jointdata, struct nuvec_s *scale,
struct nuvec_s *parentscale, struct numtx_s* T, struct NUJOINTANIM_s* offset)
{
  char procanim_flags;
  struct nuvec_s r;
  float tmp;
  float tmp2;
  float tmp3;
  struct nuangvec_s rf;


  if (offset != NULL) {
    procanim_flags = offset->flags;
  }
  else {
        procanim_flags = 0;
    }
  r.x = NuAnimCurve2CalcVal(animcurveset + 3,atime,(int)curveflags[3]);
  r.y = NuAnimCurve2CalcVal(animcurveset + 4,atime,(int)curveflags[4]);
  r.z = NuAnimCurve2CalcVal(animcurveset + 5,atime,(int)curveflags[5]);
  if (procanim_flags & 1U) {
    r.x += offset->rx;
    r.y += offset->ry;
    r.z += offset->rz;
    rf.x = (int)(r.x * 10430.378f);
    rf.y = (int)(r.y * 10430.378f);
    rf.z = (int)(r.z * 10430.378f);
    if ((procanim_flags & 8U) != 0) {
        rf.x &= 0xFFFF;
        if (0x7fff < rf.x) {
            rf.x -= 0x10000;
        }
        if (offset->max_rx < rf.x) {
            rf.x = (int)offset->max_rx;
        }
        else if (rf.x < offset->min_rx) {
            rf.x = (int)offset->min_rx;
        }
    }
    if ((procanim_flags & 0x10) != 0) {
        rf.y &= 0xFFFF;
        if (0x7fff < rf.y) {
            rf.y -= 0x10000;
        }
        if (offset->max_ry < rf.y) {
            rf.y = (int)offset->max_ry;
        }
        else if (rf.y < offset->min_ry) {
            rf.y = (int)offset->min_ry;
        }
    }
    if ((procanim_flags & 0x20U) != 0) {
      rf.z &= 0xFFFF;
      if (0x7fff < rf.z) {
        rf.z -= 0x10000;
      }
      if (offset->max_rz < rf.z) {
        rf.z = (int)offset->max_rz;
      }
        else if (rf.z < offset->min_rz) {
          rf.z = (int)offset->min_rz;
        }
    }
  }
   else {
    rf.x = (int)(r.x * 10430.378f);
    rf.y = (int)(r.y * 10430.378f);
    rf.z = (int)(r.z * 10430.378f);
  }
  NuMtxSetRotateXYZ(T,&rf); //NuMtxSetRotateXYZVU0
  if ((curvesetflags & 0x20U) != 0) {
    NuMtxMulR(T,T,&jointdata->orient); //NuMtxMulRVU0
  }
  tmp = (animcurveset->data).constant;
  T->_30 = tmp;
    tmp2 = (animcurveset + 1)->data.constant;
  T->_31 = tmp2;
    tmp3 = (animcurveset + 2)->data.constant;
  T->_32 = tmp3;
  if ((procanim_flags & 2U) != 0) {
    T->_30 = tmp + offset->tx;
    T->_31 = tmp2 + offset->ty;
    T->_32 = tmp3 + offset->tz;
  }
    //memcpy
  *scale = *parentscale;

  T->_02 = -T->_02;
  T->_12 = -T->_12;
  T->_20 = -T->_20;
  T->_21 = -T->_21;
  T->_23 = -T->_23;
  T->_32 = -T->_32;
  return;
}
