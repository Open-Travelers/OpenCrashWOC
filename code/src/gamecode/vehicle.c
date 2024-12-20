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
void DrawWeatherBoss(void) {
  if (WeatherBoss.Active != 0) {
    DrawWeatherBoss_a(&WeatherBoss);
  }
}

//NGC MATCH
void ProcessCrashteroidsIntro(void) {
    ProcessFireFlyIntro();
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
void InitLighteningHail(void) {
  memset(&HailList,0,0x438);
  NuMtxSetScale(&BoltMtxC,SetNuVecPntr(0.5f,1.5f,0.5f));
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