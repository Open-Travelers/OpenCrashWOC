#include "../nu.h"





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


void TerrFlush(void)

{
  curSphereter = 0;
  TerrShapeAdjCnt = 0;
  curPickInst = 0;
  return;
}


void noterraininit(void)

{
  terraincnt = 0;
  curSphereter = 0;
  platinrange = 0;
  ShadPoly = (Poly *)0x0;
  CurTerr = (TempTerr *)0x0;
  TerrFlush();
  return;
}

void TerrainSetCur(void *curterr)

{
  CurTerr = (TempTerr *)curterr;
  return;
}


void DerotateMovementVector(void)		//need correction

{
  uint uVar1;
  double dVar2;
  float fVar3;
  
  uVar1 = NuAtan2D((TerI->curvel).x,(TerI->curvel).z);
  TerI->ay = (float)((double)CONCAT44(0x43300000,uVar1 ^ 0x80000000) - 4503601774854144.0);
  dVar2 = -(double)(TerI->curvel).y;
  fVar3 = NuFsqrt((TerI->curvel).x * (TerI->curvel).x + (TerI->curvel).z * (TerI->curvel).z);
  uVar1 = NuAtan2D((float)dVar2,fVar3);
  TerI->ax = (float)((double)CONCAT44(0x43300000,uVar1 ^ 0x80000000) - 4503601774854144.0);
  fVar3 = NuFsqrt((TerI->curvel).x * (TerI->curvel).x + (TerI->curvel).y * (TerI->curvel).y +
                  (TerI->curvel).z * (TerI->curvel).z);
  TerI->len = fVar3;
  return;
}


void RotateVec(nuvec_s *in,nuvec_s *out)

{
  float tz;
  
  tz = in->y * NuTrigTable[(int)TerI->ax & 0xffff] +
       in->z * NuTrigTable[(int)(TerI->ax + 16384.0) & 0xffff];
  out->y = in->y * NuTrigTable[(int)(TerI->ax + 16384.0) & 0xffff] -
           in->z * NuTrigTable[(int)TerI->ax & 0xffff];
  out->z = tz * NuTrigTable[(int)(TerI->ay + 16384.0) & 0xffff] -
           in->x * NuTrigTable[(int)TerI->ay & 0xffff];
  out->x = tz * NuTrigTable[(int)TerI->ay & 0xffff] +
           in->x * NuTrigTable[(int)(TerI->ay + 16384.0) & 0xffff];
  return;
}


void DeRotateTerrain(hitdata *ter)

{
  float cosax;
  float cosay;
  float sinax;
  float sinay;
  float tz;
  
  sinax = NuTrigTable[(int)-TerI->ax & 0xffff];
  cosax = NuTrigTable[(int)(-TerI->ax + 16384.0) & 0xffff];
  sinay = NuTrigTable[(int)-TerI->ay & 0xffff];
  cosay = NuTrigTable[(int)(-TerI->ay + 16384.0) & 0xffff];
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
  if (ter->norm[1].y < 65536.0) {
    tz = (ter->pnts[3].z - TerI->csz) * cosay - (ter->pnts[3].x - TerI->csx) * sinay;
    (TerI->rotter).pnts[3].x =
         (ter->pnts[3].z - TerI->csz) * sinay + (ter->pnts[3].x - TerI->csx) * cosay;
    (TerI->rotter).pnts[3].y = (ter->pnts[3].y - TerI->csy) * cosax - tz * sinax;
    (TerI->rotter).pnts[3].z = (ter->pnts[3].y - TerI->csy) * sinax + tz * cosax;
  }
  return;
}


void DeRotatePoint(nuvec_s *pnt)

{
  float fVar1;
  float fVar2;
  float fVar3;
  
  fVar1 = NuTrigTable[(int)-TerI->ax & 0xffff];
  fVar2 = NuTrigTable[(int)(-TerI->ax + 16384.0) & 0xffff];
  fVar3 = (pnt->z - (TerI->curpos).z) * NuTrigTable[(int)(-TerI->ay + 16384.0) & 0xffff] -
          (pnt->x - (TerI->curpos).x) * NuTrigTable[(int)-TerI->ay & 0xffff];
  pnt->x = (pnt->z - (TerI->curpos).z) * NuTrigTable[(int)-TerI->ay & 0xffff] +
           (pnt->x - (TerI->curpos).x) * NuTrigTable[(int)(-TerI->ay + 16384.0) & 0xffff];
  pnt->z = ((pnt->y + TerI->size) - (TerI->curpos).y) * fVar1 + fVar3 * fVar2;
  pnt->y = ((pnt->y + TerI->size) - (TerI->curpos).y) * fVar2 - fVar3 * fVar1;
  return;
}


short InsideLineF(float _x,float _z,float _x0,float _z0,float _x1,float _z1)

{
  return (short)(0.0 <= (_x - _x0) * (_z1 - _z0) + (_z - _z0) * (_x0 - _x1));
}

short InsidePolLines(float x, float y, float z, float x0, float y0, float z0, float x1, float y1, float z1, nuvec_s* norm)
{
	//TODO
}


TrackInfo * ScanTerrId(void *id)
{
  int c;
  
  c = 0;
  while( true ) {
    if (3 < c) {
      return (TrackInfo *)0x0;
    }
    if (CurTerr->Trackinfo[c].ptrid == id) break;
    c = c + 1;
  }
  return CurTerr->Trackinfo + c;
}

TrackInfo * AllocTerrId(void)

{
  int c;
  
  c = 0;
  while( true ) {
    if (3 < c) {
      return (TrackInfo *)0x0;
    }
    if (CurTerr->Trackinfo[c].ptrid == (void *)0x0) break;
    c = c + 1;
  }
  return CurTerr->Trackinfo + c;
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


int CheckSphere(int p)

{
  int ret;
  float fVar1;
  float fVar2;
  
  if (((TerI->rotter).pnts[p].z < -TerI->size) ||
     (TerI->size + TerI->len < (TerI->rotter).pnts[p].z)) {
    ret = 0;
  }
  else {
    fVar2 = (TerI->rotter).pnts[p].x * (TerI->rotter).pnts[p].x +
            (TerI->rotter).pnts[p].y * (TerI->rotter).pnts[p].y;
    if (fVar2 <= TerI->sizesq) {
      fVar1 = NuFsqrt(TerI->sizesq - fVar2);
      if (((TerI->rotter).pnts[p].z - fVar1 < 0.0) || (TerI->len < (TerI->rotter).pnts[p].z - fVar 1)
         ) {
        fVar2 = fVar2 + (TerI->rotter).pnts[p].z * (TerI->rotter).pnts[p].z;
        if (TerI->sizesq <= fVar2) {
          ret = 0;
        }
        else {
          fVar2 = NuFsqrt(fVar2);
          fVar2 = 1.0 / fVar2;
          TerI->hittype = 0x13;
          TerI->hittime = 0.0;
          (TerI->hitnorm).x = -(TerI->rotter).pnts[p].x * fVar2;
          (TerI->hitnorm).y = -(TerI->rotter).pnts[p].y * fVar2;
          (TerI->hitnorm).z = -(TerI->rotter).pnts[p].z * fVar2;
          ret = 1;
        }
      }
      else {
        fVar2 = ((TerI->rotter).pnts[p].z - fVar1) / TerI->len;
        if (TerI->hittime <= fVar2) {
          ret = 0;
        }
        else {
          TerI->hittype = 3;
          TerI->hittime = fVar2;
          (TerI->hitnorm).x = -(TerI->rotter).pnts[p].x;
          (TerI->hitnorm).y = -(TerI->rotter).pnts[p].y;
          (TerI->hitnorm).z = -fVar1;
          ret = 1;
        }
      }
    }
    else {
      ret = 0;
    }
  }
  return ret;
}

int CheckSphereTer(nuvec_s *pnt,float radius)

{
  float fVar1;
  int ret;
  float fVar2;
  float fVar3;
  
  if ((pnt->z < -TerI->size - radius) || (TerI->size + radius + TerI->len < pnt->z)) {
    ret = 0;
  }
  else {
    fVar3 = pnt->x * pnt->x + pnt->y * pnt->y;
    fVar1 = (TerI->size + radius) * (TerI->size + radius);
    if (fVar3 <= fVar1) {
      fVar2 = NuFsqrt(fVar1 - fVar3);
      if ((pnt->z - fVar2 < 0.0) || (TerI->len < pnt->z - fVar2)) {
        fVar3 = fVar3 + pnt->z * pnt->z;
        if (fVar1 <= fVar3) {
          ret = 0;
        }
        else {
          fVar3 = NuFsqrt(fVar3);
          fVar3 = 1.0 / fVar3;
          TerI->hittype = 0x14;
          TerI->hittime = 0.0;
          (TerI->hitnorm).x = -pnt->x * fVar3;
          (TerI->hitnorm).y = -pnt->y * fVar3;
          (TerI->hitnorm).z = -pnt->z * fVar3;
          ret = 1;
        }
      }
      else {
        fVar3 = (pnt->z - fVar2) / TerI->len;
        if (TerI->hittime <= fVar3) {
          ret = 0;
        }
        else {
          TerI->hittype = 4;
          TerI->hittime = fVar3;
          (TerI->hitnorm).x = -pnt->x;
          (TerI->hitnorm).y = -pnt->y;
          (TerI->hitnorm).z = -fVar2;
          ret = 1;
        }
      }
    }
    else {
      ret = 0;
    }
  }
  return ret;
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


void ScanTerrain(int platscan,int extramask)
{
	//TODO
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

int FullDeflectTest(nuvec_s *N,nuvec_s *L,nuvec_s *R)

{
  float dotp;
  
  dotp = -L->x * N->x + -L->y * N->y + -L->z * N->z + 0.001;
  NuFsqrt(L->x * L->x + L->y * L->y + L->z * L->z);
  R->x = N->x * dotp + L->x;
  R->y = N->y * dotp + L->y;
  R->z = N->z * dotp + L->z;
  return (uint)(0.0 < dotp);
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


void TerrainMoveImpactData(void)		//need corrections

{
  hitdata *hit;
  hitdata *TPolyInf;
  int i;
  short type;
  
  type = TerI->hittype;
  if (type < 5) {
    if (type < 1) {
      return;
    }
  }
  else {
    if (0x14 < type) {
      return;
    }
    if (type < 0x11) {
      return;
    }
  }
  if (TerI->hitterrno != -1) {
    hit = TerI->hitter;
    TPolyInf = &TerrPolyInfo;
    i = 0x60;
    do {
      i = i + -0x18;
      TPolyInf->minx = hit->minx;
      TPolyInf->maxx = hit->maxx;
      TPolyInf->miny = hit->miny;
      TPolyInf->maxy = hit->maxy;
      TPolyInf->minz = hit->minz;
      TPolyInf->maxz = hit->maxz;
      hit = (hitdata *)hit->pnts;
      TPolyInf = (hitdata *)TPolyInf->pnts;
    } while (i != 0);
    ((nuvec_s *)TPolyInf)->x = ((nuvec_s *)hit)->x;
    TerrPoly = &TerrPolyInfo;
    TerrPolyObj = (int)TerI->hitterrno;
  }
  return;
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

void PlatformConnect(char *flags,nuvec_s *vel,nuvec_s *ivel,int platid)

{
  short id;
  
  id = (short)platid;
  if (CurTrackInfo == (TrackInfo *)0x0) {
    CurTrackInfo = AllocTerrId();
    if (CurTrackInfo != (TrackInfo *)0x0) {
      CurTrackInfo->ptrid = flags;
      CurTrackInfo->platinf = CurTrackInfo->platinf | 1;
      CurTrackInfo->platid = id;
      CurTrackInfo->timer = 4;
      vel->y = 0.0;
      ivel->y = 0.0;
    }
  }
  else {
    CurTrackInfo->platinf = CurTrackInfo->platinf | 1;
    CurTrackInfo->platid = id;
    CurTrackInfo->timer = 4;
  }
  return;
}


void TerrainImpact(nuvec_s *vpos,nuvec_s *vvel,uchar *flags)

{
  short type;
  
  TerrainMoveImpactData();
  type = TerI->hittype;
  if (type < 5) {
    if (type < 1) {
      if (type == 0) {
        *flags = '\0';
        vpos->x = (TerI->curpos).x + (TerI->curvel).x;
        vpos->y = ((TerI->curpos).y + (TerI->curvel).y) * TerI->yscale - TerI->size * TerI->yscale ;
        vpos->z = (TerI->curpos).z + (TerI->curvel).z;
      }
    }
    else {
      TerI->hittime = TerI->hittime - TerI->timeadj;
      if (TerI->hittime < 0.0) {
        TerI->hittime = 0.0;
      }
      (TerI->curpos).x = (TerI->curpos).x + (TerI->curvel).x * TerI->hittime;
      (TerI->curpos).y = (TerI->curpos).y + (TerI->curvel).y * TerI->hittime;
      (TerI->curpos).z = (TerI->curpos).z + (TerI->curvel).z * TerI->hittime;
      (TerI->curvel).x = (TerI->curvel).x - (TerI->curvel).x * TerI->hittime;
      (TerI->curvel).y = (TerI->curvel).y - (TerI->curvel).y * TerI->hittime;
      (TerI->curvel).z = (TerI->curvel).z - (TerI->curvel).z * TerI->hittime;
      FullDeflect(&TerI->hitnorm,&TerI->curvel,&TerI->curvel);
      FullDeflect(&TerI->uhitnorm,vvel,vvel);
      if (0.707 <= (TerI->uhitnorm).y) {
        terrhitflags = terrhitflags | 2;
        *flags = '\x01';
        flags[1] = '\x01';
        (TerI->curpos).y = (TerI->curpos).y + (TerI->hitnorm).y * 0.001;
      }
      else {
        *flags = '\0';
        terrhitflags = terrhitflags | 1;
        (TerI->curpos).x = (TerI->curpos).x + (TerI->hitnorm).x * 0.002;
        (TerI->curpos).z = (TerI->curpos).z + (TerI->hitnorm).z * 0.002;
        (TerI->curvel).x = (TerI->curvel).x + (TerI->hitnorm).x * 0.003;
        (TerI->curvel).z = (TerI->curvel).z + (TerI->hitnorm).z * 0.003;
        vvel->x = vvel->x + (TerI->hitnorm).x * 0.003;
        vvel->z = vvel->z + (TerI->hitnorm).z * 0.003;
      }
    }
  }
  else if ((type < 0x15) && (0x10 < type)) {
    FullDeflect(&TerI->hitnorm,&TerI->curvel,&TerI->curvel);
    FullDeflect(&TerI->uhitnorm,vvel,vvel);
    if (0.707 <= (TerI->uhitnorm).y) {
      (TerI->curpos).y = (TerI->curpos).y + (TerI->hitnorm).y * 0.005;
      (TerI->curvel).y = (TerI->curvel).y + (TerI->hitnorm).y * 0.01;
    }
    else {
      (TerI->curpos).x = (TerI->curpos).x + (TerI->hitnorm).x * 0.005;
      (TerI->curpos).y = (TerI->curpos).y + (TerI->hitnorm).y * 0.002;
      (TerI->curpos).z = (TerI->curpos).z + (TerI->hitnorm).z * 0.005;
      (TerI->curvel).x = (TerI->curvel).x + (TerI->hitnorm).x * 0.005;
      (TerI->curvel).y = (TerI->curvel).y + (TerI->hitnorm).y * 0.002;
      (TerI->curvel).z = (TerI->curvel).z + (TerI->hitnorm).z * 0.005;
      *flags = '\x01';
    }
  }
  if ((((TerI->hittype != 0) && (TerI->hittype < 0x10)) && (TerI->hitterrno != -1)) &&
     ((CurTerr->terr[TerI->hitterrno].type == TERR_TYPE_PLATFORM && (0.707 < (TerI->uhitnorm).y))) )
  {
    PlatformConnect((char *)TerI->flags,&TerI->curvel,vvel,(int)CurTerr->terr[TerI->hitterrno].in fo)
    ;
  }
  return;
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


int TerrainPlatformMoveCheck (nuvec_s *dest,nuvec_s *norm,int platid,int calcimpact,int leaveoffonfail) //need corrections
{
  ushort uVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  nuvec_s *hitnorm;
  float fVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  float fVar11;
  float fVar12;
  uint uVar13;
  short *scanStart;
  bool b;
  short cnt;
  short hittype;
  short plathit;
  TerrI *ptrTI;
  short terrno;
  short terrnoI;
  float time;
  
  terrno = CurTerr->platdat[platid].terrno;
  for (scanStart = TerI->platScanStart; 0 < *scanStart;
      scanStart = scanStart + ((uVar13 ^ (int)*scanStart) - uVar13) * 2 + 2) {
    if (scanStart[1] == terrno) {
      uVar1 = *scanStart >> 0xf;
      *scanStart = -((uVar1 ^ *scanStart) - uVar1);
    }
    uVar13 = (int)*scanStart >> 0x1f;
  }
  fVar2 = (TerI->curvel).x;
  fVar9 = (TerI->curvel).y;
  fVar5 = (TerI->curvel).z;
  cnt = TerI->hitcnt;
  terrnoI = TerI->hitterrno;
  hittype = TerI->hittype;
  plathit = TerI->plathit;
  time = TerI->hittime;
  fVar3 = (TerI->hitnorm).x;
  fVar10 = (TerI->hitnorm).y;
  fVar6 = (TerI->hitnorm).z;
  fVar4 = (TerI->uhitnorm).x;
  fVar11 = (TerI->uhitnorm).y;
  fVar7 = (TerI->uhitnorm).z;
  (TerI->curvel).x = dest->x - (TerI->curpos).x;
  (TerI->curvel).y = dest->y - (TerI->curpos).y;
  (TerI->curvel).z = dest->z - (TerI->curpos).z;
  hitnorm = dest;
  DerotateMovementVector();
  HitTerrain(hitnorm);
  if ((calcimpact != 0) && (TerI->hittype != 0)) {
    TerrainImpactNorm();
    TerrainImpactPlatform(TerI->flags);
    if (norm != (nuvec_s *)0x0) {
      fVar12 = (TerI->hitnorm).y;
      fVar8 = (TerI->hitnorm).z;
      norm->x = (TerI->hitnorm).x;
      norm->y = fVar12;
      norm->z = fVar8;
    }
    fVar12 = (TerI->curpos).y;
    fVar8 = (TerI->curpos).z;
    dest->x = (TerI->curpos).x;
    dest->y = fVar12;
    dest->z = fVar8;
  }
  ptrTI = TerI;
  (TerI->curvel).x = fVar2;
  (ptrTI->curvel).y = fVar9;
  (ptrTI->curvel).z = fVar5;
  TerI->hitcnt = cnt;
  TerI->hitterrno = terrnoI;
  TerI->plathit = plathit;
  TerI->hittime = time;
  ptrTI = TerI;
  (TerI->hitnorm).x = fVar3;
  (ptrTI->hitnorm).y = fVar10;
  (ptrTI->hitnorm).z = fVar6;
  ptrTI = TerI;
  (TerI->uhitnorm).x = fVar4;
  (ptrTI->uhitnorm).y = fVar11;
  (ptrTI->uhitnorm).z = fVar7;
  scanStart = TerI->platScanStart;
  if ((TerI->hittype < 0x10) || (leaveoffonfail == 0)) {
    for (; *scanStart != 0; scanStart = scanStart + ((uVar13 ^ (int)*scanStart) - uVar13) * 2 + 2)  {
      if (scanStart[1] == terrno) {
        uVar1 = *scanStart >> 0xf;
        *scanStart = (uVar1 ^ *scanStart) - uVar1;
      }
      uVar13 = (int)*scanStart >> 0x1f;
    }
  }
  b = TerI->hittype == 0;
  if (b) {
    TerI->hittype = hittype;
  }
  else {
    TerI->hittype = hittype;
  }
  return (uint)b;
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


int ShadowInfo(void)

{
  int info;
  
  if (ShadPoly == (Poly *)0x0) {
    info = -1;
  }
  else {
    info = (int)ShadPoly->info[0];
  }
  return info;
}

void ShadowDir(nuvec_s *dir)

{
  dir->x = ShadPoly->pnts[1].x - ShadPoly->pnts[0].x;
  dir->y = ShadPoly->pnts[1].y - ShadPoly->pnts[0].y;
  dir->z = ShadPoly->pnts[1].z - ShadPoly->pnts[0].z;
  return;
}


int EShadowInfo(void)

{
  int ret;
  
  if (EShadPoly == (hitdata *)0x0) {
    ret = -1;
  }
  else {
    ret = (int)EShadPoly->info[1];
  }
  return ret;
}


int ShadowRoofInfo(void)

{
  int ret;
  
  if (ShadRoofPoly == (hitdata *)0x0) {
    ret = -1;
  }
  else {
    ret = (int)ShadRoofPoly->info[0];
  }
  return ret;
}

float NewShadow(nuvec_s *ppos,float size)

{
  nuvec_s pos;
  
  if (CurTerr == (TempTerr *)0x0) {
    pos.y = 2000000.0;
  }
  else {
    TerI = (TerrI *)NuScratchAlloc32(0x930);
    pos.x = ppos->x;
    pos.y = ppos->y;
    pos.z = ppos->z;
    NewScan(&pos,0,0);
    NewCast(&pos,5.0);
    NuScratchRelease();
  }
  return pos.y;
}

float NewShadowMask(nuvec_s *ppos,float size,int extramask)

{
  nuvec_s pos;
  
  if (CurTerr == (TempTerr *)0x0) {
    pos.y = 2000000.0;
  }
  else {
    TerI = (TerrI *)NuScratchAlloc32(0x930);
    pos.x = ppos->x;
    pos.y = ppos->y;
    pos.z = ppos->z;
    NewScan(&pos,extramask,0);
    NewCast(&pos,5.0);
    NuScratchRelease();
  }
  return pos.y;
}

float NewShadowPlat(nuvec_s *ppos,float size)

{
  nuvec_s pos;
  
  if (CurTerr == (TempTerr *)0x0) {
    pos.y = 2000000.0;
  }
  else {
    TerI = (TerrI *)NuScratchAlloc32(0x930);
    pos.x = ppos->x;
    pos.y = ppos->y;
    pos.z = ppos->z;
    NewScan(&pos,0,1);
    NewCast(&pos,5.0);
    NuScratchRelease();
  }
  return pos.y;
}

float NewShadowMaskPlat(nuvec_s *ppos,float size,int extramask)

{
  nuvec_s pos;
  
  if (CurTerr == (TempTerr *)0x0) {
    pos.y = 2000000.0;
  }
  else {
    TerI = (TerrI *)NuScratchAlloc32(0x930);
    pos.x = ppos->x;
    pos.y = ppos->y;
    pos.z = ppos->z;
    NewScan(&pos,extramask,1);
    NewCast(&pos,5.0);
    NuScratchRelease();
  }
  return pos.y;
}

float NewShadowMaskPlatRot(nuvec_s *ppos,float size,int extramask)

{
  nuvec_s v;
  
  if (CurTerr == (TempTerr *)0x0) {
    v.y = 2000000.0;
  }
  else {
    TerI = (TerrI *)NuScratchAlloc32(0x930);
    v.x = ppos->x;
    v.y = ppos->y;
    v.z = ppos->z;
    NewScanRot(&v,extramask);
    NewCast(&v,5.0);
    NuScratchRelease();
  }
  return v.y;
}

void TerrainSideClamp(nuvec_s *slide,nuvec_s *pos)

{
  float dotp;
  float dotq;
  
  dotp = (TerrShape->offset).x * slide->x + (TerrShape->offset).z * slide->z;
  dotq = (TerrShape->offset).x * slide->z - (TerrShape->offset).z * slide->x;
  if (TerrShape->size < dotp) {
    dotp = TerrShape->size;
  }
  if (dotp < -TerrShape->size) {
    dotp = -TerrShape->size;
  }
  if (TerrShape->size * 0.2 < dotq) {
    dotq = TerrShape->size * 0.2;
  }
  if (dotq < -TerrShape->size * 0.2) {
    dotq = -TerrShape->size * 0.2;
  }
  pos->x = pos->x - ((dotp * slide->x + dotq * slide->z) - (TerrShape->offset).x);
  pos->z = pos->z - ((dotp * slide->z - dotq * slide->x) - (TerrShape->offset).z);
  (TerrShape->offset).x = dotp * slide->x + dotq * slide->z;
  (TerrShape->offset).z = dotp * slide->z - dotq * slide->x;
  return;
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

void RayImpact(nuvec_s *vvel)

{
  short type;
  
  TerrainMoveImpactData();
  type = TerI->hittype;
  if (type < 5) {
    if (0 < type) {
      TerI->hittime = TerI->hittime - TerI->timeadj;
      if (TerI->hittime < 0.0) {
        TerI->hittime = 0.0;
      }
      vvel->x = (TerI->curvel).x * TerI->hittime;
      vvel->y = (TerI->curvel).y * TerI->hittime;
      vvel->z = (TerI->curvel).z * TerI->hittime;
    }
  }
  else if ((type < 0x15) && (0x10 < type)) {
    vvel->x = 0.0;
    vvel->y = 0.0;
    vvel->z = 0.0;
  }
  return;
}


int NewRayCast(nuvec_s *vpos,nuvec_s *vvel,float size)

{
  int hit;
  TerrI *tmpTerrI;
  float yPos;
  float tmp;
  
  if (CurTerr == (TempTerr *)0x0) {
    hit = 0;
  }
  else {
    plathitid = -1;
    TerrPolyObj = -1;
    TerrPoly = (hitdata *)0x0;
    tmpTerrI = (TerrI *)NuScratchAlloc32(0x930);
    TerI = tmpTerrI;
    tmpTerrI->yscale = 1.0;
    tmpTerrI->yscalesq = 1.0;
    tmpTerrI->inyscale = 1.0;
    tmpTerrI->inyscalesq = 1.0;
    TerI->size = size;
    TerI->sizediv = 1.0 / TerI->size;
    TerI->sizesq = TerI->size * TerI->size;
    tmpTerrI = TerI;
    tmp = vpos->x;
    (TerI->curpos).x = tmp;
    (tmpTerrI->origpos).x = tmp;
    tmpTerrI = TerI;
    tmp = vpos->y;
    (TerI->curpos).y = tmp;
    (tmpTerrI->origpos).y = tmp;
    tmpTerrI = TerI;
    tmp = vpos->z;
    (TerI->curpos).z = tmp;
    (tmpTerrI->origpos).z = tmp;
    tmpTerrI = TerI;
    yPos = vvel->y;
    tmp = vvel->z;
    (TerI->curvel).x = vvel->x;
    (tmpTerrI->curvel).y = yPos;
    (tmpTerrI->curvel).z = tmp;
    yPos = (tmpTerrI->curvel).y;
    tmp = (tmpTerrI->curvel).z;
    (tmpTerrI->origvel).x = (tmpTerrI->curvel).x;
    (tmpTerrI->origvel).y = yPos;
    (tmpTerrI->origvel).z = tmp;
    TerI->id = -1;
    TerI->flags = (uchar *)0x0;
    TerI->scanmode = 1;
    TerI->timeadj = 0.01;
    TerI->impactadj = 1e-05;
    ScanTerrain(0,0);
    DerotateMovementVector();
    HitTerrain();
    if (TerI->hittype != 0) {
      RayImpact(vvel);
      TerrainImpactNorm();
      ShadNorm.x = (TerI->hitnorm).x;
      ShadNorm.y = (TerI->hitnorm).y;
      ShadNorm.z = (TerI->hitnorm).z;
    }
    NuScratchRelease();
    hit = (int)TerI->hittype;
  }
  return hit;
}


int NewRayCastMask(nuvec_s *vpos,nuvec_s *vvel,float size,int mask)

{
  int hit;
  TerrI *tmpTerrI;
  float tmp;
  float yPos;
  
  if (CurTerr == (TempTerr *)0x0) {
    hit = 0;
  }
  else {
    plathitid = -1;
    TerrPolyObj = -1;
    TerrPoly = (hitdata *)0x0;
    tmpTerrI = (TerrI *)NuScratchAlloc32(0x930);
    TerI = tmpTerrI;
    tmpTerrI->yscale = 1.0;
    tmpTerrI->yscalesq = 1.0;
    tmpTerrI->inyscale = 1.0;
    tmpTerrI->inyscalesq = 1.0;
    TerI->size = size;
    TerI->sizediv = 1.0 / TerI->size;
    TerI->sizesq = TerI->size * TerI->size;
    tmpTerrI = TerI;
    tmp = vpos->x;
    (TerI->curpos).x = tmp;
    (tmpTerrI->origpos).x = tmp;
    tmpTerrI = TerI;
    tmp = vpos->y;
    (TerI->curpos).y = tmp;
    (tmpTerrI->origpos).y = tmp;
    tmpTerrI = TerI;
    tmp = vpos->z;
    (TerI->curpos).z = tmp;
    (tmpTerrI->origpos).z = tmp;
    tmpTerrI = TerI;
    yPos = vvel->y;
    tmp = vvel->z;
    (TerI->curvel).x = vvel->x;
    (tmpTerrI->curvel).y = yPos;
    (tmpTerrI->curvel).z = tmp;
    yPos = (tmpTerrI->curvel).y;
    tmp = (tmpTerrI->curvel).z;
    (tmpTerrI->origvel).x = (tmpTerrI->curvel).x;
    (tmpTerrI->origvel).y = yPos;
    (tmpTerrI->origvel).z = tmp;
    TerI->id = -1;
    TerI->flags = (uchar *)0x0;
    TerI->scanmode = 1;
    TerI->timeadj = 0.01;
    TerI->impactadj = 1e-05;
    ScanTerrain(0,mask);
    DerotateMovementVector();
    HitTerrain();
    if (TerI->hittype != 0) {
      RayImpact(vvel);
      TerrainImpactNorm();
      ShadNorm.x = (TerI->hitnorm).x;
      ShadNorm.y = (TerI->hitnorm).y;
      ShadNorm.z = (TerI->hitnorm).z;
    }
    NuScratchRelease();
    hit = (int)TerI->hittype;
  }
  
  
  int NewPlatInst(numtx_s *mat,int instance)		//Need Correction!!

{
  terr *tmpTot;
  terr *CurrT1;
  int k;
  terr *tmpNo;
  terr *CurrTno;
  short inst;
  int i;
  int j;
  int lp;
  
  if (terlistptr < 0x1000) {
    if (situtotal < 0x800) {
      if (mat == (numtx_s *)0x0) {
        lp = -1;
      }
      else {
        lp = -1;
        for (j = 0; j < 0x80; j = j + 1) {
          if (CurTerr->platdat[j].curmtx == (numtx_s *)0x0) {
            lp = j;
            break;
          }
        }
        if (lp == -1) {
          lp = -1;
        }
        else {
          i = -1;
          for (j = 0; j < 0x80; j = j + 1) {
            if ((CurTerr->platdat[j].curmtx != (numtx_s *)0x0) &&
               (CurTerr->platdat[j].instance == instance)) {
              i = j;
              break;
            }
          }
          if (i == -1) {
            lp = -1;
          }
          else {
            k = 0x30;
            CurrT1 = CurTerr->terr + situtotal;
            CurrTno = CurTerr->terr + CurTerr->platdat[i].terrno;
            do {
              tmpNo = CurrTno;
              tmpTot = CurrT1;
              k = k + -0x18;
              (tmpTot->Location).x = (tmpNo->Location).x;
              (tmpTot->Location).y = (tmpNo->Location).y;
              (tmpTot->Location).z = (tmpNo->Location).z;
              tmpTot->model = tmpNo->model;
              (tmpTot->min).x = (tmpNo->min).x;
              (tmpTot->min).y = (tmpNo->min).y;
              CurrTno = (terr *)&(tmpNo->min).z;
              CurrT1 = (terr *)&(tmpTot->min).z;
            } while (k != 0);
            *(float *)CurrT1 = *(float *)CurrTno;
            (tmpTot->max).x = (tmpNo->max).x;
            CurTerr->terr[situtotal].info = lp._2_2_;
            CurTerr->terr[situtotal].type = TERR_TYPE_PLATFORM;
            CurTerr->platdat[lp].curmtx = mat;
            CurTerr->platdat[lp].terrno = situtotal._2_2_;
            inst = (short)instance;
            CurTerr->platdat[lp].instance = inst;
            CurTerr->platdat[lp].status =
                 (platattrib)((uint)CurTerr->platdat[lp].status & 0x7fffffff);
            CurTerr->terrlist[terlistptr] = situtotal._2_2_;
            terlistptr = terlistptr + 1;
            CurTerr->platdat[lp].plrgrav = 0.0;
            CurTerr->platdat[lp].ypos = 0.0;
            CurTerr->platdat[lp].yvel = 0.0;
            CurTerr->platdat[lp].damp = 0.0;
            CurTerr->platdat[lp].tension = 0.0;
            situtotal = situtotal + 1;
            CurTerr->group[0x100].count = CurTerr->group[0x100].count + 1;
          }
        }
      }
    }
    else {
      lp = -1;
    }
  }
  else {
    lp = -1;
  }
  return lp;
}


int FindPlatInst(int instance)

{
  int orig;
  int lp;
  
  if (instance == -1) {
    orig = -1;
  }
  else {
    orig = -1;
    for (lp = 0; lp < 0x80; lp = lp + 1) {
      if ((CurTerr->platdat[lp].curmtx != (numtx_s *)0x0) &&
         (CurTerr->platdat[lp].instance == instance)) {
        orig = lp;
        break;
      }
    }
    if (orig == -1) {
      orig = -1;
    }
  }
  return orig;
}


void PlatInstRotate(int platid,int state)

{
  CurTerr->platdat[platid].status =
       (platattrib)(state << 0x1f | (uint)CurTerr->platdat[platid].status & 0x7fffffff);
  return;
}

void PlatInstBounce(int platid,float plrgrav,float tension,float damp)

{
  if ((-1 < platid) && (platid < 0x80)) {
    if (CurTerr->platdat[platid].plrgrav == 0.0) {
      CurTerr->platdat[platid].ypos = 0.0;
      CurTerr->platdat[platid].yvel = 0.0;
    }
    CurTerr->platdat[platid].plrgrav = plrgrav;
    CurTerr->platdat[platid].tension = tension;
    CurTerr->platdat[platid].damp = damp;
  }
  return;
}

int PlatInstGetHit(int platid)

{
  int status;
  
  if ((platid < 0) || (0x7f < platid)) {
    status = 0;
  }
  else {
    status = (uint)CurTerr->platdat[platid].status >> 0x1e & 1;
  }
  return status;
}

void UpdatePlatinst(int platid,numtx_s *mat)

{
  if ((-1 < platid) && (platid < 0x80)) {
    CurTerr->platdat[platid].curmtx = mat;
  }
  return;
}

int PlatImpactInfo(nuvec_s *norm,int *info,int *extra)

{
  int t;
  float y;
  float z;
  
  z = PlatImpactNorm.z;
  y = PlatImpactNorm.y;
  if (PlatImpactId != 0) {
    norm->x = PlatImpactNorm.x;
    norm->y = y;
    norm->z = z;
    *info = (uint)PlatImpactTer.info[0];
    *extra = (uint)PlatImpactTer.info[1];
  }
  t = PlatImpactId;
  PlatImpactId = 0;
  return t;
}


int PlatformCrush(void)

{
  return PlatCrush;
}


void NewScanInit(void)

{
  TempStackPtr = TempScanStack;
  TerrPlatDis = -1;
  return;
}

short * NewScanHandelFull(nuvec_s *vpos,nuvec_s *vvel,float size,int platscan,int extramask)

{
	//TODO
	
}

short * NewScanHandelSubset(short *handel,nuvec_s *vpos,nuvec_s *vvel,float size,int extramask)

{
	//TODO
	
}

short * NewScanHandel(nuvec_s *vpos,nuvec_s *vvel,float size,int plats,short *handel)

{
  short *psVar1;
  
  if (CurTerr == (TempTerr *)0x0) {
    psVar1 = (short *)0x0;
  }
  else {
    TerrPlatDis = -1;
    if (handel == (short *)0x0) {
      psVar1 = NewScanHandelFull(vpos,vvel,size,plats,0);
    }
    else {
      psVar1 = NewScanHandelSubset(handel,vpos,vvel,size,0);
    }
  }
  return psVar1;
}

void ScanTerrainHandel(int extramask,short *Handel)

{
	//TODO
}


int NewRayCastSetHandel(nuvec_s *vpos,nuvec_s *vvel,float size,float timeadj,float impactadj,
                       short *Handel)

{
  int hit;
  TerrI *tmpTerr;
  float fVar1;
  float tmp;
  
  if (CurTerr == NULL) {
    hit = 0;
  }
  else if (Handel == NULL) {
    hit = 0;
  }
  else {
    plathitid = -1;
    TerrPolyObj = -1;
    TerrPoly = NULL;
    tmpTerr = (TerrI *)NuScratchAlloc32(0x930);
    TerI = tmpTerr;
    tmpTerr->yscale = 1.0;
    tmpTerr->yscalesq = 1.0;
    tmpTerr->inyscale = 1.0;
    tmpTerr->inyscalesq = 1.0;
    TerI->size = size;
    TerI->sizediv = 1.0 / TerI->size;
    TerI->sizesq = TerI->size * TerI->size;
    tmpTerr = TerI;
    tmp = vpos->x;
    (TerI->curpos).x = tmp;
    (tmpTerr->origpos).x = tmp;
    tmpTerr = TerI;
    tmp = vpos->y;
    (TerI->curpos).y = tmp;
    (tmpTerr->origpos).y = tmp;
    tmpTerr = TerI;
    tmp = vpos->z;
    (TerI->curpos).z = tmp;
    (tmpTerr->origpos).z = tmp;
    tmpTerr = TerI;
    fVar1 = vvel->y;
    tmp = vvel->z;
    (TerI->curvel).x = vvel->x;
    (tmpTerr->curvel).y = fVar1;
    (tmpTerr->curvel).z = tmp;
    fVar1 = (tmpTerr->curvel).y;
    tmp = (tmpTerr->curvel).z;
    (tmpTerr->origvel).x = (tmpTerr->curvel).x;
    (tmpTerr->origvel).y = fVar1;
    (tmpTerr->origvel).z = tmp;
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
    if (TerI->hittype != 0) {
      RayImpact(vvel);
      TerrainImpactNorm();
      ShadNorm.x = (TerI->hitnorm).x;
      ShadNorm.y = (TerI->hitnorm).y;
      ShadNorm.z = (TerI->hitnorm).z;
    }
    NuScratchRelease();
    hit = (int)TerI->hittype;
  }
  return hit;
}

int TerrainPlatId(void)

{
  return plathitid;
}

void TerrainPlatGetMtx(int platid,numtx_s **old,numtx_s **cur)

{
  if (-1 < platid) {
    *old = (numtx_s *)(CurTerr->platdat + platid);
    *cur = CurTerr->platdat[platid].curmtx;
  }
  return;
}


void ScanTerrainPlatform(int msituid,int extramask)

{
	//TODO
}


int NewRayCastPlatForm(nuvec_s *vpos,nuvec_s *vvel,float size,float timeadj,int platformid)

{
  int hit;
  TerrI *tmpTerrI;
  float y;
  float tmp;
  
  if (CurTerr == (TempTerr *)0x0) {
    hit = 0;
  }
  else {
    plathitid = -1;
    TerrPolyObj = -1;
    TerrPoly = (hitdata *)0x0;
    tmpTerrI = (TerrI *)NuScratchAlloc32(0x930);
    TerI = tmpTerrI;
    tmpTerrI->yscale = 1.0;
    tmpTerrI->yscalesq = 1.0;
    tmpTerrI->inyscale = 1.0;
    tmpTerrI->inyscalesq = 1.0;
    TerI->size = size;
    TerI->sizediv = 1.0 / TerI->size;
    TerI->sizesq = TerI->size * TerI->size;
    tmpTerrI = TerI;
    tmp = vpos->x;
    (TerI->curpos).x = tmp;
    (tmpTerrI->origpos).x = tmp;
    tmpTerrI = TerI;
    tmp = vpos->y;
    (TerI->curpos).y = tmp;
    (tmpTerrI->origpos).y = tmp;
    tmpTerrI = TerI;
    tmp = vpos->z;
    (TerI->curpos).z = tmp;
    (tmpTerrI->origpos).z = tmp;
    tmpTerrI = TerI;
    y = vvel->y;
    tmp = vvel->z;
    (TerI->curvel).x = vvel->x;
    (tmpTerrI->curvel).y = y;
    (tmpTerrI->curvel).z = tmp;
    y = (tmpTerrI->curvel).y;
    tmp = (tmpTerrI->curvel).z;
    (tmpTerrI->origvel).x = (tmpTerrI->curvel).x;
    (tmpTerrI->origvel).y = y;
    (tmpTerrI->origvel).z = tmp;
    TerI->id = -1;
    TerI->flags = (uchar *)0x0;
    TerI->scanmode = 1;
    TerI->timeadj = timeadj;
    TerI->impactadj = 0.0;
    ScanTerrainPlatform((int)CurTerr->platdat[platformid].terrno,0);
    DerotateMovementVector();
    HitTerrain();
    if (TerI->hittype != 0) {
      RayImpact(vvel);
      TerrainImpactNorm();
      ShadNorm.x = (TerI->hitnorm).x;
      ShadNorm.y = (TerI->hitnorm).y;
      ShadNorm.z = (TerI->hitnorm).z;
    }
    NuScratchRelease();
    hit = (int)TerI->hittype;
  }
  return hit;
}