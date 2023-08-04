#include "nucvtskin.h"


struct primdef_s* primdefs_sorted;
struct primdef_s* primdefs;
s32 stritot;
s32 totaldupes;
s32 totalpts;
s32 tritot;
s32 stats[15];
static char mtxused[256];

void InitSkinning(s32 buffsize) //makes primitive

{
  primdefs = (struct primdef_s *)NuMemAlloc(0xc7ce0); // 0xC7CE0 bytes can fit 0x3548 NuPrims perfectly
  primdefs_sorted = (struct primdef_s *)NuMemAlloc(0xc7ce0);
  return;
}


void CloseSkinning(void)

{
  NuMemFree(primdefs);
  NuMemFree(primdefs_sorted);
  primdefs_sorted = NULL;
  primdefs = NULL;
  return;
}


void NuPs2CreateSkin(struct nugobj_s* gobj) {
    struct nugeom_s* tmp;

    tmp = gobj->geom;
    if ((tmp != NULL) && (tmp->prim->type == NUPT_NDXTRI)) {
        NuPs2CreateSkinNorm(gobj);
    }
return;
}



/********************************WIP*************************************************/

void CreateSkinGeom(struct nugeom_s* ge, struct primdef_s* pd, s32 pdcnt) {

    u32 amount_per_range_arr[300];    //primsize
    s32 uniq_baseid_ranges;
    s32 i;
    s32 j;
    s32 k;
    s32 vtx_k;
    u16 j1;
    s32 cur_baseid;
    u32* amount_per_range;
    struct nuprim_s* new_prim_list;    //nextprim
    struct nuprim_s* cur_prim;
    struct nuvtx_sk3tc1_s* vertex_buffer;
    struct primdef_s* cur_pd;
    float const* cur_vert_weights;
    s32 weight_idx;
    s32 nonzero_weight_count;
    struct nuprim_s* old_prims;



    if (ge->vtxtype != NUVT_TC1) {
        NuErrorProlog("C:/source/crashwoc/code/nu3dx/nucvtskn.c",0x69)("CreateSkinGeom : Unknown vertex type!");
    }

  memset(amount_per_range_arr, 0, sizeof(amount_per_range_arr));
  uniq_baseid_ranges = 1;
  for (i = 0, cur_baseid = pd[0].baseid; i < pdcnt; ++i) {
    if (pd[i].baseid != cur_baseid) {
      cur_baseid = pd[i].baseid;
      ++uniq_baseid_ranges;
    }
    amount_per_range_arr[uniq_baseid_ranges] += 3;
  }

  // Since the first element is never referenced, it's easier to read if working from index 1
  amount_per_range = amount_per_range_arr + 1;
  new_prim_list = NuPrimCreate(amount_per_range[0], NUPT_NDXTRI);
  cur_prim = new_prim_list; // r28
  for (i = 1; i < uniq_baseid_ranges; i++) {
    cur_prim->next = NuPrimCreate(amount_per_range[i], NUPT_NDXTRI);
    cur_prim = cur_prim->next;
  }

  vertex_buffer = (struct nuvtx_sk3tc1_s*)GS_CreateBuffer(ge->vtxcnt * sizeof(struct nuvtx_sk3tc1_s), 1);

  cur_pd = pd; // bound to r4
  cur_prim = new_prim_list; // bound to r27
  for (i = 0; i < uniq_baseid_ranges; i++) {
    for (j = 0; j < 15; j++) {
      cur_prim->skinmtxlookup[j] = cur_pd->mtxid[j];
    }

    // j ~= r10, r18
    for (j1 = 0; j1 < cur_prim->vertexCount; j1 += 3) {
      ((u16*)cur_prim->idxbuff)[j1] = cur_pd->vid[0];
      ((u16*)cur_prim->idxbuff)[j1 + 1] = cur_pd->vid[1];
      ((u16*)cur_prim->idxbuff)[j1 + 2] = cur_pd->vid[2];

      // k ~= r10, r24
      for (k = 0; k < 3; k++) {
        s32 vtx_k = cur_pd->vid[k];
        vertex_buffer[vtx_k].pnt.x = cur_pd->vrts[k].pnt.x;
        vertex_buffer[vtx_k].pnt.y = cur_pd->vrts[k].pnt.y;
        vertex_buffer[vtx_k].pnt.z = cur_pd->vrts[k].pnt.z;
        vertex_buffer[vtx_k].nrm.x = cur_pd->vrts[k].nrm.x;
        vertex_buffer[vtx_k].nrm.y = cur_pd->vrts[k].nrm.y;
        vertex_buffer[vtx_k].nrm.z = cur_pd->vrts[k].nrm.z;
        vertex_buffer[vtx_k].tc[0] = cur_pd->vrts[k].tc[0];
        vertex_buffer[vtx_k].tc[1] = cur_pd->vrts[k].tc[1];
        vertex_buffer[vtx_k].diffuse = cur_pd->vrts[k].diffuse;
        memset(vertex_buffer[vtx_k].weights, 0, sizeof(float[2]));
        memset(vertex_buffer[vtx_k].indexes, 0, sizeof(float[3]));

        // TODO: Likely need to flip weights around to be [3][15]
        cur_vert_weights = (float const*)cur_pd->weights + j * 15;
        weight_idx = 0;
        nonzero_weight_count = 0;
        while (weight_idx < 15 && nonzero_weight_count < 3) {
          if (cur_vert_weights[weight_idx] != 0.0f) {
            vertex_buffer[vtx_k].indexes[nonzero_weight_count] = (float)weight_idx;
            if (nonzero_weight_count > 0) {
              vertex_buffer[vtx_k].weights[nonzero_weight_count - 1] = cur_vert_weights[weight_idx];
            }
            ++nonzero_weight_count;
          }
          ++weight_idx;
        }
      }
    }
  }

  GS_DeleteBuffer((u8*)ge->hVB);
  old_prims = ge->prim;
  while (old_prims != NULL) {
    struct NuPrim* old_prim_next = old_prims->next;
    NuPrimDestroy(old_prims);
    old_prims = old_prim_next;
  }

  ge->hVB = vertex_buffer;
  ge->vtxtype = NUVT_SK3TC1;
  ge->prim = new_prim_list;
}


/*void NuPs2CreateSkinNorm(nugobj_s *gobj)

{
  struct primdef_s *primdef_2;
  int iVar2;
  int *num_mtx;
  int pdcnt;
  struct nuprim_s *prims;
  int skinix;
  int iVar3;
  struct nuvtx_tc1_s *srcvb;
  struct nugeom_s *ge;
  struct primdef_s *pd;
  int vid;
  int iVar4;

  memset(stats,0,0x3c);
  ge = gobj->geom;
  stritot = 0;
  totaldupes = 0;
  totalpts = 0;
  tritot = 0;
  pd = primdefs;
  for (; ge != (nugeom_s *)0x0; ge = ge->next) {
    primdefs = pd;
    if ((ge->skin != (nuskin_s *)0x0) || (ge->vtxskininfo != (NUVTXSKININFO_s *)0x0)) {
      prims = ge->prim;
      pdcnt = 0;
      vid = prims->idxbuff;
      srcvb = (nuvtx_tc1_s *)ge->hVB;
      if (0x898 < prims->vertexCount / 3) {
        error = NuErrorProlog("C:/source/crashwoc/code/nu3dx/nucvtskn.c",0x35e);
        (*error)("NuPs2CreateSkinNorm: TOO MANY PRIMS!");
      }
      skinix = 0;
      if (prims->vertexCount != 0) {
        do {
          memset(pd,0,0x174);
          memset(pd->mtxid,-1,0x3c);
          if (ge->vtxskininfo == (NUVTXSKININFO_s *)0x0) {
            SetVtxSkinData(pd,0,srcvb,(uint)*(ushort *)vid,ge);
            SetVtxSkinData(pd,1,srcvb,(uint)*(ushort *)(vid + 2),ge);
            SetVtxSkinData(pd,2,srcvb,(uint)*(ushort *)(vid + 4),ge);
          }
          else {
            SetVtxSkinData2(pd,0,srcvb,(uint)*(ushort *)vid,ge);
            SetVtxSkinData2(pd,1,srcvb,(uint)*(ushort *)(vid + 2),ge);
            SetVtxSkinData2(pd,2,srcvb,(uint)*(ushort *)(vid + 4),ge);
          }
          if (((pd->vid[0] == pd->vid[1]) || (pd->vid[1] == pd->vid[2])) ||
             (pd->vid[0] == pd->vid[2])) {
            pd = pd + -1;
            pdcnt = pdcnt + -1;
          }
          num_mtx = &pd->nummtx;
          skinix = skinix + 3;
          pd = pd + 1;
          pdcnt = pdcnt + 1;
          vid = vid + 6;
          stats[*num_mtx] = stats[*num_mtx] + 1;
        } while (skinix < (int)(uint)prims->vertexCount);
      }
      iVar3 = 0;
      SortPrimdefs(primdefs,pdcnt);
      num_mtx = stats;
      iVar4 = 0xf;
      skinix = 0;
      iVar2 = 0;
      do {
        if ((*num_mtx != 0) && (iVar2 < iVar3)) {
          iVar2 = iVar3;
        }
        if (stats[skinix] <= *num_mtx) {
          skinix = iVar3;
        }
        num_mtx = num_mtx + 1;
        iVar3 = iVar3 + 1;
        iVar4 = iVar4 + -1;
      } while (iVar4 != 0);
      CreateSkinGeom(ge,primdefs,pdcnt);
    }
    pd = primdefs;
  }
  primdefs = pd;
  return;
}*/


// This function creates a skinned geometry. It does this by creating an array of
// primitives, setting vertex skin data, sorting the primitives and then creating a
// new geometry from the sorted primitive array. A better name for this function

void NuPs2CreateSkinNorm(struct nugobj_s *gobj)
{
    struct nuvtx_tc1_s *vertexBuffer;
    struct primdef_s *pd;

    memset(stats, NULL, sizeof(stats));

    struct nugeom_s* currGeom = gobj->geom;
    stritot = 0;
    totaldupes = 0;
    totalpts = 0;

    for (tritot = 0; currGeom; currGeom = currGeom->next)
    {
		s32 pdcnt = 0;

        if (currGeom->skin == NULL && !currGeom->vtxskininfo) {
            continue;
        }

        struct nuprim_s* prims = currGeom->prim;

        // Maximum amount of faces is 2200 (0x898)
        if ((prims->vertexCount / 3) > 2200)
        {
            /*ASSERT_FILE_LINE_MSG("C:/source/crashwoc/code/nu3dx/nucvtskn.c",
                            862, "NuPs2CreateSkinNorm: TOO MANY PRIMS!");*/
        }

        if (prims->vertexCount) {
            int skinnedVtxCount = 0;
            for (; skinnedVtxCount < prims->vertexCount; skinnedVtxCount += 3)
            {
                memset(primdefs, NULL, sizeof(primdefs));
                memset(primdefs->mtxid, -1, 60);

                if (currGeom->vtxskininfo)
                {
                    SetVtxSkinData2(primdefs, 0, vertexBuffer, prims->idxbuff, currGeom);
                    SetVtxSkinData2(primdefs, 1, vertexBuffer, prims->idxbuff + 2, currGeom);
                    SetVtxSkinData2(primdefs, 2, vertexBuffer, prims->idxbuff + 4, currGeom);
                }
                else
                {
                    SetVtxSkinData(primdefs, 0, vertexBuffer, prims->idxbuff, currGeom);
                    SetVtxSkinData(primdefs, 1, vertexBuffer, prims->idxbuff + 2, currGeom);
                    SetVtxSkinData(primdefs, 2, vertexBuffer, prims->idxbuff + 4, currGeom);
                }

				 if (((pd->vid[0] == pd->vid[1]) || (pd->vid[1] == pd->vid[2])) || (pd->vid[0] == pd->vid[2]))
				 {
					 primdefs -= 1;
					 pdcnt -= 1;

				 }

					primdefs+= 1;
					prims->idxbuff += 6;
					stats[primdefs->nummtx] = stats[primdefs->nummtx] + 1;
            }
        }

		SortPrimdefs(primdefs,pdcnt);

			//TODO


		CreateSkinGeom(currGeom,primdefs,pdcnt);
    }

	return;
}




int FillFreeMatrixSlots(struct primdef_s *pd,int cnt,int start) //todo
{
  int iVar1;
  int numbatchmtx;
  int m;
  int n;
  struct primdef_s *primdef_batch;
  int iVar2;
  int *piVar3;
  int iVar4;
  int iVar5;
  bool check;

  numbatchmtx = pd[start].nummtx;
  if (((start < cnt) && (numbatchmtx < 0xc)) && (n = start + 1, n < cnt)) {
    iVar1 = n * 0x174;
    iVar5 = numbatchmtx * 4 + 300;
    primdef_batch = pd + n;
    do {
      if ((primdef_batch->sorted != 1) && (m = 0, primdef_batch->mtxid[0] != -1)) {
        iVar2 = 0;
        piVar3 = (int *)((int)pd[start].vrts[0].tc + iVar5 + -0x1c);
        do {
          iVar4 = *(int *)((int)pd->mtxid + iVar2 + iVar1 + 300 + -300);
          if (mtxused[iVar4] == '\0') {
            *piVar3 = iVar4;
            check = numbatchmtx == 0xb;
            piVar3 = piVar3 + 1;
            iVar5 = iVar5 + 4;
            numbatchmtx = numbatchmtx + 1;
            mtxused[*(int *)((int)pd->mtxid + iVar2 + iVar1 + 300 + -300)] = '\x01';
            if (check) {
              return 0xc;
            }
          }
          m = m + 1;
          iVar2 = iVar2 + 4;
        } while ((m < 0xc) && (*(int *)((int)pd->mtxid + iVar2 + iVar1) != -1));
      }
      n = n + 1;
      primdef_batch = primdef_batch + 1;
      iVar1 = iVar1 + 0x174;
    } while (n < cnt);
  }
  return numbatchmtx;
}


int SortPrimdefs(struct primdef_s *primdef,int count)
{
    return;
}

/*
int SortPrimdefs(primdef_s *primdef,int count)

{
  int iVar1;
  NUERRORFUNC *e;
  int iVar2;
  int iVar3;
  primdef_s *ppVar4;
  float fVar5;
  int iVar6;
  primdef_s *ppVar7;
  primdef_s *ppVar8;
  int iVar9;
  int *piVar11;
  matchingslot_s *pmVar12;
  primdef_s *pafVar12;
  primdef_s *pSorted;
  uint uVar13;
  float *pfVar14;
  int *piVar15;
  primdef_s *piVar10;
  float totalweights;
  int iVar16;
  int iVar17;
  int iVar18;
  primdef_s *ppVar19;
  primdef_s *ppVar20;
  int iVar21;
  int iVar22;
  int iVar23;
  int n;
  int i;
  primdef_s tpd;
  int batchmatrices [15] [200];
  int batchmatrices_sorted [15] [200];
  int matrixusecount [150];
  matchingslot_s matchingslot [200];
  int cnt;
  int (*batchmtx_sorted) [200];
  int *mtxusecnt;
  int bID;
  int k;

  mtxusecnt = matrixusecount;
  batchmtx_sorted = batchmatrices_sorted;
  bID = 0;
  n = 0;
  if (0 < count) {
    do {
      i = n + 1;
      if (i < count) {
        pSorted = primdef + n;
        n = i;
        do {
          iVar17 = n + 1;
          if (pSorted->nummtx < primdef[n].nummtx) {
            k = 0x168;
            ppVar19 = &tpd;
            ppVar20 = pSorted;
            do {
              ppVar4 = ppVar20;
              ppVar8 = ppVar19;
              k = k + -0x18;
              ppVar8->vrts[0].pnt.x = ppVar4->vrts[0].pnt.x;
              ppVar8->vrts[0].pnt.y = ppVar4->vrts[0].pnt.y;
              ppVar8->vrts[0].pnt.z = ppVar4->vrts[0].pnt.z;
              ppVar8->vrts[0].nrm.x = ppVar4->vrts[0].nrm.x;
              ppVar8->vrts[0].nrm.y = ppVar4->vrts[0].nrm.y;
              ppVar20 = (primdef_s *)&ppVar4->vrts[0].diffuse;
              ppVar8->vrts[0].nrm.z = ppVar4->vrts[0].nrm.z;
              ppVar19 = (primdef_s *)&ppVar8->vrts[0].diffuse;
            } while (k != 0);
            k = 0x168;
            *(int *)ppVar19 = *(int *)ppVar20;
            ppVar8->vrts[0].tc[0] = ppVar4->vrts[0].tc[0];
            ppVar8->vrts[0].tc[1] = ppVar4->vrts[0].tc[1];
            ppVar19 = pSorted;
            ppVar20 = primdef + n;
            do {
              ppVar4 = ppVar20;
              ppVar8 = ppVar19;
              k = k + -0x18;
              ppVar8->vrts[0].pnt.x = ppVar4->vrts[0].pnt.x;
              ppVar8->vrts[0].pnt.y = ppVar4->vrts[0].pnt.y;
              ppVar8->vrts[0].pnt.z = ppVar4->vrts[0].pnt.z;
              ppVar8->vrts[0].nrm.x = ppVar4->vrts[0].nrm.x;
              ppVar8->vrts[0].nrm.y = ppVar4->vrts[0].nrm.y;
              ppVar20 = (primdef_s *)&ppVar4->vrts[0].diffuse;
              ppVar8->vrts[0].nrm.z = ppVar4->vrts[0].nrm.z;
              ppVar19 = (primdef_s *)&ppVar8->vrts[0].diffuse;
            } while (k != 0);
            k = 0x168;
            *(int *)ppVar19 = *(int *)ppVar20;
            ppVar8->vrts[0].tc[0] = ppVar4->vrts[0].tc[0];
            ppVar8->vrts[0].tc[1] = ppVar4->vrts[0].tc[1];
            ppVar19 = primdef + n;
            ppVar20 = &tpd;
            do {
              ppVar4 = ppVar20;
              ppVar8 = ppVar19;
              k = k + -0x18;
              ppVar8->vrts[0].pnt.x = ppVar4->vrts[0].pnt.x;
              ppVar8->vrts[0].pnt.y = ppVar4->vrts[0].pnt.y;
              ppVar8->vrts[0].pnt.z = ppVar4->vrts[0].pnt.z;
              ppVar8->vrts[0].nrm.x = ppVar4->vrts[0].nrm.x;
              ppVar8->vrts[0].nrm.y = ppVar4->vrts[0].nrm.y;
              ppVar20 = (primdef_s *)&ppVar4->vrts[0].diffuse;
              ppVar8->vrts[0].nrm.z = ppVar4->vrts[0].nrm.z;
              ppVar19 = (primdef_s *)&ppVar8->vrts[0].diffuse;
            } while (k != 0);
            *(int *)ppVar19 = *(int *)ppVar20;
            ppVar8->vrts[0].tc[0] = ppVar4->vrts[0].tc[0];
            ppVar8->vrts[0].tc[1] = ppVar4->vrts[0].tc[1];
          }
          n = iVar17;
        } while (iVar17 < count);
      }
      n = i;
    } while (i < count);
  }
  n = 0;
  i = n;
  iVar17 = bID;
  k = 0;
  if (0 < count) {
    do {
      iVar23 = k + 1;
      n = i;
      bID = iVar17;
      if (primdef[k].sorted == 0) {
        bID = iVar17 + 1;
        memset(mtxused,0,0x100);
        n = 0;
        iVar21 = i + 1;
        if (0 < primdef[k].nummtx) {
          piVar11 = primdef[k].mtxid;
          do {
            iVar9 = *piVar11;
            n = n + 1;
            piVar11 = piVar11 + 1;
            mtxused[iVar9] = '\x01';
          } while (n < primdef[k].nummtx);
        }
        n = FillFreeMatrixSlots(primdef,count,k);
        pSorted = primdefs_sorted;
        ppVar19 = primdef + k;
        ppVar19->nummtx = n;
        n = 0x168;
        ppVar19->baseid = i;
        pSorted = pSorted + i;
        do {
          ppVar8 = ppVar19;
          ppVar20 = pSorted;
          n = n + -0x18;
          ppVar20->vrts[0].pnt.x = ppVar8->vrts[0].pnt.x;
          ppVar20->vrts[0].pnt.y = ppVar8->vrts[0].pnt.y;
          ppVar20->vrts[0].pnt.z = ppVar8->vrts[0].pnt.z;
          ppVar20->vrts[0].nrm.x = ppVar8->vrts[0].nrm.x;
          ppVar20->vrts[0].nrm.y = ppVar8->vrts[0].nrm.y;
          ppVar19 = (primdef_s *)&ppVar8->vrts[0].diffuse;
          ppVar20->vrts[0].nrm.z = ppVar8->vrts[0].nrm.z;
          pSorted = (primdef_s *)&ppVar20->vrts[0].diffuse;
        } while (n != 0);
        *(int *)pSorted = *(int *)ppVar19;
        ppVar20->vrts[0].tc[0] = ppVar8->vrts[0].tc[0];
        ppVar20->vrts[0].tc[1] = ppVar8->vrts[0].tc[1];
        pSorted = primdefs_sorted;
        n = iVar21;
        if (iVar23 < count) {
          iVar9 = iVar23;
          ppVar19 = primdefs_sorted + iVar21;
          iVar16 = iVar21 * 0x174;
          do {
            iVar18 = iVar9 + 1;
            ppVar20 = ppVar19;
            iVar22 = iVar16;
            if ((primdef[iVar9].sorted == 0) && (primdef[iVar9].nummtx <= primdef[k].nummtx)) {
              iVar6 = 0;
              iVar2 = 0;
              if (0 < primdef[iVar9].nummtx) {
                iVar3 = 0;
                do {
                  iVar2 = iVar2 + 1;
                  piVar11 = (int *)((int)primdef[iVar9].mtxid + iVar3);
                  iVar3 = iVar3 + 4;
                  uVar13 = (int)((int)mtxused[*piVar11] - 1U) >> 0x1f;
                  iVar6 = iVar6 + ((uVar13 ^ (int)mtxused[*piVar11] - 1U) - uVar13);
                } while (iVar2 < primdef[iVar9].nummtx);
              }
              if (iVar6 == 0) {
                primdef[iVar9].baseid = i;
                n = 0x168;
                iVar22 = iVar16 + 0x174;
                ppVar20 = ppVar19 + 1;
                iVar21 = iVar21 + 1;
                ppVar8 = primdef + iVar9;
                do {
                  ppVar7 = ppVar8;
                  ppVar4 = ppVar19;
                  n = n + -0x18;
                  ppVar4->vrts[0].pnt.x = ppVar7->vrts[0].pnt.x;
                  ppVar4->vrts[0].pnt.y = ppVar7->vrts[0].pnt.y;
                  ppVar4->vrts[0].pnt.z = ppVar7->vrts[0].pnt.z;
                  ppVar4->vrts[0].nrm.x = ppVar7->vrts[0].nrm.x;
                  ppVar4->vrts[0].nrm.y = ppVar7->vrts[0].nrm.y;
                  ppVar8 = (primdef_s *)&ppVar7->vrts[0].diffuse;
                  ppVar4->vrts[0].nrm.z = ppVar7->vrts[0].nrm.z;
                  ppVar19 = (primdef_s *)&ppVar4->vrts[0].diffuse;
                } while (n != 0);
                *(int *)ppVar19 = *(int *)ppVar8;
                ppVar4->vrts[0].tc[0] = ppVar7->vrts[0].tc[0];
                pfVar14 = (float *)((int)pSorted->weights[10] + iVar16);
                ppVar4->vrts[0].tc[1] = ppVar7->vrts[0].tc[1];
                n = 0xf;
                primdef[iVar9].sorted = 1;
                do {
                  pfVar14[0xf] = -NAN;
                  pfVar14[-0x1e] = 0.0;
                  pfVar14[-0xf] = 0.0;
                  *pfVar14 = 0.0;
                  pfVar14 = pfVar14 + 1;
                  n = n + -1;
                } while (n != 0);
                iVar2 = 0;
                n = iVar21;
                if (0 < primdef[k].nummtx) {
                  do {
                    iVar6 = 0;
                    iVar3 = iVar2 + 1;
                    if (0 < primdef[iVar9].nummtx) {
                      iVar1 = iVar2 * 4;
                      totalweights = (float)primdef[iVar9].weights[10];
                      do {
                        if (primdef[k].mtxid[iVar2] == *(int *)((int)totalweights + 0x3c)) {
                          *(undefined4 *)((int)pSorted->weights + iVar1 + iVar16) =
                               *(undefined4 *)((int)totalweights + -0x78);
                          *(undefined4 *)((int)pSorted->weights[5] + iVar1 + iVar16) =
                               *(undefined4 *)((int)totalweights + -0x3c);
                          *(undefined4 *)((int)pSorted->weights[10] + iVar1 + iVar16) =
                               *(undefined4 *)totalweights;
                          *(int *)((int)pSorted->mtxid + iVar1 + iVar16) = primdef[k].mtxid[iVar2] ;
                        }
                        iVar6 = iVar6 + 1;
                        totalweights = (float)((int)totalweights + 4);
                      } while (iVar6 < primdef[iVar9].nummtx);
                    }
                    iVar2 = iVar3;
                  } while (iVar3 < primdef[k].nummtx);
                }
              }
            }
            iVar9 = iVar18;
            ppVar19 = ppVar20;
            iVar16 = iVar22;
          } while (iVar18 < count);
        }
        k = 0xf;
        piVar15 = batchmatrices + iVar17 * 0xf;
        piVar11 = primdefs_sorted[i].mtxid;
        do {
          i = *piVar11;
          piVar11 = piVar11 + 1;
          *piVar15 = i;
          piVar15 = piVar15 + 1;
          k = k + -1;
        } while (k != 0);
      }
      i = n;
      iVar17 = bID;
      k = iVar23;
    } while (iVar23 < count);
  }
  iVar17 = 0;
  memset(mtxusecnt,0,0x258);
  i = -1;
  if (0 < bID) {
    do {
      iVar23 = iVar17 * 0x3c;
      iVar21 = 0xf;
      iVar17 = iVar17 + 1;
      k = i;
      do {
        iVar9 = *(int *)((int)batchmatrices + iVar23);
        i = k;
        if (iVar9 != -1) {
          mtxusecnt[iVar9] = mtxusecnt[iVar9] + 1;
          i = *(int *)((int)batchmatrices + iVar23);
          if (i < k) {
            i = k;
          }
        }
        iVar23 = iVar23 + 4;
        iVar21 = iVar21 + -1;
        k = i;
      } while (iVar21 != 0);
    } while (iVar17 < bID);
  }
  memset(batchmtx_sorted,-1,0x2ee0);
  cnt = count;
  do {
    iVar17 = 0;
    iVar23 = 0;
    k = *mtxusecnt;
    piVar11 = mtxusecnt;
    if (-1 < i) {
      do {
        if (mtxusecnt[iVar17] < *piVar11) {
          iVar17 = iVar23;
          k = *piVar11;
        }
        iVar23 = iVar23 + 1;
        piVar11 = piVar11 + 1;
      } while (iVar23 <= i);
    }
    if (k == 0) break;
    iVar23 = 0;
    if (0 < bID) {
      iVar9 = 0;
      iVar21 = 0;
      do {
        iVar18 = 0xf;
        iVar16 = 0;
        iVar22 = iVar21 + 1;
        piVar11 = (int *)((int)&matchingslot[0].slot + iVar9);
        piVar15 = batchmatrices + iVar21 * 0xf;
        do {
          iVar2 = *piVar15;
          piVar15 = piVar15 + 1;
          if (iVar2 == iVar17) {
            piVar11[-1] = iVar21;
            iVar9 = iVar9 + 8;
            *piVar11 = iVar16;
            iVar23 = iVar23 + 1;
            piVar11 = piVar11 + 2;
          }
          iVar16 = iVar16 + 1;
          iVar18 = iVar18 + -1;
        } while (iVar18 != 0);
        iVar21 = iVar22;
      } while (iVar22 < bID);
    }
    iVar9 = 0;
    iVar21 = iVar9;
    if (iVar23 < 1) {
LAB_800ae5b0:
      if (0 < iVar23) {
        pmVar12 = matchingslot;
        do {
          piVar11 = &pmVar12->batch;
          iVar23 = iVar23 + -1;
          pmVar12 = pmVar12 + 1;
          batchmtx_sorted[-1][*piVar11 * 0xf + iVar9 + 199] = iVar17;
          mtxusecnt[iVar17] = 0;
        } while (iVar23 != 0);
      }
    }
    else {
      do {
        iVar16 = 0;
        iVar9 = iVar21 + 1;
        if (0 < iVar23) {
          pmVar12 = matchingslot;
          iVar22 = iVar23;
          do {
            piVar11 = &pmVar12->batch;
            pmVar12 = pmVar12 + 1;
            if ((*batchmtx_sorted)[*piVar11 * 0xf + iVar21] == -1) {
              iVar16 = iVar16 + 1;
            }
            iVar22 = iVar22 + -1;
          } while (iVar22 != 0);
        }
        if (iVar23 <= iVar16) goto LAB_800ae5b0;
        iVar21 = iVar9;
      } while (iVar9 < 0xf);
      e = NuErrorProlog("C:/source/crashwoc/code/nu3dx/nucvtskn.c",0x232);
      (*e)("SortPrimDefs: Unable to find a matching free slot in all batches!!");
    }
  } while (k != 0);
  pSorted = primdefs_sorted;
  bID = 0;
  if (0 < cnt) {
    iVar17 = 0;
    i = 0;
    do {
      k = pSorted[i].baseid;
      if (k != bID) {
        iVar17 = iVar17 + 0x3c;
        bID = k;
      }
      iVar23 = i + 1;
      k = 0x168;
      ppVar19 = pSorted + i;
      ppVar20 = primdef + i;
      do {
        ppVar4 = ppVar20;
        ppVar8 = ppVar19;
        k = k + -0x18;
        ppVar4->vrts[0].pnt.x = ppVar8->vrts[0].pnt.x;
        ppVar4->vrts[0].pnt.y = ppVar8->vrts[0].pnt.y;
        ppVar4->vrts[0].pnt.z = ppVar8->vrts[0].pnt.z;
        ppVar4->vrts[0].nrm.x = ppVar8->vrts[0].nrm.x;
        ppVar4->vrts[0].nrm.y = ppVar8->vrts[0].nrm.y;
        ppVar19 = (primdef_s *)&ppVar8->vrts[0].diffuse;
        ppVar4->vrts[0].nrm.z = ppVar8->vrts[0].nrm.z;
        ppVar20 = (primdef_s *)&ppVar4->vrts[0].diffuse;
      } while (k != 0);
      pafVar12 = (primdef_s *)primdef[i].weights[10];
      *(int *)ppVar20 = *(int *)ppVar19;
      k = 0xf;
      ppVar4->vrts[0].tc[0] = ppVar8->vrts[0].tc[0];
      ppVar4->vrts[0].tc[1] = ppVar8->vrts[0].tc[1];
      do {
        pafVar12->vrts[1].diffuse = -1;
        pafVar12[-1].weights[0xb][0] = 0.0;
        pafVar12[-1].mtxid[3] = (int)0.0;
        pafVar12->vrts[0].pnt.x = 0.0;
        pafVar12 = (primdef_s *)&pafVar12->vrts[0].pnt.y;
        k = k + -1;
      } while (k != 0);
      k = pSorted[bID].nummtx;
      primdef[i].baseid = bID;
      primdef[i].nummtx = k;
      k = 0;
      do {
        iVar21 = k + 1;
        piVar10 = (primdef_s *)pSorted[i].mtxid;
        fVar5 = *(float *)((int)*batchmtx_sorted + k * 4 + iVar17);
        iVar9 = 0xf;
        do {
          if (fVar5 == piVar10->vrts[0].pnt.x) {
            primdef[i].weights[k] = piVar10[-1].weights[6][0];
            primdef[i].mtxid[k + -0x1e] = (int)piVar10[-1].weights[0xb][0];
            primdef[i].mtxid[k + -0xf] = piVar10[-1].mtxid[3];
            primdef[i].mtxid[k] = (int)piVar10->vrts[0].pnt.x;
          }
          piVar10 = (primdef_s *)&piVar10->vrts[0].pnt.y;
          iVar9 = iVar9 + -1;
        } while (iVar9 != 0);
        k = iVar21;
      } while (iVar21 < 0xf);
      i = iVar23;
    } while (iVar23 < cnt);
  }
  i = 0;
  bID = -1;
  pSorted = primdefs_sorted;
  if (0 < n) {
    do {
      if (pSorted->baseid != bID) {
        i = i + pSorted->nummtx;
        bID = pSorted->baseid;
      }
      n = n + -1;
      pSorted = pSorted + 1;
    } while (n != 0);
  }
  return i;
}
*/



s32 AddMtxToPrimDef(struct primdef_s *primdef,s32 mtxid)

{
  s32 n;
  s32 i;

  i = 0;
  if (0 < primdef->nummtx) {
 do {
      n = *primdef->mtxid;
      *primdef->mtxid += 1;
      if (n == mtxid) {
        return i;
      }
      i = i + 1;
    } while (i < primdef->nummtx);
  }
  if (i < 0xf) {
    primdef->mtxid[i] = mtxid;
    primdef->nummtx = primdef->nummtx + 1;
    return i;
  }
  return 0;
}



void SetVtxSkinData(struct primdef_s *pd,int pdix,struct nuvtx_tc1_s *vb,int vid,struct nugeom_s *currgeom) //WIP
{
  int iVar1;
  struct nuvtx_tc1_s *pnVar2;
  struct nuvtx_tc1_s *pnVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  struct nuvtx_tc1_s *pnVar7;
  struct nuvtx_tc1_s *pnVar8;
  float *pfVar9;
  int iVar10;
  int iVar11;
  struct nuskin_s *skin_;

  iVar5 = 0x18;
  pnVar2 = pd->vrts + pdix;
  pnVar3 = vb + vid;
  do {
    pnVar8 = pnVar3;
    pnVar7 = pnVar2;
    iVar5 = iVar5 + -0x18;
    (pnVar7->pnt).x = (pnVar8->pnt).x;
    (pnVar7->pnt).y = (pnVar8->pnt).y;
    (pnVar7->pnt).z = (pnVar8->pnt).z;
    (pnVar7->nrm).x = (pnVar8->nrm).x;
    (pnVar7->nrm).y = (pnVar8->nrm).y;
    (pnVar7->nrm).z = (pnVar8->nrm).z;
    pnVar2 = (struct nuvtx_tc1_s *)&pnVar7->diffuse;
    pnVar3 = (struct nuvtx_tc1_s *)&pnVar8->diffuse;
  } while (iVar5 != 0);
  pnVar7->diffuse = pnVar8->diffuse;
  pnVar7->tc[0] = pnVar8->tc[0];
  pnVar7->tc[1] = pnVar8->tc[1];
  pd->vid[pdix] = vid;
  if (pdix < 3) {
    for (skin_ = currgeom->skin; skin_ != NULL; skin_ = skin_->next) {
      iVar5 = vid - skin_->vtxoffset;
      if ((-1 < iVar5) && (iVar5 < skin_->vtxcnt)) {
        iVar4 = skin_->mtxcnt;
        iVar10 = 0;
        pfVar9 = skin_->weights;
        if (iVar4 < 1) {
          return;
        }
        iVar11 = 0;
        do {
          iVar10 = iVar10 + 1;
          iVar6 = AddMtxToPrimDef(pd,*(int *)(iVar11 + (int)skin_->mtxid));
          iVar1 = iVar11 + iVar4 * iVar5 * 4;
          iVar11 = iVar11 + 4;
          pd->weights[pdix * 5][iVar6] = *(float *)((int)pfVar9 + iVar1);
        } while (iVar10 < skin_->mtxcnt);
        return;
      }
    }
  }
  return;
}



void SetVtxSkinData2(struct primdef_s *pd,int pdix,struct nuvtx_tc1_s *vertexbuf,int vid,struct nugeom_s *currgeom) //WIP
{
  bool bVar1;
  struct nuvtx_tc1_s *pnVar2;
  struct nuvtx_tc1_s *pnVar3;
  struct nuvtx_tc1_s *pnVar4;
  int iVar5;
  struct nuvtx_tc1_s *pnVar6;
  struct NUVTXSKININFO_s *pNVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  double dVar11;

  iVar8 = 0x18;
  pnVar2 = pd->vrts + pdix;
  pnVar3 = vertexbuf + vid;
  do {
    pnVar6 = pnVar3;
    pnVar4 = pnVar2;
    iVar8 = iVar8 + -0x18;
    (pnVar4->pnt).x = (pnVar6->pnt).x;
    (pnVar4->pnt).y = (pnVar6->pnt).y;
    (pnVar4->pnt).z = (pnVar6->pnt).z;
    (pnVar4->nrm).x = (pnVar6->nrm).x;
    (pnVar4->nrm).y = (pnVar6->nrm).y;
    (pnVar4->nrm).z = (pnVar6->nrm).z;
    pnVar2 = (struct nuvtx_tc1_s *)&pnVar4->diffuse;
    pnVar3 = (struct nuvtx_tc1_s *)&pnVar6->diffuse;
  } while (iVar8 != 0);
  pnVar4->diffuse = pnVar6->diffuse;
  iVar8 = 0;
  dVar11 = 0.0;
  pnVar4->tc[0] = pnVar6->tc[0];
  pnVar4->tc[1] = pnVar6->tc[1];
  pd->vid[pdix] = vid;
  pNVar7 = currgeom->vtxskininfo + vid;
  if (pNVar7->wts[0] != 0.0) {
    iVar9 = 0;
    iVar10 = 0;
    do {
      iVar9 = iVar9 + 4;
      iVar5 = AddMtxToPrimDef(pd,(u32)pNVar7->joint_ixs[iVar8]);
      bVar1 = iVar8 == 2;
      pd->weights[pdix * 5][iVar5] = *(float *)((int)pNVar7->wts + iVar10);
      iVar10 = iVar10 + 4;
      iVar8 = iVar8 + 1;
      if (bVar1) {
        return;
      }
    } while ((double)*(float *)((int)pNVar7->wts + iVar9) != dVar11);
  }
  return;
}
