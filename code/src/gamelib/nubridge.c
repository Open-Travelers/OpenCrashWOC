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

//63.80% NGC
void NuBridgeUpdate(struct nuvec_s* playerpos) {
    s32 i;
    s32 lp;
    struct Bridge_s* bridge;
    struct nuvec_s dir;
    s32 gsec;
    float ratio;
    float ratio2;
    float gn;
    float gf;
    float dot;
    
    if (NuBridgeProc != 0) {
        bridge = Bridges;
        for (i = 0; i < BridgeFree; i++, bridge++) {
            float dx = bridge->center.x - global_camera.mtx._30;
            float dy = bridge->center.y - global_camera.mtx._31;
            float dz = bridge->center.z - global_camera.mtx._32;
            float dist_sq = dx * dx + dy * dy + dz * dz;
            
            if (dist_sq < (global_camera.farclip * global_camera.farclip + bridge->radius)) {
                bridge->inrange = 1;
                
                if (bridge->onscreen != 0) {
                    if (bridge->hit != 0) {
                        gsec = bridge->sections - 1;
                        
                        dir.x = ((bridge->pos[gsec][0].x + bridge->pos[gsec][1].x) - 
                                bridge->pos[0][0].x - bridge->pos[0][1].x) * 0.5f;
                        
                        dir.y = ((bridge->pos[0][0].y + bridge->pos[0][1].y) * 0.5f);
                        
                        dir.z = ((bridge->pos[gsec][0].z + bridge->pos[gsec][1].z) - 
                                bridge->pos[0][0].z - bridge->pos[0][1].z) * 0.5f;
                        
                        dot = dir.x * dir.x + dir.z * dir.z;
                        
                        
                        ratio = ((dir.x * (playerpos->x - (bridge->pos[0][0].x + bridge->pos[0][1].x) * 0.5f) +
                                 dir.z * (playerpos->z - dir.y)) * (float)gsec) / dot;
                        
                        if (ratio < 0.0f) {
                            ratio = 0.0f;
                        }
                        gn = ((gsec) - 1.0f);
                        //gn = (float)gsec;
                        if (ratio > gn) {
                            gsec = (s32)ratio;
                        }
                        ratio -= (float)gsec;
                        
                        ratio2 = (2.0f / (NuFsqrt(dot) * bridge->width)) *
                                (dir.z * (playerpos->x - (bridge->pos[0][0].x + bridge->pos[0][1].x) * 0.5f) +
                                 dir.x * ((bridge->pos[0][0].z + bridge->pos[0][1].z) * 0.5f - playerpos->z));
                        
                        if (ratio2 > 1.0f) {
                            ratio2 = 1.0f;
                        }
                        if (ratio2 < -1.0f) {
                            ratio2 = -1.0f;
                        }
                        
                        gf = bridge->gravity * (ratio2 + 3.0f) * 0.25f;
                        gn = bridge->gravity * (3.0f - ratio2) * 0.25f;
                        
                        if (ratio == 0.0f) {
                            if (gsec > 0 && gsec < bridge->sections - 1) {
                                bridge->vel[gsec][0].y += gn * bridge->plrweight;
                                bridge->vel[gsec][1].y += gf * bridge->plrweight;
                            }
                        } else {
                            if (gsec > 0 && gsec < bridge->sections - 1) {
                                bridge->vel[gsec][0].y += gn * bridge->plrweight * (1.0f - ratio);
                                bridge->vel[gsec][1].y += gf * bridge->plrweight * (1.0f - ratio);
                            }
                            if (gsec + 1 < bridge->sections - 1) {
                                bridge->vel[gsec + 1][0].y += gn * bridge->plrweight * ratio;
                                bridge->vel[gsec + 1][1].y += gf * bridge->plrweight * ratio;
                            }
                        }
                        bridge->hit--;
                    } else {
                        gsec++;
                    }
                    
                    if (PlatInstGetHit((s32)bridge->plat[0]) != 0) {
                        bridge->hit = 5;
                    }
                    if (PlatInstGetHit((s32)bridge->plat[bridge->sections - 1]) != 0) {
                        bridge->hit = 5;
                    }
                    
                    for (lp = 1; lp < bridge->sections - 1; lp++) {
                        // Damping velocities
                        bridge->vel[lp][0].x -= bridge->vel[lp][0].x * bridge->damp;
                        bridge->vel[lp][0].y += bridge->gravity - bridge->vel[lp][0].y * bridge->damp;
                        bridge->vel[lp][0].z -= bridge->vel[lp][0].z * bridge->damp;
                        
                        bridge->vel[lp][1].x -= bridge->vel[lp][1].x * bridge->damp;
                        bridge->vel[lp][1].y += bridge->gravity - bridge->vel[lp][1].y * bridge->damp;
                        bridge->vel[lp][1].z -= bridge->vel[lp][1].z * bridge->damp;
                        
                        if (PlatInstGetHit((s32)bridge->plat[lp]) != 0) {
                            bridge->hit = 5;
                        }
                        
                        // Spring forces - previous segment to current (side 0)
                        dx = bridge->pos[lp][0].x - bridge->pos[lp - 1][0].x;
                        dy = bridge->pos[lp][0].y - bridge->pos[lp - 1][0].y;
                        dz = bridge->pos[lp][0].z - bridge->pos[lp - 1][0].z;
                        
                        bridge->vel[lp][0].x += NuFabs(dx) * dx * bridge->tension * 0.5f + dx * bridge->tension;
                        bridge->vel[lp][0].y += NuFabs(dy) * dy * bridge->tension * 0.5f + dy * bridge->tension;
                        bridge->vel[lp][0].z += NuFabs(dz) * dz * bridge->tension * 0.5f + dz * bridge->tension;
                        
                        // Spring forces - current to next segment (side 0)
                        dx = bridge->pos[lp + 1][0].x - bridge->pos[lp][0].x;
                        dy = bridge->pos[lp + 1][0].y - bridge->pos[lp][0].y;
                        dz = bridge->pos[lp + 1][0].z - bridge->pos[lp][0].z;
                        
                        bridge->vel[lp][0].x += NuFabs(dx) * dx * bridge->tension * 0.5f + dx * bridge->tension;
                        bridge->vel[lp][0].y += NuFabs(dy) * dy * bridge->tension * 0.5f + dy * bridge->tension;
                        bridge->vel[lp][0].z += NuFabs(dz) * dz * bridge->tension * 0.5f + dz * bridge->tension;
                        
                        // Spring forces - previous segment to current (side 1)
                        dx = bridge->pos[lp][1].x - bridge->pos[lp - 1][1].x;
                        dy = bridge->pos[lp][1].y - bridge->pos[lp - 1][1].y;
                        dz = bridge->pos[lp][1].z - bridge->pos[lp - 1][1].z;
                        
                        bridge->vel[lp][1].x += NuFabs(dx) * dx * bridge->tension * 0.5f + dx * bridge->tension;
                        bridge->vel[lp][1].y += NuFabs(dy) * dy * bridge->tension * 0.5f + dy * bridge->tension;
                        bridge->vel[lp][1].z += NuFabs(dz) * dz * bridge->tension * 0.5f + dz * bridge->tension;
                        
                        // Spring forces - current to next segment (side 1)
                        dx = bridge->pos[lp + 1][1].x - bridge->pos[lp][1].x;
                        dy = bridge->pos[lp + 1][1].y - bridge->pos[lp][1].y;
                        dz = bridge->pos[lp + 1][1].z - bridge->pos[lp][1].z;
                        
                        bridge->vel[lp][1].x += NuFabs(dx) * dx * bridge->tension * 0.5f + dx * bridge->tension;
                        bridge->vel[lp][1].y += NuFabs(dy) * dy * bridge->tension * 0.5f + dy * bridge->tension;
                        bridge->vel[lp][1].z += NuFabs(dz) * dz * bridge->tension * 0.5f + dz * bridge->tension;
                    }
                    
                    for (lp = 1; lp < bridge->sections - 1; lp++) {
                        bridge->pos[lp][0].x += bridge->vel[lp][0].x;
                        bridge->pos[lp][0].y += bridge->vel[lp][0].y;
                        bridge->pos[lp][0].z += bridge->vel[lp][0].z;
                        
                        bridge->pos[lp][1].x += bridge->vel[lp][1].x;
                        bridge->pos[lp][1].y += bridge->vel[lp][1].y;
                        bridge->pos[lp][1].z += bridge->vel[lp][1].z;
                        
                        NuMtxSetRotationY(&bridge->mtx[lp], (s32)bridge->yang);
                        
                        dx = bridge->pos[lp][1].x - bridge->pos[lp][0].x;
                        dz = bridge->pos[lp][1].z - bridge->pos[lp][0].z;
                        dist_sq = NuFsqrt(dx * dx + dz * dz);
                        
                        NuMtxPreRotateX(&bridge->mtx[lp], 
                            NuAtan2D(bridge->pos[lp][1].y - bridge->pos[lp][0].y, dist_sq));
                        
                        dx = (bridge->pos[lp + 1][0].x + bridge->pos[lp + 1][1].x) -
                             (bridge->pos[lp - 1][0].x + bridge->pos[lp - 1][1].x);
                        dz = (bridge->pos[lp + 1][0].z + bridge->pos[lp + 1][1].z) -
                             (bridge->pos[lp - 1][0].z + bridge->pos[lp - 1][1].z);
                        dist_sq = NuFsqrt(dx * dx + dz * dz);
                        
                        dy = (bridge->pos[lp + 1][0].y + bridge->pos[lp + 1][1].y) -
                             (bridge->pos[lp - 1][0].y + bridge->pos[lp - 1][1].y);
                        
                        NuMtxPreRotateZ(&bridge->mtx[lp], NuAtan2D(dy, dist_sq));
                        
                        bridge->mtx[lp]._30 = (bridge->pos[lp][0].x + bridge->pos[lp][1].x) * 0.5f;
                        bridge->mtx[lp]._31 = (bridge->pos[lp][0].y + bridge->pos[lp][1].y) * 0.5f;
                        bridge->mtx[lp]._32 = (bridge->pos[lp][0].z + bridge->pos[lp][1].z) * 0.5f;
                    }
                    
                    // First segment matrix
                    NuMtxSetRotationY(&bridge->mtx[0], (s32)bridge->yang);
                    
                    dx = bridge->pos[1][0].x - bridge->pos[0][0].x;
                    dz = bridge->pos[1][0].z - bridge->pos[0][0].z;
                    dist_sq = NuFsqrt(dx * dx + dz * dz);
                    
                    NuMtxPreRotateX(&bridge->mtx[0], 
                        NuAtan2D(bridge->pos[1][0].y - bridge->pos[0][0].y, dist_sq));
                    
                    dx = (bridge->pos[2][0].x + bridge->pos[2][1].x) -
                         (bridge->pos[0][0].x + bridge->pos[0][1].x);
                    dz = (bridge->pos[2][0].z + bridge->pos[2][1].z) -
                         (bridge->pos[0][0].z + bridge->pos[0][1].z);
                    dist_sq = NuFsqrt(dx * dx + dz * dz);
                    
                    gf = (bridge->pos[0][0].y + bridge->pos[0][1].y + 
                          bridge->pos[2][0].y + bridge->pos[2][1].y) * 0.25f;
                    
                    NuMtxPreRotateZ(&bridge->mtx[0], 
                        NuAtan2D((bridge->pos[2][0].y + bridge->pos[2][1].y) - gf * 2.0f, dist_sq));
                    
                    bridge->mtx[0]._30 = (bridge->pos[0][0].x + bridge->pos[0][1].x) * 0.5f;
                    bridge->mtx[0]._31 = gf;
                    bridge->mtx[0]._32 = (bridge->pos[0][0].z + bridge->pos[0][1].z) * 0.5f;
                    
                    // Last segment matrix
                    gsec = bridge->sections - 1;
                    NuMtxSetRotationY(&bridge->mtx[gsec], (s32)bridge->yang);
                    
                    dx = bridge->pos[gsec][1].x - bridge->pos[gsec][0].x;
                    dz = bridge->pos[gsec][1].z - bridge->pos[gsec][0].z;
                    dist_sq = NuFsqrt(dx * dx + dz * dz);
                    
                    NuMtxPreRotateX(&bridge->mtx[gsec], 
                        NuAtan2D(bridge->pos[gsec][1].y - bridge->pos[gsec][0].y, dist_sq));
                    
                    dx = (bridge->pos[gsec][0].x + bridge->pos[gsec][1].x) -
                         (bridge->pos[gsec - 1][0].x + bridge->pos[gsec - 1][1].x);
                    dz = (bridge->pos[gsec][0].z + bridge->pos[gsec][1].z) -
                         (bridge->pos[gsec - 1][0].z + bridge->pos[gsec - 1][1].z);
                    dist_sq = NuFsqrt(dx * dx + dz * dz);
                    
                    dy = (bridge->pos[gsec][0].y + bridge->pos[gsec][1].y) -
                         (bridge->pos[gsec - 1][0].y + bridge->pos[gsec - 1][1].y);
                    
                    gf = (bridge->pos[gsec][0].y + bridge->pos[gsec][1].y + dy) * 0.25f;
                    
                    NuMtxPreRotateZ(&bridge->mtx[gsec - 1], NuAtan2D(dy - gf * 2.0f, dist_sq));
                    
                    bridge->mtx[gsec]._30 = (bridge->pos[gsec][0].x + bridge->pos[gsec][1].x) * 0.5f;
                    bridge->mtx[gsec]._31 = gf;
                    bridge->mtx[gsec]._32 = (bridge->pos[gsec][0].z + bridge->pos[gsec][1].z) * 0.5f;
                }
            } else {
                bridge->inrange = 0;
            }
        }
    }
}