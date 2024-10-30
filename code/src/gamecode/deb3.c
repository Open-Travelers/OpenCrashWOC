
s32 rsfxcount;
s32* rsfxpt;
volatile volatile float HotCoals[13];
volatile volatile float HotRocks[73];
s32 cmask;
struct deb3_s deb3[64];
s32 debpt;
s32 dmask;
s32 dropfire;
s32 exroty;
struct firedrop_s firedrop[16];
s32 firedroppt;
float floor1;
s32 flooron;
s32 hutexplode;
s32 hutframe;
s32 jcrunch;
s32 jonframe1;
s32 lcrunch;
s32 maskoff;
s32 maskrot;
s32 maskx;
s32 masky;
float* rockpt;
float roof1;
s32 roofon;
s32 subprop;
float torndist;
s32 water1;
s32 water2;
s32 wdir;
s32 wtimer;

/*
	AddDeb3Ang     73%
	AddDeb3           65%
	LaunchObjects  86%
	ProcDeb3       92%
*/

//NGC MATCH
void InitRandSFX(void) {
  rsfxcount = 0;
  rsfxpt = NULL;
  return;
}

//NGC MATCH
void PlayRandSFX(void) {
  struct nuvec_s vec;
  s32 iVar1;
  
  if (rsfxpt != NULL) {
    rsfxcount = rsfxcount + -0x3c;
    if (rsfxcount < 1) {
      vec.x = ((s32)(qrand() - 0x8000U) * 0.00024414062f) + global_camera.mtx._30;
      vec.y = ((s32)(qrand() - 0x8000U) * 0.00024414062f) + global_camera.mtx._31;
      vec.z = ((s32)(qrand() - 0x8000U) * 0.00024414062f) + global_camera.mtx._32;
      iVar1 = qrand() * *rsfxpt;
      if (iVar1 < 0) {
        iVar1 += 0xffff;
      }
      rsfxcount = ((iVar1 >> 0x10) + rsfxpt[1]) * 0x3c;
      iVar1 = qrand() * rsfxpt[2];
      if (iVar1 < 0) {
        iVar1 += 0xffff;
      }
      GameSfx(rsfxpt[(iVar1 >> 0x10) + 3],&vec);
    }
  }
  return;
}

//NGC MATCH
void RBodySetSphereClass(int classid,float r,float mass,float kr,float kf) {
  struct rbclass_s *rbc;
  
  rbc = &rbclass[classid];
  rbc->invBodyInertiaTensor = numtx_identity;
  rbc->invBodyInertiaTensor._00 = (mass * 0.4f) * r * r;
  rbc->invBodyInertiaTensor._11 = (mass * 0.4f) * r * r;
  rbc->invBodyInertiaTensor._22 = (mass * 0.4f) * r * r;
  rbc->mass = mass;
  rbc->kr = kr;
  rbc->kf = kf;
  return;
}

//NGC MATCH
void RBodySetFakeClass(s32 classid,float r,float kr) {
  struct rbclass_s* rbc;

  rbc = &rbclass[classid];    
  rbc->mass = 0.0f;
  rbc->kr = kr;
  rbc->kf = r;
  return;
}

//NGC MATCH
static void RBodySetCubiodClass(int classid,float dx,float dy,float dz,float mass,float kr,float kf) {
  struct rbclass_s* rbc; 
  float dz2;
  float dx2;
  float dy2;

  dx2 = (dx * 0.5f);
  dy2 = (dy * 0.5f);
  dz2 = (dz * 0.5f);
  rbc = &rbclass[classid];
  rbc->invBodyInertiaTensor = numtx_identity;
  rbc->invBodyInertiaTensor._00 = 3.0f / (mass * ((dy2 * dy2) + (dz2 * dz2)));
  rbc->invBodyInertiaTensor._11 = 3.0f / (mass * ((dx2 * dx2) + (dz2 * dz2)));
  rbc->invBodyInertiaTensor._22 = 3.0f / (mass * ((dx2 * dx2) + (dy2 * dy2)));
  rbc->mass = mass;
  rbc->kr = kr;
  rbc->kf = kf;
  return;
}

//NGC MATCH
void RBodyInitClasses(void) {
  RBodySetSphereClass(0,1.0f,1.0f,0.5f,0.1f);
  RBodySetCubiodClass(1,1.0f,1.0f,1.0f,0.1f,0.9f,0.2f);
  RBodySetCubiodClass(2,1.0f,2.0f,1.0f,0.1f,0.75f,0.2f);
  RBodySetFakeClass(3,0.0f,0.9f);
  RBodySetFakeClass(4,10.0f,0.75f);
  return;
}

//NGC MATCH
void RBodyMove(struct deb3_s *deb,float dt) {
  struct rbclass_s * rbc;
  struct numtx_s tm;
  struct numtx_s tmtx;
  struct nuvec_s v;
  
  rbc = &rbclass[deb->info->classid];
  if (rbc->mass != 0.0f) {
    tmtx = deb->mtx;
    NuVecScale(&v,&deb->angularvelocity,dt);
    NuMtxSkewSymmetric(&tmtx,&v);
    NuMtxMulR(&tmtx,&deb->mtx,&tmtx);
    NuMtxAddR(&tmtx,&tmtx,&deb->mtx);
    tmtx._30 = ((deb->velocity).x * dt + (deb->mtx)._30);
    tmtx._31 = ((deb->velocity).y * dt + (deb->mtx)._31);
    tmtx._32 = ((deb->velocity).z * dt + (deb->mtx)._32);
    (deb->velocity).y = (deb->grav * dt + (deb->velocity).y);
    NuMtxOrth(&tmtx);
    NuMtxInvR(&tm,&tmtx);
    NuMtxMulR(&deb->invWorldInertiaTensor,&tm,&rbc->invBodyInertiaTensor);
    NuMtxMulR(&deb->invWorldInertiaTensor,&deb->invWorldInertiaTensor,&tmtx);
    NuVecInvMtxRotate(&deb->angularvelocity,&deb->angularMomentum,&deb->invWorldInertiaTensor);
    deb->mtx = tmtx;
  }
  else {
    if (rbc->kf != 0.0f) {
      NuMtxPreRotateX(&deb->mtx,(deb->angularMomentum).x);
      NuMtxPreRotateY(&deb->mtx,(deb->angularMomentum).y);
      NuMtxPreRotateZ(&deb->mtx,(deb->angularMomentum).z);
    }
    (deb->mtx)._30 = ((deb->velocity).x * dt + (deb->mtx)._30);
    (deb->mtx)._31 = ((deb->velocity).y * dt + (deb->mtx)._31);
    (deb->mtx)._32 = ((deb->velocity).z * dt + (deb->mtx)._32);
    (deb->velocity).y = (deb->grav * dt + (deb->velocity).y);
      
  }
  return;
}

//NGC MATCH
void RBodyImpact(struct deb3_s *deb,struct nuvec_s *pnt,struct nuvec_s *nrm) {
  struct rbclass_s* rbc;
  struct nuvec_s R;
  struct nuvec_s velocity;
  struct nuvec_s v;
  struct nuvec_s impulse;
  float impulseD;
  float impulseND;

  rbc = &rbclass[deb->info->classid];
  NuVecSub(&R,pnt,(struct nuvec_s *)&(deb->mtx)._30);
  NuVecCross(&velocity,&R,&deb->angularvelocity);
  NuVecAdd(&velocity,&velocity,&deb->velocity);
  impulseD = NuVecDot(&velocity,nrm);
  impulseND = (impulseD * -(rbc->kr + 1.0f));
  NuVecCross(&v,&R,nrm);
  NuVecInvMtxRotate(&v,&v,&deb->invWorldInertiaTensor);
  NuVecCross(&v,&v,&R);
  impulseD = NuVecDot(&v,nrm);
  impulseD = (impulseND / ((1.0f / rbc->mass) + impulseD));
  impulse.x = impulseD * nrm->x;
  impulse.y = impulseD * nrm->y;
  impulse.z = impulseD * nrm->z;
  (deb->velocity).x = (deb->velocity).x + (impulse.x / rbc->mass );
  (deb->velocity).y = (deb->velocity).y + impulse.y / rbc->mass;
  (deb->velocity).z = (deb->velocity).z + impulse.z / rbc->mass;
  NuVecCross(&v,&impulse,&R);
  (deb->angularMomentum).x = (deb->angularMomentum).x + v.x;
  (deb->angularMomentum).y = (deb->angularMomentum).y + v.y;
  (deb->angularMomentum).z = (deb->angularMomentum).z + v.z;
  NuVecInvMtxRotate(&v,&deb->angularMomentum,&deb->invWorldInertiaTensor);
  (deb->angularvelocity).x = (deb->angularvelocity).x + v.x;
  (deb->angularvelocity).y = (deb->angularvelocity).y + v.y;
  (deb->angularvelocity).z = (deb->angularvelocity).z + v.z;
  return;
}

//NGC MATCH
void InitDeb3(void) {
  InitRandSFX();
  memset(firedrop,0,0x140);
  torndist = 99999.0f;
  debpt = 0;
  firedroppt = 0;
  dropfire = 0;
  hutexplode = 0;
  hutframe = 0;
  exroty = 0;
  jonframe1 = 0;
  subprop = 0;
  water1 = 0;
  water2 = 0;
  wdir = 0;
  wtimer = 0;
  jcrunch = 0;
  lcrunch = 0;
  cmask = 0;
  dmask = 0;
  maskx = 0;
  masky = 0;
  maskoff = 0;
  maskrot = 0;
  memset(deb3,0,0x3800);
  RBodyInitClasses();
  rockpt = NULL;
  if (Level == 1) {
    rockpt = HotRocks;
  }
  if (Level == 0x11) {
    rockpt = HotCoals;
  }
  if (Level == 0x14) {
    floor1 = -0.725f;
    roof1 = 2.225f;
    flooron = 0;
    roofon = 0;
  }
  return;
}

//NGC MATCH
s32 JonMaskFPS(s32 val,s32 add) {
    return add * 0x3c + val <= 0x3c0000 ? val + add * 0x3c : (add * 0x3c + val) - 0x3c0000;
}

//NGC MATCH
void JonExtraDraw(void) {
    ELEC* pt;
    s32 i;
    s32 d;
    s32 col;
    struct nuvec_s vec;

    if (Level == 0x19) {
        switch (cmask) {
            default:
                i = -1;
                d = 0x7e;
                break;
            case 1:
                i = CRemap[85];
                d = 0x7e;
                break;
            case 2:
                i = CRemap[87];
                d = 0x7f;
                break;
            case 3:
                i = CRemap[86];
                d = 0x80;
                break;
            case 4:
                i = CRemap[88];
                d = 0x81;
                break;
        }
        vec.x = 0.0f;
        vec.y = 5.0f - NuTrigTable[(u16)(maskoff / 0x3c)] * 3.0f;
        vec.z = 2.87f;
        vec.x = NuTrigTable[(u16)(maskx / 0x3c)] * 1.5f + vec.x;
        vec.y += (NuTrigTable[(u16)(masky / 0x3c)] * 0.75f);
        vec.z = NuTrigTable[(u16)((maskx / 0x3c) + 0x4000)] * 3.0f + vec.z;
        if (i != -1) {
            Draw3DCharacter(&vec, 0, (u16)(NuTrigTable[(u16)(maskrot / 0x3c)] * 8192.0f), 0, &CModel[i], -1, 1.0f, 1.0f, 0);
            vec.y += 0.2f;
            if (Paused == 0) {
                AddVariableShotDebrisEffect(GDeb[d].i, &vec, 1, 0, 0);
            }
        }
    }
    if (Level != 2) {
        return;
    }
    pt = (ELEC*)&H2OElec[0];
    if (Paused != 0) {
        return;
    }
        for (pt; (pt->start).x != 99999.0f; pt++) {
            vec.x = (pt->start).x - (player->obj).pos.x;
            vec.z = pt->start.z - (player->obj).pos.z;
            if ((vec.x * vec.x + vec.z * vec.z) < 625.0f) {
                pt->time = pt->time - 0x3c;
                if (pt->time < 0) {
                    pt->time = ((qrand() & 0xff) + 0x3c) * 0x3c;
                    pt->ang = qrand() & 0xffff;
                }
                if (pt->time < 0x960) {
                    if (qrand() < 0x4000) {
                        GameSfx(0x89, &pt->start);
                    }
                    if (pt->time > 0x5a0) {
                        col = ((0x28 - pt->time) << 4) / 0x3c;
                    } else if (pt->time < 0x3c0) {
                        col = (pt->time << 4) / 0x3c;
                    } else {
                        col = 0x80;
                    }
                    col = col << 0x18;
                    vec.x = NuTrigTable[pt->ang & 0xffff] * 0.47999999f;
                    vec.y = 0.0f;
                    vec.z = NuTrigTable[(pt->ang + 0x4000U) & 0x3fffc / 4] * 0.47999999f;
                    NuLgtArcLaser( 0, &pt->start, &pt->end, &vec, 0.05f, 0.1f, 0.01f, 0.1f, col + 0x00FF7F00);
                    NuLgtArcLaser(0, &pt->start, &pt->end, &vec, 0.4f, 0.3f, 0.001f, 0.1f, col | 0x800000);
                    AddVariableShotDebrisEffect(GDeb[143].i, &pt->end, 1, 0, 0);
                }
            }
        }
    return;
}

//NGC MATCH
static void DrawDeb3() {
  struct nuvec_s pos;
  struct deb3_s *deb;
  float r;
  s32 i;
  s32 j;
  
  JonExtraDraw();
  deb = deb3;
  for(i = 0; i < 0x40; i++, deb++) {
    if (deb->timer != 0) {
      if (ObjTab[deb->info->type].obj.special != NULL) {
        NuSpecialDrawAt(&ObjTab[deb->info->type].obj,&deb->mtx);
        if ((deb->info->info & 0x10U) != 0) {
          if (deb->shadow != 2000000.0f) {
            pos.x = deb->mtx._30;
            pos.y = deb->shadow;
            pos.z = deb->mtx._32;
            r = (deb->info->size * 0.75f);
            if (0.0f < r) {
              NuRndrAddShadow(&pos,r,0x7f,0,0,0);
            }
          }
        }
      }
    }
  }
  if (rockpt == HotRocks) {
    i = (s32)(pVIS - world_scene[0]->splines);
    if ((i == 1) && (iVIS < 0x8d)) {
      if (ObjTab[i + 0x3e].obj.special != NULL) {
        NuSpecialDrawAt(&ObjTab[i + 0x3e].obj,&(ObjTab[i + 0x3e].obj.special)->mtx);
      }
    }
    else if (((i == 6 || i == 7) || (i == 9))) {
        i = 64;
        if ((ObjTab[i].obj.special != NULL))  {
            NuSpecialDrawAt(&ObjTab[i].obj,&(ObjTab[i].obj.special)->mtx);
        }
    }
  }
}

//NGC MATCH PART OF DRAWDEB3
void RockSpark(struct deb3_s *deb) {
  s32 key;
  struct nuvec_s vec;
  
  vec.x = (deb->mtx)._30;
  vec.y = (deb->mtx)._31;
  vec.z = (deb->mtx)._32;
  key = -1;
  deb->data--;
  if (deb->data < 0) {
    deb->timer = 1;
  }
  else {
    AddFiniteShotDebrisEffect(&key,GDeb[33].i,&vec,1);
    GameSfx(0x5f,&vec);
  }
  return;
}

//NGC MATCH PART OF DRAWDEB3
void CoalSpark(struct deb3_s *deb) {
  s32 key;
  struct nuvec_s vec;
  
  vec.x = (deb->mtx)._30;
  vec.y = (deb->mtx)._31;
  vec.z = (deb->mtx)._32;
  key = -1;
  deb->data--;
  if (deb->data < 0) {
    deb->timer = 1;
  }
  else {
    AddFiniteShotDebrisEffect(&key,GDeb[35].i,&vec,1);
  }
  return;
}

//NGC MATCH PART OF DRAWDEB3
void RockBreak(struct deb3_s *deb) {
  s32 key;
  struct nuvec_s vec;
  
  vec.x = (deb->mtx)._30;
  vec.y = (deb->mtx)._31;
  vec.z = (deb->mtx)._32;
  key = -1;
  AddFiniteShotDebrisEffect(&key,GDeb[33].i,&vec,1);
  GameSfx(0x5f,&vec);
  return;
}

//NGC MATCH PART OF DRAWDEB3
void CoalBreak(struct deb3_s *deb) {
  s32 key;
  struct nuvec_s vec;
  
  vec.x = (deb->mtx)._30;
  vec.y = (deb->mtx)._31;
  vec.z = (deb->mtx)._32;
  key = -1;
  AddFiniteShotDebrisEffect(&key,GDeb[36].i,&vec,1);
  return;
}

//NGC MATCH PART OF DRAWDEB3
void DynaBreak(struct deb3_s *deb) {
  struct nuvec_s pos;
  
  pos.x = (deb->mtx)._30;
  pos.y = (deb->mtx)._31;
  pos.z = (deb->mtx)._32;
  AddGameDebris(0x44,&pos);
  GameSfx(0x3b,&pos);
  return;
}