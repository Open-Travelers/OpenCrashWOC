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