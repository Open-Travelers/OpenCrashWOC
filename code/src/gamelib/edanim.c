//#include "edanim.h"
#include "../nu.h"

//NGC MATCH
float edanimPlayerAnimDistance(s32 id) {
  if (edmainQueryLocVec() != NULL) {
    return NuVecDist((struct nuvec_s*)&edanim_base_scene->specials[AnimParams[id].objid].mtx._30,edmainQueryLocVec(),NULL);
  }
  else {
    return 0.0f;
  }
}

//NGC MATCH
void edanimUpdateObjects(float dt) {
  s32 i;
  s32 j;
  s32 k;
  struct nuinstance_s *instance;
  struct nuinstanim_s *instanim;
  struct nuvec_s emit;
  struct numtx_s tmtx;
  struct numtx_s orient;
  s32 sid;
  
  if (edanim_base_scene != NULL) {
    for(i = 0; i < 0x40; i++) {
      if (AnimParams[i].objid != -1) {
        instance = edanim_base_scene->specials[AnimParams[i].objid].instance;
        instanim = instance->anim;
        if (instanim != NULL) {
            switch(AnimParams[i].trigger_type) {
                case 0:
                break;
                case 1:
                instanim->playing = 0;
                if ((AnimParams[i].trigger_id != -1) && (object_switches[AnimParams[i].trigger_id] != 0)) {
                    instanim->playing = 1;
                }
                break;
                case 2:
              instanim->repeating = 0;
              if (((AnimParams[i].trigger_id != -1) &&
                  (object_switches[AnimParams[i].trigger_id] != 0)) && (instanim->playing == 0)) {
                instanim->ltime = 1.0f;
                instanim->playing = 1;
                instanim->waiting = 0;
                instanim->backwards = 0;
              }
                break;
                case 3:
                instanim->repeating = 1;
              if ((AnimParams[i].trigger_id != -1) && (object_switches[AnimParams[i].trigger_id] != 0)) {
                  instanim->playing = 1;
              }
                break;
                case 4:
              if ((edanimPlayerAnimDistance(i) < AnimParams[i].trigger_var)) {
                  instanim->playing = 1; 
              }
              else {
                  instanim->playing = 0;
              }
                break;
                case 5:
              instanim->repeating = 0;
            if ((edanimPlayerAnimDistance(i) < AnimParams[i].trigger_var) ) {
                if (instanim->playing == 0) {
                    instanim->playing = 1;
                    instanim->ltime = 1.0f;
                    instanim->waiting = 0;
                    instanim->backwards = 0;
                }
            }
                break;
                case 6:
                instanim->repeating = 1;
                if (edanimPlayerAnimDistance(i) < AnimParams[i].trigger_var) {
                  instanim->playing = 1;
                }
                break;
                case 7:
                case 8:
                case 9:
                    break;
                case 0xa:
                  instanim->ltime = 1.0f; 
                  instanim->playing = 0;
                break;
                case 0xb:
                  instanim->playing = 1; 
                break;
                case 0xc:
                  instanim->playing = 1; 
                  instanim->repeating = 1;
                break;
            }
          if ((i == edanim_nearest_param_id) && ((edanim_particle_mode != 0 || (edanim_sound_mode != 0)))) {
            instanim->ltime = 1.0f;
          }
        }
        for(j = 0; j < (s32)dt; j++) {
            for(k = 0; k < AnimParams[i].usedpart; k++) {
                if (AnimParams[i].particle_type[k] != -1) {
                    if ((AnimParams[i].particle_switch[k] == 0) || (((instanim != NULL && (instanim->playing > 0)) &&
                         !(instanim->ltime >= edanim_base_scene->instanimdata[instanim->anim_ix]->time)))) {
                  if (instanim != NULL) {
                    NuMtxInvRSS(&tmtx,&edanim_base_scene->specials[AnimParams[i].objid].mtx );
                    NuMtxMul(&tmtx,&tmtx,&instanim->mtx);
                  }
                  else {
                    NuMtxInvRSS(&tmtx,&edanim_base_scene->specials[AnimParams[i].objid].mtx );
                    NuMtxMul(&tmtx,&tmtx,&instance->mtx);
                  }
                  emit = AnimParams[i].particle_offset[k];
                  NuVecMtxTransform(&emit,&emit,&tmtx);
                    orient = numtx_identity;
                  NuMtxRotateZ(&orient,AnimParams[i].particle_emitrotz[k]);
                  NuMtxRotateY(&orient,(s32)AnimParams[i].particle_emitroty[k]);
                  NuMtxMul(&tmtx,&orient,&tmtx);
                  if (AnimParams[i].particle_rate[k] > 0) {
                    AddVariableShotDebrisEffectMtx2
                              (AnimParams[i].particle_type[k],&emit,AnimParams[i].particle_rate[k],&tmtx,&numtx_identity);
                  }
                  else {
                    if ((AnimParams[i].particle_rate[k] < 0) &&
                        (localframecount_156 + j) % 
                          (AnimParams[i].particle_rate[k] >= 0 ? AnimParams[i].particle_rate[k] : -AnimParams[i].particle_rate[k]) == 0) {
                      AddVariableShotDebrisEffectMtx2(AnimParams[i].particle_type[k],&emit,1,&tmtx,&numtx_identity);
                    }
                  }
                }
                }
                else {
                  if (AnimParams[i].particle_name[k][0] != 0) {
                    AnimParams[i].particle_type[k] = LookupDebrisEffect(AnimParams[i].particle_name[k]);
                    if (AnimParams[i].particle_type[k] == -1) {
                      edanimParticleDestroy(i,k);
                      k--;
                    }
                  }  
                }
            }
        }
        for(j = 0; j < AnimParams[i].usedsound; j++) {
            if (AnimParams[i].sound_id[j] != -1) {
              sid = -1;
              if (AnimParams[i].sound_type[j] == 1) {
                if ( (s32)(localframecount_156 + dt) / (s32)AnimParams[i].sound_time[j] > (s32)localframecount_156 / (s32)AnimParams[i].sound_time[j]) {
                  sid = AnimParams[i].sound_id[j];
                }
              }
              else if (instanim != NULL) {
                if ((instanim->ltime >= AnimParams[i].sound_time[j]) &&
                   (AnimParams[i].sound_time[j] > AnimParams[i].sound_last_time)) {
                  sid = AnimParams[i].sound_id[j];
                }
                if ((((instanim->oscillate) != 0) &&
                (instanim->ltime <= AnimParams[i].sound_time[j])) &&
                AnimParams[i].sound_time[j] < ((s32)AnimParams[i].sound_last_time >= 0 ? (s32)AnimParams[i].sound_last_time : -(s32)AnimParams[i].sound_last_time)
                ) {
                    sid = AnimParams[i].sound_id[j];
                }
              }
              if (sid != -1) {
                if (instanim != NULL) {
                  NuMtxInvRSS(&tmtx,&edanim_base_scene->specials[AnimParams[i].objid].mtx);
                  NuMtxMul(&tmtx,&tmtx,&instanim->mtx);
                }
                else {
                  NuMtxInvRSS(&tmtx,&edanim_base_scene->specials[AnimParams[i].objid].mtx);
                  NuMtxMul(&tmtx,&tmtx,&instance->mtx); 
                }
                emit = AnimParams[i].sound_offset[k];
                NuVecMtxTransform(&emit,&emit,&tmtx);
                edbitsSoundPlay(&emit,sid);
              }
            }
            else {
              if (AnimParams[i].sound_name[j][0] != 0) {
                AnimParams[i].sound_id[j] = edbitsLookupSoundFX(AnimParams[i].sound_name[j]);
                if (AnimParams[i].sound_id[j] == -1) {
                  edanimSoundDestroy(i,j);
                  j--;
                }
              }
            }
        }
        if (instanim != NULL) {
          if ((instanim->oscillate) != 0) {
            AnimParams[i].sound_last_time = -instanim->ltime;
          }
          else {
            AnimParams[i].sound_last_time = instanim->ltime;
          }
        }
      }
    }
    localframecount_156 += (s32)dt;
  }
  return;
}

//NGC MATCH
void edanimDetermineNearestAnim(float ndist) {
  s32 i;
  float dist;
  struct nuvec_s distv;
  
  if (edanim_base_scene != NULL) {
    if ((edanim_nearest != -1)) {
        NuVecSub(&distv,&edanim_cam_pos,(struct nuvec_s *)&(edanim_base_scene->specials[edanim_nearest].instance)->mtx._30);
        dist = (distv.x * distv.x + distv.y * distv.y + distv.z * distv.z);
        if (dist == 0.0) return;
    }
    edanim_nearest_param_id = -1;
    edanim_nearest = -1;
    for(i = 0; i < edanim_base_scene->numspecial; i++) {
        NuVecSub(&distv,&edanim_cam_pos,(struct nuvec_s *)&(edanim_base_scene->specials[i].instance)->mtx._30);
        dist = (distv.x * distv.x + distv.y * distv.y + distv.z * distv.z);
        if ((ndist < 0.0f) || (dist < ndist)) {
          ndist = dist;
          edanim_nearest = i;
        }
    }
    if (edanim_nearest != -1) {
        for (i = 0; i < 0x40; i++) {
            if (AnimParams[i].objid == edanim_nearest) {
                edanim_nearest_param_id = i;
                break;
            }
        }
    }
  }
  return;
}

//NGC MATCH
void edanimSoundDestroy(s32 obj,s32 sound) {
  s32 i;
  
  for(i = sound; i < AnimParams[obj].usedsound - 1; i++) {
      AnimParams[obj].sound_id[i] = AnimParams[obj].sound_id[i + 1];
      AnimParams[obj].sound_time[i] = AnimParams[obj].sound_time[i + 1];
      AnimParams[obj].sound_offset[i] = AnimParams[obj].sound_offset[i + 1];
      strcpy(AnimParams[obj].sound_name[i],AnimParams[obj].sound_name[i + 1]);
  }
  AnimParams[obj].usedsound--;
  return;
}

//NGC MATCH
void edanimParticleDestroy(s32 obj,s32 ptl) {
  s32 i;

  for(i = ptl; i < AnimParams[obj].usedpart - 1; i++) {
      AnimParams[obj].particle_type[i] = AnimParams[obj].particle_type[i + 1];
      AnimParams[obj].particle_rate[i] = AnimParams[obj].particle_rate[i + 1];
      AnimParams[obj].particle_switch[i] = AnimParams[obj].particle_switch[i + 1];
      AnimParams[obj].particle_offset[i] = AnimParams[obj].particle_offset[i + 1];
      strcpy(AnimParams[obj].particle_name[i],AnimParams[obj].particle_name[i + 1]);
  }
  AnimParams[obj].usedpart--;
  return;
}

//NGC MATCH
s32 edanimFileLoad(char *file) {
  s32 i;
  s32 j;
  s32 version;
  s32 numparams;
  char tname[20];
  //s32 maxversion;
  s32 dummyreads;
  struct nuvec_s dummyvec;
  char dummyname[16];

  if (EdFileOpen(file,NUFILE_READ) != 0) {
    version = EdFileReadInt();
    if (version > 4) {
        EdFileClose();
        return 0;
    }
  } else {
      return 0;
  }
      numparams = EdFileReadInt();
      for(i = 0; i < numparams; i++) {
          EdFileRead(tname,0x14);
          AnimParams[i].objid = edanimLookupSpecial(tname);
          AnimParams[i].usedpart = EdFileReadInt();
          if (1 < version) {
            AnimParams[i].usedsound = EdFileReadInt();
          }
          else {
            AnimParams[i].usedsound = 0;
          }
          dummyreads = 0;
          AnimParams[i].trigger_type = EdFileReadInt();
          AnimParams[i].trigger_id = EdFileReadInt();
          AnimParams[i].trigger_var = EdFileReadFloat();
          AnimParams[i].trigger_wait = EdFileReadFloat();
          if (8 < AnimParams[i].usedpart) {
            dummyreads = 8 - AnimParams[i].usedpart;
            AnimParams[i].usedpart = 8;
          }
          for(j = 0; j < AnimParams[i].usedpart; j++) {
              EdFileRead(AnimParams[i].particle_name[j],0x10);
              AnimParams[i].particle_type[j] = -1;
              AnimParams[i].particle_rate[j] = EdFileReadInt();
              AnimParams[i].particle_switch[j] = EdFileReadInt();
              EdFileRead(&AnimParams[i].particle_offset[j],0xc);
              if (version > 2) {
                AnimParams[i].particle_emitrotz[j] = EdFileReadShort();
                AnimParams[i].particle_emitroty[j] = EdFileReadShort();
              }
              else {
                AnimParams[i].particle_emitrotz[j] = 0;
                AnimParams[i].particle_emitroty[j] = 0;
              }
          }
          for(j = 0; j < dummyreads; j++) {
              EdFileRead(dummyname,0x10);
              EdFileReadInt();
              EdFileReadInt();
              EdFileRead(&dummyvec,0xc);
              if (2 < version) {
                EdFileReadShort();
                EdFileReadShort();
              }
          }
          AnimParams[i].sound_last_time = 0.99f;
          if (1 < version) {
            dummyreads = 0;
            if (8 < AnimParams[i].usedsound) {
              dummyreads = 8 - AnimParams[i].usedsound;
              AnimParams[i].usedsound = 8;
            }
            for(j = 0; j < AnimParams[i].usedsound; j++) {
                EdFileRead(AnimParams[i].sound_name[j],0x10);
                AnimParams[i].sound_id[j] = -1;
                AnimParams[i].sound_type[j] = EdFileReadInt();
                AnimParams[i].sound_time[j] = EdFileReadFloat();
                EdFileRead(&AnimParams[i].sound_offset[j],0xc);
            }
            for(j = 0; j < dummyreads; j++) {
                EdFileRead(dummyname,0x10);
                EdFileReadInt();
                EdFileReadFloat();
                EdFileRead(&dummyvec,0xc);
            }
          }
          AnimParams[i].terrplatid = FindPlatInst(edanimLookupInstanceIndex(AnimParams[i].objid));
          if (version > 3) {
            AnimParams[i].playergrav = EdFileReadFloat();
            AnimParams[i].tension = EdFileReadFloat();
            AnimParams[i].damping = EdFileReadFloat();
          }
          else {
            AnimParams[i].playergrav = 0.0f;
            AnimParams[i].tension = 0.0f;
            AnimParams[i].damping = 0.0f;
          }
          if (AnimParams[i].terrplatid != -1) {
            PlatInstBounce(AnimParams[i].terrplatid,AnimParams[i].playergrav,AnimParams[i].tension ,
                           AnimParams[i].damping);
          }
      }
      EdFileClose();
      edanim_next_param = numparams;
      edanim_nearest = -1;
      edanim_particle_mode = 0;
      edanim_nearest_param_id = -1;
      edanimDetermineNearestAnim(1.0f);
      return 1;
}

//NGC MATCH
s32 edanimLookupInstanceIndex(s32 specialid) {
  s32 i;
  
  if (edanim_base_scene == NULL) {
    return -1;
  }
    for(i = 0; i < edanim_base_scene->numinstance; i++) {
      if (&edanim_base_scene->instances[i] == edanim_base_scene->specials[specialid].instance) {
        return i;
      }
    }
  return -1;
}

//NGC MATCH
void edanimRegisterBaseScene(struct nugscn_s *scn) {
  edanim_base_scene = scn;
  return;
}

//NGC MATCH
void edanimParamReset(void) {
  s32 i;

  for(i = 0; i < 64; i++) {
    AnimParams[i].objid = -1;
  }
  edanim_next_param = 0;
  return;
}

//NGC MATCH
s32 edanimLookupSpecial(char *name) {
  s32 i;
  
  if (edanim_base_scene != NULL) {
    for(i = 0; i < edanim_base_scene->numspecial; i++) {
      if (strncmp(edanim_base_scene->specials[i].name,name,0x13) == 0) {
        return i;
      }
    }
  }
  return -1;
}