extern char* LevelFileName;
extern void* Chase;
extern s32 POINTCOUNT;
extern s32 AMBIENTCOUNT;
extern s32 LIGHTCOUNT;
extern s32 DIRECTCOUNT;

void LoadLights(void) 		//WIP
{
    s32 temp_r3;
    s32 var_ctr;
    s32 var_r0;
    s32 var_r26;
    s32 var_r27;
    s32 var_r28;
    s32 var_r29;
    s32* var_r30;
    void* lgt;
    s32 file;

    var_ctr = 0x100;
    POINTCOUNT = 0;
    lgt = &Lights;
    LIGHTCOUNT = 0;
    AMBIENTCOUNT = 0;
    DIRECTCOUNT = 0;
    do {
        //*lgt = -1;
        //lgt += 0x48;
        var_ctr -= 1;
    } while (var_ctr != 0);
    sprintf((char*)tbuf, "%s.lgt", &LevelFileName);
    file = NuFileLoadBuffer(tbuf - 0x58A4, &Chase, 0x7FFFFFFF);
    if (NuFileExist(tbuf - 0x58A4) != 0) {
        temp_r3 = NuMemFileOpen(&Chase, &file, 0);
        if (temp_r3 != 0) {
            LIGHTCOUNT = NuFileReadInt(temp_r3);
            AMBIENTCOUNT = NuFileReadInt(temp_r3);
            DIRECTCOUNT = NuFileReadInt(temp_r3);
            var_r0 = LIGHTCOUNT;
            POINTCOUNT = NuFileReadInt(temp_r3);
            if (var_r0 > 0x100) {
                LIGHTCOUNT = 0x100;
                var_r0 = 0x100;
            }
            var_r28 = 0;
            if (var_r0 > 0) {
                var_r26 = 0x38;
                var_r27 = 0;
                //var_r30 = Lights + 0x20;
                do {
                    var_r29 = var_r26;
                    /*var_r30->unk-20 = NuFileReadInt(temp_r3);
                    var_r30->unk-1C = NuFileReadFloat(temp_r3);
                    var_r30->unk-18 = NuFileReadFloat(temp_r3);
                    var_r30->unk-14 = NuFileReadFloat(temp_r3);
                    var_r30->unk-10 = NuFileReadFloat(temp_r3);
                    var_r30->unk-C = NuFileReadFloat(temp_r3);
                    var_r30->unk-8 = NuFileReadFloat(temp_r3);
                    var_r30->unk-4 = NuFileReadFloat(temp_r3);
                    var_r30->unk0 = NuFileReadChar(temp_r3);
                    var_r30->unk1 = NuFileReadChar(temp_r3);
                    var_r30->unk2 = NuFileReadChar(temp_r3);
                    var_r30->unk4 = NuFileReadFloat(temp_r3);
                    var_r30->unk8 = NuFileReadFloat(temp_r3);
                    var_r30->unkC = NuFileReadFloat(temp_r3);*/
                    /*if ((u32) (var_r30->unk-20 - 1) <= 1U) {
                        var_r30->unk10 = NuFileReadFloat(temp_r3);
                        var_r30->unk14 = NuFileReadFloat(temp_r3);
                        var_r30->unk18 = NuFileReadFloat(temp_r3);
                    } else {
                        var_r29 = var_r27 + 0x38;
                    }*/
                    var_r28 += 1;
                    var_r26 += 0x48;
                    var_r27 += 0x48;
                    //(var_r29 + &Lights)->unk4 = NuFileReadInt(temp_r3);
                    //var_r30->unk1E = NuFileReadInt(temp_r3);
                    var_r30 += 0x48;
                } while (var_r28 < (s32) LIGHTCOUNT);
            }
            NuFileClose(temp_r3);
        }
    }
}


void UpdateGlobals(Nearest_Light_s *nl)

{


}



void ResetLights(Nearest_Light_s *nl)

{
  nl->pDir3rd = &nl->dir3;
  nl->pDir2nd = &nl->dir2;
  nl->pDir1st = &nl->dir1;
  nl->ambientdist = 8000.0;
  (nl->dir1).Distance = 8000.0;
  (nl->dir2).Distance = 8000.0;
  (nl->dir3).Distance = 8000.0;
  nl->AmbIndex = -1;
  (nl->AmbCol).x = 0.0;
  (nl->AmbCol).y = 0.0;
  (nl->AmbCol).z = 0.0;
  (nl->dir1).Index = -1;
  (nl->dir2).Index = -1;
  (nl->dir3).Index = -1;
  (nl->dir1).Direction.x = 0.0;
  (nl->pDir1st->Direction).y = 0.0;
  (nl->pDir1st->Direction).z = 0.0;
  (nl->pDir3rd->Direction).x = 0.0;
  (nl->pDir2nd->Direction).x = 0.0;
  (nl->pDir2nd->Direction).y = 0.0;
  (nl->pDir2nd->Direction).z = 0.0;
  (nl->pDir3rd->Direction).x = 0.0;
  (nl->pDir3rd->Direction).x = 0.0;
  (nl->pDir3rd->Direction).y = 0.0;
  (nl->pDir3rd->Direction).z = 0.0;
  (nl->pDir3rd->Direction).x = 0.0;
  nl->negativeindex = -1;
  nl->negativedist = 8000.0;
  UpdateGlobals(nl);
  return;
}


void ScaleColour(nucolour3_s* colour, unsigned char r, unsigned char g, unsigned char b, unsigned char power)
{


}


void SortLights(Nearest_Light_s *nearLgt)

{
  pdir_s *tptr;
  
  tptr = nearLgt->pDir1st;
  if (nearLgt->pDir2nd->Distance < tptr->Distance) {
    nearLgt->pDir1st = nearLgt->pDir2nd;
    nearLgt->pDir2nd = tptr;
  }
  tptr = nearLgt->pDir2nd;
  if (nearLgt->pDir3rd->Distance < tptr->Distance) {
    nearLgt->pDir2nd = nearLgt->pDir3rd;
    nearLgt->pDir3rd = tptr;
  }
  tptr = nearLgt->pDir2nd;
  if (nearLgt->pDir1st->Distance <= tptr->Distance) {
    return;
  }
  nearLgt->pDir2nd = nearLgt->pDir1st;
  nearLgt->pDir1st = tptr;
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


void SetLevelLights(void)

{

}


void SetCreatureLights(creature_s *c)

{
  pdir_s *ppVar1;
  pdir_s *ppVar2;
  pdir_s *ppVar3;
  nuvec_s ambcol;
  nuvec_s dir [3];
  nucolour3_s col [3];
  float dur;
  float t;
  
  if ((USELIGHTS == 0) || (LIGHTCREATURES == 0)) {
    if ((c->obj).dead != '\x11') {
      return;
    }
    ambcol.x = acol.x;
    ambcol.y = acol.y;
    ambcol.z = acol.z;
    col[0].r = lcol[0].r;
    col[0].g = lcol[0].g;
    col[0].b = lcol[0].b;
    dir[0].x = ldir[0].x;
    dir[0].y = ldir[0].y;
    dir[0].z = ldir[0].z;
    col[1].r = lcol[1].r;
    col[1].g = lcol[1].g;
    col[1].b = lcol[1].b;
    dir[1].x = ldir[1].x;
    dir[1].z = ldir[1].z;
    dir[1].y = ldir[1].y;
    col[2].r = lcol[2].r;
    col[2].g = lcol[2].g;
    col[2].b = lcol[2].b;
    dir[2].x = ldir[2].x;
    dir[2].y = ldir[2].y;
    dir[2].z = ldir[2].z;
  }
  else {
    ambcol.x = (c->lights).AmbCol.x;
    ambcol.z = (c->lights).AmbCol.z;
    ambcol.y = (c->lights).AmbCol.y;
    ppVar3 = (c->lights).pDir1st;
    ppVar1 = (c->lights).pDir2nd;
    col[0].r = (ppVar3->Colour).r;
    col[0].b = (ppVar3->Colour).b;
    col[0].g = (ppVar3->Colour).g;
    ppVar2 = (c->lights).pDir3rd;
    dir[0].x = (ppVar3->Direction).x;
    dir[0].z = (ppVar3->Direction).z;
    dir[0].y = (ppVar3->Direction).y;
    col[1].r = (ppVar1->Colour).r;
    col[1].b = (ppVar1->Colour).b;
    col[1].g = (ppVar1->Colour).g;
    dir[1].x = (ppVar1->Direction).x;
    dir[1].z = (ppVar1->Direction).z;
    dir[1].y = (ppVar1->Direction).y;
    col[2].r = (ppVar2->Colour).r;
    col[2].b = (ppVar2->Colour).b;
    col[2].g = (ppVar2->Colour).g;
    dir[2].x = (ppVar2->Direction).x;
    dir[2].z = (ppVar2->Direction).z;
    dir[2].y = (ppVar2->Direction).y;
  }
  if ((c->obj).dead == '\x11') {
    dur = (c->obj).die_duration;
    t = (c->obj).die_time;
    if (dur <= t) {
      dur = 0.0;
    }
    else {
      dur = 1.0 - t / dur;
    }
  }
  else {
    dur = 1.0;
  }
  if (dur != 1.0) {
    ambcol.x = ambcol.x * dur;
    ambcol.y = ambcol.y * dur;
    ambcol.z = ambcol.z * dur;
    col[0].r = col[0].r * dur;
    col[0].g = col[0].g * dur;
    col[0].b = col[0].b * dur;
    col[1].r = col[1].r * dur;
    col[1].g = col[1].g * dur;
    col[1].b = col[1].b * dur;
    col[2].r = col[2].r * dur;
    col[2].g = col[2].g * dur;
    col[2].b = col[2].b * dur;
  }
  SetLights(col,dir,col + 1,dir + 1,col + 2,dir + 2,&ambcol);
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


nuvec_s * GetLightPosition(int index)

{
  return &Lights[index].pos;
}


