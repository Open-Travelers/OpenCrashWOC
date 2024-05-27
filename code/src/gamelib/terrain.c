#include "../nu.h"


#define POW2(x) ((x) * (x))


int ReadTerrain(uchar *name2,int situ,short **store,TempTerr *Tempterr)

{
  int size;
  sceneptr *scnptr;
  int i;
  int c;
  int count;
  char LevName [108];
  WallSpl *wallspl;
  float x;
  float x2;
  float z;
  float z2;
  int c2;
  int k;
  int cnt;
  short type;
  
  strcpy(LevName,(char *)name2);
  strcat(LevName,".ter");
  size = NuFileLoadBuffer(LevName,*store,0x7fffffff);
  if (size == 0) {
    count = -1;
  }
  else {
    scnptr = (sceneptr *)((int)*store + *(int *)*store * 2);
    scnptr->count = scnptr->count & 0xffff;
    *store = *store + 2;
    count = 0;
    Tempterr->wallinfo = (void *)0x0;
    crashdata = (void *)0x0;
    i = situ;
    for (c = 0; c < scnptr->count; c = c + 1) {
      type = scnptr->offlist[c].type;
      if (type < 2) {
        if (-1 < type) {
          Tempterr->terr[i].id = scnptr->offlist[c].id;
          Tempterr->terr[i].type = (int)scnptr->offlist[c].type;
          Tempterr->terr[i].info = scnptr->offlist[c].info;
          Tempterr->terr[i].Location.x = scnptr->offlist[c].translation.x;
          Tempterr->terr[i].Location.y = scnptr->offlist[c].translation.y;
          Tempterr->terr[i].Location.z = scnptr->offlist[c].translation.z;
          Tempterr->terr[i].model = *store;
          Tempterr->terr[i].flags.rot = scnptr->offlist[c].flags.rot;
          count = count + 1;
          i = i + 1;
        }
      }
      else if (type == 2) {
        if (*(int *)*store == 0x12345678) {
          *(void **)*store = Tempterr->wallinfo;
          Tempterr->wallinfo = *store + 2;
                    /* WARNING: Load size is inaccurate */
          *(short *)Tempterr->wallinfo = *Tempterr->wallinfo + -1;
        }
      }
      else if (type == 3) {
        crashdata = *store;
      }
      *store = *store + scnptr->offlist[c].offset;
    }
    for (wallspl = (WallSpl *)Tempterr->wallinfo; wallspl != (WallSpl *)0x0;
        wallspl = (WallSpl *)wallspl[-1].spl[0xfff].z) {
      for (c2 = 0; c2 < (int)(uint)wallspl->count; c2 = c2 + 0x10) {
        if (wallspl->spl[c2].y != 2.147484e+09) {
          if (c2 + 0xf < (int)(uint)wallspl->count) {
            x = wallspl->spl[c2].x;
            z = wallspl->spl[c2].z;
            x2 = x;
            z2 = z;
            size = c2;
            while (cnt = size, k = cnt + 1, k <= c2 + 0x10) {
              if (wallspl->spl[cnt + 1].x < x) {
                x = wallspl->spl[cnt + 1].x;
              }
              if (x2 < wallspl->spl[cnt + 1].x) {
                x2 = wallspl->spl[cnt + 1].x;
              }
              if (wallspl->spl[cnt + 1].z < z) {
                z = wallspl->spl[cnt + 1].z;
              }
              size = k;
              if (z2 < wallspl->spl[cnt + 1].z) {
                z2 = wallspl->spl[cnt + 1].z;
                size = k;
              }
            }
            wallspl->spl[c2].y = x;
            wallspl->spl[c2 + 1].y = x2;
            wallspl->spl[c2 + 2].y = z;
            wallspl->spl[c2 + 3].y = z2;
          }
          else {
            wallspl->spl[c2].y = 2.147484e+09;
          }
        }
      }
    }
  }
  return count;
}

//NGC MATCH
void TerrFlush(void) {
  curSphereter = 0;
  TerrShapeAdjCnt = 0;
  curPickInst = 0;
}


//NGC MATCH
void noterraininit(void)  {
  s32 platid;
    
  terraincnt = 0;
  curSphereter = 0;
  platinrange = 0;
  platid = 0;
  ShadPoly = NULL;
  CurTerr = NULL;
  TerrFlush();
}

//NGC MATCH
void TerrainSetCur(void *curterr) {
  CurTerr = (struct CurTerr_s*)curterr;
}

//NGC MATCH
void TerrainPlatformNewUpdate(void) {
  s32 b;
  short *ttemp;
  struct terr_s *mbuf2;
  
  if (CurTerr == NULL) {
      return;
  }
    ttemp = CurTerr->terrlist + CurTerr->terrgroup[0x100].tabindex;
    for (b = 0; b < CurTerr->terrgroup[0x100].count; b++, ttemp++) {
      mbuf2 = CurTerr->terr + *ttemp;
      if (CurTerr->platdata[mbuf2->info].curmtx != NULL) {
        if (CurTerr->platdata[mbuf2->info].hitcnt != 0) {
          if (CurTerr->platdata[mbuf2->info].hitcnt > 0x7c) {
            CurTerr->platdata[mbuf2->info].yvel += (CurTerr->platdata[mbuf2->info].plrgrav -
                 CurTerr->platdata[mbuf2->info].yvel * CurTerr->platdata[mbuf2->info].damp);
          }
          else {
            CurTerr->platdata[mbuf2->info].yvel +=
                 -CurTerr->platdata[mbuf2->info].yvel * CurTerr->platdata[mbuf2->info].damp;
          }
          CurTerr->platdata[mbuf2->info].yvel +=
               ((NuFabs(CurTerr->platdata[mbuf2->info].ypos) * -CurTerr->platdata[mbuf2->info].ypos) *
                CurTerr->platdata[mbuf2->info].tension * 0.5f -
               CurTerr->platdata[mbuf2->info].ypos * CurTerr->platdata[mbuf2->info].tension);
          CurTerr->platdata[mbuf2->info].ypos += CurTerr->platdata[mbuf2->info].yvel;
          CurTerr->platdata[mbuf2->info].hitcnt--;
          (CurTerr->platdata[mbuf2->info].curmtx)->_31 += CurTerr->platdata[mbuf2->info].ypos;
        }
        (mbuf2->Location).x = (CurTerr->platdata[mbuf2->info].curmtx)->_30;
        (mbuf2->Location).y = (CurTerr->platdata[mbuf2->info].curmtx)->_31;
        (mbuf2->Location).z = (CurTerr->platdata[mbuf2->info].curmtx)->_32;
      }
      if ((CurTerr->platdata[mbuf2->info].status.hit) != 0) {
        CurTerr->platdata[mbuf2->info].hitcnt = 0x80;
      }
      CurTerr->platdata[mbuf2->info].status.hit = 0;
    }
}

//NGC MATCH
void DerotateMovementVector(void) {
  TerI->ay = NuAtan2D((TerI->curvel).x,(TerI->curvel).z);
  TerI->ax = NuAtan2D(-(TerI->curvel).y,NuFsqrt((TerI->curvel).x * (TerI->curvel).x + (TerI->curvel).z * (TerI->curvel).z));
  TerI->len = NuFsqrt((TerI->curvel).x * (TerI->curvel).x + (TerI->curvel).y * (TerI->curvel).y +
                  (TerI->curvel).z * (TerI->curvel).z);
}

//NGC MATCH
void RotateVec(struct nuvec_s *in,struct nuvec_s *out) {
  float tz;
  
  tz = in->y * NuTrigTable[(s32)TerI->ax & 0xffff] +
       in->z * NuTrigTable[(s32)(TerI->ax + 16384.0f) & 0xffff];
  out->y = in->y * NuTrigTable[(s32)(TerI->ax + 16384.0f) & 0xffff] -
           in->z * NuTrigTable[(s32)TerI->ax & 0xffff];
  out->z = tz * NuTrigTable[(s32)(TerI->ay + 16384.0f) & 0xffff] -
           in->x * NuTrigTable[(s32)TerI->ay & 0xffff];
  out->x = tz * NuTrigTable[(s32)TerI->ay & 0xffff] +
           in->x * NuTrigTable[(s32)(TerI->ay + 16384.0f) & 0xffff];
}

//NGC MATCH
void DeRotateTerrain(struct scaleterrain_s *ter) {
  float sinax;
  float cosax;
  float sinay;
  float cosay;
  float tz;
  float dist[8];
  
  sinax = NuTrigTable[(s32)-TerI->ax & 0xffff];
  cosax = NuTrigTable[(s32)(-TerI->ax + 16384.0f) & 0xffff];
  sinay = NuTrigTable[(s32)-TerI->ay & 0xffff];
  cosay = NuTrigTable[(s32)(-TerI->ay + 16384.0f) & 0xffff];
    dist[0] = (f32) (((ter->pnts[0].x - TerI->csx) * (ter->pnts[0].x - TerI->csx)) + ((ter->pnts[0].y - TerI->csy) * (ter->pnts[0].y - TerI->csy)) + ((ter->pnts[0].z - TerI->csz) * (ter->pnts[0].z - TerI->csz)));
    dist[1] = (f32) (((ter->pnts[1].x - TerI->csx) * (ter->pnts[1].x - TerI->csx)) + ((ter->pnts[1].y - TerI->csy) * (ter->pnts[1].y - TerI->csy)) + ((ter->pnts[1].z - TerI->csz) * (ter->pnts[1].z - TerI->csz)));
    dist[2] = (f32) (((ter->pnts[2].x - TerI->csx) * (ter->pnts[2].x - TerI->csx)) + ((ter->pnts[2].y - TerI->csy) * (ter->pnts[2].y - TerI->csy)) + ((ter->pnts[2].z - TerI->csz) * (ter->pnts[2].z - TerI->csz)));
    dist[3] = (f32) (((ter->pnts[3].x - TerI->csx) * (ter->pnts[3].x - TerI->csx)) + ((ter->pnts[3].y - TerI->csy) * (ter->pnts[3].y - TerI->csy)) + ((ter->pnts[3].z - TerI->csz) * (ter->pnts[3].z - TerI->csz)));
  tz = (ter->pnts[0].z - TerI->csz) * cosay - (ter->pnts[0].x - TerI->csx) * sinay;
  (TerI->rotter).pnts[0].x =
       (ter->pnts[0].z - TerI->csz) * sinay + (ter->pnts[0].x - TerI->csx) * cosay;
  (TerI->rotter).pnts[0].y = (ter->pnts[0].y - TerI->csy) * cosax - tz * sinax;
  (TerI->rotter).pnts[0].z = (ter->pnts[0].y - TerI->csy) * sinax + tz * cosax;
  tz = (ter->pnts[1].z - TerI->csz) * cosay - (ter->pnts[1].x - TerI->csx) * sinay;
  (TerI->rotter).pnts[1].x =
       (ter->pnts[1].z - TerI->csz) * sinay + (ter->pnts[1].x - TerI->csx) * cosay;
  (TerI->rotter).pnts[1].y = (ter->pnts[1].y - TerI->csy) * cosax - tz * sinax;
  (TerI->rotter).pnts[1].z = (ter->pnts[1].y - TerI->csy) * sinax + tz * cosax;
  tz = (ter->pnts[2].z - TerI->csz) * cosay - (ter->pnts[2].x - TerI->csx) * sinay;
  (TerI->rotter).pnts[2].x =
       (ter->pnts[2].z - TerI->csz) * sinay + (ter->pnts[2].x - TerI->csx) * cosay;
  (TerI->rotter).pnts[2].y = (ter->pnts[2].y - TerI->csy) * cosax - tz * sinax;
  (TerI->rotter).pnts[2].z = (ter->pnts[2].y - TerI->csy) * sinax + tz * cosax;
  if (ter->norm[1].y < 65536.0f) {
    tz = (ter->pnts[3].z - TerI->csz) * cosay - (ter->pnts[3].x - TerI->csx) * sinay;
    (TerI->rotter).pnts[3].x =
         (ter->pnts[3].z - TerI->csz) * sinay + (ter->pnts[3].x - TerI->csx) * cosay;
    (TerI->rotter).pnts[3].y = (ter->pnts[3].y - TerI->csy) * cosax - tz * sinax;
    (TerI->rotter).pnts[3].z = (ter->pnts[3].y - TerI->csy) * sinax + tz * cosax;
  }
    dist[4] = (f32) ((TerI->rotter.pnts[0].x * TerI->rotter.pnts[0].x) + (TerI->rotter.pnts[0].y * TerI->rotter.pnts[0].y) + (TerI->rotter.pnts[0].z * TerI->rotter.pnts[0].z));
    dist[5] = (f32) ((TerI->rotter.pnts[1].x * TerI->rotter.pnts[1].x) + (TerI->rotter.pnts[1].y * TerI->rotter.pnts[1].y) + (TerI->rotter.pnts[1].z * TerI->rotter.pnts[1].z));
    dist[6] = (f32) ((TerI->rotter.pnts[2].x * TerI->rotter.pnts[2].x) + (TerI->rotter.pnts[2].y * TerI->rotter.pnts[2].y) + (TerI->rotter.pnts[2].z * TerI->rotter.pnts[2].z));
    dist[7] = (f32) ((TerI->rotter.pnts[3].x * TerI->rotter.pnts[3].x) + (TerI->rotter.pnts[3].y * TerI->rotter.pnts[3].y) + (TerI->rotter.pnts[3].z * TerI->rotter.pnts[3].z));
}

//NGC MATCH
void DeRotatePoint(struct nuvec_s *pnt) {
  float sinax;
  float cosax;
  float sinay;
  float cosay;
  float tz;
  
  sinax = NuTrigTable[(s32)-TerI->ax & 0xffff];
  cosax = NuTrigTable[(s32)(-TerI->ax + 16384.0f) & 0xffff];
  sinay = NuTrigTable[(s32)-TerI->ay & 0xffff];
  cosay = NuTrigTable[(s32)(-TerI->ay + 16384.0f) & 0xffff];
  tz = (pnt->z - (TerI->curpos).z) * cosay - (pnt->x - (TerI->curpos).x) * sinay;
  pnt->x = (pnt->z - (TerI->curpos).z) * sinay +(pnt->x - (TerI->curpos).x) * cosay;
  pnt->z = ((pnt->y + TerI->size) - (TerI->curpos).y) * sinax + tz * cosax;
  pnt->y = ((pnt->y + TerI->size) - (TerI->curpos).y) * cosax - tz * sinax;
}

//NGC MATCH
short InsideLineF(float _x,float _z,float _x0,float _z0,float _x1,float _z1) {
    if ((_x - _x0) * (_z1 - _z0) + (_z - _z0) * (_x0 - _x1) < 0.0f) {
        return 0;
    } else {
        return 1;
    }
}

short InsidePolLines(float x, float y, float z, float x0, float y0, float z0, float x1, float y1, float z1, nuvec_s* norm)
{
	//TODO
}

//NGC MATCH
struct trackinfo_s* ScanTerrId(void *id) {
  s32 c;
  
  for(c = 0; c < 4; c++) {
    if (CurTerr->TrackInfo[c].ptrid == id) {
      return CurTerr->TrackInfo + c;
    }
  }
    return NULL;
}

//NGC MATCH
struct trackinfo_s* AllocTerrId(void) {
  s32 c;
  
  for(c = 0; c < 4; c++) {
    if (CurTerr->TrackInfo[c].ptrid == 0) {
      return CurTerr->TrackInfo + c;
    }
  }
    return NULL;
}

void NewScan(nuvec_s* ppos, int extramask, int platscan)


void NewScanRot(nuvec_s* ppos, int extramask)

int CheckCylinder(int p1,int p2,int *spherechecks,int mask)	//TODO
{
  float fVar1;
  float fVar2;
  int ret;
  float fVar3;
  float fVar4;
  float fVar5;
  float local_54;
  nuvec_s tmp;
  float local_28;
  float local_24;
  float local_20;
  nuvec_s D;
  
  if ((((((TerI->size < (TerI->rotter).pnts[p1].x) && (TerI->size < (TerI->rotter).pnts[p2].x)) ||
        (((TerI->rotter).pnts[p1].x < -TerI->size && ((TerI->rotter).pnts[p2].x < -TerI->size))))  ||
       ((TerI->size < (TerI->rotter).pnts[p1].y && (TerI->size < (TerI->rotter).pnts[p2].y)))) ||
      (((TerI->rotter).pnts[p1].y < -TerI->size && ((TerI->rotter).pnts[p2].y < -TerI->size)))) ||
     ((((TerI->rotter).pnts[p1].z < -TerI->size && ((TerI->rotter).pnts[p2].z < -TerI->size)) ||
      ((TerI->size + TerI->len < (TerI->rotter).pnts[p1].z &&
       (TerI->size + TerI->len < (TerI->rotter).pnts[p2].z)))))) {
    *spherechecks = *spherechecks & mask;
    ret = 0;
  }
  else {
    tmp.x = (TerI->rotter).pnts[p2].x - (TerI->rotter).pnts[p1].x;
    tmp.y = (TerI->rotter).pnts[p2].y - (TerI->rotter).pnts[p1].y;
    fVar3 = tmp.x * tmp.x + tmp.y * tmp.y;
    if (1e-12 <= fVar3) {
      fVar1 = (TerI->rotter).pnts[p1].x * tmp.y - (TerI->rotter).pnts[p1].y * tmp.x;
      if (fVar1 * fVar1 <= TerI->sizesq * fVar3) {
        tmp.z = (TerI->rotter).pnts[p2].z - (TerI->rotter).pnts[p1].z;
        fVar4 = NuFsqrt(tmp.x * tmp.x + tmp.y * tmp.y + tmp.z * tmp.z);
        fVar5 = 1.0 / fVar4;
        fVar2 = tmp.x * fVar5;
        tmp.y = tmp.y * fVar5;
        tmp.z = tmp.z * fVar5;
        fVar5 = -tmp.y;
        D.x = -(TerI->rotter).pnts[p1].x;
        D.y = -(TerI->rotter).pnts[p1].y;
        D.z = -(TerI->rotter).pnts[p1].z;
        tmp.x = fVar2;
        NuVecCross(&D,&D,&tmp);
        local_54 = -(D.x * fVar5 + D.y * fVar2) / (fVar5 * fVar5 + fVar2 * fVar2);
        local_28 = fVar2 * tmp.z;
        local_24 = -(fVar5 * tmp.z);
        local_20 = fVar5 * tmp.y - fVar2 * tmp.x;
        fVar5 = NuFsqrt(local_28 * local_28 + local_24 * local_24 + local_20 * local_20);
        local_20 = local_20 / fVar5;
        fVar3 = NuFsqrt(TerI->sizesq - (fVar1 * fVar1) / fVar3);
        fVar3 = fVar3 / local_20;
        if (0.0 <= fVar3) {
          local_54 = local_54 - fVar3;
        }
        else {
          local_54 = local_54 + fVar3;
        }
        if ((((local_54 < 0.0) || (TerI->len < local_54)) ||
            (fVar3 = -(TerI->rotter).pnts[p1].x * tmp.x + -(TerI->rotter).pnts[p1].y * tmp.y +
                     (local_54 - (TerI->rotter).pnts[p1].z) * tmp.z, fVar3 <= 0.0)) ||
           (fVar4 < fVar3)) {
          if (((-TerI->size <= (TerI->rotter).pnts[p1].z) ||
              (-TerI->size <= (TerI->rotter).pnts[p2].z)) &&
             (((TerI->rotter).pnts[p1].z <= TerI->size || ((TerI->rotter).pnts[p2].z <= TerI->size ))
             )) {
            fVar3 = (-tmp.x * (TerI->rotter).pnts[p1].x - tmp.y * (TerI->rotter).pnts[p1].y) -
                    tmp.z * (TerI->rotter).pnts[p1].z;
            if ((fVar3 < 0.0) ||
               (((TerI->rotter).pnts[p2].x - (TerI->rotter).pnts[p1].x) * tmp.x +
                ((TerI->rotter).pnts[p2].y - (TerI->rotter).pnts[p1].y) * tmp.y +
                ((TerI->rotter).pnts[p2].z - (TerI->rotter).pnts[p1].z) * tmp.z < fVar3)) {
              return 0;
            }
            tmp.x = tmp.x * fVar3 + (TerI->rotter).pnts[p1].x;
            tmp.y = tmp.y * fVar3 + (TerI->rotter).pnts[p1].y;
            tmp.z = tmp.z * fVar3 + (TerI->rotter).pnts[p1].z;
            fVar3 = tmp.x * tmp.x + tmp.y * tmp.y + tmp.z * tmp.z;
            if (fVar3 < TerI->sizesq) {
              fVar3 = NuFsqrt(fVar3);
              fVar3 = 1.0 / fVar3;
              TerI->hittype = 0x12;
              TerI->hittime = 0.0;
              (TerI->hitnorm).x = -tmp.x * fVar3;
              (TerI->hitnorm).y = -tmp.y * fVar3;
              (TerI->hitnorm).z = -tmp.z * fVar3;
              return 1;
            }
          }
          ret = 0;
        }
        else if (TerI->hittime <= local_54 / TerI->len) {
          *spherechecks = *spherechecks & mask;
          ret = 0;
        }
        else {
          TerI->hittype = 2;
          TerI->hittime = local_54 / TerI->len;
          (TerI->hitnorm).x = -(tmp.x * fVar3 + (TerI->rotter).pnts[p1].x);
          (TerI->hitnorm).y = -(tmp.y * fVar3 + (TerI->rotter).pnts[p1].y);
          (TerI->hitnorm).z = local_54 - (tmp.z * fVar3 + (TerI->rotter).pnts[p1].z);
          *spherechecks = *spherechecks & mask;
          ret = 1;
        }
      }
      else {
        *spherechecks = *spherechecks & mask;
        ret = 0;
      }
    }
    else if ((((0.0 < (TerI->rotter).pnts[p1].z) || ((TerI->rotter).pnts[p2].z < 0.0)) &&
             (((TerI->rotter).pnts[p1].z < 0.0 || (0.0 < (TerI->rotter).pnts[p2].z)))) ||
            (fVar3 = (TerI->rotter).pnts[p2].x * (TerI->rotter).pnts[p2].x +
                     (TerI->rotter).pnts[p2].y * (TerI->rotter).pnts[p2].y, TerI->sizesq < fVar3))  {
      ret = 0;
    }
    else {
      fVar3 = NuFsqrt(fVar3);
      TerI->hittype = 0x12;
      TerI->hittime = 0.0;
      (TerI->hitnorm).x = -(TerI->rotter).pnts[p2].x * (1.0 / fVar3);
      (TerI->hitnorm).y = -(TerI->rotter).pnts[p2].y * (1.0 / fVar3);
      (TerI->hitnorm).z = 0.0;
      ret = 1;
    }
  }
  return ret;
}

//NGC MATCH
s32 CheckSphere(s32 p) {
  float a;
  float b;
  
  if (((TerI->rotter).pnts[p].z < -TerI->size) || ((TerI->rotter).pnts[p].z > TerI->size + TerI->len)) {
    return 0;
  }
    b = (TerI->rotter).pnts[p].x * (TerI->rotter).pnts[p].x + (TerI->rotter).pnts[p].y * (TerI->rotter).pnts[p].y;
    if (b > TerI->sizesq) {
      return 0;
    }
    a = NuFsqrt(TerI->sizesq - b);
    if (((TerI->rotter).pnts[p].z - a < 0.0f) || ((TerI->rotter).pnts[p].z - a) > TerI->len) {
        b += (TerI->rotter).pnts[p].z * (TerI->rotter).pnts[p].z;
        if (b < TerI->sizesq) {
          a = 1.0f / NuFsqrt(b);
          TerI->hittype = 0x13;
          TerI->hittime = 0.0f;
          (TerI->hitnorm).x = -(TerI->rotter).pnts[p].x * a;
          (TerI->hitnorm).y = -(TerI->rotter).pnts[p].y * a;
          (TerI->hitnorm).z = -(TerI->rotter).pnts[p].z * a;
          return 1;
        }
        return 0;
    }
    b = ((TerI->rotter).pnts[p].z - a) / TerI->len;
    if (b < TerI->hittime) {
          TerI->hittype = 3;
          TerI->hittime = b;
          (TerI->hitnorm).x = -(TerI->rotter).pnts[p].x;
          (TerI->hitnorm).y = -(TerI->rotter).pnts[p].y;
          (TerI->hitnorm).z = -a;
          return 1;
    }
    return 0;
}

//NGC MATCH
s32 CheckSphereTer(struct nuvec_s *pnt,float radius) {
  float a;
  float b;
  float rsq;
  
  if ((pnt->z < -TerI->size - radius) || (pnt->z > TerI->size + radius + TerI->len)) {
    return 0;
  }

    b = pnt->x * pnt->x + pnt->y * pnt->y;
    rsq = (TerI->size + radius) * (TerI->size + radius);
    if (b > rsq) {
      return 0;
    }
    a = NuFsqrt(rsq - b);
    if ((pnt->z - a < 0.0f) || (pnt->z - a > TerI->len)) {
        b += pnt->z * pnt->z;
        if (b < rsq) {
          a = 1.0f / NuFsqrt(b);
          TerI->hittype = 0x14;
          TerI->hittime = 0.0f;
          (TerI->hitnorm).x = -pnt->x * a;
          (TerI->hitnorm).y = -pnt->y * a;
          (TerI->hitnorm).z = -pnt->z * a;
          return 1;
        }
          return 0;
    }

    b = (pnt->z - a) / TerI->len;
    if (b < TerI->hittime) {
    TerI->hittype = 4;
    TerI->hittime = b;
    (TerI->hitnorm).x = -pnt->x;
    (TerI->hitnorm).y = -pnt->y;
    (TerI->hitnorm).z = -a;
    return 1;
    }
    return 0;
}


uint HitPoly(float ps,float pe,float ps2,float pe2,hitdata *ter,nuvec_s *hitpos)

{
  short sVar1;
  uint checkC1;
  uint checkC2;
  uint check;
  nuvec_s pos;
  float time;
  uint ret;
  int spherechecks;
  bool b;
  
  ret = 0;
  pos.x = (TerI->csx - TerI->size * ter->norm[0].x) - ter->pnts[0].x;
  pos.y = (TerI->csy - TerI->size * ter->norm[0].y) - ter->pnts[0].y;
  pos.z = (TerI->csz - TerI->size * ter->norm[0].z) - ter->pnts[0].z;
  b = false;
  if ((ps <= 0.0) || (0.0 <= pe)) {
    if ((ps < 0.0) && (pe < 0.0)) {
      time = 1e-09;
      pos.x = pos.x + -ps * ter->norm[0].x;
      pos.y = pos.y + -ps * ter->norm[0].y;
      pos.z = pos.z + -ps * ter->norm[0].z;
      b = true;
    }
  }
  else {
    time = ps / (ps - pe);
    pos.x = pos.x + (TerI->curvel).x * time;
    pos.y = pos.y + (TerI->curvel).y * time;
    pos.z = pos.z + (TerI->curvel).z * time;
    b = true;
  }
  if (b) {
    sVar1 = InsidePolLines(pos.x,pos.y,pos.z,ter->pnts[1].x - ter->pnts[0].x,
                           ter->pnts[1].y - ter->pnts[0].y,ter->pnts[1].z - ter->pnts[0].z,
                           ter->pnts[2].x - ter->pnts[0].x,ter->pnts[2].y - ter->pnts[0].y,
                           (float)ter->norm,hitpos);
    if ((sVar1 != 0) && (time <= TerI->hittime)) {
      if (ps <= 0.0) {
        TerI->hittype = 0x11;
      }
      else {
        TerI->hittype = 1;
      }
      TerI->hittime = time;
      (TerI->hitnorm).x = ter->norm[0].x;
      (TerI->hitnorm).y = ter->norm[0].y;
      (TerI->hitnorm).z = ter->norm[0].z;
      ret = 1;
    }
  }
  if (ter->norm[1].y < 65536.0) {
    pos.x = (TerI->csx - TerI->size * ter->norm[1].x) - ter->pnts[3].x;
    pos.y = (TerI->csy - TerI->size * ter->norm[1].y) - ter->pnts[3].y;
    pos.z = (TerI->csz - TerI->size * ter->norm[1].z) - ter->pnts[3].z;
    b = false;
    if ((ps2 <= 0.0) || (0.0 <= pe2)) {
      if ((ps2 < 0.0) && (pe2 < 0.0)) {
        time = 1e-09;
        pos.x = pos.x + -ps2 * ter->norm[1].x;
        pos.y = pos.y + -ps2 * ter->norm[1].y;
        pos.z = pos.z + -ps2 * ter->norm[1].z;
        b = true;
      }
    }
    else {
      time = ps2 / (ps2 - pe2);
      pos.x = pos.x + (TerI->curvel).x * time;
      pos.y = pos.y + (TerI->curvel).y * time;
      pos.z = pos.z + (TerI->curvel).z * time;
      b = true;
    }
    if (b) {
      sVar1 = InsidePolLines(pos.x,pos.y,pos.z,ter->pnts[2].x - ter->pnts[3].x,
                             ter->pnts[2].y - ter->pnts[3].y,ter->pnts[2].z - ter->pnts[3].z,
                             ter->pnts[1].x - ter->pnts[3].x,ter->pnts[1].y - ter->pnts[3].y,
                             (float)(ter->norm + 1),hitpos);
      if ((sVar1 != 0) && (time < TerI->hittime)) {
        if (ps2 <= 0.0) {
          TerI->hittype = 0x11;
          TerI->hittime = time;
          (TerI->hitnorm).x = ter->norm[1].x;
          (TerI->hitnorm).y = ter->norm[1].y;
          (TerI->hitnorm).z = ter->norm[1].z;
        }
        else {
          TerI->hittype = 1;
          TerI->hittime = time;
          (TerI->hitnorm).x = ter->norm[1].x;
          (TerI->hitnorm).y = ter->norm[1].y;
          (TerI->hitnorm).z = ter->norm[1].z;
        }
        ret = 1;
      }
    }
  }
  if (TerI->size != 0.0) {
    spherechecks = 0xf;
    DeRotateTerrain(ter);
    check = CheckCylinder(0,1,&spherechecks,0xc);
    checkC1 = CheckCylinder(1,2,&spherechecks,9);
    checkC2 = CheckCylinder(2,0,&spherechecks,10);
    ret = ret | check | checkC1 | checkC2;
    if (ter->norm[1].y < 65536.0) {
      check = CheckCylinder(1,3,&spherechecks,5);
      checkC1 = CheckCylinder(3,2,&spherechecks,3);
      ret = ret | check | checkC1;
      if ((spherechecks & 8U) != 0) {
        check = CheckSphere(3);
        ret = ret | check;
      }
    }
    if ((spherechecks & 1U) != 0) {
      check = CheckSphere(0);
      ret = ret | check;
    }
    if ((spherechecks & 2U) != 0) {
      check = CheckSphere(1);
      ret = ret | check;
    }
    if ((spherechecks & 4U) != 0) {
      check = CheckSphere(2);
      ret = ret | check;
    }
  }
  if (ret != 0) {
    TerI->hitter = ter;
  }
  return ret;
}

int HitWallSpline(void)		//CHECK

{
  float fVar1;
  int lp;
  double dVar2;
  int i;
  nuvec_s tnorm;
  nuvec_s tnorm2;
  nuvec_s tpos;
  float time2;
  float hit;
  float time;
  float size;
  float size2;
  float vellen;
  float vellen2;
  
  tnorm.y = 0.0;
  if (WallSplCount == 0) {
    lp = 0;
  }
  else {
    hit = 0.0;
    for (i = 0; i < WallSplCount; i = i + 2) {
      size = (TerI->curpos).x - WallSplList[i].x;
      dVar2 = (double)size;
      NuFabs(size);
      if (dVar2 < 64.0) {
        size = (TerI->curpos).z - WallSplList[i].z;
        dVar2 = (double)size;
        NuFabs(size);
        if (dVar2 < 64.0) {
          tnorm.x = WallSplList[i + 1].z - WallSplList[i].z;
          tnorm.z = WallSplList[i].x - WallSplList[i + 1].x;
          NuVecNorm(&tnorm,&tnorm);
          size = ((((TerI->curpos).x + (TerI->curvel).x) - WallSplList[i].x) * tnorm.x +
                 (((TerI->curpos).z + (TerI->curvel).z) - WallSplList[i].z) * tnorm.z) - TerI->siz e;
          vellen = (((TerI->curpos).x - WallSplList[i].x) * tnorm.x +
                   ((TerI->curpos).z - WallSplList[i].z) * tnorm.z) - TerI->size;
          if (((size < 0.0) || (vellen < 0.0)) && (-TerI->size < vellen)) {
            tnorm2.x = WallSplList[i + 1].x - WallSplList[i].x;
            tnorm2.z = WallSplList[i + 1].z - WallSplList[i].z;
            if (0.0 <= vellen) {
              time = vellen / (vellen - size);
              tpos.x = ((TerI->curvel).x * time + (TerI->curpos).x) - TerI->size * tnorm.x;
              tpos.z = ((TerI->curvel).z * time + (TerI->curpos).z) - TerI->size * tnorm.z;
            }
            else {
              time = 1e-09;
              tpos.x = ((TerI->curpos).x - tnorm.x * vellen) - TerI->size * tnorm.x;
              tpos.z = ((TerI->curpos).z - tnorm.z * vellen) - TerI->size * tnorm.z;
            }
            if (((-5e-05 <= (tpos.x - WallSplList[i].x) * tnorm2.x +
                            (tpos.z - WallSplList[i].z) * tnorm2.z) &&
                (-5e-05 <= -tnorm2.x * (tpos.x - WallSplList[i + 1].x) -
                           (tpos.z - WallSplList[i + 1].z) * tnorm2.z)) && (time < TerI->hittime))  {
              if (vellen <= 0.0) {
                TerI->hittype = 0x11;
              }
              else {
                TerI->hittype = 1;
              }
              TerI->hittime = time;
              (TerI->hitnorm).x = tnorm.x;
              (TerI->hitnorm).y = tnorm.y;
              (TerI->hitnorm).z = tnorm.z;
              TerI->hitterrno = -1;
              hit = 1.0;
            }
          }
          size = TerI->size;
          vellen = TerI->vellen;
          size2 = TerI->size;
          vellen2 = TerI->vellen;
          fVar1 = WallSplList[i].x - (TerI->curpos).x;
          dVar2 = (double)fVar1;
          NuFabs(fVar1);
          if (dVar2 < 64.0) {
            fVar1 = WallSplList[i].z - (TerI->curpos).z;
            dVar2 = (double)fVar1;
            NuFabs(fVar1);
            if ((dVar2 < 64.0) &&
               ((WallSplList[i].x - (TerI->curpos).x) * (WallSplList[i].x - (TerI->curpos).x) +
                (WallSplList[i].z - (TerI->curpos).z) * (WallSplList[i].z - (TerI->curpos).z) <
                (size + 0.005 + vellen) * (size2 + 0.005 + vellen2))) {
              tnorm2.x = (TerI->curvel).x;
              tnorm2.y = 0.0;
              tnorm2.z = (TerI->curvel).z;
              NuVecNorm(&tnorm2,&tnorm2);
              size = (WallSplList[i].x - (TerI->curpos).x) * tnorm2.x +
                     (WallSplList[i].z - (TerI->curpos).z) * tnorm2.z;
              if (size < 0.0) {
                if ((WallSplList[i].x - (TerI->curpos).x) * (WallSplList[i].x - (TerI->curpos).x)  +
                    (WallSplList[i].z - (TerI->curpos).z) * (WallSplList[i].z - (TerI->curpos).z)  <=
                    (TerI->size + 0.0005) * (TerI->size + 0.0005)) {
                  tnorm.x = (TerI->curpos).x - WallSplList[i].x;
                  tnorm.z = (TerI->curpos).z - WallSplList[i].z;
                  NuVecNorm(&tnorm,&tnorm);
                  TerI->hittype = 0x11;
                  TerI->hittime = 0.0;
                  (TerI->hitnorm).x = tnorm.x;
                  (TerI->hitnorm).y = tnorm.y;
                  (TerI->hitnorm).z = tnorm.z;
                  hit = 1.0;
                }
              }
              else {
                tpos.x = (TerI->curpos).x + tnorm2.x * size;
                tpos.z = (TerI->curpos).z + tnorm2.z * size;
                if ((TerI->size + 0.0005) * (TerI->size + 0.0005) <
                    (WallSplList[i].x - tpos.x) * (WallSplList[i].x - tpos.x) +
                    (WallSplList[i].z - tpos.z) * (WallSplList[i].z - tpos.z)) {
                  if ((WallSplList[i].x - (TerI->curpos).x) * (WallSplList[i].x - (TerI->curpos).x )
                      + (WallSplList[i].z - (TerI->curpos).z) *
                        (WallSplList[i].z - (TerI->curpos).z) <=
                      (TerI->size + 0.0005) * (TerI->size + 0.0005)) {
                    tnorm.x = (TerI->curpos).x - WallSplList[i].x;
                    tnorm.z = (TerI->curpos).z - WallSplList[i].z;
                    NuVecNorm(&tnorm,&tnorm);
                    TerI->hittype = 0x11;
                    TerI->hittime = 0.0;
                    (TerI->hitnorm).x = tnorm.x;
                    (TerI->hitnorm).y = tnorm.y;
                    (TerI->hitnorm).z = tnorm.z;
                    hit = 1.0;
                  }
                }
                else {
                  vellen = NuFsqrt((TerI->size * TerI->size -
                                   (WallSplList[i].x - tpos.x) * (WallSplList[i].x - tpos.x)) -
                                   (WallSplList[i].z - tpos.z) * (WallSplList[i].z - tpos.z));
                  size = size - vellen;
                  if ((size < -0.0005) ||
                     ((TerI->curvel).x * (TerI->curvel).x + (TerI->curvel).z * (TerI->curvel).z <
                      size * size)) {
                    if ((WallSplList[i].x - (TerI->curpos).x) *
                        (WallSplList[i].x - (TerI->curpos).x) +
                        (WallSplList[i].z - (TerI->curpos).z) *
                        (WallSplList[i].z - (TerI->curpos).z) <=
                        (TerI->size + 0.0005) * (TerI->size + 0.0005)) {
                      tnorm.x = (TerI->curpos).x - WallSplList[i].x;
                      tnorm.z = (TerI->curpos).z - WallSplList[i].z;
                      NuVecNorm(&tnorm,&tnorm);
                      TerI->hittype = 0x11;
                      TerI->hittime = 0.0;
                      (TerI->hitnorm).x = tnorm.x;
                      (TerI->hitnorm).y = tnorm.y;
                      (TerI->hitnorm).z = tnorm.z;
                      hit = 1.0;
                    }
                  }
                  else {
                    time2 = NuFsqrt((TerI->curvel).x * (TerI->curvel).x +
                                    (TerI->curvel).z * (TerI->curvel).z);
                    time2 = size / time2;
                    if (time2 < 0.0) {
                      time2 = 0.0;
                    }
                    if (time2 < TerI->hittime) {
                      tnorm.x = (tnorm2.x * size + (TerI->curpos).x) - WallSplList[i].x;
                      tnorm.z = (tnorm2.z * size + (TerI->curpos).z) - WallSplList[i].z;
                      NuVecNorm(&tnorm,&tnorm);
                      TerI->hittype = 1;
                      TerI->hittime = time2;
                      (TerI->hitnorm).x = tnorm.x;
                      (TerI->hitnorm).y = tnorm.y;
                      (TerI->hitnorm).z = tnorm.z;
                      TerI->hitterrno = -1;
                      hit = 1.0;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    lp = (int)hit;
  }
  return lp;
}



int HitTerrain(nuvec_s *v)		//CHECK

{
  hitdata *ptr;
  uint check;
  nuvec_s *extraout_r4;
  nuvec_s *hitpos;
  float fVar1;
  hitdata **terr;
  hitdata **currter;
  nuvec_s pos;
  float ps;
  float pe;
  float ps2;
  float pe2;
  float size;
  int i;
  int hit;
  int lp;
  
  terr = TerI->hitdat;
  size = TerI->size;
  lp = 0;
  TerI->hittype = 0;
  TerI->hittime = 999.9;
  HitWallSpline();
  hitpos = extraout_r4;
  fVar1 = NuFsqrt((TerI->curvel).x * (TerI->curvel).x + (TerI->curvel).z * (TerI->curvel).z);
  TerI->vellen = fVar1;
  while( true ) {
    while (0 < *(short *)terr) {
      TerI->csx = (TerI->curpos).x - CurTerr->terr[*(short *)((int)terr + 2)].Location.x;
      TerI->csy = (TerI->curpos).y - CurTerr->terr[*(short *)((int)terr + 2)].Location.y;
      TerI->csz = (TerI->curpos).z - CurTerr->terr[*(short *)((int)terr + 2)].Location.z;
      TerI->cex = ((TerI->curpos).x + (TerI->curvel).x) -
                  CurTerr->terr[*(short *)((int)terr + 2)].Location.x;
      TerI->cey = ((TerI->curpos).y + (TerI->curvel).y) -
                  CurTerr->terr[*(short *)((int)terr + 2)].Location.y;
      TerI->cez = ((TerI->curpos).z + (TerI->curvel).z) -
                  CurTerr->terr[*(short *)((int)terr + 2)].Location.z;
      currter = terr;
      for (i = (int)*(short *)terr; currter = currter + 1, 0 < i; i = i + -1) {
        ptr = *currter;
        hit = 0;
        pe = (((TerI->cex - ptr->pnts[0].x) * ptr->norm[0].x +
               (TerI->cey - ptr->pnts[0].y) * ptr->norm[0].y +
              (TerI->cez - ptr->pnts[0].z) * ptr->norm[0].z) - size) - TerI->impactadj;
        if ((pe < 0.0) &&
           (ps = ((TerI->csx - ptr->pnts[0].x) * ptr->norm[0].x +
                  (TerI->csy - ptr->pnts[0].y) * ptr->norm[0].y +
                 (TerI->csz - ptr->pnts[0].z) * ptr->norm[0].z) - size, -size < ps)) {
          hit = 1;
        }
        if (((ptr->norm[1].y < 65536.0) &&
            (pe2 = (((TerI->cex - ptr->pnts[3].x) * ptr->norm[1].x +
                     (TerI->cey - ptr->pnts[3].y) * ptr->norm[1].y +
                    (TerI->cez - ptr->pnts[3].z) * ptr->norm[1].z) - size) - TerI->impactadj,
            pe2 < 0.0)) &&
           (ps2 = ((TerI->csx - ptr->pnts[3].x) * ptr->norm[1].x +
                   (TerI->csy - ptr->pnts[3].y) * ptr->norm[1].y +
                  (TerI->csz - ptr->pnts[3].z) * ptr->norm[1].z) - size, -size < ps2)) {
          hit = 1;
        }
        if ((hit != 0) && (check = HitPoly(ps,pe,ps2,pe2,ptr,hitpos), check != 0)) {
          TerI->hitterrno = *(short *)((int)terr + 2);
          lp = 1;
          if ((TerI->hitter->info[2] & 0x80) != 0) {
            PlatCrush = TerI->hitter->info[0] + 1;
          }
        }
      }
      terr = currter;
    }
    if (-1 < *(short *)terr) break;
    for (; *(short *)terr < 0; terr = terr + ((check ^ (int)*(short *)terr) - check) + 1) {
      check = (int)*(short *)terr >> 0x1f;
    }
  }
  for (i = 0; i < curSphereter; i = i + 1) {
    pos.x = SphereData[i].pos.x;
    pos.y = SphereData[i].pos.y;
    pos.z = SphereData[i].pos.z;
    DeRotatePoint(&pos);
    check = CheckSphereTer(&pos,SphereData[i].radius);
    lp = lp | check;
  }
  if (((TerI->hittype != 0) && (TerI->hitterrno != -1)) &&
     (CurTerr->terr[TerI->hitterrno].type == TERR_TYPE_PLATFORM)) {
    plathitid = (int)CurTerr->terr[TerI->hitterrno].info;
  }
  return lp;
}

//NGC MATCH
void ScanTerrain(s32 platscan, s32 extramask) {
    s32 a; // 0x10(r31)
    s32 b; // 0x14(r31)
    s32 c; // 0x18(r31)
    s32 HitCnt; // 0x1C(r31)
    // Size: 0x64, DWARF: 0x776DF4 
    struct scaleterrain_s** HitData; // 0x20(r31)
    // Size: 0x64, DWARF: 0x776DF4
    struct scaleterrain_s** MaxData; // 0x24(r31)
    short* LastWrite; // 0x28(r31)
    short* ttemp; // 0x2C(r31)
    struct wallspl_s* WallSpl; // 0x30(r31)
    float maxx; // 0x34(r31)
    float maxy; // 0x38(r31)
    float maxz; // 0x3C(r31)
    float tmaxx; // 0x40(r31)
    float tmaxy; // 0x44(r31)
    float tmaxz; // 0x48(r31)
    float radmov; // 0x4C(r31)
    float minx; // 0x50(r31)
    float miny; // 0x54(r31)
    float minz; // 0x58(r31)
    float tminx; // 0x5C(r31)
    float tminy; // 0x60(r31)
    float tminz; // 0x64(r31)
    float tn; // 0x68(r31)
    struct terr_s* mbuf2; // 0x6C(r31)
    struct scaleterrain_s* ter; // 0x70(r31)
    short* modp; // 0x74(r31)
    s32 curscltemp; // 0x78(r31)
    struct nuvec4_s pnts[4]; // 0x80(r31)
    struct nuvec4_s norm[2]; // 0xC0(r31)
    
    curscltemp = 0;
    ScaleTerrain = ScaleTerrainT1;
    TerI->hitcnt = 0;
    LastWrite = TerI->hitdata;
    HitData = TerI->hitdata + 1;
    MaxData = &HitData[508]; // ???
    HitCnt = 0;
    platinrange = 0;
    TerI->plathit = -1;
    switch (TerI->scanmode) {
        default:
            radmov = NuFsqrt(POW2(TerI->curvel.x) + POW2(TerI->curvel.y) + POW2(TerI->curvel.z));
            radmov += TerI->size + 0.02f;
            maxx = TerI->curpos.x + radmov;
            maxy = TerI->curpos.y + (radmov * TerI->yscale);
            maxz = TerI->curpos.z + radmov;
            minx = TerI->curpos.x - radmov;
            miny = TerI->curpos.y - (radmov * TerI->yscale);
            minz = TerI->curpos.z - radmov;
            radmov *= radmov;
        break;
        case 1:
            if (TerI->curvel.x > 0.0f) {
                minx = (TerI->curpos.x - 0.02f) - TerI->size;
                maxx = TerI->curpos.x + TerI->curvel.x + 0.02f + TerI->size;
            }
            else {
                minx = (((TerI->curpos).x + (TerI->curvel).x) - 0.02f) - TerI->size;
                maxx = (TerI->curpos).x + 0.02f + TerI->size;
            }
            
            if ((TerI->curvel).y > 0.0f) {
                miny = ((TerI->curpos).y - 0.02f) - TerI->size;
                maxy = (TerI->curpos).y + (TerI->curvel).y + 0.02f + TerI->size;
            }
            else {
                miny = (((TerI->curpos).y + (TerI->curvel).y) - 0.02f) - TerI->size;
                maxy = (TerI->curpos).y + 0.02f + TerI->size;
            }
            
            if ((TerI->curvel).z > 0.0f) {
                minz = ((TerI->curpos).z - 0.02f) - TerI->size;
                maxz = (TerI->curpos).z + (TerI->curvel).z + 0.02f + TerI->size;
            }
            else {
                minz = (((TerI->curpos).z + (TerI->curvel).z) - 0.02f) - TerI->size;
                maxz = (TerI->curpos).z + 0.02f + TerI->size;
            }
            
            radmov = POW2(minx - maxx) + POW2(miny - maxy) + POW2(minz - maxz);
            radmov += 0.02f;
        break;
    }
    
    for (a = 0; a < CurTerr->terrgcnt; a++) {
        if (
            (maxx >= CurTerr->terrgroup[a].minx) 
            && (maxz >= CurTerr->terrgroup[a].minz)
            && (minx <= CurTerr->terrgroup[a].maxx)
            && (minz <= CurTerr->terrgroup[a].maxz)
        ) {
            ttemp = CurTerr->terrlist + CurTerr->terrgroup[a].tabindex;
            for (b = 0; b < CurTerr->terrgroup[a].count; b++, ttemp++) {
                mbuf2 = CurTerr->terr + *ttemp;
                if (
                    (maxx >= mbuf2->min.x) 
                    && (maxy >= mbuf2->min.y) 
                    && (maxz >= mbuf2->min.z)
                    && (minx <= mbuf2->max.x)
                    && (miny < mbuf2->max.y)
                    && (minz < mbuf2->max.z) 
                    && (mbuf2->type != ~TERR_TYPE_NORMAL)
                ) {
                    tmaxx = maxx - mbuf2->Location.x;
                    tmaxy = maxy - mbuf2->Location.y;
                    tmaxz = maxz - mbuf2->Location.z;
                    
                    tminx = minx - mbuf2->Location.x;
                    tminy = miny - mbuf2->Location.y;
                    tminz = minz - mbuf2->Location.z;

                    modp = (struct scaleterrain_s *)mbuf2->model;
                    while (*modp >= 0) {
                        c = modp[1];
                        ter = modp + 10;
                        if (
                            (tmaxx >= *(float*)&modp[2]) 
                            && (tminx < *(float*)&modp[4])
                            ){
                                if (
                                    (tmaxz >= *(float*)&modp[6])
                                    && (tminz < *(float*)&modp[8])
                                ) {
                                    for (; c > 0; c--) {
                                        if (
                                            (tmaxx >= ter->minx)
                                            && (tminx < ter->maxx)
                                            && (tmaxz >= ter->minz)
                                            && (tminz < ter->maxz)
                                            && (tmaxy >= ter->miny)
                                            && (tminy < ter->maxy)
                                            && (HitData < MaxData) 
                                            && ((ter->info[1] == 0) || ((ter->info[1] & extramask) != 0))
                                        ) {
                                            if (TerI->yscale == 1.0f) {
                                                *HitData = ter;
                                                HitData++;
                                                HitCnt++;
                                            }
                                            else {
                                                ScaleTerrain[curscltemp].info[0] = ter->info[0];
                                                ScaleTerrain[curscltemp].info[1] = ter->info[1];
                                                ScaleTerrain[curscltemp].info[2] = ter->info[2];
                                                ScaleTerrain[curscltemp].info[3] = ter->info[3];
                                                ScaleTerrain[curscltemp].pnts[0].x = ter->pnts[0].x;
                                                ScaleTerrain[curscltemp].pnts[0].z = ter->pnts[0].z;
                                                ScaleTerrain[curscltemp].pnts[0].y =
                                                 (ter->pnts[0].y + mbuf2->Location.y) * TerI->inyscale - mbuf2->Location.y;
                                                ScaleTerrain[curscltemp].pnts[1].x = ter->pnts[1].x;
                                                ScaleTerrain[curscltemp].pnts[1].z = ter->pnts[1].z;
                                                ScaleTerrain[curscltemp].pnts[1].y =
                                                 (ter->pnts[1].y + mbuf2->Location.y) * TerI->inyscale - mbuf2->Location.y;
                                                ScaleTerrain[curscltemp].pnts[2].x = ter->pnts[2].x;
                                                ScaleTerrain[curscltemp].pnts[2].z = ter->pnts[2].z;
                                                ScaleTerrain[curscltemp].pnts[2].y =
                                                 (ter->pnts[2].y + mbuf2->Location.y) * TerI->inyscale - mbuf2->Location.y;
                                                if (65535.0f > ter->norm[1].y) {
                                                    ScaleTerrain[curscltemp].pnts[3].x = ter->pnts[3].x;
                                                    ScaleTerrain[curscltemp].pnts[3].z = ter->pnts[3].z;
                                                    ScaleTerrain[curscltemp].pnts[3].y =
                                                       (ter->pnts[3].y + mbuf2->Location.y) * TerI->inyscale - mbuf2->Location.y;
                                                    tn =  1.0f / NuFsqrt(ter->norm[1].x * ter->norm[1].x +
                                                                   ter->norm[1].y * ter->norm[1].y * TerI->yscalesq +
                                                                   ter->norm[1].z * ter->norm[1].z);
                                                    ScaleTerrain[curscltemp].norm[1].x = ter->norm[1].x * tn;
                                                    ScaleTerrain[curscltemp].norm[1].y = ter->norm[1].y * TerI->yscale * tn;
                                                    ScaleTerrain[curscltemp].norm[1].z = ter->norm[1].z * tn;
                                                }
                                                else {
                                                    ScaleTerrain[curscltemp].norm[1].y = 65536.0f;
                                                }
                                                tn = 1.0f / NuFsqrt(ter->norm[0].x * ter->norm[0].x +
                                                             ter->norm[0].y * ter->norm[0].y * TerI->yscalesq +
                                                             ter->norm[0].z * ter->norm[0].z);
                                                
                                                ScaleTerrain[curscltemp].norm[0].x = ter->norm[0].x * tn;
                                                ScaleTerrain[curscltemp].norm[0].y = ter->norm[0].y * TerI->yscale * tn;
                                                ScaleTerrain[curscltemp].norm[0].z = ter->norm[0].z * tn;
                                                *HitData = ScaleTerrain + curscltemp;
                                                HitData++;
                                                HitCnt++;
                                                curscltemp++;
                                            }
                                        }
                                        ter++;
                                    }
                                modp = ter;
                            } else {
                                modp = (struct scaleterrain_s *)ter + c;
                            }
                        } else {
                            modp = (struct scaleterrain_s *)ter + c;
                        }
                    }
                    if (HitCnt != 0) {
                        LastWrite[0] = HitCnt;
                        LastWrite[1] = *ttemp;
                        LastWrite = HitData;
                        HitData++;
                        HitCnt = 0;
                    }
                } 
            }
        }
    }
    
    if (curPickInst != 0) {
        for (a = 0; a < curPickInst; a++) {
            mbuf2 = &CurTerr->terr[a+2048];
            
            tmaxx = maxx - mbuf2->Location.x;
            tmaxy = maxy - mbuf2->Location.y;
            tmaxz = maxz - mbuf2->Location.z;
            
            tminx = minx - mbuf2->Location.x;
            tminy = miny - mbuf2->Location.y;
            tminz = minz - mbuf2->Location.z;
            
            if (
                (tmaxx >= mbuf2->min.x)
                && (tmaxy >= mbuf2->min.y)
                && (tmaxz >= mbuf2->min.z)
                && (tminx <= mbuf2->max.x)
                && (tminy < mbuf2->max.y)
                && (tminz < mbuf2->max.z)
                && (mbuf2->type != ~TERR_TYPE_NORMAL)
            ) {
                modp = (struct scaleterrain_s *)mbuf2->model;
                while (*modp >= 0) {
                    c = modp[1];
                    ter = modp + 10;
                    if (
                        (tmaxx >= *(float*)&modp[2]) 
                        && (tminx < *(float*)&modp[4])
                    ) {
                        if (
                            (tmaxz >= *(float*)&modp[6])
                            && (tminz < *(float*)&modp[8])
                        ) {
                            for (; c > 0; c--) {
                                if (
                                    (tmaxx >= ter->minx)
                                    && (tminx < ter->maxx)
                                    && (tmaxz >= ter->minz)
                                    && (tminz < ter->maxz)
                                    && (tmaxy >= ter->miny)
                                    && (tminy < ter->maxy)
                                    && (HitData < MaxData) 
                                    && ((ter->info[1] == 0) || ((ter->info[1] & extramask) != 0))
                                ) {
                                    if (TerI->yscale == 1.0f) {
                                        *HitData = ter;
                                        HitData++;
                                        HitCnt++;
                                    }
                                    else {
                                        ScaleTerrain[curscltemp].info[0] = ter->info[0];
                                        ScaleTerrain[curscltemp].info[1] = ter->info[1];
                                        ScaleTerrain[curscltemp].info[2] = ter->info[2];
                                        ScaleTerrain[curscltemp].info[3] = ter->info[3];
                                        ScaleTerrain[curscltemp].pnts[0].x = ter->pnts[0].x;
                                        ScaleTerrain[curscltemp].pnts[0].z = ter->pnts[0].z;
                                        ScaleTerrain[curscltemp].pnts[0].y =
                                         (ter->pnts[0].y + mbuf2->Location.y) * TerI->inyscale - mbuf2->Location.y;
                                        ScaleTerrain[curscltemp].pnts[1].x = ter->pnts[1].x;
                                        ScaleTerrain[curscltemp].pnts[1].z = ter->pnts[1].z;
                                        ScaleTerrain[curscltemp].pnts[1].y =
                                         (ter->pnts[1].y + mbuf2->Location.y) * TerI->inyscale - mbuf2->Location.y;
                                        ScaleTerrain[curscltemp].pnts[2].x = ter->pnts[2].x;
                                        ScaleTerrain[curscltemp].pnts[2].z = ter->pnts[2].z;
                                        ScaleTerrain[curscltemp].pnts[2].y =
                                         (ter->pnts[2].y + mbuf2->Location.y) * TerI->inyscale - mbuf2->Location.y;
                                        if (65535.0f > ter->norm[1].y) {
                                            ScaleTerrain[curscltemp].pnts[3].x = ter->pnts[3].x;
                                            ScaleTerrain[curscltemp].pnts[3].z = ter->pnts[3].z;
                                            ScaleTerrain[curscltemp].pnts[3].y =
                                               (ter->pnts[3].y + mbuf2->Location.y) * TerI->inyscale - mbuf2->Location.y;
                                            tn =  1.0f / NuFsqrt(POW2(ter->norm[1].x) + POW2(ter->norm[1].y) * TerI->yscalesq + POW2(ter->norm[1].z));
                                            ScaleTerrain[curscltemp].norm[1].x = ter->norm[1].x * tn;
                                            ScaleTerrain[curscltemp].norm[1].y = ter->norm[1].y * TerI->yscale * tn;
                                            ScaleTerrain[curscltemp].norm[1].z = ter->norm[1].z * tn;
                                        }
                                        else {
                                            ScaleTerrain[curscltemp].norm[1].y = 65536.0f;
                                        }
                                        tn = 1.0f / NuFsqrt(POW2(ter->norm[0].x) + POW2(ter->norm[0].y) * TerI->yscalesq + POW2(ter->norm[0].z));
                                        
                                        ScaleTerrain[curscltemp].norm[0].x = ter->norm[0].x * tn;
                                        ScaleTerrain[curscltemp].norm[0].y = ter->norm[0].y * TerI->yscale * tn;
                                        ScaleTerrain[curscltemp].norm[0].z = ter->norm[0].z * tn;
                                        *HitData = ScaleTerrain + curscltemp;
                                        HitData++;
                                        HitCnt++;
                                        curscltemp++;
                                    }
                                }
                                ter++;
                            }
                            modp = ter;
                        }
                        else {
                            modp = (struct scaleterrain_s *)ter + c;
                        }
                    }
                    else {
                        modp = (struct scaleterrain_s *)ter + c;
                    }
                }
                
                if (HitCnt != 0) {
                    LastWrite[0] = HitCnt;
                    LastWrite[1] = a + 0x800;
                    LastWrite = HitData;
                    HitData++;
                    HitCnt = 0;
                }
            }
        }
    }
    
    TerI->PlatScanStart = LastWrite;
    a = 0x100;
    minx -= 0.05f;
    maxx += 0.05f;
    miny -= 0.05f;
    maxy += 0.05f;
    minz -= 0.05f;
    maxz += 0.05f;
    if (platscan != 0) {
        ttemp = CurTerr->terrlist + CurTerr->terrgroup[a].tabindex;
        for (b = 0; b < CurTerr->terrgroup[a].count; b++, ttemp++) {
            mbuf2 = CurTerr->terr + *ttemp;
            if (CurTerr->platdata[mbuf2->info].curmtx != NULL) {
                if (CurTerr->platdata[mbuf2->info].curmtx->_30 > CurTerr->platdata[mbuf2->info].oldmtx._30) {
                    tminx = minx - mbuf2->Location.x;
                    tmaxx = ((CurTerr->platdata[mbuf2->info].curmtx->_30 -
                    CurTerr->platdata[mbuf2->info].oldmtx._30) * 1.5f + maxx) - mbuf2->Location.x;
                }
                else {
                    tmaxx = maxx - mbuf2->Location.x;
                    tminx = ((CurTerr->platdata[mbuf2->info].curmtx->_30 -
                    CurTerr->platdata[mbuf2->info].oldmtx._30) * 1.5f + minx) - mbuf2->Location.x;
                }
                if (CurTerr->platdata[mbuf2->info].curmtx->_31 > CurTerr->platdata[mbuf2->info].oldmtx._31) {
                    tminy = miny - mbuf2->Location.y;
                    tmaxy = ((CurTerr->platdata[mbuf2->info].curmtx->_31 -
                    CurTerr->platdata[mbuf2->info].oldmtx._31) * 1.5f + maxy) - mbuf2->Location.y;
                }
                else {
                    tmaxy = maxy - mbuf2->Location.y;
                    tminy = ((CurTerr->platdata[mbuf2->info].curmtx->_31 -
                    CurTerr->platdata[mbuf2->info].oldmtx._31) * 1.5f + miny) - mbuf2->Location.y;
                }
                if (CurTerr->platdata[mbuf2->info].curmtx->_32 > CurTerr->platdata[mbuf2->info].oldmtx._32) {
                    tminz = minz - mbuf2->Location.z;
                    tmaxz = ((CurTerr->platdata[mbuf2->info].curmtx->_32 -
                    CurTerr->platdata[mbuf2->info].oldmtx._32) * 1.5f + maxz) - mbuf2->Location.z;
                }
                else {
                    tmaxz = maxz - mbuf2->Location.z;
                    tminz = ((CurTerr->platdata[mbuf2->info].curmtx->_32 -
                    CurTerr->platdata[mbuf2->info].oldmtx._32) * 1.5f + minz) - mbuf2->Location.z;
                }
            }
            else {
                tmaxx = maxx - mbuf2->Location.x;
                tminx = minx - mbuf2->Location.x;
                tmaxy = maxy - mbuf2->Location.y;
                tminy = miny - mbuf2->Location.y;
                tmaxz = maxz - mbuf2->Location.z;
                tminz = minz - mbuf2->Location.z;
            }
            
            if ((s32)CurTerr->platdata[mbuf2->info].status.rotate != 0) {
                tn = POW2((maxx + minx) * 0.5f - mbuf2->Location.x) +
                    POW2(((maxy + miny) * 0.5f - mbuf2->Location.y) * TerI->inyscale)  +
                    POW2((maxz + minz) * 0.5f - mbuf2->Location.z);
                if ((mbuf2->type != ~TERR_TYPE_NORMAL) && (tn < mbuf2->radius + radmov)) {
                    modp = (struct scaleterrain_s *)mbuf2->model;
                    while (*modp >= 0) {
                        c = modp[1];
                        ter = modp + 10;
                        for (; c > 0; c--) {
                            platinrange = 1;
                            if (
                                (HitData < MaxData)
                                && ((ter->info[1] == 0) || ((ter->info[1] & extramask) != 0))
                            ) {
                                pnts[0].x = ter->pnts[0].x;
                                pnts[0].y = ter->pnts[0].y;
                                pnts[0].z = ter->pnts[0].z;
                                pnts[0].w = 0.0f;
                                pnts[1].x = ter->pnts[1].x;
                                pnts[1].y = ter->pnts[1].y;
                                pnts[1].z = ter->pnts[1].z;
                                pnts[1].w = 0.0f;
                                pnts[2].x = ter->pnts[2].x;
                                pnts[2].y = ter->pnts[2].y;
                                pnts[2].z = ter->pnts[2].z;
                                pnts[2].w = 0.0f;
                                NuVec4MtxTransformVU0(&pnts[0], &pnts[0], CurTerr->platdata[mbuf2->info].curmtx);
                                NuVec4MtxTransformVU0(&pnts[1], &pnts[1], CurTerr->platdata[mbuf2->info].curmtx);
                                NuVec4MtxTransformVU0(&pnts[2], &pnts[2], CurTerr->platdata[mbuf2->info].curmtx);
                                norm[0].x = ter->norm[0].x;
                                norm[0].y = ter->norm[0].y;
                                norm[0].z = ter->norm[0].z;
                                norm[0].w = 0.0f;
                                NuVec4MtxTransformVU0(&norm[0], &norm[0], CurTerr->platdata[mbuf2->info].curmtx);
                                    
                                if (65535.0f > ter->norm[1].y) {
                                    pnts[3].x = ter->pnts[3].x;
                                    pnts[3].y = ter->pnts[3].y;
                                    pnts[3].z = ter->pnts[3].z;
                                    pnts[3].w = 0.0f;
                                    NuVec4MtxTransformVU0(&pnts[3], &pnts[3], CurTerr->platdata[mbuf2->info].curmtx);
                                    norm[1].x = ter->norm[1].x;
                                    norm[1].y = ter->norm[1].y;
                                    norm[1].z = ter->norm[1].z;
                                    norm[1].w = 0.0f;
                                    NuVec4MtxTransformVU0(&norm[1], &norm[1], CurTerr->platdata[mbuf2->info].curmtx);
                                }
                                else {
                                    pnts[3] = pnts[2];
                                }
                                
                                if (
                                    (
                                        (pnts[0].x > tminx) 
                                        || (pnts[1].x > tminx) 
                                        || (pnts[2].x > tminx)
                                        || (pnts[3].x > tminx)
                                    ) 
                                    &&
                                    (
                                        (pnts[0].x < tmaxx) 
                                        || (pnts[1].x < tmaxx)
                                        || (pnts[2].x < tmaxx)
                                        || (pnts[3].x < tmaxx)
                                    ) 
                                    &&
                                    (
                                        (pnts[0].z > tminz)
                                        || (pnts[1].z > tminz)
                                        || (pnts[2].z > tminz)
                                        || (pnts[3].z > tminz)
                                    )
                                    &&
                                    (
                                        (pnts[0].z < tmaxz)
                                        || (pnts[1].z < tmaxz)
                                        || (pnts[2].z < tmaxz)
                                        || (pnts[3].z < tmaxz)
                                    )
                                ) {
                                    ScaleTerrain[curscltemp].info[0] = ter->info[0];
                                    ScaleTerrain[curscltemp].info[1] = ter->info[1];
                                    ScaleTerrain[curscltemp].info[2] = ter->info[2];
                                    ScaleTerrain[curscltemp].info[3] = ter->info[3];
                                    ScaleTerrain[curscltemp].pnts[0].x = pnts[0].x;
                                    ScaleTerrain[curscltemp].pnts[0].z = pnts[0].z;
                                    ScaleTerrain[curscltemp].pnts[0].y =
                                     (pnts[0].y + mbuf2->Location.y) * TerI->inyscale - mbuf2->Location.y;
                                    ScaleTerrain[curscltemp].pnts[1].x = pnts[1].x;
                                    ScaleTerrain[curscltemp].pnts[1].z = pnts[1].z;
                                    ScaleTerrain[curscltemp].pnts[1].y =
                                     (pnts[1].y + mbuf2->Location.y) * TerI->inyscale - mbuf2->Location.y;
                                    ScaleTerrain[curscltemp].pnts[2].x = pnts[2].x;
                                    ScaleTerrain[curscltemp].pnts[2].z = pnts[2].z;
                                    ScaleTerrain[curscltemp].pnts[2].y =
                                     (pnts[2].y + mbuf2->Location.y) * TerI->inyscale - mbuf2->Location.y;
                                    if (65535.0f > ter->norm[1].y) {
                                        ScaleTerrain[curscltemp].pnts[3].x = pnts[3].x;
                                        ScaleTerrain[curscltemp].pnts[3].z = pnts[3].z;
                                        ScaleTerrain[curscltemp].pnts[3].y =
                                           (pnts[3].y + mbuf2->Location.y) * TerI->inyscale - mbuf2->Location.y;
                                        tn =  1.0f / NuFsqrt(POW2(norm[1].x) + POW2(norm[1].y) * TerI->yscalesq + POW2(norm[1].z));
                                        ScaleTerrain[curscltemp].norm[1].x = norm[1].x * tn;
                                        ScaleTerrain[curscltemp].norm[1].y = norm[1].y * TerI->yscale * tn;
                                        ScaleTerrain[curscltemp].norm[1].z = norm[1].z * tn;
                                    }
                                    else {
                                        ScaleTerrain[curscltemp].norm[1].y = 65536.0f;
                                    }
                            
                                    tn =  1.0f / NuFsqrt(POW2(norm[0].x) + POW2(norm[0].y) * TerI->yscalesq + POW2(norm[0].z));
                                    
                                    ScaleTerrain[curscltemp].norm[0].x = norm[0].x * tn;
                                    ScaleTerrain[curscltemp].norm[0].y = norm[0].y * TerI->yscale * tn;
                                    ScaleTerrain[curscltemp].norm[0].z = norm[0].z * tn;
                                    *HitData = ScaleTerrain + curscltemp;
                                    HitData++;
                                    HitCnt++;
                                    curscltemp++;
                                }
                            }
                            ter++;
                        }
                        modp = ter;
                    }
                    
                    if (HitCnt != 0) {
                        LastWrite[0] = HitCnt;
                        LastWrite[1] = *ttemp;
                        LastWrite = HitData;
                        HitData++;
                        HitCnt = 0;
                    }
                }
            }
            else if (
                (tmaxx >= mbuf2->min.x) 
                && (tmaxy >= mbuf2->min.y)
                && (tmaxz >= mbuf2->min.z) 
                && (tminx <= mbuf2->max.x) 
                && (tminy < mbuf2->max.y)
                && (tminz < mbuf2->max.z)
                && (mbuf2->type != ~TERR_TYPE_NORMAL)
            ) {
                modp = (struct scaleterrain_s *)mbuf2->model;
                while (*modp >= 0) {
                    c = modp[1];
                    ter = modp + 10;
                    if (
                        (tmaxx >= *(float*)&modp[2]) 
                        && (tminx < *(float*)&modp[4])
                    ) {
                        if (
                            (tmaxz >= *(float*)&modp[6])
                            && (tminz < *(float*)&modp[8])
                        ) {
                            for (; c > 0; c--) {
                                if (
                                    (tmaxx >= ter->minx)
                                    && (tminx < ter->maxx)
                                    && (tmaxz >= ter->minz)
                                    && (tminz < ter->maxz)
                                    && (tmaxy >= ter->miny)
                                    && (tminy < ter->maxy)
                                    && (platinrange = 1,HitData < MaxData) 
                                    && ((ter->info[1] == 0) || ((ter->info[1] & extramask) != 0))
                                ) {
                                    if (TerI->yscale == 1.0f) {
                                        *HitData = ter;
                                        HitData++;
                                        HitCnt++;
                                    }
                                    else {
                                        ScaleTerrain[curscltemp].info[0] = ter->info[0];
                                        ScaleTerrain[curscltemp].info[1] = ter->info[1];
                                        ScaleTerrain[curscltemp].info[2] = ter->info[2];
                                        ScaleTerrain[curscltemp].info[3] = ter->info[3];
                                        ScaleTerrain[curscltemp].pnts[0].x = ter->pnts[0].x;
                                        ScaleTerrain[curscltemp].pnts[0].z = ter->pnts[0].z;
                                        ScaleTerrain[curscltemp].pnts[0].y =
                                         (ter->pnts[0].y + mbuf2->Location.y) * TerI->inyscale - mbuf2->Location.y;
                                        ScaleTerrain[curscltemp].pnts[1].x = ter->pnts[1].x;
                                        ScaleTerrain[curscltemp].pnts[1].z = ter->pnts[1].z;
                                        ScaleTerrain[curscltemp].pnts[1].y =
                                         (ter->pnts[1].y + mbuf2->Location.y) * TerI->inyscale - mbuf2->Location.y;
                                        ScaleTerrain[curscltemp].pnts[2].x = ter->pnts[2].x;
                                        ScaleTerrain[curscltemp].pnts[2].z = ter->pnts[2].z;
                                        ScaleTerrain[curscltemp].pnts[2].y =
                                         (ter->pnts[2].y + mbuf2->Location.y) * TerI->inyscale - mbuf2->Location.y;
                                        if (65535.0f > ter->norm[1].y) {
                                            ScaleTerrain[curscltemp].pnts[3].x = ter->pnts[3].x;
                                            ScaleTerrain[curscltemp].pnts[3].z = ter->pnts[3].z;
                                            ScaleTerrain[curscltemp].pnts[3].y =
                                               (ter->pnts[3].y + mbuf2->Location.y) * TerI->inyscale - mbuf2->Location.y;
                                            tn =  1.0f / NuFsqrt(POW2(ter->norm[1].x) + POW2(ter->norm[1].y) * TerI->yscalesq + POW2(ter->norm[1].z));
                                            ScaleTerrain[curscltemp].norm[1].x = ter->norm[1].x * tn;
                                            ScaleTerrain[curscltemp].norm[1].y = ter->norm[1].y * TerI->yscale * tn;
                                            ScaleTerrain[curscltemp].norm[1].z = ter->norm[1].z * tn;
                                        }
                                        else {
                                            ScaleTerrain[curscltemp].norm[1].y = 65536.0f;
                                        }
                                        tn = 1.0f / NuFsqrt(POW2(ter->norm[0].x) + POW2(ter->norm[0].y) * TerI->yscalesq + POW2(ter->norm[0].z));
                                        
                                        ScaleTerrain[curscltemp].norm[0].x = ter->norm[0].x * tn;
                                        ScaleTerrain[curscltemp].norm[0].y = ter->norm[0].y * TerI->yscale * tn;
                                        ScaleTerrain[curscltemp].norm[0].z = ter->norm[0].z * tn;
                                        *HitData = ScaleTerrain + curscltemp;
                                        HitData++;
                                        HitCnt++;
                                        curscltemp++;
                                    }
                                }
                                ter++;
                            }
                            modp = ter;
                        }
                        else {
                            modp = (struct scaleterrain_s *)ter + c;
                        }
                    }
                    else {
                        modp = (struct scaleterrain_s *)ter + c;
                    }
                }
                
                if (HitCnt != 0) {
                    LastWrite[0] = HitCnt;
                    LastWrite[1] = *ttemp;
                    LastWrite = HitData;
                    HitData++;
                    HitCnt = 0;
                }
            }
        }
    }

    LastWrite[0] = 0;
    LastWrite[1] = 0;
    minx -= 0.02f;
    minz -= 0.02f;
    maxx += 0.02f;
    maxz += 0.02f;
    WallSplCount = 0;

    for (WallSpl = (struct wallspl_s *)CurTerr->wallinfo; WallSpl != 0; WallSpl = ((int*)WallSpl)[-1]) {
        for (a = 0; a < WallSpl->count; a += 0x10) {
            if (WallSpl->spl[a].y != 0.02f) {
                if (
                    (WallSpl->spl[a + 1].y >= minx) 
                    && (WallSpl->spl[a].y <= maxx)
                    && (WallSpl->spl[a + 3].y >= minz)
                    && (WallSpl->spl[a + 2].y <= maxz)
                ) {
                    c = a + 0x10;
                    if (c > WallSpl->count) {
                        c = WallSpl->count;
                    }
                }
                else {
                    c = 0;
                } 
            } else {
                c = WallSpl->count;
            }
            
            for (b = a; b < c; b++) {
                if (
                    (
                        ((WallSpl->spl[b].x >= minx) && (WallSpl->spl[b + 1].x <= maxx)) 
                        || ((WallSpl->spl[b + 1].x >= minx) && (WallSpl->spl[b].x <= maxx))
                    )
                    && 
                    (
                        ((WallSpl->spl[b].z >= minz) && (WallSpl->spl[b + 1].z <= maxz)) 
                        || ((WallSpl->spl[b + 1].z >= minz) && (WallSpl->spl[b].z <= maxz))
                    )
                    && (WallSplCount < 0x40)
                ) {
                    WallSplList[WallSplCount] = WallSpl->spl[b];
                    WallSplCount++; 
                    WallSplList[WallSplCount] = WallSpl->spl[b + 1];
                    WallSplCount++;
                }
            }
        }
    }
}


void FullDeflect(nuvec_s *N,nuvec_s *L,nuvec_s *R)

{
  float dotp;
  
  dotp = -L->x * N->x + -L->y * N->y + -L->z * N->z + 0.001;
  R->x = dotp * N->x + L->x;
  R->y = dotp * N->y + L->y;
  R->z = dotp * N->z + L->z;
  return;
}

//NGC MATCH
s32 FullDeflectTest(struct nuvec_s *N,struct nuvec_s *L,struct nuvec_s *R) {
  float dotp;
  float len;
  
  dotp = (N->x * -L->x + N->y * -L->y + N->z * -L->z) + 0.001f;
  len = 0.001f / NuFsqrt(L->x * L->x + L->y * L->y + L->z * L->z);
  R->x = N->x * dotp + L->x;
  R->y = N->y * dotp + L->y;
  R->z = N->z * dotp + L->z;
  if (dotp > 0.0f) {
        return 1;
  } else {
    return 0;
  }
}

void FullReflect(nuvec_s *N,nuvec_s *L,nuvec_s *R)

{
  float fVar1;
  float fVar2;
  
  fVar1 = -L->x * N->x + -L->y * N->y + -L->z * N->z;
  fVar2 = fVar1 * N->x;
  R->x = fVar2 + fVar2 + L->x;
  fVar2 = fVar1 * N->y;
  R->y = fVar2 + fVar2 + L->y;
  fVar1 = fVar1 * N->z;
  R->z = fVar1 + fVar1 + L->z;
  return;
}

//NGC MATCH
void TerrainMoveImpactData() {
    switch (TerI->hittype) 
    {
        case 0:
        break;
        
        case 1:
        case 2:
        case 3:
        case 4:
        case 0x11:
        case 0x12:
        case 0x13:
        case 0x14:
        if (TerI->hitterrno != -1) {
            TerrPolyInfo = *TerI->hitter;
            TerrPoly = &TerrPolyInfo;
            TerrPolyObj = TerI->hitterrno;
        }
        return;
    }
}

void TerrainImpactNorm(void)

{
  float fVar1;
  short type;
  
  TerrainMoveImpactData();
  type = TerI->hittype;
  if (type == 4) {
LAB_8009d934:
    terrhitflags = terrhitflags | 4;
  }
  else if (type < 5) {
    if (type == 1) goto LAB_8009d9c8;
    if (type < 1) {
      return;
    }
  }
  else {
    if (0x13 < type) {
      if (type != 0x14) {
        return;
      }
      goto LAB_8009d934;
    }
    if (type < 0x12) {
      if (type != 0x11) {
        return;
      }
      goto LAB_8009d9c8;
    }
  }
  RotateVec(&TerI->hitnorm,&TerI->hitnorm);
  if ((TerI->hittype & 0x10U) == 0) {
    (TerI->hitnorm).x = (TerI->hitnorm).x * TerI->sizediv;
    (TerI->hitnorm).y = (TerI->hitnorm).y * TerI->sizediv;
    (TerI->hitnorm).z = (TerI->hitnorm).z * TerI->sizediv;
  }
LAB_8009d9c8:
  if (TerI->yscale == 1.0) {
    (TerI->uhitnorm).x = (TerI->hitnorm).x;
    (TerI->uhitnorm).y = (TerI->hitnorm).y;
    (TerI->uhitnorm).z = (TerI->hitnorm).z;
  }
  else {
    fVar1 = NuFsqrt((TerI->hitnorm).x * (TerI->hitnorm).x +
                    (TerI->hitnorm).y * (TerI->hitnorm).y * TerI->inyscalesq +
                    (TerI->hitnorm).z * (TerI->hitnorm).z);
    fVar1 = 1.0 / fVar1;
    (TerI->uhitnorm).x = (TerI->hitnorm).x * fVar1;
    (TerI->uhitnorm).y = (TerI->hitnorm).y * TerI->inyscale * fVar1;
    (TerI->uhitnorm).z = (TerI->hitnorm).z * fVar1;
  }
  return;
}

//NGC MATCH
void PlatformConnect(char *flags,struct nuvec_s *vel,struct nuvec_s *ivel,s32 platid) {
  if (CurTrackInfo != NULL) {
    CurTrackInfo->platinf = CurTrackInfo->platinf | 1;
    CurTrackInfo->platid = platid;
    CurTrackInfo->timer = 4;
    return;
  }
    CurTrackInfo = AllocTerrId();
    if (CurTrackInfo != NULL) {
      CurTrackInfo->ptrid = flags;
      CurTrackInfo->platinf = CurTrackInfo->platinf | 1;
      CurTrackInfo->platid = platid;
      CurTrackInfo->timer = 4;
      vel->y = 0.0f;
      ivel->y = 0.0f;
    } 
}

//NGC MATCH
void TerrainImpact(struct nuvec_s *vpos,struct nuvec_s *vvel,u8 *flags) {
  TerrainMoveImpactData();
  switch (TerI->hittype) {
    case 0:
        *flags = 0;
        vpos->x = (TerI->curpos).x + (TerI->curvel).x;
        vpos->y = ((TerI->curpos).y + (TerI->curvel).y) * TerI->yscale - TerI->size * TerI->yscale ;
        vpos->z = (TerI->curpos).z + (TerI->curvel).z;
    break;
        case 1:
        case 2:
        case 3:
    case 4:
      TerI->hittime = TerI->hittime - TerI->timeadj;
      if (TerI->hittime < 0.0f) {
        TerI->hittime = 0.0f;
      }
      (TerI->curpos).x = (TerI->curpos).x + (TerI->curvel).x * TerI->hittime;
      (TerI->curpos).y = (TerI->curpos).y + (TerI->curvel).y * TerI->hittime;
      (TerI->curpos).z = (TerI->curpos).z + (TerI->curvel).z * TerI->hittime;
      (TerI->curvel).x = (TerI->curvel).x - (TerI->curvel).x * TerI->hittime;
      (TerI->curvel).y = (TerI->curvel).y - (TerI->curvel).y * TerI->hittime;
      (TerI->curvel).z = (TerI->curvel).z - (TerI->curvel).z * TerI->hittime;
      FullDeflect(&TerI->hitnorm,&TerI->curvel,&TerI->curvel);
      FullDeflect(&TerI->uhitnorm,vvel,vvel);
      if (0.707f > (TerI->uhitnorm).y) {
        *flags = 0;
        terrhitflags = terrhitflags | 1;
        (TerI->curpos).x = (TerI->curpos).x + (TerI->hitnorm).x * 0.002f;
        (TerI->curpos).z = (TerI->curpos).z + (TerI->hitnorm).z * 0.002f;
        (TerI->curvel).x = (TerI->curvel).x + (TerI->hitnorm).x * 0.003f;
        (TerI->curvel).z = (TerI->curvel).z + (TerI->hitnorm).z * 0.003f;
        vvel->x = vvel->x + (TerI->hitnorm).x * 0.003f;
        vvel->z = vvel->z + (TerI->hitnorm).z * 0.003f;
      }
      else {
        terrhitflags = terrhitflags | 2;
        *flags = 1;
        flags[1] = 1;
        (TerI->curpos).y = (TerI->curpos).y + (TerI->hitnorm).y * 0.001f;

      }
    break;
    case 17:
    case 18:
    case 19:
    case 20:
        FullDeflect(&TerI->hitnorm,&TerI->curvel,&TerI->curvel);
        FullDeflect(&TerI->uhitnorm,vvel,vvel);
        if (0.707f > (TerI->uhitnorm).y) {
          (TerI->curpos).x = (TerI->curpos).x + (TerI->hitnorm).x * 0.005f;
          (TerI->curpos).y = (TerI->curpos).y + (TerI->hitnorm).y * 0.002f;
          (TerI->curpos).z = (TerI->curpos).z + (TerI->hitnorm).z * 0.005f;
          (TerI->curvel).x = (TerI->curvel).x + (TerI->hitnorm).x * 0.005f;
          (TerI->curvel).y = (TerI->curvel).y + (TerI->hitnorm).y * 0.002f;
          (TerI->curvel).z = (TerI->curvel).z + (TerI->hitnorm).z * 0.005f;
          *flags = 1;
        } else {
          (TerI->curpos).y = (TerI->curpos).y + (TerI->hitnorm).y * 0.005f;
          (TerI->curvel).y = (TerI->curvel).y + (TerI->hitnorm).y * 0.01f; 
        }
        break;
  }
    if ((((TerI->hittype != 0) && (TerI->hittype < 0x10)) && (TerI->hitterrno != -1)) &&
         ((CurTerr->terr[TerI->hitterrno].type == TERR_TYPE_PLATFORM && (0.707f < (TerI->uhitnorm).y))) )
      {
        PlatformConnect((char *)TerI->flags,&TerI->curvel,vvel,CurTerr->terr[TerI->hitterrno].info);
    }
}

int TerrainImpactPlatform(uchar *flags)

{
  short sVar1;
  
  TerrainMoveImpactData();
  sVar1 = TerI->hittype;
  if (sVar1 < 5) {
    if (0 < sVar1) {
      TerI->hittime = TerI->hittime - TerI->timeadj;
      if (TerI->hittime < 0.0) {
        TerI->hittime = 0.0;
      }
      (TerI->curpos).x = (TerI->curpos).x + (TerI->curvel).x * TerI->hittime;
      (TerI->curpos).y = (TerI->curpos).y + (TerI->curvel).y * TerI->hittime;
      (TerI->curpos).z = (TerI->curpos).z + (TerI->curvel).z * TerI->hittime;
      if (0.707 <= (TerI->uhitnorm).y) {
        *flags = '\x01';
        flags[1] = '\x01';
        (TerI->curpos).y = (TerI->curpos).y + (TerI->hitnorm).y * 0.002;
      }
      else {
        *flags = '\0';
      }
      return 0;
    }
    if (sVar1 == 0) {
      *flags = '\0';
      (TerI->curpos).x = (TerI->curpos).x + (TerI->curvel).x;
      (TerI->curpos).y = (TerI->curpos).y + (TerI->curvel).y;
      (TerI->curpos).z = (TerI->curpos).z + (TerI->curvel).z;
      return 0;
    }
  }
  else if ((sVar1 < 0x15) && (0x10 < sVar1)) {
    return 1;
  }
  return 1;
}

//NGC MATCH
s32 TerrainPlatformMoveCheck(struct nuvec_s *dest,struct nuvec_s *norm,s32 platid,s32 calcimpact,s32 leaveoffonfail) {
  short *CurData;
  struct nuvec_s curvel; // 0x20(r31)
  short hitcnt;
  short hitterrno;
  short hittype;
  short plathit;
  float hittime;
  struct nuvec_s hitnorm; // 0x40(r31)
  struct nuvec_s uhitnorm; // 0x50(r31)
  s32 terrid;
  
  terrid = CurTerr->platdata[platid].terrno;
  CurData = TerI->PlatScanStart;
  while ( 0 < *CurData) {
    if (CurData[1] == terrid) {
        *CurData = (*CurData <= 0 ? *CurData  : -*CurData);
    }
      CurData += (*CurData >= 0 ? *CurData  : -*CurData) * 2 + 2;
  }
  curvel = TerI->curvel;
  hitcnt = TerI->hitcnt;
  hitterrno = TerI->hitterrno;
  hittype = TerI->hittype;
  plathit = TerI->plathit;
  hittime = TerI->hittime;
  hitnorm = TerI->hitnorm;
  uhitnorm = TerI->uhitnorm;
  (TerI->curvel).x = dest->x - (TerI->curpos).x;
  (TerI->curvel).y = dest->y - (TerI->curpos).y;
  (TerI->curvel).z = dest->z - (TerI->curpos).z;
  DerotateMovementVector();
  HitTerrain();
  if ((calcimpact != 0) && (TerI->hittype != 0)) {
    TerrainImpactNorm();
    TerrainImpactPlatform(TerI->flags);
    if (norm != NULL) {
      *norm = TerI->hitnorm;
    }
    *dest = TerI->curpos;
  }
  (TerI->curvel) = curvel;
  TerI->hitcnt = hitcnt;
  TerI->hitterrno = hitterrno;
  TerI->plathit = plathit;
  TerI->hittime = hittime;
  TerI->hitnorm = hitnorm;
  TerI->uhitnorm = uhitnorm;
  CurData = TerI->PlatScanStart;
  if ((TerI->hittype < 0x10) || (leaveoffonfail == 0)) {
    while (*CurData != 0)  {
      if (CurData[1] == terrid) {
          *CurData = -(*CurData <= 0 ? *CurData  : -*CurData);
      }
      CurData += (*CurData >= 0 ? *CurData  : -*CurData) * 2 + 2;
    }
  }
  if (TerI->hittype != 0) {
    TerI->hittype = hittype;
    return 0;
  }
  else {
    TerI->hittype = hittype;
    return 1;
  }
}

int TerrainPlatformEmbedded(nuvec_s* vvel)
{
		//TODO
}


void StorePlatImpact(void)

{
  hitdata *hitTer;
  hitdata *PImpTer;
  int i;
  float fVar1;
  nuvec_s hitnorm;
  short hType;
  
  if (TerI->hittype == 0) {
    return;
  }
  if (TerI->hitterrno == -1) {
    return;
  }
  if (CurTerr->terr[TerI->hitterrno].type != TERR_TYPE_PLATFORM) {
    return;
  }
  hitnorm.x = (TerI->hitnorm).x;
  hitnorm.y = (TerI->hitnorm).y;
  hitnorm.z = (TerI->hitnorm).z;
  hType = TerI->hittype;
  if (hType < 5) {
    if (hType < 2) {
      if (hType != 1) {
        return;
      }
      goto LAB_800a0254;
    }
  }
  else {
    if (hType == 0x11) goto LAB_800a0254;
    if (hType < 0x11) {
      return;
    }
    if (0x14 < hType) {
      return;
    }
  }
  RotateVec(&hitnorm,&hitnorm);
  if ((TerI->hittype & 0x10U) == 0) {
    hitnorm.x = hitnorm.x * TerI->sizediv;
    hitnorm.y = hitnorm.y * TerI->sizediv;
    hitnorm.z = hitnorm.z * TerI->sizediv;
  }
LAB_800a0254:
  if (TerI->yscale != 1.0) {
    fVar1 = NuFsqrt(hitnorm.x * hitnorm.x + hitnorm.y * hitnorm.y * TerI->inyscalesq +
                    hitnorm.z * hitnorm.z);
    fVar1 = 1.0 / fVar1;
    hitnorm.x = hitnorm.x * fVar1;
    hitnorm.y = hitnorm.y * TerI->inyscale * fVar1;
    hitnorm.z = hitnorm.z * fVar1;
  }
  PlatImpactId = (int)CurTerr->terr[TerI->hitterrno].info;
  PlatImpactNorm.x = hitnorm.x;
  PlatImpactNorm.y = hitnorm.y;
  PlatImpactNorm.z = hitnorm.z;
  hitTer = TerI->hitter;
  PImpTer = &PlatImpactTer;
  i = 0x60;
  do {
    i = i + -0x18;
    PImpTer->minx = hitTer->minx;
    PImpTer->maxx = hitTer->maxx;
    PImpTer->miny = hitTer->miny;
    PImpTer->maxy = hitTer->maxy;
    PImpTer->minz = hitTer->minz;
    PImpTer->maxz = hitTer->maxz;
    hitTer = (hitdata *)hitTer->pnts;
    PImpTer = (hitdata *)PImpTer->pnts;
  } while (i != 0);
  ((nuvec_s *)PImpTer)->x = ((nuvec_s *)hitTer)->x;
  return;
}


int PlatformChecks(int itterationcnt,nuvec_s *vvel)		//WIP

{
  short sVar1;
  numtx_s *pnVar2;
  TerrI *TerITemp3;
  nuvec4_s *v;
  int test;
  numtx_s *pnVar3;
  numtx_s *pnVar4;
  int iVar5;
  float fVar6;
  platdata *ppVar7;
  platdata *ppVar8;
  numtx_s *pnVar9;
  double dVar10;
  int cnt;
  short *platSstart;
  hitdata **hitD;
  int local_48;
  int check;
  nuvec4_s pnts [2];
  float lp;
  char flags [2];
  TerrI *TerrITemp2;
  short *curDat;
  float dist;
  hitdata **ptrHitDat;
  TerrI *terrITMP;
  
  terrITMP = TerI;
  TerI = terrITMP;
  if (((TerI->platScanStart != (short *)0x0) && (*TerI->platScanStart != 0)) &&
     (CurTrackInfo != (TrackInfo *)0x0)) {
    TerITemp3 = (TerrI *)NuScratchAlloc32(0x930);
    platSstart = terrITMP->platScanStart;
    hitD = TerITemp3->hitdat;
    TerI = TerITemp3;
    while (0 < *platSstart) {
      sVar1 = *platSstart;
      *(short *)hitD = sVar1;
      local_48 = (int)sVar1;
      *(short *)((int)hitD + 2) = platSstart[1];
      curDat = platSstart;
      ptrHitDat = hitD;
      while( true ) {
        hitD = ptrHitDat + 1;
        platSstart = curDat + 2;
        if (local_48 < 1) break;
        *(short *)hitD = *platSstart;
        *(short *)((int)ptrHitDat + 6) = curDat[3];
        local_48 = local_48 + -1;
        curDat = platSstart;
        ptrHitDat = hitD;
      }
    }
    *(short *)hitD = 0;
    *(short *)((int)hitD + 2) = 0;
    TerI->size = terrITMP->size;
    TerI->yscale = terrITMP->yscale;
    TerI->inyscalesq = terrITMP->inyscalesq;
    TerI->sizediv = terrITMP->sizediv;
    TerI->timeadj = terrITMP->timeadj;
    TerI->inyscale = terrITMP->inyscale;
    TerI->impactadj = terrITMP->impactadj;
    TerrITemp2 = TerI;
    fVar6 = (terrITMP->curpos).y;
    dist = (terrITMP->curpos).z;
    TerI->tempvec[0].x = (terrITMP->curpos).x;
    TerrITemp2->tempvec[0].y = fVar6;
    TerrITemp2->tempvec[0].z = dist;
    v = (nuvec4_s *)TerITemp3;
    if ((CurTrackInfo != (TrackInfo *)0x0) && ((CurTrackInfo->platinf & 1U) != 0)) {
      if ((int)CurTerr->platdat[CurTrackInfo->platid].status < 0) {
        iVar5 = 0x30;
        pnVar2 = &tertempmat;
        ppVar8 = CurTerr->platdat + CurTrackInfo->platid;
        do {
          ppVar7 = ppVar8;
          pnVar3 = pnVar2;
          iVar5 = iVar5 + -0x18;
          pnVar3->_00 = (ppVar7->oldmtx)._00;
          pnVar3->_01 = (ppVar7->oldmtx)._01;
          pnVar3->_02 = (ppVar7->oldmtx)._02;
          pnVar3->_03 = (ppVar7->oldmtx)._03;
          pnVar3->_10 = (ppVar7->oldmtx)._10;
          pnVar3->_11 = (ppVar7->oldmtx)._11;
          ppVar8 = (platdata *)&(ppVar7->oldmtx)._12;
          pnVar2 = (numtx_s *)&pnVar3->_12;
        } while (iVar5 != 0);
        pnVar3->_12 = *(float *)ppVar8;
        pnVar3->_13 = (ppVar7->oldmtx)._13;
        pnVar3->_20 = (ppVar7->oldmtx)._20;
        pnVar3->_21 = (ppVar7->oldmtx)._21;
        tertempvec4.x = (terrITMP->curpos).x;
        tertempvec4.y = (terrITMP->curpos).y * TerI->yscale;
        tertempvec4.z = (terrITMP->curpos).z;
        tertempvec4.w = 1.0;
        NuVec4MtxInvTransform(&tertempvec4,&tertempvec4,&tertempmat);
        iVar5 = 0x30;
        pnVar2 = &tertempmat;
        pnVar3 = CurTerr->platdat[CurTrackInfo->platid].curmtx;
        do {
          pnVar9 = pnVar3;
          pnVar4 = pnVar2;
          iVar5 = iVar5 + -0x18;
          pnVar4->_00 = pnVar9->_00;
          pnVar4->_01 = pnVar9->_01;
          pnVar4->_02 = pnVar9->_02;
          pnVar4->_03 = pnVar9->_03;
          pnVar4->_10 = pnVar9->_10;
          pnVar4->_11 = pnVar9->_11;
          pnVar2 = (numtx_s *)&pnVar4->_12;
          pnVar3 = (numtx_s *)&pnVar9->_12;
        } while (iVar5 != 0);
        pnVar4->_12 = pnVar9->_12;
        pnVar4->_13 = pnVar9->_13;
        pnVar4->_20 = pnVar9->_20;
        pnVar4->_21 = pnVar9->_21;
        v = &tertempvec4;
        NuVec4MtxTransformVU0(&tertempvec4,&tertempvec4,&tertempmat);
        tertempvec4.y = tertempvec4.y * TerI->inyscale;
        TerI->tempvec[0].x = TerI->tempvec[0].x + (tertempvec4.x - (terrITMP->curpos).x);
        TerI->tempvec[0].y = TerI->tempvec[0].y + (tertempvec4.y - (terrITMP->curpos).y);
        TerI->tempvec[0].z = TerI->tempvec[0].z + (tertempvec4.z - (terrITMP->curpos).z);
      }
      else {
        TerI->tempvec[0].x =
             TerI->tempvec[0].x +
             ((CurTerr->platdat[CurTrackInfo->platid].curmtx)->_30 -
             CurTerr->platdat[CurTrackInfo->platid].oldmtx._30);
        TerI->tempvec[0].y =
             TerI->tempvec[0].y +
             ((CurTerr->platdat[CurTrackInfo->platid].curmtx)->_31 -
             CurTerr->platdat[CurTrackInfo->platid].oldmtx._31) * terrITMP->inyscale;
        TerI->tempvec[0].z =
             TerI->tempvec[0].z +
             ((CurTerr->platdat[CurTrackInfo->platid].curmtx)->_32 -
             CurTerr->platdat[CurTrackInfo->platid].oldmtx._32);
      }
    }
    TerrITemp2 = TerI;
    fVar6 = TerI->tempvec[0].y;
    dist = TerI->tempvec[0].z;
    TerI->tempvec[1].x = TerI->tempvec[0].x;
    TerrITemp2->tempvec[1].y = fVar6;
    TerrITemp2->tempvec[1].z = dist;
    fVar6 = TerrITemp2->tempvec[1].y;
    dist = TerrITemp2->tempvec[1].z;
    (TerrITemp2->curpos).x = TerrITemp2->tempvec[1].x;
    (TerrITemp2->curpos).y = fVar6;
    (TerrITemp2->curpos).z = dist;
    (TerI->curvel).x = 0.0;
    (TerI->curvel).z = 0.0;
    (TerI->curpos).y = (TerI->curpos).y + 0.2;
    (TerI->curvel).y = -0.4;
    DerotateMovementVector();
    HitTerrain((nuvec_s *)v);
    StorePlatImpact();
    TerI->hittime = TerI->hittime + TerI->timeadj;
    TerrainImpactNorm();
    TerrainImpactPlatform((uchar *)flags);
    TerI->tempvec[0].y = (TerI->curpos).y - TerI->size;
    if (TerITemp3->hittype == 0) {
      TerI = terrITMP;
      NuScratchRelease();
    }
    else {
      dist = (TerITemp3->tempvec[1].y - TerITemp3->tempvec[0].y) - terrITMP->size;
      dVar10 = (double)dist;
      TerI = terrITMP;
      NuFabs(dist);
      if (dVar10 <= 0.05000000074505806) {
        if (0.707 <= (TerITemp3->hitnorm).y) {
          iVar5 = (int)CurTerr->terr[TerITemp3->hitterrno].info;
          if ((vvel->y <= 0.04) ||
             (vvel->y <=
              ((CurTerr->platdat[iVar5].curmtx)->_31 + 0.04) - CurTerr->platdat[iVar5].oldmtx._31) )
          {
            if ((TerITemp3->hittype != 0) &&
               (((0.707 < (TerITemp3->hitnorm).y && (-1 < TerITemp3->hitterrno)) &&
                (CurTerr->terr[TerITemp3->hitterrno].type == TERR_TYPE_PLATFORM)))) {
              sVar1 = CurTerr->terr[TerITemp3->hitterrno].info;
              CurTerr->platdat[sVar1].status =
                   (platattrib)((uint)CurTerr->platdat[sVar1].status | 0x40000000);
            }
            TerI->tempvec[0].x = TerITemp3->tempvec[0].x;
            if ((int)CurTerr->platdat[CurTrackInfo->platid].status < 0) {
              TerI->tempvec[0].y = TerITemp3->tempvec[0].y + TerI->size;
            }
            else {
              TerI->tempvec[0].y = TerITemp3->tempvec[0].y + TerI->size + 0.0025;
            }
            TerI->tempvec[0].z = TerITemp3->tempvec[0].z;
            pnts[0].x = TerI->tempvec[0].x - (TerI->curpos).x;
            pnts[0].y = TerI->tempvec[0].y - (TerI->curpos).y;
            pnts[0].z = TerI->tempvec[0].z - (TerI->curpos).z;
            pnts[0].w = NuFsqrt(pnts[0].x * pnts[0].x + pnts[0].y * pnts[0].y +
                                pnts[0].z * pnts[0].z);
            pnts[0].x = pnts[0].x / pnts[0].w;
            pnts[0].y = pnts[0].y / pnts[0].w;
            pnts[0].z = pnts[0].z / pnts[0].w;
            pnts[1].x = (TerI->curpos).x;
            pnts[1].y = (TerI->curpos).y;
            pnts[1].z = (TerI->curpos).z;
            cnt = itterationcnt;
            do {
              check = TerrainPlatformMoveCheck(TerI->tempvec,TerI->tempvec + 1,iVar5,1,0);
              if (((TerI->hittype != 0) && (-1 < TerI->hitterrno)) &&
                 (CurTerr->terr[TerI->hitterrno].type == TERR_TYPE_PLATFORM)) {
                CurTrackInfo->platid = CurTerr->terr[TerI->hitterrno].info;
              }
              itterationcnt = cnt + -1;
              if (pnts[0].x * TerI->tempvec[1].x + pnts[0].y * TerI->tempvec[1].y +
                  pnts[0].z * TerI->tempvec[1].z < -0.7) {
                check = 1;
              }
              if ((check == 0) && (0 < itterationcnt)) {
                dist = ((TerI->curpos).x - pnts[1].x) * pnts[0].x +
                       ((TerI->curpos).y - pnts[1].y) * pnts[0].y +
                       ((TerI->curpos).z - pnts[1].z) * pnts[0].z;
                if (pnts[0].w <= dist) {
                  lp = 0.0;
                }
                else {
                  test = FullDeflectTest(TerI->tempvec + 1,(nuvec_s *)pnts,TerI->tempvec);
                  if (test == 0) {
                    lp = 0.0;
                  }
                  else {
                    fVar6 = pnts[0].x * TerI->tempvec[0].x + pnts[0].y * TerI->tempvec[0].y +
                            pnts[0].z * TerI->tempvec[0].z;
                    if (fVar6 <= 0.0) {
                      lp = 0.0;
                    }
                    else {
                      fVar6 = (pnts[0].w - dist) / fVar6;
                      if (0.8 <= fVar6) {
                        lp = 0.0;
                      }
                      else {
                        TerI->tempvec[0].x = TerI->tempvec[0].x * fVar6 + (TerI->curpos).x;
                        TerI->tempvec[0].y = TerI->tempvec[0].y * fVar6 + (TerI->curpos).y;
                        TerI->tempvec[0].z = TerI->tempvec[0].z * fVar6 + (TerI->curpos).z;
                        lp = 1.0;
                      }
                    }
                  }
                }
              }
              TerrITemp2 = TerI;
            } while (((check == 0) && (0 < itterationcnt)) && (cnt = itterationcnt, lp != 0.0));
            if (check != 0) {
              TerrPoly = TerITemp3->hitter;
              fVar6 = TerI->tempvec[0].y;
              dist = TerI->tempvec[0].z;
              (TerI->curpos).x = TerI->tempvec[0].x;
              (TerrITemp2->curpos).y = fVar6;
              (TerrITemp2->curpos).z = dist;
              if (vvel->y < 0.0) {
                FullDeflect(&ShadNorm,&TerI->curvel,&TerI->curvel);
                FullDeflect(&ShadNorm,vvel,vvel);
              }
              TerI->flags[1] = '\x01';
              PlatformConnect((char *)terrITMP->flags,&terrITMP->curvel,vvel,
                              (int)CurTerr->terr[TerITemp3->hitterrno].info);
            }
            NuScratchRelease();
            TerI = terrITMP;
          }
          else {
            NuScratchRelease();
          }
        }
        else {
          NuScratchRelease();
        }
      }
      else {
        NuScratchRelease();
      }
    }
  }
  return itterationcnt;
}

//NGC MATCH
s32 ShadowInfo(void) {
  if (ShadPoly != NULL) {
    return ShadPoly->info[0];
  }
  else {
    return -1;
  }
}

//NGC MATCH
void ShadowDir(struct nuvec_s *dir) {
  dir->x = ShadPoly->pnts[1].x - ShadPoly->pnts[0].x;
  dir->y = ShadPoly->pnts[1].y - ShadPoly->pnts[0].y;
  dir->z = ShadPoly->pnts[1].z - ShadPoly->pnts[0].z;
}

//NGC MATCH
s32 EShadowInfo(void) {  
  if (EShadPoly != NULL) {
    return EShadPoly->info[1];
  }
  else {
    return -1;
  }
}

//NGC MATCH
s32 ShadowRoofInfo(void) {
  if (ShadRoofPoly != NULL) {
    return ShadRoofPoly->info[0];
  }
  else {
    return -1;
  }
}

//NGC MATCH
float NewShadow(struct nuvec_s *ppos,float size) {
  struct nuvec_s pos;
  
  if (CurTerr == NULL) {
    return 2000000.0f;
  }
    TerI = (struct teri_s *)NuScratchAlloc32(0x930);
    pos = *ppos;
    NewScan(&pos,0,0);
    NewCast(&pos,5.0f);
    NuScratchRelease();  
    return pos.y;
}

//NGC MATCH
float NewShadowMask(struct nuvec_s *ppos,float size,int extramask) {
  struct nuvec_s pos;
  
  if (CurTerr == NULL) {
    return 2000000.0f;
  }
    TerI = (struct teri_s *)NuScratchAlloc32(0x930);
    pos = *ppos;
    NewScan(&pos,extramask,0);
    NewCast(&pos,5.0f);
    NuScratchRelease();
  return pos.y;
}

//NGC MATCH
float NewShadowPlat(struct nuvec_s *ppos,float size) {
  struct nuvec_s pos;
  
  if (CurTerr == NULL) {
    return 2000000.0f;
  }
    TerI = (struct teri_s *)NuScratchAlloc32(0x930);
    pos = *ppos;
    NewScan(&pos,0,1);
    NewCast(&pos,5.0f);
    NuScratchRelease();
  return pos.y;
}

//NGC MATCH
float NewShadowMaskPlat(struct nuvec_s *ppos,float size,int extramask) {
  struct nuvec_s pos;
  
  if (CurTerr == NULL) {
    return 2000000.0f;
  }
    TerI = (struct teri_s *)NuScratchAlloc32(0x930);
    pos = *ppos;
    NewScan(&pos,extramask,1);
    NewCast(&pos,5.0f);
    NuScratchRelease();
  return pos.y;
}

//NGC MATCH
float NewShadowMaskPlatRot(struct nuvec_s *ppos,float size,int extramask) {
  struct nuvec_s v;
  
  if (CurTerr == NULL) {
    return 2000000.0f;
  }
    TerI = (struct teri_s *)NuScratchAlloc32(0x930);
    v = *ppos;
    NewScanRot(&v,extramask);
    NewCast(&v,5.0f);
    NuScratchRelease();
  return v.y;
}

//NGC MATCH
void TerrainSideClamp(struct nuvec_s *slide,struct nuvec_s *pos) {
  float dotp;
  float dotq;
  
  dotp = (TerrShape->offset).x * slide->x + (TerrShape->offset).z * slide->z;
  dotq = (TerrShape->offset).x * slide->z - (TerrShape->offset).z * slide->x;
  if (dotp > TerrShape->size ) {
    dotp = TerrShape->size;
  }
  if (dotp < -TerrShape->size) {
    dotp = -TerrShape->size;
  }
  if (dotq > TerrShape->size * 0.2f) {
    dotq = TerrShape->size * 0.2f;
  }
  if (dotq < -TerrShape->size * 0.2f) {
    dotq = -TerrShape->size * 0.2f;
  }
  pos->x = pos->x - ((dotp * slide->x + dotq * slide->z) - (TerrShape->offset).x);
  pos->z = pos->z - ((dotp * slide->z - dotq * slide->x) - (TerrShape->offset).z);
  (TerrShape->offset).x = dotp * slide->x + dotq * slide->z;
  (TerrShape->offset).z = dotp * slide->z - dotq * slide->x;
}

int TerrShapeSideStep(nuvec_s *vpos,nuvec_s *vvel,uchar *flags)

{
  int doslide;
  nuvec_s slide;
  nuvec_s temp;
  float dotp;
  uint check;
  uchar tflags [2];
  short hittype;
  
  hittype = TerI->hittype;
  if (hittype == 0) {
    doslide = 1;
  }
  else {
    if (((hittype < 0) || (3 < hittype)) || (hittype < 2)) {
      if ((0.707 < (TerI->uhitnorm).y) || ((TerI->uhitnorm).y < -0.8)) {
        return 1;
      }
      check = 0;
    }
    else {
      check = (uint)(0.5 <= (TerI->uhitnorm).y);
    }
    slide.x = NuTrigTable[(int)(TerrShape->ang + 16384.0) & 0xffff];
    slide.y = 0.0;
    slide.z = -NuTrigTable[(int)TerrShape->ang & 0xffff];
    (TerI->origpos).x = (TerI->curpos).x;
    (TerI->origpos).y = (TerI->curpos).y;
    (TerI->origpos).z = (TerI->curpos).z;
    (TerI->origvel).x = (TerI->curvel).x;
    (TerI->origvel).y = (TerI->curvel).y;
    (TerI->origvel).z = (TerI->curvel).z;
    (TerI->curvel).y = 0.0;
    if (check == 1) {
      (TerI->curvel).x = (TerI->hitnorm).x * TerrShape->size * 0.15;
      (TerI->curvel).z = (TerI->hitnorm).z * TerrShape->size * 0.15;
      (TerrShape->offset).x = (TerrShape->offset).x + (TerI->hitnorm).x * TerrShape->size * 0.05;
      (TerrShape->offset).z = (TerrShape->offset).z + (TerI->hitnorm).z * TerrShape->size * 0.05;
    }
    else {
      dotp = slide.x * (TerI->uhitnorm).x + slide.z * (TerI->uhitnorm).z;
      if ((dotp == 0.0) &&
         (dotp = ((TerrShape->offset).x * slide.x + (TerrShape->offset).z * slide.z) /
                 TerrShape->size, dotp == 0.0)) {
        return 1;
      }
      if (0.0 <= dotp) {
        dotp = NuFsqrt(dotp);
      }
      else {
        dotp = NuFsqrt(-dotp);
        dotp = -dotp;
      }
      (TerI->curvel).x = dotp * slide.x * TerrShape->size + (TerrShape->offset).x;
      (TerI->curvel).z = dotp * slide.z * TerrShape->size + (TerrShape->offset).z;
    }
    do {
      DerotateMovementVector();
      HitTerrain(vpos);
      TerrainImpactNorm();
      vpos = &temp;
      TerrainImpact(vpos,&temp,tflags);
      TerrShapeAdjCnt = TerrShapeAdjCnt + -1;
      if (TerrShapeAdjCnt < 1) break;
    } while (TerI->hittype != 0);
    if (TerI->hittype == 0) {
      (TerI->curpos).x = (TerI->curpos).x + (TerI->curvel).x;
      (TerI->curpos).y = (TerI->curpos).y + (TerI->curvel).y;
      (TerI->curpos).z = (TerI->curpos).z + (TerI->curvel).z;
    }
    (TerrShape->offset).x = (TerrShape->offset).x + ((TerI->origpos).x - (TerI->curpos).x);
    (TerrShape->offset).z = (TerrShape->offset).z + ((TerI->origpos).z - (TerI->curpos).z);
    TerI->hittype = 1;
    (TerI->curvel).x = (TerI->origvel).x;
    (TerI->curvel).y = (TerI->origvel).y;
    (TerI->curvel).z = (TerI->origvel).z;
    TerrainSideClamp(&slide,&TerI->curpos);
    doslide = 0;
  }
  return doslide;
}


void CubeImpact(numtx_s *mat,numtx_s *nmat,nuvec_s *norm,float size,nuvec_s *impact)
{
	//TODO
}



void NewTerrainScaleY(nuvec_s *vpos,nuvec_s *vvel,uchar *flags,int terid,float stopflag,float siz e, float yscale) //Need Correction!!
{
  short sVar1;
  nuvec_s *v;
  double dVar2;
  short sStack_22;
  nuvec_s local_14;
  TerrI *Terr;
  float tmp;
  
  if (CurTerr != (TempTerr *)0x0) {
    plathitid = -1;
    TerrPolyObj = -1;
    TerrPoly = (hitdata *)0x0;
    PlatCrush = 0;
    terrhitflags = 0;
    CurTrackInfo = ScanTerrId(flags);
    TerI = (TerrI *)NuScratchAlloc32(0x930);
    TerI->yscale = yscale;
    TerI->yscalesq = TerI->yscale * TerI->yscale;
    TerI->inyscale = 1.0 / yscale;
    TerI->inyscalesq = TerI->inyscale * TerI->inyscale;
    TerI->size = size;
    TerI->sizediv = 1.0 / TerI->size;
    TerI->sizesq = TerI->size * TerI->size;
    Terr = TerI;
    tmp = vpos->x;
    (TerI->curpos).x = tmp;
    (Terr->origpos).x = tmp;
    Terr = TerI;
    tmp = TerI->size * yscale + vpos->y;
    (TerI->curpos).y = tmp;
    (Terr->origpos).y = tmp;
    Terr = TerI;
    tmp = vpos->z;
    (TerI->curpos).z = tmp;
    (Terr->origpos).z = tmp;
    Terr = TerI;
    tmp = vvel->x;
    (TerI->curvel).x = tmp;
    (Terr->origvel).x = tmp;
    Terr = TerI;
    tmp = vvel->y;
    (TerI->curvel).y = tmp;
    (Terr->origvel).y = tmp;
    Terr = TerI;
    tmp = vvel->z;
    (TerI->curvel).z = tmp;
    (Terr->origvel).z = tmp;
    sStack_22 = (short)terid;
    TerI->id = sStack_22;
    TerI->stopflag = stopflag;
    TerI->flags = flags;
    TerI->scanmode = 0;
    TerI->timeadj = 0.01;
    TerI->impactadj = 1e-05;
    ScanTerrain(1,0);
    if (flags[1] != '\0') {
      dVar2 = (double)vvel->x;
      NuFabs(vvel->x);
      if (dVar2 < (double)stopflag) {
        dVar2 = (double)vvel->y;
        NuFabs(vvel->y);
        if (dVar2 < (double)stopflag) {
          dVar2 = (double)vvel->z;
          NuFabs(vvel->z);
          if ((dVar2 < (double)stopflag) && (platinrange == 0)) {
            NuScratchRelease();
            TerrFlush();
            return;
          }
        }
      }
    }
    (TerI->curpos).y = (TerI->curpos).y * TerI->inyscale;
    (TerI->curvel).y = (TerI->curvel).y * TerI->inyscale;
    *flags = '\0';
    flags[1] = '\0';
    v = (nuvec_s *)PlatformChecks(4,vvel);
    local_14.x = (float)v;
    do {
      DerotateMovementVector();
      v = (nuvec_s *)HitTerrain(v);
      StorePlatImpact();
      if (((TerI->hittype < 0x11) || (TerI->hitterrno == -1)) ||
         (CurTerr->terr[TerI->hitterrno].type != TERR_TYPE_PLATFORM)) {
        local_14.y = 1.401298e-45;
      }
      else {
        local_14.x = (float)((int)local_14.x + -1);
        v = (nuvec_s *)TerrainPlatformEmbedded(vvel);
        local_14.y = (float)v;
      }
      if (local_14.y != 0.0) {
        local_14.x = (float)((int)local_14.x + -1);
        TerrainImpactNorm();
        if (TerI->hittype != 0) {
          ShadNorm.x = (TerI->uhitnorm).x;
          ShadNorm.y = (TerI->uhitnorm).y;
          ShadNorm.z = (TerI->uhitnorm).z;
          if (((0.707 < (TerI->uhitnorm).y) && (-1 < TerI->hitterrno)) &&
             (CurTerr->terr[TerI->hitterrno].type == TERR_TYPE_PLATFORM)) {
            sVar1 = CurTerr->terr[TerI->hitterrno].info;
            CurTerr->platdat[sVar1].status =
                 (platattrib)((uint)CurTerr->platdat[sVar1].status | 0x40000000);
          }
        }
        if (TerrShapeAdjCnt == 0) {
          v = vpos;
          TerrainImpact(vpos,vvel,flags);
        }
        else {
          v = (nuvec_s *)TerrShapeSideStep(vpos,vvel,flags);
          if (v != (nuvec_s *)0x0) {
            v = vpos;
            TerrainImpact(vpos,vvel,flags);
          }
        }
      }
    } while (((TerI->hittype != 0) && (0 < (int)local_14.x)) &&
            ((TerI->hitnorm).x * (TerI->hitnorm).x + (TerI->hitnorm).y * (TerI->hitnorm).y +
             (TerI->hitnorm).z * (TerI->hitnorm).z <= 1.5));
    if (TerI->hittype != 0) {
      vpos->x = (TerI->curpos).x;
      vpos->y = (TerI->curpos).y * TerI->yscale - TerI->size * TerI->yscale;
      vpos->z = (TerI->curpos).z;
    }
    if (testlock != 0) {
      vpos->x = (TerI->origpos).x;
      vpos->z = (TerI->origpos).z;
    }
    NuScratchRelease();
    TerrFlush();
  }
  return;
}

//NGC MATCH
void RayImpact(struct nuvec_s *vvel) 
{
    TerrainMoveImpactData();
    
    switch (TerI->hittype) 
    {
    case 0:
        break;
    case 1:
    case 2:
    case 3:
    case 4:
        TerI->hittime -= TerI->timeadj;
        
        if (TerI->hittime < 0.0f) 
        {
            TerI->hittime = 0.0f;
        }
        
        vvel->x = TerI->curvel.x * TerI->hittime;
        vvel->y = TerI->curvel.y * TerI->hittime;
        vvel->z = TerI->curvel.z * TerI->hittime;
        break;
    case 17:
    case 18:
    case 19:
    case 20:
        vvel->x = 0.0f;
        vvel->y = 0.0f;
        vvel->z = 0.0f;
        break;
    }
}

//NGC MATCH
s32 NewRayCast(struct nuvec_s *vpos,struct nuvec_s *vvel,float size) {
  s32 hit;
  
  if (CurTerr == NULL) {
    return 0;
  }
    plathitid = -1;
    TerrPolyObj = -1;
    TerrPoly = NULL;
    TerI = (struct teri_s *)NuScratchAlloc32(0x930);
    TerI->inyscalesq = TerI->inyscale = TerI->yscalesq = TerI->yscale = 1.0f;
    TerI->size = size;
    TerI->sizediv = 1.0f / TerI->size;
    TerI->sizesq = TerI->size * TerI->size;
    (TerI->origpos).x = (TerI->curpos).x = vpos->x;
    (TerI->origpos).y = (TerI->curpos).y = vpos->y;
    (TerI->origpos).z = (TerI->curpos).z = vpos->z;
    (TerI->origvel) = (TerI->curvel) = *vvel;
    TerI->id = -1;
    TerI->flags = NULL;
    TerI->scanmode = 1;
    TerI->timeadj = 0.01f;
    TerI->impactadj = 1e-05f;
    ScanTerrain(0,0);
    DerotateMovementVector();
    HitTerrain();
    hit = 0;
    if (TerI->hittype != 0) {
      hit = 1;
      RayImpact(vvel);
      TerrainImpactNorm();
      ShadNorm = (TerI->hitnorm);
    }
    NuScratchRelease();
  return (s32)TerI->hittype;
}
  
//NGC MATCH
s32 NewRayCastMask(struct nuvec_s *vpos,struct nuvec_s *vvel,float size,s32 mask) {
  s32 hit;
  
  if (CurTerr == NULL) {
    return 0;
  }
    plathitid = -1;
    TerrPolyObj = -1;
    TerrPoly = NULL;
    TerI = (struct teri_s* *)NuScratchAlloc32(0x930);
    TerI->inyscalesq = TerI->inyscale = TerI->yscalesq = TerI->yscale = 1.0f;
    TerI->size = size;
    TerI->sizediv = 1.0f / TerI->size;
    TerI->sizesq = TerI->size * TerI->size;
    (TerI->origpos).x = (TerI->curpos).x = vpos->x;
    (TerI->origpos).y = (TerI->curpos).y = vpos->y;
    (TerI->origpos).z = (TerI->curpos).z = vpos->z;
    (TerI->origvel) = (TerI->curvel) = *vvel;
    TerI->id = -1;
    TerI->flags = NULL;
    TerI->scanmode = 1;
    TerI->timeadj = 0.01f;
    TerI->impactadj = 1e-05f;
    ScanTerrain(0,mask);
    DerotateMovementVector();
    HitTerrain();
    hit = 0;
    if (TerI->hittype != 0) {
      hit = 1;
      RayImpact(vvel);
      TerrainImpactNorm();
      ShadNorm = (TerI->hitnorm);
    }
    NuScratchRelease();
  return (s32)TerI->hittype;
}
  
//NGC MATCH
s32 NewPlatInst(struct numtx_s *mat,s32 instance) {
  s32 orig;
  s32 lp;
  s32 platid;
  
  if (terlistptr >= 0x1000) {
    return -1;
  }
    if (situtotal >= 0x800) {
        return -1;
    }
      if (mat == NULL) {
        return -1;
      }
        platid = -1;
        for (lp = 0; lp < 0x80; lp++) {
          if (CurTerr->platdata[lp].curmtx == NULL) {
            platid = lp;
            break;
          }
        }
        if (platid == -1) {
          return -1;
        }
          orig = -1;
          for (lp = 0; lp < 0x80; lp++) {
            if ((CurTerr->platdata[lp].curmtx != NULL) &&
               (CurTerr->platdata[lp].instance == instance)) {
              orig = lp;
              break;
            }
          }
          if (orig == -1) {
            return -1;
          }
            CurTerr->terr[situtotal] =  CurTerr->terr[CurTerr->platdata[orig].terrno];
            CurTerr->terr[situtotal].info = platid;
            CurTerr->terr[situtotal].type = TERR_TYPE_PLATFORM;
            CurTerr->platdata[platid].curmtx = mat;
            CurTerr->platdata[platid].terrno = situtotal;
            CurTerr->platdata[platid].instance = instance;
            CurTerr->platdata[platid].status.rotate = 0;
            CurTerr->terrlist[terlistptr++] = situtotal;
            CurTerr->platdata[platid].plrgrav = 0.0f;
            CurTerr->platdata[platid].ypos = 0.0f;
            CurTerr->platdata[platid].yvel = 0.0f;
            CurTerr->platdata[platid].damp = 0.0f;
            CurTerr->platdata[platid].tension = 0.0f;
            situtotal++;
            CurTerr->terrgroup[0x100].count++;
  return platid;
}
  
//NGC MATCH
s32 FindPlatInst(s32 instance) {
  s32 orig;
  s32 lp;
  
  if (instance == -1) {
    return -1;
  }
    orig = -1;
    for (lp = 0; lp < 0x80; lp = lp + 1) {
      if ((CurTerr->platdata[lp].curmtx != NULL) && (CurTerr->platdata[lp].instance == instance)) {
        orig = lp;
        break;
      }
    }
    if (orig == -1) {
      return -1;
    }
  return orig;
}
  
//NGC MATCH
void PlatInstRotate(int platid,int state) {
  CurTerr->platdata[platid].status.rotate = state;
}
  
//NGC MATCH
void PlatInstBounce(s32 platid,float plrgrav,float tension,float damp) {
  if ((platid >= 0) && (platid < 0x80)) {
    if (CurTerr->platdata[platid].plrgrav == 0.0f) {
      CurTerr->platdata[platid].ypos = 0.0f;
      CurTerr->platdata[platid].yvel = 0.0f;
    }
    CurTerr->platdata[platid].plrgrav = plrgrav;
    CurTerr->platdata[platid].tension = tension;
    CurTerr->platdata[platid].damp = damp;
  }
}
  
//NGC MATCH
s32 PlatInstGetHit(s32 platid) {
  if ((platid >= 0) && (platid < 0x80)) {
    return CurTerr->platdata[platid].status.hit;
  } else {
     return 0;
  }
}
  
//NGC MATCH
void UpdatePlatinst(s32 platid,struct numtx_s *mat) {
  if ((platid >= 0) && (platid < 0x80)) {
    CurTerr->platdata[platid].curmtx = mat;
  }
}
  
//NGC MATCH
s32 PlatImpactInfo(struct nuvec_s *norm,s32 *info,s32 *extra) {
  s32 t;
  
  if (PlatImpactId != 0) {
    *norm = PlatImpactNorm;
    *info = (uint)PlatImpactTer.info[0];
    *extra = (uint)PlatImpactTer.info[1];
  }
  t = PlatImpactId;
  PlatImpactId = 0;
  return t;
}

//NGC MATCH
s32 PlatformCrush(void) {
  return PlatCrush;
}

//NGC MATCH
void NewScanInit(void) {
  TempStackPtr = TempScanStack;
  TerrPlatDis = -1;
}

short * NewScanHandelFull(nuvec_s *vpos,nuvec_s *vvel,float size,int platscan,int extramask)

{
	//TODO
	
}

short * NewScanHandelSubset(short *handel,nuvec_s *vpos,nuvec_s *vvel,float size,int extramask)

{
	//TODO
	
}

//NGC MATCH
short* NewScanHandel(struct nuvec_s *vpos,struct nuvec_s *vvel,float size,s32 plats,short *handel) {
    if (CurTerr == NULL) {
       return NULL;
    }
    TerrPlatDis = -1;
    if (handel == NULL) {
      return NewScanHandelFull(vpos,vvel,size,plats,0);
    }
    else {
      return NewScanHandelSubset(handel,vpos,vvel,size,0);
    }
}

void ScanTerrainHandel(int extramask,short *Handel)

{
	//TODO
}

//NGC MATCH
s32 NewRayCastSetHandel(struct nuvec_s *vpos,struct nuvec_s *vvel,float size,float timeadj,float impactadj,short *Handel) {
  s32 hit;
  
  if (CurTerr == NULL) {
    return 0;
  }
  if (Handel == NULL) {
    return 0;
  }
    plathitid = -1;
    TerrPolyObj = -1;
    TerrPoly = NULL;
    TerI = (struct teri_s *)NuScratchAlloc32(0x930);
    TerI->inyscalesq = TerI->inyscale = TerI->yscalesq = TerI->yscale = 1.0f;
    TerI->size = size;
    TerI->sizediv = 1.0f / TerI->size;
    TerI->sizesq = TerI->size * TerI->size;
    (TerI->origpos).x = (TerI->curpos).x = vpos->x;
    (TerI->origpos).y = (TerI->curpos).y = vpos->y;
    (TerI->origpos).z = (TerI->curpos).z = vpos->z;
    (TerI->origvel) = (TerI->curvel) = *vvel;
    TerI->id = -1;
    TerI->flags = NULL;
    TerI->scanmode = 1;
    TerI->timeadj = timeadj;
    TerI->impactadj = impactadj;
    if (Handel == NULL) {
      ScanTerrain(1,1);
    }
    else {
      ScanTerrainHandel(0,Handel);
    }
    DerotateMovementVector();
    HitTerrain();
    hit = 0;
    if (TerI->hittype != 0) {
      hit = 1;
      RayImpact(vvel);
      TerrainImpactNorm();
      ShadNorm = (TerI->hitnorm);
    }
    NuScratchRelease();
    return TerI->hittype;
}

//NGC MATCH
s32 TerrainPlatId(void) {
  return plathitid;
}

//NGC MATCH
void TerrainPlatGetMtx(s32 platid,struct numtx_s **old,struct numtx_s **cur) {
  if (platid > -1) {
    *old = &CurTerr->platdata[platid].oldmtx;
    *cur = CurTerr->platdata[platid].curmtx;
  }
}


void ScanTerrainPlatform(int msituid,int extramask)

{
	//TODO
}

//NGC MATCH
s32 NewRayCastPlatForm(struct nuvec_s *vpos,struct nuvec_s *vvel,float size,float timeadj,s32 platformid) {
  s32 hit;
  
  if (CurTerr == NULL) {
    return 0;
  }
    plathitid = -1;
    TerrPolyObj = -1;
    TerrPoly = NULL;
    TerI = (struct teri_s *)NuScratchAlloc32(0x930);
    TerI->inyscalesq = TerI->inyscale = TerI->yscalesq = TerI->yscale = 1.0f;
    TerI->size = size;
    TerI->sizediv = 1.0f / TerI->size;
    TerI->sizesq = TerI->size * TerI->size;
    (TerI->origpos).x = (TerI->curpos).x = vpos->x;
    (TerI->origpos).y = (TerI->curpos).y = vpos->y;
    (TerI->origpos).z = (TerI->curpos).z = vpos->z;
    (TerI->origvel) = (TerI->curvel) = *vvel;
    TerI->id = -1;
    TerI->flags = NULL;
    TerI->scanmode = 1;
    TerI->timeadj = timeadj;
    TerI->impactadj = 0.0f;
    ScanTerrainPlatform(CurTerr->platdata[platformid].terrno,0);
    DerotateMovementVector();
    HitTerrain();
    hit = 0;
    if (TerI->hittype != 0) {
      hit = 1;
      RayImpact(vvel);
      TerrainImpactNorm();
      ShadNorm = (TerI->hitnorm);
    }
    NuScratchRelease();
  return TerI->hittype;
}