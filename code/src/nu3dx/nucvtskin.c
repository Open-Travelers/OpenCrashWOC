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


//78% NGC
static s32 SortPrimdefs(struct primdef_s* pd, s32 count) {
    s32 scnt; // r1+0x67D8
    s32 tot; // r8
    s32 baseid; // r19
    s32 totsmtx; // r3
    s32 totusmtx;
    s32 n; // r28
    s32 m; // r7
    s32 o; // r5
    s32 p;
    s32 s; // r30
    s32 t; // r4
    struct primdef_s tpd; // r1+0x8
    s32 batchmatrices[200][15]; // r1+0x180
    s32 batchmatrices_sorted[200][15]; // r1+0x3060
    s32 batchcount; // r1+0x67DC
    s32 matrixusecount[150]; // r1+0x5F40
    s32 matrixindex; // r4
    s32 matrixcount; // r8
    s32 maxmatrix; // r30
    struct matchingslot_s matchingslots[200]; // r1+0x6198
    s32 nummatches; // r6
    s32 freeslotcount; // r11
    s32 matrixslot; // r8
    s32 batchnum;
    float totalweights1; // f10
    float totalweights2; // f9
    float totalweights3; // f8
    s32 totalmtx; // r5
    s32 i; // r30 only meant to be used in a single block
    s32 j; // r5 only meant to be used in a single block
    
    baseid = 0;

    scnt = count;
    
    for (i = 0; i < scnt; i++)
    {
        for (j = i + 1; j < scnt; j++)
        {
            if (pd[j].nummtx > pd[i].nummtx) 
            {
                tpd = pd[i];
                
                pd[i] = pd[j];
                
                pd[j] = tpd; 
            }
        }
    }
    
    for (m = 0, s = 0; s < scnt; s++)
    {
        if (pd[s].sorted == 0) 
        {
            memset(&mtxused, 0, sizeof(mtxused));
            
            for (matrixindex = 0; matrixindex < pd[s].nummtx; n++) 
            {
                mtxused[pd[s].mtxid[n]] = 1;
            }
            
            pd[s].nummtx = FillFreeMatrixSlots(pd, scnt, s);
            
            pd[s].baseid = m;
            
            memcpy(&primdefs_sorted[m], &pd[s], sizeof(struct primdef_s));
            
            m++;
            
            for (j = s + 1; j < scnt; j++) 
            {
                if ((pd[j].sorted == 0) && (pd[s].nummtx >= pd[j].nummtx)) 
                {
                    for (t = 0, i = 0; i < pd[j].nummtx; i++) 
                    {
                        matrixslot = mtxused[pd[j].mtxid[i]] - 1;
                        
                        t += matrixslot > 0 ? matrixslot : -matrixslot;
                    }
                    
                    if (t == 0) 
                    {
                        pd[j].baseid = m;
                        
                        memcpy(&primdefs_sorted[m], &pd[j], sizeof(struct primdef_s));
                        
                        pd[j].sorted = 1;
                        
                        for (n = 0; n < 15; n++) 
                        {
                            primdefs_sorted[m].mtxid[n] = -1;
                            
                            primdefs_sorted[m].weights[0][n] = 0.0f;
                            primdefs_sorted[m].weights[1][n] = 0.0f;
                            primdefs_sorted[m].weights[2][n] = 0.0f;
                        }
                        
                        for (n = 0; n < pd[s].nummtx; n++) 
                        {
                            for (t = 0; t < pd[j].nummtx; t++) 
                            {
                                if (pd[s].mtxid[n] == pd[j].mtxid[t]) 
                                {
                                    primdefs_sorted[m].weights[0][n] = pd[j].weights[0][t];
                                    primdefs_sorted[m].weights[1][n] = pd[j].weights[1][t];
                                    primdefs_sorted[m].weights[2][n] = pd[j].weights[2][t];
                                    
                                    primdefs_sorted[m].mtxid[n] = pd[s].mtxid[n];
                                }
                            }
                        }
                        
                        m++;
                    }
                }
            }
            
            for (t = 0; t < 15; t++) 
            {
                batchmatrices[baseid][t] = primdefs_sorted[baseid].mtxid[t];
            }
            
            baseid++;
        }
    }
    
    memset(matrixusecount, 0, sizeof(matrixusecount));
    
    for (batchnum = -1, m = 0; m < baseid; m++) 
    {
        for (s = 0; s < 15; s++)
        {
            if (batchmatrices[m][s] != -1) 
            {
                matrixusecount[batchmatrices[m][s]]++;
                
                if (batchmatrices[m][s] < batchnum) 
                {
                    batchnum = batchmatrices[m][s];
                }
            }
        }
    }
    
    memset(batchmatrices_sorted, -1, sizeof(batchmatrices_sorted));

    for ( ; ; )
    {
        for (m = 0, matrixcount = matrixusecount[m], o = 0; o <= batchnum; o++) 
        {
            if (matrixusecount[o] > matrixusecount[m]) 
            {
                matrixcount = matrixusecount[o];
                
                m = o;
            }
        }

        if (matrixcount == 0) 
        {
            break;
        }
        
        for (o = 0, p = 0, s = 0; s < baseid; s++) 
        {
            for (j = 0; j < 15; j++) 
            {
                if (batchmatrices[s][j] == m) 
                {
                    matchingslots[p].batch = s;
                    
                    matchingslots[p].slot = j;
                    
                    p++;
                    o++;
                }
            }
        }
        
        for (o = 0; ; )
        { 
            for (t = 0, s = 0; s < o; s++) 
            {
                if (batchmatrices_sorted[matchingslots[s].batch][p] == -1) 
                {
                    t++;
                }
            }
            
            o++;
            
            if (t < o) 
            {
                if (o > 14) 
                {
                    NuErrorProlog("C:/source/crashwoc/code/nu3dx/nucvtskn.c", 562)("SortPrimDefs: Unable to find a matching free slot in all batches!!");
                    break;
                }
            }
        }
            
        for (p = 0; p < o; p++) 
        {
            batchmatrices_sorted[matchingslots[p].batch][o] = o;
            
            matrixusecount[m] = 0;
        }
    }
    
    for (m = 0, baseid = 0, o = 0; o < scnt; o++) 
    {
        if (primdefs_sorted[baseid].baseid != baseid) 
        {
            baseid = primdefs_sorted[baseid].baseid;
            
            m++;
        }
        
        memcpy(&primdefs_sorted[baseid], &pd[baseid], sizeof(struct primdef_s));
        
        for (s = 0; s < 15; s++) 
        {
            pd[o].mtxid[s] = -1;
            
            pd[o].weights[0][s] = 0.0f;
            pd[o].weights[1][s] = 0.0f;
            pd[o].weights[2][s] = 0.0f;
        }
        
        pd[o].nummtx = &primdefs_sorted[o].nummtx; 

        pd[o].baseid = &primdefs_sorted[o].baseid;
        
        for (s = 0; s < 15; s++) 
        {
            for (t = 0; t < 15; t++) 
            {
                if (batchmatrices_sorted[m][o] == primdefs_sorted[o].mtxid[t]) 
                {
                    pd[o].weights[0][s] = primdefs_sorted[o].weights[0][t];
                    pd[o].weights[1][s] = primdefs_sorted[o].weights[1][t];
                    pd[o].weights[2][s] = primdefs_sorted[o].weights[2][t];
                   
                    pd[o].mtxid[s] = primdefs_sorted[o].mtxid[t];
                }
            }
        }
    }
    
    for (totalmtx = 0, baseid = -1, s = 0; s < n; s++) 
    {
        if (primdefs_sorted[s].baseid != baseid) 
        {
            totalmtx += primdefs_sorted[s].nummtx;
            
            baseid = primdefs_sorted[s].baseid;
        }
    }
    
    return totalmtx;
}