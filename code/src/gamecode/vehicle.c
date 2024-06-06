
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
