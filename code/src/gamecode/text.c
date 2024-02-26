char* tDEATHTORNADOSPIN[6];
char* tDEATHTORNADOSPINTEXT[6][7]; 
char* tDOUBLEJUMP[6];
char* tDOUBLEJUMPTEXT[6][7];
char* tFRUITBAZOOKA[6];
char* tFRUITBAZOOKATEXT[6][7]; 
char* tSNEAKSHOES[6];
char* tSNEAKSHOESTEXT[6][7]; 
char* tSPEEDSHOES[6];
char* tSPEEDSHOESTEXT[6][7];
char* tSUPERBELLYFLOP[6];
char* tSUPERBELLYFLOPTEXT[6][7]; 

//NGC MATCH
s32 JStrLen(char *txt) {
  s32 count;
  
  count = 0;
  while ((*txt != 0 && (txt[1] != 0))) {
    if ((txt[2] == 'B') && (((txt[3] == 'D' && (CombinationCharacterBD(*txt,txt[1]) != 0)) ||
        ((txt[3] == 'C' && (CombinationCharacterBC(*txt,txt[1]) != 0)))))) {
      txt = txt + 4;
    }
    else {
      txt = txt + 2;
    }
    count++;
  }
  return count;
}

//NGC MATCH
void AddSpacesIntoText(char *txt, u32 bits) {
  static char buf_159[256];
  s32 j;
  s32 i;
  
  if (Game.language == 0x63) {
    j = 0;
    for(i = 0; txt[i] != 0; i++) {
        buf_159[j] = txt[i];
        j++;
        if ((((bits & 1) != 0) || ((((bits & 2) != 0 && (0x2f < txt[i])) && (txt[i] < 0x3a)))) ||
           (((bits & 4) != 0 && (txt[i] == 0x2d)))) {
          buf_159[j] = 0x20;
          j++;
        }
    }
    buf_159[j] = 0;
    strcpy(txt,buf_159);
  }
  return;
}

char* tCORRUPTDATA[6][2];
char* tCORRUPTED[6][2];
char* tDAMAGED[6][2];
char* tINSUFFICIENTSPACE[6][3];
char* tNOCARD[6];
char* tNOTCOMPAT[6][2];
char* tOTHERMARKET[6][4];
char* tWRONGDEV[6][2];

//NGC MATCH
char* GetStringIdx(s32 errcode, s32 arg1) {

    switch (errcode) {
    case 1:
        if (arg1 > 0) {
            return 0;
        }
        return tNOCARD[arg1 + Game.language];
    case 2:
        if (arg1 <= 1) {
            return tCORRUPTED[Game.language][arg1];
        }
        return 0;
    case 3:
        if (arg1 <= 3) {
            return tOTHERMARKET[Game.language][arg1];
        }
        return 0;
    case 4:
        if (arg1 <= 1) {
            return tDAMAGED[Game.language][arg1];
        }
        return 0;
    case 5:
        if (arg1 <= 1) {
            return tWRONGDEV[Game.language][arg1];
        }
        return 0;
    case 6:
        if (arg1 <= 2) {
            return tINSUFFICIENTSPACE[Game.language][arg1];
        }
        return 0;
    case 7:
        if (arg1 <= 1) {
            return tNOTCOMPAT[Game.language][arg1];
        } 
        return 0;
    case 8:
        if (arg1 <= 1) {
            return tCORRUPTDATA[Game.language][arg1];
        }
        return 0;
    default:
        return 0;
    }
}

//NGC MATCH
void NewLanguage(s32 l) {
  PData[0].name = tSNEAKSHOES[(u8)l];
  PData[0].description = (s32 *)tSNEAKSHOESTEXT[(u8)l];
  PData[1].name = tDOUBLEJUMP[(u8)l];
  PData[1].description = (s32 *)tDOUBLEJUMPTEXT[(u8)l];
  PData[2].name = tDEATHTORNADOSPIN[(u8)l];
  PData[2].description = (s32 *)tDEATHTORNADOSPINTEXT[(u8)l];
  PData[3].name = tFRUITBAZOOKA[(u8)l];
  PData[3].description = (s32 *)tFRUITBAZOOKATEXT[(u8)l];
  PData[4].name = tSPEEDSHOES[(u8)l];
  PData[4].description = (s32 *)tSPEEDSHOESTEXT[(u8)l];
  PData[5].name = tSUPERBELLYFLOP[(u8)l];
  PData[5].description = (s32 *)tSUPERBELLYFLOPTEXT[(u8)l];
  Game.language = l;
  DefaultTimeTrialNames(0);
  return;
}

//CHECK
char* GetDiskErrString(s32 errcode, s32 index) {
    switch (errcode) {
    case 0:
        if (index > 2) {
            return 0;
        }
        return tNOCARD[(u32)Game.language];
    case 1:
        if (index <= 1) {
            return tOTHERMARKET[(u32)Game.language][index];
        }
        return 0;
    case 2:
        if (index <= 2) {
            return tCORRUPTED[(u32)Game.language][index];
        }
        return 0;
    case 3:
        if (index < 4) {
            return tOTHERMARKET[(u32)Game.language][index];
        }
        return 0;
    case 4:
        if (index > 1) {
          return tDAMAGED[(u32)Game.language][index];
        }
    return 0;
    case 5:
        if (index > 1) {
            return tWRONGDEV[(u32)Game.language][index];
        }
    return 0;
    case 6:
        if (index > 2) {
            return tINSUFFICIENTSPACE[(u32)Game.language][index];
        }
        return 0;
    case 7:
        if (index <= 2) {
            return tNOTCOMPAT[(u32)Game.language][index];
        }
        return 0;
    case 8:
        if (1 < index) {
            return tCORRUPTDATA[(u32)Game.language][index];
        }
        return 0;
    default:
        return 0;
    }
}


