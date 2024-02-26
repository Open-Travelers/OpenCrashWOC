extern char* LevelFileName;
extern void* Chase[3]; //struct chase_s
s32 POINTCOUNT;
s32 AMBIENTCOUNT;
s32 LIGHTCOUNT;
s32 DIRECTCOUNT;

struct glowlight_s
{
    struct nucolour3_s HighColour;
    struct nucolour3_s LowColour;
    struct nucolour3_s CurColour;
    struct nucolour3_s ColDiff;
    u16 Step;
    u16 CurAngle;
};

// Size: 0x48
struct lights_s
{
    s32 type;
    struct nuvec_s pos;
    struct nuvec_s radius_pos;
    f32 radius;
    u8 r;
    u8 g;
    u8 b;
    u8 glow;
    struct nucolour3_s colour;
    struct nuvec_s direction;
    u8 globalflag;
    u8 blendtype;
    u8 brightness;
    struct glowlight_s* glowlight;
    u8 pad2;
};

struct lights_s Lights[320];

//87% NGC
void LoadLights(void) {
  s32 fsize;
  s32 handle;
  s32 i;
  float fVar1;
    
  POINTCOUNT = 0;
  LIGHTCOUNT = 0;
  AMBIENTCOUNT = 0;
  DIRECTCOUNT = 0;
  for (fsize = 0; fsize < 0x100; fsize++) {
    Lights[fsize].type = -1;
  }
  sprintf(tbuf,"%s.lgt",LevelFileName);
  if (NuFileExists(tbuf) != 0) {
    fsize = NuFileLoadBuffer(tbuf,Chase + 1,0x7fffffff);
    handle = NuMemFileOpen(Chase + 1,fsize,NUFILE_READ);
    if (handle != 0) {
      LIGHTCOUNT = NuFileReadInt(handle);
      AMBIENTCOUNT = NuFileReadInt(handle);
      DIRECTCOUNT = NuFileReadInt(handle);
      POINTCOUNT = NuFileReadInt(handle);
      if (0x100 < LIGHTCOUNT) {
        LIGHTCOUNT = 0x100;
      }
        for (i = 0; i < LIGHTCOUNT; i++) {
          Lights[i].type = NuFileReadInt(handle);
          fVar1 = NuFileReadFloat(handle);
          (Lights[i].pos.x) = fVar1;
          fVar1 = NuFileReadFloat(handle);
          (Lights[i].pos.y) = fVar1;
          fVar1 = NuFileReadFloat(handle);
          (Lights[i].pos.z) = fVar1;
          fVar1 = NuFileReadFloat(handle);
          Lights[i].radius_pos.x  = fVar1;
          fVar1 = NuFileReadFloat(handle);
          Lights[i].radius_pos.y  = fVar1;
          fVar1 = NuFileReadFloat(handle);
          Lights[i].radius_pos.z  = fVar1;
          fVar1 = NuFileReadFloat(handle);
          Lights[i].radius = fVar1;
          Lights[i].r = NuFileReadChar(handle);
          Lights[i].g = NuFileReadChar(handle);
          Lights[i].b = NuFileReadChar(handle);
          fVar1 = NuFileReadFloat(handle);
          Lights[i].colour.r = fVar1;
          fVar1 = NuFileReadFloat(handle);
          Lights[i].colour.g = fVar1;
          fVar1 = NuFileReadFloat(handle);
          Lights[i].colour.b = fVar1; //
          if (Lights[i].type - 1U < 2) {
            fVar1 = NuFileReadFloat(handle);
            Lights[i].direction.x = fVar1;
            fVar1 = NuFileReadFloat(handle);
            Lights[i].direction.y = fVar1;
            fVar1 = NuFileReadFloat(handle);
            Lights[i].direction.z = fVar1;
          }
          Lights[i].globalflag = (u8)NuFileReadInt(handle);
          Lights[i].brightness = (u8)NuFileReadInt(handle);
        }
      NuFileClose(handle);
    }
  }
  return;
}

//NGC MATCH
void UpdateGlobals(struct Nearest_Light_s *nl) {
  s32 i;
  s32 found_amb;
  s32 found_dir;

  found_amb = 0;
  found_dir = 0;
  nl->glbambindex = -1;
  (nl->glbdirectional).Index = -1;
  for(i = 0; (i < LIGHTCOUNT) && (!found_amb || !found_dir); i++) {
        if (((Lights[i].type) == 0) && (Lights[i].globalflag == 4)) {
            nl->glbambindex = i;
            found_amb = 1;
        }
        if ( ((Lights[i].type == 1) || (Lights[i].type == 2)) && (Lights[i].globalflag == 4)) {
            (nl->glbdirectional).Index = i;
            found_dir = 1;
        }
  }
  return;
}

//NGC MATCH
void ResetLights(struct Nearest_Light_s *nl) {
  nl->pDir1st = &nl->dir1;
  nl->pDir2nd = &nl->dir2;
  nl->pDir3rd = &nl->dir3;
  nl->ambientdist = 8000.0f;
  (nl->dir1).Distance = 8000.0f;
  (nl->dir2).Distance = 8000.0f;
  (nl->dir3).Distance = 8000.0f;
  nl->AmbIndex = -1;
  (nl->AmbCol).x = 0.0f;
  (nl->AmbCol).y = 0.0f;
  (nl->AmbCol).z = 0.0f;
  (nl->dir1).Index = -1;
  (nl->dir2).Index = -1;
  (nl->dir3).Index = -1;
  //(nl->dir1).Direction.x = 0.0f;
  (nl->pDir1st->Direction).x = 0.0f;
  (nl->pDir1st->Direction).y = 0.0f;
  (nl->pDir1st->Direction).z = 0.0f;
  (nl->pDir3rd->Direction).x = 0.0f;
  (nl->pDir2nd->Direction).x = 0.0f;
  (nl->pDir2nd->Direction).y = 0.0f;
  (nl->pDir2nd->Direction).z = 0.0f;
  (nl->pDir3rd->Direction).x = 0.0f;
  (nl->pDir3rd->Direction).x = 0.0f;
  (nl->pDir3rd->Direction).y = 0.0f;
  (nl->pDir3rd->Direction).z = 0.0f;
  (nl->pDir3rd->Direction).x = 0.0f;
  nl->negativeindex = -1;
  nl->negativedist = 8000.0f;
  UpdateGlobals(nl);
  return;
}

//NGC MATCH
void ScaleColour(struct nucolour3_s *colour,u8 r,u8 g,u8 b,u8 power) {
  if ((power == 6)) {
    colour->r = ((s32)r) * sf2;
    colour->g = ((s32)g) * sf2;
    colour->b = ((s32)b) * sf2;
  } else if (power == 7) {
    colour->r = ((s32)r) * sf;
    colour->g = ((s32)g) * sf;
    colour->b = ((s32)b) * sf;
    }
  return;
}


//96%
void SortLights(struct Nearest_Light_s *nearLgt) {
  struct pdir_s *tptr;
  
  tptr = nearLgt->pDir1st;
  if (nearLgt->pDir2nd->Distance > tptr->Distance) {
    nearLgt->pDir1st = nearLgt->pDir2nd;
    nearLgt->pDir2nd = tptr;
  }
  tptr = nearLgt->pDir2nd;
  if (nearLgt->pDir3rd->Distance > tptr->Distance) {
    nearLgt->pDir2nd = nearLgt->pDir3rd;
    nearLgt->pDir3rd = tptr;
  }
  tptr = nearLgt->pDir2nd;
  if (tptr->Distance > nearLgt->pDir1st->Distance) {
      nearLgt->pDir2nd = nearLgt->pDir1st;
      nearLgt->pDir1st = tptr;
  }
  return;
}


int FindNearestLights(nuvec_s *vec,Nearest_Light_s *nearest_light,int SearchMode)

{


}



void CalculateSingleLightProportion(nuvec_s *vec,pdir_s *light,int global)

{


}



float FindLightProportion(nuvec_s *vec,Nearest_Light_s *nearest_light)

{


}


void GetLights(nuvec_s *pos,Nearest_Light_s *nearest_lights,int SearchMode)

{
  FindNearestLights(pos,nearest_lights,SearchMode);
  FindLightProportion(pos,nearest_lights);
  return;
}


void RotateDirectionalLight(nuvec_s *v,int xrot,int yrot)

{
  NuVecRotateX(v,&v001,xrot);
  NuVecRotateY(v,v,yrot);
  return;
}

void SetLights(nucolour3_s *vCOL0,nuvec_s *vDIR0,nucolour3_s *vCOL1,nuvec_s *vDIR1, nucolour3_s *vCOL2,nuvec_s *vDIR2,nuvec_s *vAMB)

{
  NuLightSetDirectionalLights(vDIR0,vCOL0,vDIR1,vCOL1,vDIR2,vCOL2);
  NuLightSetAmbientLight(vAMB);
  return;
}

//98% NGC
void SetLevelLights(void) {
  if ((((GameMode == 1) && (gamecut != 0)) && (gamecut != 6)) && ((gamecut != 0xb && (gamecut != 0x10)))) {
    RotateDirectionalLight(lcutdir,((GameTimer.frame % 0x78) * 0x10000) / 0x78,((GameTimer.frame % 0x78) * 0x10000) / 0x78);
    RotateDirectionalLight(lcutdir + 1,((GameTimer.frame % 0x78) * 0x10000) / 0x78,((GameTimer.frame % 0x78) * 0x10000) / 0x78);
    RotateDirectionalLight(lcutdir + 2,((GameTimer.frame % 0x78) * 0x10000) / 0x78,((GameTimer.frame % 0x78) * 0x10000) / 0x78);
    SetLights(lcutdircol,lcutdir,lcutdircol + 1,lcutdir + 1,lcutdircol + 2,lcutdir + 2,&lcutambcol );
  }
  else if (cutmovie == 0) {
    RotateDirectionalLight(titledir,((GameTimer.frame % 0xf0) * 0x10000) / 0xf0,
               ((GameTimer.frame % 0x186) * 0x10000) / 0x186 );
    RotateDirectionalLight(titledir + 1,((GameTimer.frame % 0x14a) * 0x10000) / 0x14a,
               ((GameTimer.frame % 0x1c2) * 0x10000) / 0x1c2);
    RotateDirectionalLight(titledir + 2,((GameTimer.frame % 0x78) * 0x10000) / 0x78,
               ((GameTimer.frame % 0x96) * 0x10000) / 0x96);
    SetLights(&titlergb,titledir,&titlergb,titledir + 1,&titlergb2,titledir + 2,&titlergb);
  }
  else if ((Level == 0x2b) && (cutmovie == -1)) {
    intensity = NuFabs(NuTrigTable[creditsang[0] & 0xffff]);
    NuVecRotateY(creditsdir,&ZVec,(s32)(creditsang[0] / 40.0f));
    creditsang[0] = creditsang[0] + 0x100;
    creditsdircol[0].r = intensity;
    intensity = NuFabs(NuTrigTable[creditsang[1] & 0xffff]);
    NuVecRotateY(creditsdir + 1,&ZVec,(s32)(-creditsang[1] / 40.0f));
    creditsang[1] = creditsang[1] + 0x100;
    creditsdircol[1].r = intensity;
    intensity = NuFabs(NuTrigTable[creditsang[2] & 0xffff]);
    NuVecRotateX(creditsdir + 2,&ZVec,(s32)(creditsang[2] / 40.0f));
    creditsang[2] = creditsang[2] + 0x100;
    creditsdircol[2].r = intensity;
    NuLightSetDirectionalLights(creditsdir,creditsdircol,creditsdir + 1,creditsdircol + 1,creditsdir + 2,
               creditsdircol + 2);
    NuLightSetAmbientLight(&creditsrgb);
  }
  else {
    SetLights(lcol,ldir,lcol + 1,ldir + 1,lcol + 2,ldir + 2,&acol);
  }
  return;
}

//NGC MATCH
void SetCreatureLights(struct creature_s *c) {
  struct nuvec_s ambcol;
  struct nuvec_s dir[3];
  struct nucolour3_s col[3];
  float f;
  
  if ((USELIGHTS != 0) && (LIGHTCREATURES != 0)) {
    ambcol = (c->lights).AmbCol;
    col[0] = ((c->lights).pDir1st->Colour);
    dir[0] = ((c->lights).pDir1st->Direction);
    col[1] = ((c->lights).pDir2nd->Colour);
    dir[1] = ((c->lights).pDir2nd->Direction);
    col[2] = ((c->lights).pDir3rd->Colour);
    dir[2] = ((c->lights).pDir3rd->Direction);
  }
  else {
    if ((c->obj).dead != 0x11) {
      return;
    }
    ambcol = acol;
    col[0] = lcol[0];
    dir[0] = ldir[0];
    col[1] = lcol[1];
    dir[1] = ldir[1];
    col[2] = lcol[2];
    dir[2] = ldir[2];
  }
  if ((c->obj).dead == 0x11) {
    if ( (c->obj).die_time < (c->obj).die_duration) {
      f = 1.0f - (c->obj).die_time / (c->obj).die_duration;
    }
    else {
      f = 0.0f;
    }
  }
  else {
    f = 1.0f;
  }
  if (f != 1.0f) {
    ambcol.x = ambcol.x * f;
    ambcol.y = ambcol.y * f;
    ambcol.z = ambcol.z * f;
    col[0].r = col[0].r * f;
    col[0].g = col[0].g * f;
    col[0].b = col[0].b * f;
    col[1].r = col[1].r * f;
    col[1].g = col[1].g * f;
    col[1].b = col[1].b * f;
    col[2].r = col[2].r * f;
    col[2].g = col[2].g * f;
    col[2].b = col[2].b * f;
  }
  SetLights(&col[0],dir,&col[1],&dir[1],&col[2],&dir[2],&ambcol);
  return;
}

void SetNearestLights(Nearest_Light_s *l)

{
  if ((USELIGHTS != 0) && (LIGHTCREATURES != 0)) {
    SetLights(&l->pDir1st->Colour,&l->pDir1st->Direction,&l->pDir2nd->Colour,&l->pDir2nd->Directio n,
              &l->pDir3rd->Colour,&l->pDir3rd->Direction,&l->AmbCol);
  }
  return;
}

//NGC MATCH
struct nuvec_s * GetLightPosition(s32 index) {
  return &Lights[index].pos;
}
