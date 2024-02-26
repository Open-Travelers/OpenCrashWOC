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
struct numtl_s* CreateCopyMat(struct numtl_s* origmtl, int uvmode, int alphamod, int zcmp, int filter) {
    struct numtl_s* mtl;

    mtl = NuMtlCreate(1);
    
    (mtl->diffuse).r = 1.0f;
    (mtl->diffuse).g = 1.0f;
    (mtl->diffuse).b = 1.0f;
    
    mtl->attrib.cull = 2;
    mtl->attrib.zmode = zcmp;
    mtl->attrib.filter = filter;
    
    mtl->alpha = 0.999f;
    
    mtl->attrib.alpha = alphamod;
    
    if (uvmode != 0) {
        mtl->attrib.uvmode = 1;
    }
    
    mtl->attrib.utc = 1;
    
    mtl->attrib.vtc = 1;
    mtl->tid = origmtl->tid;
    mtl->alpha_sort = origmtl->alpha_sort;
    mtl->attrib.afail = 1;
    mtl->attrib.colour = origmtl->attrib.colour;
    mtl->attrib.lighting = origmtl->attrib.lighting;
    NuMtlUpdate(mtl);
    return mtl;
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

//NGC MATCH
void RemoveChunkControlFromStack(struct debris_chunk_control_s *chunk,struct debris_chunk_control_s **stack) {
  struct debris_chunk_control_s *pdVar1;
  
  if (*stack != NULL) {
    if (*stack == chunk) {
      *stack = chunk->next;
    }
    else {
      do {
        pdVar1 = *stack;
        stack = &pdVar1->next;
        if (pdVar1->next == NULL) goto LAB_80080cb0;
      } while (pdVar1->next != chunk);
      pdVar1->next = chunk->next;
    }
  }
LAB_80080cb0:
  chunk->next = NULL;
  return;
}

//NGC MATCH
void AddChunkControlToStack(struct debris_chunk_control_s *chunk,struct debris_chunk_control_s **stack) {
  struct debris_chunk_control_s *pdVar1;
    struct debris_chunk_control_s** current;
  
  for (current = stack; *current != NULL;) {
      pdVar1 = *current;
      stack = &pdVar1->next;
      current = &pdVar1->next;
  }
  *current = chunk;
  chunk->next = NULL;
  return;
}

//NGC MATCH
void DebFreeInstantly(s32 *key) {
  s32 i;
  s32 j;
  struct debkeydatatype_s **stack;
  
  if (*key != -1) {
    freedebchkptr = freedebchkptr - debkeydata[*key].count;
    for(i = 0; i < debkeydata[*key].count; i++) {
        freedebchunks[i + freedebchkptr] = debkeydata[*key].chunks[i];
        for(j = 0; j < 0x100; j++) {
          if (ParticleChunkRenderStack[j].chunk == debkeydata[*key].chunks[i]) {
            ParticleChunkRenderStack[j].chunk = NULL;
            ParticleChunkRenderStack[j].debinfo = NULL;
          }
        }
    }
    stack = FindDebrisEffectStack(&debkeydata[*key]);
    RemoveDebrisEffectFromStack(&debkeydata[*key],stack);
    debkeydata[*key].type = 0;
    freedebkeyptr--;
    freedebkeys[freedebkeyptr] = *key;
    *key = -1;
  }
  return;
}

//NGC MATCH
void DebFreeWithoutKey(struct debkeydatatype_s *debkeydatatofree) {
  s32 lp;
  s32 key;
  
  for(lp = 0; lp <= 0xff; lp++) {
        if (&debkeydata[lp] == debkeydatatofree) {
              key = lp;
              DebFree(&key);
              break;
        }
  }
  return;
}

//NGC MATCH
void DebrisEmiterPos(s32 key,float x,float y,float z) {
  if (key == -1) {
    return;
  }
  debkeydata[key].x = x;
  debkeydata[key].y = y;
  debkeydata[key].z = z;
  return;
}

//NGC MATCH
void DebrisStartOffset(s32 key,s32 offset) {
  struct debinftype *debinfo;
  s32 looptime;
  s32 loopfrac;
  
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
          looptime = debinfo->ival_on + debinfo->ival_off;
          loopfrac = (globalframes % looptime);
          loopfrac -= offset;
          debkeydata[key].delay = (loopfrac != 0) ? looptime - loopfrac : loopfrac; 
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
struct debkeydatatype_s ** FindDebrisEffectStack(struct debkeydatatype_s *effect) {
  struct debkeydatatype_s **current;
  struct debkeydatatype_s *pdVar4;
  s32 lp;

  for (lp = 0; lp < 0x20; lp++) {
        current = &debris_emitter_stack[lp];
        if (debris_emitter_stack[lp] != 0) {
          do {
            pdVar4 = *current;
            if (pdVar4 == effect) {
              return &debris_emitter_stack[lp];
            }
            current = &pdVar4->next;
          } while (pdVar4->next != NULL);
        }
  }
  return NULL;
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
void RemoveDebrisEffectFromStack(struct debkeydatatype_s *effect,struct debkeydatatype_s **stack) {
  struct debkeydatatype_s *current;
  
  if (stack == NULL) {
    return;
  }
  if (effect == NULL) {
    return;
  }
  if (*stack != NULL) {
    if (*stack == effect) {
      *stack = effect->next;
    }
    else {
        do {
            current = *stack;
            stack = &current->next;
            if (current->next == NULL) goto LAB_80081630;
        } while (current->next != effect);
        current->next = effect->next;
    }
  }
LAB_80081630:
  effect->next = NULL;
  return;
}

//NGC MATCH
void AddFiniteShotDebrisEffect(s32 *key,s32 type,struct nuvec_s *pos,s32 repeats) {
  AddDebrisEffect(key,type,pos->x,pos->y,pos->z);
  if (*key != -1) {
    debkeydata[*key].instances = repeats;
    debkeydata[*key].delay = 0;
  }
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
void AddVariableShotDebrisEffect(s32 type,struct nuvec_s *pos,s32 numdeb,short emitrotz,short emitroty) {
  AddVariableShotDebrisEffectMtx(type,pos,numdeb,emitrotz,emitroty,&numtx_identity);
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

void DebrisSetup(void) {
  SetupDebris();
  return;
}

void DebrisSetRenderGroup(s32 group) {
  debris_render_group = group;
  return;
}

//NGC MATCH
void DebrisDoSounds(struct debkeydatatype_s *debkey,s32 flag) {
  struct debinftype *dt;
  s32 i;
  
  dt = debtab[debkey->type];
  if (dt != NULL) {
    for(i = 0; i < 4; i++) {
          if ((dt->sounds[i].id != -1) && (dt->sounds[i].type == flag)) {
                if (debkey->sound_next[i] < 1) {
                      debris_sfx = 1;
                      edbitsSoundPlay(&debkey->x,dt->sounds[i].id);
                      if (flag == 1 || flag == 2) {
                        debkey->sound_next[i] = 9999;
                      }
                      else {
                        debkey->sound_next[i] = dt->sounds[i].delay;
                      }
                }
          }
    }
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

