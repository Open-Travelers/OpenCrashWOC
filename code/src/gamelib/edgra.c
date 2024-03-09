//#include "edgra.h"
#include "../nu.h"

//NGC MATCH
void edgraInitAllClumps(void) {
  struct nuvec4_s pos[256];
  s32 i;
  s32 j;
  s32 a;
  float d;
  float r;
  
  NuWindInit();
  edgra_units_used = 0;
  edgra_clumps_used = 0;
   for(i = 0; i < 0x40; i++) {
    if (GrassClumps[i].num != 0) {
      for(j = 0; j < GrassClumps[i].num; j++) {
          a = edqrand();
          d = (float)edqrand() * (GrassClumps[i].radius * 0.000015258789f);
          pos[j].x = NuTrigTable[a & 0xffff] * d;
          pos[j].y = 0.0f;
          pos[j].z = NuTrigTable[(a + 0x4000) & 0x3fffcU / 4] * d;
          d = NuFsqrt(pos[j].x * pos[j].x + pos[j].z * pos[j].z);
          //j++;
          pos[j].x *= (d + 1.5f);
          pos[j].z *= (d + 1.5f);
          r = (s32)(edqrand() - 0x4000U) * 0.000015258789f;
          d *= 1.25f;
          pos[j].w = r * r + (1.2f - d);
          if (pos[j].w < 0.2f) {
            pos[j].w = 0.2f;
          }
          pos[j].x += GrassClumps[i].centre.x;
          pos[j].y += GrassClumps[i].centre.y;
          pos[j].z += GrassClumps[i].centre.z;
      }
      NuWindCreate(edbits_base_scene->specials[GrassClumps[i].id].instance,pos,
                   GrassClumps[i].num,GrassClumps[i].wind, GrassClumps[i].height,GrassClumps[i].reactive);
      edgra_units_used = edgra_units_used + GrassClumps[i].num;
      edgra_clumps_used++;
    }
  }
  return;
}

//NGC MATCH
void edgraClumpsReset(void) {
  s32 i;
  
  for(i = 0; i < 0x40; i++) {
    GrassClumps[i].num = 0;
  }
  edgra_next_clump = 0;
  return;
}

//NGC MATCH
void edgraDetermineNearestClump(float ndist) {
  float dist;
  s32 i;
  struct nuvec_s distv;
  
    if ((edgra_nearest != -1)) {
        NuVecSub(&distv,&edgra_cam_pos,&GrassClumps[edgra_nearest].centre);
        dist = (distv.x * distv.x + distv.y * distv.y + distv.z * distv.z);
        if (dist == 0.0) return;
    }
    edgra_nearest = -1;
    for(i = 0; i < 0x40; i++) {
        if (GrassClumps[i].num != 0) {
            NuVecSub(&distv,&edgra_cam_pos,&GrassClumps[i].centre);
            dist = (distv.x * distv.x + distv.y * distv.y + distv.z * distv.z);
            if ((ndist < 0.0f) || (dist < ndist)) {
                ndist = dist;
                edgra_nearest = i;
            }
        }
    }
  return;
}

//NGC MATCH
s32 edgraFileLoad(char *file) {
  s32 i;
  s32 version;
  s32 numclumps;
  char tname[20];
  
      if (EdFileOpen(file,NUFILE_READ) != 0) {
        version = EdFileReadInt();
        if (version > 3) {
            EdFileClose();
            return 0;
        }
      } else {
          return 0;
      }
      numclumps = EdFileReadInt();
      for(i = 0; i < numclumps; i++) {
          EdFileRead(tname,0x14);
          GrassClumps[i].id = edbitsLookupInstance(tname);
          GrassClumps[i].num = EdFileReadInt();
          if (GrassClumps[i].id == -1) {
            GrassClumps[i].num = 0;
          }
          EdFileRead(&GrassClumps[i].centre,0xc);
          GrassClumps[i].radius = EdFileReadFloat();
          GrassClumps[i].wind = EdFileReadFloat();
          if (version > 1) {
            GrassClumps[i].reactive = EdFileReadInt();
          }
          else {
            GrassClumps[i].reactive = 1;
          }
          if (version > 2) {
            GrassClumps[i].height = EdFileReadFloat();
          }
          else {
            GrassClumps[i].height = 1.0f;
          }
      }
      EdFileClose();
      edgraInitAllClumps();
      edgra_next_clump = numclumps;
      edgra_nearest = -1;
      edgraDetermineNearestClump(1.0f);
    return 1;
}