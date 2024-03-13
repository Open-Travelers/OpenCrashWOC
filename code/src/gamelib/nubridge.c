
// Size: 0xB4C, DWARF: 0x73CED2
struct Bridge_s
{
    struct NuVec pos[24][2]; // Offset: 0x0, DWARF: 0x73CEE4
    struct NuVec vel[24][2]; // Offset: 0x240, DWARF: 0x73CF0C
    struct numtx_s mtx[24]; // Offset: 0x480, DWARF: 0x73CF34
    struct nuinstance_s* instance[24]; // Offset: 0xA80, DWARF: 0x73CF5C
    struct nuinstance_s* ipost; // Offset: 0xAE0, DWARF: 0x73CF89
    struct NuVec center; // Offset: 0xAE4, DWARF: 0x73CFB6
    float radius; // Offset: 0xAF0, DWARF: 0x73CFE1
    short plat[24]; // Offset: 0xAF4, DWARF: 0x73D00A
    char inrange; // Offset: 0xB24, DWARF: 0x73D033
    char onscreen; // Offset: 0xB25, DWARF: 0x73D05D
    char sections; // Offset: 0xB26, DWARF: 0x73D088
    char postint; // Offset: 0xB27, DWARF: 0x73D0B3
    s32 colour; // Offset: 0xB28, DWARF: 0x73D0DD
    short hit; // Offset: 0xB2C, DWARF: 0x73D106
    short yang; // Offset: 0xB2E, DWARF: 0x73D12C
    float width; // Offset: 0xB30, DWARF: 0x73D153
    float tension; // Offset: 0xB34, DWARF: 0x73D17B
    float gravity; // Offset: 0xB38, DWARF: 0x73D1A5
    float damp; // Offset: 0xB3C, DWARF: 0x73D1CF
    float plrweight; // Offset: 0xB40, DWARF: 0x73D1F6
    float postw; // Offset: 0xB44, DWARF: 0x73D222
    float posth; // Offset: 0xB48, DWARF: 0x73D24A
};

struct nugscn_s* NuBridge_base_scene;
#define SQR(x) ((x)*(x))


//NGC MATCH
struct Bridge_s* NuBridgeAlloc(void) {
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
    struct Bridge_s* bridge;
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

//97% NGC
void ropesegment(struct numtl_s *mtl,struct nuvec_s *start,s32 count,s32 colour) {
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

    
    next = start + 1;
    vtx[0].diffuse = colour;
    vtx[1].diffuse = colour;
    vtx[2].diffuse = colour;
    vtx[3].diffuse = colour;
    vtx[4].diffuse = colour;
    vtx[5].diffuse = colour;
    vtx[6].diffuse = colour;
    vtx[7].diffuse = colour;
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
    vtx3[0] = vtx2;
    vtx3[1] = vtx;
    vtx3[16] = vtx2;
    vtx3[17] = vtx;
        for (s = 0; s < count; s++) {
            if (s != count - 1U) {
                normal.x = next->x - start->x;
                normal.y = next->y - start->y;
                normal.z = next->z - start->z;
                NuVecNorm(&normal,&normal);
                if (normal.x != normal.z) {
                    normal2.x = -normal.z;
                    normal2.y = normal.y;
                    normal2.z = normal.x;
                }
                else {
                    normal2.x = -normal.y;
                    normal2.y = normal.x;
                    normal2.z = normal.z;
                }
                NuVecCross(&normal3,&normal,&normal2);
                NuVecNorm(&normal3,&normal3);
                normal4.x = ((normal3.x - normal2.x) * 0.5f  + normal2.x);
                normal4.y = ((normal3.y - normal2.y) * 0.5f  + normal2.y);
                normal4.z = ((normal3.z - normal2.z) * 0.5f  + normal2.z);
                NuVecNorm(&normal4,&normal4);
                normal5.x = ((-normal2.x - normal3.x) * 0.5f + normal3.x);
                normal5.y = ((-normal2.y - normal3.y) * 0.5f + normal3.y);
                normal5.z = ((-normal2.z - normal3.z) * 0.5f + normal3.z);
                NuVecNorm(&normal5,&normal5);
                normal2.x = (normal2.x * 0.079999998f);
                normal2.y = (normal2.y * 0.079999998f);
                normal2.z = (normal2.z * 0.079999998f);
                normal3.x = (normal3.x * 0.079999998f);
                normal3.y = (normal3.y * 0.079999998f);
                normal3.z = (normal3.z * 0.079999998f);
                normal4.x = (normal4.x * 0.079999998f);
                normal4.y = (normal4.y * 0.079999998f);
                normal4.z = (normal4.z * 0.079999998f);
                normal5.x = (normal5.x * 0.079999998f);
                normal5.y = (normal5.y * 0.079999998f);
                normal5.z = (normal5.z * 0.079999998f);
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
                vtx[3].tc[0] = ropeu1;
                vtx[7].tc[0] = ropeu2;
                vtx[7].tc[1] = ropev2;
            }
            else {
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
                NuRndrStrip3d(vtx3,mtl,0,0x12);
            }
            memcpy(vtx2, vtx, 0x120);
            //*vtx2 = *vtx;
            start = start + 1;
            next = next + 1;
        }
    return;
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
    struct Bridge_s *bridge;
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

//36% NGC
void NuBridgeUpdate(struct nuvec_s *playerpos) {
/*
  char cVar1;
  float fVar2;
  float fVar3;
  float *pfVar4;
  float *pfVar5;
  short sVar6;
  float fVar7;
  float fVar8;
  u32 uVar9;
  struct numtx_s *m;
  int iVar10;
  int iVar11;
  u32 sections;
  int iVar12;
  int iVar13;
  struct numtx_s *m_00;
  struct Bridge_s *bridge2;
  int iVar14;
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
  struct Bridge_s *bridge1;
  int i;
  //double local_80;
  struct Bridge_s *bridge;
  
  if (NuBridgeProc != 0) {
    //i = 0;
    bridge1 = Bridges;
    //if (0 < BridgeFree) {
      //dVar16 = 3.0;
      //dVar17 = 0.5f;
      for (i = 0; i < BridgeFree; i++) {
        fVar22 = (bridge1->center).y - global_camera.mtx._31;
        fVar24 = (bridge1->center).x - global_camera.mtx._30;
        fVar23 = (bridge1->center).z - global_camera.mtx._32;
        if (global_camera.farclip * global_camera.farclip + bridge1->radius <
            fVar23 * fVar23 + fVar24 * fVar24 + fVar22 * fVar22) {
          bridge1->inrange = '\x01';
          if (bridge1->onscreen != '\0') {
            if (bridge1->hit != 0) {
              dVar20 = 0.5f;
              sections = (u32)bridge1->sections;
              fVar22 = bridge1->pos[0]->x;
              fVar24 = bridge1->pos[1]->x;
              //local_80 = sections - 1;
              fVar7 = ((((bridge1->pos[0][sections].x) + (bridge1->pos[sections][0].z)) - fVar22) - fVar24) * 0.5f;
              //fVar23 = bridge1->pos[0]->z;
              dVar18 = 0.0f;
              fVar2 = bridge1->pos[1]->z;
              fVar8 = ((((bridge1->pos[sections][1].x) +
                        bridge1->pos[sections][1].y) - bridge1->pos[sections][1].z) - fVar2) * 0.5f;
              fVar3 = fVar7 * fVar7 + fVar8 * fVar8;
              dVar21 = (((fVar7 * (playerpos->x - (fVar22 + fVar24) * 0.5f) +
                                 fVar8 * (playerpos->z - (bridge1->pos[sections][1].z + fVar2) * 0.5f)) *
                                (sections - 1)) / fVar3);
              if (dVar21 < 0.0f) {
                dVar21 = dVar18;
              }
              //dVar19 = 1.0;
              //local_80 = sections;
              dVar15 = ((sections) - 1.0f);
              if (dVar15 > dVar21) {
              sections = (u32)dVar21;
              }
                dVar21 = dVar15;
              //local_80 = sections;
              dVar15 = (dVar21 - (sections));
              fVar22 = NuFsqrt(fVar3);
              dVar21 = ((2.0f / (fVar22 * bridge1->width)) * (fVar8 * (playerpos->x - ((bridge1->pos[0]->x + bridge1->pos[1]->x) * 0.5f)) +
                               fVar7 * ((bridge1->pos[0]->z + bridge1->pos[1]->z) * 0.5f - playerpos->z)));
              if (1.0f < dVar21) {
                dVar21 = 1.0f;
              }
              if (dVar21 < -1.0f) {
                dVar21 = -1.0f;
              }
              fVar22 = bridge1->gravity * (dVar21 + 3.0f) * 0.25f;
              fVar24 = bridge1->gravity * (3.0f - dVar21) * 0.25f;
              if (dVar15 == dVar18) {
                if ((0 < (int)sections) && ((int)sections < bridge1->sections + -1)) {
                  bridge1->vel[sections * 2]->y = fVar24 * bridge1->plrweight + bridge1->vel[sections * 2]->y;
                  fVar22 = fVar22 * bridge1->plrweight + bridge1->vel[sections * 2 + 1]->y;
                  uVar9 = sections;
LAB_8008d11c:
                  bridge1->vel[uVar9 * 2 + 1]->y = fVar22;
                }
              }
              else {
                if ((0 < (int)sections) && ((int)sections < bridge1->sections + -1)) {
                  bridge1->vel[sections * 2]->y =
                       fVar24 * bridge1->plrweight * (1.0f - dVar15) + bridge1->vel[sections * 2]->y;
                  bridge1->vel[sections + 1]->y =
                       fVar22 * bridge1->plrweight * (1.0f - dVar15) + bridge1->vel[sections * 2 + 1]->y;
                }
                uVar9 = sections + 1;
                if ((int)uVar9 < bridge1->sections + -1) {
                  bridge1->vel[sections * 2 + 2]->y = ((fVar24 * bridge1->plrweight) * dVar15 +
                              bridge1->vel[sections * 2 + 2]->y);
                  fVar22 = ((fVar22 * bridge1->plrweight) * dVar15 + bridge1->vel[sections * 2 + 3]->y);
                  goto LAB_8008d11c;
                }
              }
              bridge1->hit = bridge1->hit + -1;
            }
            iVar10 = PlatInstGetHit((int)bridge1->plat[0]);
            if (iVar10 != 0) {
              bridge1->hit = 5;
            }
            iVar10 = PlatInstGetHit((int)bridge1->plat[bridge1->sections + -1]);
            if (iVar10 != 0) {
              bridge1->hit = 5;
            }
            //iVar10 = 1;
            m = bridge1->mtx;
            //if (1 < bridge1->sections + -1) {
              dVar21 = 0.5f;
              iVar12 = 0x30;
              iVar13 = 0;
              iVar14 = 0x18;
              bridge = bridge1;
              for (iVar10 = 1; iVar10 < bridge1->sections + -1; iVar10++) {
                bridge2 = (struct Bridge_s *)(bridge->pos + 2);
                fVar22 = bridge->vel[2]->x;
                fVar24 = bridge->vel[2]->y;
                fVar23 = bridge->vel[2]->z;
                bridge->vel[2]->x = -fVar22 * bridge1->damp + fVar22;
                fVar22 = bridge->vel[3]->x;
                fVar2 = bridge->vel[3]->y;
                fVar3 = bridge->vel[3]->z;
                bridge->vel[2]->y = fVar24 + (bridge1->gravity - fVar24 * bridge1->damp);
                bridge->vel[2]->z = -fVar23 * bridge1->damp + fVar23;
                bridge->vel[3]->x = -fVar22 * bridge1->damp + fVar22;
                bridge->vel[3]->y = fVar2 + (bridge1->gravity - fVar2 * bridge1->damp);
                bridge->vel[3]->z = -fVar3 * bridge1->damp + fVar3;
                iVar11 = PlatInstGetHit((int)bridge1->plat[iVar10]);
                if (iVar11 != 0) {
                  bridge1->hit = 5;
                }
                //iVar10 = iVar10 + 1;
                dVar20 = NuFabs((bridge->pos[0]->x - bridge2->pos[0]->x));
                dVar18 = (bridge->pos[0]->x - bridge2->pos[0]->x);
                bridge->vel[2]->x = bridge->vel[2]->x +(((dVar20 * dVar18) * bridge1->tension) * dVar21 +
                            (dVar18 * bridge1->tension));
                dVar20 = NuFabs((*(float *)((int)&bridge1->pos[iVar13]->y) - *(float *)((int)&bridge1->pos[iVar14]->y)));
                dVar18 = (*(float *)((int)&bridge1->pos[iVar13]->y) - *(float *)((int)&bridge1->pos[iVar14]->y));
                bridge->vel[2]->y = bridge->vel[2]->y + (((dVar20 * dVar18) *
                                            bridge1->tension) * dVar21 + (dVar18 * bridge1->tension));
                dVar20 = NuFabs(((bridge1->pos[iVar13]->z) - (bridge1->pos[iVar14]->z)));
                dVar18 = ((bridge1->pos[iVar13]->z + iVar13) - (bridge1->pos[iVar14]->z));
                fVar22 = bridge->pos[4]->x;
                fVar24 = bridge2->pos[0]->x;
                bridge->vel[2]->z = bridge->vel[2]->z + (((dVar20 * dVar18) *
                                            bridge1->tension) * dVar21 + (dVar18 * bridge1->tension));
                dVar20 = NuFabs((fVar22 - fVar24));
                dVar18 = (bridge->pos[4]->x - bridge2->pos[0]->x);
                bridge->vel[2]->x = bridge->vel[2]->x +
                     (((dVar20 * dVar18) * bridge1->tension) * dVar21 + (dVar18 * bridge1->tension));
                dVar20 = NuFabs((*(float *)((int)&bridge1->pos[iVar12]->y) -
                                                *(float *)((int)&bridge1->pos[iVar14]->y)));
                dVar18 = (*(float *)((int)&bridge1->pos[iVar12]->y) -
                                 *(float *)((int)&bridge1->pos[iVar14]->y));
                bridge->vel[2]->y = bridge->vel[2]->y +
                     (((dVar20 * dVar18) * bridge1->tension) * dVar21 + (dVar18 * bridge1->tension));
                dVar20 = NuFabs((*(float *)((int)&bridge1->pos[0]->z + iVar12) -
                                                *(float *)((int)&bridge1->pos[0]->z + iVar14)));
                dVar18 = (*(float *)((int)&bridge1->pos[0]->z + iVar12) -
                                 *(float *)((int)&bridge1->pos[0]->z + iVar14));
                bridge->vel[2]->z = bridge->vel[2]->z +
                     (((dVar20 * dVar18) * bridge1->tension) * dVar21 + (dVar18 * bridge1->tension));
                dVar20 = NuFabs((*(float *)((int)&bridge1->pos[1]->x + iVar13) -
                                                *(float *)((int)&bridge1->pos[1]->x + iVar14)));
                dVar18 = (*(float *)((int)&bridge1->pos[1]->x + iVar13) -
                                 *(float *)((int)&bridge1->pos[1]->x + iVar14));
                bridge->vel[3]->x =
                     bridge->vel[3]->x + (((dVar20 * dVar18) * bridge1->tension) * dVar21 + (dVar18 * bridge1->tension));
                dVar20 = NuFabs((*(float *)((int)&bridge1->pos[1]->y + iVar13) -
                                                *(float *)((int)&bridge1->pos[1]->y + iVar14)));
                dVar18 = (bridge1->pos[iVar13]->y - bridge1->pos[iVar14]->y);
                bridge->vel[3]->y = bridge->vel[3]->y + (((dVar20 * dVar18) *  bridge1->tension) * dVar21 +
                            (dVar18 * bridge1->tension));
                dVar20 = NuFabs((*(float *)((int)&bridge1->pos[1]->z + iVar13) -
                                                *(float *)((int)&bridge1->pos[1]->z + iVar14)));
                pfVar4 = (float *)((int)&bridge1->pos[1]->z + iVar13);
                iVar13 = iVar13 + 0x18;
                dVar18 = (*pfVar4 - *(float *)((int)&bridge1->pos[1]->z + iVar14));
                bridge->vel[3]->z = bridge->vel[3]->z +
                     (((dVar20 * dVar18) * bridge1->tension) * dVar21 + (dVar18 * bridge1->tension));
                dVar20 = NuFabs((*(float *)((int)&bridge1->pos[1]->x + iVar12) -
                                                *(float *)((int)&bridge1->pos[1]->x + iVar14)));
                dVar18 = (*(float *)((int)&bridge1->pos[1]->x + iVar12) -
                                 *(float *)((int)&bridge1->pos[1]->x + iVar14));
                bridge->vel[3]->x = bridge->vel[3]->x +
                     (((dVar20 * dVar18) * bridge1->tension) * dVar21 + (dVar18 * bridge1->tension));
                dVar20 = NuFabs((*(float *)((int)&bridge1->pos[1]->y + iVar12) -
                                                *(float *)((int)&bridge1->pos[1]->y + iVar14)));
                dVar18 = (*(float *)((int)&bridge1->pos[1]->y + iVar12) -
                                 *(float *)((int)&bridge1->pos[1]->y + iVar14));
                bridge->vel[3]->y = bridge->vel[3]->y + (((dVar20 * dVar18) * bridge1->tension) * dVar21 +
                            (dVar18 * bridge1->tension));
                dVar20 = NuFabs((*(float *)((int)&bridge1->pos[1]->z + iVar12) -
                                                *(float *)((int)&bridge1->pos[1]->z + iVar14)));
                pfVar4 = bridge1->pos[iVar12];
                pfVar5 = bridge1->pos[iVar14];
                iVar12 = iVar12 + 0x18;
                iVar14 = iVar14 + 0x18;
                dVar18 = (*pfVar4 - *pfVar5);
                bridge->vel[3]->z =
                     bridge->vel[3]->z + (((dVar20 * dVar18) *  bridge1->tension) * dVar21 +  (dVar18 * bridge1->tension));
                bridge = bridge2;
              } //while (iVar10 < bridge1->sections + -1);
            //}
            //if (1 < bridge1->sections + -1) {
              //dVar21 = 0.5f;
              //iVar10 = 1;
              for (iVar13 = 1; iVar13 < bridge1->sections + -1; iVar13++) {
                //iVar13 = iVar10 + 1;
                m_00 = &bridge1->mtx[iVar13];
                bridge1->pos[iVar10 * 2]->x = bridge1->pos[iVar13 * 2]->x + bridge1->vel[iVar10 * 2]->x;
                bridge1->pos[iVar10 * 2]->y = bridge1->pos[iVar10 * 2]->y + bridge1->vel[iVar10 * 2]->y;
                bridge1->pos[iVar10 * 2]->z = bridge1->pos[iVar10 * 2]->z + bridge1->vel[iVar10 * 2]->z;
                bridge1->pos[iVar10 * 2 + 1]->x =
                     bridge1->pos[iVar10 * 2 + 1]->x + bridge1->vel[iVar10 * 2 + 1]->x;
                bridge1->pos[iVar10 * 2 + 1]->y =
                     bridge1->pos[iVar10 * 2 + 1]->y + bridge1->vel[iVar10 * 2 + 1]->y;
                bridge1->pos[iVar10 * 2 + 1]->z =
                     bridge1->pos[iVar10 * 2 + 1]->z + bridge1->vel[iVar10 * 2 + 1]->z;
                NuMtxSetRotationY(m_00,(int)bridge1->yang);
                fVar22 = bridge1->pos[iVar10 * 2 + 1]->z - bridge1->pos[iVar10 * 2]->z;
                fVar24 = bridge1->pos[iVar10 * 2 + 1]->x - bridge1->pos[iVar10 * 2]->x;
                fVar22 = NuFsqrt(fVar24 * fVar24 + fVar22 * fVar22);
                iVar12 = NuAtan2D(bridge1->pos[iVar10 * 2 + 1]->y - bridge1->pos[iVar10 * 2]->y,fVar22);
                NuMtxPreRotateX(m_00,iVar12);
                fVar22 = ((bridge1->pos[iVar10 * 2 + 2]->z + bridge1->pos[iVar10 * 2 + 3]->z) -
                        (bridge1->pos[iVar10]->x)) - (bridge1->pos[iVar10]->x);
                fVar24 = ((bridge1->pos[iVar10 * 2 + 2]->x + bridge1->pos[iVar10 * 2 + 3]->x) -
                         *(float *)((int)bridge1->pos[iVar10] + 0x108)) -
                         *(float *)((int)bridge1->pos[iVar10] + 0x114);
                fVar22 = NuFsqrt(fVar24 * fVar24 + fVar22 * fVar22);
                iVar12 = NuAtan2D(((bridge1->pos[iVar10 * 2 + 2]->y + bridge1->pos[iVar10 * 2 + 3]->y)
                                  - *(float *)((int)bridge1->pos[iVar10] + 0x10c)) -
                                  *(float *)((int)bridge1->pos[iVar10] + 0x118),fVar22);
                NuMtxPreRotateZ(m_00,iVar12);
                bridge1->mtx[iVar10]._30 =
                     ((bridge1->pos[iVar10 * 2]->x + bridge1->pos[iVar10 * 2 + 1]->x)  * 0.5f);
                bridge1->mtx[iVar10]._31 =
                     ((bridge1->pos[iVar10 * 2]->y + bridge1->pos[iVar10 * 2 + 1]->y)  * 0.5f);
                bridge1->mtx[iVar10]._32 =
                     ((bridge1->pos[iVar10 * 2]->z + bridge1->pos[iVar10 * 2 + 1]->z)  * 0.5f);
                iVar10 = iVar13;
              } //while (iVar13 < bridge1->sections + -1);
            //}
            NuMtxSetRotationY(m,(int)bridge1->yang);
            fVar22 = bridge1->pos[1]->z - bridge1->pos[0]->z;
            fVar24 = bridge1->pos[1]->x - bridge1->pos[0]->x;
            fVar22 = NuFsqrt(fVar24 * fVar24 + fVar22 * fVar22);
            iVar10 = NuAtan2D(bridge1->pos[1]->y - bridge1->pos[0]->y,fVar22);
            NuMtxPreRotateX(m,iVar10);
            fVar22 = ((bridge1->pos[2]->z + bridge1->pos[3]->z) - bridge1->pos[0]->z) - bridge1->pos[1]->z;
            fVar24 = ((bridge1->pos[2]->x + bridge1->pos[3]->x) - bridge1->pos[0]->x) - bridge1->pos[1]->x;
            fVar23 = NuFsqrt(fVar24 * fVar24 + fVar22 * fVar22);
            fVar22 = bridge1->pos[2]->y;
            fVar24 = bridge1->pos[3]->y;
            dVar18 = 0.25f;
            dVar21 = ((bridge1->pos[0]->y + bridge1->pos[1]->y + fVar22 + fVar24) * 0.25f);
            iVar10 = NuAtan2D((fVar22 + fVar24) - (dVar21 + dVar21),fVar23);
            NuMtxPreRotateZ(m,iVar10);
            cVar1 = bridge1->sections;
            fVar22 = bridge1->pos[1]->x;
            fVar24 = bridge1->pos[1]->z;
            fVar23 = bridge1->pos[0]->x;
            fVar2 = bridge1->pos[0]->z;
            sVar6 = bridge1->yang;
            bridge1->mtx[0]._31 = dVar21;
            bridge1->mtx[0]._30 = ((fVar23 + fVar22) * 0.5f);
            bridge1->mtx[0]._32 = ((fVar2 + fVar24) * 0.5f);
            NuMtxSetRotationY(bridge1->mtx + cVar1 + -1,(int)sVar6);
            iVar10 = (int)bridge1->sections;
            fVar22 = bridge1->pos[iVar10][15].x - bridge1->pos[iVar10][10].x;
            fVar24 = *(float *)((int)bridge1->pos[iVar10] + 0xfc) - *(float *)((int)bridge1->pos[iVar10] + 0xf0);
            fVar22 = NuFsqrt(fVar24 * fVar24 + fVar22 * fVar22);
            iVar10 = NuAtan2D(*(float *)((int)bridge1->pos[bridge1->sections] + 0x100) -
                              *(float *)((int)bridge1->pos[bridge1->sections] + 0xf4),
                              fVar22);
            NuMtxPreRotateX(m,iVar10);
            iVar10 = (int)bridge1->sections;
            fVar22 = ((*(float *)((int)bridge1->pos[iVar10] + 0x110) +
                      *(float *)((int)bridge1->pos[iVar10] + 0x11c)) -
                     bridge1->pos[iVar10][10].x) - bridge1->pos[iVar10][15].x;
            fVar24 = ((*(float *)((int)bridge1->pos[iVar10] + 0x108) +
                      *(float *)((int)bridge1->pos[iVar10] + 0x114)) -
                *(float *)((int)bridge1->pos[iVar10] + 0xf0)) - *(float *)((int)bridge1->pos[iVar10] + 0xfc);
            fVar24 = NuFsqrt(fVar24 * fVar24 + fVar22 * fVar22);
            iVar12 = (int)bridge1->sections;
            fVar22 = *(float *)((int)bridge1->pos[iVar12] + 0x10c) + *(float *)((int)bridge1->pos[iVar12] + 0x118);
            dVar21 = ((fVar22 + *(float *)((int)bridge1->pos[iVar12] + 0xf4) +
                                             *(float *)((int)bridge1->pos[iVar12] + 0x100)) * dVar18);
            iVar10 = NuAtan2D(fVar22 - (dVar21 + dVar21),fVar24);
            NuMtxPreRotateZ(bridge1->mtx + iVar12 + -1,iVar10);
            iVar10 = (int)bridge1->sections;
            bridge1->mtx[iVar10 - 1]._30 = ((*(float *)((int)bridge1->pos[iVar10] + 0x108) +
                                 *(float *)((int)bridge1->pos[iVar10] + 0x114)) * 0.5f);
            bridge1->mtx[bridge1->sections + -1]._31 = dVar21;
            iVar10 = (int)bridge1->sections;
            bridge1->mtx[iVar10 + -1]._32 = ((*(float *)((int)bridge1->pos[iVar10] + 0x110) +
                                 *(float *)((int)bridge1->pos[iVar10] + 0x11c)) * 0.5f);
          }
        }
        else {
          bridge1->inrange = '\0';
        }
        //i = i + 1;
        bridge1++;
      } //while (i < BridgeFree);
    //}
  }
*/
  return;
}