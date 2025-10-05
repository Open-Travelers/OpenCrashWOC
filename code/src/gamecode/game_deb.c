//#include "gamecode/game_deb.h"

s32 atlasbits[7];
s32 helibits[7];
s32 jeepbits[8];
s32 mechbits[7];

//NGC MATCH
void InitGameDebris(void) {
  s32 i;
  
  for(i = 0; i < 170; i++) {
    GDeb[i].i = -1;
    if (((GDeb[i].levbits >> Level) & 1) != 0) {
      GDeb[i].i = LookupDebrisEffect(GDeb[i].name);
    }
  }
  return;
}

//NGC MATCH
void AddGameDebris(s32 i,struct nuvec_s *pos) {
  s32 key;
  
  if (((NODEBRIS == 0) && ((uint)i < 0xaa)) && (GDeb[i].i != -1)) {
    key = -1;
    AddFiniteShotDebrisEffect(&key,GDeb[i].i,pos,1);
  }
  return;
}

//NGC MATCH
void AddGameDebrisRot(s32 i,struct nuvec_s *pos,s32 n,u16 xrot,u16 yrot) {
  if (((NODEBRIS == 0) && ((u32)i < 0xaa)) && (GDeb[i].i != -1)) {
    AddVariableShotDebrisEffect(GDeb[i].i,pos,n,xrot,yrot + 0x4000);
  }
  return;
}

//NGC MATCH
void AddGameDebrisMtx(s32 i,struct nuvec_s *pos,s32 n,struct numtx_s *m) {
  if (((NODEBRIS == 0) && ((u32)i < 0xaa)) && (GDeb[i].i != -1)) {
    NuMtxSetRotationX(&mTEMP,0x4000);
    NuMtxMulR(&mTEMP,&mTEMP,m);
    AddVariableShotDebrisEffectMtx2(GDeb[i].i,pos,n,&mTEMP,&numtx_identity);
  }
  return;
}

//NGC MATCH
void AddWarpDebris(struct obj_s *obj,struct nuvec_s *pos) {
  struct nuvec_s v;
  s32 key;
  
  key = -1;
  v.x = (obj->pos).x;
  v.y = (obj->bot + obj->top) * obj->SCALE * 0.5f + (obj->pos).y;
  v.z = (obj->pos).z;
  AddFiniteShotDebrisEffect(&key,GDeb[77].i,&v,1);
  return;
}

//NGC MATCH
void AddMechanicalDebris(struct nuvec_s* pos, s32 vehicle) {
    s32 i;
    s32* pt;

    pt = jeepbits;
    i = 1;
    switch (vehicle) {
    case 0x44:
    case 0xB2:
        i = 1;
        pt = mechbits;
        break;
    case 0x53:
        pt = atlasbits;
        break;
    case 0x3B:
        pt = helibits;
        break;
    case 0x20:
        AddGameDebris(0x8E, pos);
        i = 6;
        break;
    }
    while (*pt != -1) {
        AddDeb3(pos, pt[0], i, NULL);
        pt++;
    }
}

//94.88% NGC
void AddAnimDebris(CharacterModel *model,struct numtx_s *mtxLOCATOR,s32 action,float time,struct obj_s *obj) {
  struct adeb_s *adeb;
  u16 xrot;
  u16 yrot;
  float y;
  float sizex;
  float sizez;
  s32 n;
  s32 i;
  s32 j;
  s32 count;
  s32 list[16]; // ??
  s32 sfx;
  struct creature_s *c;
  struct numtx_s* mtx[16];
  struct nuvec_s pos;
  struct nuvec_s *p;
  struct nuvec_s v[16];
  s32 inf;
  s32 gindex;
  s32 depth;
  s32 DoSound;
  
  if ((Paused != 0) || (ADeb[0].character == -1)) {
      return;
  }
  for(adeb = ADeb; adeb->character != -1; adeb++) {
      if (((model->character == adeb->character) && (action == adeb->action)) && (((adeb->levbits >> Level) & 1) != 0)) {
        if (((obj == NULL) || ((((adeb->flags & 0x10) == 0 || ((obj->ground & 2U) != 0)) &&
             (((adeb->flags & 0x80) == 0 || (obj->submerged == 0)))))) && (((adeb->flags & 4) != 0 ||
            (((((adeb->flags & 8) != 0 && (time >= adeb->time)) &&
              ((adeb->time2 <= 1.0f || ((adeb->time2 > adeb->time && (time <= adeb->time2)))))) ||
             ((time > adeb->time && ((time - (model->animlist[action]->speed * 0.5f)) <= adeb->time) ))))))) {
          if (adeb->character == 0x9a) {
            pos = v000;  //CData[1].max ??
          }
          pos = v000;
          count = 0; 
          //p = (struct nuvec_s *)&mtxLOCATOR->_30;
          //pLoc = model->pLOCATOR;
          //ppnVar13 = mtx;
          //pnVar7 = mtxLOCATOR;
          for(j = 0; j < 16; j++) { //pnVar15 & 0x3fU
            if ((((adeb->locators >> j) & 1) != 0) && (model->pLOCATOR[j] != NULL)) {
              list[count] = j;
              v[j] = *(struct nuvec_s *)(mtxLOCATOR);
              if ((count == 0) || ((adeb->flags & 0x4000) != 0)) {
                NuVecAdd(&pos,&pos,(struct nuvec_s *)(mtxLOCATOR));
              }
              //pnVar16++;
              count++;
            }
            //p = &p[5].y;
            //pnVar7 = pnVar7 + 1;
            //pLoc = pLoc + 1;
          }
          if (count > 0) {
            NuVecScale(&pos,&pos,(1.0f / j));
          }
          if (((adeb->flags & 0x8000) != 0) && (obj != NULL)) {
            count = 1;
            pos.x = obj->pos.x;
            pos.y = obj->bot * obj->SCALE + obj->pos.y;
            pos.z = obj->pos.z;
          }
          if ((count == 0) && (obj != NULL)) {
            v[count] = obj->pos;
            pos = v[count];
            count = 1;
          }
          if ((adeb->flags & 0x10000) != 0) {
            JudderGameCamera(GameCam,adeb->max,NULL);
          }
          if ((count != 0) && ((adeb->flags & 0x80) != 0)) {
              if (obj == NULL) goto LAB_8003d550;
            if ((obj->layer_type != -1) && (pos.y < obj->layer_shadow)) continue; //goto LAB_8003dd9c;
          }
          if ((obj == NULL) || (((((((obj->flags & 4) != 0 && (obj->dead == 1)) ||
                   (((adeb->flags & 0x100) != 0 && (obj->xz_distance < adeb->min)))) ||
                  (((adeb->flags & 0x200) != 0 && (obj->xz_distance > adeb->max)))) ||
                 (((adeb->flags & 0x40000) != 0 && (obj->xyz_distance < adeb->min)))) ||
                ((((adeb->flags & 0x80000) != 0 && (obj->xyz_distance > adeb->max)) ||
                 (((obj->flags & 1) != 0 && (((((GemPath == 1 || (GemPath == 3)) || (Death == 1)) ||
                    ((Death == 3 || (Bonus == 1)))) || (Bonus != 3)))))))))) {
                //continue;//goto LAB_8003d550;
           // } else {
LAB_8003d550:
               if ((count != 0) && (adeb->gdeb < 0xaa)) {
                  if ((adeb->flags & 1) != 0) {
                        n = adeb->count;
                        if (adeb->count < 0) {
                          n = (jonframe1 == (jonframe1 / (s32)-adeb->count) * -adeb->count);
                        }
                        if ((n != 0) &&
                           ((adeb->random == 0 || (qrand() <= (s32)(adeb->random << 8))))) {
                          if ((adeb->flags & 0x8000) != 0) {
                            AddGameDebrisRot(adeb->gdeb,&pos,n,0,0);
                          }
                          else {
                                if ((adeb->flags & 0x400) != 0) {
                                    if ((adeb->flags & 0x4000) != 0) {
                                      AddGameDebrisMtx(adeb->gdeb,&pos,n,mtx[0]);
                                    }
                                    else {
                                      for(i = 0; i < count; i++) {
                                          AddGameDebrisMtx(adeb->gdeb,&v[i],n,mtx[i]);
                                          //count = count - 1;
                                        }
                                    }
                                    //goto LAB_1;
                                } else {
                                    if ((adeb->flags & 2) != 0) {
                                        xrot = (u16)adeb->xrot << 8;
                                        if (obj == NULL) {
                                            yrot = 0;
                                        } else {
                                            yrot = obj->hdg;
                                        }
                                    }
                                    else {
                                        xrot = 0;
                                        yrot = 0;
                                    }
                                    if ((adeb->flags & 0x4000) != 0) {
                                        AddGameDebrisRot(adeb->gdeb,&pos,n,xrot,yrot);
                                    }
                                    else {
                                          //pnVar16 = v;
                                        for(i = 0; i < count; i++) {
                                            AddGameDebrisRot(adeb->gdeb,&v[i],n,xrot,yrot);
                                            //pnVar16++;
                                        }
                                    }
                                }
                          }
                        }
                  } else if ((adeb->flags & 0x8000) != 0) { 
                      AddGameDebris(adeb->gdeb,&pos);
                  } else if ((adeb->flags & 0x4000) != 0) {
                      AddGameDebris(adeb->gdeb,&pos);
                  } else {
                        //pnVar16 = v;
                    for(i = 0; i < count; i++) {
                          AddGameDebris(adeb->gdeb,&v[i]);
                          //pnVar16++;
                    }
                  }
               }
//LAB_1:
               if (adeb->rumble != 0) {
                 NewRumble(&player->rumble,adeb->rumble);
               }
               if (adeb->buzz != 0) {
                 NewBuzz(&player->rumble,adeb->buzz);
               }
          }
          //sfx = (s32)adeb->sfx;
          if (((((adeb->sfx >= 0) && (adeb->sfx < SFXCOUNT_ALL)) || (adeb->sfx == 0x400)) || (adeb->sfx == 0x401)) &&
             ((adeb->random == 0 || (qrand() <= (adeb->random << 8))))) {
            if (adeb->sfx == 0x400) {
              sfx = 0x8d;
              if (obj != NULL) {
                if (obj->layer_type == 1) {
                  sfx = 0x2d;
                }
                else {
                  //cVar10 = obj->surface_type;
                /*  if (cVar10 < 6) {
                   if ((cVar10 < 6) && (cVar10 != 1)) {
LAB_8003d7f0:
                      sfx = 0x1f;
                    }
                  }*/
                  //else if ((cVar10 < '\x10') && ('\f' < cVar10)) goto LAB_8003d7f0;
                    switch(obj->surface_type) {
                        case 1:
                        case 5:
                        case 6:
                        case 13:
                        case 14:
                        case 15:
                            sfx = 0x1f;
                        break;
                        default:
                        break;
                    }
                }
              }
            }
            else if ((adeb->sfx == 0x401) && (sfx = 0x8e, obj != NULL)) {
              if (obj->layer_type == 1) {
                sfx = 0x2e;
              }
              else {
                //cVar10 = obj->surface_type;
               /* if (cVar10 < 6) {
                  if ((6 > cVar10) && (cVar10 != 1)) {
LAB_8003d850:
                    sfx = 0x20;
                  }
                }
                else if ((cVar10 < '\x10') && ('\f' < cVar10)) goto LAB_8003d850;*/
                    switch(obj->surface_type) {
                        case 1:
                        case 5:
                        case 6:
                        case 13:
                        case 14:
                        case 15:
                            sfx = 0x20;
                        break;
                        default:
                        break;
                    }
              }
            }
            //0x8003D448
            DoSound = 1;
            if (((adeb->flags & 0x400000) != 0) && (adeb->max > frand())) {
              DoSound = 0;
            }
            if (DoSound) {
              if ((adeb->flags & 0x100000) != 0) {
                gamesfx_effect_volume = (s32)adeb->min;
              }
              if ((adeb->flags & 0x200000) != 0) {
                gamesfx_pitch = (s32)adeb->max;
              }
              GameSfx(sfx,((count == 0) || ((adeb->flags & 0x2000) == 0)) ? NULL : &pos);
            }
          }
          if ((((count != 0) && ((adeb->flags & 0x800) != 0)) && (adeb->type != -1)) &&
             (((obj == NULL || (obj->dead == 0)) &&
              ((adeb->random == 0 || (qrand() <= (s32)(adeb->random << 8)))))) )
          {
            n = adeb->count;
            if (n < 0) {
              n = (jonframe1 % -n) == 0;
            }
            if (n != 0) {
              c = NULL;
              if (obj != NULL) {
                c = (struct creature_s*)obj->parent;
              }
              //cVar10 = adeb->type;
              AddProjectile(&pos,((adeb->target != -1) && (c != NULL)) && (c->i_aitab != -1) ? &AITab[sfx].pos[adeb->target] : NULL, NULL, (s32)adeb->type,(obj != NULL) ? obj->hdg : (u16)qrand(),obj);
            }
          }
          if (((adeb->flags & 0x1000) != 0) && ((obj == NULL || (obj->dead == 0)))) {
            p = NULL;
            if (adeb->target != -1) {
              if (obj != NULL) {
                  p = &AITab[c->i_aitab].pos[adeb->target];
              }
            }
            else if (count != 0) {
                p = &pos;
            } else if (obj != NULL) {
                p = &obj->pos;
            }
            if (p != NULL) {
              AddKaboom(0x10,p,(!(adeb->max > 0.0f)) ? 1.0f : adeb->max);
            }
          }
          if (((obj != NULL) && ((adeb->flags & 0x60) != 0)) && ((obj->ground & 2U) != 0)) {
            y = NewShadowMask(&pos,0.0f,-1);
            //0x8003CFD4
            if (y != 2000000.0f) {
              inf = ShadowInfo();
              gindex = -1;
              if ((EShadY != 2000000.0f) && (EShadowInfo() == 4)) {
                y = EShadY;
                depth = 0x78;
                obj->ddsnow = 0x78;
                gindex = 4;
              }
            switch(inf) {
                case 2:
                    if (gindex == -1) {
                      depth = 0x78;
                      obj->ddr = 0x60;
                      gindex = 4;
                      obj->ddg = 0x70;
                      obj->ddb = -0x40;
                      obj->ddsnow = 0x78;
                    }
                break;
                case 8:
                      depth = 0x78;
                      obj->ddr = -0x80;
                      gindex = 4;
                      obj->ddg = 0x50;
                      obj->ddb = 0x20;
                      obj->ddsand = 0x78;
                break;
                default:
                  //cVar10 = obj->ddsnow;
                  if ((obj->ddsnow > 0) && (gindex == -1)) {
                    obj->ddr = 0x60;
                    obj->ddg = 0x70;
                    gindex = 6;
                    obj->ddb = -0x40;
                    obj->ddsnow = obj->ddsnow - 2;
                    depth = (u8)obj->ddsnow;
                  }
                  //cVar10 = obj->ddsand;
                  if ((obj->ddsand > 0) && (gindex == -1)) {
                    obj->ddsand -= 2;
                    obj->ddr = -0x80;
                    obj->ddg = 0x50;
                    gindex = 0;
                    obj->ddb = 0x20;
                    depth = obj->ddsand;
                  }
                  if (obj->ddwater > 0) {
                    if (gindex != -1) break;
                    if (obj->ddwater < 0x78) {
                      obj->ddr = 0x30;
                      obj->ddg = 0x30;
                      obj->ddb = 0x30;
                      depth = obj->ddwater;
                      gindex = 2;
                    }
                    obj->ddwater -= 2;
                  }
                break;
            }
//LAB_8003dc38:
              if (gindex != -1) { //goto LAB_8003dd9c;
                  if (model->character == 1) {
                    sizex = 0.07f;
                    sizez = 0.12f;
                  }
                  else {
                    sizex = 0.1f;
                    sizez = 0.2f;
                  }
                  pos.y = y + 0.02f;
                  if ((adeb->flags & 0x20) != 0) {
                    //yrot = obj->hdg;
                    if ((obj->hdg & 0x8000) == 0) {
                      gindex++;
                      //cVar10 = obj->ddr;
//LAB_8003dd48:
                      NuRndrAddFootPrint(obj->hdg,sizex,sizez,depth,&pos,&ShadNorm,gindex,obj->ddb * 0x10000 + obj->ddg * 0x100 + obj->ddr);
                    }
                    else {
                      //sizex = -sizex;
                      NuRndrAddFootPrint(obj->hdg,-sizex,sizez,depth,&pos,&ShadNorm,gindex,obj->ddb * 0x10000 + obj->ddg * 0x100 + obj->ddr);
                    }
                  }
                  else {
                    //yrot = obj->hdg;
                    if ((obj->hdg & 0x8000) == 0) {
                      //cVar10 = obj->ddr;
                      //sfx = 0x30;
                      NuRndrAddFootPrint(obj->hdg,sizex,sizez,0x30,&pos,&ShadNorm,gindex,obj->ddb * 0x10000 + obj->ddg * 0x100 + obj->ddr);
                      
                    } else {
                      gindex++;
                      //sizex = -sizex;
                      NuRndrAddFootPrint(obj->hdg,-sizex,sizez,0x30,&pos,&ShadNorm,gindex,obj->ddb * 0x10000 + obj->ddg * 0x100 + obj->ddr);
                    }
                  }
              }
            }
          }
        }
      }
//LAB_8003dd9c:
      //adeb = adeb + 1;
  }
}