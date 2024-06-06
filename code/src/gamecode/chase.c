//NGC MATCH
struct nugspline_s * NuSplineFindPartial(struct nugscn_s *scene,char *name,char *txt) {
  struct nugspline_s *spl;
  s32 i;
  
  spl = scene->splines;
  for(i = 0; i < scene->numsplines; i++) {
      if (strncasecmp(name,spl->name,strlen(name)) == 0) {
        strcpy(txt,spl->name);
        return spl;
      }
      spl++;
  }
  return NULL;
}

//NGC MATCH
s32 ChaseActive(void) {
  s32 i;

  for(i = 0; i < 3; i++) {
    if (Chase[i].status == 2) {
      return i;
    }
  }
  return -1;
}

//NGC MATCH
float NearestChaserDistance(struct chase_s *chase,struct obj_s *obj) {
  s32 iVar1;
  s32 iVar2;
  float d0;
  float d;
  
  d0 = 100.0f;
  for(iVar2 = 0; iVar2 < 6; iVar2++) {
    if (chase->ok[iVar2] != 0) {
      if ((best_cRPos != NULL) && (FurtherALONG((s32)chase->RPos[iVar2].iRAIL,(s32)chase->RPos[iVar2].iALONG,
                 chase->RPos[iVar2].fALONG,(s32)best_cRPos->iRAIL,(s32)best_cRPos->iALONG,best_cRPos->fALONG) != 0)) {
        return 0.0f;
      }
      d = NuVecXZDistSqr(&obj->pos,&chase->pos[iVar2],NULL);
      if (d < d0) {
        d0 = d;
      }
    }
  }
    return NuFsqrt(d0);
}

/* UPDATECHASE LOCAL VAR
    nuvec_s oldpos; // 0x8(r1)
    nuvec_s* p0; // r9
    nuvec_s* p1; // r11
    nuvec_s pos; // 0x18(r1)
    CharacterModel* model; // r3
    float y; // f31
    float t; // f31
    float tmul; // f28
    float d; // f29
    float f; // f30
    int i; // r21
    int j; // r30
    int k; // r28
    int l; // r31
    int holding; // 0x1F0(r1)
    int kill; // r9
    int cuboid; // 
    int count; // 0x1F4(r1)
    int sfx; // 0x1F8(r1)
    // Size: 0x188
    struct obj_s  c_obj;
    struct CData_s * cdata; // r28
*/