//NGC MATCH
void ResetCrateType2(CrateCube *crt) {
  crt->type2 = crt->type1;
  if (crt->model == NULL) {
    return;
  }
  crt->model->type[1] = crt->type1;
  return;
}

//NGC MATCH
struct crate_s * NextCrate(struct crate_s *a) {
  return (struct crate_s *)NuLstGetNext(crates,(struct nulnkhdr_s *)a);
}

//NGC MATCH
void InitCrates(void) {
  s32 iVar2;
  s32 iVar4;
  
  crates = NuLstCreate(0x100,0x28);
  CRATECOUNT = 0;
  CRATEGROUPCOUNT = 0;
  if (crate_scene != NULL) {
    for(iVar2 = 0; iVar2 < 28; iVar2++) {
      NuSpecialFind(crate_scene,(struct nuhspecial_s* )&crate_list[iVar2].obj,crate_list[iVar2].name);
    }
  }
  else {
    for(iVar2 = 0; iVar2 < 0x1d; iVar2++) {
      crate_list[iVar2].obj.special = 0;
    }
  }
  MarkerCrate.type[0] = 0x1a;
  marker_crate = &MarkerCrate;
  LockCrate.type[0] = 0x1b;
  lock_crate = &LockCrate;
  HighlightCrate.type[0] = 0x1c;
  highlight_crate = &HighlightCrate;
  FlashCrate.type[0] = (char)current_selected_crate;
  flash_crate = &FlashCrate;
  locked_crate = 0;
  triggerorigin_crate = 0;
  triggerdest_crate = 0;
  highlighted_crate = 0;
  return;
}

//NGC MATCH
void CloseCrates(void) {
  if (crates != NULL) {
    NuLstDestroy(crates);
    crates = NULL;
  }
  return;
}

//NGC MATCH
s32 ReadCrateData(void) {
  s32 handle;
  s32 i;
  s32 iVar7;
  s32 version;
  void * fbuff;
  
  CRATECOUNT = 0;
  sprintf(tbuf,"%s.crt",LevelFileName);
  if (NuFileExists(tbuf) != 0) {
    fbuff = Chase;
    handle = NuMemFileOpen(fbuff,NuFileLoadBuffer(tbuf,fbuff,0x7fffffff),NUFILE_READ);
    if (handle != 0) {
      version = NuFileReadInt(handle);
      if (version > 5) {
          NuFileClose(handle);
          return 0;
      }
      CRATEGROUPCOUNT = (s32)NuFileReadShort(handle);
      for(i = 0; i < CRATEGROUPCOUNT; i++) {
            CrateGroup[i].origin.x = NuFileReadFloat(handle);
            CrateGroup[i].origin.y = NuFileReadFloat(handle);
            CrateGroup[i].origin.z = NuFileReadFloat(handle);
            CrateGroup[i].radius = 0.0f;
            CrateGroup[i].iCrate = NuFileReadShort(handle);
            CrateGroup[i].nCrates = NuFileReadShort(handle);
            CRATECOUNT = CRATECOUNT + CrateGroup[i].nCrates;
            CrateGroup[i].angle = NuFileReadShort(handle);
            for (iVar7 = (s32)CrateGroup[i].iCrate; iVar7 < (s32)CrateGroup[i].iCrate + (s32)CrateGroup[i].nCrates; iVar7++) {
                Crate[iVar7].pos0.x = NuFileReadFloat(handle);
                Crate[iVar7].pos0.y = NuFileReadFloat(handle);
                Crate[iVar7].pos0.z = NuFileReadFloat(handle);
                Crate[iVar7].shadow = NuFileReadFloat(handle);
                Crate[iVar7].dx = NuFileReadShort(handle);
                Crate[iVar7].dy = NuFileReadShort(handle);
                Crate[iVar7].dz = NuFileReadShort(handle);
                Crate[iVar7].type1 = NuFileReadChar(handle);
                if (version > 2) {
                  Crate[iVar7].type2 = NuFileReadChar(handle);
                  Crate[iVar7].type3 = NuFileReadChar(handle);
                  Crate[iVar7].type4 = NuFileReadChar(handle);
                }
                else {
                  Crate[iVar7].type2 = -1;
                  Crate[iVar7].type3 = -1;
                  Crate[iVar7].type4 = -1;
                }
                Crate[iVar7].timer = 0.0f;
                Crate[iVar7].on = 1;
                Crate[iVar7].iU = NuFileReadShort(handle);
                Crate[iVar7].iD = NuFileReadShort(handle);
                Crate[iVar7].iN = NuFileReadShort(handle);
                Crate[iVar7].iS = NuFileReadShort(handle);
                Crate[iVar7].iE = NuFileReadShort(handle);
                Crate[iVar7].iW = NuFileReadShort(handle);
                if (version > 3) {
                  Crate[iVar7].trigger = NuFileReadShort(handle);
                }
                else {
                  Crate[iVar7].trigger = -1;
                }
            }
      }
      NuFileClose(handle);
      return 1;        
    }
  }
  return 0;
}

//NGC MATCH
s32 NewCrateAnimation(CrateCube* crate, s32 type, s32 action, s32 random) {
    struct CharacterModel* model;
    s32 i;
    s32 character;

    if ((u32)type > 0x14) {
        return 0;
    }
    
    character = crate_list[type].character;
    crate->character = -1;
    crate->action = -1;
    
    if (((u32)character > 0xbe) || ((u32)action > 0x75)) {
        return 0;
    }

    i = CRemap[character];
    if (i == -1) {
        return 0;
    }
    
    model = &CModel[i];
    if (model->anmdata[action] != NULL) {
        i = 1;
        crate->anim_duration = model->anmdata[action]->time;
        crate->anim_cycle = model->animlist[action]->flags & 1;
        crate->anim_speed = model->animlist[action]->speed;
    } else if (model->fanmdata[action] != NULL) {
        i = 1;
        crate->anim_duration = model->fanmdata[action]->time;
        crate->anim_cycle = model->fanimlist[action]->flags & 1;
        crate->anim_speed = model->fanimlist[action]->speed;
    } else {
        i = 0;
    }

    if (i != 0) {
        crate->anim_time = 1.0f;
        if (random != 0) {
            crate->anim_time = (s32)qrand() * 0.000015259022f * (crate->anim_duration - 1.0f) + crate->anim_time;
            if (crate->anim_time >= crate->anim_duration) {
                crate->anim_time = 1.0f;
            }
        }
        crate->character = character;
        crate->action = action;
    }
    return i;
}

//NGC MATCH
void OpenPreviousCheckpoints(s32 iRAIL,s32 iALONG,float fALONG) {
  CrateCubeGroup *group;
  CrateCube *crate;
  s32 i;
  s32 j;

  group = CrateGroup;
  for(i = 0; i < CRATEGROUPCOUNT; i++, group++) {
      crate = &Crate[group->iCrate];
      for(j = 0; j < group->nCrates; j++, crate++) {
          if ((((crate->on != 0) && (crate->type1 == 7)) && (crate->iRAIL != -1)) &&
             (FurtherBEHIND(crate->iRAIL,crate->iALONG,crate->fALONG,iRAIL,iALONG,fALONG) != 0)) {
            crate->on = 0;
            if (crate->model != NULL) {
              crate->model->draw = 0;
            }
            NewCrateAnimation(crate,7,0x34,0);
          }
      }
  }
  return;
}

//NGC MATCH
void HeightSortCrateData(void) {
  s32 i;
  s32 j;
  s32 l;
  s32 k;
  CrateCube tcr;
  CrateCubeGroup* crg;

  for(i = 0; i < CRATEGROUPCOUNT; i++) {
   crg = &CrateGroup[i];
      for(j = crg->iCrate; j < (s32)crg->iCrate + (s32)crg->nCrates - 1; j++) {
          for(k = j + 1; k < (crg->iCrate + crg->nCrates); k++) {
              if (Crate[j].dy > Crate[k].dy) {
                tcr = Crate[k];
                Crate[k] = Crate[j];
                Crate[j] = tcr;
                for(l = 0; l < CRATECOUNT; l++) {
                    if (Crate[l].trigger == j) {
                      Crate[l].trigger = k;
                    }
                    else if (Crate[l].trigger == k) {
                      Crate[l].trigger = j;
                    }
                }
              }
          }
      }
  }
  return;
}

//NGC MATCH
void StartExclamationCrateSequence(CrateCubeGroup *group,CrateCube *crate) {
  struct nuvec_s pos;
  
  pos.x = (crate->pos).x;
  pos.y = (crate->pos).y + 0.25f;
  pos.z = (crate->pos).z;
  temp_pGroup = group;
  temp_pCrate = crate;
  AddKaboom(0x20,&pos,0.0f);
  crate->newtype = 0xf;
  crate->metal_count = 1;
  GameSfx(0x35,&temp_pCrate->pos);
  return;
}

//NGC MATCH
void SaveCrateTypeData(CrateCube *crate) {
  CRATETYPEDATA *data;
  
  if (i_cratetypedata > 0x1f) {
    return;
  }
  data = &CrateTypeData[i_cratetypedata];
  data->crate = crate;
  data->type1 = crate->type1;
  data->type2 = crate->type2;
  data->type3 = crate->type3;
  data->type4 = crate->type4;
  i_cratetypedata++;
  return;
}

//NGC MATCH
void RestoreCrateTypeData(void) {
  s32 i;
  CRATETYPEDATA *data;
  
  data = CrateTypeData;
  for(i = 0; i < i_cratetypedata; i++, data++) {
      data->crate->type1 = data->type1;
      data->crate->type2 = data->type2;
      data->crate->type3 = data->type3;
      data->crate->type4 = data->type4;
  }
  i_cratetypedata = 0;
  return;
}

//NGC MATCH
void AddExtraLife(struct nuvec_s *pos,int pdeb) {
  struct nuvec_s scr_pos;
  struct nuvec_s cV[2];
  float scale;
  
  NuCameraTransformScreenClip(&scr_pos,pos,1,NULL);
  cV[0].x = GameCam[0].vX.x * 0.1f + pos->x;
  cV[0].y = GameCam[0].vX.y * 0.1f + pos->y;
  cV[0].z = GameCam[0].vX.z * 0.1f + pos->z;
  NuCameraTransformScreenClip(&cV[1],&cV[0],1,NULL);
  scale = NuFabs((scr_pos.x - cV[1].x));
  AddPanelDebris(scr_pos.x,scr_pos.y,pdeb,(scale * 3.6363637f),1);
  return;
}

//NGC MATCH
CrateCube * CrateInSlot(CrateCubeGroup *group,s32 x,s32 y,s32 z) {
  CrateCube *crate;
  s32 i;
  
  crate = &Crate[group->iCrate];
  for(i = 0; i < group->nCrates; i++, crate++) {
      if (((crate->dx == x) && (crate->dy == y)) && (crate->dz == z)) {
        return crate;
      }
  }
  return NULL;
}

//NGC MATCH
void ResetInvisibility(void) {
  plr_invisibility_time = 5.0f;
  glass_mix = (Level != 0x17) ? 0.0f : WATERBOSSGLASSMIX;
  glass_col_mix = 0;
  glass_col_enabled = 0;
  glass_enabled = 0;
  return;
}

//NGC MATCH
void ResetCrate(CrateCube *crt) {
  crt->mom = 0.0f;
  crt->oldy = crt->pos0.y;
  crt->pos.y = crt->pos0.y;
  crt->newtype = -1;
  crt->subtype = -1;
  if (((crt->type1 == 6) || (crt->type2 == 6)) || ((crt->type1 == 0 && (crt->type3 == 6)))) {
    crt->counter = 0xa;
  }
  else {
    crt->counter = 0;
  }
  crt->metal_count = 0;
  crt->action = -1;
  crt->appeared = 0;
  return;
}

//NGC MATCH
void InitCrateExplosions(void) {
  s32 i;
  
  iBOXEXP = 0;
  for(i = 0; i < 0x10; i++) {
    BoxExpList[i].time = 0;
  }
  return;
}

//NGC MATCH
void UpdateCrateExplosions(void) {
  BoxExpType *box;
  BoxPolType *face;
  s32 i;
  s32 j;
  s32 k;
  s32 bVar6;
  
  box = BoxExpList;
  for(i = 0; i < 0x10; i++, box++) {
    if (box->time != 0) {
      box->time--;
      if (box->time != 0) {
          face = BoxExpList[i].BoxPol;
          for(j = 0; j < 5; j++) {
                for(k = 0; k < 6; k++, face++) {
                  bVar6 = 0;
                  face->pos.x = face->pos.x + face->mom.x;
                  face->pos.y = face->pos.y + face->mom.y;
                  face->mom.y = face->mom.y - 0.004f;
                  face->pos.z = face->pos.z + face->mom.z;
                  (face->ang).x = (face->ang).x + face->angmom.x;
                  (face->ang).y = (face->ang).y + face->angmom.y;
                  (face->ang).z = (face->ang).z + face->angmom.z;
                  if ((face->pos.x < box->colbox[0].x) && (face->mom.x < 0.0f)) {
                    face->pos.x = box->colbox[0].x;
                    bVar6 = 1;
                    face->mom.x = -face->mom.x * 0.8f;
                  }
                  if ((face->pos.y < box->colbox[0].y) && (face->mom.y < 0.0f)) {
                    face->pos.y = box->colbox[0].y;
                    bVar6 = 1;
                    face->mom.y = -face->mom.y * 0.8f;
                  }
                  if ((face->pos.z < box->colbox[0].z) && (face->mom.z < 0.0f)) {
                    face->pos.z = box->colbox[0].z;
                    bVar6 = 1;
                    face->mom.z = -face->mom.z * 0.8f;
                  }
                  if (face->pos.x > (box->colbox[1].x) && (0.0f < face->mom.x)) {
                    face->pos.x = box->colbox[1].x;
                    bVar6 = 1;
                    face->mom.x = -face->mom.x * 0.8f;
                  }
                  if (face->pos.z > (box->colbox[1].z) && (0.0f < face->mom.z)) {
                    face->pos.z = box->colbox[1].z;
                    bVar6 = 1;
                    face->mom.z = -face->mom.z * 0.8f;
                  }
                  if (bVar6) {
                    face->angmom.x = -face->angmom.x;
                    face->angmom.y = -face->angmom.y;
                    face->angmom.z = -face->angmom.z;
                  }
                }
          }
      }
    }
  }
  return;
}

//NGC MATCH
void DestroyAllNitroCrates(CrateCubeGroup *group,CrateCube *crate) {
  CrateCubeGroup *group2;
  CrateCube *crate2;
  struct nuvec_s pos;
  s32 i;
  s32 j;
  
  group2 = CrateGroup;
  for(i = 0; i < CRATEGROUPCOUNT; i++, group2++) {
      crate2 = Crate + group2->iCrate;
      for(j = 0; j < group2->nCrates; j++, crate2++) {
          if ((crate2->on != 0) && (GetCrateType(crate2,0) == 0x10)) {
            CrateOff(group2,crate2,0,0);
          }
      }
  }
  pos.x = (crate->pos).x;
  pos.y = (crate->pos).y + 0.25f;
  pos.z = (crate->pos).z;
  temp_pGroup = group;
  temp_pCrate = crate;
  AddKaboom(0x20,&pos,0.0f);
  crate->newtype = 0xf;
  crate->metal_count = 1;
  JudderGameCamera(GameCam,0.5f,NULL);
  GameSfx(0x33,&crate->pos);
  return;
}

//NGC MATCH
s32 AttackCrate(struct obj_s *obj,CrateCubeGroup *group,CrateCube *crate) {
  s32 type;
  
  type = GetCrateType(crate,0);
  if (type == -1) {
    return 1;
  }
    if (type == 0xe) {
      if ((crate->action == -1) && (NewCrateAnimation(crate,0xe,0x35,0) == 0)) {
        StartExclamationCrateSequence(group,crate);
      }
      return 2;
    }
    if (type == 0x11) {
        if ((crate->action == -1) && (NewCrateAnimation(crate,0x11,0x35,0) == 0)) {
            DestroyAllNitroCrates(group,crate);
        }
        return 2;
    }
    if (CrateOff(group,crate,0,obj->attack >> 9 & 1) != 0) {
        if ((type == 9) || (type == 0x10)) {
                KillPlayer(obj,GetDieAnim(obj,6));   
        }
        return 1;
    }
  return 0;
}

//NGC MATCH
s32 LowestCrate(CrateCubeGroup *group,CrateCube *crate) {
  CrateCube *crate2;
  s32 i;
  s32 dx;
  s32 dz;
  
  crate2 = &Crate[group->iCrate];
  dx = crate->dx;
  dz = crate->dz;
  for(i = 0; i < group->nCrates; i++, crate2++) {
      if ((((crate2 != crate) && (crate2->dx == dx)) && (crate2->dz == dz)) &&
         ((crate2->pos0).y < (crate->pos0).y)) {
        return 0;
      }
  }
  return 1;
}

//NGC MATCH
s32 LowestActiveCrate(CrateCubeGroup *group,CrateCube *crate) {
  CrateCube *crate2;
  s32 i;
  s32 dx;
  s32 dz;
  
  crate2 = &Crate[group->iCrate];
  dx = crate->dx;
  dz = crate->dz;
  for(i = 0; i < group->nCrates; i++, crate2++) {
      if ((((crate2 != crate) && (crate2->on != 0)) && (crate2->dx == dx)) &&
         ((crate2->dz == dz && ((crate2->pos).y < (crate->pos).y)))) {
        return 0;
      }
  }
  return 1;
}

//NGC MATCH
s32 CrateInTheWay(struct obj_s *obj,CrateCubeGroup *group,CrateCube *crate,s32 dx,s32 dz,char *hit) {
  CrateCube *crate2;
  s32 i;
  
  crate2 = &Crate[group->iCrate];
  for(i = 0; i < group->nCrates; i++, crate2++) {
      if ((((crate2 != crate) && (hit[i] == 1)) && (crate2->dx == dx)) &&
         (crate2->dz == dz)) {
        return 1;
      }
  }
  return 0;
}

//NGC MATCH
s32 CrateOnTop(CrateCubeGroup *group,CrateCube *crate) {
  s32 i;
  CrateCube *crate2;
  
  crate2 = &Crate[group->iCrate];
  for(i = 0; i < group->nCrates; i++, crate2++) {
      if ((((crate2 != crate) && (crate2->on != 0)) && (GetCrateType(crate2,0) != 0)) &&
         (((crate2->dx == crate->dx && (crate2->dz == crate->dz)) &&
          (crate2->pos.y == crate->pos.y + 0.5f)))) {
        return 1;
      }
  }
  return 0;
}

//NGC MATCH
void BreakCrate(CrateCubeGroup *group,CrateCube *crate,s32 type,s32 attack) {
  if ((type == 0x13) && ((attack & 0x200U) == 0)) {
    if (crate->action == -1) {
      NewCrateAnimation(crate,0x13,0x58,0);
      GameSfx(0x39,&crate->pos);
    }
  }
  else if (type == 0xe) {
    if ((crate->action == -1) && (NewCrateAnimation(crate,0xe,0x35,0) == 0)) {
      StartExclamationCrateSequence(group,crate);
    }
  }
  else if (type == 0x11) {
    if ((crate->action == -1) && (NewCrateAnimation(crate,0x11,0x35,0) == 0)) {
      DestroyAllNitroCrates(group,crate);
    }
  }
  else {
    if (CrateOff(group,crate,0,(attack >> 9) & 1) != 0) {
      HopCratesAbove(CRATEHOPSPEED,group,crate);
    }
  }
  return;
}

//NGC MATCH
s32 HitCrates(struct obj_s *obj,s32 destroy) {
  if ((level_part_2 == 0) &&
     (InCrate(obj->pos.x,obj->pos.z,(obj->top * obj->SCALE + obj->pos.y),(obj->bot * obj->SCALE + obj->pos.y),obj->RADIUS) != 0)) {
        if ((destroy == 1) || ((((destroy == 2 && (temp_crate_type != 7)) && (temp_crate_type != 0xe)) && (temp_crate_type != 0x11)))) {
          BreakCrate(temp_pGroup,temp_pCrate,temp_crate_type,(uint)obj->attack);
        }
        return 1;
  }
  else {
    return 0;
  }
}

//NGC MATCH
s32 WipeCrates(s32 iRAIL0,s32 iALONG0,float fALONG0,s32 iRAIL1,s32 iALONG1,float fALONG1,s32 destroy) {
  CrateCubeGroup *group;
  CrateCube *crate;
  s32 i;
  s32 j;
  s32 type;

  group = CrateGroup;
  for(i = 0; i < CRATEGROUPCOUNT; i++, group++) {
      crate = &Crate[group->iCrate];
      for(j = 0; j < group->nCrates; j++, crate++) {
        if (crate->on != 0) {
        type = GetCrateType(crate,0);
            if((u32)(type + 1) > 1 && 
                (FurtherALONG(crate->iRAIL,crate->iALONG,crate->fALONG,iRAIL0,iALONG0,fALONG0) != 0) &&
                  (FurtherALONG(iRAIL1,iALONG1,fALONG1,crate->iRAIL,crate->iALONG,crate->fALONG) != 0) &&
                 ((destroy == 1) || ((destroy == 2 && (type != 7)) && (type != 0xe && (type != 0x11))))) {
                BreakCrate(group,crate,type,0x200);
            }
        }
      }
  }
  return 0;
}

//NGC MATCH
CrateCube * HitCrateBalloons(struct nuvec_s *pos,float radius) {
  CrateCubeGroup *group;
  CrateCube *crate;
  float r2;
  s32 iVar1;
  s32 iVar3;
  struct nuvec_s v;
  struct nuvec_s d;
  
  temp_pGroup = NULL;
  temp_pCrate = NULL;
  if (level_part_2 != 0) {
    return temp_pCrate;
  }
    group = CrateGroup;
    r2 = (radius + CRATEBALLOONRADIUS);
    r2 *= r2;
    for(iVar1 = 0; iVar1 < CRATEGROUPCOUNT; iVar1++, group++) {
        crate = Crate + group->iCrate;
        for(iVar3 = 0; iVar3 < group->nCrates; iVar3++, crate++) {
            if ((crate->on != 0) && ((crate->flags & 0x400) != 0)) {
              v.x = (crate->pos).x;
              v.y = (crate->pos).y + CRATEBALLOONOFFSET;
              v.z = (crate->pos).z;
              NuVecSub(&d,pos,&v);
              if ((d.x * d.x + d.y * d.y + d.z * d.z) < r2) {
                temp_crate_type = GetCrateType(crate,0);
                temp_pGroup = group;
                temp_pCrate = crate;
                GameSfx(0x50,&v);
                return temp_pCrate;
              }
            }
        }
    }
  return temp_pCrate;
}

//NGC MATCH
s32 RayIntersectCuboid(struct nuvec_s *p0,struct nuvec_s *p1,struct nuvec_s *min,struct nuvec_s *max) {
  float new_ratio;
  float dx;
  float dy;
  float dz;
  s32 face;
  struct nuvec_s v;
  
  dx = (p1->x - p0->x);
  dy = (p1->y - p0->y);
  dz = (p1->z - p0->z);
  if ((p0->x <= min->x) && (p1->x >= min->x)) {
    new_ratio = ((min->x - p0->x) / dx);
    face = 8;
    v.x = min->x;
  }
  else {
    if ((p0->x >= max->x) && (p1->x <= max->x)) {
      new_ratio = ((max->x - p0->x) / dx);
      face = 4;
      v.x = max->x;
    }
    else {
      face = 0;
    }
  }
  if (face != 0) {
    v.z = (dz * new_ratio + p0->z);
    if ((v.z >= min->z) && (v.z <= max->z)) {
        v.y = (dy * new_ratio + p0->y);
         if ((v.y >= min->y) && (v.y <= max->y)) {
            temp_ratio = new_ratio;
            temp_face = face;
            vTEMP = v;
            return 1;
        }
    }
  }
    if ((p0->z <= min->z) && (p1->z >= min->z)) {
      new_ratio = ((min->z - p0->z) / dz);
      face = 2;
      v.z = min->z;
    }
    else {
      if ((p0->z >= max->z) && (p1->z <= max->z)) {
        new_ratio = ((max->z - p0->z) / dz);
        face = 1;
        v.z = max->z;
      }
      else {
        face = 0;
      }
    }
    if (face != 0) {
      v.x = (dx * new_ratio + p0->x);
      if ((v.x >= min->x) && (v.x <= max->x)) {
        v.y = (dy * new_ratio + p0->y);
        if ((v.y >= min->y) && (v.y <= max->y)) {
            temp_ratio = new_ratio;
            temp_face = face;
            vTEMP = v;
            return 1;
        }
      }
    }
    if ((p0->y <= min->y) && (p1->y >= min->y)) {
      new_ratio = ((min->y - p0->y) / dy);
      face = 0x20;
      v.y = min->y;
    }
    else {
      if ((p0->y >= max->y) && (p1->y <= max->y)) {
        new_ratio = ((max->y - p0->y) / dy);
        face = 0x10;
        v.y = max->y;
      }
      else {
        face = 0;
      }
    }
    if (face != 0) {
      v.x = (dx * new_ratio + p0->x);
      if ((v.x >= min->x) && (v.x <= max->x)) {
        v.z = (dz * new_ratio + p0->z);
        if ((v.z >= min->z) && (v.z <= max->z)) {
            temp_ratio = new_ratio;
            temp_face = face;
            vTEMP = v;
            return 1;
        }
      }
    }
  return 0;
}

//NGC MATCH
s32 CrateRayCast(struct nuvec_s *p0,struct nuvec_s *p1) {
  struct nuvec_s vMIN;
  struct nuvec_s vMAX;
  struct nuvec_s v0;
  struct nuvec_s v1;
  struct nuvec_s min;
  struct nuvec_s max;
  s32 i;
  s32 j;
  s32 face;
  s32 type;
  float ratio;
  CrateCubeGroup *group;
  CrateCube *crate;
  
  ratio = 1.0f;
  vMIN.x = (p0->x < p1->x) ? p0->x : p1->x;
  vMIN.z = (p0->z < p1->z) ? p0->z : p1->z;
  vMAX.x = (p0->x > p1->x) ? p0->x : p1->x;
  vMAX.z = (p0->z > p1->z) ? p0->z : p1->z;
  group = CrateGroup;
  for(i = 0; i < CRATEGROUPCOUNT; i++, group++) {
      if ((((vMAX.x >= group->minclip.x) && (vMIN.x <= group->maxclip.x)) &&
          (vMAX.z >= group->minclip.z )) && (vMIN.z <= group->maxclip.z)) {
        v0.x = p0->x - (group->origin).x;
        v0.y = p0->y;
        v0.z = p0->z - (group->origin).z;
        NuVecRotateY(&v0,&v0,-(uint)group->angle);
        v1.x = p1->x - (group->origin).x;
        v1.y = p1->y;
        v1.z = p1->z - (group->origin).z;
        NuVecRotateY(&v1,&v1,-(uint)group->angle);
        crate = &Crate[group->iCrate];
        for(j = 0; j < group->nCrates; j++, crate++) {
           if ((crate->on != 0) && (crate->in_range != 0)) {
                type = GetCrateType(crate,0) + 1;
                 if ((u32)type > 1) {
                  min.x = ((s32)crate->dx * 0.5f);
                  min.y = crate->pos.y;
                  min.z = (crate->dz * 0.5f);
                  max.x = (min.x + 0.5f);
                  max.y = min.y + 0.5f;
                  max.z = (min.z + 0.5f);
                  if ((RayIntersectCuboid(&v0,&v1,&min,&max) != 0) && (temp_ratio < ratio)) {
                    face = temp_face;
                    ratio = temp_ratio;
                  }
                }
            }
        }
      }
  }
  temp_face = face;
  temp_ratio = (float)ratio;
  if (ratio < 1.0f) {
      return 1;
  }
  return 0;
}

//NGC MATCH
s32 GotoCheckpoint(struct nuvec_s *pos,s32 direction) {
  CrateCubeGroup *group;
  CrateCube *crate;
  s32 i;
  s32 j;
  s32 iRAIL;
  s32 iALONG;
  float fALONG;
  
  cp_goto = -1;
  if ((best_cRPos == NULL) || ((u32)direction > 1)) {
      return 0;
  }
    group = CrateGroup;
    iRAIL = -1;
    for(i = 0; i < CRATEGROUPCOUNT; i++, group++) {
        crate = Crate + group->iCrate;
        for(j = 0; j < group->nCrates; j++, crate++) {
            if ((((crate->type1 == 7) && (NuVecDist(pos,&crate->pos0,NULL) > 5.0f)) && ((((direction == 0) &&
                  (FurtherALONG(crate->iRAIL,crate->iALONG,crate->fALONG,
                                        best_cRPos->iRAIL,best_cRPos->iALONG,
                                        best_cRPos->fALONG) != 0)) || ((direction == 1 &&
                  (FurtherBEHIND(crate->iRAIL,crate->iALONG,crate->fALONG,
                                         best_cRPos->iRAIL,best_cRPos->iALONG,
                                         best_cRPos->fALONG) != 0))))))
                && ((iRAIL == -1 || (((direction == 0 &&
                  (FurtherBEHIND(crate->iRAIL,crate->iALONG,crate->fALONG,iRAIL,iALONG,fALONG) != 0)) ||
                 ((direction == 1 &&
                   (FurtherALONG(crate->iRAIL,crate->iALONG,crate->fALONG,iRAIL,iALONG,fALONG) != 0)))))))) {
              cpGOTO.x = crate->pos.x;
              cpGOTO.y = (crate->pos0.y + 0.5f) + 1.0f;
              cpGOTO.z = crate->pos.z;
              iRAIL = crate->iRAIL;
              iALONG = crate->iALONG;
              fALONG = crate->fALONG;
            }
        }
    }
    if (iRAIL != -1) {
        cp_goto = direction;
        return 1;
    }
    return 0;
}