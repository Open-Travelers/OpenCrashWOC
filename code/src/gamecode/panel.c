s32 MAXVPSIZEX;
s32 MINVPSIZEX;
s32 MAXVPSIZEY;
s32 MINVPSIZEY;
s32 screendump;
s32 save_paused;
s32 editor_active;

//NGC MATCH
void MaxVP(void) {
  NuVpSetSize(MAXVPSIZEX,MAXVPSIZEY);
  return;
}

//PS2
void GameVP(void)
{
    s32 paused;
    if (screendump != 0) {
        paused = save_paused;
    } else {
        paused = Paused;
    }
    
    if (editor_active != 0 || ((paused != 0) && (Cursor.menu - 3 >= 6u))) {
        NuVpSetSize(MAXVPSIZEX * 16, MAXVPSIZEY * 8);
    }
    else {
        NuVpSetSize((MAXVPSIZEX * 0x10 + ((MINVPSIZEX * 0x10 + MAXVPSIZEX * -0x10) * paused) / 0x1e),
                (MAXVPSIZEY * 8 + ((MINVPSIZEY * 8 + MAXVPSIZEY * -8) * paused) / 0x1e));
    }
    return;
}

//NGC MATCH
s32 NextLetter(u8 *txt,struct nuspecial_s **obj) {    
  if (txt == NULL) {
    return 1;
  }
  if (obj != NULL) {
    obj[1] = NULL;
    obj[0] = NULL;
  }
  if ((Font3DRemap[*txt] != -1) && (obj != NULL)) {
    *obj = Font3DTab[Font3DRemap[*txt]].obj.special;
  }
  return 1;
}

//NGC MATCH
void BigOutOf(float x,float y,float scale,int value,int total) {
    char *txt;
    float x0;
    float y0;
    float x1;
    float y1;

  if (Game.language == 'c') {
    txt = "/ ";
  }
  else {
    txt = "/";
  }
  Text3D(txt,x,y,1.0f,(scale + scale),(scale + scale),(scale + scale),1,0);
  x0 = font3d_xleft;
  y0 = font3d_ytop;
  x1 = font3d_xright;
  y1 = font3d_ybottom;
  sprintf(tbuf,"%i",value);
  if (Game.language == 'c') {
    AddSpacesIntoText(tbuf,1);
  }
  Text3D(tbuf,((x0 - x) * 0.5f + x),((y0 - y) * 0.5f + y),1.0f,scale,scale,scale,0x10,0);
  sprintf(tbuf,"%i",total);
  if (Game.language == 'c') {
    AddSpacesIntoText(tbuf,1);
  }
  Text3D(tbuf,((x1 - x) * 0.5f + x),((y1 - y) * 0.5f + y),1.0f,scale,scale,scale,4,0);
  return;
}

//NGC MATCH
void ResetPanelDebris(void) {
  struct pdeb_s *deb;
  s32 i;
  
  deb = PDeb;
  for (i = 0; i < 0x20; i++, deb++) {
    deb->active = 0;
  }
  return;
}

//NGC MATCH
void AddPanelDebris(float x,float y,int type,float scale,int count) {
  struct pdeb_s* deb;
  float f;
  s32 i;
  u32 uVar6;

  i = count;
  do {
        if ((u32)type < 7) {
              deb = &PDeb[i_pdeb];
              deb->info = &PDebInfo[type];
              uVar6 = deb->info->i_objtab;
              if ((uVar6 == 0x11) && (LDATA->character == 1)) {
                uVar6 = 0x12;
              }
                if ((uVar6 < 0xc9) && (ObjTab[uVar6].obj.special != NULL)) {
                        deb->type = (s8)type;
                        deb->xrot = 0;
                        deb->yrot = 0;
                        deb->zrot = 0;
                           switch ((s8)type) {
                              case '\x01':
                                    deb->mom.x = (( ((float)qrand()) * 0.000015259022f) * 0.5f) * 0.016666668f;
                                    deb->mom.y = ( ((float)qrand()) * 0.000015259022f + 2.0f) * 0.016666668f;
                                    deb->pos.x = (x + (deb->mom.x + deb->mom.x));
                                    deb->pos.y = (y + 0.05f) + deb->mom.y;
                            break;
                            case '\x02':
                            case '\x03':
                            case '\x04':
                            case '\x05':
                                    deb->pos.x = x;
                                    deb->pos.y = y;
                                    deb->oldpos = deb->pos;
                                    deb->oldscale = scale;
                            break;
                            case '\x06':
                                  deb->zrot = (qrand() / 2) + 0xc000;
                                  f = ( ((float)qrand()) * 0.000015259022f) * 0.016666668f;
                                  deb->mom.x  = NuTrigTable[deb->zrot] * f;
                                  deb->mom.y =  NuTrigTable[(deb->zrot + 0x4000) & 0x3fffc / 4] * f;
                                  deb->pos.x = x + (deb->mom.x + deb->mom.x);
                                  deb->pos.y = y + (deb->mom.y + deb->mom.y);
                            break;
                          default:
                                  deb->mom.x = 0.0f;
                                  deb->mom.y = 0.0f;
                                  deb->pos.x = x;
                                  deb->pos.y = y;
                          break;
                          }
                        deb->active = '\x01';
                        i_pdeb++;
                        deb->i_objtab = (short)uVar6;
                        deb->time = 0.0f;
                        if (i_pdeb == 0x20) {
                          i_pdeb = 0;
                        }
                }
        }
  i--;
  } while(i > 0);
    return;
}

//NGC MATCH
void UpdatePanelItem(struct plritem_s* item, int force_update, int use_change) {

    if (item->frame == 30) {
        if (item->wait != 0) {
            item->wait--;
        }
        if (force_update != 0) {
            item->wait = 60;
        }
        if (item->delay != 0) {
            item->delay--;
        } else {
            if (item->draw < item->count) {
                item->draw++;
                if ((item == &plr_wumpas) || (item == &plr_bonus_wumpas)) {
                    GameSfx(0x19, NULL);
                    item->delay = 6;
                }
                item->wait = 60;
            } else if (item->draw > item->count) {
                item->wait = 60;
                item->draw = item->count;
            }
        }
        if (item->draw == item->count && item->wait == 0) {
            item->frame--;
        }
        return;
    }
    
    if (force_update || (use_change && item->draw != item->count && (new_mode == -1) && (new_level == -1))) {
        if (item->frame < 30) {
            item->frame += 2;
            if (item->frame > 30) {
                item->frame = 30;
            }
        }
        return;
    }
    if (item->draw != item->count) {
        item->draw = item->count;
    }
    if (item->frame > 0) {
        item->frame--;
    }
}

//NGC 56%
void DrawWorldToPanelWumpa(void) {
  //char *pcVar2;
  float xs;
    float x;
    float y;
  float ys;
  float f;
  float size;
  s32 i;
    s16 rot;
    
  //pcVar2 = &WScr[0].bonus;
  //iVar3 = 0;
      rot = 0;
  for (i = 0; i < 32; i++) {
    if (WScr[i].timer > 0.0f) {
      f = (0.25f - WScr[i].timer) * 4.0f;
      x = WScr[i].pos.x;
      y = WScr[i].pos.y;
      if (WScr[i].bonus != 0) {
          size =  (0.125f - WScr[i].scale) * f + WScr[i].scale;
      }
      else {
          size = (0.2f - WScr[i].scale) * f + WScr[i].scale;  
      }
        ys = ((WScr[i].ys - y) * f + y);
        xs = ((WScr[i].xs -  x) * f + x);
      if (WScr[i].bonus == 0) {
          rot = -0x1800;
      }
      DrawPanel3DObject(0,xs,ys,1.0f,size,size,size,rot,
                        PANELWUMPAYROT,0,ObjTab[0].obj.scene,ObjTab[0].obj.special,1);
    }
    //pcVar2 = pcVar2 + 0x20;
    //iVar3 = iVar3 + 0x20;
  }
  return;
}

//NGC MATCH
void DrawGameMessage(char* txt, s32 message_frame, float ys) {
    s32 i;
    s32 j;
    s32 temp;
    s32 frame;
    s32 frames[4];
    float xs;
    float size;
    float sx;
    char msg[5];
    char* p;
    s32 x;

    if (Game.language == LANGUAGE_JAPANESE) {
        temp = JStrLen(txt);
    } else {
        temp = strlen(txt);
    }

    frame = (temp - 1) * 0x14;
    frames[0] = frame;
    frames[1] = frame + 0x1E;
    frames[2] = frame + 0x4B;
    frames[3] = frame + 0x87;

    x = (frames[2] + message_frame) - (((frames[2] + message_frame) / frames[3]) * frames[3]);

    i = -1;
    if (x < frames[0]) {
        if (x % 0x14 < 0xd) {
            i = x / 0x14 + 1;
        }
    } else {
        if (x < frames[1]) {
            if ((x - frames[0]) < 0x16) {
                i = temp;
            }
        } else if (x < frames[2]) {
            if ((x - frames[1]) < 0x25) {
                i = temp;
            }
        } else if (((x - frames[2]) % 0x3c) < 0x34) {
            i = temp;
        }
    }

    if (i != -1) {
        size = 1.0f;
        if ((strcmp(txt, tLOADING[Game.language]) == 0) && (Game.language == 3)) {
            size = 0.8f;
        }

        xs = -((((temp - 1) * 0.1f) * size) * 0.5f);
        sx = (Game.language != LANGUAGE_JAPANESE) ? 1.0f : FONT3D_JSCALEX;
        p = txt;
        for (j = 0; j < i; j++) {
            if (Game.language == LANGUAGE_JAPANESE) {
                msg[0] = *p++;
                msg[1] = *p++;
                if ((p[0] == 0x42)
                    && ((
                        (p[1] == 0x44 && (CombinationCharacterBD(msg[0], msg[1]) != 0))
                        || ((p[1] == 0x43 && (CombinationCharacterBC(msg[0], msg[1]) != 0)))
                    )))
                {
                    msg[2] = *p++;
                    msg[3] = *p++;
                    msg[4] = 0;
                } else {
                    msg[2] = 0;
                }
            } else {
                msg[0] = *p++;
                msg[1] = 0;
            }
            Text3D(&msg[0], xs, ys, 1.0f, (size * sx), 1.0f, 1.0f, 1, 4);
            xs = (size * 0.1f + xs);
        }
    }
    return;
}

//87% NGC
void DrawPanel(void) {
    s32 bVar1;
    float fVar3;
    float fVar5;
    float fVar6;
    float scale;
    float *pfVar7;
    s32 uVar8;
    u16 uVar10;
    int object;
    char *pcVar9;
    char **ppcVar13;
    float *pfVar14;
    struct creature_s *plr;
    int iVar17;
    s32 i;
    s32 temp_paused;
    float xs;
    float ys;
    float zs;
    float size;
    float dVar21;
    float dVar22;
    float dVar24;
    float dVar25;
    char txt [32];
    short WibbleXRot;
    short WibbleYRot;
    
    ys = 0.0f;
    plr = NULL;
    if (PLAYERCOUNT != 0) {
        plr = player;
    }
    WibbleYRot = (u16)(NuTrigTable[((GlobalTimer.frame % 300) * 0x10000) / 0x4b & 0x3fffc] * 2731.0f + 1820.0f);
    WibbleXRot = (u16)(NuTrigTable[(((GlobalTimer.frame % 0xf0) * 0x10000) / 0xf0 + 0x4000) & 0x3fffc] * 2731.0f);
    //zs = SWIDTH;
    //xs = PHYSICAL_SCREEN_X;
    //dVar25 = SHEIGHT;
    PANEL3DMULX = (float)SWIDTH / (DIVPANEL3DX * ((float)SWIDTH / PHYSICAL_SCREEN_X));
    PANEL3DMULY = ((float)SHEIGHT) / (DIVPANEL3DY * (((float)SHEIGHT) / (PHYSICAL_SCREEN_Y)));
    if (screendump != 0) {
        temp_paused = save_paused;
    }
    else {
        temp_paused = Paused;
    }
    //bVar1 = object == 0;
    if (((temp_paused == 0) && ((LDATA->flags & 1) != 0)) && (PANELOFF != 0)) {
        return;
    }
    uVar10 = -(short)(((GlobalTimer.frame % 0x78) * 0x10000) / 0x78);
    if ((pause_rndr_on == 0) && (cutmovie == -1)) {
        Draw3DCheckpointLetters();
    }
    MaxVP();
    if ((ShowPlayerCoordinate != 0) && (plr != NULL)) {
        NuCameraTransformScreenClip(&vTEMP,&(plr->obj).pos,1,NULL);
    }
    SetLevelLights();
    NuShaderSetBypassShaders(1);
    DrawMenu(&Cursor,temp_paused);
    if ((Cursor.menu == 0x13) || (cutmovie != -1)) goto LAB_800601c8;
    if ((Level == 0x25) && ((((Hub != -1 && (temp_paused == 0x80)) && (SHEIGHT != 0, hubleveltext_pos > 0.0f)) && (hubleveltext_level != -1)))) {
        ys = ((0.333f - hubleveltext_pos) / 0.333f);
        if (hubleveltext_open != 0) {
            Text3D(LevelName[hubleveltext_level][Game.language],0.0f,(ys * 0.4f + 0.8f),1.0f,0.6f,0.6f,0.6f,1,0);
        }
        if ((u32)hubleveltext_i < 5) {
            if (hubleveltext_open != 0) {
                if (hubleveltext_i == 2) {
                    fVar3 = -0.68f - (ys * 0.8f);
                }
                else {
                    fVar3 = -0.68f;
                    if (hubleveltext_i > 2) {
                        dVar25 = (-0.5f - (ys * 0.8f));
                    }
                    else {
                        dVar25 = (ys * 0.8f + 0.5f);
                    }
                }
                DrawTimeTrialTimes(hubleveltext_level,dVar25,fVar3,1.0f);
            }
            else {
                if (Hub == 5) {
                    sprintf(tbuf,tRELICSREQUIRED[Game.language],(hubleveltext_i) * 5);
                    Text3D(tbuf,0.0f,(ys * 0.3f + 0.85f),1.0f,0.6f,0.6f,0.6f,1,3);
                }
            }
        }
    }
    if ((LDATA->flags & 1) == 0) goto LAB_800601c8;
    //bVar2 = plr != NULL;
    if ((plr != NULL) && (ShowPlayerCoordinate != 0)) {
        sprintf(tbuf,"X:%.2f Y:%.2f Z:%.2f",(plr->obj).pos.x,(plr->obj).pos.y,(plr->obj).pos.z);
        if (Game.language == 0x63) {
            AddSpacesIntoText(tbuf,1);
        }
        Text3D(tbuf,vTEMP.x,vTEMP.y,1.0f,0.4f,0.5f,0.5f,1,3);
    }
    if (temp_paused != 0) {
        if (Cursor.menu == 1 || Cursor.menu == 2 || Cursor.menu == 3 || Cursor.menu == 4 || Cursor.menu == 5 || Cursor.menu == 6) {
            fVar3 = -0.02f;
            if (pause_dir != 0) {
                ys = ((s32)(0x1eU - temp_paused) * (PAUSEVMUL / 30.0f));
                fVar3 = (-0.02f - ys);
            }
            //dVar25 = 0.1f;
            DrawPanel3DObject(99,0.0f,fVar3,(PAUSEPANELZ + 0.01),0.1f,0.1f,(ys * 0.1f),0,0,0,
                                ObjTab[99].obj.scene,ObjTab[99].obj.special,1);
            fVar3 = 0.0f;
            if (pause_dir != 0) {
                ys = ((0x1eU - temp_paused) * (PAUSEHMUL / 30.0f));
                fVar3 = (ys + 0.0f);
            }
            DrawPanel3DObject(0x62,fVar3,0.0f,PAUSEPANELZ - 0.01f,0.1f,0.1f,
                              (ys * 0.1f),0,0,0,ObjTab[98].obj.scene,ObjTab[98].obj.special,1);
            if (temp_paused == 0x1e) {
                Text3D(tTCR_PRESSSTARTTORESUME[Game.language],PANELMENUX * 0.9f,
                       menu_pulsate * 0.05f + 0.64f,1.0f,0.4f,0.4f,0.4f,1,0);
                Text3D(Game.name,PAUSEPANELX,PAUSENAMEY,PAUSETEXTZ,0.6f,0.6f,0.6f,1,0);
                sprintf(tbuf,"%i%%",(u32)Game.percent);
                if (Game.language == 0x63) {
                    AddSpacesIntoText(tbuf,1);
                }
                //FIX THIS
                Text3D(tbuf,PAUSEPANELX,PAUSEPERCENTY - 0.01,PAUSETEXTZ,1.0f,1.0f,1.0f,1,0);
                if (LDATA->hub != -1) {
                    object = 5;
                }
                else{
                    object = 4;
                }
                xs = PAUSEPANELX;
                ys = PAUSEINFOY;
                zs = PAUSETEXTZ;
                uVar8 = (pausestats_frame - (pausestats_frame / (u32)(object * 0xb4)) * object) / 0xb4;

                switch(uVar8) {
                    case 0:
                        if (Game.language == 5) {
                            Text3D(tCRYSTALS[5],PAUSEPANELX,(ys + 0.34999999f),PAUSETEXTZ,0.5f,0.6f,0.6f,1,0);
                        }
                        else {
                            Text3D(tCRYSTALS[Game.language],PAUSEPANELX,
                                   (ys + 0.34999999f),PAUSETEXTZ,0.6f,0.6f,0.6f,1,0);
                        }
                        BigOutOf((xs - 0.1f),(ys - 0.125f),0.52f,Game.crystals,0x19);
                        //fVar3 = 0.2f;
                        dVar25 = (xs + 0.15f);
                        ys -= 0.175f;
                        //object = 0x75;
                        DrawPanel3DCharacter(0x75,dVar25,ys,zs,0.2f,0.2f,0.2f,0,uVar10,0,-1,1.0f,1);
                    break;
                    case 1:
                            Text3D(tPOWERS[Game.language],PAUSEPANELX,(ys + 0.34999999f),PAUSETEXTZ,
                                        0.6f,0.6f,0.6f,1,0);
                            if (Game.language == 0x63) {
                                tbuf[2] = '\0';
                                tbuf[1] = ' ';
                            }
                            else {
                                tbuf[1] = '\0';
                            }
                            if ((ExtraMoves != 0) || ((Game.powerbits & 1) != 0)) {
                                tbuf[0] = PData[0].font3d_letter;
                                Text3D2(tbuf,(xs - 0.16f),(ys + 0.05f),zs,1.4f,1.4f,1.4f,1,0);
                            }
                            if ((ExtraMoves != 0) || ((Game.powerbits & 2) != 0)) {
                                tbuf[0] = PData[1].font3d_letter;
                                Text3D2(tbuf,xs,(ys + 0.05f),
                                        zs,1.4f,1.4f,1.4f,1,0);
                            }
                            if ((ExtraMoves != 0) || ((Game.powerbits & 4) != 0)) {
                                tbuf[0] = PData[2].font3d_letter;
                                Text3D2(tbuf,(xs + 0.16f),(ys + 0.05f),zs,1.4f,1.4f,1.4f,1,0);
                            }
                            if ((ExtraMoves != 0) || ((Game.powerbits & 8) != 0)) {
                                tbuf[0] = PData[3].font3d_letter;
                                Text3D2(tbuf,(xs - 0.16f),(ys - 0.2f),zs,1.4f,1.4f,1.4f,1,0);
                            }
                            if ((ExtraMoves != 0) || ((Game.powerbits & 0x10) != 0)) {
                                tbuf[0] = PData[4].font3d_letter;
                                Text3D2(tbuf,xs,(ys - 0.2f),
                                        zs,1.4f,1.4f,1.4f,1,0);
                            }
                            if ((ExtraMoves != 0) || ((Game.powerbits & 0x20) != 0)) {
                                tbuf[0] = PData[5].font3d_letter;
                                Text3D2(tbuf,(xs + 0.16f),(ys - 0.2f),zs,1.4f,1.4f,1.4f,1,0);
                            }
                    break;
                    case 2:
                        //uVar8 = (u32)Game.language;
                        if ((Game.language == 2) || (Game.language == 5)) {
                            Text3D(tGEMS[Game.language],PAUSEPANELX,(ys + 0.34999999f),
                                   PAUSETEXTZ,0.5f,0.6f,0.6f,1,0);
                        }
                        else {
                            Text3D(tGEMS[Game.language],PAUSEPANELX,(ys + 0.34999999f),
                                   PAUSETEXTZ,0.6f,0.6f,0.6f,1,0);
                        }
                        if ((Game.gembits & 2) != 0) {
                            fVar3 = 0.1f;
                        }
                        else {
                            fVar3 = 0.0333f;
                        }
                        DrawPanel3DCharacter(0x7a,(xs - 0.079999998f),(ys + 0.2f),zs,fVar3,fVar3,fVar3,
                                   0,uVar10,0,-1,1.0f,1);
                        if ((Game.gembits & 4) != 0) {
                            fVar3 = 0.1f;
                        }
                        else {
                            fVar3 = 0.0333f;
                        }
                        DrawPanel3DCharacter (0x7b,(xs + 0.079999998f),(ys + 0.2f),zs,fVar3,fVar3,fVar3,
                                   0,uVar10,0,-1,1.0f,1);
                        if ((Game.gembits & 1) != 0) {
                            fVar3 = 0.1f;
                        }
                        else {
                            fVar3 = 0.0333f;
                        }
                        DrawPanel3DCharacter(0x79,(xs - 0.15f),(ys + 0.059999999f),zs,fVar3,fVar3,fVar3
                                   ,0,uVar10,0,-1,1.0f,1);
                        if ((Game.gembits & 0x10) != 0) {
                            fVar3 = 0.1f;
                        }
                        else {
                            fVar3 = 0.0333f;
                        }
                        DrawPanel3DCharacter(0x7d,xs,(ys + 0.059999999f),zs,fVar3,fVar3,fVar3,0,uVar10,0,-1,1.0f,1);
                        if ((Game.gembits & 8) != 0) {
                            fVar3 = 0.1f;
                        }
                        else {
                            fVar3 = 0.0333f;
                        }
                        dVar25 = (xs + 0.15f);
                        DrawPanel3DCharacter(0x7c,(xs + 0.15f),(ys + 0.059999999f),zs,fVar3,fVar3,fVar3
                                   ,0,uVar10,0,-1,1.0f,1);
                        sprintf(tbuf,"+%i %c =",Game.crate_gems + Game.bonus_gems,CLetter[119]);
                        if (Game.language == 0x63) {
                            AddSpacesIntoText(tbuf,1);
                        }
                        Text3D(tbuf,xs,0.0f,zs,0.6f,0.6f,0.6f,1,0);
                        BigOutOf((xs - 0.1f),(ys - 0.3f),0.52f,Game.gems,0x2e);
                        //fVar3 = 0.175f;
                        ys -= 0.275f;
                        //object = 0x78;
                        DrawPanel3DCharacter(0x78,dVar25,ys,zs,0.175f,0.175f,0.175f,0,uVar10,0,-1,1.0f,1);
                    break;
                    case 3:
                            if (Game.language == 5) {
                                Text3D(tRELICS[5],PAUSEPANELX,(ys + 0.34999999f),
                                       PAUSETEXTZ,0.5f,0.6f,0.6f,1,0);
                            }
                            else {
                                Text3D(tRELICS[Game.language],PAUSEPANELX,
                                       (ys + 0.34999999f),PAUSETEXTZ,0.6f,0.6f,0.6f,1,0);
                            }
                            //dVar25 = 0.15f;
                            dVar21 = (ys + 0.12f);
                            dVar24 = (xs - 0.15f);
                            DrawPanel3DObject(3,dVar24,dVar21,zs,0.08f
                                            ,0.08f,0.08f,0,0,0,ObjTab[3].obj.scene,ObjTab[3].obj.special,1);
                            sprintf(tbuf,"%i",sapphire_relics);
                            if (Game.language == 0x63) {
                                AddSpacesIntoText(tbuf,1);
                            }
                            dVar22 = (ys - 0.029999999f);
                            Text3D(tbuf,dVar24,(ys - 0.029999999f),zs,0.6f,0.6f,0.6f,1,0);
                            DrawPanel3DObject(2,xs,dVar21,zs,0.08f,0.08f,
                                              0.08f,0,0,0,ObjTab[2].obj.scene,ObjTab[2].obj.special,1);
                            sprintf(tbuf,"%i",gold_relics);
                            if (Game.language == 0x63) {
                                AddSpacesIntoText(tbuf,1);
                            }
                            Text3D(tbuf,xs,dVar22,zs,0.6f,0.6f,0.6f,1,0);
                            dVar24 = (xs + 0.15f);
                            DrawPanel3DObject(1,dVar24,dVar21,zs
                                            ,0.08f,0.08f,0.08f,0,0,0,ObjTab[1].obj.scene,ObjTab[1].obj.special,1);
                            sprintf(tbuf,"%i",platinum_relics);
                            if (Game.language == 0x63) {
                                AddSpacesIntoText(tbuf,1);
                            }
                            Text3D(tbuf,dVar24,dVar22,zs,0.6f,0.6f,0.6f,1,0);
                            BigOutOf((xs - 0.1f),(ys - 0.3f),0.52f,Game.relics,0x1e);
                            DrawPanel3DObject(3,dVar24,(ys - 0.32499999f),zs,0.125f,0.125f,0.125f,0,uVar10,0,
                                              ObjTab[3].obj.scene,ObjTab[3].obj.special,1);
                    break;
                    case 4:
                            Text3D(tTIMES[Game.language],PAUSEPANELX,
                                (ys + 0.34999999f),PAUSETEXTZ,0.6f,0.6f,0.6f,1,0);
                            DrawTimeTrialTimes(Level,(xs + 0.045f),(ys - 0.1f),(zs + 0.1f));
                    break;
                }
            }
        }
        goto LAB_800601c8;
    }
    if (plr != NULL) {
        bVar1 = 0;
        if (((plr_bonus_wumpas.count != 0) || (bonus_wumpa_delay != '\0')) ||
           (0.0f < bonus_wumpa_wait)) {
            bVar1 = 1;
        }
        if (((Bonus == 2) && (((plr->obj).dead == '\0' || (bVar1)))) || ((Bonus - 3U < 2 && (bVar1)))) {
            size = bonus_panel_wait;
            if ((size > 0.0f)) {
                if (0.0f < bonus_wumpa_wait){
                    size = (0.5f - bonus_wumpa_wait);
                }
                else{
                    size = 0.0f;
                }   
            }
            if (0.0f < size) {
                fVar3 = ((-1.2f - BONUSPANELSY) * size);
                fVar3 = BONUSPANELSY + fVar3 + fVar3;
            }
            else{
                fVar3 = BONUSPANELSY;
            }
            dVar25 = fVar3;
            sprintf(tbuf,"%i",(int)plr_bonus_wumpas.draw);
            if (Game.language == 0x63) {
                AddSpacesIntoText(tbuf,1);
            }
            Text3D(tbuf,BONUSWUMPATXTSX,dVar25,1.0f,0.6f,0.6f,0.6f,4,1);
            DrawPanel3DObject(0,BONUSWUMPAOBJSX,dVar25,1.0f,0.125f,0.125f,0.125f,0,PANELWUMPAYROT
                              ,0,ObjTab[0].obj.scene,ObjTab[0].obj.special,1);
        }
        bVar1 = 0;
        if ((bonus_finish_frame < save_bonus_crates_destroyed * 6 + 6) || (0.0f < bonus_crates_wait ))
        {
            bVar1 = 1;
        }
        if (((Bonus == 2) && (((plr->obj).dead == '\0' || (bVar1)))) || ((Bonus - 3U < 2 && (bVar1)))) {
            size = bonus_panel_wait;
            if ((size > 0.0f)) {
                if (0.0f < bonus_crates_wait){
                    size = (0.5f - bonus_crates_wait);
                }
                else{
                    size = 0.0f;
                }
            }
            if (0.0f < size) {
                fVar3 = ((-1.2f - BONUSPANELSY) * size);
                fVar3 = BONUSPANELSY + fVar3 + fVar3;
            }
            else{
                fVar3 = BONUSPANELSY;
            }
            dVar25 = fVar3;
            sprintf(tbuf,"%i/%i",bonus_crates_destroyed,DESTRUCTIBLEBONUSCRATECOUNT);
            if (Game.language == 0x63) {
                AddSpacesIntoText(tbuf,1);
            }
            Text3D(tbuf,BONUSCRATETXTSX,dVar25,1.0f,0.6f,0.6f,0.6f,4,2);
            DrawPanel3DObject(-1,BONUSCRATEOBJSX,dVar25,1.0f,0.09375f,0.09375f,0.09375f,0,
                              PANELCRATEYROT,0,crate_scene,crate_list[5].obj.special,1);
        }
        bVar1 = 0;
        if (((bonus_lives != 0) || (bonus_life_delay != '\0')) || (0.0f < bonus_lives_wait)) {
            bVar1 = 1;
        }
        if (((Bonus == 2) && (((plr->obj).dead == '\0' || (bVar1)))) || ((Bonus - 3U < 2 && (bVar1)))) {
            size = bonus_panel_wait;
            if ((size > 0.0f)) {
                if (0.0f < bonus_lives_wait){
                    size = (0.5f - bonus_lives_wait);
                }
                else{
                    size = 0.0f;
                }
            }

            if (0.0f < size) {
                fVar3 = ((-1.2f - BONUSPANELSY) * size);
                fVar3 = BONUSPANELSY + fVar3 + fVar3;
            }
            else{
                fVar3 = BONUSPANELSY;
            }
            //dVar25 = fVar3;
            sprintf(tbuf,"%i",bonus_lives);
            if (Game.language == 0x63) {
                AddSpacesIntoText(tbuf,1);
            }
            object = 0x11;
            Text3D(tbuf,BONUSLIVESTXTSX,fVar3,1.0f,0.6f,0.6f,0.6f,4,0);
            if (LDATA->character == 1) {
                object = 0x12;
            }
            DrawPanel3DObject(object,BONUSLIVESOBJSX,fVar3,1.0f,BONUSLIFESCALE,BONUSLIFESCALE
                              ,BONUSLIFESCALE,panel_head_xrot,panel_head_yrot + 0x2000,0,
                              ObjTab[object].obj.scene,ObjTab[object].obj.special,1);
        }
        if ((Bonus == 2) && ((plr->obj).dead == '\0')) {
            DrawGameMessage(tBONUS[Game.language],bonus_frame,0.5f);
        }
        if (Demo != 0) {
            DrawGameMessage(tDEMO[Game.language],GameTimer.frame,0.5f);
        }
        if (plr != NULL) {
            if (plr_wumpas.frame != 0) {
                ys = (0x1eU - plr_wumpas.frame) * 0.03333334f * 0.4f + PANELSY; //ternary?
                //ys = fVar3;
                DrawPanel3DObject(0,WUMPAOBJSX,ys,1.0f,0.2f,0.2f,0.2f,0xe800,PANELWUMPAYROT,0,
                                  ObjTab[0].obj.scene,ObjTab[0].obj.special,1);
                sprintf(tbuf,"%i",plr_wumpas.draw);
                if (Game.language == 0x63) {
                    AddSpacesIntoText(tbuf,1);
                }
                Text3D(tbuf,WUMPATXTSX,ys,1.0f,1.0f,1.0f,1.0f,4,1);
            }
            if (((plr_crates.frame != 0) && (LDATA->hub != -1)) && ((LDATA->flags & 2) == 0)) {
                ys = 0x1eU - plr_crates.frame * 0.03333334f * 0.4f + PANELSY;
                //ys = fVar3;
                DrawPanel3DObject(-1,CRATEOBJSX,ys,1.0f,0.15f,0.15f,0.15f,0xe800,PANELCRATEYROT,0,
                                  crate_scene,crate_list[5].obj.special,1);
                sprintf(tbuf,"%i",plr_crates.draw);
                if (Game.language == 0x63) {
                    AddSpacesIntoText(tbuf,1);
                }
                Text3D(tbuf,CRATETXTSX,ys,1.0f,1.0f,1.0f,1.0f,4,2);
                sprintf(tbuf,"/%i",DESTRUCTIBLECRATECOUNT);
                if (Game.language == 0x63) {
                    AddSpacesIntoText(tbuf,1);
                }
                Text3D(tbuf,font3d_xright,font3d_ybottom,1.0f,0.6f,0.6f,0.6f,0xc,2);
            }
            if (plr_lives.frame != 0) {
                object = 0x11;
                ys = (0x1eU - plr_lives.frame) * 0.03333334f * 0.4f + PANELSY;
                //ys = fVar3;
                if (LDATA->character == 1) {
                    object = 0x12;
                }
                DrawPanel3DObject(object,LIVESOBJSX,ys,1.0f,LIFESCALE,LIFESCALE,LIFESCALE,
                                  panel_head_xrot - 0x1000,panel_head_yrot + 0x2000,0,
                                  ObjTab[object].obj.scene,ObjTab[object].obj.special,1);
                sprintf(tbuf,"%i",plr_lives.draw);
                if (Game.language == 0x63) {
                    AddSpacesIntoText(tbuf,1);
                }
                Text3D(tbuf,LIVESTXTSX,ys,1.0f,1.0f,1.0f,1.0f,4,0);
            }
            uVar10 = (u16)(((GameTimer.frame % 0x78) * 0x10000) / 0x78);
            if (((plr_crystal.count != 0) && (plr_crystal.frame != 0)) && (LDATA->hub != -1)) {
                fVar3 = -0.7f - (0x1eU - plr_crystal.frame) * 0.03333334f * 0.6f;
                ys = fVar3;
                DrawPanel3DCharacter(0x75,0.0f,fVar3,1.0f,0.15f,0.15f,0.15f,0,uVar10,0,-1,0.0f,1);
            }
            if (((plr_crategem.count != 0) && (plr_crategem.frame != 0)) && (LDATA->hub != -1)) {
                ys = -0.7f - (0x1eU - plr_crategem.frame) * 0.03333334f * 0.6f;
                //ys = fVar3;
                DrawPanel3DCharacter(0x77,-0.2f,ys,1.0f,0.15f,0.15f,0.15f,0,uVar10,0,-1,0.0f,1);
            }
            if (((plr_bonusgem.count != 0) && (plr_bonusgem.frame != 0)) && (LDATA->hub != -1)) {
                ys = -0.7f - (0x1eU - plr_bonusgem.frame) * 0.03333334f * 0.6f;
                //ys = fVar3;
                if (plr_bonusgem.item == 8) {
                  object = 0x79;
                }
                else if (plr_bonusgem.item == 0x20) {
                  object = 0x7a;
                }
                else if (plr_bonusgem.item == 0x10) {
                  object = 0x7b;
                }
                else if (plr_bonusgem.item == 0x40) {
                  object = 0x7c;
                } else if (plr_bonusgem.item == 0x80) {
                    object = 0x7d;
                  }
                else {
                  object = 0x78;
                }
                DrawPanel3DCharacter(object,0.2f,ys,1.0f,0.15f,0.15f,0.15f,0,uVar10,0,-1,0.0f,1);
            }
            DrawWorldToPanelWumpa();
            //pNVar18 = NewWumpa;
            for (i = 0; i < 32; i++) {
                if ((NewWumpa[i].active != 0) && (NewWumpa[i].count != 0)) {
                    uVar10 = 0;
                    fVar3 = NewWumpa[i].screen_scale;
                    if (NewWumpa[i].bonus == 0) {
                        uVar10 = 0xe800;
                    }
                    DrawPanel3DObject(0,(NewWumpa[i].screen_pos).x,(NewWumpa[i].screen_pos).y,1.0f,fVar3,
                                      fVar3,fVar3,uVar10,PANELWUMPAYROT,0,ObjTab[0].obj.scene,
                                      ObjTab[0].obj.special,1);
                }
                //pNVar18 = pNVar18 + 1;
            } //while ((int)pNVar18 < 32);
            object = 0x20;
            if (TimeTrial != 0) {
                //cVar4 = (plr->obj).finished;
                if ((plr->obj).finished == 1) {
                    object = (finish_frame + 0x3c) - finish_frames;
                    if (object < 0) {
                        object = 0;
                    }
                    if (newleveltime_slot < 3) {
                        if ((new_lev_flags & 7) != 0) {
                            fVar3 = 0.35f;
                        }
                        else {
                            fVar3 = 0.2f;
                        }
                        iVar17 = 3;
                    }
                    else {
                        iVar17 = 4;
                        fVar3 = 0.1f;
                    }
                    dVar25 = iVar17 - 1;
                    zs = 0.0f;
                    xs = (fVar3 - (dVar25 * TT_TIMESDY) * 0.5f);
                    dVar25 = ((newleveltime_slot) * TT_TIMESDY + xs);
                    if (object < 0x1e) {
                        if ((object == 0) || (dVar21 = zs, 5 < object % 0xc)) {
                            zs = tt_sx;
                            dVar25 = tt_sy;
                            dVar21 = TT_SCALE;
                        }
                    }
                    else {
                        dVar21 = 0.89999998f;
                        if (object < 0x3c) {
                            ys = ((object - 0x1eU) / 30.0f);
                            zs = ((0.0f - tt_sx) * ys  + tt_sx);
                            dVar25 = ((dVar25 - tt_sy) * ys + tt_sy);
                            dVar21 = ((0.89999998f - TT_SCALE) * ys + TT_SCALE);
                        }
                    }
                    //bVar1 = 0x3b < object;
                    if (((newleveltime_slot == 3) || (0x3b > object)) && (0.0f < dVar21)) {
                        MakeTimeI(TimeTrialTimer.itime,0,txt);
                        if (Game.language == 0x63) {
                            if (finish_frame < finish_frames - 0x3c) {
                                pcVar9 = "    %c   %s";
//LAB_8005ef58:
                                sprintf(tbuf,pcVar9,CLetter[118],txt);
                            }
                            else {
                                if (newleveltime_slot < 3) {
                                    pcVar9 = "_ _ _   %s";
                                }
                                else {
                                    pcVar9 = "        %s";
                                }
                                sprintf(tbuf,pcVar9,txt);
                            }
                        }
                        else {
                            if (finish_frame < finish_frames - 0x3c) {
                                pcVar9 = "  %c %s";
                                sprintf(tbuf,pcVar9,CLetter[118],txt);
                                //goto LAB_8005ef58;
                            }
                            if (newleveltime_slot < 3) {
                                pcVar9 = "___ %s";
                            }
                            else {
                                pcVar9 = "    %s";
                            }
                            sprintf(tbuf,pcVar9,txt);
                        }
                        Text3D(tbuf,zs,dVar25,1.0f,dVar21,dVar21,dVar21,1,4);
                    }
                    for (i = 0; i < 3; i++) {
                        if (0x3b > i) {
                            return;
                        }
                        MakeLevelTimeString(&Game.level[Level].time[i],tbuf);
                        if (i == newleveltime_slot) {
                            pcVar9 = MakeEditText(tbuf);
                            Text3D(pcVar9,zs,dVar25,1.0f,dVar21,dVar21,dVar21,1,4);
                        }
                        else {
                            Text3D(tbuf,0.0f,xs,1.0f,0.8f,0.8f,0.8f,1,0);
                        }
                        if (i == newleveltime_slot) {
                            if ((new_lev_flags & 4) != 0) {
                                iVar17 = 1;
                            }
                            else {
                                if ((new_lev_flags & 2) != 0) {
                                    iVar17 = 2;
                                }
                                else {
                                    if ((new_lev_flags & 1) != 0) {
                                        iVar17 = 3;
                                    } else{
                                       iVar17 = -1; 
                                    }
                                }
                                if (iVar17 != -1) {
                                    DrawPanel3DObject(iVar17,TT_RELICX,TT_RELICY,1.0f,0.125f,0.125f,0.125f,0,0,0,ObjTab[iVar17].obj.scene,ObjTab[iVar17].obj.special,1);
                                }
                            }

                        }
                        //object = object + 1;
                        xs = (xs + TT_TIMESDY);
                    } //while (object < 3);
                    if (((tt_flash != 0) && (0x3b < i)) && ((Cursor.menu != '\x10' || ((newleveltime_slot == 3 && (Cursor.menu_frame < 300)))))) {
                        switch(newleveltime_slot){
                            case 1:
                                strcpy(tbuf,t2NDBESTTIME[Game.language]);
                            break;
                            case 0:
                                strcpy(tbuf,tBESTTIME[Game.language]);
                            break;
                            case 2:
                                strcpy(tbuf,t3RDBESTTIME[Game.language]);
                            break;
                            default:
                            strcpy(tbuf,tNONEWTIME[Game.language]);
                            break;
                        }
                        Text3D(tbuf,0.0f,TT_MESSAGEY,1.0f,0.8f,0.8f,0.8f,1,3);
                    }
                }
                else /*if ((plr->obj).finished == '\0')*/ {
                    if ((timetrial_frame == 0x3c || timetrial_frame == 0x3d) && (timetrial_frame % 0x3c < 0x30)) {
                        Text3D(tENTERINGTIMETRIAL[Game.language],0.0f,0.0f,1.0f,0.6f,0.6f,0.6f,1,4);
                    }
                    uVar8 = TimeTrialTimer.itime;
                    if (0x2bf1d < (int)TimeTrialTimer.itime) {
                        uVar8 = 0x2bf1d;
                    }
                    MakeTimeI(uVar8,0,txt);
                    if (Game.language == 0x63) {
                        pcVar9 = "    %c   %s";
                    }
                    else {
                        pcVar9 = "  %c %s";
                    }
                    sprintf(tbuf,pcVar9,CLetter[118],txt);
                    fVar3 = tt_sx;
                    fVar6 = tt_sy;
                    if (timetrial_frame < 0x2d) {
                        if (timetrial_frame % 0xf < 7) {
                            fVar5 = 0.5f;
                            fVar3 = TTScrPos.x;
                            fVar6 = TTScrPos.y;
                        }
                        else {
                            fVar5 = 0.0f;
                        }
                    }
                    else if (timetrial_frame < 0x3c) {
                        ys = ((timetrial_frame - 0x2d) / 15.0);
                        fVar6 = (TTScrPos.y - tt_sy) * (1.0f - ys) + tt_sy;
                        fVar3 = (TTScrPos.x - tt_sx) * (1.0f - ys) + tt_sx;
                        fVar5 = (ys * 0.5f + 0.5f);
                    }
                    else {
                        fVar5 = 1.0f;
                    }
                    if (0.0f < fVar5) {
                        iVar17 = 0;
                        fVar5 = TT_SCALE * fVar5;
                        if (0.0f < TimeTrialWait) {
                            iVar17 = 3;
                        }
                        Text3D(tbuf,fVar3,fVar6,1.0f,fVar5,fVar5,fVar5,1,iVar17);
                    }
                }
            }
            if ((((LDATA->flags & 0x200) != 0) && (VEHICLECONTROL == 1)) && ((plr->obj).finished == '\0')) {
                object = 0x11;
                if (LDATA->character == 1) {
                    object = 0x12;
                }
                DrawPanel3DObject(object,-0.75f,-0.75f,1.0f,0.2f,0.2f,0.2f,WibbleYRot,WibbleXRot + 0xe000,0,
                                  ObjTab[object].obj.scene,ObjTab[object].obj.special,1);
                object = GetGliderHealthPercentage(plr);
                sprintf(tbuf,"%.3i%%",object);
                if (tbuf[0] == '0') {
                    if (tbuf[1] == '0') {
                        tbuf[1] = ' ';
                    }
                    else{
                        tbuf[0] = ' ';
                    }
                }
                if (Game.language == 0x63) {
                    AddSpacesIntoText(tbuf,1);
                }
                Text3D(tbuf,-0.7f,-0.75f,1.0f,0.8f,0.8f,0.8f,4,0);
                if (Level == 0x18) {
                    dVar21 = ATMBOSSBARX;
                    xs = ATMBOSSBARY;
                    zs = ATMBOSSBARZ;
                    dVar25 = ATMBOSSBARSCALEX;
                    ys = ATMBOSSBARSCALEY;
                    object = GetCurrentLevelObjectives();
                    //iVar17 = GetMaxLevelObjectives();
                    BossBar(dVar21,xs,zs,dVar25,ys,object,GetMaxLevelObjectives());
                }
                else {
                    object = GetCurrentLevelObjectives();
                    iVar17 = GetMaxLevelObjectives();
                    sprintf(tbuf,"%i/%i",iVar17 - object, iVar17);
                    if (Game.language == 0x63) {
                        AddSpacesIntoText(tbuf,1);
                    }
                    Text3D(tbuf,0.65,-0.75f,1.0f,0.8f,0.8f,0.8f,0x10,0);
                }
                if (Level == 0x12) {
                    uVar8 = (u32)((unsigned long long)GameTimer.frame * 0x88888889 >> 0x20);
                    fVar3 = PANELSINKX;
                    fVar6 = PANELSINKY;
                    fVar5 = PANELSINKZ;
                    scale = PANELSINKSCALE;
                }
                else {
                    if (Level == 0x18) {
                        uVar10 = WibbleXRot + 0x2000;
                        WibbleYRot = WibbleYRot + PANELCRUNCHXROT;
                        fVar3 = PANELCRUNCHY;
                        //goto LAB_8005f9b4;
                    }
                    else{
                        //uVar8 = (u32)((unsigned long long)GameTimer.frame * 0x88888889 >> 0x20);
                        fVar3 = PANELBLIMPX;
                        fVar6 = PANELBLIMPY;
                        fVar5 = PANELBLIMPZ;
                        scale = PANELBLIMPSCALE;          
                    }
                }
                DrawPanel3DTempCharacter(fVar3,fVar6,fVar5,scale,panel_head_xrot, panel_head_yrot +
                           (((GameTimer.frame + (uVar8 >> 6) * -0x78) * 0x10000) / 0x78),0,0);
            }
            else {
                if ((Level == 0x15) && ((plr->obj).finished == '\0')) {
                    DrawPanel3DObject(0x11,-0.75f,-0.75f,1.0f,0.2f,0.2f,0.2f,WibbleYRot,WibbleXRot + 0xe000,0,
                                      ObjTab[17].obj.scene,ObjTab[17].obj.special,1);
                    //object = GetRumblePlayerHealthPercentage(plr);
                    sprintf(tbuf,"%.3i%%",GetRumblePlayerHealthPercentage(plr));
                    if (tbuf[0] == '0') {
                        if (tbuf[1] == '0') {
                            tbuf[0] = ' ';
                            tbuf[1] = ' ';
                        }
                        else {
                            tbuf[0] = ' ';
                        }
                    }
                    if (Game.language == 0x63) {
                        AddSpacesIntoText(tbuf,1);
                    }
                    Text3D(tbuf,-0.7f,-0.75f,1.0f,0.8f,0.8f,0.8f,4,0);
                    RumbleHeadUpDisplay();
                    dVar21 = RUMBOSSBARX;
                    xs = RUMBOSSBARY;
                    zs = RUMBOSSBARZ;
                    dVar25 = RUMBOSSBARSCALEX;
                    ys = RUMBOSSBARSCALEY;
                    //object = GetCurrentLevelObjectives();
                    //iVar17 = GetMaxLevelObjectives();
                    BossBar(dVar21,xs,zs,dVar25,ys,GetCurrentLevelObjectives(),GetMaxLevelObjectives());
                    uVar10 = WibbleXRot + 0x2000;
                    WibbleYRot = WibbleYRot + PANELCRUNCHXROT;
                    fVar3 = PANELCRUNCHY;
                }
                else if ((Level == 0x16) && ((plr->obj).finished == '\0')) {
                    dVar21 = FIREBOSSBARX;
                    xs = FIREBOSSBARY;
                    zs = FIREBOSSBARZ;
                    dVar25 = FIREBOSSBARSCALEX;
                    ys = FIREBOSSBARSCALEY;
                   // object = GetCurrentLevelObjectives();
                    //iVar17 = GetMaxLevelObjectives();
                    BossBar(dVar21,xs,zs,dVar25,ys,GetCurrentLevelObjectives(),GetMaxLevelObjectives());
                    uVar10 = 0x2000 - WibbleXRot;
                    WibbleYRot = WibbleYRot + PANELCRUNCHXROT;
                    fVar3 = PANELCRUNCHY;
                }
                else {
                    if ((Level != 0x19) || ((plr->obj).finished == '\0')) {
                    dVar21 = FIREBOSSBARX;
                    xs = FIREBOSSBARY;
                    zs = FIREBOSSBARZ;
                    dVar25 = FIREBOSSBARSCALEX;
                    ys = FIREBOSSBARSCALEY;
                    //object = GetCurrentLevelObjectives();
                    //iVar17 = GetMaxLevelObjectives();
                    BossBar(dVar21,xs,zs,dVar25,ys,GetCurrentLevelObjectives(),GetMaxLevelObjectives());
                    uVar10 = 0x2000 - WibbleXRot;
                    WibbleYRot = WibbleYRot + PANELCRUNCHXROT;
                    fVar3 = PANELCRUNCHY - 0.1f;
                    DrawPanel3DTempCharacter(PANELCRUNCHX,fVar3,PANELCRUNCHZ,PANELCRUNCHSCALE,WibbleYRot,uVar10,0,0);
                    }
                    if ((Level == 3) && ((plr->obj).finished == '\0')) {
                            if (SmokeyCountDownValue > 0) {
                                tbuf[0] = (char)SmokeyCountDownValue + '0';
                                if (Game.language == 0x63) {
                                    tbuf[1] = ' ';
                                    tbuf[2] = '\0';
                                }
                                else {
                                    tbuf[1] = '\0';
                                }
                                Text3D(tbuf,0.0f,0.0f,1.0f,1.5f,1.5f,1.5f,1,0);
                            }
                            else {
                                if (SmokeyCountDownValue == -1) {
                                    fVar3 = 2.5f;
                                    if (Game.language == 4) {
                                        fVar3 = 2.25f;
                                    }
                                    Text3D(tOK[Game.language],0.0f,0.0f,1.0f,fVar3,fVar3,fVar3,1,4);
                                }
                            }
                            if (in_finish_range != 0) goto LAB_800601c4;
                            object = 0x11;
                            if (LDATA->character == 1) {
                                object = 0x12;
                            }
                            DrawPanel3DObject(object,-0.75f,-0.75f,1.0f,0.2f,0.2f,0.2f,panel_head_xrot,panel_head_yrot + 0xe000,0,ObjTab[object].obj.scene,
                                              ObjTab[object].obj.special,1);
                            Text3D(PlaceName[Game.language][(SmokeyPosition + -1) * 0x18],-0.68f,-0.75f,1.0f,0.8f,0.8f,0.8f,4,-(SmokeyPosition == 1) & 4 );
                        }
                        else if (Level == 0x1c) {
                            if (GameTimer.frame < 0xb4) {
                                tbuf[0] = '3' - (char)(GameTimer.frame / 0x3c);
                                if (Game.language == 0x63) {
                                    tbuf[2] = '\0';
                                    tbuf[1] = ' ';
                                }
                                else {
                                    tbuf[1] = '\0';
                                }
                                object = 0;
                                ys = 1.5f;
                            }
                            else if (GameTimer.frame < 0xf0) {
                                strcpy(tbuf,tOK[Game.language]);
                                ys = 2.5f;
                                if (Game.language == '\x04') {
                                    ys = 2.25f;
                                }
                                object = 4;
                            }
                            else {
                                tbuf[0] = '\0';
                            }
                            if (tbuf[0] != '\0') {
                                Text3D(tbuf,0.0f,0.0f,1.0f,ys,ys,ys, 1,object);
                            }
                            if (in_finish_range != 0) goto LAB_800601c4;
                            DrawPanel3DObject(0x11,-0.75f,-0.75f,1.0f,0.2f,0.2f,0.2f,panel_head_xrot,panel_head_yrot + 0xe000,0,ObjTab[17].obj.scene,
                                              ObjTab[17].obj.special,1);
                            Text3D(PlaceName[Game.language][(PlrTub.place + -1) * 0x18],-0.68f,-0.75f,1.0f,0.8f,0.8f,0.8f,4,-(u32)(PlrTub.place == 1) & 4);
                            object = PlrTub.laps + 1;
                            if (MAXLAPS > PlrTub.laps + 1) {
                                object = MAXLAPS;
                            }
                            sprintf(tbuf,"%i/%i",object);
                            if (Game.language == 0x63) {
                                AddSpacesIntoText(tbuf,1);
                            }
                            iVar17 = 0;
                            if (0 < object) {
                                iVar17 = 4;
                            }
                            Text3D(tbuf,0.7f,-0.75f,1.0f,0.8f,0.8f,0.8f,0x10,iVar17);
                        }
                        else if (Level == 0x1d) {
                            if (GameTimer.frame < 0xb4) {
                                ys = 1.5f;
                                tbuf[1] = '\0';
                                object = 0;
                                tbuf[0] = '3' - (char)(GameTimer.frame / 0x3c);
                            }
                            else if (GameTimer.frame < 0xf0) {
                                strcpy(tbuf,tOK[Game.language]);
                                ys = 2.5f;
                                if (Game.language == '\x04') {
                                    ys = 2.25f;
                                }
                                object = 4;
                            }
                            else {
                                tbuf[0] = '\0';
                            }
                            if (tbuf[0] != '\0') {
                                Text3D(tbuf,0.0f,0.0f,1.0f,ys,ys,ys,1,object);
                            }
                            if (in_finish_range != 0) goto LAB_800601c4;
                            if (0 < i_ring) {
                                ys = (((0x1eU - carpet_panelpos) / 30.0f) * 0.4f);
                                DrawPanel3DObject(0x11,-0.75f,(-0.75f - ys),1.0f,0.2f,0.2f,0.2f,panel_head_xrot,panel_head_yrot + 0xe000,0,
                                                  ObjTab[17].obj.scene,ObjTab[17].obj.special,1);
                                if (carpet_place == 1) {
                                    pcVar9 = PlaceName[1][Game.language];
                                }
                                else {
                                    pcVar9 = PlaceName[0][Game.language];
                                }
                                if (carpet_place != 1) {
                                    object = 4;
                                }
                                else{
                                    object = 0;
                                }
                                Text3D(pcVar9,-0.7f,(-0.75f - ys),1.0f,0.8f,0.8f,0.8f,4,object);
                            }
                            sprintf(tbuf,"%i/%i",i_ring,RINGCOUNT);
                            if (Game.language == 0x63) {
                                AddSpacesIntoText(tbuf,1);
                            }
                            object = 0;
                            if (0 < i_ring) {
                                object = 4;
                            }
                            else{
                                object = 0;
                            }
                            Text3D(tbuf,0.7f,-0.75f,1.0f,0.8f,0.8f,0.8f,0x10,object);
                            DrawPanel3DObject(0x68,PANELRINGX,PANELRINGY,1.0f,PANELRINGSIZE,PANELRINGSIZE,PANELRINGSIZE,0,0,0,
                                              ObjTab[104].obj.scene,ObjTab[104].obj.special,1);
                            if (ObjTab[108].obj.special != NULL) {
                                DrawPanel3DArrow();
                            }
                        }
                        //goto LAB_8006012c;
                }
//LAB_8005f9b4:
            }
//LAB_8006012c:
            if ((((in_finish_range == 0) && (TimeTrial == 0)) && (GATECOUNT != 0)) &&
               (plr_gates != 0)) {
                sprintf(tbuf,"%i/%i");
                if (Game.language == 0x63) {
                    AddSpacesIntoText(tbuf,1);
                }
                Text3D(tbuf,-0.75f,-0.75f,1.0f,0.8f,0.8f,0.8f,4,4);
            }
        }
    }
LAB_800601c4:
    DrawPanelDebris();
LAB_800601c8:
    NuShaderSetBypassShaders(0);
    pNuCam->mtx = pCam->m;
    NuCameraSet(pNuCam);
    GameVP();
    return;
}

//NGC MATCH
s32 DrawPanel3DTempCharacter(float x, float y, float z, float scale, u16 xrot, u16 yrot, u16 zrot, s32 rot) {
    struct numtx_s m;
    struct nuvec_s s;
    struct numtx_s *cammtx;
    struct CharacterModel* model;
    float *dwa[7];
    
    if ((temp_character != 2) && (temp_character != 0x62) && (temp_character != 0xb8) &&
        (temp_character != 0xb9) && (temp_character != 0xba) && (temp_character != 0xbc) ||
       (CRemap[temp_character] == -1)) {
        return 0;
    }
    
    s.x = s.y = s.z = scale;
    model = &CModel[CRemap[temp_character]];
    NuMtxSetScale(&m, &s);
    switch (rot) {
        case 0:
            if (xrot != 0) {
                NuMtxRotateX(&m, xrot);
            }
            if (yrot != 0) {
                NuMtxRotateY(&m, yrot);
            }
            if (zrot != 0) {
                NuMtxRotateY(&m, zrot);
            }
            break;
        case 1:
            if (yrot != 0) {
                NuMtxRotateY(&m, yrot);
            }
            if (xrot != 0) {
                NuMtxRotateX(&m, xrot);
            }
            if (zrot != 0) {
                NuMtxRotateX(&m, zrot);
            }
            break;
    }
    m._30 = x * PANEL3DMULX;
    m._32 = z;
    m._31 = y * PANEL3DMULY;
    cammtx = NuCameraGetMtx();
    NuMtxMul(&m, &m, cammtx);
    EvalModelAnim(model, &TempAnim, &m, tmtx, &dwa, NULL);
    return NuHGobjRndrMtxDwa(model->hobj, &m, 1, NULL, tmtx, *dwa);
}

//NGC MATCH
s32 DrawPanel3DObject(s32 object,float x,float y,float z,float scalex,float scaley,float scalez,
                     u16 xrot,u16 yrot,u16 zrot,struct nugscn_s *scn,struct nuspecial_s *obj,s32 rot) {
    struct numtx_s m;
    struct nuvec_s s;
    s32 i;
    struct numtx_s *cammtx;
    char pad [17];
    

    if (((scn != NULL) && (obj != NULL)) &&
       ((scalex != 0.0f || ((scaley != 0.0f || (scalez != 0.0f)))))) {
        s.x = scalex;
        s.y = scaley;
        s.z = scalez;
        NuMtxSetScale(&m,&s);
        switch (rot) {
                case 0:
                    if (xrot != 0) {
                        NuMtxRotateX(&m,xrot);
                    }
                    if (yrot != 0) {
                        NuMtxRotateY(&m, yrot);
                    }
                    if (zrot != 0) {
                        NuMtxRotateZ(&m, zrot);
                    }
                    break;
                case 1:
                    if (yrot != 0) {
                        NuMtxRotateY(&m, yrot);
                    }
                    if (xrot != 0) {
                        NuMtxRotateX(&m, xrot);
                    }
                    if (zrot != 0) {
                        NuMtxRotateZ(&m, zrot);
                    }
                    break;
        }
        m._30 = x * PANEL3DMULX;
        m._32 = z;
        m._31 = y * PANEL3DMULY;
        cammtx = NuCameraGetMtx();
        NuMtxMul(&m,&m,cammtx);
        SetLevelLights();
        i = NuRndrGScnObj(scn->gobjs[obj->instance->objid],&m);
    }
    else {
        i = 0;
    }
    return i;
}


//NGC MATCH
s32 DrawPanel3DCharacter(s32 character,float x,float y,float z,float scalex,float scaley,float scalez,
              u16 xrot,u16 yrot,u16 zrot,s32 action,float anim_time,s32 rot) {
    s32 iVar1;
    s32 i;
    struct numtx_s *cammtx;
    struct CharacterModel *model;
    struct numtx_s m;
    struct nuvec_s s;
    char pad[25];
    

    if ((((scalex == 0.0f) && (scaley == 0.0f)) && (scalez == 0.0f)) ||
       ((0xbe < (uint)character || (CRemap[character] == -1)))) {
        i = 0;
    }
    else {
        s32 iVar3;
        model = &CModel[CRemap[character]];
        if (character == 0x75) {
            iVar3 = 0x84;
        } else if (character == 0x77) {
            iVar3 = 0x88;
        } else if (character == 0x78) {
            iVar3 = 0x89;
        } else if (character == 0x79) {
            iVar3 = 0x8a;
        } else if (character == 0x7a) {
            iVar3 = 0x8b;
        } else if (character == 0x7b) {
            iVar3 = 0x8c;
        } else if (character == 0x7c) {
            iVar3 = 0x8D;
        } else if (character == 0x7D) {
            iVar3 = 0x8E;
        } else if (character == 0x7E) {
            iVar3 = 0x8F;
        } else if (character == 0x7F) {
            iVar3 = 0x90;
        } else {
            iVar3 = -1;
        }
        s.x = scalex;
        s.y = scaley;
        s.z = scalez;
        NuMtxSetScale(&m,&s);
        switch (rot) {
                case 0:
                    if (xrot != 0) {
                        NuMtxRotateX(&m,xrot);
                    }
                    if (yrot != 0) {
                        NuMtxRotateY(&m,yrot);
                    }
                    if (zrot != 0) {
                        NuMtxRotateZ(&m,zrot);
                    }
                    break;
                case 1:
                    if (yrot != 0) {
                        NuMtxRotateY(&m,yrot);
                    }
                    if (xrot != 0) {
                        NuMtxRotateX(&m,xrot);
                    }
                    if (zrot != 0) {
                        NuMtxRotateZ(&m,zrot);
                    }
                    break;
        }
        m._30 = x * PANEL3DMULX;
        m._32 = z;
        m._31 = y * PANEL3DMULY;
        cammtx = NuCameraGetMtx();
        NuMtxMul(&m,&m,cammtx);
        if (((uint)action < 0x76) && (model->anmdata[action] != NULL)) {
            NuHGobjEvalAnim(model->hobj,model->anmdata[action],anim_time,0,NULL,tmtx);
        }
        else {
            NuHGobjEval(model->hobj,0,NULL,tmtx);
        }
        i = NuHGobjRndrMtx(model->hobj,&m,1,NULL,tmtx);
    }
    return i;
}

//NGC MATCH
void DrawPanelDebris(void) {
  s32 i;
  struct pdeb_s *deb;
  float f;

  deb = PDeb;
  for (i = 0; i < 0x20; i++, deb++) {
    if (deb->active != '\0') {
      if (deb->type == 5 || deb->type == 4 || deb->type == 3 || deb->type == 2) {
        if ((0.25f <= deb->time) && (deb->time < 1.0f)) {
          f = (float)fmod((deb->time - 0.25f),0.25f);
          if ((0.0625f <= f) && (f < 0.1875f)) continue;
        }
      }
      DrawPanel3DObject(deb->i_objtab,(deb->pos).x,(deb->pos).y,1.0f,deb->scale,deb->scale,deb->scale,deb->xrot,
                        deb->yrot,deb->zrot,ObjTab[deb->i_objtab].obj.scene,ObjTab[deb->i_objtab].obj.special,1);
    }
  }
    return;
}
