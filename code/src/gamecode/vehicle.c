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
  return;
}

void DrawEarthBossLevelExtra(void) {
  DrawEarthBoss();
  DrawJeepRocks();
  DrawVehMasks();
  return;
}

void ProcessEarthBossVortex(void) {
  if (EarthBossVortexOpen != 0) {
    CheckAtlasVortex(&EarthBoss);
    CheckAtlasVortex(&PlayerAtlas);
  }
  return;
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
void DrawSatellite(SATELLITESTRUCT *Satellite) {
  mTEMP = Satellite->SpaceStation->Matrix;
  NuMtxPreRotateY(&mTEMP,(int)(Satellite->AngleY * 182.04445f));
  NuMtxPreScale(&mTEMP,SetNuVecPntr(3.0f,3.0f,3.0f));
  *(struct nuvec_s*)(&mTEMP._30) = Satellite->Position;
  Satellite->Seen = MyDrawModelNew(&Satellite->MainDraw,&mTEMP,NULL);
  return;
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
  return;
}

//NGC MATCH
void DrawAsteroids(void) {
  s32 i;

  for(i = 0; i < 100; i++) {
    if (AsteroidList[i].Active != 0) {
      DrawAsteroid(&AsteroidList[i]);
    }
  }
  return;
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
void InitGliderBombs(void) {
  memset(&GliderBombs,0,0x2a8);
  memset(EnemyGliderBombs,0,0x7f8);
  return;
}

//NGC MATCH
void InitGunBoats(void) {
  memset(&GunBoatList,0,0x4d0);
  InitGunBoat(SetNuVecPntr(-24.0f,0.0f,50.0f),0.0f,0);
  InitGunBoat(SetNuVecPntr(36.0f,0.0f,-15.0f),0.0f,1);
  InitGunBoat(SetNuVecPntr(93.0f,0.0f,43.0f),0.0f,2);
  InitGunBoat(SetNuVecPntr(21.0f,0.0f,106.0f),0.0f,3);
  return;
}

//NGC MATCH
struct nuvec_s * GetWeatherBossPos(void) {
  return &WeatherBoss.Position;
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
    return;
}