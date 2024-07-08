//NGC MATCH
void ResetPlayerEvents(void) {
  struct plrevent_s *event;
  s32 i;
  s32 j;
  
  if (world_scene[0] != NULL) {
    event = PlrEvent;
    for(i = 0; i < 8; i++,event++) {
      if (event->spl != NULL) {
        if (AheadOfCheckpoint((s32)event->iRAIL,(s32)event->iALONG,event->fALONG) != 0) {
          for(j = 0; j < 0x10; j++) {
            ResetHGobjAnim(&event->obj[j]);
            event->played = 0;
          }
        }
        else {
          if (Level == 0x14) {
            for(j = 0; j < 0x10; j++) {
              FinishHGobjAnim(&event->obj[j]);
              event->played = 1;
            }
          }
        }
      }
    }
  }
  return;
}

//NGC MATCH
void CheckPlayerEvents(struct obj_s *obj) {
  struct plrevent_s *event;
  struct nuvec_s* p0;
  struct nuvec_s* p1;
  s32 i;
  s32 j;
  s32 k;
  
  if (world_scene[0] != NULL) {
    event = PlrEvent;
    for(i = 0; i < 8; i++, event++) {
      if (event->spl != NULL) {
        p0 = (struct nuvec_s *)event->spl->pts;
        p1 = (struct nuvec_s *) (event->spl->pts + event->spl->ptsize);
        if ((event->played == 0) &&
           (LineCrossed((obj->oldpos).x,(obj->oldpos).z,(obj->pos).x,(obj->pos).z,p0->x,p0->z,p1->x,p1->z) == 2)) {
          event->played = 1;
          k = 0;
          for(j = 0; j < 0x10; j++) {
            if (StartHGobjAnim(&event->obj[j]) != 0) {
              k++;
            }
          }
          if (k != 0) {
            JudderGameCamera(GameCam,0.5f,NULL);
            GameSfx(0x3b,NULL);
          }
        }
      }
    }
  }
  return;
}

//NGC MATCH
void DrawCarpet() {
  struct numtx_s m;
  float **dwa;
  struct nuvec_s v;
  
  if (carpet_model[0] != NULL) {
    SetNearestLights(&carpet_lights);
    v.x = v.y = v.z = CData[carpet_character[0]].scale * 0.5f;
    NuMtxSetScale(&m,&v);
    NuMtxRotateX(&m,carpet_xrot);
    NuMtxRotateY(&m,carpet_yrot + 0x8000);
    NuMtxTranslate(&m,&carpet_pos);
    EvalModelAnim(carpet_model[0],&carpet_anim,&m,tmtx,&dwa,NULL);
    NuHGobjRndrMtxDwa(carpet_model[0]->hobj,&m,1,NULL,tmtx,dwa);
    SetLevelLights();
  }
  return;
}

//NGC MATCH
void ResetTub(struct opptub_s *tub,struct creature_s *c) {
  tub->c = c;
  tub->track = -1;
  tub->count = -1;
  tub->laps = -1;
  tub->wait = 0;
  tub->finished = 0;
  tub->stall = 0;
  tub->boost = 0;
  return;
}

//NGC MATCH
void UpdateArrow(struct nuvec_s *src,struct nuvec_s *dst) {
  s32 temp;
  u16 xrot;
  u16 yrot;
  struct nuvec_s v;
  
  NuVecSub(&v,src,dst);
  temp = NuAtan2D(v.y,NuFsqrt(v.x * v.x + v.z * v.z));
  xrot = -(short)temp;
  yrot = (u16)NuAtan2D(v.x,v.z);
  if (GameTimer.frame != 0) {
    arrow_xrot = SeekRot(arrow_xrot,-(short)temp,2);
    arrow_yrot = SeekRot(arrow_yrot,(u16)yrot,2);
    return;
  }
  arrow_xrot = xrot;
  arrow_yrot = yrot;
  return;
}

//NGC MATCH
void DrawPanel3DArrow(void) {
  if (ObjTab[108].obj.special != NULL) {
    DrawPanel3DObject(0x6c,0.0f,0.5f,1.0f,0.125f,0.125f,0.125f,arrow_xrot,RotDiff(GameCam[0].yrot,arrow_yrot),0,ObjTab[108].obj.scene,
                      ObjTab[108].obj.special,0);
  }
  return;
}

//NGC MATCH
void ResetRings(void) {
  struct nuvec_s *pos;
  struct nuvec_s v;
  s32 i;
  s32 temp;
  
  RINGCOUNT = 0;
  if (((SplTab[64].spl != NULL) && (ObjTab[103].obj.special != NULL)) && (ObjTab[104].obj.special != NULL)) {
    RINGCOUNT = (s32)(SplTab[64].spl)->len / 2;
    if (0x30 < RINGCOUNT) {
      RINGCOUNT = 0x30;
    }
    pos = (struct nuvec_s *)(SplTab[64].spl)->pts;
    GetALONG(pos,NULL,-1,-1,1);
    if (AheadOfCheckpoint(temp_iRAIL,temp_iALONG,temp_fALONG) != 0) {
      i_ring = 0;
    }
    for (i = 0; i < RINGCOUNT; i++, pos+=2) {
        NuVecSub(&v,(pos + 1),pos);
        ring_yrot[i] = (short) NuAtan2D(v.x,v.z);
        temp = NuAtan2D(v.y,NuFsqrt(v.x * v.x + v.z * v.z));
        ring_xrot[i] = -(short)temp;
        if (i == i_ring) {
          ring_pos = *pos;
        }
    }
  }
  return;
}

//NGC MATCH
void CheckRings(struct obj_s* obj, s32* index) {
    struct nugspline_s* spl;
    struct nugspline_s* spl1;
    struct nuvec_s* pos;
    struct nuvec_s oldpos;
    struct nuvec_s newpos;
    struct nuvec_s opos;
    struct nuvec_s npos;
    struct nuvec_s n;
    struct nuvec_s* p0;
    struct nuvec_s* p1;
    struct nuvec_s lpos;
    struct nuvec_s rpos;
    struct nuvec_s v;
    float d;
    float r;
    s32 PLAYER;
    s32 i;
    
    if ((RINGCOUNT != 0) && (index[0] != RINGCOUNT)) {
        oldpos.x = obj->oldpos.x;
        oldpos.y = (((obj->bot * obj->old_SCALE) + obj->oldpos.y) + ((obj->top * obj->old_SCALE) + obj->oldpos.y)) * 0.5f;
        oldpos.z = obj->oldpos.z;
        
        newpos.x = obj->pos.x;
        newpos.y = (((obj->bot + obj->top) * obj->SCALE) * 0.5f) + obj->pos.y;
        newpos.z = obj->pos.z;
        
        PLAYER = obj == &player->obj;
        
        pos = (struct nuvec_s*)(SplTab[64].spl)->pts;
        
        for (i = 0; i < RINGCOUNT; i++, pos += 2) {
            d = NuVecDist(pos + 1, pos, NULL);
            if (NuVecDist(&oldpos, pos, NULL) <= d) {
                if (NuVecDist(&newpos, pos, NULL) <= d) {
                    NuVecSub(&n, pos + 1, pos);
                    NuVecNorm(&n, &n);
                    NuVecSub(&opos, &oldpos, pos);
                    NuVecNorm(&opos, &opos);
                    NuVecSub(&npos, &newpos, pos);
                    NuVecNorm(&npos, &npos);
                    if (((NuVecDot(&n, &opos) < 0.0f) && (0.0f <= NuVecDot(&n, &npos)))
                        || ((0.0f <= NuVecDot(&n, &opos) && (NuVecDot(&n, &npos) < 0.0f))))
                    {
                        NewBuzz(&player->rumble, 6);
                        if (i == index[0]) {
                            GameSfx(0x1e, &obj->pos);
                            index[0]++;
                            if (PLAYER == 0) {
                                return;
                            }
                            if (index[0] < RINGCOUNT) {
                                ring_pos = *(pos + 3);
                            }
                        } else {
                            GameSfx(0x4f, &obj->pos);
                        }
                        break;
                    }
                }
            }
        }
        
        if (PLAYER != 0) {
            if (best_cRPos != NULL) {
                spl = Rail[best_cRPos->iRAIL].pLEFT;
                p0 = (struct nuvec_s*)(spl->pts + (best_cRPos->iALONG * spl->ptsize));
                p1 = (struct nuvec_s*)(spl->pts + (best_cRPos->i1 * spl->ptsize));
                
                lpos.x = (p0->x + p1->x) * 0.5f;
                lpos.y = (p0->y + p1->y) * 0.5f;
                lpos.z = (p0->z + p1->z) * 0.5f;

                spl1 = Rail[best_cRPos->iRAIL].pRIGHT;
                p0 = (struct nuvec_s*)(spl1->pts + (best_cRPos->iALONG * spl1->ptsize));
                p1 = (struct nuvec_s*)(spl1->pts + (best_cRPos->i1 * spl1->ptsize));
                
                rpos.x = (p0->x + p1->x) * 0.5f;
                rpos.y = (p0->y + p1->y) * 0.5f;
                rpos.z = (p0->z + p1->z) * 0.5f;
                
                v.x = (lpos.x + rpos.x) * 0.5f;
                v.y = (lpos.y + rpos.y) * 0.5f;
                v.z = (lpos.z + rpos.z) * 0.5f;
                
                d = NuVecDist(&newpos, &v, NULL);
                if (2.5f < d) {
                    obj->mom.x = ((v.x - newpos.x) * RINGPULL) + obj->mom.x;
                    obj->mom.y = ((v.y - newpos.y) * RINGPULL) + obj->mom.y;
                    obj->mom.z = ((v.z - newpos.z) * RINGPULL) + obj->mom.z;
                }
            }
            
            if (PLAYER != 0) {
                UpdateArrow(&newpos, &ring_pos);
            }
        }
    }
    return;
}

//NGC MATCH
void DrawRings(void) {
    struct nugspline_s* spl;
    struct nuvec_s* pos;
    s32 j;
    s32 i;

    if (RINGCOUNT != 0) {
        spl = SplTab[64].spl;
        for (i = 0; i < RINGCOUNT; i++) {
            pos = (struct nuvec_s*)(spl->pts + ((i * 2) * spl->ptsize));

            if ((i == (RINGCOUNT - 1)) && (i_ring == RINGCOUNT)) {
                j = 0x69;
            } else if (i == i_ring) {
                if ((i == 0) || (i == RINGCOUNT - 1)) {
                    j = 0x69;
                } else {
                    j = 0x68;
                }
            } else {
                j = 0x67;
            }

            Draw3DObject(j, pos, ring_xrot[i], ring_yrot[i], 0, 1.0f, 1.0f, 1.0f, ObjTab[j].obj.scene, ObjTab[j].obj.special, 0);
        }
    }
    return;
}