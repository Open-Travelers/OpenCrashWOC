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
void UpdateCrateBallsOfFireDoors(void) {
    struct nuhspecial_s obj[8];
    char txt[9];
    s32 i;

    if ((best_cRPos != NULL) && (Rail[best_cRPos->iRAIL].type == 0)) {
        NuSpecialFind(world_scene[0], obj + 0, "door1");
        NuSpecialFind(world_scene[0], obj + 1, "door2");
        NuSpecialFind(world_scene[0], obj + 2, "door3");
        NuSpecialFind(world_scene[0], obj + 3, "door4");
        NuSpecialFind(world_scene[0], obj + 4, "door25");
        NuSpecialFind(world_scene[0], obj + 5, "door26");
        NuSpecialFind(world_scene[0], obj + 6, "door27");
        NuSpecialFind(world_scene[0], obj + 7, "door28");

        if (FurtherBEHIND(best_cRPos->iRAIL, best_cRPos->iALONG, best_cRPos->fALONG, 2, 0x14, 0.5f) != 0) {
            strcpy(txt, "00000000");
        } else if (FurtherBEHIND(best_cRPos->iRAIL, best_cRPos->iALONG, best_cRPos->fALONG, 2, 0x21, 0.5f) != 0) {
            strcpy(txt, "00001111");
        } else {
            strcpy(txt, "11110000");
        }

        for (i = 0; i < 8; i++) {
            if (obj[i].special != NULL) {
                obj[i].special->instance->flags.visible = txt[i] == '1';
            }
        }

        NuSpecialFind(world_scene[0], obj + 0, "door5");
        NuSpecialFind(world_scene[0], obj + 1, "door6");
        NuSpecialFind(world_scene[0], obj + 2, "door7");
        NuSpecialFind(world_scene[0], obj + 3, "door8");
        NuSpecialFind(world_scene[0], obj + 4, "door29");
        NuSpecialFind(world_scene[0], obj + 5, "door30");
        NuSpecialFind(world_scene[0], obj + 6, "door31");
        NuSpecialFind(world_scene[0], obj + 7, "door32");

        if (FurtherBEHIND(best_cRPos->iRAIL, best_cRPos->iALONG, best_cRPos->fALONG, 2, 0x38, 0.5f) != 0) {
            strcpy(txt, "00000000");
        } else if (FurtherBEHIND(best_cRPos->iRAIL, best_cRPos->iALONG, best_cRPos->fALONG, 2, 0x41, 0.5f) != 0) {
            strcpy(txt, "11110000");
        } else {
            strcpy(txt, "00001111");
        }

        for (i = 0; i < 8; i++) {
            if (obj[i].special != NULL) {
                obj[i].special->instance->flags.visible = txt[i] == '1';
            }
        }
    }
    return;
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

//NGC MATCH
void DrawChases(s32 render) {
  CHASE *chase;
  struct numtx_s m;
  struct CharacterModel *model;
  struct nuvec_s s;
  s32 i;
  s32 j;
  s32 local_64;
  struct numtx_s mtxLOCATOR [16];
  float **dwa;
  AnimPacket* anim;
  
  chase = Chase;
  local_64 = 0;
  for(i = 0; i < 3; i++, chase++) {
    if ((chase->status == 1) && (Level == 0x1f)) {
      local_64 = 1;
    }
    if (((local_64 != 0) ||
        ((chase->status == 2 && ((chase->i_last == -1 || (Chase[chase->i_last].status == 3))))))
       || ((chase->status == 3 &&
           ((chase->i_next != -1 &&
            (((chase->i_next > 2 || (Chase[chase->i_next].status == 1)) || (Chase[chase->i_next].status == 0)))))) ))
    {
      for(j = 0; j < 6; j++) {
        if (chase->ok[j] != 0) {
          if (chase->obj[j].special != NULL) {
            if (local_64 == 0) {
              s.x = s.y = s.z = 1.0f;
              NuMtxSetScale(&m,&s);
              NuMtxRotateY(&m,chase->yrot[j] + 0x8000);
              NuMtxRotateZ(&m,chase->zrot[j]);
              NuMtxRotateX(&m,chase->xrot[j]);
              NuMtxTranslate(&m,&chase->pos[j]);
            }
            if (render != 0) {
              if (Level != 0x1f) {
                NuRndrGScnObj((chase->obj[j].scene)->gobjs
                              [(chase->obj[j].special)->instance->objid],&m);
              }
              chase->obj[j].special->instance->mtx = m;
              if ((Level == 0x1f) && (ObjTab[0x4e].obj.special != NULL)) {
                NuRndrGScnObj((ObjTab[0x4e].obj.scene)->gobjs
                              [(ObjTab[0x4e].obj.special)->instance->objid],&m);
              }
            }
          }
          else {
            if ((s32)chase->character[j] != -1) {
              model = &CModel[CRemap[(s32)chase->character[j]]];
              s.x = s.y = s.z = CData[(s32)chase->character[j]].scale * chase->scale[j];
              NuMtxSetScale(&m,&s);
              NuMtxRotateY(&m,chase->yrot[j] + 0x8000);
              NuMtxRotateZ(&m,chase->zrot[j]);
              NuMtxRotateX(&m,chase->xrot[j]);
              NuMtxTranslate(&m,&chase->pos[j]);
              anim = &chase->anim[j];
              EvalModelAnim(model,anim,&m,tmtx,&dwa,mtxLOCATOR);
              if ((temp_action != -1) && (Paused == 0)) {
                AddAnimDebris(model,mtxLOCATOR,temp_action,temp_time,NULL);
              }
              if (render != 0) {
                if ((USELIGHTS != 0) && (LIGHTCHASECHARACTERS != 0)) {
                  SetLights(&chase->lights[j].pDir1st->Colour,&chase->lights[j].pDir1st->Direction,&chase->lights[j].pDir2nd->Colour,&chase->lights[j].pDir2nd->Direction,
                            &chase->lights[j].pDir3rd->Colour,&chase->lights[j].pDir3rd->Direction,&chase->lights[j].AmbCol);
                }
                NuHGobjRndrMtxDwa(model->hobj,&m,1,NULL,tmtx,dwa);
              }
            }
          }
        }
      }
    }
  }
  if (((render != 0) && (USELIGHTS != 0)) && (LIGHTCHASECHARACTERS != 0)) {
    SetLevelLights();
  }
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

//NGC MATCH
void UpdateChases(void) {
    struct chase_s* chase;
    struct nuvec_s* p0;
    struct nuvec_s* p1;
    struct obj_s* obj;
    s32 i;

    if (Level == 0x1f) {
        UpdateCrateBallsOfFireDoors();
    }

    chase = Chase;
    obj = &player->obj;
    for (i = 0; i < 3; i++, chase++) {
        if (chase->status == 1) {
            if (player->obj.transporting == 0) {
                p0 = (struct nuvec_s*)(chase->spl_START->pts);
                p1 = (struct nuvec_s*)(chase->spl_START->pts + (s32)chase->spl_START->ptsize);
                if (LineCrossed(obj->oldpos.x, obj->oldpos.z, obj->pos.x, obj->pos.z, p0->x, p0->z, p1->x, p1->z) == 2)
                {
                    InitChase(chase);
                }
            }
        } else if (chase->status > 1) {
            if (chase->status < 4) {
                UpdateChase(chase, obj);
            }
        }
    }
    return;
}