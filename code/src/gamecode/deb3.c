
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
	RBodySetCubiodClass  95%
	AddDeb3Ang     73%
	AddDeb3           65%
	JonExtraDraw    87%
	LaunchObjects  67%
	ProcDeb3 TODO
	DrawDeb3      99%
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