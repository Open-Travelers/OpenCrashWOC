s32 MAXVPSIZEX;
s32 MINVPSIZEX;
s32 MAXVPSIZEY;
s32 MINVPSIZEY;
s32 screendump;
s32 save_paused;
s32 editor_active;

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


void DrawPanel(void)

{
  bool bVar1;
  bool bVar2;
  float fVar3;
  char cVar4;
  float fVar5;
  float fVar6;
  float scale;
  nucamera_s *camera;
  float *pfVar7;
  uint uVar8;
  int object;
  char *pcVar9;
  ushort uVar10;
  short sVar11;
  ushort uVar12;
  char **ppcVar13;
  float *pfVar14;
  nucamera_s *pnVar15;
  nucamera_s *pnVar16;
  creature_s *plr;
  int iVar17;
  NewWumpa *pNVar18;
  double dVar19;
  double dVar20;
  double dVar21;
  double dVar22;
  double dVar23;
  double dVar24;
  double dVar25;
  char txt [32];
  double local_70;
  
  dVar23 = 0.0;
  plr = (creature_s *)0x0;
  if (PLAYERCOUNT != 0) {
    plr = player;
  }
  uVar12 = (ushort)(int)(*(float *)((int)NuTrigTable +
                                   (((GlobalTimer.frame % 300) * 0x10000) / 0x4b & 0x3fffc)) *
                         2731.0 + 1820.0);
  sVar11 = (short)(int)(*(float *)((int)NuTrigTable +
                                  ((((GlobalTimer.frame % 0xf0) * 0x10000) / 0xf0 + 0x4000) * 4 &
                                  0x3fffc)) * 2731.0);
  local_70 = (double)CONCAT44(0x43300000,SWIDTH ^ 0x80000000);
  dVar19 = local_70 - 4503601774854144.0;
  local_70 = (double)CONCAT44(0x43300000,PHYSICAL_SCREEN_X ^ 0x80000000);
  dVar20 = local_70 - 4503601774854144.0;
  local_70 = (double)CONCAT44(0x43300000,SHEIGHT ^ 0x80000000);
  dVar25 = local_70;
  local_70 = (double)CONCAT44(0x43300000,PHYSICAL_SCREEN_Y ^ 0x80000000);
  PANEL3DMULX = (float)dVar19 / (DIVPANEL3DX * ((float)dVar19 / (float)dVar20));
  PANEL3DMULY = (float)(dVar25 - 4503601774854144.0) /
                (DIVPANEL3DY *
                ((float)(dVar25 - 4503601774854144.0) / (float)(local_70 - 4503601774854144.0)));
  object = Paused;
  if (screendump != 0) {
    object = save_paused;
  }
  bVar1 = object == 0;
  if (((bVar1) && ((LDATA->flags & 1) != 0)) && (PANELOFF != 0)) {
    return;
  }
  uVar10 = -(short)(((GlobalTimer.frame % 0x78) * 0x10000) / 0x78);
  if ((pause_rndr_on == 0) && (cutmovie == -1)) {
    Draw3DCheckpointLetters();
  }
  MaxVP();
  if ((ShowPlayerCoordinate != 0) && (plr != (creature_s *)0x0)) {
    NuCameraTransformScreenClip(&vTEMP,&(plr->obj).pos,1,(numtx_s *)0x0);
  }
  SetLevelLights();
  NuShaderSetBypassShaders(1);
  DrawMenu(&Cursor,object);
  if ((Cursor.menu == '\x13') || (cutmovie != -1)) goto LAB_800601c8;
  if ((Level == 0x25) &&
     ((((Hub != -1 && (bVar1)) && (dVar25 = 0.0, hubleveltext_pos < 0.0)) &&
      (hubleveltext_level != -1)))) {
    dVar23 = (double)((0.333 - hubleveltext_pos) / 0.333);
    if (hubleveltext_open != 0) {
      Text3D(LevelName[hubleveltext_level * 6 + (uint)Game.language],0.0,
             (float)(dVar23 * 0.4000000059604645 + 0.800000011920929),1.0,0.6,0.6,0.6,1,0);
    }
    if ((uint)hubleveltext_i < 5) {
      if (hubleveltext_open == 0) {
        if (Hub == 5) {
          sprintf(tbuf,tRELICSREQUIRED[Game.language],(hubleveltext_i + 1) * 5);
          Text3D(tbuf,0.0,(float)(dVar23 * 0.300000011920929 + 0.8500000238418579),1.0,0.6,0.6,0. 6,1
                 ,3);
        }
      }
      else {
        if (hubleveltext_i == 2) {
          fVar3 = -0.68 - (float)(dVar23 * 0.800000011920929);
        }
        else {
          fVar3 = -0.68;
          if (hubleveltext_i < 3) {
            dVar25 = (double)(float)(dVar23 * 0.800000011920929 + 0.5);
          }
          else {
            dVar25 = (double)(-0.5 - (float)(dVar23 * 0.800000011920929));
          }
        }
        DrawTimeTrialTimes(hubleveltext_level,(float)dVar25,fVar3,1.0);
      }
    }
  }
  if ((LDATA->flags & 1) == 0) goto LAB_800601c8;
  bVar2 = plr != (creature_s *)0x0;
  if ((bVar2) && (ShowPlayerCoordinate != 0)) {
    sprintf(tbuf,"X:%.2f Y:%.2f Z:%.2f",(double)(plr->obj).pos.x,(double)(plr->obj).pos.y,
            (double)(plr->obj).pos.z);
    if (Game.language == 'c') {
      AddSpacesIntoText(tbuf,1);
    }
    Text3D(tbuf,vTEMP.x,vTEMP.y,1.0,0.4,0.5,0.5,1,3);
  }
  if (!bVar1) {
    if ((byte)Cursor.menu - 5 < 6) {
      fVar3 = -0.02;
      if (pause_dir != 0) {
        local_70 = (double)CONCAT44(0x43300000,0x1eU - object ^ 0x80000000);
        dVar23 = (double)((float)(local_70 - 4503601774854144.0) * (PAUSEVMUL / 30.0));
        fVar3 = (float)(-0.01999999955296516 - dVar23);
      }
      dVar25 = 0.1000000014901161;
      DrawPanel3DObject(99,0.0,fVar3,PAUSEPANELZ + 0.01,0.1,0.1,(float)(dVar23 * 0.100000001490116 1)
                        ,0,0,0,ObjTab[99].obj.scene,ObjTab[99].obj.special,1);
      fVar3 = 0.0;
      if (pause_dir != 0) {
        local_70 = (double)CONCAT44(0x43300000,0x1eU - object ^ 0x80000000);
        dVar23 = (double)((float)(local_70 - 4503601774854144.0) * (PAUSEHMUL / 30.0));
        fVar3 = (float)(dVar23 + 0.0);
      }
      DrawPanel3DObject(0x62,fVar3,0.0,PAUSEPANELZ - 0.01,(float)dVar25,(float)dVar25,
                        (float)(dVar23 * dVar25),0,0,0,ObjTab[98].obj.scene,ObjTab[98].obj.special ,1
                       );
      if (object == 0x1e) {
        Text3D(tTCR_PRESSSTARTTORESUME[Game.language],PANELMENUX * 0.9,menu_pulsate * 0.05 + 0.64 ,
               1.0,0.4,0.4,0.4,1,0);
        Text3D(Game.name,PAUSEPANELX,PAUSENAMEY,PAUSETEXTZ,0.6,0.6,0.6,1,0);
        sprintf(tbuf,"%i%%",(uint)Game.percent);
        if (Game.language == 'c') {
          AddSpacesIntoText(tbuf,1);
        }
        Text3D(tbuf,PAUSEPANELX,PAUSEPERCENTY - 0.0,PAUSETEXTZ,1.0,1.0,1.0,1,0);
        object = 4;
        if (LDATA->hub != -1) {
          object = 5;
        }
        dVar20 = (double)PAUSEPANELX;
        dVar23 = (double)PAUSEINFOY;
        dVar19 = (double)PAUSETEXTZ;
        uVar8 = (pausestats_frame - (pausestats_frame / (uint)(object * 0xb4)) * object * 0xb4) /
                0xb4;
        if (uVar8 == 2) {
          uVar8 = (uint)Game.language;
          if ((uVar8 == 2) || (uVar8 == 5)) {
            Text3D(tGEMS[uVar8],PAUSEPANELX,(float)(dVar23 + 0.3499999940395355),PAUSETEXTZ,0.5,0. 6,
                   0.6,1,0);
          }
          else {
            Text3D(tGEMS[uVar8],PAUSEPANELX,(float)(dVar23 + 0.3499999940395355),PAUSETEXTZ,0.6,0. 6,
                   0.6,1,0);
          }
          if ((Game.gembits & 2) == 0) {
            fVar3 = 0.0333;
          }
          else {
            fVar3 = 0.1;
          }
          DrawPanel3DCharacter
                    (0x7a,(float)(dVar20 - 0.07999999821186066),(float)(dVar23 + 0.200000002980232 2)
                     ,(float)dVar19,fVar3,fVar3,fVar3,0,uVar10,0,-1,1.0,1);
          if ((Game.gembits & 4) == 0) {
            fVar3 = 0.0333;
          }
          else {
            fVar3 = 0.1;
          }
          DrawPanel3DCharacter
                    (0x7b,(float)(dVar20 + 0.07999999821186066),(float)(dVar23 + 0.200000002980232 2)
                     ,(float)dVar19,fVar3,fVar3,fVar3,0,uVar10,0,-1,1.0,1);
          if ((Game.gembits & 1) == 0) {
            fVar3 = 0.0333;
          }
          else {
            fVar3 = 0.1;
          }
          DrawPanel3DCharacter
                    (0x79,(float)(dVar20 - 0.1500000059604645),(float)(dVar23 + 0.0599999986588954 9)
                     ,(float)dVar19,fVar3,fVar3,fVar3,0,uVar10,0,-1,1.0,1);
          if ((Game.gembits & 0x10) == 0) {
            fVar3 = 0.0333;
          }
          else {
            fVar3 = 0.1;
          }
          DrawPanel3DCharacter
                    (0x7d,(float)dVar20,(float)(dVar23 + 0.05999999865889549),(float)dVar19,fVar3,
                     fVar3,fVar3,0,uVar10,0,-1,1.0,1);
          if ((Game.gembits & 8) == 0) {
            fVar3 = 0.0333;
          }
          else {
            fVar3 = 0.1;
          }
          dVar25 = (double)(float)(dVar20 + 0.1500000059604645);
          DrawPanel3DCharacter
                    (0x7c,(float)(dVar20 + 0.1500000059604645),(float)(dVar23 + 0.0599999986588954 9)
                     ,(float)dVar19,fVar3,fVar3,fVar3,0,uVar10,0,-1,1.0,1);
          sprintf(tbuf,"+%i %c =",(uint)Game.crate_gems + (uint)Game.bonus_gems,(uint)CLetter[119 ]);
          if (Game.language == 'c') {
            AddSpacesIntoText(tbuf,1);
          }
          Text3D(tbuf,(float)dVar20,0.0,(float)dVar19,0.6,0.6,0.6,1,0);
          BigOutOf((float)(dVar20 - 0.1000000014901161),(float)(dVar23 - 0.300000011920929),0.52,
                   (uint)Game.gems,0x2e);
          fVar3 = 0.175;
          dVar23 = dVar23 - 0.2750000059604645;
          object = 0x78;
        }
        else {
          if (2 < uVar8) {
            if (uVar8 == 3) {
              if (Game.language == 5) {
                Text3D(tRELICS[5],PAUSEPANELX,(float)(dVar23 + 0.3499999940395355),PAUSETEXTZ,0.5,
                       0.6,0.6,1,0);
              }
              else {
                Text3D(tRELICS[Game.language],PAUSEPANELX,(float)(dVar23 + 0.3499999940395355),
                       PAUSETEXTZ,0.6,0.6,0.6,1,0);
              }
              dVar25 = 0.1500000059604645;
              dVar21 = (double)(float)(dVar23 + 0.119999997317791);
              dVar24 = (double)(float)(dVar20 - 0.1500000059604645);
              DrawPanel3DObject(3,(float)(dVar20 - 0.1500000059604645),
                                (float)(dVar23 + 0.119999997317791),(float)dVar19,0.08,0.08,0.08,0 ,0
                                ,0,ObjTab[3].obj.scene,ObjTab[3].obj.special,1);
              sprintf(tbuf,"%i",sapphire_relics);
              if (Game.language == 'c') {
                AddSpacesIntoText(tbuf,1);
              }
              dVar22 = (double)(float)(dVar23 - 0.02999999932944775);
              Text3D(tbuf,(float)dVar24,(float)(dVar23 - 0.02999999932944775),(float)dVar19,0.6,0. 6,
                     0.6,1,0);
              DrawPanel3DObject(2,(float)dVar20,(float)dVar21,(float)dVar19,0.08,0.08,0.08,0,0,0,
                                ObjTab[2].obj.scene,ObjTab[2].obj.special,1);
              sprintf(tbuf,"%i",gold_relics);
              if (Game.language == 'c') {
                AddSpacesIntoText(tbuf,1);
              }
              Text3D(tbuf,(float)dVar20,(float)dVar22,(float)dVar19,0.6,0.6,0.6,1,0);
              dVar24 = (double)(float)(dVar20 + dVar25);
              DrawPanel3DObject(1,(float)(dVar20 + dVar25),(float)dVar21,(float)dVar19,0.08,0.08,
                                0.08,0,0,0,ObjTab[1].obj.scene,ObjTab[1].obj.special,1);
              sprintf(tbuf,"%i",platinum_relics);
              if (Game.language == 'c') {
                AddSpacesIntoText(tbuf,1);
              }
              Text3D(tbuf,(float)dVar24,(float)dVar22,(float)dVar19,0.6,0.6,0.6,1,0);
              BigOutOf((float)(dVar20 - 0.1000000014901161),(float)(dVar23 - 0.300000011920929),0. 52
                       ,(uint)Game.relics,0x1e);
              DrawPanel3DObject(3,(float)dVar24,(float)(dVar23 - 0.324999988079071),(float)dVar19,
                                0.125,0.125,0.125,0,uVar10,0,ObjTab[3].obj.scene,
                                ObjTab[3].obj.special,1);
            }
            else if (uVar8 == 4) {
              Text3D(tTIMES[Game.language],PAUSEPANELX,(float)(dVar23 + 0.3499999940395355),
                     PAUSETEXTZ,0.6,0.6,0.6,1,0);
              DrawTimeTrialTimes(Level,(float)(dVar20 + 0.04500000178813934),
                                 (float)(dVar23 - dVar25),(float)(dVar19 + dVar25));
            }
            goto LAB_800601c8;
          }
          if (uVar8 != 0) {
            if (uVar8 == 1) {
              Text3D(tPOWERS[Game.language],PAUSEPANELX,(float)(dVar23 + 0.3499999940395355),
                     PAUSETEXTZ,0.6,0.6,0.6,1,0);
              if (Game.language == 'c') {
                tbuf[2] = '\0';
                tbuf[1] = ' ';
              }
              else {
                tbuf[1] = '\0';
              }
              if ((ExtraMoves != 0) || ((Game.powerbits & 1) != 0)) {
                tbuf[0] = PData[0].font3d_letter;
                Text3D2(tbuf,(float)(dVar20 - 0.1599999964237213),
                        (float)(dVar23 + 0.05000000074505806),(float)dVar19,1.4,1.4,1.4,1,0);
              }
              if ((ExtraMoves != 0) || ((Game.powerbits & 2) != 0)) {
                tbuf[0] = PData[1].font3d_letter;
                Text3D2(tbuf,(float)dVar20,(float)(dVar23 + 0.05000000074505806),(float)dVar19,1.4 ,
                        1.4,1.4,1,0);
              }
              if ((ExtraMoves != 0) || ((Game.powerbits & 4) != 0)) {
                tbuf[0] = PData[2].font3d_letter;
                Text3D2(tbuf,(float)(dVar20 + 0.1599999964237213),
                        (float)(dVar23 + 0.05000000074505806),(float)dVar19,1.4,1.4,1.4,1,0);
              }
              if ((ExtraMoves != 0) || ((Game.powerbits & 8) != 0)) {
                tbuf[0] = PData[3].font3d_letter;
                Text3D2(tbuf,(float)(dVar20 - 0.1599999964237213),
                        (float)(dVar23 - 0.2000000029802322),(float)dVar19,1.4,1.4,1.4,1,0);
              }
              if ((ExtraMoves != 0) || ((Game.powerbits & 0x10) != 0)) {
                tbuf[0] = PData[4].font3d_letter;
                Text3D2(tbuf,(float)dVar20,(float)(dVar23 - 0.2000000029802322),(float)dVar19,1.4,
                        1.4,1.4,1,0);
              }
              if ((ExtraMoves != 0) || ((Game.powerbits & 0x20) != 0)) {
                tbuf[0] = PData[5].font3d_letter;
                Text3D2(tbuf,(float)(dVar20 + 0.1599999964237213),
                        (float)(dVar23 - 0.2000000029802322),(float)dVar19,1.4,1.4,1.4,1,0);
              }
            }
            goto LAB_800601c8;
          }
          if (Game.language == 5) {
            Text3D(tCRYSTALS[5],PAUSEPANELX,(float)(dVar23 + 0.3499999940395355),PAUSETEXTZ,0.5,0. 6,
                   0.6,1,0);
          }
          else {
            Text3D(tCRYSTALS[Game.language],PAUSEPANELX,(float)(dVar23 + 0.3499999940395355),
                   PAUSETEXTZ,0.6,0.6,0.6,1,0);
          }
          BigOutOf((float)(dVar20 - 0.1000000014901161),(float)(dVar23 - 0.125),0.52,
                   (uint)Game.crystals,0x19);
          fVar3 = 0.2;
          dVar25 = (double)(float)(dVar20 + 0.1500000059604645);
          dVar23 = dVar23 - 0.1749999970197678;
          object = 0x75;
        }
        DrawPanel3DCharacter
                  (object,(float)dVar25,(float)dVar23,(float)dVar19,fVar3,fVar3,fVar3,0,uVar10,0,- 1,
                   1.0,1);
      }
    }
    goto LAB_800601c8;
  }
  if (bVar2) {
    bVar1 = false;
    if (((plr_bonus_wumpas.count != 0) || (bonus_wumpa_delay != '\0')) || (0.0 < bonus_wumpa_wait) )
    {
      bVar1 = true;
    }
    if (((Bonus == 2) && (((plr->obj).dead == '\0' || (bVar1)))) || ((Bonus - 3U < 2 && (bVar1))))  {
      dVar23 = (double)bonus_panel_wait;
      if ((dVar23 <= 0.0) && (dVar23 = 0.0, 0.0 < bonus_wumpa_wait)) {
        dVar23 = (double)(0.5 - bonus_wumpa_wait);
      }
      fVar3 = BONUSPANELSY;
      if (0.0 < dVar23) {
        fVar3 = (float)((double)(-1.2 - BONUSPANELSY) * dVar23);
        fVar3 = BONUSPANELSY + fVar3 + fVar3;
      }
      dVar25 = (double)fVar3;
      sprintf(tbuf,"%i",(int)plr_bonus_wumpas.draw);
      if (Game.language == 'c') {
        AddSpacesIntoText(tbuf,1);
      }
      Text3D(tbuf,BONUSWUMPATXTSX,(float)dVar25,1.0,0.6,0.6,0.6,4,1);
      DrawPanel3DObject(0,BONUSWUMPAOBJSX,(float)dVar25,1.0,0.125,0.125,0.125,0,PANELWUMPAYROT,0,
                        ObjTab[0].obj.scene,ObjTab[0].obj.special,1);
    }
    bVar1 = false;
    if ((bonus_finish_frame < save_bonus_crates_destroyed * 6 + 6) || (0.0 < bonus_crates_wait)) {
      bVar1 = true;
    }
    if (((Bonus == 2) && (((plr->obj).dead == '\0' || (bVar1)))) || ((Bonus - 3U < 2 && (bVar1))))  {
      dVar23 = (double)bonus_panel_wait;
      if ((dVar23 <= 0.0) && (dVar23 = 0.0, 0.0 < bonus_crates_wait)) {
        dVar23 = (double)(0.5 - bonus_crates_wait);
      }
      fVar3 = BONUSPANELSY;
      if (0.0 < dVar23) {
        fVar3 = (float)((double)(-1.2 - BONUSPANELSY) * dVar23);
        fVar3 = BONUSPANELSY + fVar3 + fVar3;
      }
      dVar25 = (double)fVar3;
      sprintf(tbuf,"%i/%i",bonus_crates_destroyed,DESTRUCTIBLEBONUSCRATECOUNT);
      if (Game.language == 'c') {
        AddSpacesIntoText(tbuf,1);
      }
      Text3D(tbuf,BONUSCRATETXTSX,(float)dVar25,1.0,0.6,0.6,0.6,4,2);
      DrawPanel3DObject(-1,BONUSCRATEOBJSX,(float)dVar25,1.0,0.09375,0.09375,0.09375,0,
                        PANELCRATEYROT,0,crate_scene,crate_list[5].obj.special,1);
    }
    bVar1 = false;
    if (((bonus_lives != 0) || (bonus_life_delay != '\0')) || (0.0 < bonus_lives_wait)) {
      bVar1 = true;
    }
    if (((Bonus == 2) && (((plr->obj).dead == '\0' || (bVar1)))) || ((Bonus - 3U < 2 && (bVar1))))  {
      dVar23 = (double)bonus_panel_wait;
      if ((dVar23 <= 0.0) && (dVar23 = 0.0, 0.0 < bonus_lives_wait)) {
        dVar23 = (double)(0.5 - bonus_lives_wait);
      }
      fVar3 = BONUSPANELSY;
      if (0.0 < dVar23) {
        fVar3 = (float)((double)(-1.2 - BONUSPANELSY) * dVar23);
        fVar3 = BONUSPANELSY + fVar3 + fVar3;
      }
      dVar25 = (double)fVar3;
      sprintf(tbuf,"%i",bonus_lives);
      if (Game.language == 'c') {
        AddSpacesIntoText(tbuf,1);
      }
      object = 0x11;
      Text3D(tbuf,BONUSLIVESTXTSX,(float)dVar25,1.0,0.6,0.6,0.6,4,0);
      if (LDATA->character == 1) {
        object = 0x12;
      }
      DrawPanel3DObject(object,BONUSLIVESOBJSX,(float)dVar25,1.0,BONUSLIFESCALE,BONUSLIFESCALE,
                        BONUSLIFESCALE,panel_head_xrot,panel_head_yrot + 0x2000,0,
                        ObjTab[object].obj.scene,ObjTab[object].obj.special,1);
    }
    if ((Bonus == 2) && ((plr->obj).dead == '\0')) {
      DrawGameMessage(tBONUS[Game.language],bonus_frame,0.5);
    }
    if (Demo != 0) {
      DrawGameMessage(tDEMO[Game.language],GameTimer.frame,0.5);
    }
    if (bVar2) {
      if (plr_wumpas.frame != 0) {
        local_70 = (double)CONCAT44(0x43300000,0x1eU - (int)plr_wumpas.frame ^ 0x80000000);
        fVar3 = (float)(local_70 - 4503601774854144.0) * 0.03333334 * 0.4 + PANELSY;
        dVar23 = (double)fVar3;
        DrawPanel3DObject(0,WUMPAOBJSX,fVar3,1.0,0.2,0.2,0.2,0xe800,PANELWUMPAYROT,0,
                          ObjTab[0].obj.scene,ObjTab[0].obj.special,1);
        sprintf(tbuf,"%i",(int)plr_wumpas.draw);
        if (Game.language == 'c') {
          AddSpacesIntoText(tbuf,1);
        }
        Text3D(tbuf,WUMPATXTSX,(float)dVar23,1.0,1.0,1.0,1.0,4,1);
      }
      if (((plr_crates.frame != 0) && (LDATA->hub != -1)) && ((LDATA->flags & 2) == 0)) {
        local_70 = (double)CONCAT44(0x43300000,0x1eU - (int)plr_crates.frame ^ 0x80000000);
        fVar3 = (float)(local_70 - 4503601774854144.0) * 0.03333334 * 0.4 + PANELSY;
        dVar23 = (double)fVar3;
        DrawPanel3DObject(-1,CRATEOBJSX,fVar3,1.0,0.15,0.15,0.15,0xe800,PANELCRATEYROT,0,crate_sce ne
                          ,crate_list[5].obj.special,1);
        sprintf(tbuf,"%i",(int)plr_crates.draw);
        if (Game.language == 'c') {
          AddSpacesIntoText(tbuf,1);
        }
        Text3D(tbuf,CRATETXTSX,(float)dVar23,1.0,1.0,1.0,1.0,4,2);
        sprintf(tbuf,"/%i",DESTRUCTIBLECRATECOUNT);
        if (Game.language == 'c') {
          AddSpacesIntoText(tbuf,1);
        }
        Text3D(tbuf,font3d_xright,font3d_ybottom,1.0,0.6,0.6,0.6,0xc,2);
      }
      if (plr_lives.frame != 0) {
        local_70 = (double)CONCAT44(0x43300000,0x1eU - (int)plr_lives.frame ^ 0x80000000);
        object = 0x11;
        fVar3 = (float)(local_70 - 4503601774854144.0) * 0.03333334 * 0.4 + PANELSY;
        dVar23 = (double)fVar3;
        if (LDATA->character == 1) {
          object = 0x12;
        }
        DrawPanel3DObject(object,LIVESOBJSX,fVar3,1.0,LIFESCALE,LIFESCALE,LIFESCALE,
                          panel_head_xrot - 0x1000,panel_head_yrot + 0x2000,0,
                          ObjTab[object].obj.scene,ObjTab[object].obj.special,1);
        sprintf(tbuf,"%i",(int)plr_lives.draw);
        if (Game.language == 'c') {
          AddSpacesIntoText(tbuf,1);
        }
        Text3D(tbuf,LIVESTXTSX,(float)dVar23,1.0,1.0,1.0,1.0,4,0);
      }
      uVar10 = (ushort)(((GameTimer.frame % 0x78) * 0x10000) / 0x78);
      if (((plr_crystal.count != 0) && (plr_crystal.frame != 0)) && (LDATA->hub != -1)) {
        local_70 = (double)CONCAT44(0x43300000,0x1eU - (int)plr_crystal.frame ^ 0x80000000);
        fVar3 = -0.7 - (float)(local_70 - 4503601774854144.0) * 0.03333334 * 0.6;
        dVar23 = (double)fVar3;
        DrawPanel3DCharacter(0x75,0.0,fVar3,1.0,0.15,0.15,0.15,0,uVar10,0,-1,0.0,1);
      }
      if (((plr_crategem.count != 0) && (plr_crategem.frame != 0)) && (LDATA->hub != -1)) {
        local_70 = (double)CONCAT44(0x43300000,0x1eU - (int)plr_crategem.frame ^ 0x80000000);
        fVar3 = -0.7 - (float)(local_70 - 4503601774854144.0) * 0.03333334 * 0.6;
        dVar23 = (double)fVar3;
        DrawPanel3DCharacter(0x77,-0.2,fVar3,1.0,0.15,0.15,0.15,0,uVar10,0,-1,0.0,1);
      }
      if (((plr_bonusgem.count != 0) && (plr_bonusgem.frame != 0)) && (LDATA->hub != -1)) {
        local_70 = (double)CONCAT44(0x43300000,0x1eU - (int)plr_bonusgem.frame ^ 0x80000000);
        fVar3 = -0.7 - (float)(local_70 - 4503601774854144.0) * 0.03333334 * 0.6;
        dVar23 = (double)fVar3;
        if (plr_bonusgem.item == '\b') {
          object = 0x79;
        }
        else if (plr_bonusgem.item == ' ') {
          object = 0x7a;
        }
        else if (plr_bonusgem.item == '\x10') {
          object = 0x7b;
        }
        else if (plr_bonusgem.item == '@') {
          object = 0x7c;
        }
        else {
          object = 0x78;
          if (plr_bonusgem.item == 0x80) {
            object = 0x7d;
          }
        }
        DrawPanel3DCharacter(object,0.2,fVar3,1.0,0.15,0.15,0.15,0,uVar10,0,-1,0.0,1);
      }
      DrawWorldToPanelWumpa();
      pNVar18 = NewWumpa;
      do {
        if ((pNVar18->active != '\0') && (pNVar18->count != '\0')) {
          uVar10 = 0;
          fVar3 = pNVar18->screen_scale;
          if (pNVar18->bonus == '\0') {
            uVar10 = 0xe800;
          }
          DrawPanel3DObject(0,(pNVar18->screen_pos).x,(pNVar18->screen_pos).y,1.0,fVar3,fVar3,fVar 3,
                            uVar10,PANELWUMPAYROT,0,ObjTab[0].obj.scene,ObjTab[0].obj.special,1);
        }
        pNVar18 = pNVar18 + 1;
      } while ((int)pNVar18 < -0x7fcd397b);
      object = 0x20;
      if (TimeTrial != 0) {
        cVar4 = (plr->obj).finished;
        if (cVar4 == '\0') {
          if ((timetrial_frame - 0x3c < 0xb4) && (timetrial_frame % 0x3c < 0x30)) {
            Text3D(tENTERINGTIMETRIAL[Game.language],0.0,0.0,1.0,0.6,0.6,0.6,1,4);
          }
          uVar8 = TimeTrialTimer.itime;
          if (0x2bf1d < (int)TimeTrialTimer.itime) {
            uVar8 = 0x2bf1d;
          }
          MakeTimeI(uVar8,0,txt);
          if (Game.language == 'c') {
            pcVar9 = "    %c   %s";
          }
          else {
            pcVar9 = "  %c %s";
          }
          sprintf(tbuf,pcVar9,(uint)CLetter[118],txt);
          fVar3 = tt_sx;
          fVar6 = tt_sy;
          if (timetrial_frame < 0x2d) {
            if (timetrial_frame % 0xf < 7) {
              fVar5 = 0.5;
              fVar3 = TTScrPos.x;
              fVar6 = TTScrPos.y;
            }
            else {
              fVar5 = 0.0;
            }
          }
          else if (timetrial_frame < 0x3c) {
            local_70 = (double)CONCAT44(0x43300000,timetrial_frame - 0x2d);
            dVar23 = (double)((float)(local_70 - 4503599627370496.0) / 15.0);
            fVar6 = (TTScrPos.y - tt_sy) * (float)(1.0 - dVar23) + tt_sy;
            fVar3 = (TTScrPos.x - tt_sx) * (float)(1.0 - dVar23) + tt_sx;
            fVar5 = (float)(dVar23 * 0.5 + 0.5);
          }
          else {
            fVar5 = 1.0;
          }
          if (0.0 < fVar5) {
            iVar17 = 0;
            fVar5 = TT_SCALE * fVar5;
            if (0.0 < TimeTrialWait) {
              iVar17 = 3;
            }
            Text3D(tbuf,fVar3,fVar6,1.0,fVar5,fVar5,fVar5,1,iVar17);
          }
        }
        else if (cVar4 == '\x01') {
          object = (finish_frame + 0x3c) - finish_frames;
          if (object < 0) {
            object = 0;
          }
          if (newleveltime_slot < 3) {
            if ((new_lev_flags & 7) == 0) {
              fVar3 = 0.2;
            }
            else {
              fVar3 = 0.35;
            }
            iVar17 = 3;
          }
          else {
            iVar17 = 4;
            fVar3 = 0.1;
          }
          local_70 = (double)CONCAT44(0x43300000,iVar17 - 1U ^ 0x80000000);
          dVar25 = local_70 - 4503601774854144.0;
          local_70 = (double)CONCAT44(0x43300000,newleveltime_slot ^ 0x80000000);
          dVar19 = 0.0;
          dVar20 = (double)(fVar3 - (float)((double)(float)dVar25 * (double)TT_TIMESDY) * 0.5);
          dVar25 = (double)(float)((double)(float)(local_70 - 4503601774854144.0) *
                                   (double)TT_TIMESDY + dVar20);
          if (object < 0x1e) {
            if ((object == 0) || (dVar21 = dVar19, 5 < object % 0xc)) {
              dVar19 = (double)tt_sx;
              dVar25 = (double)tt_sy;
              dVar21 = (double)TT_SCALE;
            }
          }
          else {
            dVar21 = 0.8999999761581421;
            if (object < 0x3c) {
              local_70 = (double)CONCAT44(0x43300000,object - 0x1eU ^ 0x80000000);
              dVar23 = (double)((float)(local_70 - 4503601774854144.0) / 30.0);
              dVar19 = (double)(float)((double)(float)(0.0 - (double)tt_sx) * dVar23 + (double)tt_ sx
                                      );
              dVar25 = (double)(float)((double)(float)(dVar25 - (double)tt_sy) * dVar23 +
                                      (double)tt_sy);
              dVar21 = (double)(float)((double)(float)(0.8999999761581421 - (double)TT_SCALE) *
                                       dVar23 + (double)TT_SCALE);
            }
          }
          bVar1 = 0x3b < object;
          if (((newleveltime_slot == 3) || (!bVar1)) && (0.0 < dVar21)) {
            MakeTimeI(TimeTrialTimer.itime,0,txt);
            if (Game.language == 'c') {
              if (finish_frame < finish_frames - 0x3c) {
                pcVar9 = "    %c   %s";
LAB_8005ef58:
                sprintf(tbuf,pcVar9,(uint)CLetter[118],txt);
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
                goto LAB_8005ef58;
              }
              if (newleveltime_slot < 3) {
                pcVar9 = "___ %s";
              }
              else {
                pcVar9 = "    %s";
              }
              sprintf(tbuf,pcVar9,txt);
            }
            Text3D(tbuf,(float)dVar19,(float)dVar25,1.0,(float)dVar21,(float)dVar21,(float)dVar21, 1,
                   4);
          }
          object = 0;
          do {
            if (!bVar1) {
              return;
            }
            MakeLevelTimeString((time *)(Level * 0x1c + -0x7fcfdbf0 + object * 8),tbuf);
            if (object == newleveltime_slot) {
              pcVar9 = MakeEditText(tbuf);
              Text3D(pcVar9,(float)dVar19,(float)dVar25,1.0,(float)dVar21,(float)dVar21,
                     (float)dVar21,1,4);
            }
            else {
              Text3D(tbuf,0.0,(float)dVar20,1.0,0.8,0.8,0.8,1,0);
            }
            if (object == newleveltime_slot) {
              if ((new_lev_flags & 4) == 0) {
                if ((new_lev_flags & 2) == 0) {
                  iVar17 = -1;
                  if ((new_lev_flags & 1) != 0) {
                    iVar17 = 3;
                  }
                }
                else {
                  iVar17 = 2;
                }
              }
              else {
                iVar17 = 1;
              }
              if (iVar17 != -1) {
                DrawPanel3DObject(iVar17,TT_RELICX,TT_RELICY,1.0,0.125,0.125,0.125,0,0,0,
                                  ObjTab[iVar17].obj.scene,ObjTab[iVar17].obj.special,1);
              }
            }
            object = object + 1;
            dVar20 = (double)(float)(dVar20 + (double)TT_TIMESDY);
          } while (object < 3);
          if (((tt_flash != 0) && (bVar1)) &&
             ((Cursor.menu != '\x10' || ((newleveltime_slot == 3 && (Cursor.menu_frame < 300))))))  {
            if (newleveltime_slot == 1) {
              ppcVar13 = t2NDBESTTIME;
LAB_8005f1b8:
              strcpy(tbuf,ppcVar13[Game.language]);
            }
            else {
              if (newleveltime_slot < 2) {
                if (newleveltime_slot == 0) {
                  ppcVar13 = tBESTTIME;
                  goto LAB_8005f1b8;
                }
              }
              else if (newleveltime_slot == 2) {
                ppcVar13 = t3RDBESTTIME;
                goto LAB_8005f1b8;
              }
              strcpy(tbuf,tNONEWTIME[Game.language]);
            }
            Text3D(tbuf,0.0,TT_MESSAGEY,1.0,0.8,0.8,0.8,1,3);
          }
        }
      }
      if ((((LDATA->flags & 0x200) == 0) || (VEHICLECONTROL != 1)) || ((plr->obj).finished != '\0' ))
      {
        if ((Level == 0x15) && ((plr->obj).finished == '\0')) {
          DrawPanel3DObject(0x11,-0.75,-0.75,1.0,0.2,0.2,0.2,uVar12,sVar11 + 0xe000,0,
                            ObjTab[17].obj.scene,ObjTab[17].obj.special,1);
          object = GetRumblePlayerHealthPercentage(plr);
          sprintf(tbuf,"%.3i%%",object);
          if (tbuf[0] == '0') {
            if (tbuf[1] == '0') {
              tbuf[0] = ' ';
              tbuf[1] = ' ';
            }
            else {
              tbuf[0] = ' ';
            }
          }
          if (Game.language == 'c') {
            AddSpacesIntoText(tbuf,1);
          }
          Text3D(tbuf,-0.7,-0.75,1.0,0.8,0.8,0.8,4,0);
          RumbleHeadUpDisplay();
          dVar21 = (double)RUMBOSSBARX;
          dVar20 = (double)RUMBOSSBARY;
          dVar19 = (double)RUMBOSSBARZ;
          dVar25 = (double)RUMBOSSBARSCALEX;
          dVar23 = (double)RUMBOSSBARSCALEY;
          object = GetCurrentLevelObjectives();
          iVar17 = GetMaxLevelObjectives();
          BossBar((float)dVar21,(float)dVar20,(float)dVar19,(float)dVar25,(float)dVar23,object,
                  iVar17);
          uVar10 = sVar11 + 0x2000;
          uVar12 = uVar12 + PANELCRUNCHXROT;
          fVar3 = PANELCRUNCHY;
        }
        else if ((Level == 0x16) && ((plr->obj).finished == '\0')) {
          dVar21 = (double)FIREBOSSBARX;
          dVar20 = (double)FIREBOSSBARY;
          dVar19 = (double)FIREBOSSBARZ;
          dVar25 = (double)FIREBOSSBARSCALEX;
          dVar23 = (double)FIREBOSSBARSCALEY;
          object = GetCurrentLevelObjectives();
          iVar17 = GetMaxLevelObjectives();
          BossBar((float)dVar21,(float)dVar20,(float)dVar19,(float)dVar25,(float)dVar23,object,
                  iVar17);
          uVar10 = 0x2000 - sVar11;
          uVar12 = uVar12 + PANELCRUNCHXROT;
          fVar3 = PANELCRUNCHY;
        }
        else {
          if ((Level != 0x19) || ((plr->obj).finished != '\0')) {
            if ((Level == 3) && (cVar4 = (plr->obj).finished, cVar4 == '\0')) {
              if (SmokeyCountDownValue < 1) {
                if (SmokeyCountDownValue == -1) {
                  fVar3 = 2.5;
                  if (Game.language == 4) {
                    fVar3 = 2.25;
                  }
                  Text3D(tOK[Game.language],0.0,0.0,1.0,fVar3,fVar3,fVar3,1,4);
                }
              }
              else {
                tbuf[0] = (char)SmokeyCountDownValue + '0';
                tbuf[1] = cVar4;
                if (Game.language == 'c') {
                  tbuf[1] = ' ';
                  tbuf[2] = cVar4;
                }
                Text3D(tbuf,0.0,0.0,1.0,1.5,1.5,1.5,1,0);
              }
              if (in_finish_range != 0) goto LAB_800601c4;
              object = 0x11;
              if (LDATA->character == 1) {
                object = 0x12;
              }
              DrawPanel3DObject(object,-0.75,-0.75,1.0,0.2,0.2,0.2,panel_head_xrot,
                                panel_head_yrot + 0xe000,0,ObjTab[object].obj.scene,
                                ObjTab[object].obj.special,1);
              Text3D(*(char **)((int)PlaceName +
                               (uint)Game.language * 4 + (SmokeyPosition + -1) * 0x18),-0.68,-0.75 ,
                     1.0,0.8,0.8,0.8,4,-(uint)(SmokeyPosition == 1) & 4);
            }
            else if (Level == 0x1c) {
              if (GameTimer.frame < 0xb4) {
                tbuf[0] = '3' - (char)(GameTimer.frame / 0x3c);
                if (Game.language == 'c') {
                  tbuf[2] = '\0';
                  tbuf[1] = ' ';
                }
                else {
                  tbuf[1] = '\0';
                }
                object = 0;
                dVar23 = 1.5;
              }
              else if (GameTimer.frame < 0xf0) {
                strcpy(tbuf,tOK[Game.language]);
                dVar23 = 2.5;
                if (Game.language == '\x04') {
                  dVar23 = 2.25;
                }
                object = 4;
              }
              else {
                tbuf[0] = '\0';
              }
              if (tbuf[0] != '\0') {
                Text3D(tbuf,0.0,0.0,1.0,(float)dVar23,(float)dVar23,(float)dVar23,1,object);
              }
              if (in_finish_range != 0) goto LAB_800601c4;
              DrawPanel3DObject(0x11,-0.75,-0.75,1.0,0.2,0.2,0.2,panel_head_xrot,
                                panel_head_yrot + 0xe000,0,ObjTab[17].obj.scene,
                                ObjTab[17].obj.special,1);
              Text3D(*(char **)((int)PlaceName +
                               (uint)Game.language * 4 + (PlrTub.place + -1) * 0x18),-0.68,-0.75,1 .0
                     ,0.8,0.8,0.8,4,-(uint)(PlrTub.place == 1) & 4);
              object = PlrTub.laps + 1;
              if (MAXLAPS < PlrTub.laps + 1) {
                object = MAXLAPS;
              }
              sprintf(tbuf,"%i/%i",object);
              if (Game.language == 'c') {
                AddSpacesIntoText(tbuf,1);
              }
              iVar17 = 0;
              if (0 < object) {
                iVar17 = 4;
              }
              Text3D(tbuf,0.7,-0.75,1.0,0.8,0.8,0.8,0x10,iVar17);
            }
            else if (Level == 0x1d) {
              if (GameTimer.frame < 0xb4) {
                dVar23 = 1.5;
                tbuf[1] = '\0';
                object = 0;
                tbuf[0] = '3' - (char)(GameTimer.frame / 0x3c);
              }
              else if (GameTimer.frame < 0xf0) {
                strcpy(tbuf,tOK[Game.language]);
                dVar23 = 2.5;
                if (Game.language == '\x04') {
                  dVar23 = 2.25;
                }
                object = 4;
              }
              else {
                tbuf[0] = '\0';
              }
              if (tbuf[0] != '\0') {
                Text3D(tbuf,0.0,0.0,1.0,(float)dVar23,(float)dVar23,(float)dVar23,1,object);
              }
              if (in_finish_range != 0) goto LAB_800601c4;
              if (0 < i_ring) {
                local_70 = (double)CONCAT44(0x43300000,0x1eU - carpet_panelpos ^ 0x80000000);
                dVar23 = (double)(((float)(local_70 - 4503601774854144.0) / 30.0) * 0.4);
                DrawPanel3DObject(0x11,-0.75,(float)(-0.75 - dVar23),1.0,0.2,0.2,0.2,panel_head_xr ot
                                  ,panel_head_yrot + 0xe000,0,ObjTab[17].obj.scene,
                                  ObjTab[17].obj.special,1);
                if (carpet_place == 1) {
                  pcVar9 = PlaceName[1][Game.language + 1];
                }
                else {
                  pcVar9 = PlaceName[Game.language];
                }
                object = 0;
                if (carpet_place != 1) {
                  object = 4;
                }
                Text3D(pcVar9,-0.7,(float)(-0.75 - dVar23),1.0,0.8,0.8,0.8,4,object);
              }
              sprintf(tbuf,"%i/%i",i_ring,RINGCOUNT);
              if (Game.language == 'c') {
                AddSpacesIntoText(tbuf,1);
              }
              object = 0;
              if (0 < i_ring) {
                object = 4;
              }
              Text3D(tbuf,0.7,-0.75,1.0,0.8,0.8,0.8,0x10,object);
              DrawPanel3DObject(0x68,PANELRINGX,PANELRINGY,1.0,PANELRINGSIZE,PANELRINGSIZE,
                                PANELRINGSIZE,0,0,0,ObjTab[104].obj.scene,ObjTab[104].obj.special, 1)
              ;
              if (ObjTab[108].obj.special != (nuspecial_s *)0x0) {
                DrawPanel3DArrow();
              }
            }
            goto LAB_8006012c;
          }
          dVar21 = (double)FIREBOSSBARX;
          dVar20 = (double)FIREBOSSBARY;
          dVar19 = (double)FIREBOSSBARZ;
          dVar25 = (double)FIREBOSSBARSCALEX;
          dVar23 = (double)FIREBOSSBARSCALEY;
          object = GetCurrentLevelObjectives();
          iVar17 = GetMaxLevelObjectives();
          BossBar((float)dVar21,(float)dVar20,(float)dVar19,(float)dVar25,(float)dVar23,object,
                  iVar17);
          uVar10 = 0x2000 - sVar11;
          uVar12 = uVar12 + PANELCRUNCHXROT;
          fVar3 = PANELCRUNCHY - 0.1;
        }
LAB_8005f9b4:
        DrawPanel3DTempCharacter(PANELCRUNCHX,fVar3,PANELCRUNCHZ,PANELCRUNCHSCALE,uVar12,uVar10,0 ,0)
        ;
      }
      else {
        object = 0x11;
        if (LDATA->character == 1) {
          object = 0x12;
        }
        DrawPanel3DObject(object,-0.75,-0.75,1.0,0.2,0.2,0.2,uVar12,sVar11 + 0xe000,0,
                          ObjTab[object].obj.scene,ObjTab[object].obj.special,1);
        object = GetGliderHealthPercentage(plr);
        sprintf(tbuf,"%.3i%%",object);
        if (tbuf[0] == '0') {
          if (tbuf[1] == '0') {
            tbuf[1] = ' ';
          }
          tbuf[0] = ' ';
        }
        if (Game.language == 'c') {
          AddSpacesIntoText(tbuf,1);
        }
        Text3D(tbuf,-0.7,-0.75,1.0,0.8,0.8,0.8,4,0);
        if (Level == 0x18) {
          dVar21 = (double)ATMBOSSBARX;
          dVar20 = (double)ATMBOSSBARY;
          dVar19 = (double)ATMBOSSBARZ;
          dVar25 = (double)ATMBOSSBARSCALEX;
          dVar23 = (double)ATMBOSSBARSCALEY;
          object = GetCurrentLevelObjectives();
          iVar17 = GetMaxLevelObjectives();
          BossBar((float)dVar21,(float)dVar20,(float)dVar19,(float)dVar25,(float)dVar23,object,
                  iVar17);
        }
        else {
          object = GetCurrentLevelObjectives();
          iVar17 = GetMaxLevelObjectives();
          sprintf(tbuf,"%i/%i",iVar17 - object,iVar17);
          if (Game.language == 'c') {
            AddSpacesIntoText(tbuf,1);
          }
          Text3D(tbuf,0.65,-0.75,1.0,0.8,0.8,0.8,0x10,0);
        }
        if (Level == 0x12) {
          uVar8 = (uint)((ulonglong)GameTimer.frame * 0x88888889 >> 0x20);
          fVar3 = PANELSINKX;
          fVar6 = PANELSINKY;
          fVar5 = PANELSINKZ;
          scale = PANELSINKSCALE;
        }
        else {
          if (Level == 0x18) {
            uVar10 = sVar11 + 0x2000;
            uVar12 = uVar12 + PANELCRUNCHXROT;
            fVar3 = PANELCRUNCHY;
            goto LAB_8005f9b4;
          }
          uVar8 = (uint)((ulonglong)GameTimer.frame * 0x88888889 >> 0x20);
          fVar3 = PANELBLIMPX;
          fVar6 = PANELBLIMPY;
          fVar5 = PANELBLIMPZ;
          scale = PANELBLIMPSCALE;
        }
        DrawPanel3DTempCharacter
                  (fVar3,fVar6,fVar5,scale,panel_head_xrot,
                   panel_head_yrot +
                   (short)(((GameTimer.frame + (uVar8 >> 6) * -0x78) * 0x10000) / 0x78),0,0);
      }
LAB_8006012c:
      if ((((in_finish_range == 0) && (TimeTrial == 0)) && (GATECOUNT != 0)) && (plr_gates != 0))  {
        sprintf(tbuf,"%i/%i");
        if (Game.language == 'c') {
          AddSpacesIntoText(tbuf,1);
        }
        Text3D(tbuf,-0.75,-0.75,1.0,0.8,0.8,0.8,4,4);
      }
    }
  }
LAB_800601c4:
  DrawPanelDebris();
LAB_800601c8:
  NuShaderSetBypassShaders(0);
  object = 0x30;
  pfVar7 = pCam;
  pnVar16 = pNuCam;
  do {
    pnVar15 = pnVar16;
    pfVar14 = pfVar7;
    object = object + -0x18;
    (pnVar15->mtx)._00 = *pfVar14;
    (pnVar15->mtx)._01 = pfVar14[1];
    (pnVar15->mtx)._02 = pfVar14[2];
    (pnVar15->mtx)._03 = pfVar14[3];
    (pnVar15->mtx)._10 = pfVar14[4];
    (pnVar15->mtx)._11 = pfVar14[5];
    camera = pNuCam;
    pnVar16 = (nucamera_s *)&(pnVar15->mtx)._12;
    pfVar7 = pfVar14 + 6;
  } while (object != 0);
  *(float *)pnVar16 = pfVar14[6];
  (pnVar15->mtx)._13 = pfVar14[7];
  (pnVar15->mtx)._20 = pfVar14[8];
  (pnVar15->mtx)._21 = pfVar14[9];
  NuCameraSet(camera);
  GameVP();
  return;
}

s32 DrawPanel3DObject(s32 object,float x,float y,float z,float scalex,float scaley,float scalez,
                     u16 xrot,u16 yrot,u16 zrot,struct nugscn_s *scn,struct nuspecial_s *obj,s32 rot)
{
  struct numtx_s *cammtx;
  s32 i;
  struct numtx_s m;
  struct nuvec_s s;
  
  if (((scn == NULL) || (obj == NULL)) || ((scalex == 0.0f && ((scaley == 0.0f && (scalez == 0.0f)))))) {
    i = 0;
  }
  else {
    s.x = scalex;
    s.y = scaley;
    s.z = scalez;
    NuMtxSetScale(&m,&s);
    if (rot == 0) {
      if (xrot != 0) {
        NuMtxRotateX(&m,xrot);
      }
      if (yrot != 0) {
        NuMtxRotateY(&m,yrot);
      }
      if (zrot != 0) {
        NuMtxRotateZ(&m,zrot);
      }
    }
    else if (rot == 1) {
      if (yrot != 0) {
        NuMtxRotateY(&m,yrot);
      }
      if (xrot != 0) {
        NuMtxRotateX(&m,xrot);
      }
      if (zrot != 0) {
        NuMtxRotateZ(&m,zrot);
      }
    }
    m._30 = x * PANEL3DMULX;
    m._32 = z;
    m._31 = y * PANEL3DMULY;
    cammtx = NuCameraGetMtx();
    NuMtxMul(&m,&m,cammtx);
    SetLevelLights();
    i = NuRndrGScnObj(scn->gobjs[obj->instance->objid],&m);
  }
  return i;
}


//CHECK!
s32 DrawPanel3DCharacter (s32 character,float x,float y,float z,float scalex,float scaley,float scalez,
							u16 xrot,u16 yrot,u16 zrot,s32 action,float anim_time,s32 rot)
{
  s32 i;
  struct numtx_s *cammtx;
  struct nuanimdata_s *animdata; //tmp
  struct CharacterModel *model;
  struct numtx_s m;
  struct nuvec_s s;
  

  if ((((scalex == 0.0f) && (scaley == 0.0f)) && (scalez == 0.0f)) ||
     ((0xbe < (u32)character || (CRemap[character] == -1)))) {
    i = 0;
  }
  else {
    model = CModel + CRemap[character];
    s.x = scalex;
    s.y = scaley;
    s.z = scalez;
    NuMtxSetScale(&m,&s);
    if (rot == 0) {
      if (xrot != 0) {
        NuMtxRotateX(&m,xrot);
      }
      if (yrot != 0) {
        NuMtxRotateY(&m,yrot);
      }
      if (zrot != 0) {
        NuMtxRotateZ(&m,zrot);
      }
    }
    else if (rot == 1) {
      if (yrot != 0) {
        NuMtxRotateY(&m,yrot);
      }
      if (xrot != 0) {
        NuMtxRotateX(&m,xrot);
      }
      if (zrot != 0) {
        NuMtxRotateZ(&m,zrot);
      }
    }
    m._30 = x * PANEL3DMULX;
    m._32 = z;
    m._31 = y * PANEL3DMULY;
    cammtx = NuCameraGetMtx();
    NuMtxMul(&m,&m,cammtx);
    if ((action < 0x76) &&
       (animdata = CharacterModel[CRemap[character]].anmdata + action * 4), (animdata != NULL)) {
      NuHGobjEvalAnim(model->hobj,animdata,anim_time,0,NULL,tmtx);
    }
    else {
      NuHGobjEval(model->hobj,0,NULL,tmtx);
    }
    i = NuHGobjRndrMtx(model->hobj,&m,1,NULL,tmtx);
  }
  return i;
}
