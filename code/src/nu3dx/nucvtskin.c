#include "nucvtskin.h"


struct primdef_s* primdefs_sorted;
struct primdef_s* primdefs;
s32 stritot;
s32 totaldupes;
s32 totalpts;
s32 tritot;
s32 stats[15];
static char mtxused[256];

//MATCH GCN
void InitSkinning(s32 buffsize) {  //makes primitive
  primdefs = (struct primdef_s *)NuMemAlloc(0xc7ce0); // 0xC7CE0 bytes can fit 0x3548 NuPrims perfectly
  primdefs_sorted = (struct primdef_s *)NuMemAlloc(0xc7ce0);
  return;
}

//MATCH GCN
void CloseSkinning(void) {
  NuMemFree(primdefs);
  NuMemFree(primdefs_sorted);
  primdefs = NULL;
  primdefs_sorted = NULL;
  return;
}

//MATCH GCN
void NuPs2CreateSkin(struct nugobj_s* gobj) {
    if ((gobj->geom != NULL) && (gobj->geom->prim->type == NUPT_NDXTRI)) {
        NuPs2CreateSkinNorm(gobj);
    }
return;
}

//MATCH GCN
static void CreateSkinGeom(struct nugeom_s* geom, struct primdef_s* pd, s32 pdcnt) {

    s32 amount_prim;
    struct nuprim_s* nextprim;
    struct nuprim_s* currentprim;
    struct nuprim_s* startprim;
    struct nuvtx_sk3tc1_s* newvertexbuff;
    struct primdef_s* currpd;
    s32 i;
    s32 count;
    u16* indexbuffer;
    s32 primsize[300];
    s32 currentbaseid;
    s32 iVar6;
    s32 j;
    s32 k;
    s32 l;
    s32 q;

    if (geom->vtxtype != NUVT_TC1) {
        NuErrorProlog("C:/source/crashwoc/code/nu3dx/nucvtskn.c", 0x69)("CreateSkinGeom : Unknown vertex type!");
    }

    currentbaseid = pd[0].baseid;
    count = 1;

    memset(primsize, 0, sizeof(primsize));

    for (i = 0; i < pdcnt; i++) {
        if (pd[i].baseid != currentbaseid) {
            currentbaseid = pd[i].baseid;
            count++;
        }
        primsize[count] += 3;
    }
    
    startprim = currentprim = NuPrimCreate(primsize[1], NUPT_NDXTRI);;
    for (i = 1; i < count; i++) {
        currentprim->next = nextprim = NuPrimCreate(primsize[i + 1], NUPT_NDXTRI);
        currentprim = currentprim->next;
    }
    
    currentprim = startprim;
    newvertexbuff = (struct nuvtx_sk3tc1_s*)GS_CreateBuffer(geom->vtxcnt * sizeof(struct nuvtx_sk3tc1_s), 1);
    currpd = pd;
    
    for (i = 0; i < count; i++) {
        for (j = 0; j < 15; j++) {
            currentprim->skinmtxlookup[j] = currpd->mtxid[j];
        }
        indexbuffer = (u16*)currentprim->idxbuff;
        for (amount_prim = 0; amount_prim < currentprim->cnt; amount_prim += 3) {
            indexbuffer[amount_prim] = currpd->vid[0];
            indexbuffer[amount_prim + 1] = currpd->vid[1];
            indexbuffer[amount_prim + 2] = currpd->vid[2];
            for (k = 0; k < 3; k++) {
                newvertexbuff[currpd->vid[k]].pnt = currpd->vrts[k].pnt;
                newvertexbuff[currpd->vid[k]].nrm = currpd->vrts[k].nrm;
                newvertexbuff[currpd->vid[k]].diffuse = currpd->vrts[k].diffuse;
                newvertexbuff[currpd->vid[k]].tc[0] = currpd->vrts[k].tc[0];
                newvertexbuff[currpd->vid[k]].tc[1] = currpd->vrts[k].tc[1];
                for (j = 0; j < 3; j++) {
                    newvertexbuff[currpd->vid[k]].weights[j] = 0.0f;
                    newvertexbuff[currpd->vid[k]].indexes[j] = 0.0f;
                }
                for (j = 0, iVar6 = 0; j < 15 && iVar6 < 3; j++) {
                    if (currpd->weights[k][j] != 0.0f) {
                        newvertexbuff[currpd->vid[k]].indexes[iVar6] = (float)j;
                        if (iVar6 < 2) {
                            newvertexbuff[currpd->vid[k]].weights[iVar6] = currpd->weights[k][j];
                        }
                        iVar6++;
                    }
                }
            }
            currpd++;
        }
        currentprim = currentprim->next;
    }
    GS_DeleteBuffer((u8*)geom->hVB);
    for (currentprim = geom->prim; currentprim != NULL;) {
        nextprim = currentprim->next;
        NuPrimDestroy(currentprim);
        currentprim = nextprim;
    }
    geom->hVB = (s32)newvertexbuff;
    geom->prim = startprim;
    geom->vtxtype = NUVT_SK3TC1;
    return;
}

//MATCH GCN
static s32 AddMtxToPrimDef(struct primdef_s *primdef,s32 mtxid)
{
  s32 n;
  s32 i;


     for(i = 0; i < primdef->nummtx; i++) {

          n = primdef->mtxid[i];
        if (n == mtxid) {
            return i;
        }
    }
    if (i >= 15) {
        return 0;
    }
    primdef->mtxid[i] = mtxid;
    primdef->nummtx++;
    return i;
}

//NGC MATCH
static void SetVtxSkinData (struct primdef_s * pd, s32 pdix, struct nuvtx_tc1_s * vb, s32 vid, struct nugeom_s * ge)
{
  s32 n;
  s32 offset;
  float *weights;
  s32 ix;
  struct nuskin_s *sk;

      //memcpy(&vb[vid], &pd->vrts[pdix], sizeof (struct nuvtx_tc1_s));
      pd->vrts[pdix] = vb[vid];
      pd->vid[pdix] = vid;

      if (pdix < 3) {
        for (sk = ge->skin; sk != NULL; sk = sk->next) {
          offset = vid - sk->vtxoffset;
          if ((-1 < offset) && (offset < sk->vtxcnt)) {
            weights = &sk->weights[sk->mtxcnt * offset];
            for (ix = 0, n = 0; ix < sk->mtxcnt; ix++)
            {
              n = AddMtxToPrimDef(pd,sk->mtxid[ix]);
              pd->weights[pdix * 5][n] = weights[ix];
            }
            return;
          }
        }
      }
      return;
}

//86% --> equivalent?
static void SetVtxSkinData2(struct primdef_s *pd,s32 pdix,struct nuvtx_tc1_s *vertexbuf,s32 vid,struct nugeom_s *currgeom)
{
    struct NUVTXSKININFO_s *skinfo;
    s32 i;
    s32 ix;


    pd->vrts[pdix] = vertexbuf[vid];
    pd->vid[pdix] = vid;
    skinfo = &currgeom->vtxskininfo[vid];


    for(i = ix = 0; i != 3 && skinfo->wts[ix] != 0.0f; i++) {
        pd->weights[pdix][AddMtxToPrimDef(pd,skinfo->joint_ixs[i])] = skinfo->wts[++ix];
    }

    return;
}

//94.78%
// This function creates a skinned geometry. It does this by creating an array of
// primitives, setting vertex skin data, sorting the primitives and then creating a
// new geometry from the sorted primitive array. A better name for this function
void NuPs2CreateSkinNorm(struct nugobj_s *gobj) {
    struct nugeom_s* currGeom;
    struct nuprim_s *p;
    struct nuvtx_tc1_s *srcvb;
    struct primdef_s *pd;
    u16 *vid;
    s32 pdcnt;
    s32 skinix;
    s32 totmtx;
    s32 ptot;

    s32 iVar2;
    s32 iVar3;

    memset(stats, NULL, sizeof(stats));


    totaldupes = 0;
    totalpts = 0;
    tritot = 0;
    stritot = 0;

    for (currGeom = gobj->geom; currGeom != 0; currGeom = currGeom->next)
    {
        if (currGeom->skin == NULL && !currGeom->vtxskininfo) {
            continue;
        }
        pd = primdefs;
        p = currGeom->prim;
        pdcnt = 0;
        vid = (u16*)p->idxbuff;
        srcvb = currGeom->hVB;
        // Maximum amount of faces is 2200 (0x898)
        if ((p->cnt / 3) > 2200)
        {
            NuErrorProlog("C:/source/crashwoc/code/nu3dx/nucvtskn.c", 0x35e,"NuPs2CreateSkinNorm: TOO MANY PRIMS!");
        }

        for (skinix = 0; skinix < p->cnt; skinix += 3)
        {
            memset(pd, 0, sizeof(struct primdef_s));
            memset(pd->mtxid, -1, 0x3c);

            if (currGeom->vtxskininfo)
            {
                SetVtxSkinData2(pd, 0, srcvb, (s32)vid[0], currGeom);
                SetVtxSkinData2(pd, 1, srcvb, (s32)vid[1], currGeom);
                SetVtxSkinData2(pd, 2, srcvb, (s32)vid[2], currGeom);
            }
            else
            {
                SetVtxSkinData(pd, 0, srcvb, (s32)vid[0], currGeom);
                SetVtxSkinData(pd, 1, srcvb, (s32)vid[1], currGeom);
                SetVtxSkinData(pd, 2, srcvb, (s32)vid[2], currGeom);
            }

            if (((pd->vid[0] == pd->vid[1]) || (pd->vid[1] == pd->vid[2])) || (pd->vid[0] == pd->vid[2]))
            {
                pd -= 1;
                pdcnt -= 1;
            }
            totmtx = pd->nummtx;
            pd++;
            pdcnt++;
            vid += 3; //vid += 6;
            stats[totmtx]++;
        }
        SortPrimdefs(primdefs,pdcnt);
        iVar3 = 0;
        iVar2 = 0;
        for(skinix = 0; skinix < 15; skinix++) {
            if ((stats[skinix] != 0) && (skinix > iVar2)) {
                iVar2 = skinix;
            }
            if (stats[skinix] >= stats[iVar3]) {
                iVar3 = skinix;
            }
        }
        CreateSkinGeom(currGeom,primdefs,pdcnt);
    }

	return;
}


//MATCH GCN
static s32 FillFreeMatrixSlots(struct primdef_s* pd, s32 cnt, s32 start) {
    struct primdef_s* primdef_batch; //
    int numbatchmtx; //
    int n; //
    int m; //
    int maxmtxperprim; //

    maxmtxperprim = 0xC;

    primdef_batch = &pd[start];
    numbatchmtx = primdef_batch->nummtx;

    if (start >= cnt) {
        return numbatchmtx;
    }

    if (numbatchmtx >= maxmtxperprim) {
        return numbatchmtx;
    }

    for(n = start + 1; n < cnt; n++) {
        if (pd[n].sorted != 1) {
            for(m = 0; m < maxmtxperprim && pd[n].mtxid[m] != -1; m++) {
                if (mtxused[pd[n].mtxid[m]] == 0) {
                    primdef_batch->mtxid[numbatchmtx] = pd[n].mtxid[m];
                    mtxused[pd[n].mtxid[m]] = 1;

                    if (numbatchmtx++ == 11) {
                        return maxmtxperprim;
                    }
                }
            }
        }
    }
    return numbatchmtx;
}


//69,91%
static s32 SortPrimdefs(struct primdef_s* pd, s32 count) {
    s32 scnt; //
    s32 tot; // 
    s32 baseid; // 
    s32 totsmtx; // 
    s32 totusmtx; // 
    s32 n; // 
    s32 m; // 
    s32 o; // 
    s32 p; // 
    s32 s; // 
    s32 t; // 
    struct primdef_s tpd; //
    s32 batchmatrices[200][15]; //
    s32 batchmatrices_sorted[200][15]; // 
    s32 batchcount; // 
    s32 matrixusecount[150]; // 
    s32 matrixindex; // 
    s32 matrixcount; // 
    s32 maxmatrix; // 
    struct matchingslot_s matchingslot[200]; //
    s32 nummatches; // 
    s32 freeslotcount; // 
    s32 matrixslot; // 
    s32 batchnum; // 
    float totalweights11; // 
    float totalweights12; // 
    float totalweights13; // 
    s32 totalmtx; // 
    s32 i; // 
    s32 j;

    s32 l;
    s32 u;
    s32 z;
    s32 var_r8_4;
    
    s32 iVar6;
    s32 iVar9;
    s32 iVar16;
    s32 iVar17;
    s32 iVar21;
    s32 iVar22;
    s32 iVar23;
    s32 bID;
    s32 k;
    
    //mtxusecnt = matrixusecount;
    //batchmtx_sorted = batchmatrices_sorted;
    // n = 0;
    bID = 0;
    // 'Tis but a bubble sort
    for (i = 0; i < count; i++) {
        for (j = i + 1; j < count; j++) {
            if (pd[j].nummtx > pd[i].nummtx) {
                tpd = pd[i];
                pd[i] = pd[j];
                pd[j] = tpd;
            }
        }
    }

    m = 0;
    for (k = 0; k < count; k++) {
        if (pd[k].sorted == 0) {
            //bID++;
            memset(&mtxused, 0, sizeof(mtxused));
            
            for (n = 0; n < pd[k].nummtx; n++) {
                mtxused[pd[k].mtxid[n]] = 1;
            }
            
            pd[k].nummtx = FillFreeMatrixSlots(pd, count, k);
            
            pd[k].baseid = m;
            memcpy(&primdefs_sorted[m], &pd[k], sizeof (struct primdef_s));
            
            m++;
            
            for (j = k + 1; j < count; j++) {
                if ((pd[j].sorted == 0) && (pd[k].nummtx >= pd[j].nummtx)) {
                    iVar6 = 0;
                    for (i = 0; i < pd[j].nummtx; i++) {
                        matrixslot = (mtxused[pd[j].mtxid[i]] - 1);
                        iVar6 += matrixslot > 0 ? matrixslot : -matrixslot;
                    }
                    
                    // pSorted = primdefs_sorted;
                    
                    if (iVar6 == 0) {
                        pd[j].baseid = m;
                        memcpy(&primdefs_sorted[m], &pd[j], sizeof (struct primdef_s));

                        
                        pd[j].sorted = 1;
                        
                        for (n = 0; n < 15; n++) {
                            primdefs_sorted[m].mtxid[n] = -1;
                            primdefs_sorted[m].weights[0][n] = 0.0f;
                            primdefs_sorted[m].weights[1][n] = 0.0f;
                            primdefs_sorted[m].weights[2][n] = 0.0f;
                        }
                        
                        for (n = 0; n < pd[k].nummtx; n++) {
                            for (iVar6 = 0; iVar6 < pd[j].nummtx; iVar6++) {
                                if (pd[k].mtxid[n] == pd[j].mtxid[iVar6]) {
                                    primdefs_sorted[m].weights[0][n] = pd[j].weights[0][iVar6];
                                    primdefs_sorted[m].weights[1][n] = pd[j].weights[1][iVar6];
                                    primdefs_sorted[m].weights[2][n] = pd[j].weights[2][iVar6];
                                    primdefs_sorted[m].mtxid[n] = pd[k].mtxid[n];
                                }
                            }
                        }
                        m++;
                    }
                }
            }
            
            for (t = 0; t < 15; t++) {
                batchmatrices[bID][t] = primdefs_sorted[bID].mtxid[t];
            }
            bID++;
        }
    }
    
    memset(matrixusecount, 0, sizeof(matrixusecount));
    i = -1;
    for (iVar17 = 0; iVar17 < bID; iVar17++) {
        for (iVar21 = 0; iVar21 < 15; iVar21++) {
            if (batchmatrices[iVar17][iVar21] != -1) {
                matrixusecount[batchmatrices[iVar17][iVar21]]++;
                if (batchmatrices[iVar17][iVar21] < i) {
                    i = batchmatrices[iVar17][iVar21];
                }
            }
        }
    }
    
    memset(batchmatrices_sorted, -1, sizeof(batchmatrices_sorted));
    //cnt = count;
    while (1) {
        iVar17 = 0;
        var_r8_4 = matrixusecount[0];
        for (iVar23 = 0; iVar23 <= i; iVar23++) {
            if (matrixusecount[iVar23] > matrixusecount[iVar17]) {
                var_r8_4 = matrixusecount[iVar23];
                iVar17 = iVar23;
            }
        }

        if (var_r8_4 == 0) break;
        
        iVar23 = 0;
        iVar9 = 0;
        for (iVar21 = 0; iVar21 < bID; iVar21++) {
            for (j = 0; j < 0xF; j++) {
                if (batchmatrices[iVar21][j] == iVar17) {
                    matchingslot[iVar9].batch = iVar21;
                    matchingslot[iVar9].slot = j;
                    iVar23++;
                    iVar9++;
                }
            }
        }
        
        // iVar9 = 0;
        // iVar21 = iVar9;
        // if (iVar23 > 0) {
        u = 0;
        // if (iVar23 > 0) {
        // for (iVar9 = 0; iVar9 < 0xf; iVar9++) {
            // for (iVar9 = 0; iVar9 < 0xf; iVar9++) {
        while (1){
            // do {
                iVar16 = 0;
                // iVar9 = iVar21 + 1;
                // if (0 < iVar23) {
                for (iVar22 = 0; iVar22 < iVar23; iVar22++) {
                    //pmVar12 = matchingslot;
                    // iVar22 = iVar23;
                    // do {
                        // batchcount = &matchingslot->batch;
                        //pmVar12 = pmVar12 + 1;
                        if (batchmatrices_sorted[matchingslot[iVar22].batch][iVar9] == -1) {
                            iVar16++;
                        }
                        // iVar22 = iVar22 + -1;
                    // } while (iVar22 != 0);
                }
                u++;
                if (iVar16 < iVar23) {
                    if (u > 0xE){
                        NuErrorProlog("C:/source/crashwoc/code/nu3dx/nucvtskn.c", 0x232)("SortPrimDefs: Unable to find a matching free slot in all batches!!");
                        break;
                        // goto h;
                    }
                }
                // iVar21 = iVar9;
            // }
        // }
            // } while (iVar9 < 0xf);
        // }
        // else {
            // LAB_800ae5b0:
            for (z = 0; z < iVar23; z++) {
                //pmVar12 = matchingslot;
                // do {
                    // batchcount = &matchingslot->batch;
                    // iVar23 = iVar23 + -1;
                    //pmVar12 = pmVar12 + 1;
                    batchmatrices_sorted[matchingslot[z].batch][u - 1] = iVar17;
                    matrixusecount[iVar17] = 0;
                // } while (iVar23 != 0);
            }
        }
        // }
    }
   // h:
    bID = 0;
    iVar17 = 0;
    for (iVar23 = 0; iVar23 < count; iVar23++) {
        if (primdefs_sorted[i].baseid != bID) {
            bID = primdefs_sorted[i].baseid;
            iVar17++;
        }
        
        memcpy(&pd[iVar23], &primdefs_sorted[iVar23], sizeof (struct primdef_s));
        
        for (k = 0; k < 0xF; k++) {
            pd[iVar23].mtxid[k] = -1;
            pd[iVar23].weights[0][k] = 0.0f;
            pd[iVar23].weights[1][k] = 0.0f;
            pd[iVar23].weights[2][k] = 0.0f;
        }
        
        pd[iVar23].nummtx = primdefs_sorted[iVar23].nummtx;
        pd[iVar23].baseid = bID;
        
        for (k = 0; k < 0xF; k++) {
            for (l = 0; l < 0xF; l++) {
                if (batchmatrices_sorted[iVar17][iVar23] == primdefs_sorted[iVar23].mtxid[l]) {
                    pd[iVar23].weights[0][k] = primdefs_sorted[iVar23].weights[0][l];
                    pd[iVar23].weights[1][k] = primdefs_sorted[iVar23].weights[1][l];
                    pd[iVar23].weights[2][k] = primdefs_sorted[iVar23].weights[2][l];
                    pd[iVar23].mtxid[k] = primdefs_sorted[iVar23].mtxid[l];
                }
            }
        }
    }
    
    i = 0;
    bID = -1;
    //pSorted = primdefs_sorted;
    for (k = 0; k < n; k++) {
        if (primdefs_sorted[k].baseid != bID) {
            i += primdefs_sorted[k].nummtx;
            bID = primdefs_sorted[k].baseid;
        }
    }
    return i;
}
