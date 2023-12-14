// Size: 0xB4C, DWARF: 0x73CED2
struct Bridge_s
{
    struct NuVec pos[2][24]; // Offset: 0x0, DWARF: 0x73CEE4
    struct NuVec vel[2][24]; // Offset: 0x240, DWARF: 0x73CF0C
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


/*
   nuvec_s dir; // 0x8(r1)
    nuvec_s dir90; // 0x18(r1)
    // Size: 0xB4C
    struct Bridge_s* bridge; // r30
    int lp; // r25
    float t; // f31
*/

s32 * NuBridgeCreate(struct nuinstance_s **instance,struct nuinstance_s *ipost,struct NuVec *start,
            struct NuVec *end,float width,short yang,float tension,float damp,float gravity,float plrweight,
                    s32 sections,float postw,float posth,s32 postint,s32 colour) {
    struct NuVec dir; // 0x8(r1)
    struct NuVec dir90; // 0x18(r1)
    struct Bridge_s *bridge;
    //s32 id;
    s32 lp;
    //s32 iVar8;
    //struct Bridge_s *bridgeptr;
    //struct numtx_s *mtx;
    float t;
    //char pad[55];


    if (0x18 < sections) {
        printf("to many sections/n");
    }
    NuBridgeOn(1);
    bridge = NuBridgeAlloc();
    if (bridge != NULL) {
        bridge->tension = tension;
        bridge->damp = damp;
        bridge->gravity = gravity;
        bridge->ipost = ipost;
        //lp = 0;
        bridge->plrweight = plrweight;
        bridge->postw = postw;
        bridge->posth = posth;
        bridge->postint = (char)postint;
        bridge->colour = colour;
        bridge->sections = (char)sections;
        bridge->width = width;
        (bridge->center).x = (end->x + start->x) * 0.5f;
        (bridge->center).y = (end->y + start->y) * 0.5f;
        (bridge->center).z = (end->z + start->z) * 0.5f;
        bridge->inrange = '\0';
        bridge->radius =
             end->z - start->z * 0.5f * end->z - start->z * 0.5f + end->x - start->x * 0.5f * end->x - start->x * 0.5f + end->y - start->y * 0.5f * end->y - start->y * 0.5f + 1.0f;
        t = -end->z - start->z * (1.0f / NuFsqrt(end->x - start->x * end->x - start->x + end->z - start->z * end->z - start->z));
        bridge->yang = yang;
       // if (0 < sections) {
            //dVar15 = 4503601774854144.0;
            //iVar8 = 0;
            //mtx = bridge->mtx;
            //bridgeptr = bridge;
            for (lp = 0; lp < sections; lp++) {
                //id = instance[lp];
                bridge->instance[lp] = instance[lp];
                if (instance[lp] == NULL) {
                    bridge->plat[lp] = -1;
                }
                else {
                    bridge->plat[lp] = (short)NewPlatInst(&bridge->mtx[lp],NuBridgeLookupInstanceIndex(instance[lp]));
                    PlatInstRotate(bridge->plat[lp],1);
                }
                NuMtxSetIdentity(&bridge->mtx[lp]);
                NuMtxPreRotateY(&bridge->mtx[lp],bridge->yang);
                //lp = lp + 1;
                bridge->mtx[lp]._30 = (end->x - start->x * lp) / sections + start->x;
                bridge->mtx[lp]._31 = (end->y - start->y * lp) / sections + start->y;
                bridge->mtx[lp]._32 = (end->z - start->z * lp) / sections + start->z;
                //mtx = mtx + 1;
                bridge[lp].pos[0]->x = ((end->x - start->x * lp) / sections + start->x) - ((t * width) * 0.5f);
                bridge[lp].pos[0]->y = (end->y - start->y * lp) / sections + start->y;
                bridge[lp].pos[0]->z = ((end->z - start->z * lp) / sections + start->z) - (((end->x - start->x * (1.0f / t)) * width) * 0.5f);
                bridge[lp].pos[1]->x = ((t * width) * 0.5f + ((end->x - start->x * lp) / sections + start->x));
                bridge[lp].pos[1]->y = (end->y - start->y * lp) / sections + start->y;
                bridge[lp].pos[1]->z = (((end->x - start->x * (1.0f / t)) * width) * 0.5f + ((end->z - start->z * lp) / sections + start->z));
                bridge[lp].vel[0]->x = 0.0f;
                //bridgeptr = (struct Bridge_s *)(bridgeptr->pos + 2);
                bridge[lp].vel[0]->y = 0.0f;
                bridge[lp].vel[0]->z = 0.0f;
                bridge[lp].vel[1]->x = 0.0f;
                bridge[lp].vel[1]->y = 0.0f;
                bridge[lp].vel[1]->z = 0.0f;
                bridge[lp].hit = 0;
                //iVar8 = iVar8 + 0x18;
            } //while (lp < sections);
        //}
    }
    return (s32 *)bridge;
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