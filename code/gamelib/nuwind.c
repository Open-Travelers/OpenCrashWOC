#include "nuwind.h"

//NGC MATCH
void NuWindDraw(struct nugscn_s *scn) {
    s32 lp;
    s32 i;
    float t2;
    float t3;
    struct nuwindgrp_s *grp;
    struct numtx_s *mtx;
    struct nugobj_s *gobj;
    char pad[25];
    
    grp = &NuWindGroup[0];
    for (lp = 0; lp < NuWindGCount; lp++) {
            if (grp->inrange != '\0') {
                grp->onscreen = '\0';
                mtx = grp->mtx;
                gobj = scn->gobjs[grp->instance->objid];
                    for (i = 0; i < grp->objcount; i++) {
                        t2 = mtx->_23;
                        t3 = mtx->_33;
                        mtx->_23 = 0.0f;
                        mtx->_33 = 1.0f;
                        if (NuRndrGrassGobj(gobj,mtx,NULL) != 0) {
                            grp->onscreen = '\x01';
                        }
                        mtx->_23 = t2;
                        mtx->_33 = t3;
                        mtx++;
                    }
            }
            grp++;
    }
    return;
}

//NGC 97%
s32* NuWindCreate(struct nuinstance_s* instance, struct nuvec4_s* pos, short count, 
                    float wind, float height, s32 collide) {
    struct nuwindgrp_s *grp;
    struct numtx_s *mtx;
    float minx;
    float maxx;
    float miny;
    float maxy;
    float minz;
    float maxz;
    s32 lp;
    
    lp = count;
    minx = height;
    grp = (struct nuwindgrp_s *)NuWindAllocateGrp();
    mtx = NuWindAllocMtxs(count);
    if ((grp != NULL) && (mtx != NULL)) {
        minx = 10000000.0f;
        minz = 10000000.0f; 
        miny = 10000000.0f;
        maxy = -10000000.0f;
        maxz =  -10000000.0f;
        grp->height = height;
        grp->instance = instance;
        grp->collide = collide;
        grp->mtx = mtx;
        maxx =  -10000000.0f;
        grp->objcount = count;
        grp->wind = wind;
        for (lp = 0; lp < count; lp++) {
                NuMtxSetIdentity(&mtx[lp]);
                NuMtxPreRotateY(&mtx[lp],NuWindRand());
                mtx[lp]._11 = pos[lp].w;
                mtx[lp]._00 = mtx[lp]._00 * pos[lp].w;
                mtx[lp]._02 = mtx[lp]._02 * pos[lp].w;
                mtx[lp]._20 = mtx[lp]._20 * pos[lp].w;
                mtx[lp]._22 = mtx[lp]._22 * pos[lp].w;
                mtx[lp]._30 = pos[lp].x;
                mtx[lp]._31 = pos[lp].y;
                mtx[lp]._32 = pos[lp].z;
                mtx[lp]._33 = (wind * pos[lp].w);
                if (pos[lp].x < minx) {
                    minx = pos[lp].x;
                }
                if (pos[lp].y < miny) {
                    miny = pos[lp].y;
                }
                if (pos[lp].z < minz) {
                    minz = pos[lp].z;
                }
                if (pos[lp].x > maxx) {
                    maxx = pos[lp].x;
                }
                if (pos[lp].y > maxy) {
                    maxy = pos[lp].y;
                }
                if (pos[lp].z > maxz) {
                    maxz = pos[lp].z;
                }
        } //while (lp != 0);
        grp->center.x = (maxx  + minx)  / 2;
        grp->center.y = (maxy + miny) /2;
        grp->center.z = (maxz + minz) /2;
        grp->radius =  (grp->center.x * grp->center.x) + (grp->center.y * grp->center.y) + (grp->center.z * grp->center.z) + 1.0f;
        return grp;
    }
    else {
        NuWindFreeGrp(grp);
        NuWindFreeMtxs(mtx,count);
        return NULL;
    }
}