typedef struct {
    // total size: 0xB4C
    struct nuvec_s pos[24][2]; // offset 0x0, size 0x240
    struct nuvec_s vel[24][2]; // offset 0x240, size 0x240
    struct numtx_s mtx[24]; // offset 0x480, size 0x600
    struct nuinstance_s * instance[24]; // offset 0xA80, size 0x60
    struct nuinstance_s * ipost; // offset 0xAE0, size 0x4
    struct nuvec_s center; // offset 0xAE4, size 0xC
    float radius; // offset 0xAF0, size 0x4
    short plat[24]; // offset 0xAF4, size 0x30
    char inrange; // offset 0xB24, size 0x1
    char onscreen; // offset 0xB25, size 0x1
    char sections; // offset 0xB26, size 0x1
    char postint; // offset 0xB27, size 0x1
    int colour; // offset 0xB28, size 0x4
    short hit; // offset 0xB2C, size 0x2
    short yang; // offset 0xB2E, size 0x2
    float width; // offset 0xB30, size 0x4
    float tension; // offset 0xB34, size 0x4
    float gravity; // offset 0xB38, size 0x4
    float damp; // offset 0xB3C, size 0x4
    float plrweight; // offset 0xB40, size 0x4
    float postw; // offset 0xB44, size 0x4
    float posth; // offset 0xB48, size 0x4
} BridgeType;

BridgeType Bridges[8];
s32 BridgeFree;
s32 NuBridgeProc;

struct nugscn_s* NuBridge_base_scene;
#define SQR(x) ((x)*(x))


//NGC MATCH
BridgeType* NuBridgeAlloc(void) {
  if (BridgeFree < 8) {
      return &Bridges[BridgeFree++];
  }
  return NULL;
}

//NGC MATCH
void NuBridgeRegisterBaseScene(struct nugscn_s *scn) {
  NuBridge_base_scene = scn;
  return;
}

//NGC MATCH
s32 NuBridgeLookupInstanceIndex(struct nuinstance_s *instance) {
  s32 i;
  
  if (NuBridge_base_scene == NULL) {
    return -1;
  }
  for(i = 0; i < NuBridge_base_scene->numinstance; i++) {
      if (&NuBridge_base_scene->instances[i] == instance) {
        return i;
      }
  }
  return -1;
}

//NGC MATCH
void NuBridgeInit(void) {
    BridgeFree = 0;
}

//NGC MATCH
s32* NuBridgeCreate(struct nuinstance_s** instance,struct nuinstance_s* ipost,struct nuvec_s* start,struct nuvec_s* end,float width,
    short yang,float tension,float damp,float gravity,float plrweight,s32 sections,float postw,float posth,s32 postint,s32 colour) {
 
    struct nuvec_s dir; // 0x8(r1)
    struct nuvec_s dir90; // 0x18(r1)
    BridgeType* bridge;
    s32 lp;
    float t;

    if (sections > 24) {
        printf("to many sections/n");
    }
    NuBridgeOn(1);
    bridge = NuBridgeAlloc();
    if (bridge != NULL) {
        bridge->tension = tension;
        bridge->damp = damp;
        bridge->gravity = gravity;
        bridge->ipost = ipost;
        bridge->plrweight = plrweight;
        bridge->postw = postw;
        bridge->posth = posth;
        bridge->postint = postint;
        bridge->colour = colour;
        bridge->sections = sections;
        bridge->width = width;
        
        (bridge->center).x = (end->x + start->x) * 0.5f;
        (bridge->center).y = (end->y + start->y) * 0.5f;
        (bridge->center).z = (end->z + start->z) * 0.5f;
        
        dir.x = end->x - start->x;
        dir.y = end->y - start->y;
        dir.z = end->z - start->z;

        bridge->radius = SQR(dir.x * 0.5f) + SQR(dir.y * 0.5f) + SQR(dir.z * 0.5f) + 1.0f;
        bridge->inrange = 0;

        t = 1.0f / NuFsqrt(SQR(dir.x) + SQR(dir.z));
        dir90.x = -dir.z * t;
        dir90.y = 0.0f;
        dir90.z = dir.x * t;
        bridge->yang = yang;
        
        for (lp = 0; lp < sections; lp++) {
            bridge->instance[lp] = instance[lp];
            if (bridge->instance[lp]) {
                bridge->plat[lp] = NewPlatInst(&bridge->mtx[lp], NuBridgeLookupInstanceIndex(instance[lp]));
                PlatInstRotate(bridge->plat[lp], 1);
            } else {
                bridge->plat[lp] = -1;
            }
            
            NuMtxSetIdentity(&bridge->mtx[lp]);
            NuMtxPreRotateY(&bridge->mtx[lp], bridge->yang);
            
            bridge->mtx[lp]._30 = ((dir.x * lp) / (sections - 1)) + start->x;
            bridge->mtx[lp]._31 = ((dir.y * lp) / (sections - 1)) + start->y;
            bridge->mtx[lp]._32 = ((dir.z * lp) / (sections - 1)) + start->z;
            
            bridge->pos[lp][0].x = ((dir.x * lp) / (sections - 1) + start->x) - (dir90.x * width * 0.5f);
            bridge->pos[lp][0].y = (dir.y * lp) / (sections - 1) + start->y;
            bridge->pos[lp][0].z = ((dir.z * lp) / (sections - 1) + start->z) - (dir90.z * width * 0.5f);
            
            bridge->pos[lp][1].x = ((dir.x * lp) / (sections - 1) + start->x) + (dir90.x * width * 0.5f);
            bridge->pos[lp][1].y = (dir.y * lp) / (sections - 1) + start->y;
            bridge->pos[lp][1].z = ((dir.z * lp) / (sections - 1) + start->z) + (dir90.z * width * 0.5f);
            
            bridge->vel[lp][0].x = 0.0f;
            bridge->vel[lp][0].y = 0.0f;
            bridge->vel[lp][0].z = 0.0f;
            
            bridge->vel[lp][1].x = 0.0f;
            bridge->vel[lp][1].y = 0.0f;
            bridge->vel[lp][1].z = 0.0f;
            bridge->hit = 0;
        }
    }
    return (s32*)bridge;
}

//NGC MATCH
void ropesegment(struct numtl_s* mtl, struct nuvec_s* start, s32 count, s32 colour) {
    struct nuvtx_tc1_s vtx[8];
    struct nuvtx_tc1_s vtx2[8];
    struct nuvtx_tc1_s* vtx3[18];
    s32 s;
    struct nuvec_s normal;
    struct nuvec_s normal2;
    struct nuvec_s normal3;
    struct nuvec_s normal4;
    struct nuvec_s normal5;
    struct nuvec_s* next;
    
    next = start;
    
    vtx[0].diffuse = colour;
    vtx[1].diffuse = colour;
    vtx[2].diffuse = colour;
    vtx[3].diffuse = colour;
    vtx[4].diffuse = colour;
    vtx[5].diffuse = colour;
    vtx[6].diffuse = colour;
    vtx[7].diffuse = colour;
    
    vtx3[0] = &vtx2[0];
    vtx3[1] = &vtx[0];
    vtx3[2] = &vtx2[4];
    vtx3[3] = &vtx[4];
    vtx3[4] = &vtx2[1];
    vtx3[5] = &vtx[1];
    vtx3[6] = &vtx2[5];
    vtx3[7] = &vtx[5];
    vtx3[8] = &vtx2[2];
    vtx3[9] = &vtx[2];
    vtx3[10] = &vtx2[6];
    vtx3[11] = &vtx[6];
    vtx3[12] = &vtx2[3];
    vtx3[13] = &vtx[3];
    vtx3[14] = &vtx2[7];
    vtx3[15] = &vtx[7];
    vtx3[16] = &vtx2[0];
    vtx3[17] = &vtx[0];
    
    for (s = 0, next++; s < count; start++, next++, s++) {
        if (s != count - 1) {
            normal.x = next->x - start->x;
            normal.y = next->y - start->y;
            normal.z = next->z - start->z;
            NuVecNorm(&normal, &normal);
            
            if (normal.x != normal.z) {
                normal2.x = -normal.z;
                normal2.y = normal.y;
                normal2.z = normal.x;
            } else {
                normal2.x = -normal.y;
                normal2.y = normal.x;
                normal2.z = normal.z;
            }
            
            NuVecCross(&normal3, &normal, &normal2);
            NuVecNorm(&normal3, &normal3);
            
            normal4.x = ((normal3.x - normal2.x) * 0.5f + normal2.x);
            normal4.y = ((normal3.y - normal2.y) * 0.5f + normal2.y);
            normal4.z = ((normal3.z - normal2.z) * 0.5f + normal2.z);
            NuVecNorm(&normal4, &normal4);
            
            normal5.x = ((-normal2.x - normal3.x) * 0.5f + normal3.x);
            normal5.y = ((-normal2.y - normal3.y) * 0.5f + normal3.y);
            normal5.z = ((-normal2.z - normal3.z) * 0.5f + normal3.z);
            NuVecNorm(&normal5, &normal5);
            
            normal2.x = (normal2.x * 0.08f);
            normal2.y = (normal2.y * 0.08f);
            normal2.z = (normal2.z * 0.08f);
            
            normal3.x = (normal3.x * 0.08f);
            normal3.y = (normal3.y * 0.08f);
            normal3.z = (normal3.z * 0.08f);
            
            normal4.x = (normal4.x * 0.08f);
            normal4.y = (normal4.y * 0.08f);
            normal4.z = (normal4.z * 0.08f);
            
            normal5.x = (normal5.x * 0.08f);
            normal5.y = (normal5.y * 0.08f);
            normal5.z = (normal5.z * 0.08f);
        }
        
        vtx[0].pnt.x = start->x + normal2.x;
        vtx[0].pnt.y = start->y + normal2.y;
        vtx[0].pnt.z = start->z + normal2.z;
        vtx[0].nrm = normal2;
        
        vtx[1].pnt.x = start->x + normal3.x;
        vtx[1].pnt.y = start->y + normal3.y;
        vtx[1].pnt.z = start->z + normal3.z;
        vtx[1].nrm = normal3;
        
        vtx[2].pnt.x = start->x - normal2.x;
        vtx[2].pnt.y = start->y - normal2.y;
        vtx[2].pnt.z = start->z - normal2.z;
        vtx[2].nrm = normal2;
        
        vtx[3].pnt.x = start->x - normal3.x;
        vtx[3].pnt.y = start->y - normal3.y;
        vtx[3].pnt.z = start->z - normal3.z;
        vtx[3].nrm = normal3;
        
        vtx[4].pnt.x = start->x + normal4.x;
        vtx[4].pnt.y = start->y + normal4.y;
        vtx[4].pnt.z = start->z + normal4.z;
        vtx[4].nrm = normal4;
        
        vtx[5].pnt.x = start->x + normal5.x;
        vtx[5].pnt.y = start->y + normal5.y;
        vtx[5].pnt.z = start->z + normal5.z;
        vtx[5].nrm = normal5;
        
        vtx[6].pnt.x = start->x - normal4.x;
        vtx[6].pnt.y = start->y - normal4.y;
        vtx[6].pnt.z = start->z - normal4.z;
        vtx[6].nrm = normal4;
        
        vtx[7].pnt.x = start->x - normal5.x;
        vtx[7].pnt.y = start->y - normal5.y;
        vtx[7].pnt.z = start->z - normal5.z;
        vtx[7].nrm = normal5;
        
        if ((s & 1) != 0) {
            vtx[0].tc[0] = ropeu1;
            vtx[0].tc[1] = ropev2;
            
            vtx[1].tc[0] = ropeu1;
            vtx[1].tc[1] = ropev2;
            
            vtx[2].tc[0] = ropeu1;
            vtx[2].tc[1] = ropev2;
            
            vtx[3].tc[0] = ropeu1;
            vtx[3].tc[1] = ropev2;
            
            vtx[4].tc[0] = ropeu2;
            vtx[4].tc[1] = ropev2;
            
            vtx[5].tc[0] = ropeu2;
            vtx[5].tc[1] = ropev2;

            vtx[6].tc[0] = ropeu2;
            vtx[6].tc[1] = ropev2;
            
            vtx[7].tc[0] = ropeu2;
            vtx[7].tc[1] = ropev2;
        } else {
            vtx[0].tc[0] = ropeu1;
            vtx[0].tc[1] = ropev1;
            
            vtx[1].tc[0] = ropeu1;
            vtx[1].tc[1] = ropev1;
            
            vtx[2].tc[0] = ropeu2;
            vtx[2].tc[1] = ropev1;
            
            vtx[3].tc[0] = ropeu1;
            vtx[3].tc[1] = ropev1;
            
            vtx[4].tc[0] = ropeu2;
            vtx[4].tc[1] = ropev1;
            
            vtx[5].tc[0] = ropeu2;
            vtx[5].tc[1] = ropev1;
            
            vtx[6].tc[0] = ropeu2;
            vtx[6].tc[1] = ropev1;
            
            vtx[7].tc[0] = ropeu2;
            vtx[7].tc[1] = ropev1;
        }
        if (s != 0) {
            NuRndrStrip3d(vtx3, mtl, 0, 18);
        }
        
        memcpy(vtx2, vtx, sizeof(vtx));
    }
}

//NGC MATCH
void NuBrdigeDrawRope(struct numtl_s *mtl,struct nuvec_s *rope1,struct nuvec_s *rope2,s32 ropecnt,s32 *ropetab,s32 colour) {
    s32 *ropeiter;
    
    if (*ropetab < ropetab[1]) { 
        for (ropeiter = ropetab; *ropeiter < ropeiter[1]; ropeiter++) {
            ropesegment(mtl,rope1 + *ropeiter, ropeiter[1] - *ropeiter,colour);
            ropesegment(mtl,rope2 + *ropeiter, ropeiter[1] - *ropeiter,colour);
           
        }
    }
    return;
}

//NGC MATCH
void NuBridgeDraw(struct nugscn_s *scn,struct numtl_s *mtl) {
    s32 iVar7;
    s32 i;
    s32 lp;
    BridgeType *bridge;
    struct numtx_s m;
    struct nuvec4_s v;
    struct nuvec4_s vp [2];
    struct nuvec_s rope1 [512];
    struct nuvec_s rope2 [512];
    s32 ropecnt;
    s32 ropetab [8];
    
    if (NuBridgeProc != 0) {
        iVar7 = 0;
        ropecnt = 0;
        bridge = &Bridges;
        ropetab[iVar7] = ropecnt;
        
        for (i = 0; i < BridgeFree; i++) {
            //i = i + 1;
            if (bridge->inrange) {
                bridge->onscreen = FALSE;
                NuMtxSetRotationY(&m, bridge->yang);
                
                vp[0].x = 0.0f;
                vp[0].y = 0.0f;
                vp[0].z = -bridge->postw;
                vp[0].w = 1.0f;
                
                vp[1].x = 0.0f;
                vp[1].y = 0.0f;
                vp[1].z = bridge->postw;
                vp[1].w = 1.0f;
                
                for (lp = 0; lp < bridge->sections; lp++) {
                    
                    if ((bridge->instance[lp] != 0) && (NuRndrGScnObj(scn->gobjs[bridge->instance[lp]->objid], &bridge->mtx[lp]) != 0)) {
                        bridge->onscreen = TRUE;
                    }
                    
                    if ((lp == (lp / bridge->postint) * bridge->postint) && (bridge->ipost != NULL)) {
                        NuVec4MtxTransformVU0(&v, &vp[0], &bridge->mtx[lp]);
                        if (ropecnt < 512) {
                            rope1[ropecnt].x = v.x;
                            rope1[ropecnt].y = v.y + bridge->posth;
                            rope1[ropecnt].z = v.z;
                        }
                        
                        m._30 = v.x;
                        m._31 = v.y;
                        m._32 = v.z;
                        NuRndrGScnObj(scn->gobjs[bridge->ipost->objid], &m);
                        
                        v.x = 0.0f;
                        v.y = 0.0f;
                        v.z = 1.0f;
                        v.w = 1.0f;
                        NuVec4MtxTransformVU0(&v, &vp[1], &bridge->mtx[lp]);
                        
                        if (ropecnt < 512) {
                            rope2[ropecnt].x = v.x;
                            rope2[ropecnt].y = v.y + bridge->posth;
                            rope2[ropecnt].z = v.z;
                            ropecnt++;
                        }
                        
                        m._30 = v.x;
                        m._31 = v.y;
                        m._32 = v.z;
                        NuRndrGScnObj(scn->gobjs[bridge->ipost->objid], &m);
                    }
                }
                
                if (ropetab[iVar7] != ropecnt) {
                    ropetab[iVar7 + 1] = ropecnt;
                    iVar7++;
                }
            }
            bridge++;
        }
        
        ropetab[iVar7 + 1] = ropecnt;
        if (1 < ropecnt) {
            ropemat = m;
            NuBrdigeDrawRope(mtl, rope1, rope2, ropecnt, ropetab, Bridges->colour);
        }
    }
    return;
}

//61% NGC
void NuBridgeUpdate(struct nuvec_s *playerpos) {
  s32 i;
  BridgeType *bridge;
  struct nuvec_s dir; // ??
  //float fVar2;
  float fVar3;
  float fVar7;
  float fVar8;
  s32 uVar9;
  s32 iVar10;
  s32 iVar11;
  s32 sections;
  s32 iVar12;
  s32 iVar13;
  s32 iVar14;
  float dVar15;
  //double dVar16;
  //double dVar17;
  float dVar18;
  //double dVar19;
  float dVar20;
  float dVar21;
  float fVar22;
  float fVar23;
  float fVar24;
  //double local_80;
  //struct numtx_s *m;
  //struct numtx_s *m_00;
  //BridgeType *bridge2;
  //BridgeType *bridgetmp;
  
  if (NuBridgeProc != 0) {
    //i = 0;
    bridge = Bridges;
    //if (0 < BridgeFree) {
      //dVar16 = 3.0;
      //dVar17 = 0.5f;
      for (i = 0; i < BridgeFree; i++) {
        fVar24 = (bridge->center).x - global_camera.mtx._30;
        fVar22 = (bridge->center).y - global_camera.mtx._31;
        fVar23 = (bridge->center).z - global_camera.mtx._32;
        if (global_camera.farclip * global_camera.farclip + bridge->radius <
            fVar23 * fVar23 + fVar24 * fVar24 + fVar22 * fVar22) {
          bridge->inrange = 1;
          if (bridge->onscreen != 0) {
            if (bridge->hit != 0) {
              //dVar20 = 0.5f;
              sections = bridge->sections;
              //fVar22 = bridge->pos[0]->x;
              //fVar24 = bridge->pos[1]->x;
              //local_80 = sections - 1;
              fVar7 = ((((bridge->pos[0][sections].x) + (bridge->pos[sections][0].z)) - bridge->pos[0]->x) - fVar24) * 0.5f;
              //fVar23 = bridge->pos[0]->z;
              //dVar18 = 0.0f;
              //fVar2 = bridge->pos[1]->z;
              fVar8 = ((((bridge->pos[sections][1].x) +
                        bridge->pos[sections][1].y) - bridge->pos[sections][1].z) - bridge->pos[1]->z) * 0.5f;
              fVar3 = fVar7 * fVar7 + fVar8 * fVar8;
              dVar21 = (((fVar7 * (playerpos->x - (bridge->pos[0]->x + fVar24) * 0.5f) +
                                 fVar8 * (playerpos->z - (bridge->pos[sections][1].z + bridge->pos[1]->z) * 0.5f)) *
                                (sections - 1)) / fVar3);
              if (dVar21 < 0.0f) {
                dVar21 = 0.0f;
              }
              //dVar19 = 1.0;
              //local_80 = sections;
              dVar15 = ((sections) - 1.0f);
              if (dVar15 > dVar21) {
              sections = (s32)dVar21;
              }
                dVar21 = dVar15;
              //local_80 = sections;
              dVar15 = (dVar21 - (sections));
              //fVar22 = NuFsqrt(fVar3);
              dVar21 = ((2.0f / (NuFsqrt(fVar3) * bridge->width)) * (fVar8 * (playerpos->x - ((bridge->pos[0]->x + bridge->pos[1]->x) * 0.5f)) +
                               fVar7 * ((bridge->pos[0]->z + bridge->pos[1]->z) * 0.5f - playerpos->z)));
              if (dVar21 > 1.0f) {
                dVar21 = 1.0f;
              }
              if (dVar21 < - 1.0f) {
                dVar21 = - 1.0f;
              }
              fVar22 = bridge->gravity * (dVar21 + 3.0f) * 0.25f;
              fVar24 = bridge->gravity * (3.0f - dVar21) * 0.25f;
              if (dVar15 == 0.0f) {
                if ((sections > 0) && (sections < bridge->sections - 1)) {
                  bridge->vel[sections][0].y = fVar24 * bridge->plrweight + bridge->vel[sections][0].y;
                  fVar22 = fVar22 * bridge->plrweight + bridge->vel[sections][1].y;
                  uVar9 = sections;
                  bridge->vel[uVar9][1].y = fVar22;
                }
              }
              else {
                if ((sections > 0) && (sections < bridge->sections - 1)) {
                  bridge->vel[sections][0].y =
                       fVar24 * bridge->plrweight * (1.0f - dVar15) + bridge->vel[sections][0].y;
                  bridge->vel[sections + 1]->y =
                       fVar22 * bridge->plrweight * (1.0f - dVar15) + bridge->vel[sections][1].y;
                }
                uVar9 = sections + 1;
                if ((s32)uVar9 < bridge->sections - 1) {
                  bridge->vel[sections][2].y = ((fVar24 * bridge->plrweight) * dVar15 +
                              bridge->vel[sections][2].y);
                  bridge->vel[uVar9][1].y = ((fVar22 * bridge->plrweight) * dVar15 + bridge->vel[sections][3].y);
                  //bridge->vel[uVar9][1].y = fVar22;
                }
              }
              bridge->hit--;
            }
            iVar10 = PlatInstGetHit((s32)bridge->plat[0]);
            if (iVar10 != 0) {
              bridge->hit = 5;
            }
            iVar10 = PlatInstGetHit((s32)bridge->plat[bridge->sections - 1]);
            if (iVar10 != 0) {
              bridge->hit = 5;
            }
            //iVar10 = 1;
            //m = bridge->mtx;
            //if (1 < bridge->sections - 1) {
              //dVar21 = 0.5f;
              iVar12 = 0x30;
              iVar13 = 0;
              iVar14 = 0x18;
              //bridgetmp = bridge;
              for (iVar10 = 1; iVar10 < bridge->sections - 1; iVar10++, iVar13++) {
                //bridge2 = (BridgeType *)(bridgetmp->pos + 2);
                //fVar22 = bridgetmp->vel[2]->x;
                //fVar24 = bridgetmp->vel[2]->y;
                //fVar23 = bridgetmp->vel[2]->z;
                //fVar22 = bridgetmp->vel[3]->x;
                //fVar2 = bridgetmp->vel[3]->y;
                //fVar3 = bridgetmp->vel[3]->z;
                Bridges[iVar10].vel[2]->x = -Bridges[iVar10].vel[2]->x * bridge->damp + Bridges[iVar10].vel[2]->x;
                Bridges[iVar10].vel[2]->y = Bridges[iVar10].vel[2]->y + (bridge->gravity - Bridges[iVar10].vel[2]->y * bridge->damp);
                Bridges[iVar10].vel[2]->z = -Bridges[iVar10].vel[2]->z * bridge->damp + Bridges[iVar10].vel[2]->z;
                Bridges[iVar10].vel[3]->x = -Bridges[iVar10].vel[3]->x * bridge->damp + Bridges[iVar10].vel[3]->x;
                Bridges[iVar10].vel[3]->y = Bridges[iVar10].vel[3]->y + (bridge->gravity - Bridges[iVar10].vel[3]->y * bridge->damp);
                Bridges[iVar10].vel[3]->z = -Bridges[iVar10].vel[3]->z * bridge->damp + Bridges[iVar10].vel[3]->z;
                iVar11 = PlatInstGetHit((s32)bridge->plat[iVar10]);
                if (iVar11 != 0) {
                  bridge->hit = 5;
                }
                //iVar10 = iVar10 + 1;
                dVar20 = NuFabs((Bridges[iVar10].pos[0]->x - bridge->pos[2]->x));
                dVar18 = (Bridges[iVar10].pos[0]->x - bridge->pos[2]->x);
                Bridges[iVar10].vel[2]->x = Bridges[iVar10].vel[2]->x +(((dVar20 * dVar18) * bridge->tension) * 0.5f +
                            (dVar18 * bridge->tension));
                dVar20 = NuFabs(bridge->pos[iVar13]->y - bridge->pos[iVar14]->y);
                dVar18 = bridge->pos[iVar13]->y - bridge->pos[iVar14]->y;
                Bridges[iVar10].vel[2]->y = Bridges[iVar10].vel[2]->y + (((dVar20 * dVar18) *
                                            bridge->tension) * 0.5f + (dVar18 * bridge->tension));
                dVar20 = NuFabs(((bridge->pos[iVar13]->z) - (bridge->pos[iVar14]->z)));
                dVar18 = ((bridge->pos[iVar13]->z) - (bridge->pos[iVar14]->z));
                //fVar22 = bridgetmp->pos[4]->x;
                //fVar24 = bridge2->pos[0]->x;
                Bridges[iVar10].vel[2]->z = Bridges[iVar10].vel[2]->z + (((dVar20 * dVar18) *
                                            bridge->tension) * 0.5f + (dVar18 * bridge->tension));
                dVar20 = NuFabs((Bridges[iVar10].pos[4]->x - bridge->pos[2]->x));
                dVar18 = (Bridges[iVar10].pos[4]->x - bridge->pos[2]->x);
                Bridges[iVar10].vel[2]->x = Bridges[iVar10].vel[2]->x +
                     (((dVar20 * dVar18) * bridge->tension) * 0.5f + (dVar18 * bridge->tension));
                dVar20 = NuFabs((bridge->pos[iVar12][0].y - bridge->pos[iVar14][0].y));
                dVar18 = (bridge->pos[iVar12][0].y - bridge->pos[iVar14][0].y);
                Bridges[iVar10].vel[2]->y = Bridges[iVar10].vel[2]->y +
                     (((dVar20 * dVar18) * bridge->tension) * 0.5f + (dVar18 * bridge->tension));
                dVar20 = NuFabs(bridge->pos[iVar12][0].z - bridge->pos[iVar14][0].z);
                dVar18 = (bridge->pos[iVar12][0].z - bridge->pos[iVar14][0].z);
                Bridges[iVar10].vel[2]->z = Bridges[iVar10].vel[2]->z +
                     (((dVar20 * dVar18) * bridge->tension) * 0.5f + (dVar18 * bridge->tension));
                dVar20 = NuFabs(bridge->pos[iVar13][1].x - bridge->pos[iVar14][1].x);
                dVar18 = (bridge->pos[iVar13][1].x - bridge->pos[iVar14][1].x);
                Bridges[iVar10].vel[3]->x =
                     Bridges[iVar10].vel[3]->x + (((dVar20 * dVar18) * bridge->tension) * 0.5f + (dVar18 * bridge->tension));
                dVar20 = NuFabs(bridge->pos[iVar13][1].y - bridge->pos[iVar14][1].y);
                dVar18 = (bridge->pos[iVar13]->y - bridge->pos[iVar14]->y);
                Bridges[iVar10].vel[3]->y = Bridges[iVar10].vel[3]->y + (((dVar20 * dVar18) *  bridge->tension) * 0.5f +
                            (dVar18 * bridge->tension));
                dVar20 = NuFabs(bridge->pos[iVar13][1].z - bridge->pos[iVar14][1].z);
                //pfVar4 = &bridge->pos[iVar13][1].z;
                //iVar13 = iVar13 + 0x18;
                dVar18 = bridge->pos[iVar13][1].z - bridge->pos[iVar14][1].z;
                Bridges[iVar10].vel[3]->z = Bridges[iVar10].vel[3]->z +
                     (((dVar20 * dVar18) * bridge->tension) * 0.5f + (dVar18 * bridge->tension));
                dVar20 = NuFabs(bridge->pos[iVar12][1].x - bridge->pos[iVar14][1].x);
                dVar18 = bridge->pos[iVar12][1].x - bridge->pos[iVar14][1].x;
                Bridges[iVar10].vel[3]->x = Bridges[iVar10].vel[3]->x +
                     (((dVar20 * dVar18) * bridge->tension) * 0.5f + (dVar18 * bridge->tension));
                dVar20 = NuFabs(bridge->pos[iVar12][1].y - bridge->pos[iVar14][1].y);
                dVar18 = bridge->pos[iVar12][1].y - bridge->pos[iVar14][1].y;
                Bridges[iVar10].vel[3]->y = Bridges[iVar10].vel[3]->y + (((dVar20 * dVar18) * bridge->tension) * 0.5f +
                            (dVar18 * bridge->tension));
                dVar20 = NuFabs(bridge->pos[iVar12][1].z - bridge->pos[iVar14][1].z);
                //pfVar4 = &bridge->pos[iVar12][0].x;
                //pfVar5 = &bridge->pos[iVar14][0].x;
                //iVar12 = iVar12 + 0x18;
                //iVar14 = iVar14 + 0x18;
                dVar18 = (bridge->pos[iVar12][0].x - bridge->pos[iVar14][0].x);
                Bridges[iVar10].vel[3]->z =
                     Bridges[iVar10].vel[3]->z + (((dVar20 * dVar18) *  bridge->tension) * 0.5f +  (dVar18 * bridge->tension));
                //bridgetmp = bridge2;
              } //while (iVar10 < bridge->sections - 1);
            //}
            //if (1 < bridge->sections - 1) {
              //dVar21 = 0.5f;
              //iVar10 = 1;
              for (iVar13 = 1; iVar13 < bridge->sections - 1; iVar13++) {
                //iVar13 = iVar10 + 1;
                //m_00 = &bridge->mtx[iVar13];
                bridge->pos[iVar13][0].x = bridge->pos[iVar13][0].x + bridge->vel[iVar13][0].x;
                bridge->pos[iVar13][0].y = bridge->pos[iVar13][0].y + bridge->vel[iVar13][0].y;
                bridge->pos[iVar13][0].z = bridge->pos[iVar13][0].z + bridge->vel[iVar13][0].z;
                bridge->pos[iVar13][1].x =
                     bridge->pos[iVar13][1].x + bridge->vel[iVar13][1].x;
                bridge->pos[iVar13][1].y =
                     bridge->pos[iVar13][1].y + bridge->vel[iVar13][1].y;
                bridge->pos[iVar13][1].z =
                     bridge->pos[iVar13][1].z + bridge->vel[iVar13][1].z;
                NuMtxSetRotationY(&bridge->mtx[iVar13],(s32)bridge->yang);
                fVar22 = bridge->pos[iVar13][1].z - bridge->pos[iVar13][0].z;
                fVar24 = bridge->pos[iVar13][1].x - bridge->pos[iVar13][0].x;
                fVar22 = NuFsqrt(fVar24 * fVar24 + fVar22 * fVar22);
                iVar12 = NuAtan2D(bridge->pos[iVar13][1].y - bridge->pos[iVar13][0].y,fVar22);
                NuMtxPreRotateX(&bridge->mtx[iVar13],iVar12);
                fVar22 = ((bridge->pos[iVar13][2].z + bridge->pos[iVar13][3].z) -
                        (bridge->pos[iVar10]->x)) - (bridge->pos[iVar10]->x);
                fVar24 = ((bridge->pos[iVar13][2].x + bridge->pos[iVar13][3].x) -
                         bridge->pos[20][iVar10].y) -
                         bridge->pos[21][iVar10].y;
                fVar22 = NuFsqrt(fVar24 * fVar24 + fVar22 * fVar22);
                iVar12 = NuAtan2D(((bridge->pos[iVar13][2].y + bridge->pos[iVar13][3].y)
                                  - bridge->pos[20][iVar10].z) - bridge->pos[22][iVar10].y,fVar22);
                NuMtxPreRotateZ(&bridge->mtx[iVar13],iVar12);
                bridge->mtx[iVar10]._30 =
                     ((bridge->pos[iVar13][0].x + bridge->pos[iVar13][1].x) * 0.5f);
                bridge->mtx[iVar10]._31 =
                     ((bridge->pos[iVar13][0].y + bridge->pos[iVar13][1].y) * 0.5f);
                bridge->mtx[iVar10]._32 =
                     ((bridge->pos[iVar13][0].z + bridge->pos[iVar13][1].z) * 0.5f);
                iVar10 = iVar13;
              } //while (iVar13 < bridge->sections - 1);
            //}
            NuMtxSetRotationY(&bridge->mtx[0],(s32)bridge->yang);
            fVar22 = bridge->pos[1]->z - bridge->pos[0]->z;
            fVar24 = bridge->pos[1]->x - bridge->pos[0]->x;
            fVar22 = NuFsqrt(fVar24 * fVar24 + fVar22 * fVar22);
            iVar10 = NuAtan2D(bridge->pos[1]->y - bridge->pos[0]->y,fVar22);
            NuMtxPreRotateX(&bridge->mtx[0],iVar10);
            fVar22 = ((bridge->pos[2]->z + bridge->pos[3]->z) - bridge->pos[0]->z) - bridge->pos[1]->z;
            fVar24 = ((bridge->pos[2]->x + bridge->pos[3]->x) - bridge->pos[0]->x) - bridge->pos[1]->x;
            fVar23 = NuFsqrt(fVar24 * fVar24 + fVar22 * fVar22);
            fVar22 = bridge->pos[2]->y;
            fVar24 = bridge->pos[3]->y;
            dVar21 = ((bridge->pos[0]->y + bridge->pos[1]->y + fVar22 + fVar24) * 0.25f);
            iVar10 = NuAtan2D((fVar22 + fVar24) - (dVar21 + dVar21),fVar23);
            NuMtxPreRotateZ(&bridge->mtx[0],iVar10);
            //fVar22 = bridge->pos[1]->x;
            //fVar24 = bridge->pos[1]->z;
            //fVar23 = bridge->pos[0]->x;
            //fVar2 = bridge->pos[0]->z;
            bridge->mtx[0]._31 = dVar21;
            bridge->mtx[0]._30 = ((bridge->pos[0]->x + bridge->pos[1]->x) * 0.5f);
            bridge->mtx[0]._32 = ((bridge->pos[0]->z + bridge->pos[1]->z) * 0.5f);
            NuMtxSetRotationY(bridge->mtx + bridge->sections - 1,bridge->yang);
            iVar10 = (s32)bridge->sections;
            fVar22 = bridge->pos[iVar10][15].x - bridge->pos[iVar10][10].x;
            fVar24 = bridge->pos[17][iVar10].y - bridge->pos[16][iVar10].y;
            fVar22 = NuFsqrt(fVar24 * fVar24 + fVar22 * fVar22);
            iVar10 = NuAtan2D(bridge->pos[20][iVar10].y - bridge->pos[18][bridge->sections].y, fVar22);
            NuMtxPreRotateX(&bridge->mtx[0],iVar10);
            iVar10 = (s32)bridge->sections;
            fVar22 = ((bridge->pos[21][iVar10].x + bridge->pos[22][iVar12].x) -
                     bridge->pos[iVar10][10].x) - bridge->pos[iVar10][15].x;
            fVar24 = ((bridge->pos[20][iVar10].y +
                      bridge->pos[21][iVar10].y) - bridge->pos[16][iVar10].y) - bridge->pos[17][iVar10].y;
            fVar24 = NuFsqrt(fVar24 * fVar24 + fVar22 * fVar22);
            iVar12 = (s32)bridge->sections;
            fVar22 = bridge->pos[22][iVar12].x + bridge->pos[22][iVar12].y;
            dVar21 = ((fVar22 + bridge->pos[18][iVar12].y + bridge->pos[20][iVar10].y) * 0.25f);
            iVar10 = NuAtan2D(fVar22 - (dVar21 + dVar21),fVar24);
            NuMtxPreRotateZ(bridge->mtx + iVar12 - 1,iVar10);
            iVar10 = (s32)bridge->sections;
            bridge->mtx[iVar10 - 1]._30 = ((bridge->pos[20][iVar10].y +  bridge->pos[21][iVar10].y) * 0.5f);
            bridge->mtx[bridge->sections - 1]._31 = dVar21;
            iVar10 = (s32)bridge->sections;
            bridge->mtx[iVar10 - 1]._32 = ((bridge->pos[20][iVar10].z + bridge->pos[21][iVar10].z) * 0.5f);
          }
        }
        else {
          bridge->inrange = 0;
        }
        //i = i + 1;
        bridge++;
      } //while (i < BridgeFree);
    //}
  }
  return;
}