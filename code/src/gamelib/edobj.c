#include "edobj.h"
#include "../nu.h"

//NGC MATCH
float edobjPlayerObjectDistance(s32 objid) {
  if (edmainQueryLocVec() != NULL) {
    return NuVecDist(&ObjectInstance[objid].mtx._30,edmainQueryLocVec(),NULL);
  }
  else {
    return 0.0f;
  }
}

//NGC MATCH
s32 edobjRenderCutoffTest(struct nuvec_s *pos) {
  float x;
  float y;
  float z;
  
  x = pos->x - global_camera.mtx._30;
  y = pos->y - global_camera.mtx._31;
  z = pos->z - global_camera.mtx._32;
  if (NuFsqrt(x * x + y * y + z * z) > 80.0f) {
      return 0;
  }
  return 1;
}

//NGC MATCH
void edobjRenderObjects(struct nugscn_s *scn) {
    struct nuinstance_s *instance;
    s32 i;
    struct nuinstanim_s *instanim;

    instance = ObjectInstance;
    
    for (i = 0; i < 0x40; i++) {
        instance = &ObjectInstance[i];
        if (instance->objid != -1) {
            instanim = instance->anim;
            if (((edobj_particle_mode != 0) || (edobj_sound_mode != 0)) && (edobj_nearest == i)) {
                instance->flags.onscreen = NuRndrGScnObj(scn->gobjs[instance->objid],&instance->mtx);
            }
            else if (instanim != NULL) {
                if (edobjRenderCutoffTest((struct nuvec_s *)&(instanim->mtx)._30) != 0) {
                    instance->flags.onscreen = NuRndrGScnObj(scn->gobjs[instance->objid],&instanim->mtx);
                }
            }
            else {
                if (edobjRenderCutoffTest((struct nuvec_s *)&ObjectInstance[i].mtx._30) != 0) {
                    instance->flags.onscreen = NuRndrGScnObj(scn->gobjs[instance->objid], &instance->mtx);
                }
            }
        }
    }
}

//NGC MATCH
void edobjResetAnimsToZero(void) {
  s32 i;
  struct nuinstance_s *instance;
  struct nuinstanim_s *instanim;
  
  instance = ObjectInstance;
  for(i = 0; i < 0x40; i++) {
    ObjectPath[i].particle_type[0] = -1;
    if (instance[i].objid != -1) {
    instanim = instance[i].anim;
        if(instanim != NULL) {
          instanim->tfirst = 0.0f;
          instanim->waiting = 1;
          instanim->ltime = ObjectPath[i].start_offset;
          instanim->playing = 0;
          instanim->backwards = 0;
        }
    }
  }
  return;
}

//NGC MATCH
s32 edobjLookupInstanceIndex(s32 specialid) {
  s32 i;
  
  if (edobj_base_scene == NULL) {
    return -1;
  }
    for(i = 0; i < edobj_base_scene->numinstance; i++) {
      if (&edobj_base_scene->instances[i] == edobj_base_scene->specials[specialid].instance) {
        return i;
      }
    }
  return -1;
}

//NGC MATCH
void edobjRegisterBaseScene(struct nugscn_s *scn) {
  edobj_base_scene = scn;
  return;
}

//NGC MATCH
void edobjDetermineNearestObject(float ndist) {
  s32 i;
  float dist;
  struct nuvec_s distv;
  
    if ((edobj_nearest != -1)) {
        NuVecSub(&distv,&edobj_cam_pos,(struct nuvec_s *)&ObjectInstance[edobj_nearest].mtx._30);
        dist = (distv.x * distv.x + distv.y * distv.y + distv.z * distv.z);
        if (dist == 0.0) return;
    }
    edobj_nearest = -1;
    for(i = 0; i < 0x40; i++) {
      if (ObjectInstance[i].objid != -1) {
        NuVecSub(&distv,&edobj_cam_pos,(struct nuvec_s *)&ObjectInstance[i].mtx._30);
        dist = distv.x * distv.x + distv.y * distv.y + distv.z * distv.z;
        if ((ndist < 0.0f) || (dist < ndist)) {
          ndist = dist;
          edobj_nearest = i;
        }
      }
    }
  return;
}

//NGC MATCH
u32 reverse_endian_32(u32 arg0) {
    return (arg0 >> 0x18U) | ((arg0 >> 8U) & 0xFF00) | ((arg0 << 8) & 0xFF0000) | (arg0 << 0x18);
}

//NGC MATCH
void edobjSoundDestroy(s32 obj,s32 sound) {
  s32 i;

    for(i = sound; i < (s32)ObjectPath[obj].usedsound + -1; i++) {
      ObjectPath[obj].sound_id[i] = ObjectPath[obj].sound_id[i + 1];
      ObjectPath[obj].sound_time[i] = ObjectPath[obj].sound_time[i + 1];
      ObjectPath[obj].sound_offset[i] = ObjectPath[obj].sound_offset[i + 1];
      strcpy(ObjectPath[obj].sound_name[i],ObjectPath[obj].sound_name[i + 1]);
    }
  ObjectPath[obj].usedsound--;
  return;
}

//NGC MATCH
void edobjParticleDestroy(s32 obj,s32 ptl) {
  s32 i;
  
    for(i = ptl; i < ObjectPath[obj].usedpart - 1; i++) {
      ObjectPath[obj].particle_type[i] = ObjectPath[obj].particle_type[i + 1];
      ObjectPath[obj].particle_rate[i] = ObjectPath[obj].particle_rate[i + 1];
      ObjectPath[obj].particle_switch[i] = ObjectPath[obj].particle_switch[i + 1];
      ObjectPath[obj].particle_offset[i] = ObjectPath[obj].particle_offset[i + 1];
      strcpy(ObjectPath[obj].particle_name[i],ObjectPath[obj].particle_name[i + 1]);
    }
  ObjectPath[obj].usedpart--;
  return;
}

//NGC MATCH
s32 edobjFileLoadObjects(char *file) {
  s32 i;
  s32 j;
  s32 version;
  s32 numobjects;
  char tname[20];
  s32 dummyreads;
  struct nuvec_s dummyvec;
  char dummyname[16];
  
  if (EdFileOpen(file,NUFILE_READ) != 0) {
    version = EdFileReadInt();
    if (version > 0xd) {
        EdFileClose();
        return 0;
    } 
  } else {
      return 0;
  }
    numobjects = EdFileReadInt();
    for(i = 0; i < numobjects; i++) {
          if (version == 1) {
            ObjectPath[i].objid = EdFileReadInt();
          }
          else {
            EdFileRead(tname,0x14);
            ObjectPath[i].objid = edobjLookupInstance(tname);
          }
          j = i * 0x3ec;
          if (ObjectPath[i].objid != -1) {
            ObjectInstance[i].objid = (edobj_base_scene->specials[ObjectPath[i].objid].instance)->objid;
          }
          else {
            ObjectInstance[i].objid = -1;
          }
          ObjectPath[i].speed = EdFileReadFloat();
          ObjectPath[i].oscillate = EdFileReadInt();
          if (version > 5) {
            ObjectPath[i].repeat = EdFileReadInt();
          }
          else {
            ObjectPath[i].repeat = 1;
          }
          ObjectPath[i].pause = EdFileReadFloat();
          ObjectPath[i].usedway = EdFileReadInt();
          if (version > 8) {
            ObjectPath[i].usedpart = EdFileReadInt();
          }
          else {
            ObjectPath[i].usedpart = 0;
          }
          if (version > 9) {
            ObjectPath[i].usedsound = EdFileReadInt();
          }
          else {
            ObjectPath[i].usedsound = 0;
          }
          ObjectPath[i].terrplatid = -1;
          if (version > 5) {
            ObjectPath[i].trigger_type = EdFileReadInt();
            ObjectPath[i].trigger_id = EdFileReadInt();
            ObjectPath[i].trigger_var = EdFileReadFloat();
          }
          else {
            ObjectPath[i].trigger_type = 0;
            ObjectPath[i].trigger_id = -1;
            ObjectPath[i].trigger_var = 0.0f;
          }
          if (version > 7) {
            ObjectPath[i].trigger_wait = EdFileReadFloat();
          }
          else {
            ObjectPath[i].trigger_wait = 0.0f;
          }
          if (version > 8) {
                dummyreads = 0;
                if (8 < ObjectPath[i].usedpart) {
                  dummyreads = 8 - ObjectPath[i].usedpart;
                  ObjectPath[i].usedpart = 8;
                }
                for(j = 0; j < ObjectPath[i].usedpart; j++) {
                    EdFileRead(ObjectPath[i].particle_name[j],0x10);
                    ObjectPath[i].particle_type[j] = -1;
                    ObjectPath[i].particle_rate[j] = EdFileReadInt();
                    ObjectPath[i].particle_switch[j] = EdFileReadInt();
                    EdFileRead(&ObjectPath[i].particle_offset[j],0xc);
                    if (version > 0xc) {
                      ObjectPath[i].particle_emitrotz[j] = EdFileReadShort();
                      ObjectPath[i].particle_emitroty[j] = EdFileReadShort();
                    }
                    else {
                      ObjectPath[i].particle_emitrotz[j] = 0;
                      ObjectPath[i].particle_emitroty[j] = 0;
                    }
                }
                for(j = 0; j < dummyreads; j++) {
                    EdFileRead(dummyname,0x10);
                    EdFileReadInt();
                    EdFileReadInt();
                    EdFileRead(&dummyvec,0xc);
                    if (version > 0xc) {
                      EdFileReadShort();
                      EdFileReadShort();
                    }
                }
          }
          else if (version > 6) {
              EdFileRead(ObjectPath[i].particle_name,0x10);
              ObjectPath[i].particle_type[0] = -1;
              ObjectPath[i].particle_rate[0] = EdFileReadInt();
              ObjectPath[i].particle_switch[0] = EdFileReadInt();
              if (ObjectPath[i].particle_name[0][0] != '\0') {
                ObjectPath[i].usedpart = 1;
              }
          }
          if (version > 4) {
            ObjectPath[i].start_offset = EdFileReadInt();
          }
          else {
            ObjectPath[i].start_offset = 0;
          }
          dummyreads = 0;
          if (8 < ObjectPath[i].usedway) {
            dummyreads = 8 - ObjectPath[i].usedway;
            ObjectPath[i].usedway = 8;
          }
          ObjectPath[i].usedtime = 0;
          for(j = 0; j < ObjectPath[i].usedway; j++) {
              EdFileRead(&ObjectPath[i].waypoint[j],0xc);
              ObjectPath[i].waypoint_speed[j] = EdFileReadFloat();
              if (version > 2) {
                EdFileRead(&ObjectPath[i].waypoint_rot[j],0xc);
              }
              else {
                memset(&ObjectPath[i].waypoint_rot[j],0,0xc);
              }
              if (version > 3) {
                ObjectPath[i].waypoint_time[j] = EdFileReadInt();
              }
              else {
                if (j < ObjectPath[i].usedway - 1) {
                  ObjectPath[i].waypoint_time[j] = 2;
                }
                else {
                  ObjectPath[i].waypoint_time[j] = 0;
                }
              }
              ObjectPath[i].usedtime += ObjectPath[i].waypoint_time[j];
          }
          for(j = 0; j < dummyreads; j++) {
              EdFileRead(&dummyvec,0xc);
              EdFileReadFloat();
              if (2 < version) {
                EdFileRead(&dummyvec,0xc);
              }
              if (3 < version) {
                EdFileReadInt();
              }
          }
          dummyreads = 0;
          ObjectPath[i].sound_last_time = 0.99f;
          if (8 < ObjectPath[i].usedsound) {
            dummyreads = 8 - ObjectPath[i].usedsound;
            ObjectPath[i].usedsound = 8;
          }
          if (version > 10) {
            for(j = 0; j < ObjectPath[i].usedsound; j++) {
                EdFileRead(ObjectPath[i].sound_name[j],0x10);
                ObjectPath[i].sound_id[j] = -1;
                ObjectPath[i].sound_type[j] = EdFileReadInt();
                ObjectPath[i].sound_time[j] = EdFileReadFloat();
                EdFileRead(&ObjectPath[i].sound_offset[j],0xc);
            }
            for(j = 0; j < dummyreads; j++) {
                EdFileRead(dummyname,0x10);
                EdFileReadInt();
                EdFileReadFloat();
                EdFileRead(&dummyvec,0xc);
            }
          }
          else if (version == 10) {
            for(j = 0; j < ObjectPath[i].usedsound; j++) {
                ObjectPath[i].sound_id[j] = EdFileReadFloat();
                ObjectPath[i].sound_type[j] = 1;
                ObjectPath[i].sound_time[j] = EdFileReadFloat();
                EdFileRead(&ObjectPath[i].sound_offset[j],0xc);
            }
            for(j = 0; j < dummyreads; j++) {
                EdFileReadFloat();
                EdFileReadFloat();
                EdFileRead(&dummyvec,0xc);
            }
          }
          if (version > 0xb) {
            ObjectPath[i].playergrav = EdFileReadFloat();
            ObjectPath[i].tension = EdFileReadFloat();
            ObjectPath[i].damping = EdFileReadFloat();
          }
          else {
            ObjectPath[i].playergrav = 0.0f;
            ObjectPath[i].tension = 0.0f;
            ObjectPath[i].damping = 0.0f;
          }
      }
      EdFileClose();
      for(i = 0; i < numobjects; i++) {
          if (ObjectPath[i].objid != -1) {
            edobjConvertPathToAnim(i);
          }
      }
      edobjResetAnimsToZero();
      edobj_next_instance = numobjects;
      edobj_waypoint_mode = 0;
      edobj_copy_mode = 0;
      edobj_particle_mode = 0;
      edobj_sound_mode = 0;
      edobj_nearest = -1;
      edobjDetermineNearestObject(1.0f);
      return 1;
}

//NGC MATCH
void edobjObjectReset(void) {
  s32 i;

  for(i = 0; i < 64; i++) {
    ObjectInstance[i].objid = -1;
    ObjectPath[i].objid = -1;
  }
  edobj_next_instance = 0.0f;
  return;
}

//NGC MATCH
s32 edobjLookupInstance(char *name) {
  s32 i;
  
  if (edobj_base_scene != NULL) {
    for(i = 0; i < edobj_base_scene->numspecial; i++) {
        if (strncmp(edobj_base_scene->specials[i].name,name,0x13) == 0) {
            return i;
        }
    }
  }
  return -1;
}