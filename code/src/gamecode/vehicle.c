s32 jonfirst = 0;
struct VEHMASK VehicleMask[2];
ZOFFASTRUCT EnemyZoffa[4];
HOVASTRUCT HovaBlimp[6];
static struct nuvec_s GliderAutoPosition;
s32 WBIntroOn;
float FireFlyIntroTween;
s32 FlyingLevelExtro;
GLIDERSTRUCT PlayerGlider;
float WBLIMX;
float WBMAXY;
float WBMINY;
float WBTARGLIMX;
float WBTARGLIMY;
float WeatherBossTargetAppearTimer; 
struct nuvec_s WeatherBossTargetPos;

//NGC MATCH
void VehicleSetup(void) {
    JeepInControl = 0;
}

//NGC MATCH
void MoveVehicle(struct creature_s * Cre, struct nupad_s * Pad) {
    switch (Cre->obj.vehicle) {
    case 0x81:
    case 0x8B:
    case 0x36:
        MoveGlider();
        return;
    case 0x53:
        MoveAtlas();
        return;
    case 0x63:
        MovePlayerJeep();
        return;
    }
}

//NGC MATCH
s32 PickGliderTarget(struct nuvec_s **Target,struct nuvec_s **Vel,s32 *Moving) {
  float best;
  
  if ((PlayerGlider.AutoPilot != 0) || (FlyingLevelExtro != 0)) {
    return 0;
  }
  else {
    if (Level == 0x24) {
      best = GetBigGunBestTarget(0.0f,Target,Vel,Moving);
    }
    else {
      best = GetZoffaBestTarget(GetGunBoatBestTarget(GetBattleShipBestTarget(0.0f,Target,Vel,Moving),Target,Vel,Moving),Target,Vel,Moving);
    }
      if (best > 0.96f) {
          return 1;
      }
      return 0;
  }
}

//NGC MATCH
s32 StillLockedOnTarget(struct nuvec_s *Target) {
  struct nuvec_s *v0;
  struct nuvec_s nStack_40;
  struct nuvec_s nStack_30;
  struct nuvec_s nStack_20;
  float Dot;
  float Mag;
  
  if (FlyingLevelExtro == 0) {
    v0 = SetNuVecPntr(0.0f,0.0f,1.0f);
    NuVecMtxRotate(&nStack_40,v0,&GameCam[0].m);
    NuVecSub(&nStack_30,Target,&GameCam[0].pos);
    Mag = NuVecMag(&nStack_30);
    if (10.0f < Mag) {
      NuVecScale(&nStack_30,&nStack_30,1.0f / Mag);
      Dot = DotProduct(&nStack_40,&nStack_30);
      if (0.9f < Dot) {
        NuVecSub(&nStack_20,&PlayerGlider.Position,Target);
        NuVecNorm(&nStack_20,&nStack_20);
        NuVecScale(&nStack_20,&nStack_20,3.0f);
        if(NewRayCast(Target,&nStack_20,0.0f) == 0) {
            return 1;
        }
        return 0;
      }
    }
  }
  return 0;
}

//NGC MATCH
s32 GetGliderHealthPercentage(struct creature_s *Cre) {
  if (Cre->Buggy == NULL) {
    return 100;
  }
  return Cre->Buggy[0x2f].a;
}

//NGC MATCH
void InitWeatherBossTarget(void) {
  WeatherBossTargetPos = SetNuVec(0.0f,0.0f,-15.0f);
  WeatherBossTargetVel = v000;
  WeatherBossTargetAppearTimer = 0;
  return;
}

//NGC MATCH
void JonnyParticles(struct nuvec_s *Pos) {
  struct nuvec_s vec;
  s32 i;
  
  if (ebgo != 0) {
    if (ebgo == 1) {
      ebgo = 2;
      ebpos[0].x = 99999.0f;
      ebtime[0] = 0.0f;
      ebtime[1] = 10.0f;
      ebtime[2] = 20.0f;
      ebpos[1].x = 99999.0f;
      ebpos[2].x = 99999.0f;
    }
    for(i = 0; i < 3; i++) {
      ebtime[i] = (ebtime[i] - 1.0f);
      if (ebtime[i] < 0.0f) {
        if (ebpos[i].x != 99999.0f) {
          ebpos[i].y = ebpos[i].y + 1.0f;
          AddGameDebris(0x17,&ebpos[i]);
        }
        ebtime[i] = 30.0f;
        vec = *Pos;
        vec.x = ((s32)(qrand() - 0x8000U) * 0.000061035156f + vec.x);
        vec.z = ((s32)(qrand() - 0x8000U) * 0.000061035156f + vec.z);
        vec.y = (vec.y + 12.0f);
        vec.y = (NewShadow(&vec,0.0f) + 12.0f);
        ebpos[i] = vec;
      }
      if (ebpos[i].x != 99999.0f) {
        ebpos[i].y -= 0.4f;
        AddVariableShotDebrisEffect(GDeb[64].i,&ebpos[i],1,0,0);
      }
    }
  }
  return;
}

//NGC MATCH
void DrawJonny(void) {
  s32 loop;
  struct numtx_s m;
  
  if ((JonnyOn == 1) && (ebgo != 0)) {
    for(loop = 0; loop < 3; loop++) {
      if (ebpos[loop].x != 99999.0f) {
        NuMtxSetRotationX(&m,(ebtime[loop] * 128.0f));
        NuMtxRotateZ(&m,(ebtime[loop] * 200.0f));
        NuMtxTranslate(&m,&ebpos[loop]);
        NuSpecialDrawAt(&ObjTab[0x58].obj,&m);
      }
    }
  }
  return;
}

//NGC MATCH
void BossBar(float x,float y,float z,float xs,float ys,s32 i,s32 j) {
  float HealthScale;
  
  HealthScale = ((float)i / (float)j);
  if (1.0f < HealthScale) {
    HealthScale = 1.0f;
  }
  if (HealthScale < 0.0f) {
    HealthScale = 0.0f;
  }
  DrawPanel3DObject(0xb9,x,y,z,xs,ys,0.1f,0,0,0,ObjTab[185].obj.scene,ObjTab[185].obj.special,1);
  DrawPanel3DObject(0xbb,x,y,z,(xs * HealthScale),ys,0.1f,0,0,0,ObjTab[187].obj.scene,ObjTab[187].obj.special,1);
  return;
}

s32 GetCurrentRumbleObjectives(void) {
  return EarthBoss.HitPoints;
}

s32 GetRumblePlayerHealthPercentage(struct creature_s *Cre) {
  return PlayerAtlas.HitPoints;
}

//NGC MATCH
void DrawAtlas(struct creature_s *Cre) {
  struct ATLASSTRUCT *Atlas;
  struct numtx_s Matrix;
  s32 i;
  
  Atlas = (struct ATLASSTRUCT *)Cre->Buggy;
  NuQuatToMtx(&Atlas->Quat,&Matrix);
  NuMtxTranslate(&Matrix,&Atlas->Position);
  i = CRemap[83];
  if (i != -1) {
    NuHGobjRndr(CModel[i].hobj,&Matrix,1,NULL);
  }
  return;
}

//NGC MATCH
void InitAtlas(struct ATLASSTRUCT *Atlas,struct nuvec_s *Pos,float Radius,s32 Type) {
  struct creature_s *Cre;
  struct creature_s *Cre2;
  struct cdata_s* cdata;
  
  Cre = Atlas->Cre;
  memset(Atlas,0,900);
  Atlas->Type = Type;
  Atlas->Cre = Cre;
  Atlas->Position = *Pos;
  Atlas->OldPosition = Atlas->Position;
  Atlas->Radius = Radius;
  Atlas->AngleY = 90.0f;
  Atlas->Quat = SetNuQuat(0.0f,0.0f,0.0f,1.0f);
  Atlas->FrameQuat[0] = Atlas->Quat;
  Atlas->FrameQuat[1] = Atlas->Quat;
  Atlas->FrameQuat[2] = Atlas->Quat;
  Atlas->FrameQuat[3] = Atlas->Quat;
  Atlas->LastQuat = Atlas->Quat;
  Atlas->LastNormal = SetNuVec(0.0f,1.0f,0.0f);
  Atlas->PlatformId = -1;
  if (Cre != NULL) {
    InitTrail();
    Cre2 = Atlas->Cre;
    (Cre2->obj).SCALE = 1.0f;
    (Cre2->obj).scale = 1.0f;
    cdata = &CData[83];
    (Atlas->Cre->obj).bot = cdata->min.y;
    (Atlas->Cre->obj).top = cdata->max.y;
    (Atlas->Cre->obj).vehicle = 0x53;
  }
  return;
}

//NGC MATCH
void ResetAtlas(struct creature_s *Cre) {
  struct ATLASSTRUCT *Atlas;
  struct nuvec_s Pos;
  
  Atlas = (struct ATLASSTRUCT *)Cre->Buggy;
  if (Atlas == NULL) {
    Cre->Buggy = (struct NEWBUGGY *)&PlayerAtlas;
    Atlas = &PlayerAtlas;
    memset(&PlayerAtlas,0,900);
    PlayerAtlas.Cre = Cre;
  }
  AtlasFrame = -1;
  AtlasEmbeddedFrame = 0;
  Pos.x = (Cre->obj).pos.x;
  Pos.y = (Cre->obj).pos.y + 1.0f;
  Pos.z = (Cre->obj).pos.z;
  AtlasWhackValue = 0.0f;
  ResetAtlasCamera = 1;
  AtlasWhackTimer = 0.0f;
  RumbleCamTween = 1.0f;
  RumbleCamTweenDest = 1.0f;
  RumbleCamVal = 0;
  InitAtlas(Atlas,&Pos,0.65f,1);
  Atlas->HitPoints = 100;
  Atlas->DestHitPoints = 100;
  return;
}

//NGC MATCH
void KillAtlasphere(struct ATLASSTRUCT *Atlas) {
  if (Atlas->Cre != NULL) {
    Atlas->Dead = 1;
    Atlas->Cre->obj.invincible = 0;
    Atlas->Velocity = v000;
    if ((Atlas->Cre->obj).dead == 0) {
      KillGameObject(&Atlas->Cre->obj,0xb);
    }
  }
  return;
}

struct nuvec_s lbl_80118DA8 = {0.0f, -2.35f, 7.98f};

//NGC MATCH
void InitEarthBoss(void) {
  struct nuvec_s Start;
  
  Start = lbl_80118DA8;
  InitAtlas(&EarthBoss,&Start,1.0f,2);
  EarthBoss.HitPoints = 3;
  MyInitModelNew(&EarthBoss.Shell,0xad,0x1f,0,NULL,&EarthBoss.Position);
  MyInitModelNew(&EarthBoss.Crunch,0x7f,0x22,0,NULL,&EarthBoss.Position);
  ShootRockSound = 0;
  EarthBoss.LastAction = -1;
  EarthBoss.Action = 0;
  return;
}

void InitRumblePanel(void) {
  NumRockPanel = 0;
}

void DrawEarthBossLevelExtra(void) {
  DrawEarthBoss();
  DrawJeepRocks();
  DrawVehMasks();
  return;
}

//NGC MATCH
void ProcessEarthBossVortex(void) {
  if (EarthBossVortexOpen != 0) {
    CheckAtlasVortex(&EarthBoss);
    CheckAtlasVortex(&PlayerAtlas);
  }
}

//NGC MATCH
void LoadVehicleStuff(void) {
  switch(Level) {
      case 3:
          LoadWesternArenaData();
      break;
      case 0xd:
          jonfirst = 0;
      break;
  }
  return;
}

//NGC MATCH
void InitVehMasks(void) {
    memset(VehicleMask,0,0x2e8);
}

//NGC MATCH
void InitVehMask(s32 Indx,s32 Id) {
  struct VEHMASK* Mask;
  
  memset(&VehicleMask[Indx],0,0x174);
  Mask = &VehicleMask[Indx];
  if (MyInitModelNew(&Mask->MainDraw,Id,0x22,0,NULL,&Mask->Position) != 0) {
    Mask->Id = Id;
  }
  return;
}

//NGC MATCH
void SetNewMaskStuff(s32 Indx,struct nuvec_s *Centre,struct nuvec_s *Off,float Rad,float AngInc,float TweenInc,
                        s32 FixedNUVEC,s32 KillAtEnd,float Scale,float TiltX) {
  struct VEHMASK* Mask;

  Mask = &VehicleMask[Indx];
  Mask->KillAtEnd = KillAtEnd;
  if (Mask->Active != 0) {
    Mask->Tween = 1.0f;
    Mask->TweenInc = TweenInc * 0.01666667f;
    Mask->Offset[1] = Mask->Offset[0];
    Mask->AngInc[1] = Mask->AngInc[0];
    Mask->Rad[1] = Mask->Rad[0];
    Mask->Scale[1] = Mask->Scale[0];
    Mask->TiltX[1] = Mask->TiltX[0];
    if (Mask->Point[0] == &Mask->Store[0]) {
      Mask->Point[1] = &Mask->Store[1];
      Mask->Store[1] = *Mask->Point[0];
    }
    else {
      Mask->Point[1] = Mask->Point[0];
    }
  }
  else {
    Mask->Tween = 0.0f;
    Mask->Active = 1;
    Mask->Point[1] = &v000;
  }
  if (FixedNUVEC != 0) {
    Mask->Store[0] = *Centre;
    Mask->Point[0] = &Mask->Store[0];
  }
  else {
    Mask->Point[0] = Centre;
  }
  Mask->Offset[0] = *Off;
  Mask->AngInc[0] = AngInc * 0.01666667f;
  Mask->Rad[0] = Rad;
  Mask->Scale[0] = Scale;
  Mask->TiltX[0] = TiltX;
  return;
}

//NGC MATCH
void ProcessGliderMovementWB(GLIDERSTRUCT *Glider) {
  float Dest;
  struct nuvec_s Temp;
  
  if (WBIntroOn != 0) {
    Glider->Position = WBIntroGliderPos;
    Glider->Position.z = 0.0f;
  }
  else {
    if (Glider->BarrelRoll == 0) {
      Glider->Velocity.z = -Level_GliderSpeed;
      Glider->Velocity.x = Glider->TiltZ * WBTILTZSCALE;
      Glider->Velocity.y = Glider->TiltX * WBTILTXSCALE;
    }
    NuVecScaleAccum(&Glider->Position,&Glider->Velocity,0.01666667f);
    if (Glider->Position.x > WBLIMX) {
      Glider->Position.x = WBLIMX;
    }
    if (Glider->Position.x < -WBLIMX) {
      Glider->Position.x = -WBLIMX;
    }
    if (Glider->Position.y > WBMAXY) {
      Glider->Position.y = WBMAXY;
    }
    if (Glider->Position.y < WBMINY) {
      Glider->Position.y = WBMINY;
    }
    if (Glider->BarrelRoll > 1) {
      Dest = -WBBARRELSPEED;
    }
    else if (Glider->BarrelRoll <= -2) {
        Dest = WBBARRELSPEED;
    } else {
        Dest = 0.0f;
    }
    if (Dest == 0.0f) {
      SeekHalfLife(&Glider->BarrelSpeedX,Dest,WBBARRELSEEKSPEEDZERO,0.01666667f);
    }
    else {
      SeekHalfLife(&Glider->BarrelSpeedX,Dest,WBBARRELSEEKSPEEDNONZERO,0.01666667f);
    }
    Temp = SetNuVec(Glider->BarrelSpeedX,0.0f,0.0f);
    NuVecRotateY(&Temp,&Temp,(s32)(Glider->AngleY * 182.0444f));
    Glider->Position.x = Temp.x * 0.01666667f + Glider->Position.x;
    Glider->Position.y = Temp.y * 0.01666667f + Glider->Position.y;
    Glider->Position.z = Temp.z * 0.01666667f + Glider->Position.z;
  }
  return;
}

//NGC MATCH
void DrawGlider(struct creature_s *Cre) {
  GLIDERSTRUCT *Glider;
  s32 i;
  float ExtraX;
  
  Glider = (GLIDERSTRUCT *)Cre->Buggy;
  if ((Level == 0xd) || (Level == 0x1a)) {
    ExtraX = 0.0f;
  }
  else {
    ExtraX = 15.0f;
  }
  if (Glider != NULL) {
    if ((Glider->AutoPilot != 0) && (Level == 0xd)) {
      NuMtxSetRotationX(&mTEMP,(int)(ExtraX * 182.04445f));
      NuMtxRotateY(&mTEMP,0);
      NuMtxTranslate(&mTEMP,&GliderAutoPosition);
    }
    else {
      if (Level == 0x1a) {
        NuMtxSetRotationZ(&mTEMP,(int)(Glider->TiltZ * 182.04445f));
        NuMtxRotateX(&mTEMP,(int)((Glider->TiltX + Glider->TiltX) * 182.04445f));
        NuMtxRotateY(&mTEMP,(int)(Glider->AngleY * 182.04445f));
        NuMtxTranslate(&mTEMP,&Glider->Position);
      }
      else {
        NuMtxSetRotationZ(&mTEMP,(int)(Glider->TiltZ * 182.04445f));
        NuMtxRotateX(&mTEMP,(int)((float)(Glider->TiltX + ExtraX) * 182.04445f));
        NuMtxRotateY(&mTEMP,(int)(Glider->AngleY * 182.04445f));
        NuMtxTranslate(&mTEMP,&Glider->Position);
      }
    }
    switch (Level) {
        case 0x1a:
          i = CRemap[0x81];
        break;
        case 0x12:
          i = CRemap[0x36];
        break;
        default:
          i = CRemap[0x8b];
        break;
    }
    if (i != -1) {
      DrawCharacterModel(CModel + i,&(Cre->obj).anim,&mTEMP,NULL,1,NULL,Cre->mtxLOCATOR[8],Cre->momLOCATOR[8],&Cre->obj);
    }
  }
  return;
}

//NGC MATCH
void InitGlider(GLIDERSTRUCT *Glider,struct nuvec_s *StartPos,float StartAng) {
  struct cdata_s *cdata;
  struct creature_s *temp;
  s32 i;

  WBIntroOn = 0;
  memset(Glider,0,0x440);
  Glider->Cre = player;
  if (StartPos != NULL) {
    Glider->Position = *StartPos;
  }
  Glider->HitPoints = 100;
  Glider->AngleY = StartAng;
  Glider->BarrelRoll = 0;
  Glider->TiltX = 0.0f;
  Glider->DestTiltX = 0.0f;
  Glider->TiltZ = 0.0f;
  Glider->DestTiltZ = 0.0f;
  Glider->FireTimer = 0.0f;
  Glider->RailAngle = 0.0f;
  for(i = 0; i < 0x10; i++) {
    Glider->LocatorList[i] = 0x7f;
    Glider->LocatorTime[i] = 0.0f;
  }
  edppRegisterPointerToGameCharLocation(&Glider->Position);
  if (Level == 0x1a) {
    cdata = CData + 0x81;
  }
  else {
    cdata = CData + 0x36;
  }
  temp = Glider->Cre;
  temp->obj.SCALE = 1.0f;
  temp->obj.scale = 1.0f;
  Glider->Cre->obj.bot = cdata->min.y;
  Glider->Cre->obj.top = cdata->max.y;
  FireFlyIntroTween = 0.0f;
  return;
}

//NGC MATCH
void DrawWeatherBossTarget(void) {
  s32 Obj;
  float Scale;
    
  if ((WBIntroOn == 0) && (FlyingLevelExtro == 0)) {
    Scale = (1.0f - WeatherBossTargetAppearTimer / WeatherBossTargetAppearTime) * WBTARGSCALE;
    Obj = 0x5a;
    if (ObjTab[Obj].obj.special != NULL) {
      mTEMP = WeatherBossCamMtx;
      NuVecAdd((struct nuvec_s *)&mTEMP._30,(struct nuvec_s *)&mTEMP._30,(struct nuvec_s *)&WeatherBossTargetPos);
      NuMtxPreScale(&mTEMP,SetNuVecPntr(Scale,Scale,0.05f));
      NuRndrGScnObj((ObjTab[Obj].obj.scene)->gobjs[(ObjTab[Obj].obj.special)->instance->objid],&mTEMP);
    }
  }
  return;
}

//NGC MATCH
void ProcessWeatherBossTarget(void) {
  float CentreY;
  float RangeY;
  
  CentreY = WBMAXY - WBMINY;
  RangeY = (WBMAXY + WBMINY) * 0.5f;
  if ((WBIntroOn == 0) && (FlyingLevelExtro == 0)) {
    ProcessTimer(&WeatherBossTargetAppearTimer);
  }
  WeatherBossTargetPos.x = (PlayerGlider.Position.x * WBTARGLIMX) / WBLIMX;
  WeatherBossTargetPos.y = ((PlayerGlider.Position.y - RangeY) * WBTARGLIMY) / (CentreY * 0.5f) + 1.0f;
  return;
}

//NGC MATCH
static char GrabAnotherLocator(char *List,struct NUPOINTOFINTEREST_s **PList) {
  s32 i;
  s32 j;
  s32 k;
  
  j = (s32)(frand() * 16.0f);
  for(i = 0; i < 0x10; i++, j++) {
    if (0xf < j) {
      j -= 0x10;
    }
    if (PList[j] != NULL) {
      for(k = 0; k < 0x10; k++) {
        if ((List[k] == 0x7f) || (List[k] == j)) break;
      }
      if ((k == 0x10) || (List[k] == 0x7f)) {
        return (char)j;
      }
    }
  }
  return 0x7f;
}

//NGC MATCH
void ExplodeGlider(GLIDERSTRUCT *Glider) {
  NewBuzz(&player->rumble,BEENHITBIGBUZZTIME);
  NewRumble(&player->rumble,BEENHITBIGRUMTIME);
  if (Level == 0x1a) {
    MyGameSfx(0xb7,NULL,0x3fff);
  }
  else {
    MyGameSfx(0x56,NULL,0x3fff);
  }
  if (InvincibilityCHEAT != 0) {
    return;
  }
    Glider->Dead = 1;
    FarmResetTimer = 0x3c;
    if (TimeTrial != 0) {
        new_mode = GameMode;
        return;
    }
    if (plr_lives.count != 0) {
        plr_lives.count--;
        if (Adventure != 0) {
            LivesLost++;
            Game.lives = plr_lives.count;
        }
        new_mode = GameMode;
        return;
    }
    new_level = 0x26;
    return;
}

//NGC MATCH
void DeadGliderWB(GLIDERSTRUCT *Glider) {
  if (Glider->Dead == 0) {
    ExplodeGlider(Glider);
  }
  return;
}

//NGC MATCH
void DeadGliderCoco(GLIDERSTRUCT *Glider) {
  if (Glider->CocoDead == 0) {
    Glider->CocoDead = 1;
    Glider->CocoDeadTimer = 3.0f;
    Glider->Velocity.x = frand() - 0.5f;
    Glider->Velocity.y = (frand() - 0.5f);
    Glider->Velocity.z = (frand() - 0.5f);
    Glider->CocoDeathSpinX = (frand() - 0.5f) * 360.0f;
    Glider->CocoDeathSpinZ = (frand() - 0.5f) * 360.0f;
    NuVecNorm(&Glider->Velocity,&Glider->Velocity);
    NuVecScale(&Glider->Velocity,&Glider->Velocity,2.0f);
    AddGameDebris(0x18,&Glider->Position);
  }
  Glider->HitPoints = 0;
  NuVecScaleAccum(&Glider->Position,&Glider->Velocity,0.01666667f);
  Glider->TiltX = Glider->CocoDeathSpinX * 0.01666667f + Glider->TiltX;
  Glider->TiltZ = Glider->CocoDeathSpinZ * 0.01666667f + Glider->TiltZ;
  if (ProcessTimer(&Glider->CocoDeadTimer) != 0) {
    Glider->CocoDeadTimer = 1000000.0f;
    ExplodeGlider(Glider);
  }
  return;
}

struct nuvec_s lbl_8011833C = {0.0f, 0.0f, 1.0f};

//NGC MATCH
static s32 SafePosition(struct nuvec_s *Pos) {
  struct nuvec_s Rel;
  struct nuvec_s Temp;
  struct nuvec_s local_18;
  
  NuVecSub(&Rel,Pos,(struct nuvec_s *)&GameCam[0].m._30);
  if (NuVecMag(&Rel) < 10.0f) {
    return 0;
  }
  local_18 = lbl_8011833C;
  NuVecMtxRotate(&local_18,&local_18,&GameCam[0].m);
  if (NuVecDot(&Rel,&Temp) < 0.0f) {
       return 0;
  }
  return SafeFromCollisions(Pos); 
}

//NGC MATCH
static ZOFFASTRUCT * FindFreeZoffa(void) {
  s32 i;

  for(i = 0; i < 4; i++) {
    if (EnemyZoffa[i].ActiveMode == 0) {
      memset(&EnemyZoffa[i],0,0x5b0);
      return &EnemyZoffa[i];
    }
  }
  return NULL;
}

//NGC MATCH
static void InitZoffaUFOs(void) {
  s32 i;
  
  memset(EnemyZoffa,0,0x16c0);
  ZoffaTeleportIndx = 0;
  for(i = 0; i < 4; i++) {
    InitZoffa(NULL,&ZoffaStartPoints[i]);
  }
  CurrentAggressor = 0;
  ZoffaCollisionCounter = 0;
  return;
}

//NGC MATCH
void DrawZoffa(ZOFFASTRUCT *Zoffa) {
  NuMtxSetRotationZ(&mTEMP,(s32)(Zoffa->TiltZ * 182.0444f));
  NuMtxRotateX(&mTEMP,(s32)(-Zoffa->TiltX * 182.0444f));
  NuMtxRotateY(&mTEMP,(s32)((Zoffa->AngleY + 180.0f) * 182.0444f));
  NuMtxTranslate(&mTEMP,&Zoffa->Position);
  Zoffa->Seen = MyDrawModelNew(&Zoffa->MainDraw,&mTEMP,Zoffa->Locators);
  return;
}

//NGC MATCH
void DrawZoffaUFOs(void) {
  s32 i;
 
  for(i = 0; i < 4; i++) {
    if (EnemyZoffa[i].ActiveMode != 0) {
      DrawZoffa(&EnemyZoffa[i]);
    } 
  }
  return;
}

//NGC MATCH
static ZOFFASTART * FindZoffaRestartPoint(void) {
  ZOFFASTART *ret;
  s32 j;

  j = (s32)(frand() * 16.0f);
  while (j > 0x10) {
      j -= 4;
  }
  while (j < 0) {
      j += 0x10;
  }
  ret = &ZoffaStartPoints[j];
  if (SafePosition((struct nuvec_s*)ret) != 0) {
      return ret;
  }
   return NULL;
}

//NGC MATCH
static void ZoffaUFORespawn(void) {
  s32 i;
  s32 NumActive;
  ZOFFASTART *Start;
  
  NumActive = 0;
  for(i = 0; i < 4; i++) {
    if (EnemyZoffa[i].ActiveMode != 0) {
      NumActive++;
    }
  }
  if (NumActive < 4) {
    for(i = 0; i < 4; i++) {
      if ((EnemyZoffa[i].ActiveMode == 0) && (EnemyZoffa[i].RespawnTimer == 0.0f)) {
        Start = (ZOFFASTART *)FindZoffaRestartPoint();
        if (Start == NULL) {
          return;
        }
        InitZoffa(&EnemyZoffa[i],Start);
        return;
      }
    }
  }
  return;
}

//NGC MATCH
static void ZoffaSmoke(ZOFFASTRUCT *Zoffa) {
  struct nuvec_s Temp;

  NuMtxSetRotationZ(&mTEMP,(int)(Zoffa->TiltZ * 182.04445f));
  NuMtxRotateX(&mTEMP,(int)(-Zoffa->TiltX * 182.04445f));
  NuMtxRotateY(&mTEMP,(int)((Zoffa->AngleY + 180.0f) * 182.04445f));
  NuMtxTranslate(&mTEMP,&Zoffa->Position);
  if (Zoffa->TerminalDive != 0) {
    if (Zoffa->SmokeCounter != 0) {
      vec = SetNuVecPntr(-1.0f,0.2f,1.0f);
      NuVecMtxTransform(&Temp,vec,&mTEMP);
      AddGameDebrisRot(0x1d,&Temp,1,0,0);
        vec = SetNuVecPntr(0.0f,1.2f,1.0f);
      NuVecMtxTransform(&Temp,vec,&mTEMP);
      AddGameDebrisRot(0x1d,&Temp,1,0,0);
      Zoffa->SmokeCounter = 0;
    }
    else {
      NuVecMtxTransform(&Temp,SetNuVecPntr(1.0f,0.2f,1.0f),&mTEMP);
      AddGameDebrisRot(0x1d,&Temp,1,0,0);
      NuVecMtxTransform(&Temp,SetNuVecPntr(0.0f,-0.8f,1.0f),&mTEMP);
      AddGameDebrisRot(0x1d,&Temp,1,0,0);
      Zoffa->SmokeCounter = 1;
    }
    NuVecMtxTransform(&Temp,SetNuVecPntr(0.0f,0.2f,1.0f),&mTEMP);
    AddGameDebrisRot(0x45,&Temp,1,0,0);
  }
  else {
    if (((Zoffa->HitPoints < 2) &&
        (Zoffa->SmkTimer--, Zoffa->SmkTimer < 1)) && (Level != 0x1a)) {
      NuVecMtxTransform(&Temp,SetNuVecPntr(0.0f,0.2f,1.0f),&mTEMP);
      AddGameDebrisRot(0x1d,&Temp,1,0,0);
      Zoffa->SmkTimer = Zoffa->HitPoints;
    }
  }
  return;
}

//NGC MATCH
void TeleportManager(void) {
  struct nuvec_s Point;
  s32 Indx;
  s32 i;
  s32 j;
  struct ZOFFASTART Start;
  
  for(i = 0; i < 4; i++) {
    if ((EnemyZoffa[i].ActiveMode != 0) && (EnemyZoffa[i].Teleport != 0)) {
       for(j = 0; j < 2; j++) {
          Indx = j + ZoffaTeleportIndx & 3;
          NuVecMtxTransform(&Point,&TeleportPos[Indx],&GameCam[0].m);
          if (Point.y < Level_GliderFloor) {
            Point.y = Level_GliderFloor;
          }
          if (SafeFromCollisions(&Point) != 0) {
            Start.x = Point.x;
            Start.y = Point.y;
            Start.z = Point.z;
            switch(Indx) {
                case 0:
                    Start.Angle = (PlayerGlider.AngleY - 135.0f);
                break;
                case 1:
                    Start.Angle = (PlayerGlider.AngleY + 135.0f);
                break;
                case 2:
                    Start.Angle = (PlayerGlider.AngleY - 90.0f);
                break;
                case 3:
                    Start.Angle = (PlayerGlider.AngleY + 90.0f);
                break;
            }
            InitZoffa(&EnemyZoffa[i],&Start);
            EnemyZoffa[i].NoTeleportTimer = 3.0f;
            EnemyZoffa[i].KeepSameVelocityTimer = 0.8f;
            EnemyZoffa[i].Speed = 14.0f;
            NuVecMtxRotate(&EnemyZoffa[i].Velocity,&TeleportVel[(j + ZoffaTeleportIndx & 3)],&GameCam[0].m);
            ZoffaTeleportIndx = ZoffaTeleportIndx + 1 & 3;
            break;
          }
       }
    }
  }
}

//NGC MATCH
float GetZoffaBestTarget(float Best,struct nuvec_s **TargetPos,struct nuvec_s **Vel,s32 *Moving) {
  struct nuvec_s CamDir;
  struct nuvec_s Rel;
  s32 i;
  float Dot;
  float Mag;
  
  NuVecMtxRotate(&CamDir,SetNuVecPntr(0.0f,0.0f,1.0f),&GameCam[0].m);
  for(i = 0; i < 4; i++) {
    if (EnemyZoffa[i].ActiveMode != 0) {
      NuVecSub(&Rel,&EnemyZoffa[i].Position,&GameCam[0].pos);
      Mag = NuVecMag(&Rel);
      if (Mag > 10.0f) {
        NuVecScale(&Rel,&Rel,(1.0f / Mag));
        Dot = DotProduct(&CamDir,&Rel);
        if (Dot > Best) {
          *TargetPos = &EnemyZoffa[i].Position;
          *Vel = &EnemyZoffa[i].Velocity;
          *Moving = 1;
          Best = Dot;
        }
      }
    }
  }
  return Best;
}

//NGC MATCH
HOVASTRUCT* FindFreeHova(void) {
  s32 i;
  
  for(i = 0; i < 6; i++) {
    if (HovaBlimp[i].ActiveMode == 0) {
      return &HovaBlimp[i];
    }
  }
  return NULL;
}

//NGC MATCH
void InitialiseHovaBlimps(void) {
  s32 i;
  
  memset(HovaBlimp,0,0x738);
  memset(TornadoList,0,0x660);
  InitHova(SetNuVecPntr(0.0f,50.0f,0.0f),0.0f);
  InitHova(SetNuVecPntr(0.0f,50.0f,160.0f),0.0f);
  InitHova(SetNuVecPntr(152.0f,50.0f,49.0f),0.0f);
  InitHova(SetNuVecPntr(94.0f,50.0f,-129.0f),0.0f);
  InitHova(SetNuVecPntr(-94.0f,50.0f,-129.0f),0.0f);
  InitHova(SetNuVecPntr(-152.0f,50.0f,49.0f),0.0f);
  for(i = 0; i < 0x14; i++) {
    if (jontorn[i] != -1) {
      if (jonfirst != 0) {
        DebFree(&jontorn[i]);
      }
    }
    jontorn[i] = -1;
  }
  jonfirst = 1;
}

//NGC MATCH
static void ProcessTornado(TORNADOSTRUCT *Tornado,HOVASTRUCT *Hova) {
  Tornado->YAng = Tornado->YAngInc * 0.01666667f + Tornado->YAng;
  Tornado->YAng = Rationalise360f(Tornado->YAng);
  if (Hova->ActiveMode != 0) {
    Tornado->Position = Hova->Position;
    Tornado->Position.y = 27.0f;
    Tornado->StartPosition = Tornado->Position;
  }
  else {
    Tornado->Active = 0;
  }
  return;
}

static struct nuvec_s lbl_801184B0 = {6.0f, 6.0f ,6.0f};

//NGC MATCH
void DrawHovaBlimp(HOVASTRUCT *Hova) {
  struct nuvec_s Scale;
  
  Scale = lbl_801184B0;
  NuMtxSetScale(&mTEMP,&Scale);
  NuMtxRotateY(&mTEMP,(int)((Hova->AngleY + 180.0f) * 182.0444f));
  NuMtxTranslate(&mTEMP,&Hova->Position);
  MyDrawModelNew(&Hova->MainDraw,&mTEMP,NULL);
  return;
}

//NGC MATCH
static s32 ProcessAllHovaBlimps(void) {
  s32 i;
  s32 Ret;
  
  Ret = 0;
  ACTIVEBLIMPCOUNT = 0;
  for(i = 0; i < 6; i++) {
    if (HovaBlimp[i].ActiveMode != 0) {
      Ret = 1;
      ProcessHovaBlimp(&HovaBlimp[i]);
      if (HovaBlimp[i].ActiveMode != 0) {
        ACTIVEBLIMPCOUNT++;
      }
    }
  }
  for(i = 0; i < 6; i++) {
    if (TornadoList[i].Active != 0) {
      ProcessTornado(&TornadoList[i],&HovaBlimp[i]);
    }
  }
  return Ret;
}

//NGC MATCH
SATELLITESTRUCT * InitSatellite(SPACESTATIONSTRUCT *SpaceStation,float AngleY) {
  SATELLITESTRUCT *Satellite;

    Satellite = &SatelliteList[SatelliteCharacterId++];
    if (Satellite->Active != 0) {
      return NULL;
    }
    memset(Satellite,0,0x124);
    if (MyInitModelNew(&Satellite->MainDraw,0x8d,0x1b,0,NULL,&Satellite->Position) == 0) {
     return NULL;
    }
    Satellite->SpaceStation = SpaceStation;
    Satellite->Position = SpaceStation->Position;
    Satellite->AngleY = AngleY;
    Satellite->TiltX = 0.0f;
    Satellite->TiltZ = 0.0f;
    Satellite->DestTiltX = 0.0f;
    Satellite->DestTiltZ = 0;
    Satellite->Active = 1;
    Satellite->HitPoints = 0x10;
    Satellite->Velocity = v000;
    return Satellite;
}

//NGC MATCH
void DrawSatellite(SATELLITESTRUCT *Satellite) {
  mTEMP = Satellite->SpaceStation->Matrix;
  NuMtxPreRotateY(&mTEMP,(int)(Satellite->AngleY * 182.04445f));
  NuMtxPreScale(&mTEMP,SetNuVecPntr(3.0f,3.0f,3.0f));
  *(struct nuvec_s*)(&mTEMP._30) = Satellite->Position;
  Satellite->Seen = MyDrawModelNew(&Satellite->MainDraw,&mTEMP,NULL);
  return;
}

//NGC MATCH
void ProcessSatellite(SATELLITESTRUCT *Satellite) {
  s32 Id;
  short Vol;
  struct nuvec_s vec;
  
  MyAnimateModelNew(&Satellite->MainDraw,0.5f);
  NuVecRotateY(&vec,SetNuVecPntr(0.0f,0.0f,18.0f),((Satellite->AngleY + Satellite->SpaceStation->SatelliteAngleY) * 182.0444f));
  NuVecMtxTransform(&Satellite->Position,&vec,&Satellite->SpaceStation->Matrix);
  CollideGliderBullets(&Satellite->Position,3.0f,1,0.6666667f,0,1);
  if (CollideGliderBullets(&Satellite->Position,3.0f,0,0.6666667f,0,0) != 0) {
    Satellite->HitPoints--;
    if (Satellite->HitPoints > 0) {
      MyGameSfx(0xbb,&Satellite->Position,0x3fff);
    }
    AddScreenWumpa((Satellite->Position).x,(Satellite->Position).y,(Satellite->Position).z,1);
    ClockOff();
  }
  if ((Satellite->HitPoints < 1) || (Satellite->Explode != 0)) {
    Satellite->Active = 0;
    AddGameDebris(0x17,&Satellite->Position);
    MyGameSfx(0xb4,&Satellite->Position,0x7fff);
  }
  SeekAngHalfLife360f(&Satellite->TiltX,Satellite->DestTiltX,1.0f,0.01666667f);
  SeekAngHalfLife360f(&Satellite->TiltZ,Satellite->DestTiltZ,1.0f,0.01666667f);
  Id = -1;
  Vol = 0;
  if (Level == 0x1a) {
    Vol = CRASHTEROIDSSATELLITEVOL;
    Id = 0xbd;
  }
  if (Id != -1) {
    MyGameSfxLoop(Id,&Satellite->Position,Vol);
  }
}

//NGC MATCH
void ProcessSatellites(void) {
  s32 i;

  for(i = 0; i < 9; i++) {
    if (SatelliteList[i].Active != 0) {
      ProcessSatellite(&SatelliteList[i]);
    }
  }
  return;
}

//NGC MATCH
void DrawSatellites(void) {
  s32 i;

  for(i = 0; i < 9; i++) {
    if (SatelliteList[i].Active != 0) {
      DrawSatellite(&SatelliteList[i]);
    }
  }
  return;
}

//NGC MATCH
void InitSpaceStation(struct nuvec_s *Pos,float AngleY,float TiltX,float TiltZ,int Character) {
  SPACESTATIONSTRUCT *SpaceStation;
  s32 i;

  SpaceStation = &SpaceStationList[Character];
  if ((Character > 2) || (SpaceStation->Active != 0)) {
      return;
  }
  memset(SpaceStation,0,0xd0);
  SpaceStation->Position = *Pos;
  SpaceStation->Active = 1;
  SpaceStation->AngleY = AngleY;
  SpaceStation->HitPoints = 0x28;
  SpaceStation->Velocity = v000;
  SpaceStation->SatelliteAngleY = (float)(Character) * 40.0f;
  NuMtxSetScale(&mTEMP,SetNuVecPntr(1.0f,1.0f,1.0f));
  NuMtxRotateX(&mTEMP,(int)(TiltX * 182.04445f));
  NuMtxRotateZ(&mTEMP,(int)(TiltZ * 182.04445f));
  NuMtxRotateY(&mTEMP,(int)(SpaceStation->AngleY * 182.04445f));
  NuMtxTranslate(&mTEMP,&SpaceStation->Position);
  SpaceStation->Matrix = mTEMP;
      
  NuVecMtxTransform(&SpaceStation->AppCentre,&SPACETRAN,&SpaceStation->Matrix);
  for(i = 0; i < 3; i++) {
    SpaceStation->Satellite[i] = InitSatellite(SpaceStation,((float)i) * 120.0f);
  }
}

//NGC MATCH
void InitSpaceStations(void) {
  memset(SpaceStationList,0,0x270);
  memset(SatelliteList,0,0xa44);
  SatelliteCharacterId = 0;
  InitSpaceStation(SetNuVecPntr(0.0f,0.0f,80.0f),-0.0f,15.0f,20.0f,0);
  InitSpaceStation(SetNuVecPntr(69.0f,0.0f,-40.0f),-13.0f,-20.0f,7.0f,1);
  InitSpaceStation(SetNuVecPntr(-69.0f,0.0f,-40.0f),-26.0f,15.0f,-20.0f,2);
}

//NGC MATCH
void DrawSpaceStation(SPACESTATIONSTRUCT *SpaceStation) {
  s32 i;
  
  SpaceStation->Seen = 0;
  i = 0x6d;
  if (ObjTab[i].obj.special != NULL) {
    mTEMP = SpaceStation->Matrix;
    SpaceStation->Seen = NuRndrGScnObj(ObjTab[i].obj.scene->gobjs[ObjTab[i].obj.special->instance->objid],&mTEMP);
  }
  if (SpaceStation->Seen != 0) {
    if (SpaceStation->ShieldDraw == 0) {
      return;
    }
    i = 0x71;
    if (SpaceStation->ShieldGreen != 0) {
      i = 0x70;
    }
    if (ObjTab[i].obj.special != NULL) {
      mTEMP = SpaceStation->ShieldMtx;
      NuRndrGScnObj(ObjTab[i].obj.scene->gobjs[ObjTab[i].obj.special->instance->objid],&mTEMP);
      NuMtxPreRotateY(&SpaceStation->ShieldMtx,0x2000);
    }
  }
  if (SpaceStation->ShieldDraw != 0) {
    SpaceStation->ShieldDraw--;
  }
  return;
}

//NGC MATCH
void ProcessSpaceStations(void) {
  s32 i;
  
  PlayerGlider.InTornado = 0;
  for(i = 0; i < 3; i++) {
    if (SpaceStationList[i].Active != 0) {
      ProcessSpaceStation(&SpaceStationList[i]);
    }
  }
}

//NGC MATCH
void DrawSpaceStations(void) {
  s32 i;

  for(i = 0; i < 3; i++) {
    if (SpaceStationList[i].Active != 0) {
      DrawSpaceStation(&SpaceStationList[i]);
    }
  }
  return;
}

//NGC MATCH
s32 AddGliderBullet(struct numtx_s *Mat,struct nuvec_s *Pos,struct nuvec_s *Vel,s32 Enemy) {
  GLIDERBULLET *Bullet;
  
  Bullet = GrabGliderBullet(Enemy);
  if (Bullet != NULL) {
    if (Enemy != 0) {
      Bullet->Life = 0x78;
    }
    else {
      if (Level == 0xd) {
        Bullet->Life = 0x5a;
      }
      else if (Level == 0x1a) {
          Bullet->Life = 0x2d;
      } else {
          Bullet->Life = 0xb4;
      }
    }
    if (Enemy != 0) {
      Bullet->FirstLife = Bullet->Life - 1;
    }
    else {
      Bullet->FirstLife = Bullet->Life + 1;
    }
    Bullet->Mat = *Mat;
    Bullet->Mat._30 = Pos->x;
    Bullet->Mat._31 = Pos->y;
    Bullet->Mat._32 = Pos->z;
    Bullet->Vel.x = Vel->x * 0.016666668f;
    Bullet->Vel.y = Vel->y * 0.016666668f;
    Bullet->Vel.z = Vel->z * 0.016666668f;
    return 1;
  }
    return 0;
}

//NGC MATCH
void InitPlane(struct nuvec_s *Pos,float Vel,float Timer) {
  memset(&PlayerPlane,0,0x104);
  PlayerPlane.Active = 1;
  if (MyInitModelNew(&PlayerPlane.MainDraw,0x51,0x22,0,NULL,&PlayerPlane.Pos) == 0) {
    PlayerPlane.Active = 0;
  }
  PlayerPlane.Pos = *Pos;
  PlayerPlane.Vel = SetNuVec(0.0f,0.0f,-Vel);
  PlayerPlane.ActionStatus = 0;
  PlayerPlane.ActionTimer = Timer;
}

//NGC MATCH
void DrawPlane(struct PLANESTRUCT *Plane) {
static struct nuvec_s Scale = {1.0f,1.0f,1.0f};
    
  if (Plane->Active != 0) {
    NuMtxSetScale(&mTEMP,&Scale);
    NuMtxTranslate(&mTEMP,&Plane->Pos);
    MyDrawModelNew(&Plane->MainDraw,&mTEMP,NULL);
  }
  return;
}

//NGC MATCH
void MovePlane(PLANESTRUCT *Plane) {
  s32 Action;
  s32 GrabGlider;
  
  GrabGlider = 0;
  if (Plane->Active != 0) {
    Action = Plane->ActionStatus;
    switch(Action) {
        case 0:
              GrabGlider = 1;
              Level_GliderCurrentCeiling = 36.0f;
              if (ProcessTimer(&Plane->ActionTimer) != 0) {
                 Plane->ActionStatus = 1;
              }
        break;
        case 1:
              Plane->ActionStatus = 2;
              Plane->ActionTimer = 1.0f;
              PlayerGlider.ForceTurn = Action;
              GameSfx(0x82,&player->obj.pos);
              NewRumble(&player->rumble,0x7f);
              NewBuzz(&player->rumble,6);
        break;
        case 2:
              if (ProcessTimer(&Plane->ActionTimer) != 0) {
                Plane->ActionStatus = 3;
              }
        break;
        case 3:
              Plane->Active = 0;
        break;
    }
    NuVecScaleAccum(&Plane->Pos,&Plane->Vel,0.01666667f);
    PlayerGlider.AutoPilot = GrabGlider;
    if (GrabGlider != 0) {
      NuVecAdd(&PlayerGlider.Position,&Plane->Pos,&ZepplinGliderOffset);
      GliderAutoPosition = PlayerGlider.Position;
      PlayerGlider.TiltX = 0.0f;
      PlayerGlider.TiltZ = 0.0f;
      PlayerGlider.Velocity = (Plane->Vel);
    }
    MyAnimateModelNew(&Plane->MainDraw,0.5f);
  }
}

//NGC MATCH
void FarmReset(void) {
  if ((new_mode == -1) && (new_level == -1)) {
    FarmResetTimer = 0;
    Level_GliderSpeed = 13.0f;
    Level_GliderFloor = 16.0f;
    Level_GliderCeiling = 66.0f;
    Level_GliderRadius = 177.5;
    Level_DeadTime = 8.0f;
    *(struct nuvec_s*)&GameCam[0].m._30 = FarmCameraStart;
    Level_GliderCentreX = 0.0f;
    Level_GliderCentreZ = 0.0f;
    InitGlider(&PlayerGlider,NULL,0.0f);
    PlayerGlider.AutoPilot = 1;
    InitZoffaUFOs();
    InitGliderBullets();
    InitialiseHovaBlimps();
    InitPlane(&FarmZepplinStart,FarmZepplinSpeed,FarmZepplinTimer);
    GliderCamHighTimer = 1.0f;
    ResetGameCameras(GameCam,1);
    ACTIVEBLIMPCOUNT = 6;
    ResetTimeTrial();
  }
  return;
}

//NGC MATCH
void ProcessFarmLevel(struct nupad_s *Pad) {
  s32 i;
  
  FarmFrame++;
  if (GliderCamHighTimer > 0.0f) {
    GliderCamHighTimer -= 0.01666667f;
  } else {
      GliderCamHighTimer = 0.0f;
  }
  if (FarmResetTimer != 0) {
    FarmResetTimer--;
    if (FarmResetTimer == 0) {
      FarmReset(1);    
    }
  }
  TeleportManager();
  LastNumZoffasFiring = NumZoffasFiring;
  NumZoffasFiring = 0;
  for(i = 0; i < 4; i++) {
    if (EnemyZoffa[i].ActiveMode == 0) {
      if (PlayerGlider.AutoPilot == 0) {
        EnemyZoffa[i].RespawnTimer -= 0.016666668f;
        if (EnemyZoffa[i].RespawnTimer < 0.0f) {
          EnemyZoffa[i].RespawnTimer = 0;
        }
      }
    }
    else {
      MoveZoffaUFO(&EnemyZoffa[i]);
    }
  }
  ZoffaUFORespawn();
  ProcessAllHovaBlimps();
  CheckGliderCollisions();
  CheckForPotentialMidAirCollisions();
  MovePlane(&PlayerPlane);
  ProcessGliderBullets();
}

//NGC MATCH
void DrawFarmLevelExtra(void) {
  DrawZoffaUFOs();
  DrawPlane(&PlayerPlane);
  DrawAllHovaBlimps();
  DrawGliderBullets();
  DrawGliderTarget();
  return;
}

//NGC MATCH
void DrawSpaceArenaLevelExtra(void) {
  DrawGliderBullets();
  DrawAsteroids();
  DrawSatellites();
  DrawSpaceStations();
  DrawZoffaUFOs();
  DrawGliderTarget();
  return;
}

//NGC MATCH
void ProcessSpaceArenaLevel(struct nupad_s *Pad) {
  s32 i;
  
  if (CRASHTEROIDSAMBIENTVOL != 0) {
    MyGameSfxLoop(0xc1,NULL,CRASHTEROIDSAMBIENTVOL);
  }
  ActiveAsteroidListNum = 0;
  TeleportManager();
  LastNumZoffasFiring = NumZoffasFiring;
  NumZoffasFiring = 0;
  for(i = 0; i < 4; i++) {
    if (EnemyZoffa[i].ActiveMode == 0) {
      if (PlayerGlider.AutoPilot == 0) {
        EnemyZoffa[i].RespawnTimer -= 0.016666668f;
        if (EnemyZoffa[i].RespawnTimer < 0.0f) {
          EnemyZoffa[i].RespawnTimer = 0.0f;
        }
      }
    }
    else {
      MoveZoffaUFO(&EnemyZoffa[i]);
    }
  }
  ZoffaUFORespawn();
  ProcessGliderBullets();
  ProcessAsteroids();
  ProcessSatellites();
  ProcessSpaceStations();
  CheckGliderCollisions();
  CheckForPotentialMidAirCollisions();
}

//NGC MATCH
void DrawExtraCreatures(void) {
    switch (Level) {
    case 13:
        DrawFarmLevelExtra();
        return;
    case 18:
        DrawFireFlyLevelExtra();
        return;
    case 36:
        DrawWeatherResearchLevelExtra();
        return;
    case 24:
        DrawWeatherBossLevelExtra();
        return;
    case 26:
        DrawSpaceArenaLevelExtra();
        return;
    case 3:
        DrawWesternArenaLevelExtra();
        return;
    case 22:
        DrawFireBossLevelExtra();
        return;
    case 21:
        DrawEarthBossLevelExtra();
        return;
    }
}

//NGC MATCH
void InitAsteroids(void) {
  s32 i;
  
  memset(&AsteroidList,0,0x1db0);
  for(i = 0; i < 0x1e; i++) {
    InitAsteroid(0);
  }
}

//NGC MATCH
void DrawAsteroid(ASTEROIDSTRUCT *Asteroid) {  
  if (Asteroid->Stealth != 0) {
    NuMtxSetScale(&mTEMP,SetNuVecPntr(0.0f,0.0f,0.0f));
    NuMtxTranslate(&mTEMP,&Asteroid->Position);
  }
  else {
    NuMtxSetScale(&mTEMP,&Asteroid->Scale);
    NuMtxRotateY(&mTEMP,(s32)(Asteroid->AngleY * 182.04445f));
    NuMtxRotateX(&mTEMP,(s32)(Asteroid->AngleX * 182.04445f));
    NuMtxTranslate(&mTEMP,&Asteroid->Position);
  }
  Asteroid->Seen = 0;
  if (ObjTab[0x50].obj.special != NULL) {
    Asteroid->Seen = NuRndrGScnObj((ObjTab[0x50].obj.scene)->gobjs
                          [(ObjTab[0x50].obj.special)->instance->objid],&mTEMP);
  }
}

//NGC MATCH
void ProcessAsteroids(void) {
  s32 i;
  
  if (AsteroidDebug == -1) {
    for(i = 0; i < 100; i++) {
      if (AsteroidList[i].Active != 0) {
        ProcessAsteroid(&AsteroidList[i]);
      }
    }
  }
  else if (AsteroidList[AsteroidDebug].Active != 0) {
    ProcessAsteroid(&AsteroidList[AsteroidDebug]);
  }
}

//NGC MATCH
void DrawAsteroids(void) {
  s32 i;

  for(i = 0; i < 100; i++) {
    if (AsteroidList[i].Active != 0) {
      DrawAsteroid(&AsteroidList[i]);
    }
  }
}

//NGC MATCH
static s32 CheckSphereCollide(struct nuvec_s *A,struct nuvec_s *B,float CombinedRadius,float YScale) {
  struct nuvec_s Rel;
  
  Rel.y = (A->y - B->y) * YScale;
  Rel.x = A->x - B->x;
  Rel.z = A->z - B->z;
  if (Rel.x * Rel.x + Rel.y * Rel.y + Rel.z * Rel.z <= CombinedRadius * CombinedRadius) {
      return 1;
  }
  return 0;
}

//NGC MATCH
static s32 SafeFromCollisions(struct nuvec_s *Pos) {
  s32 i;

  if (CheckSphereCollide(Pos,&PlayerGlider.Position,3.0f,1.0f) != 0) {
    return 0;
  }
    for(i = 0; i < 4; i++) {
      if ((EnemyZoffa[i].ActiveMode != 0) && (CheckSphereCollide(&EnemyZoffa[i].Position,Pos,3.0f,1.0f) != 0))
         return 0;
    }
    return 1;
}

//NGC MATCH
static void GenerateGliderLines(GLIDERSTRUCT *Glider,struct nuvec_s *Lines) {
  struct numtx_s Mat;
  s32 i;
  
  NuMtxSetRotationZ(&Mat,(Glider->TiltZ * 182.04445f));
  NuMtxRotateX(&Mat,((Glider->TiltX + 15.0f) * 182.04445f));
  NuMtxRotateY(&Mat,(Glider->AngleY * 182.04445f));
  NuMtxTranslate(&Mat,&Glider->Position);
  for(i = 0; i < 5; i++) {
    NuVecMtxTransform(&Lines[i],&GliderCollPoints[i],&Mat);
  }
  Lines[5] = *Lines;
}

//NGC MATCH
s32 GetCurrentFarmObjectives(void) {
  s32 i;
  s32 j = 0;
  
  for(i = 0; i < 6; i++) {
    if (HovaBlimp[i].ActiveMode != 0) {
      j++;
    }
  }
  return j;
}

//NGC MATCH
void InitGunBoat(struct nuvec_s *Pos,float AngleY,s32 Character) {
  GUNBOATSTRUCT* GunBoat;
  s32 CharacterId;

  GunBoat = &GunBoatList[Character];
  if (GunBoat->Active != 0) {
    return;
  }
  memset(&GunBoatList[Character],0,0x134);
  GunBoat->Character = Character;
  GunBoat->Position = *Pos;
  GunBoat->AngleY = AngleY;
  GunBoat->Action = 1;
  GunBoat->LastAction = -1;
  GunBoat->FireAngleX = 45.0;
  switch(Character) {
      case 0:
          CharacterId = 0x68;
      break;
      case 1:
          CharacterId = 0x69;
      break;
      case 2:
          CharacterId = 0x67;
      break;
      default:
          CharacterId = 0x66;
      break;
  }
  if (MyInitModelNew(&GunBoat->MainDraw,CharacterId,0x22,0,NULL,&GunBoat->Position) == 0) {
    GunBoat->Character = -1;
  }
  GunBoat->Active = 1;
}

//NGC MATCH
void InitGunBoats(void) {
  memset(&GunBoatList,0,0x4d0);
  InitGunBoat(SetNuVecPntr(-24.0f,0.0f,50.0f),0.0f,0);
  InitGunBoat(SetNuVecPntr(36.0f,0.0f,-15.0f),0.0f,1);
  InitGunBoat(SetNuVecPntr(93.0f,0.0f,43.0f),0.0f,2);
  InitGunBoat(SetNuVecPntr(21.0f,0.0f,106.0f),0.0f,3);
}

//NGC MATCH
void DrawGunBoat(GUNBOATSTRUCT *GunBoat) {
  struct nuvec_s ScaleVec = {1.5f, 1.5f, 1.5f};
  
  NuMtxSetScale(&mTEMP,&ScaleVec);
  NuMtxRotateX(&mTEMP,(int)(GunBoat->FireAngleX * 182.04445f));
  NuMtxRotateY(&mTEMP,(int)((GunBoat->AngleY + GunBoat->FireAngleY) * 182.04445f));
  NuMtxTranslate(&mTEMP,&GunBoat->Position);
  GunBoat->Seen = MyDrawModelNew(&GunBoat->MainDraw,&mTEMP,NULL);
}

//NGC MATCH
float GetGunBoatBestTarget(float Best,struct nuvec_s **TargetPos,struct nuvec_s **Vel,s32 *Moving) {
  struct nuvec_s CamDir;
  struct nuvec_s Rel;
  s32 i;
  float Mag;
  float Dot;
  
  NuVecMtxRotate(&CamDir,SetNuVecPntr(0.0f,0.0f,1.0f),&GameCam[0].m);
  for(i = 0; i < 4; i++) {
    if (GunBoatList[i].Active != 0) {
      if (GunBoatList[i].Action != 2) {
        NuVecSub(&Rel,&GunBoatList[i].Position,&GameCam[0].pos);
        Mag = NuVecMag(&Rel);
        if (Mag > 10.0f) {
          NuVecScale(&Rel,&Rel,(1.0f / Mag));
          Dot = DotProduct(&CamDir,&Rel);
          if (Dot > Best) {
            *TargetPos = &GunBoatList[i].Position;
            *Vel = &v000;
            *Moving = 0;
            Best = Dot;
          }
        }
      }
    }
  }
  return Best;
}

//NGC MATCH
void ProcessGunBoats(void) {
  s32 i;

  for(i = 0; i < 4; i++) {
    if (GunBoatList[i].Active != 0) {
      ProcessGunBoat(&GunBoatList[i]);
    }
  }
}

//NGC MATCH
void InitGliderBombs(void) {
  memset(&GliderBombs,0,0x2a8);
  memset(EnemyGliderBombs,0,0x7f8);
}

//NGC MATCH
void InitBigGun(struct nuvec_s *Pos,float AngleY,int Type,float MinX,float MaxX,float MainY,float DeviationY) {
  BIGGUNSTRUCT *BigGun;
  struct nuvec_s vec;
    
  BigGun = &BigGunList[BigGunIndx++];
  if (BigGun->Active != 0) {
    return;
  }
  memset(BigGun,0,0x154);
  BigGun->FireAngMainY = MainY;
  BigGun->FireAngMinX = MinX;
  BigGun->FireAngMaxX = MaxX;
  BigGun->FireAngDeviationY = DeviationY;
  BigGun->Position = *Pos;
  BigGun->Action = 1;
  BigGun->LastAction = -1;
  BigGun->AngleY = BigGun->FireAngMainY;
  BigGun->FireAngleX = 0.0f;
  BigGun->FireAngleY = 0.0f;
  BigGun->Type = Type;
  NuVecSub(&vec,&BigGun->Position,SetNuVecPntr(0.0f,7.75f,11.17f));
  NuVecNorm(&vec,&vec);
  NuVecScale(&vec,&vec,1.5);
  NuVecAdd(&BigGun->TerrPos,&BigGun->Position,&vec);
  switch(Type) {
      case 0:
        BigGun->HitPoints = 6;
        BigGun->ExplosionEffect = 0x36;
      break;
      case 1:
        BigGun->HitPoints = 2;
        BigGun->ExplosionEffect = 0x37;
      break;
      case 2:
        BigGun->HitPoints = Type;
        BigGun->ExplosionEffect = 0x38;
      break;
      case 3:
        BigGun->HitPoints = 6;
        BigGun->ExplosionEffect = 0x39;
      break;
      case 4:
        BigGun->HitPoints = Type;
        BigGun->ExplosionEffect = 0x3a;
      break;
      case 5:
        BigGun->HitPoints = 4;
        BigGun->ExplosionEffect = 0x3b;
      break;
  }
  BigGun->Active = 1;
}

//NGC MATCH
void InitBigGuns(void) {
  memset(BigGunList,0,0xff0);
  BigGunIndx = 0;
  InitBigGun(SetNuVecPntr(0.0,18.86,30.58),0.0,0,-65.0,-10.0,0.0,60.0);
  InitBigGun(SetNuVecPntr(0.0,-1.68,16.72),0.0,4,10.0,80.0,0.0,60.0);
  InitBigGun(SetNuVecPntr(-10.83,6.32,34.15),0.0,5,-65.0,-10.0,-90.0,60.0);
  InitBigGun(SetNuVecPntr(10.83,6.32,34.15),0.0,5,-65.0,-10.0,90.0,60.0);
  InitBigGun(SetNuVecPntr(0.03,23.71,2.48),0.0,0,-65.0,-10.0,180.0,60.0);
  InitBigGun(SetNuVecPntr(0.0,-25.5,-5.97),0.0,3,-30.0,30.0,180.0,60.0);
  InitBigGun(SetNuVecPntr(10.0,9.89,14.46),0.0,1,-30.0,30.0,90.0,30.0);
  InitBigGun(SetNuVecPntr(9.5,9.89,11.25),0.0,1,-30.0,30.0,90.0,30.0);
  InitBigGun(SetNuVecPntr(9.0,9.89,7.84),0.0,1,-30.0,30.0,90.0,30.0);
  InitBigGun(SetNuVecPntr(-10.0,9.89,14.46),0.0,2,-30.0,30.0,-90.0,30.0);
  InitBigGun(SetNuVecPntr(-9.5,9.89,11.25),0.0,2,-30.0,30.0,-90.0,30.0);
  InitBigGun(SetNuVecPntr(-9.0,9.89,7.84),0.0,2,-30.0,30.0,-90.0,30.0);
}

//NGC MATCH
void DrawBigGun(BIGGUNSTRUCT *BigGun) {
  struct nuvec_s ScaleVec = {1.5f, 1.5, 1.5f};
    
  NuMtxSetScale(&mTEMP,&ScaleVec);
  NuMtxRotateX(&mTEMP,(int)(BigGun->FireAngleX * 182.04445f));
  NuMtxRotateY(&mTEMP,(int)((BigGun->AngleY + BigGun->FireAngleY) * 182.04445f));
  NuMtxTranslate(&mTEMP,&BigGun->Position);
}

//NGC MATCH
void ProcessBigGuns(void) {
  s32 i;

  for(i = 0; i < 0xc; i++) {
    if (BigGunList[i].Active != 0) {
      ProcessBigGun(&BigGunList[i]);
    }
  }
}

//NGC MATCH
void DrawBigGuns(void) {
  s32 i;
  
  for(i = 0; i < 0xc; i++) {
    if (BigGunList[i].Active != 0) {
      DrawBigGun(&BigGunList[i]);
    }
  }
}

//NGC MATCH
s32 CollideWithBattleShip(struct nuvec_s *ObjPos,BATTLESHIPSTRUCT *Ship,float Rad) {
  struct nuvec_s* Scale;
  struct nuvec_s* Pos;
  float Max;
  float NegMax;
  struct nuvec_s Rel;
  
  Scale = &BattleShipCollScale;
  Pos = &Ship->Position;
  if (Scale->x > Scale->y) {
    Max = Scale->x;
  } else {
    Max = Scale->y;
  }
  if (Scale->z > Max) {
    Max = Scale->z;
  }
  NegMax = -Max;
  Rel.x = ObjPos->x - Pos->x;
  if ((Rel.x <= Max) && (Rel.x >= NegMax)) {
      Rel.y = ObjPos->y - Pos->y;
      if ((Rel.y <= Max) && (Rel.y >= NegMax)) {
          Rel.z = ObjPos->z - Pos->z;
          if ((Rel.z <= Max) && (Rel.z >= NegMax)) {
            NuVecRotateY(&Rel,&Rel,(s32)(-Ship->AngleY * 182.04445f));
              Rel.x /= Scale->x;
              Rel.y /= Scale->y;
              Rel.z /= Scale->z;
              if (Rel.x * Rel.x + Rel.y * Rel.y + Rel.z * Rel.z <= 1.0f)  {
                if (InvincibilityCHEAT != 0) {
                  Ship->KillMeNow = 1;
                }
                return 1;
              }
        }
      }
  }
  return 0;
}

//NGC MATCH
s32 CollideWithBattleShips(struct nuvec_s *ObjPos,float Rad) {
  s32 i;

  for (i = 0; i < 6; i++) {
    if ((BattleShipList[i].Active != 0 && (CollideWithBattleShip(ObjPos,&BattleShipList[i],Rad) != 0))) {
        return 1;
    }
  }
  return 0;
}

//NGC MATCH
void InitBattleShip(struct nuvec_s *Pos,float AngleY,int Character) {
  BATTLESHIPSTRUCT * BattleShip;
  
  BattleShip = &BattleShipList[Character];
  if (BattleShip->Active == 0) {
    memset(&BattleShipList[Character],0,0x554);
    if (MyInitModelNew(&BattleShip->MainDraw,0x65,0x22,0,NULL,&BattleShip->Position) != 0) {
      BattleShip->Position = *Pos;
      BattleShip->DestY = Pos->y;
      BattleShip->AngleY = AngleY;
      BattleShip->TiltX = 0.0f;
      BattleShip->TiltZ = 0.0f;
      BattleShip->DestTiltX = 0.0f;
      BattleShip->DestTiltZ = 0.0f;
      BattleShip->Seek = 1.0f;
      BattleShip->Active = 1;
      BattleShip->HitPoints = 3;
      BattleShip->FireTimer[0] = frand() * 4.0f;
      BattleShip->FireTimer[1] = (frand() * 4.0f);
      BattleShip->GooScale = SetNuVec(0.0f,0.0f,0.0f);
      BattleShip->GooTimer.x = (float)(Character) / 6.0f;
      BattleShip->GooTimer.y = ((float)(Character + 1) / 6.0f);
      BattleShip->GooSpeed = (BattleShip->GooTimer.x * 0.1f + 0.2f) * 0.01666667f;
      if (1.0f <= BattleShip->GooTimer.y) {
        BattleShip->GooTimer.y = (BattleShip->GooTimer.y - 1.0f);
      }
      BattleShip->GooTimer.z = (float)(Character + 2) / 6.0f;
      if (1.0f <= BattleShip->GooTimer.z) {
        BattleShip->GooTimer.z = (BattleShip->GooTimer.z - 1.0f);
      }
    }
  }
}

//NGC MATCH
void InitBattleShips(void) {
  memset(BattleShipList,0,0x1ff8);
  InitBattleShip(SetNuVecPntr(-7.0f,0.0f,15.0f),-150.0f,0);
  InitBattleShip(SetNuVecPntr(-7.33,0.0f,73.5f),-45.0f,1);
  InitBattleShip(SetNuVecPntr(31.0f,0.0f,109.0f),-35.0f,2);
  InitBattleShip(SetNuVecPntr(94.0f,0.0f,54.0f),90.0f,3);
  InitBattleShip(SetNuVecPntr(60.0f,0.0f,-6.0f),160.0f,4);
  InitBattleShip(SetNuVecPntr(20.0f,0.0f,-3.0f),-150.0f,5);
}

//NGC MATCH
void DrawBattleShip(BATTLESHIPSTRUCT *BattleShip) {
  NuMtxSetRotationX(&mTEMP,(int)(BattleShip->TiltX * 182.04445f));
  NuMtxRotateZ(&mTEMP,(int)(BattleShip->TiltZ * 182.04445f));
  NuMtxRotateY(&mTEMP,(int)(BattleShip->AngleY * 182.04445f));
  NuMtxTranslate(&mTEMP,&BattleShip->Position);
  BattleShip->Seen = MyDrawModelNew(&BattleShip->MainDraw,&mTEMP,BattleShip->Locator);
}

//NGC MATCH
float GetBattleShipBestTarget(float Best,struct nuvec_s **TargetPos,struct nuvec_s **Vel,s32 *Moving) {
  struct nuvec_s CamDir;
  struct nuvec_s Rel;
  s32 i;
  float Dot;
  float Mag;
  
  NuVecMtxRotate(&CamDir,SetNuVecPntr(0.0f,0.0f,1.0f),&GameCam[0].m);
  for(i = 0; i < 6; i++) {
    if (BattleShipList[i].Active != 0) {
      if (BattleShipList[i].Seen > 0) {
        NuVecSub(&Rel,&BattleShipList[i].Position,&GameCam[0].pos);
        Mag = NuVecMag(&Rel);
        if (Mag > 10.0f) {
          NuVecScale(&Rel,&Rel,(1.0f / Mag));
          Dot = DotProduct(&CamDir,&Rel);
          if (Dot > Best) {
            *TargetPos = &BattleShipList[i].Position;
            *Vel = &v000;
            *Moving = 0;
            Best = Dot;
          }
        }
      }
    }
  }
  return Best;
}

//NGC MATCH
void ProcessBattleShips(void) {
  s32 i;

  for(i = 0; i < 6; i++) {
    if (BattleShipList[i].Active != 0) {
      ProcessBattleShip(&BattleShipList[i]);
    }
  }
}

//NGC MATCH
struct nuvec_s * GetWeatherBossPos(void) {
  return &WeatherBoss.Position;
}

//NGC MATCH
void InitWeatherBoss_a(void) {
  BOSSSTRUCT * Boss;
  struct nuvec_s Start;
  u16 StartAngle;

  Boss = &WeatherBoss;
  memset(&WeatherBoss,0,0x65c);
  if ((MyInitModelNew(&Boss->MainDraw,0x86,0x32,0,NULL,&Boss->Position) != 0) &&
     (MyInitModelNew(&Boss->BonesDraw,0xbe,0xe,0,NULL,&Boss->Position) != 0)) {
    BazookaIconOn = 0;
    Boss->Active = 1;
    Boss->ChestSoundBTimer = 100000.0f;
    Boss->HitPoints[0] = 100;
    Boss->HitPoints[1] = 100;
    Boss->HitPoints[2] = 100;
    Boss->HitPoints[3] = 100;
    Boss->Action = 0;
    Boss->OldAction = -1;
    Boss->MainSpline.Spline = SplTab[0x19].spl;
    if (SplTab[0x19].spl != NULL) {
      Boss->MainSpline.Cur = 0.0f;
      Boss->MainSpline.Inc = 0.002f;
      Boss->MainSpline.Nex = 0.0f;
      PointAlongSpline(SplTab[0x19].spl,0.0f,&Start,&StartAngle,NULL);
      Boss->AngleY = StartAngle / 182.04445f;
      Boss->MainSpline.CurPos = Start;
      Boss->MainSpline.NexPos = Start;
      Boss->MainSpline.LookaheadDist = 128.0f;
      Boss->Position = Start;
      Boss->BaseAngleY = Boss->AngleY;
      Boss->DestAngleY = Boss->AngleY;
      Boss->Position = SetNuVec(0.0f,37.0f,0.0f);
      Boss->Active = 1;
      Boss->PossYDest = Boss->Position.y;
      CamOveride = &Boss->Position;
      Boss->Distance = -96.0f;
      Boss->DistanceDest = -96.0f;
    }
  }
}

//NGC MATCH
void InitWeatherBoss(void) {
  InitWeatherBoss_a();
}

//NGC MATCH
void DrawBazookaToken(void) {
  if (BazookaIconOn != 0) {
    NuMtxSetTranslation(&mTEMP,&BazookaTokenCurrentPos);
    MyDrawModelNew(&IconMainDraw,&mTEMP,NULL);
  }
}

//NGC MATCH
void ProcessWeatherBoss(void) {
  if (WeatherBoss.Active != 0) {
    ProcessWeatherBoss_a(&WeatherBoss);
  }
}

//NGC MATCH
void DrawWeatherBoss_a(BOSSSTRUCT *Boss) {
  struct nuvec_s Pos;
  
  Pos = (Boss->Position);
  NuMtxSetScale(&mTEMP,SetNuVecPntr(WBSCALE,WBSCALE,WBSCALE));
  NuMtxRotateY(&mTEMP,(Boss->AngleY * 182.04445f));
  NuMtxTranslate(&mTEMP,&Pos);
  if (Boss->Action == 5) {
      Boss->Seen = MyDrawModelNew(&Boss->BonesDraw,&mTEMP,Boss->Locator);
  } else {
       Boss->Seen = MyDrawModelNew(&Boss->MainDraw,&mTEMP,Boss->Locator);
  }
}

//NGC MATCH
void DrawWeatherBoss(void) {
  if (WeatherBoss.Active != 0) {
    DrawWeatherBoss_a(&WeatherBoss);
  }
}

//NGC MATCH
void DrawGliderTarget(void) {
  struct nuvec_s Centre;
  struct nuvec_s nStack_20;
  s32 Obj = 0x72;
  
  NuVecSub(&nStack_20,&GliderTargetPos,&GameCam[0].pos);
  NuVecScale(&Centre,&nStack_20,5.0f / NuVecMag(&nStack_20));
  NuVecAdd(&Centre,&Centre,(struct nuvec_s *)&GameCam[0].m._30);
  if (ObjTab[Obj].obj.special != NULL) {
    mTEMP = GameCam[0].m;
    NuMtxPreScale(&mTEMP,SetNuVecPntr(0.2f,0.2f,0.0f));
    *(struct nuvec_s *)&mTEMP._30 = Centre;
    NuRndrGScnObj((ObjTab[Obj].obj.scene)->gobjs[(ObjTab[Obj].obj.special)->instance->objid],&mTEMP);
  }
}

//NGC MATCH
void DrawFireFlyLevelExtra(void) {
  s32 i;

  for(i = 0; i < 4; i++) {
    if (GunBoatList[i].Active != 0) {
      DrawGunBoat(&GunBoatList[i]);
    }
  }
  for(i = 0; i < 6; i++) {
    if (BattleShipList[i].Active != 0) {
      DrawBattleShip(&BattleShipList[i]);
    }
  }
  DrawGliderBombs();
  DrawGliderBullets();
  DrawZoffaUFOs();
  DrawTorpedoTarget();
}

//NGC MATCH
void ProcessCrashteroidsIntro(void) {
    ProcessFireFlyIntro();
}

void ProcessFireFlyLevel(struct nupad_s *Pad) {
  s32 i;
  
  TeleportManager();
  LastNumZoffasFiring = NumZoffasFiring;
  NumZoffasFiring = 0;
  for(i = 0; i < 4; i++) {
    if (EnemyZoffa[i].ActiveMode == 0) {
          if (PlayerGlider.AutoPilot == 0) {
                EnemyZoffa[i].RespawnTimer -= 0.016666668f;
                if (EnemyZoffa[i].RespawnTimer < 0.0f) {
                  EnemyZoffa[i].RespawnTimer = 0;
                }
          }
    }
    else {
      MoveZoffaUFO(&EnemyZoffa[i]);
    }
  }
  ZoffaUFORespawn();
  CheckGliderCollisions();
  CheckForPotentialMidAirCollisions();
  ProcessGliderBombs();
  ProcessBattleShips();
  ProcessGunBoats();
  ProcessGliderBullets();
  GliderFrameCounter++;
}

//NGC MATCH
s32 GetCurrentFireFlyObjectives(void) {
  s32 i;
  s32 j;
  
  for(i = 0, j = 0; i < 6; i++) {
    if ((BattleShipList[i].Active != 0) && (0 < BattleShipList[i].HitPoints)) {
      j++;
    }
  }
  return j;
}

//NGC MATCH
void WeatherResearchReset(void) {
  struct nuvec_s Start;
  
  Start = WRStartPos;
  LevelResetTimer = 0;
  Level_GliderSpeed = 16.0f;
  Level_GliderCeiling = 60.0f;
  Level_GliderFloor = -60.0f;
  Level_GliderCurrentCeiling = 60.0f;
  Level_GliderRadius = 100.0f;
  Level_GliderCentreX = 0.32f;
  Level_GliderCentreZ = 0.09f;
  Level_DeadTime = 8.0f;
  Level_TargetTime = 0.7f;
  InitGlider(&PlayerGlider,&Start,WRStartAng);
  GameCam[0].m._30 = Start.x + 0.0f;
  GameCam[0].m._31 = Start.y + 2.0f;
  GameCam[0].m._32 = Start.z + 5.0f;
  GliderCamHighTimer = 0.0f;
  ResetGameCameras(GameCam,1);
  InitGliderBullets();
  InitBigGuns();
  InitGliderBombs();
}

//NGC MATCH
void DrawWeatherResearchLevelExtra(void) {
    DrawGliderBullets();
    DrawBigGuns();
    DrawGliderBombs();
    DrawTorpedoTarget();
}

//NGC MATCH
void ProcessWeatherResearchLevel(struct nupad_s *Pad) {
    ProcessGliderBullets();
    ProcessGliderBombs();
    ProcessBigGuns();
}

//NGC MATCH
s32 GetCurrentWeatherResearchObjectives(void) {
  s32 i;
  s32 j;
  
  for(i = 0, j = 0; i < 0xc; i++) {
    if ((BigGunList[i].Active != 0) && (BigGunList[i].Action != 2)) {
      j++;
    }
  }
  return j;
}

//NGC MATCH
s32 GetCurrentSpaceArenaObjectives(void) {
  s32 j;
  s32 i;
  
  for(i = 0, j = 0; i < 3; i++) {
    if (SpaceStationList[i].Active != 0) {
      j++;
    }
  }
  return j;
}

//NGC MATCH
s32 CollidePlayerPoint(struct nuvec_s *Pos,float Rad2,s32 Hits) {
  s32 OldHits;
  float TimerInc;
  
  OldHits = PlayerGlider.HitPoints;
  if (NuVecDistSqr(Pos,&PlayerGlider.Position,NULL) <= Rad2) {
     TimerInc = (float)Hits * 0.125f;
    if ((InvincibilityCHEAT == 0) && (VehicleLevelImmune == 0)) {
      PlayerGlider.HitPoints = PlayerGlider.HitPoints - Hits;
    }
    PlayerGlider.HitTimer += TimerInc;
    if (Hits != 0) {
      NewBuzz(&player->rumble,(BEENHITBIGBUZZTIME * 0x19) / Hits);
      NewRumble(&player->rumble,(BEENHITBIGRUMTIME * 0x19) / Hits);
    }
    if (4.5f <= PlayerGlider.HitTimer) {
      PlayerGlider.HitTimer = 4.5f;
    }
    if ((PlayerGlider.NextHitSoundTimer == 0.0f) ||
       (((PlayerGlider.HitPoints < 1 && (OldHits > 0)) || (4 < Hits)))) {
      MyGameSfx(0x5a,&PlayerGlider.Position,0x7fff);
      PlayerGlider.NextHitSoundTimer = GLIDERHITSOUNDFREQUENCY;
    }
    else {
      MyGameSfx(GLIDERHITSOUNDOTHERID,&PlayerGlider.Position,0x4fff);
    }
    if (PlayerGlider.HitPoints < 1) {
      PlayerGlider.HitPoints = 0;
      PlayerGlider.TerminalDive = 1;
      PlayerGlider.HitTimer = Level_DeadTime;
    }
    return 1;
  }
  return 0;
}

//NGC MATCH
void InitLighteningHail(void) {
  memset(&HailList,0,0x438);
  NuMtxSetScale(&BoltMtxC,SetNuVecPntr(0.5f,1.5f,0.5f));
}

//NGC MATCH
void DrawLighteningHail(void) {
  s32 i;
  LIGHTENINGHAIL *Hail;
  
  if (FlyingLevelVictoryDance != 0) {
      return;
  }
  Hail = HailList;
  for(i = 0; i < 0x1e; i++, Hail++) {
    if (Hail->Mode != 0) {
       if (Hail->Mode == 1) {
         if (Paused == 0) {
           AddVariableShotDebrisEffect(GDeb[0x93].i,&Hail->Position,1,0,0);
         }
       }
       else if (Hail->Mode == 3) {
         if (Paused == 0) {
           AddVariableShotDebrisEffect(GDeb[0x93].i,&Hail->Position,1,0,0);
         }
       }
    }
  }
}

//NGC MATCH
struct WBBOLT * FindFreeWBBoltOfType(s32 Type) {
  s32 i;
  struct WBBOLT *Bolt;
  
  Bolt = BoltList;
  for (i = 0; i < 0x78; i++, Bolt++) {
    if ((Bolt->Mode != 0 && (Bolt->Type == Type))) {
      return Bolt;
    }
  }
  return NULL;
}

//NGC MATCH
struct WBBOLT * FindFreeWBBolt(void) {
  s32 i;
  struct WBBOLT *Bolt;
  
  Bolt = BoltList;
  for(i = 0; i < 0x78; i++, Bolt++) {
    if (Bolt->Mode == 0) {
      return Bolt;
    }
  }
  return NULL;
}

//NGC MATCH
void SetWeatherStartPos(struct creature_s *Cre) {
  PlayerGlider.Position = WeatherStartPos = Cre->obj.pos;
}

//NGC MATCH
void InitWBIntro(void) {
  short Temps;
  
  WBIntroOn = 0;
  WeatherBossIntroSpline.Spline = SplTab[0x48].spl;
  if (SplTab[0x48].spl != NULL) {
    WeatherBossIntroSpline.Cur = 0.0f;
    WeatherBossIntroSpline.Inc = 0.000019999999f;
    WeatherBossIntroSpline.Nex = 0.0f;
    PointAlongSpline(WeatherBossIntroSpline.Spline,0.0f,&WBIntroGliderPos,&Temps,NULL);
    WeatherBossIntroSpline.NexPos = WeatherBossIntroSpline.CurPos = WBIntroGliderPos;
    WeatherBossIntroSpline.LookaheadDist = 0.0f;
    WeatherBoss.Distance = WBBOSSINTRODIST;
    WeatherBossTargetAppearTimer = WeatherBossTargetAppearTime;
    WBIntroTweenTimer = WBIntroTweenTime;
    WeatherBoss.DistanceDest = WBBOSSINTRODIST;
    WBIntroOn = 1;
  }
}

//NGC MATCH
void DrawWeatherBossLevelExtra(void) {
  DrawGliderBullets();
  DrawWeatherBoss();
  DrawWeatherBossTarget();
  DrawLighteningHail();
  DrawWBBolts();
  DrawVehMasks();
  DrawBazookaToken();
}

//NGC MATCH
void ProcessWeatherBossLevel(struct nupad_s *Pad) {
  ProcessWBIntro();
  if (WBIntroOn == 0) {
    AtmosphericPressureHackedZ -= 1.6f;
    if (AtmosphericPressureHackedZ < -1200.0f) {
      AtmosphericPressureHackedZ += 1200.0f;
    }
  }
  else {
    AtmosphericPressureHackedZ = WBIntroGliderPos.z;
    if (WBIntroGliderPos.z < -1200.0f) {
      AtmosphericPressureHackedZ += 1200.0f;
    }
    if (AtmosphericPressureHackedZ > 0.0f) {
      AtmosphericPressureHackedZ -= 1200.0f;
    }
  }
  ProcessGliderBullets();
  ProcessWeatherBoss();
  ProcessLighteningHail();
  ProcessWBBolts();
  ProcessVehMasks();
  ProcessBazookaToken();
}

//NGC MATCH
s32 GetCurrentWeatherBossObjectives(void) {
  s32 j;
  s32 i;
  
  if (WeatherBoss.Active != 0) {
    for(i = 0, j = 0; i < 4; i++) {
      if (WeatherBoss.HitPoints[i] > 0) {
        j += WeatherBoss.HitPoints[i];
      }
    }
    i = j + 3;
    if (i < 0) {
      i = j + 6;
    }
    return i >> 2;
  }
  return 0;
}

//NGC MATCH
void ProcessVehicleLevel(struct nupad_s *Pad) {
    switch (Level) {
    case 13:
        ProcessFarmLevel(Pad);
        break;
    case 18:
        ProcessFireFlyLevel(Pad);
        break;
    case 36:
        ProcessWeatherResearchLevel(Pad);
        break;
    case 24:
        ProcessWeatherBossLevel(Pad);
        break;
    case 26:
        ProcessSpaceArenaLevel(Pad);
        break;
    case 3:
        ProcessWesternArenaLevel(Pad);
        break;
    case 21:
        ProcessEarthBossLevel(Pad);
        break;
    case 22:
        ProcessFireBossLevel(Pad);
        break;
    }
   switch(Level) {
        case 0x18:
        case 0xD:
        case 0x12:
        case 0x1A:
        case 0x24:
          if (GetCurrentLevelObjectives() == 0) {
            VehicleLevelImmune = 1;
            FlyingLevelExtro = 1;
            ProcessTimer(&FlyingLevelCompleteTimer);
          }
        break;
    }
    return;
}

//NGC MATCH
void EarthBossReset(void) {
  EarthBossDeathTimer = 0;
  EarthBossJustEntered = 0;
  EarthBossDeathEffect = 0;
  EarthBossVortexOpen = 0;
  ChrisBigBossDead = 0;
  RumbleDisplayMode = 0;
  InitVehMasks();
  InitVehMask(0,0x55);
  InitVehMask(1,3);
  InitEarthBoss();
  InitJeepRocks();
  InitRumblePanel();
}

//NGC MATCH
void ResetVehicleLevel(s32 PlayerDead) {
  FlyingLevelCompleteTimer = 1.5f;
  FireFlyIntroOldAction = -1;
  VehicleLevelImmune = 0;
  FlyingLevelExtro = 0;
  FlyingLevelVictoryDance = 0;
  FlyingLevelVictoryDanceTimer = 0;
  FireFlyIntroTween = 0.0f;
  FireFlyIntroAction = 0;
  FireFlyIntroOn = 0;
  GliderIntroInterest = v000;
  GliderIntroCamPos = v000;
    switch (Level) {
    case 13:
        FarmReset(PlayerDead);
        break;
    case 18:
        FireFlyReset(PlayerDead);
        break;
    case 36:
        WeatherResearchReset(PlayerDead);
        break;
    case 26:
        SpaceArenaReset(PlayerDead);
        break;
    case 24:
        WeatherBossReset(PlayerDead);
        break;
    case 3:
        WesternArenaReset(PlayerDead);
        break;
    case 22:
        FireBossReset(PlayerDead);
        break;
    case 21:
        EarthBossReset(PlayerDead);
        break;
    }
  if (Level == 0x12) {
    ProcessFireFlyIntro();
  }
  if (Level == 0x1a) {
    ProcessCrashteroidsIntro();
  }
}

//NGC MATCH
s32 GetMaxLevelObjectives(void) {
    switch (Level) {
    case 18:
    case 13:
        return 6;
    case 24:
        return 0x64;
    case 36:
        return 0xC;
    case 22:
        return GetTotalFireBossObjectives();
    case 26:
    case 21:
        return 3;
    case 25:
        return GetTotalSpaceBossObjectives();
    default:
        return 1;
    }
}

//NGC MATCH
s32 GetCurrentLevelObjectives(void) {
    switch (Level) {
    case 13:
        return GetCurrentFarmObjectives();
    case 18:
        return GetCurrentFireFlyObjectives();
    case 26:
        return GetCurrentSpaceArenaObjectives();
    case 24:
        return GetCurrentWeatherBossObjectives();
    case 36:
        return GetCurrentWeatherResearchObjectives();
    case 22:
        return GetCurrentFireBossObjectives();
    case 21:
        return GetCurrentRumbleObjectives();
    case 25:
        return GetCurrentSpaceBossObjectives();
    default:
        return 0;
    }
}

//NGC MATCH
void UnembedRayCastAtlas(struct ATLASSTRUCT *Atlas,short *TerrHandle) {
  struct nuvec_s Up = {0.0f, 1.0f, 0.0f};

  if (TryUnembeddPointDir(&Atlas->Position,&ShadNorm,&Up,Atlas->Radius,TerrHandle) == 0) {
    TryUnembeddPointSafe(&Atlas->Position,&Atlas->OldPosition,Atlas->Radius,TerrHandle);
  }
}

//NGC MATCH
s32 UnembedRayCastAtlasSimple(struct ATLASSTRUCT *Atlas,short *TerrHandle) {
  struct nuvec_s Up = {0.0f, 1.0f, 0.0f};
    
  return TryUnembeddPointDirSimple(&Atlas->Position,&Up,Atlas->Radius,TerrHandle,0.03f,5);
}

//NGC MATCH
void ObjectToAtlas(struct obj_s *obj,struct creature_s *c) {
  struct ATLASSTRUCT *atlas;
  
  atlas = (struct ATLASSTRUCT *)c->Buggy;
  atlas->Position.x = obj->pos.x;
  atlas->Position.y = obj->pos.y + CData[0x53].radius;
  atlas->Position.z = obj->pos.z;
  if (temp_xzmomset != 0) {
    atlas->Velocity.x = obj->mom.x / 0.01666667f;
    atlas->Velocity.z = obj->mom.z / 0.01666667f;
  }
  if (obj->boing != 0) {
    obj->mom.y = 3.333333f;
  }
  atlas->Velocity.y = obj->mom.y;
}

//NGC MATCH
void DrawEarthBoss(void) {
  struct nuvec_s Scale;
  float Temp;
  
  if (EarthBoss.Dead == 0) {
    DrawJonny();
    if (EarthBoss.DrawCrunch != 0) {
      NuMtxSetRotationY(&mTEMP,(int)(EarthBoss.CrunchY * 182.0444f));
      NuMtxTranslate(&mTEMP,&EarthBoss.Position);
      mTEMP._31 = mTEMP._31 - EarthBoss.Radius;
      MyDrawModelNew(&EarthBoss.Crunch,&mTEMP,NULL);
    }
    if (EarthBoss.DrawShell != 0) {
      Temp = (EarthBoss.Radius / 0.65f) * 2.56f;
      Scale = SetNuVec(Temp,Temp,Temp);
      NuQuatToMtx(&EarthBoss.Quat,&mTEMP);
      NuMtxScale(&mTEMP,&Scale);
      NuMtxTranslate(&mTEMP,&EarthBoss.Position);
      MyDrawModelNew(&EarthBoss.Shell,&mTEMP,NULL);
    }
  }
  return;
}

//NGC MATCH
void UpdateRumbleCamTween(void) {
  if (RumbleCamTweenDest > RumbleCamTween) {
    RumbleCamTween += 0.01666667f;
    if (RumbleCamTweenDest < RumbleCamTween) {
        RumbleCamTween = RumbleCamTweenDest;
    }
  }
  if (RumbleCamTweenDest < RumbleCamTween) {
    RumbleCamTween -= 0.01666667f;
    if (RumbleCamTweenDest > RumbleCamTween) {
        RumbleCamTween = RumbleCamTweenDest;
    }
  }
  if ((EarthBoss.Action != 7) || (EarthBoss.HitPoints > 0)) {
    RumbleCamTweenInterest = (RumbleCamTween - 0.25f) * RUMZOOM;
  }
  if (RumbleCamTweenInterest < 0.0f) {
    RumbleCamTweenInterest = 0.0f;
  }
}

//NGC MATCH
void ProcessAtlasAtlasCollisions(void) {
  if ((EarthBoss.HitPoints > 0) && (PlayerAtlas.HitPoints > 0)) {
    ProcessAtlasAtlasCollisions_a(&PlayerAtlas,&EarthBoss);
  }
}

//NGC MATCH
void InitTrail(void) {
  s32 loop;

  for(loop = 0; loop < 0x80; loop++) {
    trail[loop].pos1.x = -10000.0f;
  }
  trailpt = 0;
  trailair = 0;
}

//NGC MATCH
s32 PointsSame(struct nuvec_s *A,struct nuvec_s *B) {
  s32 ret = 0;

  if ((A->x == B->x) && (A->y == B->y)) {
    ret = (A->z == B->z) ? 1 : 0;
  }
    return ret;
}

//NGC MATCH
float PointLineSide(struct nuvec_s *A, struct nuvec_s *B, struct nuvec_s *C) 
{
    float ACx;
    float ACz;
    float BCx;
    float BCz;
    
    ACx = C->x - A->x;
    BCx = C->x - B->x;
    BCz = C->z - A->z;
    ACz = C->z - B->z;
    
    return ACx * ACz - BCz * BCx;
}

//NGC MATCH
s32 FindTrailAng(struct nuvec_s *A,struct nuvec_s *B) {
  struct nuvec_s Line;
  
  NuVecSub(&Line,B,A);
  return (NuAtan2D(Line.x,Line.z) - 0x2000) & 0xffff;
}

//NGC MATCH
void DrawVehicleTrail(void) {
    DrawJeepTrails();
}

//NGC MATCH
struct JEEPROCK * FindJeepRock(void) {
  s32 i;
  
  for(i = 0; i < 6; i++) {
    if (JeepRock[i].Active == 0) {
      return &JeepRock[i];
    }
  }
  return NULL;
}

//NGC MATCH
void InitJeepRocks(void) {
  s32 i;

  for(i = 0; i < 6; i++) {
    JeepRock[i].Active = 0;
  }
}

//NGC MATCH
void KeepHoldOnRock(struct JEEPROCK *Rock,struct nuvec_s *Pos,struct nuvec_s *Vel) {
  Rock->Pos = *Pos;
  Rock->Atlas.Position = *Pos;
  Rock->Atlas.OldPosition = Rock->Atlas.Position;
  Rock->Vel = *Vel;
  Rock->Atlas.Velocity = *Vel;
  Rock->Grabbed = 1;
}

//NGC MATCH
struct JEEPROCK * AddRock(struct nuvec_s *Pos,float Radius,s32 Type) {
  return AddRockVel(Pos,&v000,Radius,Type);
}

//NGC MATCH
void ShootRoksSkyward(void) {
  s32 i;
  
  RumbleDisplayMode = -1;
  MyGameSfx(0xb9,&EarthBoss.Position,0x7fff);
  ShootRockSound = 1;
  for(i = 0; i < 6; i++) {
    if (JeepRock[i].Active != 0) {
      JeepRock[i].Atlas.Velocity = SetNuVec(0.0f,13.0f,0.0f);
      JeepRock[i].Mode = 0x14;
      JeepRock[i].Atlas.BeenHit = 0;
      JeepRock[i].FlameTimer = 10.0f;
    }
  }
}

//NGC MATCH
s32 GetRumbleTotalRoks(void) {
  s32 i;
  s32 j;
  
  if (RumbleDisplayMode == -1) {
    return RumbleStoreTotalRoks;
  }
  for(i = 0, j = 0; i < 6; i++) {
    if (JeepRock[i].Active != 0) {
      j++;
    }
  }
  RumbleStoreTotalRoks = j;
  return RumbleStoreTotalRoks;
}

//NGC MATCH
s32 GetRumbleCrunchRoks(void) {
  s32 i;
  s32 j;
  
  if (RumbleDisplayMode == -1) {
    return RumbleStoreCrunchRoks;
  }
  for(i = 0, j = 0; i < 6; i++) {
    if (((JeepRock[i].Active != 0) && (0.0f < JeepRock[i].FlameTimer)) && (JeepRock[i].Mode == -1)) {
      j++;
    }
  }
  RumbleStoreCrunchRoks = j;
  return RumbleStoreCrunchRoks;
}

//NGC MATCH
s32 GetRumblePlayerRoks(void) {
  s32 i;
  s32 j;
  
  if (RumbleDisplayMode == -1) {
    return RumbleStoreCrashRoks;
  }
  for(i = 0, j = 0; i < 6; i++) {
    if (((JeepRock[i].Active != 0) && (JeepRock[i].FlameTimer > 0.0f)) && (JeepRock[i].Mode == 1)) {
      j++;
    }
  }
  RumbleStoreCrashRoks = j;
  return RumbleStoreCrashRoks;
}

//NGC MATCH
void SmashRockIntoTwo(struct JEEPROCK *Rock) {
  struct nuvec_s vel;
  struct nuvec_s pos;
  float NewRad;
  
  NewRad = Rock->Atlas.Radius / 1.2599f;
  vel = Rock->Vel;
  pos = Rock->Pos;
  pos.x -= (NewRad * 0.5f);
  vel.x -= 2.0f;
  AddRockVel(&pos,&vel,NewRad,0);
  pos.x = (NewRad * 1.01f + pos.x);
  vel.x += 4.0f;
  AddRockVel(&pos,&vel,NewRad,0);
  Rock->Explode = 1;
}

//NGC MATCH
void DrawJeepRocks(void) {
  s32 i;
  
  for(i = 0; i < 6; i++) {
    if (JeepRock[i].Active != 0) {
      DrawJeepRock(&JeepRock[i]);
    }
  }
}

//NGC MATCH
s32 CheckAgainstRocks(struct nuvec_s *Position,struct nuvec_s *Move) {
  struct nuvec_s Rel;
  struct nuvec_s Pos;
  s32 i;
  s32 Ret;
  
  Ret = 0;
  if (Level != 0x16) {
    return 0;
  }
  NuVecAdd(&Pos,Position,Move);
  for(i = 0; i < 6; i++) {
    if (JeepRock[i].Active != 0) {
      NuVecSub(&Rel,&Pos,&JeepRock[i].Pos);
      if (NuVecMagSqr(&Rel) < 0.5625f) {
         JeepRock[i].Explode = 1;
         Ret = 1;
      }
    }
  }
  return Ret;
}

//NGC MATCH
void DrawVehMasks(void) {
    struct VEHMASK* Mask;
    s32 d;
    s32 i;
    s32 iVar2;
    struct nuvec_s vec;

    Mask = VehicleMask;
    
    for (i = 0; i < 2; i++, Mask++) {
        if (Mask->Active != 0) {
            if (Level == 0x17) {
                NuMtxSetRotationY(
                    &mTEMP, NuAtan2D(GameCam[0].pos.x - Mask->Position.x, GameCam[0].pos.z - Mask->Position.z) + 0x8000
                );
            } else {
                NuMtxSetRotationY(&mTEMP, (s32)(Mask->DrawAngY * 182.04445f));
            }
            NuMtxScale(&mTEMP, SetNuVecPntr(Mask->DrawScale, Mask->DrawScale, Mask->DrawScale));
            NuMtxTranslate(&mTEMP, &Mask->Position);
            d = MyDrawModelNew(&Mask->MainDraw, &mTEMP, NULL);
            
            Mask->Seen = d;
            vec = Mask->Position;
            vec.y += 0.2f;
            
            iVar2 = 0x7e;
            switch (Mask->Id) {
                case 0x57:
                    iVar2 = 0x7f;
                    break;
                case 0x56:
                    iVar2 = 0x80;
                    break;
                case 0x58:
                    iVar2 = 0x81;
                    break;
                case 3:
                    iVar2 = 0xa3;
                    break;
            }
            
            if (Paused == 0) {
                AddVariableShotDebrisEffect(GDeb[iVar2].i, &vec, 1, 0, 0);
            }
        }
    }
}