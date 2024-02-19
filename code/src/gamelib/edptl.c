#include "../nu.h"

void edppRegisterPointerToGameCharLocation(struct nuvec_s *charloc) {
  edmainRegisterLocVec(charloc);
  return;
}

//NGC MATCH
void edppPtlDestroy(s32 sel) {
  if (edpp_ptls[sel].handle != -1) {
    DebFreeInstantly(&edpp_ptls[sel].handle);
    edpp_ptls[sel].handle = -1;
  }
  return;
}

//NGC MATCH
void edppDetermineNearest(float ndist) {
  float dist;
  s32 n;
  struct nuvec_s distv;
  
  if ((edpp_nearest == -1) || (NuVecSub(&distv,&edpp_cam_pos,&edpp_ptls[edpp_nearest].pos), 
      dist = (distv.x * distv.x + distv.y * distv.y + distv.z * distv.z),
      (distv.x * distv.x + distv.y * distv.y + distv.z * distv.z) != 0.0)) {
        edpp_nearest = -1;
        for(n = 0; n < 0x100; n++) {
              if (edpp_ptls[n].handle != -1) {
                    NuVecSub(&distv,&edpp_cam_pos,&edpp_ptls[n].pos);
                    dist = (distv.x * distv.x + distv.y * distv.y + distv.z * distv.z);
                    if ((ndist < 0.0f) || (dist < ndist)) {
                      ndist = dist;
                      edpp_nearest = n;
                    }
              }
        }
  }
  return;
}

//NGC MATCH
void FileLoadSingleEffectType(struct debinftype *effect,s32 version,char list) {
  s32 i;
  
  EdFileRead(effect,0x10);
  effect->listid = list;
  effect->generate = EdFileReadShort();
  effect->debnum = EdFileReadShort();
  effect->ival_offset = EdFileReadShort();
  effect->ival_on = EdFileReadShort();
  effect->ival_on_ran = EdFileReadShort();
  effect->ival_off = EdFileReadShort();
  effect->ival_off_ran = EdFileReadShort();
  effect->gensort = EdFileReadChar();
  effect->gencode = EdFileReadChar();
  effect->mat = EdFileReadChar();
  effect->efftab = EdFileReadChar();
  effect->radius = EdFileReadFloat();
  if (version < 6) {
    effect->cuton = 0.0f;
    effect->cutoff = 25.0f;
  }
  else {
    effect->cuton = EdFileReadFloat();
    effect->cutoff = EdFileReadFloat();
  }
  if (version < 10) {
    effect->drawcutoff = 0.0f;
  }
  else {
    effect->drawcutoff = EdFileReadFloat();
  }
  if (version < 7) {
    EdFileReadInt();
    EdFileReadInt();
  }
  effect->emitmag = EdFileReadFloat();
  EdFileRead(&effect->variable_emit,0xc);
  EdFileRead(&effect->variable_emit_ranscale,0xc);
  EdFileRead(&effect->variable_start,0xc);
  EdFileRead(&effect->variable_start_ranscale,0xc);
  effect->rsx = EdFileReadFloat();
  effect->rsy = EdFileReadFloat();
  effect->rsz = EdFileReadFloat();
  effect->osx = EdFileReadFloat();
  effect->osy = EdFileReadFloat();
  effect->osz = EdFileReadFloat();
  effect->rvx = EdFileReadFloat();
  effect->rvy = EdFileReadFloat();
  effect->rvz = EdFileReadFloat();
  effect->ovx = EdFileReadFloat();
  effect->ovy = EdFileReadFloat();
  effect->ovz = EdFileReadFloat();
  effect->grav = EdFileReadFloat();
  effect->etime = EdFileReadFloat();
  effect->uw = EdFileReadShort();
  effect->frames = EdFileReadChar();
  effect->rate = EdFileReadChar();
  effect->uvanim = EdFileReadFloat();
  effect->jibxfreq = EdFileReadFloat();
  effect->jibxamp = EdFileReadFloat();
  effect->jibyfreq = EdFileReadFloat();
  effect->jibyamp = EdFileReadFloat();
  for(i = 0; i < 8; i++) {
    EdFileRead(&effect->colslot[i],0x10);
  }
  for(i = 0; i < 8; i++) {
    EdFileRead(&effect->alpslot[i],8);
  }
  effect->minsize = EdFileReadFloat();
  effect->maxsize = EdFileReadFloat();
  for(i = 0; i < 8; i++) {
    EdFileRead(&effect->wslot[i],8);
  }
  for(i = 0; i < 8; i++) {
    EdFileRead(&effect->hslot[i],8);
  }
  effect->minrot = EdFileReadFloat();
  effect->maxrot = EdFileReadFloat();
  for(i = 0; i < 8; i++) {
    EdFileRead(&effect->rotslot[i],8);
  }
  for(i = 0; i < 8; i++) {
    EdFileRead(&effect->jibxslot[i],8);
  }
  for(i = 0; i < 8; i++) {
    EdFileRead(&effect->jibyslot[i],8);
  }
  effect->u1 = EdFileReadFloat();
  effect->v1 = EdFileReadFloat();
  effect->u2 = EdFileReadFloat();
  effect->v2 = EdFileReadFloat();
  if (version > 2) {
        for(i = 0; i < 8; i++) {
          EdFileRead(&effect->sphereslot[i],8);
        }
        effect->numspheres = EdFileReadChar();
  }
  else {
    effect->numspheres = 0;
  }
  for(i = 0; i < 4; i++) {
        if (version > 0xa) {
          EdFileRead(&effect->sounds[i],0xc);
        }
        else {
          effect->sounds[i].id = -1;
          effect->sounds[i].type = 0;
          effect->sounds[i].delay = 0;
        }
  }
  return;
}

//83% NGC
int edppLoadEffects(char *file,char list) {
  int version;
  int i;
  int iVar8;

  
  iVar8 = EdFileOpen(file,NUFILE_READ);
  if (iVar8 == 0) {
      EdFileClose();
      return 0;
  } else {
        version = EdFileReadInt();
        if (version > 0xf) {
            EdFileClose();
            return 0;
        } else {
              if (version > 1) {
                effect_types_used = EdFileReadInt();
              }
              else {
                effect_types_used = 0x80;
              }
              if (version > 3) {
                effect_types_used++;
                  for(i = 0; i < effect_types_used; i++) {
                    FileLoadSingleEffectType(&effecttypes[i],version,list);
                  }
              }
              else {
                  for(i = 1; i < effect_types_used; i++) {
                    FileLoadSingleEffectType(&effecttypes[i],version,list);
                  }
              }
              if (effect_types_used < 0x80) {
                i = 0x80 - effect_types_used;
                effecttypes[i] = effecttypes[effect_types_used];
              }
              if (version < 3) {
                    iVar8 = 0;
                    EdFileRead(debtab,0x200);
                    effect_types_used = 0;
                    for(i = 0; i < 0x80; i++) {
                          if (debtab[i] != 0) {
                            debtab[i] = &effecttypes[i];
                            effect_types_used++;
                            UpdateTotalPtls(&effecttypes[i]);
                          }
                    }
              }
              else {
                    for(i = 0; i < 0x80; i++) {
                      if (i < effect_types_used) {
                        debtab[i] = &effecttypes[i];
                        UpdateTotalPtls(&effecttypes[i]);
                      }
                      else {
                        debtab[i] = NULL;
                      }
                    }
              }
              if ((list == 0) || (list == 1)) {
                ParticleReset();
                if (version < 5) {
                      for(i = 0; i <= 256; i++) {
                        EdFileRead(&edpp_ptls[i],0xc);
                        edpp_ptls[i].type = EdFileReadInt();
                        edpp_ptls[i].handle = EdFileReadInt();
                        edpp_ptls[i].emitrotz = (short)EdFileReadInt();
                        edpp_ptls[i].emitroty = (short)EdFileReadInt();
                        if (debtab[edpp_ptls[i].type] == NULL) {
                          *edpp_ptls[i].name = 0;
                        }
                        else {
                           memcpy(edpp_ptls[i].name, debtab[edpp_ptls[i].type]->id, 16);
                        }
                        edpp_ptls[i].offset = 0;
                        edpp_ptls[i].trigger_type = 0;
                        edpp_ptls[i].trigger_id = -1;
                        edpp_ptls[i].trigger_var = 0.0f;
                      }
                }
                else {
                  i = EdFileReadInt();
                  if (0x100 < i) {
                    i = 0x100;
                  }
                    for(iVar8 = 0; iVar8 < i; iVar8++) {
                      EdFileRead(&edpp_ptls[iVar8],0xc);
                      if (version < 7) {
                        edpp_ptls[iVar8].rotz = 0;
                        edpp_ptls[iVar8].roty = 0;
                        edpp_ptls[iVar8].emitrotz = EdFileReadInt();
                        edpp_ptls[iVar8].emitroty = EdFileReadInt();
                      }
                      else {
                        edpp_ptls[iVar8].rotz = EdFileReadShort();
                        edpp_ptls[iVar8].roty = EdFileReadShort();
                        edpp_ptls[iVar8].emitrotz = EdFileReadShort();
                        edpp_ptls[iVar8].emitroty = EdFileReadShort();
                      }
                      if (version < 8) {
                        edpp_ptls[iVar8].offset = 0;
                      }
                      else {
                        edpp_ptls[iVar8].offset = EdFileReadInt();
                      }
                      EdFileRead(edpp_ptls[iVar8].name,0x10);
                      edpp_ptls[iVar8].handle = iVar8;
                      edpp_ptls[iVar8].type = LookupDebrisEffect(edpp_ptls[iVar8].name);
                      if (version > 8) {
                        edpp_ptls[iVar8].trigger_type = EdFileReadInt();
                        edpp_ptls[iVar8].trigger_id = EdFileReadInt();
                        edpp_ptls[iVar8].trigger_var = EdFileReadFloat();
                      }
                      else {
                        edpp_ptls[iVar8].trigger_id = -1;
                        edpp_ptls[iVar8].trigger_var = 0.0f;
                        edpp_ptls[iVar8].trigger_type = 0;
                      }
                      if (version > 0xb) {
                        edpp_ptls[iVar8].refrotz = EdFileReadShort();
                        edpp_ptls[iVar8].refroty = EdFileReadShort();
                        edpp_ptls[iVar8].refoff = EdFileReadFloat();
                      }
                      else {
                        edpp_ptls[iVar8].refrotz = 0;
                        edpp_ptls[iVar8].refroty = 0;
                        edpp_ptls[iVar8].refoff = 0.0f;
                      }
                      if (version > 0xc) {
                        edpp_ptls[iVar8].refbounce = EdFileReadFloat();
                      }
                      else {
                        edpp_ptls[iVar8].refbounce = 0.89999998f;
                      }
                      if (version > 0xe) {
                        edpp_ptls[iVar8].group_id = EdFileReadShort();
                      }
                      else {
                        edpp_ptls[iVar8].group_id = 0;
                      }
                    }
                }
              }
              EdFileClose();
              for(i = 0; i < 0x80; i++) {
                effecttypes[i].DmaDebTypePointer = 0;
                effecttypes[i].variable_key = -1;
              }
              freeDmaDebType = 0;
              edppDetermineNearest(1.0f);
              return 1;
        }
   }
}