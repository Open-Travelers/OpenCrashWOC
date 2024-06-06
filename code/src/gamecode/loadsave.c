/*
  DoOpen
  DoRead
  DoSave
  ChecksumCheck
  MC_Init
  MC_Check
  ChecksumGen
  MemCardRetry
  MemCardDelete
*/

//NGC MATCH
void loadsaveCallEachFrame(void) {
  saveloadASCallEachFrame();
  if (saveload_cardtype != 2) {
    memcard_loadattempted = 0;
  }
  return;
}

//NGC MATCH
void UpdateSaveSlots(struct cursor_s *cur) {
  saveload_cardtype = 2;
  if (memcard_loadmessage_delay != 0) {
    memcard_loadmessage_delay--;
  }
  if (memcard_savemessage_delay != 0) {
    memcard_savemessage_delay--;
  }
  else {
    if (memcard_saveresult_delay != 0) {
      memcard_saveresult_delay--;
    }  
  }
  if (memcard_formatmessage_delay != 0) {
    memcard_formatmessage_delay--;
  }
  if (memcard_loadresult_delay != 0) {
    memcard_loadresult_delay--;
  }
  if (saveload_cardtype != 2) {
    memcard_loadattempted = 0;
  }
  if (memcard_loadattempted == 0) {
    if ((saveload_status == 1) && (saveload_cardtype == 2)) {
      memcard_gamesavailable = XbEnumerateSaveGames();
      GetMenuInfo(cur);
      memcard_loadcompleted = 0;
      memcard_loadattempted = 1;
      memcard_loadmessage_delay = 0;
    }
  }
  else if ((memcard_loadcompleted == 0) && (saveload_status == 1)) {
    if (saveload_error != 0) {
      memcard_gamesavailable = memcard_loadcompleted;
    }
    memcard_loadcompleted = saveload_status;
  }
  if (memcard_saveneeded != 0) {
    if ((saveload_status == 1) && (saveload_cardtype == 2)) {
        saveloadASSave(tTCR_SAVEBANK[Game.language],SaveSlot,0x1050);
        memcard_savestarted = 1;
        memcard_saveneeded = 0;
        memcard_savemessage_delay = 0;
      }
      
  }
  else if ((memcard_savestarted != 0) && (saveload_status == 1)) {
      memcard_savestarted = memcard_saveneeded;
  }
  if (memcard_deleteneeded != 0) {
      if ((saveload_status == 1) && (saveload_cardtype == 2)) {
        saveloadASDelete(tTCR_SAVEBANK[Game.language],0);
        memcard_deletestarted = 1;
        memcard_deleteneeded = 0;
        memcard_savemessage_delay = 0;
      }
  }
  else if ((memcard_deletestarted != 0) && (saveload_status == 1)) {
      memcard_deletestarted = memcard_deleteneeded;
  }   
  if (memcard_formatme != 0) {
    saveloadASFormat();
    memcard_formatme = 0;
    memcard_formatting = 1;
    memcard_formatmessage_delay = 0x3c;
  }
  else {
    if ((memcard_formatting != 0) && (saveload_status == 1)) {
      memcard_formatting = memcard_formatme;
      memcard_loadattempted = memcard_formatme;
    }
  }
  return;
}

//NGC MATCH
void DrawGameSlot(struct game_s *game,float x,float y,s32 col,float size) {
  if (game->empty == NULL) {
    Text3D(game->name,x,(y + 0.1500000059604645f),1.0f,size,size,size,1,col);
    CalculateGamePercentage(game);
    sprintf(tbuf,"%i%%",(u32)game->percent);
    if (Game.language == 0x63) {
      AddSpacesIntoText(tbuf,1);
    }
    Text3D(tbuf,(x - 0.1500000059604645f),y,1.0f,0.6f,0.6f,0.6f,1,col);
    sprintf(tbuf,"%2i ",(u32)game->crystals);
    if (Game.language == 0x63) {
      AddSpacesIntoText(tbuf,1);
    }
    Text3D(tbuf,(x + 0.1500000059604645f),y,1.0f,0.6f,0.6f,0.6f,1,col);
    sprintf(tbuf,"%2i ",(u32)game->gems);
    if (Game.language == 0x63) {
      AddSpacesIntoText(tbuf,1);
    }
    Text3D(tbuf,(x - 0.125f),(y - 0.1500000059604645f),1.0f,0.6f,0.6f,0.6f,1,col);
    sprintf(tbuf,"%i%c",(u32)game->relics,(s32)ObjTab[3].font3d_letter);
    if (Game.language == 0x63) {
      AddSpacesIntoText(tbuf,1);
    }
    Text3D(tbuf,(x + 0.125f),y - 0.1500000059604645f,1.0f,0.6f,0.6f,0.6f,1,col);
    nurndr_forced_mtl = numtl_white;
    sprintf(tbuf,"  %c",(u32)CLetter[117]);
    if (Game.language == 0x63) {
      AddSpacesIntoText(tbuf,1);
    }
    Text3D(tbuf,(x + 0.1500000059604645f),y,1.0f,0.6f,0.6f,0.6f,1,col);
    sprintf(tbuf,"  %c",(u32)CLetter[119]);
    if (Game.language == 0x63) {
      AddSpacesIntoText(tbuf,1);
    }
    Text3D(tbuf,x - 0.125f,y - 0.1500000059604645f,1.0f,0.6f,0.6f,0.6f,1,col);
    nurndr_forced_mtl = NULL;
    sprintf(tbuf,"%02d:%02d %02d.%02d.%02d",(u32)game->hours,(u32)game->mins,(u32)game->month,(u32)game->day,(u32)game->year);
    if (Game.language == 0x63) {
      AddSpacesIntoText(tbuf,1);
    }
    Text3D(tbuf,x,(y - 0.300000011920929f),1.0f,0.4f,0.4f,0.4f,1,col);
  }
  else {
    Text3D(tEMPTY[Game.language],x,(y - 0.01999999955296516f),1.0f,size,size,size,1,col);
  }
  DrawPanel3DObject(0x81,x,(y - 0.05000000074505806f),1.01f,SLOTPANELSX,0.125f,
                    SLOTPANELSZ,0xc000,0,0,ObjTab[129].obj.scene,ObjTab[129].obj.special,0);
  return;
}

void DrawGameSlots(Cursor *cursor)

{
  float fVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  double dVar5;
  double dVar6;
  
  iVar3 = 3;
  if (GlobalTimer.frame % 0xc < 6) {
    iVar3 = 4;
  }
  iVar4 = -1;
  dVar5 = (double)(SLOTPANELDY * 0.5);
  if (cursor->y < 2) {
    iVar4 = cursor->x * 2 + (int)cursor->y;
  }
  dVar6 = -(double)(SLOTPANELDX * 0.5);
  iVar2 = 0;
  if ((iVar4 != 0) || (iVar2 = iVar3, iVar4 != 0)) {
    fVar1 = 0.6;
  }
  else {
    fVar1 = menu_pulsate * 0.6;
  }
  DrawGameSlot(SaveSlot,(float)dVar6,SLOTPANELDY * 0.5,iVar2,fVar1);
  iVar2 = 0;
  if ((iVar4 != 1) || (iVar2 = iVar3, iVar4 != 1)) {
    fVar1 = 0.6;
  }
  else {
    fVar1 = menu_pulsate * 0.6;
  }
  DrawGameSlot(SaveSlot + 1,(float)dVar6,(float)(dVar5 - (double)SLOTPANELDY),iVar2,fVar1);
  iVar2 = 0;
  if ((iVar4 != 2) || (iVar2 = iVar3, iVar4 != 2)) {
    fVar1 = 0.6;
  }
  else {
    fVar1 = menu_pulsate * 0.6;
  }
  DrawGameSlot(SaveSlot + 2,(float)(dVar6 + (double)SLOTPANELDX),(float)dVar5,iVar2,fVar1);
  iVar2 = 0;
  if ((iVar4 != 3) || (iVar2 = iVar3, iVar4 != 3)) {
    fVar1 = 0.6;
  }
  else {
    fVar1 = menu_pulsate * 0.6;
  }
  DrawGameSlot(SaveSlot + 3,(float)(dVar6 + (double)SLOTPANELDX),
               (float)(dVar5 - (double)SLOTPANELDY),iVar2,fVar1);
  return;
}


void InitLoadSaveDeleteScreen(Cursor *cur,int menu)

{
  char cVar1;
  int iVar2;
  int iVar3;
  Game_s *sSlot;
  _ULARGE_INTEGER dVar4;
  _SYSTEMTIME temp_time;
  _FILETIME file_time;
  
  iVar3 = 0;
  memset(&temp_time,0,0x10);
  iVar2 = -1;
  dVar4.QuadPart = 0.0;
  sSlot = SaveSlot;
  do {
    if (sSlot->empty == '\0') {
      temp_time.wYear = sSlot->year + 2000;
      temp_time.wHour = (ushort)sSlot->hours;
      temp_time.wMonth = (ushort)sSlot->month;
      temp_time.wDay = (ushort)sSlot->day;
      temp_time.wMinute = (ushort)sSlot->mins;
      GetLastError();
      if (dVar4.QuadPart < (double)CONCAT44(file_time.dwLowDateTime,file_time.dwHighDateTime)) {
        iVar2 = iVar3;
        dVar4.QuadPart = (double)CONCAT44(file_time.dwLowDateTime,file_time.dwHighDateTime);
      }
    }
    iVar3 = iVar3 + 1;
    sSlot = sSlot + 1;
  } while (iVar3 < 4);
  if (iVar2 < 0) {
    cur->remember[menu].x = '\0';
    cur->remember[menu].y = '\0';
  }
  else {
    cVar1 = (char)(iVar2 / 2);
    cur->remember[menu].x = cVar1;
    cur->remember[menu].y = (char)iVar2 + cVar1 * -2;
  }
  return;
}

//NGC MATCH
s32 XbEnumerateSaveGames(void) {
  s32 i;
  s32 ret;
  
  ret = 0;
  DoRead(1);
  if (saveload_error != 0) {
    for(i = 0; i < 4; i++) {
      SaveSlot[i].empty = 1;
    }
    saveload_status = 1;
    saveload_error = 0;
  }
  for(i = 0; i < 4; i++) {
    if (SaveSlot[i].empty == 0) {
      ret++;
    }
  }
  return ret;
}

//NGC MATCH
void InvalidateSaveSlots(void) {
  s32 i;

  for(i = 0; i < 4; i++) {
    SaveSlot[i].empty = 1;
  }
  return;
}