#include "../nu.h"

//NGC MATCH
s32 SolveQuadratic(float a, float b, float c, float* t1, float* t2) {
    float x;

    if (a == 0.0f) {
        if (b == 0.0f) {
            return 0;
        }
        *t1 = *t2 = -c / b;
        return 1;
    }
    
    x = (b * b);
    if ((a * 4.0f) * c > x) {
        return 0;
    }
    
    if ((a * 4.0f) * c == x) {
        *t1 = *t2 = (-b) / (a + a);
        return 1;
    } else {
        x = NuFsqrt(x - (a * 4.0f) * c);
        *t1 = (-b - x) / (a + a);
        *t2 = (-b + x) / (a + a);
        return 1;
    }
}

//NGC MATCH
void DebrisMalloc(void) {
  if (debbuffer == NULL) {
    debbuffer = (char *)malloc_x(0x93400);
  }
  return;
}

//NGC MATCH
s32 DebAlloc(void) {
  s32 key;
  
  if (freedebkeyptr > 0xff) {
   return -1;
  }
    key = (int)freedebkeys[freedebkeyptr];
    freedebkeyptr++;
    debkeydata[key].count = 0;
    debkeydata[key].debcount = 0;
    debkeydata[key].reqcount = 0;
    debkeydata[key].reqdebcount = 0;
    debkeydata[key].chunks[0] = NULL;
    return key;
}

//NGC MATCH
void DebReAlloc(struct debkeydatatype_s *debkey,s32 newdebcount) {
  s32 newchunksneeded;
  
  debkey->reqdebcount = (short)newdebcount;
  newchunksneeded = newdebcount + 0x1f;
  if (newchunksneeded < 0) {
    newchunksneeded = newdebcount + 0x3e;
  }
  debkey->reqcount = (newchunksneeded >> 5);
  if (debkey->reqcount == debkey->count) {
    debkey->debcount = (short)newdebcount;
  }
  else {
    DebReAlloc2(debkey);
  }
  return;
}

//93%
void DebrisStartOffset(int key,int offset) {
  struct debinftype *debinfo;
  int iVar1;
  int iVar3;
  
  if (key != -1) {
    debinfo = debtab[debkeydata[key].type];
    if ((debinfo->ival_on_ran != 0) || (debinfo->ival_off_ran != 0)) {
      debkeydata[key].delay = 0;
    }
    else {
      if (debinfo->gensort == 7) {
        debkeydata[key].delay = 0;
        debkeydata[key].rotory = (short)(offset * (debinfo->variable_emit).y);
        debkeydata[key].rotorz = (short)(offset * (debinfo->variable_emit).z);
      }
      iVar3 = (int)debinfo->ival_on + (int)debinfo->ival_off;
      iVar1 = (globalframes % iVar3) - offset;
      if (iVar1 != 0) {
        iVar1 = iVar3 - iVar1;
      }
      debkeydata[key].delay = (short)iVar1;
      debkeydata[key].oncount = debinfo->ival_on;
    }
  }
  return;
}

//NGC MATCH
void DebrisOrientation(s32 key,short rotz,short roty) {
  if (key != -1) {
    debkeydata[key].rotmtx = numtx_identity;
    NuMtxRotateZ(&debkeydata[key].rotmtx,rotz);
    NuMtxRotateY(&debkeydata[key].rotmtx,roty);
  }
  return;
}

//NGC MATCH
void AddDebrisEffectToStack(struct debkeydatatype_s *effect,struct debkeydatatype_s **stack) {
  struct debkeydatatype_s *rdat;
  
  if (effect->next != NULL) {
    effect->next = NULL;
  }
  while (*stack != NULL) {
    rdat = *stack;
    stack = &rdat->next;
  }
  *stack = effect;
  return;
}

//NGC MATCH
void AddVariableShotDebrisEffectMtx(s32 type,struct nuvec_s *pos,s32 numdeb,short emitrotz,short emitroty,struct numtx_s *rotmtx) {
  struct numtx_s tmtx;

  tmtx = numtx_identity;
  NuMtxRotateZ(&tmtx,emitrotz);
  NuMtxRotateY(&tmtx,emitroty);
  AddVariableShotDebrisEffectMtx2(type,pos,numdeb,&tmtx,rotmtx);
  return;
}

//NGC MATCH
void AddDebrisEffect(s32 *key,s32 type,float x,float y,float z) {
  s32 i;
  struct debinftype *debinfo;

  if (type != -1) {
    if (*key == -1) {
      *key = DebAlloc();
      if (*key == -1) {
        return;
      }
    }
    debinfo = debtab[type];
    debkeydata[*key].type = (short)type;
    debkeydata[*key].active = 1;
    DebrisStartOffset(*key,(s32)debinfo->ival_offset);
    debkeydata[*key].oncount = debtab[type]->ival_on + (s32)((randy() * debtab[type]->ival_on_ran) * 0.125); //4.656612873077393e-10 --> asm (lfd)
    debkeydata[*key].genptr = gensorttab[debinfo->gensort];
    debkeydata[*key].gencode = gencodetab[debinfo->gencode];
    debkeydata[*key].rotory = 0;
    debkeydata[*key].rotorz = 0;
    debkeydata[*key].sphere_next = 0;
    debkeydata[*key].sphere_next_emit = 1;
    for (i = 0; i < debtab[type]->numspheres; i++) {
        debkeydata[*key].spheres[i].t = 0xbf800000;
    }
    debkeydata[*key].toffx = 0.0f;
    debkeydata[*key].toffy = 0.0f;
    debkeydata[*key].toffz = 0.0f;
    DebrisEmiterPos(*key,x,y,z);
    DebrisEmitterOrientation(*key,0,0);
    DebrisOrientation(*key,0,0);
    DebrisReflectionOrientation(*key,0,0,0.0f,0.9f);
    AddDebrisEffectToStack(debkeydata + *key,debris_emitter_stack + debris_emitter_stack_index);
  }
  return;
}

//NGC MATCH
s32 LookupDebrisEffect(char *name) {
  s32 i;
  
  for (i = 1; i < 128; i++) {
    if ((debtab[i] != NULL) && (strcmp((debtab[i])->id,name) == 0)) return i;
  }
    return -1;
}

struct nuvec_s* CutoffCameraVec;

//NGC MATCH
void DebrisRegisterCutoffCameraVec(struct nuvec_s *cutoff) {
  CutoffCameraVec = cutoff;
  return;
}

//NGC MATCH
float CameraEmitterDistance(struct nuvec_s *vec) {
  if (CutoffCameraVec == NULL) {
    return 0.0f;
  }
  else {
    return NuVecDist(vec,CutoffCameraVec,NULL);
  }
}

