#include "../nu.h"

//NGC MATCH
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
void edppDestroyAllParticles(void) {
  s32 n;

  for(n = 0; n < 0x100; n++) {
    edppPtlDestroy(n);
  }
  return;
}

//NGC MATCH
void edppDestroyAllEffects(void) {
  s32 i;

  for(i = 1; i < 0x80; i++) {
    debtab[i] = NULL;
  }
  effect_types_used = 1;
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
void UpdateTotalPtls(struct debinftype* dt) {
/*
    s32 i; // r30
    s32 frames; // r11
    s32 on_frames; // r9
    s32 off_frames; // r7
    s32 count; // r8
    s32 fcount; // r10
*/

    s32 i;
    s32 frames;
    s32 iVar1;
    s32 iVar3;
    s32 iVar5;
    s32 fcount;
    s32 iVar9;

    iVar1 = 0;
    iVar3 = 0;
    
    fcount = (s32)(dt->etime * 60.0f);
    
    iVar9 = 1;
    
    if (dt->ival_off == 0) {
        frames = fcount;
    } else {
        frames = (s32)dt->ival_on + (s32)dt->ival_on_ran;
    }
    
    for (i = 0; i < fcount; i++) {
        if (frames != 0) {
            if (dt->generate >= 0) {
                iVar1 += dt->generate;
            } else {
                iVar5 = dt->generate > 0 ? dt->generate : -dt->generate;
                if (iVar3 == (iVar3 / iVar5) * iVar5) {
                    iVar1++;
                }
            }
            iVar3++;
            frames--;
            if (frames == 0) {
                iVar9 = (s32)dt->ival_off;
            }
        } else {
            iVar9--;
            if (iVar9 == 0) {
                iVar3 = 0;
                frames = (s32)dt->ival_on + (s32)dt->ival_on_ran;
            }
        }
    }

    dt->debnum = iVar1;
    if (dt->debnum < 1) {
        dt->debnum = 1;
    }

    for (i = 0; i < 0x100; i++) {
        if ((edpp_ptls[i].handle != -1) && (debtab[debkeydata[edpp_ptls[i].handle].type] == dt)) {
            DebReAlloc(debkeydata + edpp_ptls[i].handle, (s32)dt->debnum);
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

//93% NGC (fix) //PS2 MATCH
s32 edppLoadEffects(char* file, char list) {
    s32 i;
    s32 version;
    s32 numptls;
    s32 maxversion = 15;
    s32 maxptl;

    if (EdFileOpen(file, NUFILE_READ) != 0) {
        version = EdFileReadInt();
        if (version > maxversion) {
            EdFileClose();
            return 0;
        }
    } else {
        return 0;
    }
    if (version > 1) {
        effect_types_used = EdFileReadInt();
    } else {
        effect_types_used = 128;
    }
    if (version > 3) {
        effect_types_used++;
    }
    if (version < 4) {
        for (i = 0; i < effect_types_used; i++) {
            FileLoadSingleEffectType((struct debinftype*)&effecttypes[i], version, list);
        }
    } else {
        for (i = 1; i < effect_types_used; i++) {
            FileLoadSingleEffectType((struct debinftype*)&effecttypes[i], version, list);
        }
    }
    for (i = effect_types_used; i < 128; i++) {
        *((struct debinftype*)&effecttypes[i]) = *((struct debinftype*)&effecttypes[0]);
    }
    if (version < 4) {
        EdFileRead(debtab, sizeof(debtab));
        effect_types_used = 0;
        for (i = 0; i < 128; i++) {
            if (debtab[i] != 0) {
                debtab[i] = &effecttypes[i];
                effect_types_used++;
                UpdateTotalPtls((struct debinftype*)&effecttypes[i]);
            }
        }
    } else {
        for (i = 0; i < 128; i++) {
            if (i < effect_types_used) {
                debtab[i] = &effecttypes[i];
                UpdateTotalPtls((struct debinftype*)&effecttypes[i]);
            } else {
                debtab[i] = NULL;
            }
        }
    }
    if (list == 1 || list == 2) {
        ParticleReset();
        if (version < 5) {
            for (i = 0; i <= 255; i++) {
                EdFileRead(&edpp_ptls[i].pos, sizeof(struct nuvec_s));
                edpp_ptls[i].type = EdFileReadInt();
                edpp_ptls[i].handle = EdFileReadInt();
                edpp_ptls[i].emitrotz = EdFileReadInt();
                edpp_ptls[i].emitroty = EdFileReadInt();
                if (debtab[edpp_ptls[i].type] == NULL) {
                    *edpp_ptls[i].name = 0;
                } else {
                    memcpy(edpp_ptls[i].name, (void *)debtab[edpp_ptls[i].type]->id, 16);
                }
                edpp_ptls[i].offset = 0;
                edpp_ptls[i].trigger_type = 0;
                edpp_ptls[i].trigger_id = -1;
                edpp_ptls[i].trigger_var = 0.0f;
            }
        } else {
            numptls = EdFileReadInt();
            maxptl = numptls > 256 ? 256 : numptls;
            for (i = 0; i < maxptl; i++) {
                EdFileRead(&edpp_ptls[i].pos, sizeof(struct nuvec_s));
                if (version < 7) {
                    edpp_ptls[i].rotz = 0;
                    edpp_ptls[i].roty = 0;
                    edpp_ptls[i].emitrotz = EdFileReadInt();
                    edpp_ptls[i].emitroty = EdFileReadInt();
                } else {
                    edpp_ptls[i].rotz = EdFileReadShort();
                    edpp_ptls[i].roty = EdFileReadShort();
                    edpp_ptls[i].emitrotz = EdFileReadShort();
                    edpp_ptls[i].emitroty = EdFileReadShort();
                }
                if (version < 8) {
                    edpp_ptls[i].offset = 0;
                } else {
                    edpp_ptls[i].offset = EdFileReadInt();
                }
                EdFileRead(edpp_ptls[i].name, 16);
                edpp_ptls[i].handle = i;
                edpp_ptls[i].type = LookupDebrisEffect(edpp_ptls[i].name);
                if (version > 8) {
                    edpp_ptls[i].trigger_type = EdFileReadInt();
                    edpp_ptls[i].trigger_id = EdFileReadInt();
                    edpp_ptls[i].trigger_var = EdFileReadFloat();
                } else {
                    edpp_ptls[i].trigger_type = 0;
                    edpp_ptls[i].trigger_id = -1;
                    edpp_ptls[i].trigger_var = 0.0f;
                }
                if (version > 11) {
                    edpp_ptls[i].refrotz = EdFileReadShort();
                    edpp_ptls[i].refroty = EdFileReadShort();
                    edpp_ptls[i].refoff = EdFileReadFloat();
                } else {
                    edpp_ptls[i].refrotz = 0;
                    edpp_ptls[i].refroty = 0;
                    edpp_ptls[i].refoff = 0.0f;
                }
                if (version > 12) {
                    edpp_ptls[i].refbounce = EdFileReadFloat();
                } else {
                    edpp_ptls[i].refbounce = 0.9f; // if it's 0x3f666666 it can be replaced with 0.9f
                }
                if (version > 14) {
                    edpp_ptls[i].group_id = EdFileReadShort();
                } else {
                    edpp_ptls[i].group_id = 0;
                }
            }
        }
    }
    EdFileClose();
    for (i = 0; i < 128; i++) {
        effecttypes[i].DmaDebTypePointer = 0;
        effecttypes[i].variable_key = -1;
    }
    freeDmaDebType = 0;
    edppDetermineNearest(1.0f);
    return 1;
}

//NGC MATCH
void edppRestartAllEffectsInLevel(void) {
  s32 i;
  
  for (i = 0; i < 256; i++) {
    if (edpp_ptls[i].handle != -1) {
      edpp_ptls[i].handle = -1;
      edpp_ptls[i].type = LookupDebrisEffect(edpp_ptls[i].name);
      AddDebrisEffect(&edpp_ptls[i].handle,edpp_ptls[i].type,(edpp_ptls[i].pos).x,edpp_ptls[i].pos.y,edpp_ptls[i].pos.z);
      if (edpp_ptls[i].handle != -1) {
        DebrisOrientation(edpp_ptls[i].handle,edpp_ptls[i].rotz,edpp_ptls[i].roty);
        DebrisEmitterOrientation(edpp_ptls[i].handle,edpp_ptls[i].emitrotz,edpp_ptls[i].emitroty);
        DebrisStartOffset(edpp_ptls[i].handle,edpp_ptls[i].offset);
        DebrisSetTrigger(edpp_ptls[i].handle,edpp_ptls[i].trigger_type,edpp_ptls[i].trigger_id,
                         edpp_ptls[i].trigger_var);
        DebrisReflectionOrientation(edpp_ptls[i].handle,edpp_ptls[i].refrotz,edpp_ptls[i].refroty,edpp_ptls[i].refoff,edpp_ptls[i].refbounce);
        DebrisSetGroupID(edpp_ptls[i].handle,edpp_ptls[i].group_id);
      }
    }
  }
  return;
}

//NGC MATCH //PS2 99% (extra nop)
s32 edppMergeEffects(char* file, char list) {
    struct debinftype dummy;
    s32 i;
    s32 version;
    s32 new_effects;

    TidyAllEffects();
    //EdFileSetMedia(1); //PS2 only
    if (EdFileOpen(file, NUFILE_READ) != 0) {
        version = EdFileReadInt();
        if (version < 2) {
            EdFileClose();
            return 0;
        }
    } else {
        return 0;
    }
    new_effects = EdFileReadInt();
    if (version < 4) {
        new_effects--;
        FileLoadSingleEffectType(&dummy, version, list);
    }
    if (0x80 < effect_types_used + new_effects) {
        new_effects = 0x80 - effect_types_used;
    }
    for (i = effect_types_used; i < effect_types_used + new_effects; i++) {
        FileLoadSingleEffectType((struct debinftype*)&effecttypes[i], version, list);
    }
    for (i = effect_types_used + new_effects; i < 0x80; i++) {
        *((struct debinftype*)&effecttypes[i]) = *((struct debinftype*)&effecttypes[0]);
    }
    for (i = effect_types_used; i < effect_types_used + new_effects; i++) {
        debtab[i] = &effecttypes[i];
        UpdateTotalPtls((struct debinftype*)debtab[i]);
    }
    effect_types_used += new_effects;
    EdFileClose();
    for (i = 0; i < 0x80; i++) { //extra nop PS2
        effecttypes[i].DmaDebTypePointer = NULL;
        effecttypes[i].variable_key = -1;
    }
    freeDmaDebType = 0;
    return 1;
}

//NGC MATCH
void ParticleReset(void) {
  s32 n;
  
  for(n = 0; n < 0x100; n++) {
    edpp_ptls[n].handle = -1;
  }
  edpp_nextalloc = 0;
  return;
}