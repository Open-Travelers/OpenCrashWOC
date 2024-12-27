#define ALIGN_ADDRESS(addr, al) (((s32)addr + (al-1)) & ~(al-1))

struct nuvec_s BaseWheelPosition[6];
struct enemyjeep_s EnemyJeep[4];
struct jeeptrail_s JeepTrail[20][32];
struct JEEPBALLOON JeepBalloon[6];
struct FIREBOSSSTRUCT FireBoss;
s32 FireBossFinished;
s32 FireBossWon;
f32 WaterTimer;

//NGC MATCH
struct nuvec_s GenerateJeepWheelPoint(s32 WheelId) {
  return BaseWheelPosition[WheelId];
}

//NGC MATCH
void NewGenerateJeepMatrix(struct numtx_s *Mat,short YAng,short SurfaceX,short SurfaceZ,short TiltX,short TiltZ,struct nuvec_s *Pos) {  
  NuMtxSetRotationX(Mat,TiltX);
  NuMtxRotateZ(Mat,TiltZ);
  NuMtxRotateY(Mat,YAng);
  NuMtxRotateZ(Mat,SurfaceZ);
  NuMtxRotateX(Mat,SurfaceX);
  if (Pos != NULL) {
    NuMtxTranslate(Mat,Pos);
  }
  return;
}

//NGC MATCH
void TiltSeek(struct JEEPSTRUCT *Jeep,float DeltaTime) {
  float fVar1;
  float TempX;
  float TempZ;
  
  if (Jeep->TiltSeekTime == 0.0f) {
    fVar1 = 1.0f;
  }
  else {
    fVar1 = (1.0 - 1.0 / pow(2.0,(DeltaTime / Jeep->TiltSeekTime)));
  }
  TempX = (f32)(s16)(Jeep->aDestTiltX - Jeep->aTiltX) * fVar1;
  TempZ = (f32)(s16)(Jeep->aDestTiltZ - Jeep->aTiltZ) * fVar1;
  Jeep->aTiltX += (short)(TempX);
  Jeep->aTiltZ += (short)(TempZ);
  return;
}

//NGC MATCH
void LimitSpeedZbyXZ(struct nuvec_s *Vec,float LimitSpeed) {
  float MaxZ;
  
  if (Vec->x > LimitSpeed) {
    Vec->x = LimitSpeed;
  }
  else {
    if (Vec->x < -LimitSpeed) {
      Vec->x = -LimitSpeed;
    }
  }
  MaxZ = NuFsqrt(LimitSpeed * LimitSpeed - Vec->x * Vec->x) * 1.1f;
  if (Vec->z > MaxZ) {
    Vec->z = MaxZ;
  }
  return;
}

//NGC MATCH
static void ProcessJeepMovement(struct JEEPSTRUCT *Jeep,float DeltaTime) {
  struct numtx_s MoveMatrixA;
  struct numtx_s MoveMatrixB;
  float SteepVal;
  float TractionVal;
  struct nuvec_s *Vel;
  struct nuvec_s Resolved;
  float SeekTime;
  float NewCent;
  float NewAcc;
  float Temp;
  float Dot;
  
  if (Jeep->Quick != 0) {
    NuMtxSetRotationY(&MoveMatrixA,(int)Jeep->aAngleY);
    NuMtxSetRotationY(&MoveMatrixB,-(int)Jeep->aAngleY);
    SteepVal = 1.0f;
    TractionVal = 1.0f;
  }
  else {
    NuMtxSetRotationY(&MoveMatrixA,(int)Jeep->aAngleY);
    NuMtxRotateZ(&MoveMatrixA,(int)Jeep->Move.aActSurfRotZ);
    NuMtxRotateX(&MoveMatrixA,(int)Jeep->Move.aActSurfRotX);
    NuMtxSetRotationX(&MoveMatrixB,-Jeep->Move.aTarSurfRotX);
    NuMtxRotateZ(&MoveMatrixB,-(int)Jeep->Move.aTarSurfRotZ);
    NuMtxRotateY(&MoveMatrixB,-(int)Jeep->aAngleY);
    if ((Jeep->Move.AnyOnGroundBits & 1U) != 0) {
        Dot = DotProduct(&Jeep->Move.WheelAxis[1],&WorldAxis[1]);
        if (Dot > 0.866f) {
          SteepVal = 0.0f;
        }
        else if (Dot < 0.707f) {
            SteepVal = 1.0f;
        }
        else {
            SteepVal = ((0.866f - Dot) / 0.159f);
        }
        Dot = DotProduct(&Jeep->Move.WheelAxis[2],&WorldAxis[1]);
        if (Dot > 0.707f) {
          TractionVal = 0.0f;
        }
        else if (Dot < 0.5f) {
            TractionVal = 1.0f;
        }
        else {
            TractionVal = (0.707f - Dot) / 0.207f;
        }
        TractionVal = TractionVal * (2.0f - SteepVal) * 0.5f;
    } else {
        SteepVal = 1.0f;
        TractionVal = 1.0f;
    }
  }
  Vel = &Jeep->Move.Velocity;
  if ((Jeep->Move.AnyOnGroundBits & 1U) != 0 || (Jeep->Quick != 0)) {
  SeekTime = 0.5f;
  NuVecMtxRotate(&Resolved,Vel,&MoveMatrixB);
  NewCent = Resolved.x;
  if (Level == 0x16) {
    ApplyFriction(&Resolved.x,11.4f,DeltaTime);
  }
  else {
    ApplyFriction(&Resolved.x,9.5f,DeltaTime);
  }
  NewCent -= Resolved.x;
  NewCent /= DeltaTime;
  NewCent /= 9.5f;
  if (Jeep->WheelSpin != 0) {
    NewCent = -Jeep->TurnSin;
  }
  if (((NewCent < -0.1f) && (Jeep->CentrefugalForce > 0.1f)) ||
     ((NewCent > 0.1f && (Jeep->CentrefugalForce < -0.1f)))) {
    Jeep->CentrefugalForce = 0.0f;
  }
  else if (Jeep->WheelSpin != 0) {
    SeekTime = 0.029999999f;
  }
  else if (NuFabs(Jeep->CentrefugalForce) > NuFabs(NewCent)) {
      SeekTime = 0.1f;
  }
  SeekHalfLife(&Jeep->CentrefugalForce,NewCent,SeekTime,DeltaTime);
  if (Jeep->Finished != 0) {
    ApplyFriction(&Resolved.z,2.0f,DeltaTime);
  }
  else {
    ApplyFriction(&Resolved.z,6.0f,DeltaTime);
  }
  NewAcc = -Resolved.z;
  if (Jeep->WheelSpin != 0) {
    SeekHalfLife(&Jeep->Traction,0.0f,0.05f,DeltaTime);
  }
  else {
    SeekHalfLife(&Jeep->Traction,1.0f,0.3f,DeltaTime);
  }
  SeekTime = 2.0f;
  Temp = (Jeep->MaxSpeed * Jeep->Accelerator * Jeep->Traction) * TractionVal;
  if ((Jeep->TerrainType == 10) && (Level != 0x16)) {
    Temp *= 0.5f;
  }
  if (Resolved.z < Temp) {
    SeekHalfLife(&Resolved.z,Temp,SeekTime,DeltaTime);
  }
  NewAcc = ((NewAcc + Resolved.z) / DeltaTime) / Jeep->MySpeed;
  SeekHalfLife(&NewAcc,0.0f,3.0f,NuFabs(Resolved.x));
  if (Jeep->WheelSpin != 0) {
    if (NewAcc < 1.0f) {
      NewAcc = (NewAcc + 1.0f) * 0.5f;
    }
  }
  SeekHalfLife(&Jeep->AccelerationForce,NewAcc,0.1f,DeltaTime);
  LimitSpeedZbyXZ(&Resolved,Jeep->MaxSpeed);
  if ((Jeep->Move.AllOnGroundBits & 1U) != 0 || (Jeep->Quick != 0)) {
    Jeep->GroundTractionAcc = Jeep->GroundTractionAcc - Resolved.y;
    if (Resolved.y < 0.0f) {
      Resolved.y = 0.0f;
    }
  }
  else {
    Jeep->GroundTractionAcc = 0.0f;
  }
  SeekHalfLife(&Jeep->GroundTractionAcc,0.0f,0.4f,DeltaTime);
  Jeep->Move.Resolved = Resolved;
  NuVecMtxRotate(Vel,&Resolved,&MoveMatrixA);
} else {
    Jeep->CentrefugalForce = 0.0f;
    ApplyFriction(&Vel->x,0.2f,DeltaTime);
    ApplyFriction(&Vel->z,0.2f,DeltaTime);
    NuVecMtxRotate(&Jeep->Move.Resolved,Vel,&MoveMatrixB);
}
  if ((Jeep->Move.AllTouchingGroundBits & 1U) == 0) {
    if (Jeep->Quick == 0) {
      if (((Jeep->Move.AnyOnGroundBits & 1U) != 0) && (SteepVal < 1.0f)) {
        SteepVal = 1.0f;
      }
      Jeep->Move.Velocity.x -= (WorldAxis[1].x * 0.5f) * SteepVal;
      Jeep->Move.Velocity.y -= (WorldAxis[1].y * 0.5f) * SteepVal;
      Jeep->Move.Velocity.z -= (WorldAxis[1].z * 0.5f) * SteepVal;
    }
  }
}

//NGC MATCH
void FindTerrainType(struct JEEPSTRUCT *Jeep) {
  struct nuvec_s Pos;
  float FloorY;
  
  Pos = (Jeep->Move).ActualPosition;
  Pos.y += 1.0f;
  FloorY = (Jeep->Move).ActualPosition.y - NewShadowMaskPlat(&Pos,0.0f,-1);
  Jeep->FloorHeight = FloorY;
  if (FloorY < 0.1f) {
    Jeep->TerrainType = ShadowInfo();
  }
  else {
    Jeep->TerrainType = -1;
  }
  return;
}

//NGC MATCH
void SetUpJeepWheelPositions(struct JEEPSTRUCT *Jeep) {
  float fVar1;
  float *pfVar2;
  float fVar3;
  s32 iVar4;
  struct numtx_s Mtx[16];
  
  GetLocatorMtx((Jeep->ChassisDraw).Model,Mtx,180.0f);
  for(iVar4 = 0; iVar4 < 4; iVar4++) {
    BaseWheelPosition[iVar4] = *(struct nuvec_s*)&Mtx[iVar4]._30;
  }
  BaseWheelPosition[0] = SetNuVec(-0.35f,0.23669f,0.529f);
  BaseWheelPosition[1] = SetNuVec(0.35f,0.23669f,0.529f);
  BaseWheelPosition[2] = SetNuVec(-0.35f,0.23669f,-0.31f);
  BaseWheelPosition[3] = SetNuVec(0.35f,0.23669f,-0.31f);
  BaseWheelPosition[4].x = (BaseWheelPosition[0].x + BaseWheelPosition[1].x) * 0.5f;
  BaseWheelPosition[4].y = (BaseWheelPosition[0].y + BaseWheelPosition[1].y) * 0.5f;
  BaseWheelPosition[4].z = (BaseWheelPosition[0].z + BaseWheelPosition[1].z) * 0.5f;
  NuVecAdd(&BaseWheelPosition[5],BaseWheelPosition,&BaseWheelPosition[1]);
  NuVecAdd(&BaseWheelPosition[5],&BaseWheelPosition[5],&BaseWheelPosition[2]);
  NuVecAdd(&BaseWheelPosition[5],&BaseWheelPosition[5],&BaseWheelPosition[3]);
  NuVecScale(&BaseWheelPosition[5],&BaseWheelPosition[5],0.25f);
  return;
}

//NGC MATCH
struct numtx_s* DrawPlayerJeep(struct creature_s *c) {
  struct JEEPSTRUCT* Jeep = (struct JEEPSTRUCT *)c->Buggy;
  
  if (((Jeep == NULL) || (VEHICLECONTROL == 0)) || ((c->obj).vehicle != 99)) {
    return NULL;
  }

  return DrawJeep(Jeep);
}

//NGC MATCH
void LimitCam(struct MYSPLINE *Spline,struct nuvec_s *Cam,float LimitDist,float YOff) {
  float Dist;
  float MinY;
  struct nuvec_s Point;
  struct nuvec_s Temp;
  
  Dist = FindSplineClosestPointAndDist(Spline,0x303,Cam,&Point,0,0);
  MinY = Point.y + YOff;
  if (Cam->y < MinY) {
    Cam->y = MinY;
  }
  if (Dist > LimitDist) {
    Point.y = Cam->y;
    NuVecSub(&Temp,Cam,&Point);
    NuVecScaleAccum(&Point,&Temp,(LimitDist / NuVecMag(&Temp)));
    *Cam = Point;
  }
  return;
}

//NGC MATCH
void DoCamMtx(struct cammtx_s *CamMtx,struct nuvec_s *Obj,struct nuvec_s *Pos) {
  struct nuvec_s DeltaVec;
  float MagXZ;
  
  NuVecSub(&DeltaVec,Obj,Pos);
  MagXZ = NuFsqrt(DeltaVec.x * DeltaVec.x + DeltaVec.z * DeltaVec.z);
  JeepCamY = NuAtani((DeltaVec.x * 256.0f),(DeltaVec.z * 256.0f)) / 182.04445f;
  JeepCamX = NuAtani((-DeltaVec.y * 256.0f),(MagXZ * 256.0f)) / 182.04445f;
  NuMtxSetRotationX(&CamMtx->m,(s32)(JeepCamX * 182.04445f));
  NuMtxRotateY(&CamMtx->m,(s32)(JeepCamY * 182.04445f));
  NuMtxTranslate(&CamMtx->m,Pos);
  (CamMtx->pos) = *Pos;
  CamMtx->xrot = (s32)(JeepCamX * 182.04445f);
  CamMtx->yrot = (s32)(JeepCamY * 182.04445f);
  return;
}

//NGC MATCH
void BlendNUVECs(struct nuvec_s *Dest,struct nuvec_s *A,struct nuvec_s *B,float Blend) {
  NuVecScale(Dest,A,(1.0f - Blend));
  NuVecScaleAccum(Dest,B,Blend);
  return;
}

//NGC MATCH
void JeepCamIntro(struct cammtx_s *CamMtx) {
  float ratio;
  float blend;
  
  if (WesternCountdown < 1.0f) {
    ratio = 1.0f;
  } else if (WesternCountdown > 6.0f) {
      ratio = 0.0f;
  }
  else {
      ratio = (5.0f - (WesternCountdown - 1.0f)) / 5.0f;
  }
  if (0.94999999f < ratio) {
    ratio = 0.94999999f;
  }
  if (WesternCountdown < 1.0f) {
      blend = 1.0f;
  } else if (WesternCountdown > 6.0f) {
      blend = 0.0f;
  } else {
      blend = (5.0f - (WesternCountdown - 1.0f)) / 5.0f;
  }
  PointAlongSpline(JeepIntroCamSpline,ratio,&JeepvPos,NULL,NULL);
  PointAlongSpline(JeepIntroLookSpline,ratio,&JeepvObj,NULL,NULL);
  if (blend != 0.0f) {
    JeepCamFollowAng(NULL,1);
    blend = (blend * blend);
    NuVecScale(&JeepvPos,&JeepvPos,(1.0f - blend));
    NuVecScale(&JeepvObj,&JeepvObj,(1.0f - blend));
    NuVecScaleAccum(&JeepvPos,&IdealCamPos,blend);
    NuVecScaleAccum(&JeepvObj,&IdealObjPos,blend);
  }
  DoCamMtx(CamMtx,&JeepvObj,&JeepvPos);
  if (WesternCountdown <= 0.1f) {
    SmokeyCam = 0x14;
  }
  if (best_cRPos != NULL) {
    pVIS = Rail[best_cRPos->iRAIL].pCAM;
    iVIS = best_cRPos->iALONG;
  }
  return;
}

//NGC MATCH
void AnimateForLightsEnemyJeep(struct enemyjeep_s *Jeep) {
  MyAnimateModelNew(&Jeep->Draw,0.5f);
  return;
}

//NGC MATCH
void InitEnemyJeeps() {
  memset(&EnemyJeep,0,0x1d00);
  InitEnemyJeep(&EnemyJeep[0],0x90);
  InitEnemyJeep(&EnemyJeep[1],0x8f);
  InitEnemyJeep(&EnemyJeep[2],0x8e);
  InitEnemyJeep(&EnemyJeep[3],0x91);
}

//NGC MATCH
void DrawEnemyJeeps() {
  s32 i;
  
  for(i = 0; i < 4; i++) {
    if (EnemyJeep[i].Active != 0) {
      DrawEnemyJeep(&EnemyJeep[i]);
    }
  }
  return;
}

//NGC MATCH
void ProcessEnemyJeeps(void) {
  s32 i;
  
  for(i = 0; i < 4; i++) {
    if (EnemyJeep[i].Active != 0) {
      ProcessEnemyJeep(&EnemyJeep[i]);
    }
  }
  return;
}

//NGC MATCH
void AnimateForLightsEnemyJeeps() {
  s32 i;
  
  for(i = 0; i < 4; i++) {
    if (EnemyJeep[i].Active != 0) {
      AnimateForLightsEnemyJeep(&EnemyJeep[i]);
    }
  }
  return;
}

void WesternRaceManager(void) {
  return;
}

//NGC MATCH
void LoadWesternArenaData(void) {
  s32 i;
  
  TerrainSetCur(superbuffer_ptr.voidptr);
  terraininit(Level,&superbuffer_ptr.s16,superbuffer_end.s16,0,LevelFileName,world_scene[0],0);
  superbuffer_ptr.intaddr = ALIGN_ADDRESS(superbuffer_ptr.voidptr, 0x10);
  
  for(i = 0; i < 5; i++) {
    BuggyData[i] = superbuffer_ptr.voidptr;
    superbuffer_ptr.voidptr = LoadBuggyData(superbuffer_ptr.voidptr,i);
  }
  superbuffer_ptr.intaddr = ALIGN_ADDRESS(superbuffer_ptr.voidptr, 0x10);
  return;
}

//NGC MATCH
void WesternArenaReset(s32 PlayerDead) {
  struct nuvec_s Temp;
  
  PlayerJeep.Finished = 0;
  SmokeyFinished = 0;
  SmokeyCountDownValue = 0;
  WesternCountdown = 5.999f;
  WesternTime = 0;
  InitEnemyJeeps();
  JeepIntroCamSpline = SplTab[68].spl;
  JeepIntroLookSpline = SplTab[69].spl;
  if ((SplTab[68].spl != NULL) && (SplTab[69].spl != NULL)) {
    SmokeyCam = 0x15;
  } else {
    SmokeyCam = 0x14;
  }
  JeepFollowSpline.Spline = Rail[0].pCAM;
  if (Rail[0].pCAM != NULL) {
    JeepFollowSpline.Cur = 0.0f;
    JeepFollowSpline.Inc = 0.0005f;
    JeepFollowSpline.Nex = 0.0f;
    JeepFollowSpline.Act = 0.0f;
    PointAlongSpline(Rail[0].pCAM,0.0f,&Temp,NULL,NULL);
    JeepFollowSpline.CurPos = Temp;
    JeepFollowSpline.NexPos = Temp;
  }
  return;
}

void DrawWesternArenaLevelExtra(void) {
  DrawEnemyJeeps();
  return;
}

//NGC MATCH
void ProcessWesternArenaLevel(struct nupad_s *Pad) {
  if (WesternCountdown == 0.0f) {
    if (PlayerJeep.Finished == 0) {
      WesternTime += 0.01666667f;
    }
    WesternRaceManager();
    ProcessEnemyJeeps();
  }
  else {
    AnimateForLightsEnemyJeeps();
  }
  ProcessJeepTrails();
  SmokeyPosition = CurrentWesternPosition();
  return;
}

//NGC MATCH
void EmptyTrail(int i) {
  s32 j;
  
  for(j = 0; j < 0x20; j++) {
    JeepTrail[i][j].pos1.x = -10000.0f;
  }
  TrailAir[i] = 0;
  TrailPntr[i] = 0;
  return;
}

//NGC MATCH
void NewInitTrail(void) {
  s32 i;

  for(i = 0; i < 0x14; i++) {
    EmptyTrail(i);
  }
  return;
}

//NGC MATCH
s32 NewFindTrailAng(struct nuvec_s *A,struct nuvec_s *B) {
  struct nuvec_s Line;
  
  NuVecSub(&Line,B,A);
  return (u16) (NuAtan2D(Line.x,Line.z) - 0x2000); // & 0xffff
}

//NGC MATCH
void ProcessJeepTrails() {
  s32 i;
  
  if ((Level == 3) || (Level == 0x16)) {
    for(i = 0; i < 0x14; i++) {
      if (TrailActive[i] != 0) {
        TrailActive[i]--;
        if (TrailActive[i] == 0) {
            EmptyTrail(i);
        }  
      }
    }
  }
  return;
}

//NGC MATCH
void DrawJeepTrails(void) {
  s32 i;
  
  if ((((Level == 3) || (Level == 0x16)) || (Level == 9)) || (Level == 5)) {
    for(i = 0; i < 0x14; i++) {
      if (TrailActive[i] != 0) {
        NuRndrTrail(TrailPntr[i],&JeepTrail[i][0],0x20);
      }
    }
  }
  return;
}

//NGC MATCH
void DrawFireBoss(struct FIREBOSSSTRUCT *Boss) {
  struct nuvec_s P0;
  struct nuvec_s P1;
  struct nuvec_s Rel;
  
  NuMtxSetScale(&mTEMP,&FIREBOSSSCALE);
  NuMtxRotateY(&mTEMP,(s32)((Boss->AngleY + 180.0f) * 182.0444f));
  NuMtxTranslate(&mTEMP,&Boss->Position);
  Boss->DrawMtx = mTEMP;
  if (Boss->Action == 5) {
      Boss->Seen = MyDrawModelNew(&Boss->ExplodeDraw,&mTEMP,Boss->Locator);
  } else {
      Boss->Seen = MyDrawModelNew(&Boss->MainDraw,&mTEMP,Boss->Locator);
  }
  if ((WallOfFireOn != 0) && (0 < FireBoss.HitPoints)) {
    if (Paused == 0) {
      P0 = WallOfFirePosition;
      P0.y += 0.5f;
      AddVariableShotDebrisEffect(GDeb[0x95].i,&P0,4,0,(short)WallOfFireAngleY);
    }
    NuVecRotateY(&P0,SetNuVecPntr(4.0f,0.0f,0.0f),(s32)(WallOfFireAngleY * 182.0444f));
    NuVecRotateY(&P1,SetNuVecPntr(-4.0f,0.0f,0.0f),(s32)(WallOfFireAngleY * 182.0444f));
    NuVecAdd(&P0,&WallOfFirePosition,&P0);
    NuVecAdd(&P1,&WallOfFirePosition,&P1);
    NuVecSub(&Rel,&P1,&P0);
  }
  return;
}

//NGC MATCH
void JonnySteam(struct FIREBOSSSTRUCT *Boss) {
  struct nuvec_s vec;
  
  if (Boss->BeenHitThisPassTimer < 3.0f) {
    vec = (Boss->Position);
    vec.y += 1.5f;
    AddVariableShotDebrisEffect(GDeb[162].i,&vec,1,0,0);
  }
  return;
}

//NGC MATCH
s32 CheckAgainstFireBoss(struct nuvec_s *Position,struct nuvec_s *Move,float RAD) {
  float RAD2;
  struct nuvec_s Rel;
  struct nuvec_s Pos;
  s32 Ret;
  float Dist;
  float Dist2;
  
  RAD2 = RAD * RAD;
  Ret = 0;
  if (Level != 0x16) {
    return 0;
  }
  else {
    if (Move != NULL) {
      NuVecAdd(&Pos,Position,Move);
    }
    else {
      Pos = *Position;
    }
    NuVecSub(&Rel,&Pos,&FireBoss.Position);
    Rel.y = 0.0f;
    Dist = NuVecMagSqr(&Rel);
    if (Dist < RAD2) {
      if (Move != NULL) {
        DotProduct(&Rel,Move); // ?
        Dist2 = NuFsqrt(Dist);
        NuVecScale(&Rel,&Rel,(RAD / Dist2));
        NuVecAdd(&Pos,&FireBoss.Position,&Rel);
        NuVecSub(Move,&Pos,Position);
      }
      Ret = 1;
    }
  }
  return Ret;
}

//NGC MATCH
void FireBossReset(void) {
  FireBossFinished = 0;
  FireBossWon = 0;
  ChrisBigBossDead = 0;
  InitVehMasks();
  InitVehMask(0,0x56);
  InitVehMask(1,3);
  InitFireBoss(&FireBoss);
  InitJeepRocks();
  VEHICLECONTROL = 0;
  WaterTimer = 0;
  return;
}

//NGC MATCH
void DrawFireBossLevelExtra(void) {
  DrawFireBoss(&FireBoss);
  DrawJeepRocks();
  DrawVehMasks();
  return;
}

//NGC MATCH
void ProcessFireBossLevel(struct nupad_s *Pad) {
  struct nuvec_s Rel;
  
  ProcessFireBoss(&FireBoss);
  ProcessJeepRocks();
  ProcessVehMasks();
  ProcessRockRockCollisions();
  NuVecSub(&Rel,&player->obj.pos,&player->obj.oldpos);
  if (CheckAgainstRocks(&player->obj.pos,&Rel) != 0) {
    NewRumble(&player->rumble,SMASHRUMPOWER);
    NewBuzz(&player->rumble,6);
    if ((FireBossHoldPlayer == 0) && (0 < FireBoss.HitPoints)) {
      KillPlayer(&player->obj,0xd);
    }
  }
  if (FireBossHealth < 1) {
    FireBossWon = 1;
    FireBossFinished = 1;
  }
  return;
}

//NGC MATCH
s32 GetTotalFireBossObjectives(void) {
    return 3;
}

//NGC MATCH
s32 GetCurrentFireBossObjectives(void) {
    return FireBoss.HitPoints;
}

//NGC MATCH
struct JEEPBALLOON* FindJeepBalloon(void) {
  s32 i;
  
  for(i = 0; i < 6; i++) {
     if (JeepBalloon[i].Active == 0) {
       return &JeepBalloon[i];
     }
  }
  return NULL;
}

//NGC MATCH
s32 AddBalloon(struct nuvec_s *Pos,struct nuvec_s *Vel) {
  struct JEEPBALLOON *Balloon;
  s32 i;
  
  Balloon = FindJeepBalloon();
  if (Balloon != NULL) {
    memset(Balloon,0,0x30);
    Balloon->Active = 1;
    Balloon->Vel = *Vel;
    Balloon->Pos = *Pos;
    Balloon->Life = 5.0f;
    Balloon->AngY = (short)NuAtan2D(Vel->x * 100.0f,Vel->z * 100.0f);
    i = 1;
  } else {
      i = 0;
  }
    return i;
}

//NGC MATCH
void FireBossWaterFire(s32 WaterOn) {
  struct nuvec_s Start;
  struct nuvec_s nStack_c0;
  struct nuvec_s End;
  static short WaterCurrentAngleY;
  float WaterCurrentLength;
  float WaterCurrentWidth;
  short Rel;
  s32 i;
  struct nuvec_s CRel;
  
  if (WaterOn != 0) {
    Rel = (short)(player->obj.hdg - WaterCurrentAngleY);
    if (FireBossWaterSoundTimer == 0.0f) {
      MyGameSfx(0xb7,&player->obj.pos,WATERFIRESOUNDVOL);
      FireBossWaterSoundTimer = FIREWATERSOUNDTIME;
    }
      
    if ((Rel >= 0 ? Rel : -Rel) <= SEEKANGSPEED) {
      WaterCurrentAngleY = player->obj.hdg;
    }
    else {
      if (Rel >= 0) {
        WaterCurrentAngleY = WaterCurrentAngleY + SEEKANGSPEED;
      }
      else {
        WaterCurrentAngleY = WaterCurrentAngleY - SEEKANGSPEED;
      }       
    }
    WaterTimer += 0.01666667f;
    if (WaterTimer > MAXWATERTIME) {
      WaterTimer = MAXWATERTIME;
    }
  }
  else {
    if (ProcessTimer(&WaterTimer) != 0) {
      WaterCurrentAngleY = player->obj.hdg;
    }
  }
  Start = *(struct nuvec_s*)(&player->mtxLOCATOR[8][0]._30);
  WaterCurrentWidth = (WaterWidth * WaterTimer) / MAXWATERTIME;
  NuVecRotateY(&nStack_c0,SetNuVecPntr(0.0f,0.0f,(WaterLength * WaterTimer) / MAXWATERTIME),WaterCurrentAngleY);
  if (((FireBoss.BeenHitThisPass == 0) && (FireBoss.Action != 4)) && (WaterTimer != 0.0f)) {
    for(i = 0; i < 3; i++) {
      WaterCurrentLength = i / 3.0f;
      NuVecScale(&CRel,&nStack_c0,WaterCurrentLength);
      NuVecAdd(&End,&Start,&CRel);
      if (CheckAgainstFireBoss(&End,NULL,(WaterCurrentWidth * WaterCurrentLength + 2.0f)) != 0) {
        FireBoss.HitPoints--;
        FireBoss.BeenHitThisPass = 1;
        MyGameSfx(0xc0,&FireBoss.Position,FBSCREAMVOL);
        if (FireBoss.HitPoints < 1) {
          AshesMechOutZ = 200.0f;
        }
        i = 3;
      }
    }
  }
  return;
}

//NGC MATCH
void ProcessGenericTrail(int id,struct nuvec_s *pos,float Radius,float width) {
  GenericTrail[id].OldPosition = GenericTrail[id].Position;
  GenericTrail[id].Radius = Radius;
  GenericTrail[id].Position = *pos;
  GenericTrail[id].TrailWidth = width;
  GenericTrail[id].Platform = -1;
  ProcessJeepTrail(GenericTrail + id,id);
  return;
}
