#include "../nu.h"

#define FLOAT_INTMAX 2147483647.0f
#define POW2(x) ((x) * (x))

//NGC MATCH
s32 ReadTerrain(u8* name2, s32 situ, short** store, struct tempterr_s* Tempterr) {
    struct sceneptr_s* sceneptr;
    s32 lp;
    s32 count;
    char LevName[100];
    short* ts;
    struct wallspl_s* wallspl;
    float minx;
    float maxx;
    float minz;
    float maxz;
    s32 a;
    s32 b;
    s32 fsize;

    strcpy(LevName, name2);
    strcat(LevName, ".ter");
    fsize = NuFileLoadBuffer(LevName, *store, 0x7fffffff);
    if (fsize == 0) {
        return -1;
    }

    ts = *store;
    sceneptr = (struct sceneptr_s*)(ts + *(s32*)ts);
    sceneptr->count &= 0xffff;
    *store += 2;
    count = 0;
    Tempterr->wallinfo = NULL;
    crashdata = NULL;
    for (lp = 0; lp < sceneptr->count; lp++) {

        switch (sceneptr->offlist[lp].type) {
            case -1:
                break;
            case 0:
            case 1:
                Tempterr->terr[situ].id = sceneptr->offlist[lp].id;
                Tempterr->terr[situ].type = sceneptr->offlist[lp].type;
                Tempterr->terr[situ].info = sceneptr->offlist[lp].info;
                Tempterr->terr[situ].Location.x = sceneptr->offlist[lp].translation.x;
                Tempterr->terr[situ].Location.y = sceneptr->offlist[lp].translation.y;
                Tempterr->terr[situ].Location.z = sceneptr->offlist[lp].translation.z;
                Tempterr->terr[situ].model = *store;
                Tempterr->terr[situ].flags = sceneptr->offlist[lp].flags;
                count++;
                situ++;
                break;
            case 2:
                if (*(s32*)*store == 0x12345678) {
                    *(void**)*store = Tempterr->wallinfo;
                    Tempterr->wallinfo = *store + 2;
                    *(s16*)Tempterr->wallinfo = *(s16*)Tempterr->wallinfo - 1;
                }
                break;
            case 3:
                crashdata = *store;
                break;
        }
        *store = *store + sceneptr->offlist[lp].offset;
    }
    for (wallspl = (struct wallspl_s*)Tempterr->wallinfo; wallspl != NULL; wallspl = ((struct wallspl_s**)wallspl)[-1])
    {
        for (a = 0; a < wallspl->count; a = a + 0x10) {
            if (wallspl->spl[a].y != FLOAT_INTMAX) {
                if (a + 0xf < wallspl->count) {
                    minx = maxx = wallspl->spl[a].x;
                    minz = maxz = wallspl->spl[a].z;

                    for (b = a + 1; b <= a + 0x10; b++) {
                        if (wallspl->spl[b].x < minx) {
                            minx = wallspl->spl[b].x;
                        }

                        if (wallspl->spl[b].x > maxx) {
                            maxx = wallspl->spl[b].x;
                        }

                        if (wallspl->spl[b].z < minz) {
                            minz = wallspl->spl[b].z;
                        }

                        if (wallspl->spl[b].z > maxz) {
                            maxz = wallspl->spl[b].z;
                        }
                    }
                    wallspl->spl[a].y = minx;
                    wallspl->spl[a + 1].y = maxx;
                    wallspl->spl[a + 2].y = minz;
                    wallspl->spl[a + 3].y = maxz;
                } else {
                    wallspl->spl[a].y = FLOAT_INTMAX;
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
short * terraininit(int LevelNum, short **store, short *endstore, int opt, char *name, struct nugscn_s *gscene, int gsid) {
    struct terr_s *mbuf;
    struct scaleterrain_s *ter;
    short *modp;
    int c;
    int b;
    int tx;
    int tz;
    float terminx;
    float termaxx;
    float terminy;
    float termaxy;
    float terminz;
    float termaxz;
    float maxradius;
    float ra;
    int xsteps;
    int zsteps;
    int lp;
    short id[2048];
    float minx[2048];
    float maxx[2048];
    float minz[2048];
    float maxz[2048];
    int platid;
    struct tempterr_s *TempTerr;
    short *rv;
    
    rv = *store;
    TempTerr = (struct tempterr_s *)*store;
    *store = *store + 0x57a8;
    TempTerr->terr = (struct terr_s *)*store;
    *store = *store + 0xee00;
    
    for (lp = 0; lp < 9; lp++) {
        TempTerr->pollist[lp].timer = 0;
        TempTerr->pollist[lp].hitdata[0] = NULL;
    }
    
    memset(TempTerr->terr, 0, 0x1c000);
    
    c = 0;
    mbuf = TempTerr->terr;
    while (c < 0x800) {
        mbuf->type = -1;
        c++;
        mbuf++;
    }
    asm("nop");
    for (c = 0; c < 0x101; c++) {
        TempTerr->terrgroup[c].count = 0;
    }
    asm("nop");
    for (c = 0; c < 0x80; c++) {
        TempTerr->platdata[c].curmtx = NULL;
        TempTerr->platdata[c].plrgrav = 0.0f;
        TempTerr->platdata[c].ypos = 0.0f;
        TempTerr->platdata[c].yvel = 0.0f;
        TempTerr->platdata[c].damp = 0.0f;
        TempTerr->platdata[c].tension = 0.0f;
    }
    asm("nop");
    for (c = 0; c < 4; c++) {
        TempTerr->TrackInfo[c].ptrid = NULL;
    }
    
    terraincnt = 0;
    curSphereter = 0;
    platinrange = 0;
    platid = 0;
    ShadPoly = NULL;
    TerrPoly = NULL;
    PlatImpactId = 0;
    
    situtotal = ReadTerrain((u8*)name, 0, store, TempTerr);
    
    if (situtotal < 0) {
        *store = rv;
        return 0;
    }
    
    TempTerr->terrainlow = 0x7fffffff;
    c = 0;
    mbuf = TempTerr->terr;
    
    while (c < 0x800) {
        if ((mbuf->type == TERR_TYPE_NORMAL) || (mbuf->type == TERR_TYPE_PLATFORM)) {
            maxradius = 0.0f;
            terminx = -2e+08f; //min val
            terminy = -2e+08f; //min val
            terminz = -2e+08f; //min val
            termaxx = 2e+08f; //max val
            termaxy = 2e+08f; //max val
            termaxz = 2e+08f; //max val
            
            modp = mbuf->model;
            
            while (*modp >= 0) {
                b = *(modp + 1);
                ter = (struct scaleterrain_s *)(modp + 10);
                
                while (b > 0) {
                    if (ter->info[0] == 0xff) {
                        ter->info[0] = ter->info[1];
                        ter->info[1] = 0;
                        ter->info[2] = ter->info[2] | 0x80;
                    }
                    
                    if (ter->minx < terminx) {
                        terminx = ter->minx;
                    }
                    if (ter->miny < terminy) {
                        terminy = ter->miny;
                    }
                    if (ter->minz < terminz) {
                        terminz = ter->minz;
                    }
                    if (ter->maxx > termaxx) {
                        termaxx = ter->maxx;
                    }
                    if (ter->maxy > termaxy) {
                        termaxy = ter->maxy;
                    }
                    if (ter->maxz > termaxz) {
                        termaxz = ter->maxz;
                    }
                    
                    if (ter->pnts[0].y + mbuf->Location.y < (float)TempTerr->terrainlow) {
                        TempTerr->terrainlow = (int)(ter->pnts[0].y + mbuf->Location.y);
                    }
                    if (ter->pnts[1].y + mbuf->Location.y < (float)TempTerr->terrainlow) {
                        TempTerr->terrainlow = (int)(ter->pnts[1].y + mbuf->Location.y);
                    }
                    if (ter->pnts[2].y + mbuf->Location.y < (float)TempTerr->terrainlow) {
                        TempTerr->terrainlow = (int)(ter->pnts[2].y + mbuf->Location.y);
                    }
                    if (ter->pnts[3].y + mbuf->Location.y < (float)TempTerr->terrainlow) {
                        TempTerr->terrainlow = (int)(ter->pnts[3].y + mbuf->Location.y);
                    }
                    
                    ra = ter->pnts[0].x * ter->pnts[0].x + ter->pnts[0].y * ter->pnts[0].y + ter->pnts[0].z * ter->pnts[0].z;
                    if (maxradius < ra) {
                        maxradius = ra;
                    }
                    
                    ra = ter->pnts[1].x * ter->pnts[1].x + ter->pnts[1].y * ter->pnts[1].y + ter->pnts[1].z * ter->pnts[1].z;
                    if (maxradius < ra) {
                        maxradius = ra;
                    }
                    
                    ra = ter->pnts[2].x * ter->pnts[2].x + ter->pnts[2].y * ter->pnts[2].y + ter->pnts[2].z * ter->pnts[2].z;
                    if (maxradius < ra) {
                        maxradius = ra;
                    }
                    
                    ra = ter->pnts[3].x * ter->pnts[3].x + ter->pnts[3].y * ter->pnts[3].y + ter->pnts[3].z * ter->pnts[3].z;
                    if (maxradius < ra) {
                        maxradius = ra;
                    }
                    
                    ter++;
                    b--;
                }
                
                modp = (short *)ter;
            }
            
            mbuf->radius = maxradius;
            
            if (mbuf->type == TERR_TYPE_PLATFORM) {
                mbuf->min.x = terminx;
                mbuf->min.y = terminy;
                mbuf->min.z = terminz;
                mbuf->max.x = termaxx;
                mbuf->max.y = termaxy;
                mbuf->max.z = termaxz;
            } else {
                mbuf->min.x = terminx + mbuf->Location.x;
                mbuf->min.y = terminy + mbuf->Location.y;
                mbuf->min.z = terminz + mbuf->Location.z;
                mbuf->max.x = termaxx + mbuf->Location.x;
                mbuf->max.y = termaxy + mbuf->Location.y;
                mbuf->max.z = termaxz + mbuf->Location.z;
            }
            
            terraincnt++;
        }
        
        mbuf++;
        c++;
    }
    
    terminx = -2e+08f;
    terminy = -2e+08f;
    terminz = -2e+08f;
    termaxx = 2e+08f;
    termaxy = 2e+08f;
    termaxz = 2e+08f;
    
    c = 0;
    mbuf = &TempTerr->terr[c];
    
    while (c < 0x800) {
        if ((mbuf->type == TERR_TYPE_NORMAL) || (mbuf->type == TERR_TYPE_PLATFORM)) {
            modp = mbuf->model;
            
            while (*modp >= 0) {
                b = *(modp + 1);
                
                if ((*(float*)(modp + 2)) + mbuf->Location.x < terminx) {
                    terminx = (*(float*)(modp + 2)) + mbuf->Location.x;
                }
                if ((*(float*)(modp + 4)) + mbuf->Location.x > termaxx) {
                    termaxx = (*(float*)(modp + 4)) + mbuf->Location.x;
                }
                if ((*(float*)(modp + 6)) + mbuf->Location.z < terminz) {
                    terminz = (*(float*)(modp + 6)) + mbuf->Location.z;
                }
                if ((*(float*)(modp + 8)) + mbuf->Location.z > termaxz) {
                    termaxz = (*(float*)(modp + 8)) + mbuf->Location.z;
                }
                
                ter = (struct scaleterrain_s*)(modp + 10);
                modp = (short *)&ter[b];
            }
        }
        
        mbuf++;
        c++;
    }
    
    terlistptr = 0;
    xsteps = 5;
    zsteps = 5;
    
    c = 0;
    mbuf = &TempTerr->terr[c];
    
    while (c < 0x800) {
        if (mbuf->type == TERR_TYPE_NORMAL) {
            modp = mbuf->model;
            minx[c] = 2e+08f;
            minz[c] = 2e+08f;
            maxx[c] = -2e+08f;
            maxz[c] = -2e+08f;
            
            while (*modp >= 0) {
                b = *(modp + 1);
                
                if (*(float*)(modp + 2) + mbuf->Location.x < minx[c]) {
                    minx[c] = *(float*)(modp + 2) + mbuf->Location.x;
                }
                if (*(float*)(modp + 4) + mbuf->Location.x > maxx[c]) {
                    maxx[c] = *(float*)(modp + 4) + mbuf->Location.x;
                }
                if (*(float*)(modp + 6) + mbuf->Location.z < minz[c]) {
                    minz[c] = *(float*)(modp + 6) + mbuf->Location.z;
                }
                if (*(float*)(modp + 8) + mbuf->Location.z > maxz[c]) {
                    maxz[c] = *(float*)(modp + 8) + mbuf->Location.z;
                }
                
                ter = (struct scaleterrain_s*)(modp + 10);
                modp = (short *)&ter[b];
            }

            tx = ((minx[c] + maxx[c]) / 2.0f) - terminx;
            tx = (int)((tx * xsteps) / (termaxx - terminx));
            if (tx < 0) {
                tx = 0;
            }
            if (tx > xsteps - 1) {
                tx = xsteps - 1;
            }
            tz = (((minz[c] + maxz[c]) / 2.0f) - terminz);
            tz = (int)((tz * zsteps) / (termaxz - terminz));
            if (tz < 0) {
                tz = 0;
            }
            if (tz > zsteps - 1) {
                tz = zsteps - 1;
            }
            
            id[c] = tx + tz * xsteps;
        }
        
        mbuf++;
        c++;
    }
    
    TempTerr->terrgcnt = 0;
    
    for (lp = 0; lp < (xsteps * zsteps); lp++) {
        terminx = -2e+08f;
        terminy = -2e+08f;
        terminz = -2e+08f;
        termaxx = 2e+08f;
        termaxy = 2e+08f;
        termaxz = 2e+08f;
        
        TempTerr->terrgroup[TempTerr->terrgcnt].tabindex = terlistptr;
        
        c = 0;
        mbuf = &TempTerr->terr[c];
        
        while (c < 0x800) {
            if ((id[c] == lp) && (mbuf->type == TERR_TYPE_NORMAL)) {
                if (minx[c] < terminx) {
                    terminx = minx[c];
                }
                if (minz[c] < terminz) {
                    terminz = minz[c];
                }
                if (maxx[c] > termaxx) {
                    termaxx = maxx[c];
                }
                if (maxz[c] > termaxz) {
                    termaxz = maxz[c];
                }
                
                TempTerr->terrgroup[TempTerr->terrgcnt].count++;
                TempTerr->terrlist[terlistptr++] = c;
            }
            
            mbuf++;
            c++;
        }
        
        if (TempTerr->terrgroup[TempTerr->terrgcnt].count != 0) {
            TempTerr->terrgroup[TempTerr->terrgcnt].minx = terminx;
            TempTerr->terrgroup[TempTerr->terrgcnt].minz = terminz;
            TempTerr->terrgroup[TempTerr->terrgcnt].maxx = termaxx;
            TempTerr->terrgroup[TempTerr->terrgcnt].maxz = termaxz;
            TempTerr->terrgcnt++;
        }
    }
    
    TempTerr->terrgroup[0x100].tabindex = terlistptr;
    
    c = 0;
    mbuf = &TempTerr->terr[c];
    
    while (c < 0x800) {
        if (mbuf->type == TERR_TYPE_PLATFORM) {
            TempTerr->terrlist[terlistptr++] = c;
            TempTerr->terrgroup[0x100].count++;
            
            if (platid < 0x80) {
                int index = mbuf->info;
                mbuf->info = platid;
                
                if (index < gscene->numinstance) {
                    TempTerr->platdata[mbuf->info].status.rotate = mbuf->flags.rot;
                    
                    if (gscene->instances[index].anim != NULL) {
                        TempTerr->platdata[mbuf->info].curmtx = &gscene->instances[index].anim->mtx;
                    } else {
                        TempTerr->platdata[mbuf->info].curmtx = &gscene->instances[index].mtx;
                    }
                    
                    TempTerr->platdata[mbuf->info].instance = index;
                    TempTerr->platdata[mbuf->info].terrno = c;
                    
                    if (gscene->instances[index].flags.visible == 0) {
                        mbuf->type = -1;
                    }
                    
                    platid++;
                } else {
                    mbuf->type = -1;
                }
            } else {
                mbuf->type = -1;
            }
        }
        
        mbuf++;
        c++;
    }
    
    TerrFlush();
    
    return rv;
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
void TerrainPlatformOldUpdate(void) {
  s32 i;

  if (CurTerr == NULL) {
      return;
  }
    asm("nop");
    for (i = 0; i < 0x80; i++) { 
      if (CurTerr->platdata[i].curmtx != NULL) {
          CurTerr->platdata[i].oldmtx = *CurTerr->platdata[i].curmtx;
      }
    }
    asm("nop");
    for (i = 0; i < 4; i++) {
      if (CurTerr->TrackInfo[i].ptrid != NULL) {
        CurTerr->TrackInfo[i].timer--;
        if(CurTerr->TrackInfo[i].timer < 1) {
          CurTerr->TrackInfo[i].ptrid = NULL;
        }
      }
    }
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

//NGC MATCH
short InsidePolLines(float x,float y,float z,float x0,float y0,float z0,float x1,float y1,float z1, struct nuvec_s *norm) {

 if ((NuFabs(norm->y) >= NuFabs(norm->x)) && (NuFabs(norm->y) >= NuFabs(norm->z))) {
      if (norm->y < 0.0f) {
        if (InsideLineF(x,z,0.0f,0.0f,x0,z0) != 0) {
            if (InsideLineF(x,z,x1,z1,0.0f,0.0f) != 0) {
                if (InsideLineF(x,z,x0,z0,x1,z1) != 0) {
                  return 1;
                }
            }
        }
      }
      else if (InsideLineF(x,z,x0,z0,0.0f,0.0f) != 0) {
          if (InsideLineF(x,z,0.0f,0.0f,x1,z1) != 0) {
              if (InsideLineF(x,z,x1,z1,x0,z0) != 0) {
                return 1;
              }
          }
      }
  }
 else if ((NuFabs(norm->x) >= NuFabs(norm->y)) && (NuFabs(norm->x) >= NuFabs(norm->z))) {
      if (norm->x < 0.0f) {
        if (InsideLineF(y,z,y0,z0,0.0f,0.0f) != 0) {
            if (InsideLineF(y,z,0.0f,0.0f,y1,z1) != 0) {
              if (InsideLineF(y,z,y1,z1,y0,z0) != 0) {
                return 1;
              }
            }
        }
      }
      else if (InsideLineF(y,z,0.0f,0.0f,y0,z0) != 0) {
        if (InsideLineF(y,z,y1,z1,0.0f,0.0f) != 0) {
            if (InsideLineF(y,z,y0,z0,y1,z1) != 0) {
                return 1;
            }
        }
      }
  }
  else if (norm->z < 0.0f) {
    if (((InsideLineF(y,x,0.0f,0.0f,y0,x0) != 0) && (InsideLineF(y,x,y1,x1,0.0f,0.0f) != 0)) &&
       (InsideLineF(y,x,y0,x0,y1,x1) != 0)) {
      return 1;
    }
  }
  else {
    if (((InsideLineF(y,x,y0,x0,0.0f,0.0f) != 0) && (InsideLineF(y,x,0.0f,0.0f,y1,x1) != 0)) &&
       (InsideLineF(y,x,y1,x1,y0,x0) != 0)) {
      return 1;
    }
  }
  return 0;
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

//NGC MATCH
void NewScan(struct nuvec_s *ppos,s32 extramask,s32 platscan) {
  struct nuvec_s pos;
  s32 a;
  s32 b;
  s32 c;
  s32 id;
  s32 HitCnt;
  s32 index;
  s32 lp;
  s32 curobj;
  short *CurData;
  struct scaleterrain_s **curter;
  struct scaleterrain_s *ter;
  struct scaleterrain_s **HitData;
  struct scaleterrain_s **MaxData;
  short* LastWrite;
  short *ttemp;
  float maxx;
  float maxy;
  float maxz;
  float tmaxx;
  float tmaxz;
  float minx;
  float miny;
  float minz;
  float tminx;
  float tminz;
  struct terr_s *mbuf2;
  short *modp;

  pos = *ppos;
  index = -1;
  b = 0x7fffffff;
  id = -1;
  lp = 0;
  while(lp < 8) {
        if (((((CurTerr->pollist[lp].timer > 0) && (((pos.x + 1.0f) - CurTerr->pollist[lp].x) < 2.0f))
             && (((pos.z + 1.0f) - CurTerr->pollist[lp].z) < 2.0f) &&
                ((pos.x + 1.0f) - CurTerr->pollist[lp].x) > 0.0f
                && ((pos.z + 1.0f) - CurTerr->pollist[lp].z) > 0.0f
            )))
        {
            id = lp;
            break;
        }
        if (CurTerr->pollist[lp].timer < b) {
            b = (s32)CurTerr->pollist[lp].timer;
            index = lp;
        }
        lp++;
    }    
    if (id >= 0) {
    LastWrite = (short*)TerI->hitdata;
    index = id;
    CurTerr->pollist[index].timer = 8;
    maxx = pos.x + 0.1f;
    maxz = pos.z + 0.1f;
    minx = pos.x - 0.1f;
    minz = pos.z - 0.1f;
    HitData = TerI->hitdata + 1;
    MaxData = HitData + 0x1fc;
    HitCnt = 0;
    CurData = (short*)CurTerr->pollist[index].hitdata;
        while (*(short *)CurData > 0) {
            curobj = *(short *)(CurData + 1);  
            mbuf2 = CurTerr->terr + curobj;
            curter = (struct scaleterrain_s**)(CurData + 2);
          if (((maxx >= mbuf2->min.x) && (maxz >= mbuf2->min.z)) &&
             ((minx <= mbuf2->max.x &&
              ((minz < mbuf2->max.z && (mbuf2->type != ~TERR_TYPE_NORMAL)))))) {
            tmaxx = (maxx - mbuf2->Location.x);
            tmaxz = (maxz - mbuf2->Location.z);
            tminx = (minx - mbuf2->Location.x);
            tminz = (minz - mbuf2->Location.z);
            for (lp = (s32)*(short *)CurData; 0 < lp; lp--, curter++) {
              ter = *curter;
              if (((((tmaxx >= ter->minx) && (tminx < ter->maxx)) &&
                   (tmaxz >= ter->minz)) &&
                  ((tminz < ter->maxz && (HitData < MaxData)))) &&
                 ((ter->info[1] == 0 || (((uint)ter->info[1] & extramask) != 0)))) {
                *HitData = ter;
                HitData++;
                HitCnt++;
              }
            }
            if (HitCnt != 0) {
              *(short *)LastWrite = HitCnt;
              *(short *)(LastWrite + 1) = curobj;
              LastWrite = (short*)HitData;
              HitData++;
              HitCnt = 0;
            }
            CurData = (short*)curter;
          }
          else {
            curter = curter + *(short *)CurData;
            CurData = (short*)curter;
          }
        }
    } else {
        TerI->hitcnt = 0;
        LastWrite = (short*)CurTerr->pollist[index].hitdata;
        HitData = CurTerr->pollist[index].hitdata + 1;
        MaxData = HitData + 0x1fc;
        HitCnt = 0;
        maxx = pos.x + 1.0f;
        maxy = pos.y + 1.0f;
        maxz = pos.z + 1.0f;
        minx = pos.x - 1.0f;
        miny = pos.y - 1.0f;
        minz = pos.z - 1.0f;
        for (a = 0; a < CurTerr->terrgcnt; a++) {
          if (((maxx >= CurTerr->terrgroup[a].minx) && (maxz >= CurTerr->terrgroup[a].minz)) &&
             ((minx <= CurTerr->terrgroup[a].maxx && (minz <= CurTerr->terrgroup[a].maxz)))) {
            ttemp = CurTerr->terrlist + CurTerr->terrgroup[a].tabindex;
            for (b = 0; b < CurTerr->terrgroup[a].count; b++, ttemp++) {
              mbuf2 = CurTerr->terr + *ttemp;
              if ((((maxx >= mbuf2->min.x) && (maxz >= mbuf2->min.z)) &&
                  (minx <= mbuf2->max.x)) &&
                 ((minz < mbuf2->max.z && (mbuf2->type != ~TERR_TYPE_NORMAL)))) {
                tmaxx = maxx - mbuf2->Location.x;
                tmaxz = maxz - mbuf2->Location.z;
                tminx = minx - mbuf2->Location.x;
                tminz = minz - mbuf2->Location.z;
                modp = mbuf2->model;
                while (*modp >= 0) {
                  c = (s32)modp[1];
                  ter = (struct scaleterrain_s *)(modp + 10);
                  if ((tmaxx >= *(float *)(modp + 2)) && (tminx < *(float *)(modp + 4))) {
                     if ((tmaxz >= *(float *)(modp + 6)) && (tminz < *(float *)(modp + 8))) {
                        for (; c > 0; c--) {
                          if ((((tmaxx >= ter->minx) && (tminx < ter->maxx)) && (tmaxz >= ter->minz)) &&
                             ((tminz < ter->maxz && (HitData < MaxData)))) {
                            *HitData = ter;
                            HitData++;
                            HitCnt++;
                          }
                          ter++;
                        }
                        modp = (short *)ter;
                      } else {
                        modp = (short *)(ter + c);
                      }
                  } else {
                    modp = (short *)(ter + c);
                  }
                }
                if (HitCnt != 0) {
                  *(short *)LastWrite = HitCnt;
                  *(short *)(LastWrite + 1) = *ttemp;
                  LastWrite = (short*)HitData;
                  HitData++;
                  HitCnt = 0;
                }
              }
            }
          }
        }
        CurTerr->pollist[index].timer = 8;
        CurTerr->pollist[index].x = ppos->x;
        CurTerr->pollist[index].z = ppos->z;
        *(short *)LastWrite = 0;
        *(short *)(LastWrite + 1) = 0;
        LastWrite = (short*)TerI->hitdata;
        maxx = pos.x + 0.1f;
        maxz = pos.z + 0.1f;
        minx = pos.x - 0.1f;
        minz = pos.z - 0.1f;
        HitData = TerI->hitdata + 1;
        MaxData = HitData + 0x1fc;
        HitCnt = 0;
        CurData = (short*)CurTerr->pollist[index].hitdata;
        while (*(short *)CurData > 0) {
          curobj = *(short *)(CurData + 1);
          mbuf2 = CurTerr->terr + curobj;
          curter = (struct scaleterrain_s**)(CurData + 2);
          if (((maxx >= mbuf2->min.x) && (maxz >= mbuf2->min.z)) &&
             ((minx <= mbuf2->max.x &&
              ((minz < mbuf2->max.z && (mbuf2->type != ~TERR_TYPE_NORMAL)))))) {
            tmaxx = maxx - mbuf2->Location.x;
            tmaxz = maxz - mbuf2->Location.z;
            tminx = minx - mbuf2->Location.x;
            tminz = minz - mbuf2->Location.z;
            for (lp = (s32)*(short *)CurData; lp > 0; lp--, curter++) {
              ter = *curter;
              if ((((tmaxx >= ter->minx) && (tminx < ter->maxx)) &&
                  (tmaxz >= ter->minz)) &&
                 (((tminz < ter->maxz && (HitData < MaxData)) &&
                  ((ter->info[1] == 0 || (((uint)ter->info[1] & extramask) != 0)))))) {
                *HitData = ter;
                HitData++;
                HitCnt++;
              }
            }
            if (HitCnt != 0) {
              *(short *)LastWrite = HitCnt;
              *(short *)(LastWrite + 1) = curobj;
              LastWrite = (short*)HitData;
              HitData++;
              HitCnt = 0;
            }
            CurData = (short*)curter;
          }
          else {
            curter = curter + *(short *)CurData;
            CurData = (short*)curter;
          }
        }
      }
      a = 0x100;
      minx = minx - 0.05f;
      maxx = maxx + 0.05f;
      minz = minz - 0.05f;
      maxz = maxz + 0.05f;
      if (platscan != 0) {
        ttemp = CurTerr->terrlist + CurTerr->terrgroup[a].tabindex;
        for (b = 0; b < CurTerr->terrgroup[a].count; b++, ttemp++) {
          mbuf2 = CurTerr->terr + *ttemp;
          if (CurTerr->platdata[mbuf2->info].curmtx != NULL) {
            if ((CurTerr->platdata[mbuf2->info].curmtx)->_30 >
                CurTerr->platdata[mbuf2->info].oldmtx._30) {
              tminx = minx - mbuf2->Location.x;
              tmaxx = (((CurTerr->platdata[mbuf2->info].curmtx)->_30 -
                          CurTerr->platdata[mbuf2->info].oldmtx._30) * 1.5f + maxx) -
                         mbuf2->Location.x;
            }
            else {
              tmaxx = maxx - mbuf2->Location.x;
              tminx = (((CurTerr->platdata[mbuf2->info].curmtx)->_30 -
                          CurTerr->platdata[mbuf2->info].oldmtx._30) * 1.5f + minx) -
                         mbuf2->Location.x;
            }
            if ((CurTerr->platdata[mbuf2->info].curmtx)->_32 >
                CurTerr->platdata[mbuf2->info].oldmtx._32) {
              tminz = minz - mbuf2->Location.z;
              tmaxz = (((CurTerr->platdata[mbuf2->info].curmtx)->_32 -
                          CurTerr->platdata[mbuf2->info].oldmtx._32) * 1.5f + maxz) -
                         mbuf2->Location.z;
            }
            else {
              tmaxz = maxz - mbuf2->Location.z;
              tminz = (((CurTerr->platdata[mbuf2->info].curmtx)->_32 -
                          CurTerr->platdata[mbuf2->info].oldmtx._32) * 1.5f + minz) -
                         mbuf2->Location.z;
            }
          }
          else {
            tmaxx = maxx - mbuf2->Location.x;
            tminx = minx - mbuf2->Location.x;
            tmaxz = maxz - mbuf2->Location.z;
            tminz = minz - mbuf2->Location.z;
          }
          if (((tmaxx >= mbuf2->min.x) && (tmaxz >= mbuf2->min.z)) &&
             ((tminx <= mbuf2->max.x &&
              ((tminz < mbuf2->max.z && (mbuf2->type != ~TERR_TYPE_NORMAL)))))) {
            modp = mbuf2->model;
            while (*modp >= 0) {
              c = (s32)modp[1];
              ter = (struct scaleterrain_s *)(modp + 10);
              if ((tmaxx >= *(float *)(modp + 2)) && (tminx < *(float *)(modp + 4))) {
                  if ((tmaxz >= *(float *)(modp + 6)) && (tminz < *(float *)(modp + 8))) {
                    for (; c > 0; c--) {
                      if ((((tmaxx >= ter->minx) && (tminx < ter->maxx)) && (tmaxz >= ter->minz)) &&
                         (((tminz < ter->maxz && (HitData < MaxData)) &&
                          ((ter->info[1] == 0 || (((uint)ter->info[1] & extramask) != 0)))))) {
                        *HitData = ter;
                        HitData++;
                        HitCnt++;
                      }
                      ter++;
                    }
                    modp = (short *)ter;
                  } else {
                     modp = (short *)(ter + c); 
                  }
              }
              else {
                modp = (short *)(ter + c);
              }
            }
            if (HitCnt != 0) {
              *(short *)LastWrite = HitCnt;
              *(short *)((s32)LastWrite + 2) = *ttemp;
              LastWrite = (short*)HitData;
              HitData++;
              HitCnt = 0;
            }
          }
        }
      }
      *(short *)LastWrite = 0;
      *(short *)(LastWrite + 1) = 0;
}

//NGC MATCH
void NewScanRot(struct nuvec_s* ppos, s32 extramask) {
    struct nuvec_s pos;
    struct nuvec4_s pnts[4];
    struct nuvec4_s norm[2];
    s32 a;
    s32 b;
    s32 c;
    s32 id;
    s32 HitCnt;
    s32 index;
    s32 lp;
    s32 curobj;
    short* CurData;
    struct scaleterrain_s** curter;
    struct scaleterrain_s* ter;
    struct scaleterrain_s** HitData;
    struct scaleterrain_s** MaxData;
    short* LastWrite;
    short* ttemp;
    s32 curscltemp;
    float maxx;
    float maxy;
    float maxz;
    float tmaxx;
    float tmaxz;
    float tn;
    float minx;
    float miny;
    float minz;
    float tminx;
    float tminz;
    struct terr_s* mbuf2;
    short* modp;

    curscltemp = 0;
    pos = *ppos;
    index = -1;
    b = 0x7fffffff;
    id = -1;
    lp = 0;
    while (lp < 8) {
        if ((((CurTerr->pollist[lp].timer > 0) && (((pos.x + 1.0f) - CurTerr->pollist[lp].x) < 2.0f))
             && (((pos.z + 1.0f) - CurTerr->pollist[lp].z) < 2.0f))
            && ((
                ((pos.x + 1.0f) - CurTerr->pollist[lp].x) > 0.0f
                && ((pos.z + 1.0f) - CurTerr->pollist[lp].z) > 0.0f
            )))
        {
            id = lp;
            break;
        }
        if (CurTerr->pollist[lp].timer < b) {
            b = (s32)CurTerr->pollist[lp].timer;
            index = lp;
        }
        lp++;
    }
    if (id >= 0) {
        LastWrite = (short*)TerI->hitdata;
        index = id;
        CurTerr->pollist[index].timer = 8;
        maxx = pos.x + 0.1f;
        maxz = pos.z + 0.1f;
        minx = pos.x - 0.1f;
        minz = pos.z - 0.1f;
        HitData = TerI->hitdata + 1;
        MaxData = HitData + 0x1fc;
        HitCnt = 0;
        CurData = (short*)CurTerr->pollist[index].hitdata;
        while (*(short*)CurData > 0) {
            curobj = *(short*)(CurData + 1);
            mbuf2 = CurTerr->terr + curobj;
            curter = (struct scaleterrain_s**)CurData + 1;
            if (((maxx >= mbuf2->min.x) && (maxz >= mbuf2->min.z))
                && ((minx <= mbuf2->max.x && ((minz < mbuf2->max.z && (mbuf2->type != ~TERR_TYPE_NORMAL))))))
            {
                tmaxx = maxx - mbuf2->Location.x;
                tmaxz = maxz - mbuf2->Location.z;
                tminx = minx - mbuf2->Location.x;
                tminz = minz - mbuf2->Location.z;
                for (lp = (s32) * (short*)CurData; 0 < lp; lp--, curter++) {
                    ter = *curter;
                    if (((((tmaxx >= ter->minx) && (tminx < ter->maxx)) && (tmaxz >= ter->minz))
                         && ((tminz < ter->maxz && (HitData < MaxData))))
                        && ((ter->info[1] == 0 || (((uint)ter->info[1] & extramask) != 0))))
                    {
                        *HitData = ter;
                        HitData++;
                        HitCnt++;
                    }
                }
                if (HitCnt != 0) {
                    *(s16*)LastWrite = HitCnt;
                    *(short*)((s32)LastWrite + 2) = curobj;
                    LastWrite = (short*)HitData;
                    HitData++;
                    HitCnt = 0;
                }
                CurData = (short*)curter;
            } else {
                curter = curter + *(short*)CurData;
                CurData = (short*)curter;
            }
        }
    } else {
        TerI->hitcnt = 0;
        LastWrite = (short*)CurTerr->pollist[index].hitdata;
        HitData = CurTerr->pollist[index].hitdata + 1;
        MaxData = (struct scaleterrain_s**)(HitData + 0x1fc);
        HitCnt = 0;
        maxx = pos.x + 1.0f;
        maxy = pos.y + 1.0f;
        maxz = pos.z + 1.0f;
        minx = pos.x - 1.0f;
        miny = pos.y - 1.0f;
        minz = pos.z - 1.0f;
        for (a = 0; a < CurTerr->terrgcnt; a++) {
            if (((maxx >= CurTerr->terrgroup[a].minx) && (maxz >= CurTerr->terrgroup[a].minz))
                && ((minx <= CurTerr->terrgroup[a].maxx && (minz <= CurTerr->terrgroup[a].maxz))))
            {
                ttemp = CurTerr->terrlist + CurTerr->terrgroup[a].tabindex;
                for (b = 0; b < CurTerr->terrgroup[a].count; b++, ttemp++) {
                    mbuf2 = CurTerr->terr + *ttemp;
                    if ((((maxx >= mbuf2->min.x) && (maxz >= mbuf2->min.z)) && (minx <= mbuf2->max.x))
                        && ((minz < mbuf2->max.z && (mbuf2->type != ~TERR_TYPE_NORMAL))))
                    {
                        tmaxx = maxx - mbuf2->Location.x;
                        tmaxz = maxz - mbuf2->Location.z;
                        tminx = minx - mbuf2->Location.x;
                        tminz = minz - mbuf2->Location.z;
                        modp = (short*)mbuf2->model;
                        while ((*(short*)modp) >= 0) {
                            c = *(short*)(modp + 1);
                            ter = (struct scaleterrain_s*)(modp + 10);
                            if ((tmaxx >= (*(float*)(modp + 2))) && ((tminx < *(float*)(modp + 4)))) {
                                if ((tmaxz >= (*(float*)(modp + 6))) && ((tminz < *(float*)(modp + 8)))) {
                                    for (; c > 0; c--) {
                                        if ((((tmaxx >= ter->minx) && (tminx < ter->maxx))
                                             && (tmaxz >= ter->minz))
                                            && ((tminz < ter->maxz && (HitData < MaxData))))
                                        {
                                            *HitData = ter;
                                            HitData++;
                                            HitCnt++;
                                        }
                                        ter++;
                                    }
                                    modp = (short*)ter;
                                } else {
                                    modp = (short*)&ter[c];
                                }
                            } else {
                                modp = (short*)&ter[c];
                            }
                        }
                        if (HitCnt != 0) {
                            *(s16*)LastWrite = HitCnt;
                            *(short*)((s32)LastWrite + 2) = *ttemp;
                            LastWrite = (short*)HitData;
                            HitData++;
                            HitCnt = 0;
                        }
                    }
                }
            }
        }
        CurTerr->pollist[index].timer = 8;
        CurTerr->pollist[index].x = ppos->x;
        CurTerr->pollist[index].z = ppos->z;
        *(s16*)LastWrite = 0;
        *(s16*)((s32)LastWrite + 2) = 0;
        LastWrite = (short*)TerI->hitdata;
        maxx = pos.x + 0.1f;
        maxz = pos.z + 0.1f;
        minx = pos.x - 0.1f;
        minz = pos.z - 0.1f;
        HitData = TerI->hitdata + 1;
        MaxData = HitData + 0x1fc;
        HitCnt = 0;
        CurData = (short*)CurTerr->pollist[index].hitdata;
        while (*(short*)CurData > 0) {
            curobj = *(short*)(CurData + 1);
            mbuf2 = CurTerr->terr + curobj;
            curter = (struct scaleterrain_s**)CurData + 1;
            if (((maxx >= mbuf2->min.x) && (maxz >= mbuf2->min.z))
                && ((minx <= mbuf2->max.x && ((minz < mbuf2->max.z && (mbuf2->type != ~TERR_TYPE_NORMAL))))))
            {
                tmaxx = maxx - mbuf2->Location.x;
                tmaxz = maxz - mbuf2->Location.z;
                tminx = minx - mbuf2->Location.x;
                tminz = minz - mbuf2->Location.z;
                for (lp = (s32) * (short*)CurData; lp > 0; lp--, curter++) {
                    ter = *curter;
                    if ((((tmaxx >= ter->minx) && (tminx < ter->maxx)) && (tmaxz >= ter->minz))
                        && ((
                            (tminz < ter->maxz && (HitData < MaxData))
                            && ((ter->info[1] == 0 || (((uint)ter->info[1] & extramask) != 0)))
                        )))
                    {
                        *HitData = ter;
                        HitData++;
                        HitCnt++;
                    }
                }
                if (HitCnt != 0) {
                    *(s16*)LastWrite = HitCnt;
                    *(short*)((s32)LastWrite + 2) = curobj;
                    LastWrite = (short*)HitData;
                    HitData++;
                    HitCnt = 0;
                }
                CurData = (short *)curter;
            } else {
                curter = curter + *(short*)CurData;
                CurData = (short *)curter;
            }
        }
    }
    a = 0x100;
    minx = minx - 0.05f;
    maxx = maxx + 0.05f;
    minz = minz - 0.05f;
    maxz = maxz + 0.05f;
    ttemp = &CurTerr->terrlist[CurTerr->terrgroup[a].tabindex];
    for (b = 0; b < CurTerr->terrgroup[a].count; b++, ttemp++) {
        mbuf2 = CurTerr->terr + *ttemp;
        if (CurTerr->platdata[mbuf2->info].curmtx != NULL) {
            if ((CurTerr->platdata[mbuf2->info].curmtx)->_30 > CurTerr->platdata[mbuf2->info].oldmtx._30) {
                tminx = minx - mbuf2->Location.x;
                tmaxx =
                    (((CurTerr->platdata[mbuf2->info].curmtx)->_30 - CurTerr->platdata[mbuf2->info].oldmtx._30) * 1.5f
                     + maxx)
                    - mbuf2->Location.x;
            } else {
                tmaxx = maxx - mbuf2->Location.x;
                tminx =
                    (((CurTerr->platdata[mbuf2->info].curmtx)->_30 - CurTerr->platdata[mbuf2->info].oldmtx._30) * 1.5f
                     + minx)
                    - mbuf2->Location.x;
            }
            if ((CurTerr->platdata[mbuf2->info].curmtx)->_32 > CurTerr->platdata[mbuf2->info].oldmtx._32) {
                tminz = minz - mbuf2->Location.z;
                tmaxz =
                    (((CurTerr->platdata[mbuf2->info].curmtx)->_32 - CurTerr->platdata[mbuf2->info].oldmtx._32) * 1.5f
                     + maxz)
                    - mbuf2->Location.z;
            } else {
                tmaxz = maxz - mbuf2->Location.z;
                tminz =
                    (((CurTerr->platdata[mbuf2->info].curmtx)->_32 - CurTerr->platdata[mbuf2->info].oldmtx._32) * 1.5f
                     + minz)
                    - mbuf2->Location.z;
            }
        } else {
                tmaxx = maxx - mbuf2->Location.x;
                tminx = minx - mbuf2->Location.x;
                tmaxz = maxz - mbuf2->Location.z;
                tminz = minz - mbuf2->Location.z;
        }
        if (CurTerr->platdata[mbuf2->info].status.rotate != 0) {
            tn = ((maxx + minx) * 0.5f - mbuf2->Location.x) * ((maxx + minx) * 0.5f - mbuf2->Location.x)
                + ((maxz + minz) * 0.5f - mbuf2->Location.z) * ((maxz + minz) * 0.5f - mbuf2->Location.z);
            if ((mbuf2->type != ~TERR_TYPE_NORMAL) && (tn < mbuf2->radius + 0.0000099999997f)) {
                modp = (short*)mbuf2->model;
                while (*(short*)modp >= 0) {
                    c = *(short*)(modp + 1);
                    for (ter = (struct scaleterrain_s *)(modp + 10); c > 0; c--) {
                        if ((HitData < MaxData)
                            && ((ter->info[1] == 0 || (((uint)ter->info[1] & extramask) != 0))))
                        {
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
                            if (ter->norm[1].y < 65535.0f) {
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
                            } else {
                                pnts[3] = pnts[2];
                            }
                            if (((((pnts[0].x > tminx) || (pnts[1].x > tminx)) || (pnts[2].x > tminx))
                                 || (pnts[3].x > tminx))
                                && ((
                                    (((pnts[0].x < tmaxx || (pnts[1].x < tmaxx))
                                      || ((pnts[2].x < tmaxx || (pnts[3].x < tmaxx))))
                                     && ((
                                         (pnts[0].z > tminz || (pnts[1].z > tminz))
                                         || ((pnts[2].z > tminz || (pnts[3].z > tminz)))
                                     )))
                                    && ((
                                        ((pnts[0].z < tmaxz || (pnts[1].z < tmaxz)) || (pnts[2].z < tmaxz))
                                        || (pnts[3].z < tmaxz)
                                    ))
                                )))
                            {
                                ScaleTerrain[curscltemp].info[0] = ter->info[0];
                                ScaleTerrain[curscltemp].info[1] = ter->info[1];
                                ScaleTerrain[curscltemp].info[2] = ter->info[2];
                                ScaleTerrain[curscltemp].info[3] = ter->info[3];
                                ScaleTerrain[curscltemp].pnts[0].x = pnts[0].x;
                                ScaleTerrain[curscltemp].pnts[0].y = pnts[0].y;
                                ScaleTerrain[curscltemp].pnts[0].z = pnts[0].z;
                                ScaleTerrain[curscltemp].pnts[1].x = pnts[1].x;
                                ScaleTerrain[curscltemp].pnts[1].y = pnts[1].y;
                                ScaleTerrain[curscltemp].pnts[1].z = pnts[1].z;
                                ScaleTerrain[curscltemp].pnts[2].x = pnts[2].x;
                                ScaleTerrain[curscltemp].pnts[2].y = pnts[2].y;
                                ScaleTerrain[curscltemp].pnts[2].z = pnts[2].z;
                                if (ter->norm[1].y < 65535.0f) {
                                    ScaleTerrain[curscltemp].pnts[3].x = pnts[3].x;
                                    ScaleTerrain[curscltemp].pnts[3].y = pnts[3].y;
                                    ScaleTerrain[curscltemp].pnts[3].z = pnts[3].z;
                                    tn = 1.0f
                                        / NuFsqrt(norm[1].x * norm[1].x + norm[1].y * norm[1].y + norm[1].z * norm[1].z
                                        );
                                    ScaleTerrain[curscltemp].norm[1].x = norm[1].x * tn;
                                    ScaleTerrain[curscltemp].norm[1].y = norm[1].y * tn;
                                    ScaleTerrain[curscltemp].norm[1].z = norm[1].z * tn;
                                } else {
                                    ScaleTerrain[curscltemp].norm[1].y = 65536.0f;
                                }
                                tn = 1.0f
                                    / NuFsqrt(norm[0].x * norm[0].x + norm[0].y * norm[0].y + norm[0].z * norm[0].z);
                                ScaleTerrain[curscltemp].norm[0].x = norm[0].x * tn;
                                ScaleTerrain[curscltemp].norm[0].y = norm[0].y * tn;
                                ScaleTerrain[curscltemp].norm[0].z = norm[0].z * tn;
                                *HitData = ScaleTerrain + curscltemp;
                                HitData++;
                                HitCnt++;
                                curscltemp++;
                            }
                        }
                        ter = (struct scaleterrain_s*)((s32)ter + 100);
                    }
                    modp = (short*)ter;
                }
                if (HitCnt != 0) {
                    *(s16*)LastWrite = HitCnt;
                    *(short*)((s32)LastWrite + 2) = *ttemp;
                    LastWrite = (short*)HitData;
                    HitData++;
                    HitCnt = 0;
                }
            }
        }
        else if (((tmaxx >= mbuf2->min.x) && (tmaxz >= mbuf2->min.z)) &&
                ((tminx <= mbuf2->max.x &&
                 ((tminz < mbuf2->max.z && (mbuf2->type != ~TERR_TYPE_NORMAL))))))
        {
            modp = (short*)mbuf2->model;
            while (*(short*)modp >= 0) {
                c = *(short*)(modp + 1);
                ter = (struct scaleterrain_s*)(modp + 10);
                if ((tmaxx >= (*(float*)(modp + 2))) && (tminx < (*(float*)(modp + 4)))) {
                    if ((tmaxz >= (*(float*)(modp + 6))) && (tminz < (*(float*)(modp + 8)))) {
                        for (; c > 0; c--) {
                            if (((((tmaxx >= ter->minx) && (tminx < ter->maxx))
                                  && (tmaxz >= ter->minz))
                                 && ((tminz < ter->maxz && (HitData < MaxData))))
                                && ((ter->info[1] == 0 || (((uint)ter->info[1] & extramask) != 0))))
                            {
                                *HitData = ter;
                                HitData++;
                                HitCnt++;
                            }
                            ter++;
                        }
                        modp = (short*)ter;
                    } else {
                        modp = (short*)&ter[c];
                    }
                } else {
                    modp = (short*)&ter[c];
                }
            }
            if (HitCnt != 0) {
                *(s16*)LastWrite = HitCnt;
                *(short*)((s32)LastWrite + 2) = *ttemp;
                LastWrite = (short*)HitData;
                HitData++;
                HitCnt = 0;
            }
        }
    }
    *(s16*)LastWrite = 0;
    *(s16*)((s32)LastWrite + 2) = 0;
}

//NGC MATCH
float NewCast(struct nuvec_s* pos, float ytol) {
    short* CurData;
    struct scaleterrain_s** curter;
    struct scaleterrain_s* ter;
    short castnum2;
    short ecastnum2;
    float ht;
    float ht2;
    struct nuvec_s norm;
    struct nuvec_s norm2;
    struct scaleterrain_s* terhit1;
    struct scaleterrain_s* terhit2;
    float eht;
    float eht2;
    struct nuvec_s enorm;
    struct nuvec_s enorm2;
    struct scaleterrain_s* eterhit1;
    struct scaleterrain_s* eterhit2;
    s32 objnum;
    s32 lp;
    s32 t;
    s32 line;
    float tx;
    float ty;
    float tz;

    ht = -2000000.0f;
    ht2 = 2000000.0f;
    terhit1 = NULL;
    terhit2 = NULL;
    eht = -2000000.0f;
    eht2 = 2000000.0f;
    eterhit1 = NULL;
    eterhit2 = NULL;
    tx = 0.0f;
    tz = 0.0f;
    castnum2 = -1;
    castnum = -1;
    ecastnum2 = -1;
    ecastnum = -1;
    CurData = (short*)TerI->hitdata;
    while (*(short*)CurData > 0) {
        tx = pos->x - CurTerr->terr[*(short*)((s32)CurData + 2)].Location.x;
        tz = pos->z - CurTerr->terr[*(short*)((s32)CurData + 2)].Location.z;
        objnum = *(short*)((s32)CurData + 2);
        curter = (struct scaleterrain_s**)(CurData + 2);
        lp = *(short*)CurData;
        for (; lp > 0; lp--, curter++) {
            ter = (struct scaleterrain_s*)*curter;
            if (ter->norm[1].y > 65535.0f) {
                t = 0;
                if (ter->norm[0].y > 0.0f) {
                    if (((InsideLineF(tx, tz, ter->pnts[1].x, ter->pnts[1].z, ter->pnts[0].x, ter->pnts[0].z) != 0)
                         && (InsideLineF(tx, tz, ter->pnts[0].x, ter->pnts[0].z, ter->pnts[2].x, ter->pnts[2].z) != 0))
                        && (InsideLineF(tx, tz, ter->pnts[2].x, ter->pnts[2].z, ter->pnts[1].x, ter->pnts[1].z) != 0))
                    {
                        t = 1;
                    }
                } else {
                    if ((((ter->norm[0].y < 0.0f)
                          && (InsideLineF(tx, tz, ter->pnts[0].x, ter->pnts[0].z, ter->pnts[1].x, ter->pnts[1].z) != 0))
                         && (InsideLineF(tx, tz, ter->pnts[2].x, ter->pnts[2].z, ter->pnts[0].x, ter->pnts[0].z) != 0))
                        && (InsideLineF(tx, tz, ter->pnts[1].x, ter->pnts[1].z, ter->pnts[2].x, ter->pnts[2].z) != 0))
                    {
                        t = 2;
                    }
                }
                if (t != 0) {
                    ty = (ter->norm[0].x * (tx - ter->pnts[0].x) + ter->norm[0].z * (tz - ter->pnts[0].z))
                        / -ter->norm[0].y;
                    ty += ter->pnts[0].y + CurTerr->terr[objnum].Location.y;
                    if (ter->info[1] != 0) {
                        if (((ty <= pos->y) && (ty > eht)) && (t == 1)) {
                            eht = ty;
                            eterhit1 = ter;
                            enorm = ter->norm[0];
                            ecastnum = objnum;
                        } else {
                            if ((((ty > pos->y) && (ty <= eht2)) && (ty < pos->y + ytol))
                                && ((ty != eht2 || (ter->norm[0].y < enorm2.y))))
                            {
                                eterhit2 = ter;
                                enorm2 = ter->norm[0];
                                eht2 = ty;
                                ecastnum2 = objnum;
                            }
                        }
                    } else if (((ty <= pos->y) && (ty > ht)) && (t == 1)) {
                        ht = ty;
                        terhit1 = ter;
                        norm = ter->norm[0];
                        castnum = objnum;
                    } else {
                        if ((((ty > pos->y) && (ty <= ht2)) && (ty < pos->y + ytol))
                            && ((ty != ht2 || (ter->norm[0].y < norm2.y))))
                        {
                            terhit2 = ter;
                            norm2 = ter->norm[0];
                            ht2 = ty;
                            castnum2 = objnum;
                        }
                    }
                }
            } else {
                t = 0;
                if (ter->norm[0].y > 0.0f) {
                    if (((InsideLineF(tx, tz, ter->pnts[1].x, ter->pnts[1].z, ter->pnts[0].x, ter->pnts[0].z) != 0)
                         && (InsideLineF(tx, tz, ter->pnts[0].x, ter->pnts[0].z, ter->pnts[2].x, ter->pnts[2].z) != 0))
                        && ((
                            InsideLineF(tx, tz, ter->pnts[3].x, ter->pnts[3].z, ter->pnts[1].x, ter->pnts[1].z) != 0
                            && (InsideLineF(tx, tz, ter->pnts[2].x, ter->pnts[2].z, ter->pnts[3].x, ter->pnts[3].z) != 0
                            )
                        )))
                    {
                        t = 1;
                    }
                } else {
                    if ((((ter->norm[0].y < 0.0f)
                          && (InsideLineF(tx, tz, ter->pnts[0].x, ter->pnts[0].z, ter->pnts[1].x, ter->pnts[1].z) != 0))
                         && (InsideLineF(tx, tz, ter->pnts[2].x, ter->pnts[2].z, ter->pnts[0].x, ter->pnts[0].z) != 0))
                        && ((
                            InsideLineF(tx, tz, ter->pnts[1].x, ter->pnts[1].z, ter->pnts[3].x, ter->pnts[3].z) != 0
                            && (InsideLineF(tx, tz, ter->pnts[3].x, ter->pnts[3].z, ter->pnts[2].x, ter->pnts[2].z) != 0
                            )
                        )))
                    {
                        t = 2;
                    }
                }
                if (t != 0) {
                    if (t == 1) {
                        line = InsideLineF(tx, tz, ter->pnts[2].x, ter->pnts[2].z, ter->pnts[1].x, ter->pnts[1].z);
                    } else {
                        line = InsideLineF(tx, tz, ter->pnts[1].x, ter->pnts[1].z, ter->pnts[2].x, ter->pnts[2].z);
                    }
                    if ((line != 0) || (ter->norm[1].y == 0.0f)) {
                        ty = (ter->norm[0].x * (tx - ter->pnts[0].x) + ter->norm[0].z * (tz - ter->pnts[0].z))
                            / -ter->norm[0].y;
                        ty += ter->pnts[0].y + CurTerr->terr[objnum].Location.y;
                        if (ter->info[1] != 0) {
                            if (((ty <= pos->y) && (ty > eht)) && (t == 1)) {
                                eterhit1 = ter;
                                eht = ty;
                                enorm = ter->norm[0];
                                ecastnum = objnum;
                            } else {
                                if (((ty > pos->y) && (ty <= eht2))
                                    && ((ty < pos->y + ytol && ((ty != eht2 || (ter->norm[0].y < enorm2.y))))))
                                {
                                    eterhit2 = ter;
                                    enorm2 = ter->norm[0];
                                    eht2 = ty;
                                    ecastnum2 = objnum;
                                }
                            }
                        } else if (((ty <= pos->y) && (ty > ht)) && (t == 1)) {
                            terhit1 = ter;
                            ht = ty;
                            norm = ter->norm[0];
                            castnum = objnum;
                        } else {
                            if (((ty > pos->y) && (ty <= ht2))
                                && ((ty < pos->y + ytol && ((ty != ht2 || (ter->norm[0].y < norm2.y))))))
                            {
                                terhit2 = ter;
                                norm2 = ter->norm[0];
                                ht2 = ty;
                                castnum2 = objnum;
                            }
                        }
                    } else {
                        ty = (ter->norm[1].x * (tx - ter->pnts[3].x) + ter->norm[1].z * (tz - ter->pnts[3].z))
                            / -ter->norm[1].y;
                        ty += ter->pnts[3].y + CurTerr->terr[objnum].Location.y;
                        if (ter->info[1] != 0) {
                            if (((ty <= pos->y) && (ty > eht)) && (t == 1)) {
                                eterhit1 = ter;
                                eht = ty;
                                enorm = ter->norm[1];
                                ecastnum = objnum;
                            } else {
                                if (((ty > pos->y) && (ty <= eht2))
                                    && ((ty < pos->y + ytol && ((ty != eht2 || (ter->norm[1].y < enorm2.y))))))
                                {
                                    eterhit2 = ter;
                                    enorm2 = ter->norm[1];
                                    eht2 = ty;
                                    ecastnum2 = objnum;
                                }
                            }
                        } else if (((ty <= pos->y) && (ty > ht)) && (t == 1)) {
                            terhit1 = ter;
                            ht = ty;
                            norm = ter->norm[1];
                            castnum = objnum;
                        } else {
                            if (((ty > pos->y) && (ty <= ht2))
                                && ((ty < pos->y + ytol && ((ty != ht2 || (ter->norm[1].y < norm2.y))))))
                            {
                                terhit2 = ter;
                                norm2 = ter->norm[1];
                                ht2 = ty;
                                castnum2 = objnum;
                            }
                        }
                    }
                }
            }
        }
        CurData = (short*)curter;
    }
    if ((eht2 < 2000000.0f) && (enorm2.y < 0.0f)) {
        eshadroofhit = 1;
        EShadRoofY = eht2;
        EShadRoofNorm.x = enorm2.x;
        EShadRoofNorm.y = enorm2.y;
        EShadRoofNorm.z = enorm2.z;
        EShadRoofPoly = eterhit2;
        ecastroofnum = ecastnum2;
    } else {
        EShadRoofY = 2000000.0f;
        EShadRoofPoly = NULL;
    }
    if ((eht2 < 2000000.0f) && (enorm2.y > 0.0f)) {
        eshadhit = 1;
        EShadY = eht2;
        EShadNorm.x = enorm2.x;
        EShadNorm.y = enorm2.y;
        EShadNorm.z = enorm2.z;
        EShadPoly = (struct scaleterrain_s*)eterhit2;
        ecastnum = ecastnum2;
    } else {
        if (eht > -2000000.0f) {
            eshadhit = 2;
            EShadY = eht;
            EShadNorm.x = enorm.x;
            EShadNorm.y = enorm.y;
            EShadNorm.z = enorm.z;
            EShadPoly = (struct scaleterrain_s*)eterhit1;
        } else {
            eshadhit = 3;
            EShadY = 2000000.0f;
            EShadNorm.y = 1.0f;
            EShadPoly = NULL;
        }
    }
    if ((ht2 < 2000000.0f) && (norm2.y < 0.0f)) {
        shadroofhit = 1;
        ShadRoofY = ht2;
        ShadRoofNorm.x = norm2.x;
        ShadRoofNorm.y = norm2.y;
        ShadRoofNorm.z = norm2.z;
        ShadRoofPoly = (struct scaleterrain_s*)terhit2;
        castroofnum = castnum2;
    } else {
        ShadRoofY = 2000000.0f;
        ShadRoofPoly = NULL;
    }
    if ((ht2 < 2000000.0f) && (norm2.y > 0.0f)) {
        shadhit = 1;
        pos->y = ht2;
        ShadNorm.x = norm2.x;
        ShadNorm.y = norm2.y;
        ShadNorm.z = norm2.z;
        ShadPoly = terhit2;
        castnum = castnum2;
    } else {
        if (ht > -2000000.0f) {
            shadhit = 2;
            ShadNorm.x = norm.x;
            ShadNorm.y = norm.y;
            ShadNorm.z = norm.z;
            ShadPoly = terhit1;
            pos->y = ht;
        } else {
            shadhit = 3;
            pos->y = 2000000.0f;
            ShadNorm.y = 1.0f;
            ShadPoly = NULL;
        }
    }
    return 0.0f;
}

//NGC MATCH
s32 CheckCylinder(s32 p1,s32 p2,s32 *spherechecks,s32 mask) {
  float a;
  float b;
  float dist;
  float t;
  float s;
  float len;
  struct nuvec_s D;
  struct nuvec_s A;
  struct nuvec_s O;
  struct nuvec_s tmp;
  
  if ((((((TerI->rotter.pnts[p1].x > TerI->size ) && (TerI->rotter.pnts[p2].x > TerI->size)) ||
        ((TerI->rotter.pnts[p1].x < -TerI->size && (TerI->rotter.pnts[p2].x < -TerI->size)))) ||
       ((TerI->rotter.pnts[p1].y > TerI->size && (TerI->rotter.pnts[p2].y > TerI->size)))) ||
      ((TerI->rotter.pnts[p1].y < -TerI->size && (TerI->rotter.pnts[p2].y < -TerI->size)))) ||
     (((TerI->rotter.pnts[p1].z < -TerI->size && (TerI->rotter.pnts[p2].z < -TerI->size)) ||
      ((TerI->rotter.pnts[p1].z > TerI->size + TerI->len &&
       (TerI->rotter.pnts[p2].z > TerI->size + TerI->len)))))) {
    *spherechecks = *spherechecks & mask;
    return 0;
  }
    A.x = TerI->rotter.pnts[p2].x - TerI->rotter.pnts[p1].x;
    A.y = TerI->rotter.pnts[p2].y - TerI->rotter.pnts[p1].y;
    a = A.x * A.x + A.y * A.y;
    if (a < 1e-12f) {
        if ((((TerI->rotter.pnts[p1].z <= 0.0f) && (TerI->rotter.pnts[p2].z >= 0.0f)) ||
                     ((TerI->rotter.pnts[p1].z >= 0.0f && (TerI->rotter.pnts[p2].z <= 0.0f)))) &&
                    (a = TerI->rotter.pnts[p2].x * TerI->rotter.pnts[p2].x +
                             TerI->rotter.pnts[p2].y * TerI->rotter.pnts[p2].y, a <= TerI->sizesq))  {
              a = 1.0f / NuFsqrt(a);
              TerI->hittype = 0x12;
              TerI->hittime = 0.0f;
              (TerI->hitnorm).x = -TerI->rotter.pnts[p2].x * (a);
              (TerI->hitnorm).y = -TerI->rotter.pnts[p2].y * (a);
              (TerI->hitnorm).z = 0.0f;
              return 1;
        }

        return 0;
    }
    b = (TerI->rotter.pnts[p1].x * A.y - TerI->rotter.pnts[p1].y * A.x);
    if (b * b > TerI->sizesq * a) {
            *spherechecks = *spherechecks & mask;
            return 0;
    }
        dist = b * b / a;
        A.z = TerI->rotter.pnts[p2].z - TerI->rotter.pnts[p1].z;
        len = NuFsqrt(A.x * A.x + A.y * A.y + A.z * A.z);
        a = 1.0f / len;
        A.x = A.x * a;
        A.y = A.y * a;
        A.z = A.z * a;
        D.x = -A.y;
        D.y = A.x;
        b = D.x * D.x + D.y * D.y;
        tmp.x = -TerI->rotter.pnts[p1].x;
        tmp.y = -TerI->rotter.pnts[p1].y;
        tmp.z = -TerI->rotter.pnts[p1].z;
        NuVecCross(&tmp,&tmp,&A);
        t = -(tmp.x * D.x + tmp.y * D.y) / (b);
        O.x = D.y * A.z;
        O.y = -(D.x * A.z);
        O.z = D.x * A.y - D.y * A.x;
        b = NuFsqrt(O.x * O.x + O.y * O.y + O.z * O.z);
        O.z = O.z / b;
        s = NuFsqrt(TerI->sizesq - dist) / O.z;
        if (s < 0.0f) {
           t = t + s;
         }
        else {
           t = t - s;
        }
        if ((((t >= 0.0f) && (t <= TerI->len)) &&
             (b = -TerI->rotter.pnts[p1].x * A.x + -TerI->rotter.pnts[p1].y * A.y +
                      (t - TerI->rotter.pnts[p1].z) * A.z, b > 0.0f)) && (b <= len)) {
            if (t / TerI->len < TerI->hittime) {
              TerI->hittype = 2;
              TerI->hittime = t / TerI->len;
              (TerI->hitnorm).x = -(TerI->rotter.pnts[p1].x + A.x * b);
              (TerI->hitnorm).y = -(TerI->rotter.pnts[p1].y + A.y * b);
              (TerI->hitnorm).z = t - (TerI->rotter.pnts[p1].z + A.z * b);
              *spherechecks = *spherechecks & mask;
               return 1;
             }
             *spherechecks = *spherechecks & mask;
            return 0;
        }
        if (((TerI->rotter.pnts[p1].z >= -TerI->size) || (TerI->rotter.pnts[p2].z >= -TerI->size)) &&
             ((TerI->rotter.pnts[p1].z <= TerI->size || (TerI->rotter.pnts[p2].z <= TerI->size)))) {
            a = (-A.x * TerI->rotter.pnts[p1].x - A.y * TerI->rotter.pnts[p1].y) -
                    A.z * TerI->rotter.pnts[p1].z;
            b = (TerI->rotter.pnts[p2].x - TerI->rotter.pnts[p1].x) * A.x +
                (TerI->rotter.pnts[p2].y - TerI->rotter.pnts[p1].y) * A.y +
                (TerI->rotter.pnts[p2].z - TerI->rotter.pnts[p1].z) * A.z;
            if ((a < 0.0f) || (a > b)) {
              return 0;
            }
            A.x = A.x * a + TerI->rotter.pnts[p1].x;
             A.y = A.y * a + TerI->rotter.pnts[p1].y;
            A.z = A.z * a + TerI->rotter.pnts[p1].z;
            a = A.x * A.x + A.y * A.y + A.z * A.z;
            if (a < TerI->sizesq) {
              a = 1.0f / NuFsqrt(a);
              TerI->hittype = 0x12;
              TerI->hittime = 0.0f;
              (TerI->hitnorm).x = -A.x * a;
               (TerI->hitnorm).y = -A.y * a;
               (TerI->hitnorm).z = -A.z * a;
               return 1;
            }
        }
    return 0;
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

//NGC MATCH
u32 HitPoly(float ps,float pe,float ps2,float pe2,struct scaleterrain_s *ter) {
  struct nuvec_s hitpos;
  float time;
  s32 hit;
  u32 check;
  s32 spherechecks;
  
  hit = 0;
  hitpos.x = (TerI->csx - TerI->size * ter->norm[0].x) - ter->pnts[0].x;
  hitpos.y = (TerI->csy - TerI->size * ter->norm[0].y) - ter->pnts[0].y;
  hitpos.z = (TerI->csz - TerI->size * ter->norm[0].z) - ter->pnts[0].z;
  check = 0;
  if ((ps > 0.0f) && (pe < 0.0f)) {
    time = ps / (ps - pe);
    hitpos.x = hitpos.x + (TerI->curvel).x * time;
    hitpos.y = hitpos.y + (TerI->curvel).y * time;
    hitpos.z = hitpos.z + (TerI->curvel).z * time;
    check = 1;
  }
  else {
    if ((ps < 0.0f) && (pe < 0.0f)) {
      time = 1e-09;
      hitpos.x = hitpos.x + -ps * ter->norm[0].x;
      hitpos.y = hitpos.y + -ps * ter->norm[0].y;
      hitpos.z = hitpos.z + -ps * ter->norm[0].z;
      check = 1;
    }
  }
  if (check) {
    if ((InsidePolLines(hitpos.x,hitpos.y,hitpos.z,ter->pnts[1].x - ter->pnts[0].x,ter->pnts[1].y - ter->pnts[0].y,ter->pnts[1].z - ter->pnts[0].z,
                           ter->pnts[2].x - ter->pnts[0].x,ter->pnts[2].y - ter->pnts[0].y,ter->pnts[2].z  - ter->pnts[0].z,&ter->norm[0]) != 0) 
    && (time <= TerI->hittime)) {
      if (ps > 0.0f) {
        TerI->hittype = 1;
      }
      else {
        TerI->hittype = 0x11;
      }
      TerI->hittime = time;
      (TerI->hitnorm).x = ter->norm[0].x;
      (TerI->hitnorm).y = ter->norm[0].y;
      (TerI->hitnorm).z = ter->norm[0].z;
      hit = 1;
    }
  }
  if (ter->norm[1].y < 65536.0f) {
    hitpos.x = (TerI->csx - TerI->size * ter->norm[1].x) - ter->pnts[3].x;
    hitpos.y = (TerI->csy - TerI->size * ter->norm[1].y) - ter->pnts[3].y;
    hitpos.z = (TerI->csz - TerI->size * ter->norm[1].z) - ter->pnts[3].z;
    check = 0;
    if ((ps2 > 0.0f) && (pe2 < 0.0f)) {
      time = ps2 / (ps2 - pe2);
      hitpos.x = hitpos.x + (TerI->curvel).x * time;
      hitpos.y = hitpos.y + (TerI->curvel).y * time;
      hitpos.z = hitpos.z + (TerI->curvel).z * time;
      check = 1;
    }
    else {
      if ((ps2 < 0.0f) && (pe2 < 0.0f)) {
        time = 1e-09;
        hitpos.x = hitpos.x + -ps2 * ter->norm[1].x;
        hitpos.y = hitpos.y + -ps2 * ter->norm[1].y;
        hitpos.z = hitpos.z + -ps2 * ter->norm[1].z;
        check = 1;
      }
    }
    if (check) {
      if ((InsidePolLines(hitpos.x,hitpos.y,hitpos.z,ter->pnts[2].x - ter->pnts[3].x, ter->pnts[2].y - ter->pnts[3].y,ter->pnts[2].z - ter->pnts[3].z,
                             ter->pnts[1].x - ter->pnts[3].x,ter->pnts[1].y - ter->pnts[3].y,
                             ter->pnts[1].z - ter->pnts[3].z,&ter->norm[1]) != 0) && (time < TerI->hittime)) {
        if (ps2 > 0.0f) {
          TerI->hittype = 1;
          TerI->hittime = time;
          (TerI->hitnorm).x = ter->norm[1].x;
          (TerI->hitnorm).y = ter->norm[1].y;
          (TerI->hitnorm).z = ter->norm[1].z;
        }
        else {
          TerI->hittype = 0x11;
          TerI->hittime = time;
          (TerI->hitnorm).x = ter->norm[1].x;
          (TerI->hitnorm).y = ter->norm[1].y;
          (TerI->hitnorm).z = ter->norm[1].z;
        }
        hit = 1;
      }
    }
  }
  if (TerI->size != 0.0f) {
    spherechecks = 0xf;
    DeRotateTerrain(ter);
    hit = hit | CheckCylinder(0,1,&spherechecks,0xc);
    hit = hit | CheckCylinder(1,2,&spherechecks,9);
    hit = hit | CheckCylinder(2,0,&spherechecks,10);
    if (ter->norm[1].y < 65536.0f) {
      hit = hit | CheckCylinder(1,3,&spherechecks,5);
      hit = hit | CheckCylinder(3,2,&spherechecks,3);
      if ((spherechecks & 8U) != 0) {
        hit = hit | CheckSphere(3);
      }
    }
    if ((spherechecks & 1U) != 0) {
      hit = hit | CheckSphere(0);
    }
    if ((spherechecks & 2U) != 0) {
      hit = hit | CheckSphere(1);
    }
    if ((spherechecks & 4U) != 0) {
      hit = hit | CheckSphere(2);
    }
  }
  if (hit != 0) {
    TerI->hitter = ter;
  }
  return hit;
}

//NGC MATCH
int HitWallSpline(void) {
  int lp;
  float vellen;
  float pe;
  float vellen2;
  float size2;
  struct nuvec_s tnorm;
  struct nuvec_s tnorm2;
  struct nuvec_s tpos;
  float time2;
  float hit;
  float time;
  
  tnorm.y = 0.0f;
  if (WallSplCount == 0) {
    return 0;
  }
  else {
    hit = 0.0f;
    for (lp = 0; lp < WallSplCount; lp += 2) {
      if ((NuFabs((TerI->curpos.x - WallSplList[lp].x)) < 64.0f) && (NuFabs((TerI->curpos.z - WallSplList[lp].z)) < 64.0f)) {
        tnorm.x = WallSplList[lp + 1].z - WallSplList[lp].z;
        tnorm.z = WallSplList[lp].x - WallSplList[lp + 1].x;
        NuVecNorm(&tnorm,&tnorm);
        pe = (((TerI->curpos.x + TerI->curvel.x) - WallSplList[lp].x) * tnorm.x +
               ((TerI->curpos.z + TerI->curvel.z) - WallSplList[lp].z) * tnorm.z) - TerI->size;
        vellen = ((TerI->curpos.x - WallSplList[lp].x) * tnorm.x +
                 (TerI->curpos.z - WallSplList[lp].z) * tnorm.z) - TerI->size;
        if (((pe < 0.0f) || (vellen < 0.0f)) && (vellen > -TerI->size)) {
          tnorm2.x = WallSplList[lp + 1].x - WallSplList[lp].x;
          tnorm2.z = WallSplList[lp + 1].z - WallSplList[lp].z;
          if (vellen < 0.0f) {
            time = 9.9999997E-10f;
            tpos.x = (TerI->curpos.x - tnorm.x * vellen) - TerI->size * tnorm.x;
            tpos.z = (TerI->curpos.z - tnorm.z * vellen) - TerI->size * tnorm.z;
          }
          else {
            time = vellen / (vellen - pe);
            tpos.x = (TerI->curpos.x + TerI->curvel.x * time) - TerI->size * tnorm.x;
            tpos.z = (TerI->curpos.z + TerI->curvel.z * time) - TerI->size * tnorm.z;
          }
        vellen2 = (tpos.x - WallSplList[lp].x) * tnorm2.x + (tpos.z - WallSplList[lp].z) * tnorm2.z;
        if(vellen2 >= -0.000049999999f) {
                size2 = (-tnorm2.x * ((tpos.x - WallSplList[lp + 1].x)) -  (tpos.z - WallSplList[lp + 1].z) * tnorm2.z);
                  if ((size2 >= -0.000049999999f) && (time < TerI->hittime)) {
                    if (vellen > 0.0f) {
                      TerI->hittype = 1;
                    }
                    else {
                      TerI->hittype = 0x11;
                    }
                    TerI->hittime = time;
                    TerI->hitnorm.x = tnorm.x;
                    TerI->hitnorm.y = tnorm.y;
                    TerI->hitnorm.z = tnorm.z;
                    TerI->hitterrno = -1;
                    hit = 1.0f;
                  }
            }
        }
    vellen2 = (f32) ((TerI->size + 0.005f + TerI->vellen) * (TerI->size + 0.005f + TerI->vellen));
    if ((NuFabs(WallSplList[lp].x - TerI->curpos.x) < 64.0f) && (NuFabs(WallSplList[lp].z - TerI->curpos.z) < 64.0f) && ((((WallSplList[lp].x - TerI->curpos.x) * (WallSplList[lp].x - TerI->curpos.x)) + ((WallSplList[lp].z - TerI->curpos.z) * (WallSplList[lp].z - TerI->curpos.z))) < vellen2)) {
          tnorm2.x = TerI->curvel.x;
          tnorm2.y = 0.0f;
          tnorm2.z = TerI->curvel.z;
          NuVecNorm(&tnorm2,&tnorm2);
          vellen2 = (WallSplList[lp].x - TerI->curpos.x) * tnorm2.x + (WallSplList[lp].z - TerI->curpos.z) * tnorm2.z;
          time2 = -1.0f;
          if (vellen2 >= 0.0f) {
            tpos.x = TerI->curpos.x + (tnorm2.x * vellen2);
            tpos.z = TerI->curpos.z + (tnorm2.z * vellen2);
            if ((TerI->size + 0.0005f) * (TerI->size + 0.0005f) >=
                (WallSplList[lp].x - tpos.x) * (WallSplList[lp].x - tpos.x) +
                (WallSplList[lp].z - tpos.z) * (WallSplList[lp].z - tpos.z)) {
              size2 = NuFsqrt((TerI->size * TerI->size -
                               (WallSplList[lp].x - tpos.x) * (WallSplList[lp].x - tpos.x)) -
                               (WallSplList[lp].z - tpos.z) * (WallSplList[lp].z - tpos.z));
              vellen2 = vellen2 - size2;
              if ((vellen2 >= -0.0005f) && (vellen2 * vellen2 <= TerI->curvel.x * TerI->curvel.x + TerI->curvel.z * TerI->curvel.z)) {
                size2 = NuFsqrt(TerI->curvel.x * TerI->curvel.x + TerI->curvel.z * TerI->curvel.z);
                time2 = vellen2 / size2;
                if (time2 < 0.0f) {
                  time2 = 0.0f;
                }
                if (time2 < TerI->hittime) {
                  tnorm.x = (TerI->curpos.x + tnorm2.x * vellen2) - WallSplList[lp].x;
                  tnorm.z = (TerI->curpos.z + tnorm2.z * vellen2) - WallSplList[lp].z;
                  NuVecNorm(&tnorm,&tnorm);
                  TerI->hittype = 1;
                  TerI->hittime = time2;
                  TerI->hitnorm.x = tnorm.x;
                  TerI->hitnorm.y = tnorm.y;
                  TerI->hitnorm.z = tnorm.z;
                  TerI->hitterrno = -1;
                  hit = 1.0f;
                }
            }
              else {
                if ((TerI->size + 0.0005f) * (TerI->size + 0.0005f) >= (WallSplList[lp].x - TerI->curpos.x) * (WallSplList[lp].x - TerI->curpos.x) +
                    (WallSplList[lp].z - TerI->curpos.z) * (WallSplList[lp].z - TerI->curpos.z)) {
                  tnorm.x = TerI->curpos.x - WallSplList[lp].x;
                  tnorm.z = TerI->curpos.z - WallSplList[lp].z;
                  NuVecNorm(&tnorm,&tnorm);
                  TerI->hittype = 0x11;
                  TerI->hittime = 0.0f;
                  TerI->hitnorm.x = tnorm.x;
                  TerI->hitnorm.y = tnorm.y;
                  TerI->hitnorm.z = tnorm.z;
                  hit = 1.0f;
                }
              }
            }
            else {
              if ((TerI->size + 0.0005f) * (TerI->size + 0.0005f) >= (WallSplList[lp].x - TerI->curpos.x) * (WallSplList[lp].x - TerI->curpos.x) +
                  (WallSplList[lp].z - TerI->curpos.z) * (WallSplList[lp].z - TerI->curpos.z)) {
                tnorm.x = TerI->curpos.x - WallSplList[lp].x;
                tnorm.z = TerI->curpos.z - WallSplList[lp].z;
                NuVecNorm(&tnorm,&tnorm);
                TerI->hittype = 0x11;
                TerI->hittime = 0.0f;
                TerI->hitnorm.x = tnorm.x;
                TerI->hitnorm.y = tnorm.y;
                TerI->hitnorm.z = tnorm.z;
                hit = 1.0f;
              }
            }
          }
          else {
            if ((TerI->size + 0.0005f) * (TerI->size + 0.0005f) >= (WallSplList[lp].x - TerI->curpos.x) * (WallSplList[lp].x - TerI->curpos.x) +
                (WallSplList[lp].z - TerI->curpos.z) * (WallSplList[lp].z - TerI->curpos.z)) {
              tnorm.x = TerI->curpos.x - WallSplList[lp].x;
              tnorm.z = TerI->curpos.z - WallSplList[lp].z;
              NuVecNorm(&tnorm,&tnorm);
              TerI->hittype = 0x11;
              TerI->hittime = 0.0f;
              TerI->hitnorm.x = tnorm.x;
              TerI->hitnorm.y = tnorm.y;
              TerI->hitnorm.z = tnorm.z;
              hit = 1.0f;
            }
          }
        }
      }
    }
  }
  return hit;
}

//NGC MATCH
s32 HitTerrain() {
  short *CurData;
  struct scaleterrain_s **currter;
  struct scaleterrain_s *ter;
  struct nuvec_s pos;
  float ps;
  float pe;
  float ps2;
  float pe2;
  float size;
  s32 lp;
  s32 check;
  s32 hit;
  
  CurData = (short*)TerI->hitdata;
  size = TerI->size;
  hit = 0;
  TerI->hittype = 0;
  TerI->hittime = 999.9f;
  HitWallSpline();
  TerI->vellen = NuFsqrt((TerI->curvel).x * (TerI->curvel).x + (TerI->curvel).z * (TerI->curvel).z);
loop1: 
asm("nop");
    while (0 < *CurData) {
      TerI->csx = (TerI->curpos).x - CurTerr->terr[*(CurData + 1)].Location.x;
      TerI->csy = (TerI->curpos).y - CurTerr->terr[*(CurData + 1)].Location.y;
      TerI->csz = (TerI->curpos).z - CurTerr->terr[*(CurData + 1)].Location.z;
      TerI->cex = ((TerI->curpos).x + (TerI->curvel).x) -
                  CurTerr->terr[*(CurData + 1)].Location.x;
      TerI->cey = ((TerI->curpos).y + (TerI->curvel).y) -
                  CurTerr->terr[*(CurData + 1)].Location.y;
      TerI->cez = ((TerI->curpos).z + (TerI->curvel).z) -
                  CurTerr->terr[*(CurData + 1)].Location.z;
      currter = (struct scaleterrain_s **)CurData + 1;
      for (lp = *CurData; lp > 0; lp--, currter++) {
        ter = *currter;
        check = 0;
        pe = ((ter->norm[0].x * (TerI->cex - ter->pnts[0].x) +
               ter->norm[0].y * (TerI->cey - ter->pnts[0].y) +
              ter->norm[0].z * (TerI->cez - ter->pnts[0].z)) - size) - TerI->impactadj;
        if (pe < 0.0f) {
            ps = (ter->norm[0].x * (TerI->csx - ter->pnts[0].x) + ter->norm[0].y * (TerI->csy - ter->pnts[0].y) +
                    ter->norm[0].z * (TerI->csz - ter->pnts[0].z)) - size;
            if (ps > -size) {
              check = 1;
            }
        }
        if (((ter->norm[1].y < 65536.0f) &&
            (pe2 = ((ter->norm[1].x * (TerI->cex - ter->pnts[3].x) +
                    ter->norm[1].y * (TerI->cey - ter->pnts[3].y) +
                   ter->norm[1].z * (TerI->cez - ter->pnts[3].z)) - size) - TerI->impactadj,
            pe2 < 0.0f)) &&
           (ps2 = (ter->norm[1].x * (TerI->csx - ter->pnts[3].x) +
                  ter->norm[1].y * (TerI->csy - ter->pnts[3].y) +
                 ter->norm[1].z * (TerI->csz - ter->pnts[3].z)) - size, ps2 > -size)) {
          check = 1;
        }
        if ((check != 0) && (HitPoly(ps,pe,ps2,pe2,ter) != 0)) {
          TerI->hitterrno = *(CurData + 1);
          hit = 1;
          if ((TerI->hitter->info[2] & 0x80) != 0) {
            PlatCrush = TerI->hitter->info[0] + 1;
          }
        }
      } 
      CurData = (short *)currter;
    }
    if (*CurData < 0) {
      while (*CurData < 0) {
         CurData += ((*CurData >= 0) ? *CurData  : -*CurData) * 2 + 2; //correct??
      }
        goto loop1; //check
    } 
        asm("nop");
  for (lp = 0; lp < curSphereter; lp++) {
    pos = SphereData[lp].pos;
    DeRotatePoint(&pos);
    hit = hit | CheckSphereTer(&pos,SphereData[lp].radius);
  }
  if (((TerI->hittype != 0) && (TerI->hitterrno != -1)) &&
     (CurTerr->terr[TerI->hitterrno].type == TERR_TYPE_PLATFORM)) {
    plathitid = CurTerr->terr[TerI->hitterrno].info;
  }
  return hit;
}

//NGC MATCH
void ScanTerrain(s32 platscan, s32 extramask) {
    s32 a;
    s32 b;
    s32 c;
    s32 HitCnt;
    struct scaleterrain_s** HitData;
    struct scaleterrain_s** MaxData;
    short* LastWrite;
    short* ttemp;
    struct wallspl_s* WallSpl;
    float maxx;
    float maxy;
    float maxz;
    float tmaxx;
    float tmaxy;
    float tmaxz;
    float radmov;
    float minx;
    float miny;
    float minz;
    float tminx;
    float tminy;
    float tminz;
    float tn;
    struct terr_s* mbuf2;
    struct scaleterrain_s* ter;
    short* modp;
    s32 curscltemp;
    struct nuvec4_s pnts[4];
    struct nuvec4_s norm[2];
    
    curscltemp = 0;
    ScaleTerrain = ScaleTerrainT1;
    TerI->hitcnt = 0;
    LastWrite = (short*)TerI->hitdata;
    HitData = TerI->hitdata + 1;
    MaxData = HitData + 0x1fc;
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

                    modp = (short*)mbuf2->model;
                    while (*modp >= 0) {
                        c = modp[1];
                        ter = (struct scaleterrain_s *)(modp + 10);
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
                                modp = (short*)ter;
                            } else {
                                modp = (short*)(ter + c);
                            }
                        } else {
                            modp = (short*)(ter + c);
                        }
                    }
                    if (HitCnt != 0) {
                        LastWrite[0] = HitCnt;
                        LastWrite[1] = *ttemp;
                        LastWrite = (short*)HitData;
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
                modp = (short*)mbuf2->model;
                while (*modp >= 0) {
                    c = modp[1];
                    ter = (struct scaleterrain_s *)(modp + 10);
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
                            modp = (short*)ter;
                        }
                        else {
                            modp = (short*)(ter + c);
                        }
                    }
                    else {
                        modp = (short*)(ter + c);
                    }
                }
                
                if (HitCnt != 0) {
                    LastWrite[0] = HitCnt;
                    LastWrite[1] = a + 0x800;
                    LastWrite = (short*)HitData;
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
                    modp = (short *)mbuf2->model;
                    while (*modp >= 0) {
                        c = modp[1];
                        ter = (struct scaleterrain_s *)(modp + 10);
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
                        modp = (short *)ter;
                    }
                    
                    if (HitCnt != 0) {
                        LastWrite[0] = HitCnt;
                        LastWrite[1] = *ttemp;
                        LastWrite = (short *)HitData;
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
                modp = (short *)mbuf2->model;
                while (*modp >= 0) {
                    c = modp[1];
                    ter = (struct scaleterrain_s *)(modp + 10);
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
                            modp = (short *)ter;
                        }
                        else {
                            modp = (short *)(ter + c);
                        }
                    }
                    else {
                        modp = (short *)(ter + c);
                    }
                }
                
                if (HitCnt != 0) {
                    LastWrite[0] = HitCnt;
                    LastWrite[1] = *ttemp;
                    LastWrite = (short *)HitData;
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

    for (WallSpl = (struct wallspl_s *)CurTerr->wallinfo; WallSpl != 0; WallSpl = (struct wallspl_s *)((int*)WallSpl)[-1]) { //check
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

//NGC MATCH
void FullDeflect(struct nuvec_s *N,struct nuvec_s *L,struct nuvec_s *R) {
  register float dotp;
  
  dotp = (N->x * -L->x + N->y * -L->y + N->z * -L->z) + 0.001f;
  R->x = (N->x * dotp) + L->x;
  R->y = (N->y * dotp) + L->y;
  R->z = (N->z * dotp) + L->z;
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

//NGC MATCH
void FullReflect(struct nuvec_s *N,struct nuvec_s *L,struct nuvec_s *R) {
  register float dotp;
  
  dotp = (N->x * -L->x + N->y * -L->y + N->z * -L->z);
  R->x = (2.0f * (N->x * dotp)) + L->x;
  R->y = (2.0f * (N->y * dotp)) + L->y;
  R->z = (2.0f * (N->z * dotp)) + L->z;
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

//NGC MATCH
void TerrainImpactNorm() {
    float rad;

    TerrainMoveImpactData();
    switch (TerI->hittype) {
    default:

    break;
    case 4:
    case 20:
    terrhitflags |= 4;
    case 2:
    case 3:
    case 18:
    case 19:
    RotateVec(&TerI->hitnorm,&TerI->hitnorm);
    if ((TerI->hittype & 0x10U) == 0) {
        TerI->hitnorm.x = TerI->hitnorm.x * TerI->sizediv;
        TerI->hitnorm.y = TerI->hitnorm.y * TerI->sizediv;
        TerI->hitnorm.z = TerI->hitnorm.z * TerI->sizediv;
    }
    case 1:
    case 17:
          if (TerI->yscale == 1.0f) {
            TerI->uhitnorm.x = TerI->hitnorm.x;
            TerI->uhitnorm.y = TerI->hitnorm.y;
            TerI->uhitnorm.z = TerI->hitnorm.z;
          }
          else {
            rad = 1.0f / NuFsqrt(TerI->hitnorm.x * TerI->hitnorm.x +
                            TerI->hitnorm.y * TerI->hitnorm.y * TerI->inyscalesq +
                            TerI->hitnorm.z * TerI->hitnorm.z);
            TerI->uhitnorm.x = TerI->hitnorm.x * rad;
            TerI->uhitnorm.y = TerI->hitnorm.y * TerI->inyscale * rad;
            TerI->uhitnorm.z = TerI->hitnorm.z * rad;
          }
        break;
    }
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

//NGC MATCH
int TerrainImpactPlatform(unsigned char *flags)
{
    TerrainMoveImpactData();
    
    switch (TerI->hittype) 
    {
    case 0:
        flags[0] = 0;
        
        TerI->curpos.x += TerI->curvel.x;
        TerI->curpos.y += TerI->curvel.y;
        TerI->curpos.z += TerI->curvel.z;
        return 0;
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
        
        TerI->curpos.x += TerI->curvel.x * TerI->hittime;
        TerI->curpos.y += TerI->curvel.y * TerI->hittime;
        TerI->curpos.z += TerI->curvel.z * TerI->hittime;
        
        if (0.707f > TerI->uhitnorm.y) 
        {
            flags[0] = 0;
        } 
        else 
        {
            flags[0] = 1;
            flags[1] = 1;
            
            TerI->curpos.y += TerI->hitnorm.y * 0.002f;
        }
        
        return 0;
        break;
    case 17:
    case 18:
    case 19:
    case 20: 
        return 1;
        break;
    }

    return 1;
}

//NGC MATCH
s32 TerrainPlatformMoveCheck(struct nuvec_s *dest,struct nuvec_s *norm,s32 platid,s32 calcimpact,s32 leaveoffonfail) {
  short *CurData;
  struct nuvec_s curvel;
  short hitcnt;
  short hitterrno;
  short hittype;
  short plathit;
  float hittime;
  struct nuvec_s hitnorm;
  struct nuvec_s uhitnorm;
  s32 terrid;
  
  terrid = CurTerr->platdata[platid].terrno;
  CurData = TerI->PlatScanStart;
  while (*CurData > 0) {
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
  TerI->curvel.x = dest->x - TerI->curpos.x;
  TerI->curvel.y = dest->y - TerI->curpos.y;
  TerI->curvel.z = dest->z - TerI->curpos.z;
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
  TerI->curvel = curvel;
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

//NGC MATCH
s32 TerrainPlatformEmbedded(struct nuvec_s* vvel) {
    struct teri_s* TerITemp;
    struct teri_s* TerITemp2;
    s32 oldcurSphereter;
    char flags[2];
    s32 curscltemp;
    struct terr_s* mbuf2;
    struct scaleterrain_s** HitData;
    struct scaleterrain_s* ter;
    short* LastWrite;
    short* modp;
    s32 platid;
    s32 c;
    s32 HitCnt;
    float len;
    float ty;
    struct nuvec4_s pnts[4];
    struct nuvec4_s norm[2];

    curscltemp = 0;
    platid = (s32)CurTerr->terr[TerI->hitterrno].info;
    if (CurTerr->platdata[platid].curmtx != NULL) {
        if (((CurTerr->platdata[platid].oldmtx._30 == (CurTerr->platdata[platid].curmtx)->_30)
             && (CurTerr->platdata[platid].oldmtx._31 == (CurTerr->platdata[platid].curmtx)->_31))
            && (CurTerr->platdata[platid].oldmtx._32 == (CurTerr->platdata[platid].curmtx)->_32))
        {
            if ((s32)CurTerr->platdata[platid].status.rotate == 0) {
                return 1;
            }
            else if (((((CurTerr->platdata[platid].oldmtx._00 == (CurTerr->platdata[platid].curmtx)->_00)
                   && (CurTerr->platdata[platid].oldmtx._01 == (CurTerr->platdata[platid].curmtx)->_01))
                  && ((
                      CurTerr->platdata[platid].oldmtx._02 == (CurTerr->platdata[platid].curmtx)->_02
                      && ((
                          CurTerr->platdata[platid].oldmtx._10 == (CurTerr->platdata[platid].curmtx)->_10
                          && (CurTerr->platdata[platid].oldmtx._11 == (CurTerr->platdata[platid].curmtx)->_11)
                      ))
                  )))
                 && (CurTerr->platdata[platid].oldmtx._12 == (CurTerr->platdata[platid].curmtx)->_12))
                && ((
                    (CurTerr->platdata[platid].oldmtx._20 == (CurTerr->platdata[platid].curmtx)->_20
                     && (CurTerr->platdata[platid].oldmtx._21 == (CurTerr->platdata[platid].curmtx)->_21))
                    && (CurTerr->platdata[platid].oldmtx._22 == (CurTerr->platdata[platid].curmtx)->_22)
                )))
            {
                return 1;
            }
        }
        TerITemp = TerI;
        oldcurSphereter = curSphereter;
        curSphereter = 0;
        ScaleTerrain = ScaleTerrainT2;
        TerI = (struct teri_s*)NuScratchAlloc32(0x930);
        TerI->yscale = TerITemp->yscale;
        TerI->yscalesq = TerITemp->yscalesq;
        TerI->inyscale = TerITemp->inyscale;
        TerI->inyscalesq = TerITemp->inyscalesq;
        TerI->size = TerITemp->size;
        TerI->sizediv = TerITemp->sizediv;
        TerI->sizesq = TerITemp->sizesq;
        TerI->id = TerITemp->id;
        TerI->timeadj = TerITemp->timeadj;
        TerI->impactadj = TerITemp->impactadj;
        TerI->flags = TerITemp->flags;
        TerI->scanmode = TerITemp->scanmode;
        TerI->stopflag = TerITemp->stopflag;
        TerI->curvel.x = (CurTerr->platdata[platid].oldmtx._30 - (CurTerr->platdata[platid].curmtx)->_30) * 1.3f;
        TerI->curvel.y = (CurTerr->platdata[platid].oldmtx._31 - (CurTerr->platdata[platid].curmtx)->_31) * 1.3f
            * TerI->inyscale;
        TerI->curvel.z = (CurTerr->platdata[platid].oldmtx._32 - (CurTerr->platdata[platid].curmtx)->_32) * 1.3f;
        if (CurTerr->platdata[platid].status.rotate != 0) {
            tertempmat = CurTerr->platdata[platid].oldmtx;
            tertempvec4.x = TerITemp->curpos.x;
            tertempvec4.y = TerITemp->curpos.y * TerI->yscale;
            tertempvec4.z = TerITemp->curpos.z;
            tertempvec4.w = 1.0f;
            NuVec4MtxInvTransform(&tertempvec4, (struct nuvec_s*)&tertempvec4, &tertempmat);
            tertempmat = *CurTerr->platdata[platid].curmtx;
            NuVec4MtxTransformVU0(&tertempvec4, &tertempvec4, &tertempmat);
            tertempvec4.y = tertempvec4.y * TerI->inyscale;
            TerI->curvel.x = (TerITemp->curpos.x - tertempvec4.x) * 1.3f;
            TerI->curvel.y = (TerITemp->curpos.y - tertempvec4.y) * 1.3f;
            TerI->curvel.z = (TerITemp->curpos.z - tertempvec4.z) * 1.3f;
        }
        TerI->curpos.x = TerITemp->curpos.x - TerI->curvel.x * 0.91f;
        TerI->curpos.y = TerITemp->curpos.y - TerI->curvel.y * 0.91f;
        TerI->curpos.z = TerITemp->curpos.z - TerI->curvel.z * 0.91f;
        LastWrite = (short*)TerI->hitdata;
        HitData = TerI->hitdata + 1;
        HitCnt = NULL;
        mbuf2 = &CurTerr->terr[TerITemp->hitterrno];
        modp = mbuf2->model;
        if (TerI->yscale == 1.0f) {
            while (*(short*)modp >= 0) {
                for (c = (s32) * ((short*)modp + 1), ter = (struct scaleterrain_s*)(modp + 10); c > 0; c--) {
                    *HitData = ter;
                    HitData++;
                    HitCnt++;
                    ter++;
                }
                modp = (short *)ter;
            }
        } else if ((s32)CurTerr->platdata[platid].status.rotate != 0) {
            while (*(short*)modp >= 0) {
                for (c = (s32) * ((short*)modp + 1), ter = (struct scaleterrain_s*)(modp + 10); c > 0; c--) {
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
                    NuVec4MtxTransformVU0(norm, norm, CurTerr->platdata[mbuf2->info].curmtx);
                    if (ter->norm[1].y < 65535.0f) {
                        pnts[3].x = ter->pnts[3].x;
                        pnts[3].y = ter->pnts[3].y;
                        pnts[3].z = ter->pnts[3].z;
                        pnts[3].w = 0.0f;
                        NuVec4MtxTransformVU0(&pnts[3], &pnts[3], CurTerr->platdata[mbuf2->info].curmtx);
                        norm[1].x = ter->norm[1].x;
                        norm[1].y = ter->norm[1].y;
                        norm[1].z = ter->norm[1].z;
                        norm[1].w = 0.0f;
                        NuVec4MtxTransformVU0(norm + 1, norm + 1, CurTerr->platdata[mbuf2->info].curmtx);
                    } else {
                        pnts[3] = pnts[2];
                    }
                    ScaleTerrain[curscltemp].info[0] = ter->info[0];
                    ScaleTerrain[curscltemp].info[1] = ter->info[1];
                    ScaleTerrain[curscltemp].info[2] = ter->info[2];
                    ScaleTerrain[curscltemp].info[3] = ter->info[3];
                    ScaleTerrain[curscltemp].pnts[0].x = pnts[0].x;
                    ScaleTerrain[curscltemp].pnts[0].z = pnts[0].z;
                    ScaleTerrain[curscltemp].pnts[0].y = (pnts[0].y + mbuf2->Location.y) * TerI->inyscale - mbuf2->Location.y;
                    ScaleTerrain[curscltemp].pnts[1].x = pnts[1].x;
                    ScaleTerrain[curscltemp].pnts[1].z = pnts[1].z;
                    ScaleTerrain[curscltemp].pnts[1].y = (pnts[1].y + mbuf2->Location.y) * TerI->inyscale - mbuf2->Location.y;
                    ScaleTerrain[curscltemp].pnts[2].x = pnts[2].x;
                    ScaleTerrain[curscltemp].pnts[2].z = pnts[2].z;
                    ScaleTerrain[curscltemp].pnts[2].y = (pnts[2].y + mbuf2->Location.y) * TerI->inyscale - mbuf2->Location.y;
                    if (norm[1].y < 65535.0f) {
                        ScaleTerrain[curscltemp].pnts[3].x = pnts[3].x;
                        ScaleTerrain[curscltemp].pnts[3].z = pnts[3].z;
                        ScaleTerrain[curscltemp].pnts[3].y =
                            (pnts[3].y + mbuf2->Location.y) * TerI->inyscale - mbuf2->Location.y;
                        len =
                            1.0f
                            / NuFsqrt(
                                norm[1].x * norm[1].x + norm[1].y * norm[1].y * TerI->yscalesq + norm[1].z * norm[1].z
                            );
                        ScaleTerrain[curscltemp].norm[1].x = norm[1].x * len;
                        ScaleTerrain[curscltemp].norm[1].y = norm[1].y * TerI->yscale * len;
                        ScaleTerrain[curscltemp].norm[1].z = norm[1].z * len;
                    } else {
                        ScaleTerrain[curscltemp].norm[1].y = 65536.0f;
                    }
                    len = 1.0f
                        / NuFsqrt(
                                norm[0].x * norm[0].x + norm[0].y * norm[0].y * TerI->yscalesq + norm[0].z * norm[0].z
                        );
                    ScaleTerrain[curscltemp].norm[0].x = norm[0].x * len;
                    ScaleTerrain[curscltemp].norm[0].y = norm[0].y * TerI->yscale * len;
                    ScaleTerrain[curscltemp].norm[0].z = norm[0].z * len;
                    *HitData = ScaleTerrain + curscltemp;
                    HitData++;
                    HitCnt++;
                    curscltemp++;
                    ter++;
                }
                modp = (short*)ter;
            }
        } else {
            while (*(short*)modp >= 0) {
                for (c = (s32) * ((short*)modp + 1), ter = (struct scaleterrain_s*)(modp + 10); c > 0; c--) {
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
                    if (ter->norm[1].y < 65535.0f) {
                        ScaleTerrain[curscltemp].pnts[3].x = ter->pnts[3].x;
                        ScaleTerrain[curscltemp].pnts[3].z = ter->pnts[3].z;
                        ScaleTerrain[curscltemp].pnts[3].y =
                            (ter->pnts[3].y + mbuf2->Location.y) * TerI->inyscale - mbuf2->Location.y;
                        len = 1.0f
                            / NuFsqrt(
                                    ter->norm[1].x * ter->norm[1].x
                                    + ter->norm[1].y * ter->norm[1].y * TerI->yscalesq
                                    + ter->norm[1].z * ter->norm[1].z
                            );
                        ScaleTerrain[curscltemp].norm[1].x = ter->norm[1].x * len;
                        ScaleTerrain[curscltemp].norm[1].y = ter->norm[1].y * TerI->yscale * len;
                        ScaleTerrain[curscltemp].norm[1].z = ter->norm[1].z * len;
                    } else {
                        ScaleTerrain[curscltemp].norm[1].y = 65536.0f;
                    }
                    len = 1.0f
                        / NuFsqrt(
                                ter->norm[0].x * ter->norm[0].x
                                + ter->norm[0].y * ter->norm[0].y * TerI->yscalesq
                                + ter->norm[0].z * ter->norm[0].z
                        );
                    ScaleTerrain[curscltemp].norm[0].x = ter->norm[0].x * len;
                    ScaleTerrain[curscltemp].norm[0].y = ter->norm[0].y * TerI->yscale * len;
                    ScaleTerrain[curscltemp].norm[0].z = ter->norm[0].z * len;
                    *HitData = &ScaleTerrain[curscltemp];
                    HitData++;
                    HitCnt++;
                    curscltemp++;
                    ter++;
                }
                modp = (short*)ter;
            }
        }
        if (HitCnt != NULL) {
            *(s16*)LastWrite = HitCnt;
            *(short*)(LastWrite + 1) = TerITemp->hitterrno;
            LastWrite = (short *)HitData;
            HitData++;
            HitCnt = 0;
        }
        *(short*)LastWrite = 0;
        *(s16*)(LastWrite + 1) = 0;
        flags[0] = 0;
        flags[1] = 0;
        DerotateMovementVector();
        HitTerrain();
        TerrainImpactNorm();
        if (TerrainImpactPlatform((u8*)flags) != 0) {
            NuScratchRelease();
            ScaleTerrain = ScaleTerrainT1;
            curSphereter = oldcurSphereter;
            TerI = TerITemp;
            return 1;
        }
        TerITemp2 = TerI;
        TerI = TerITemp;
        TerITemp2->origpos = TerITemp2->curpos;
        NewCast(&TerITemp2->origpos, 5.0f);
        flags[0] = 0;
        if (TerITemp2->origpos.y >= 2000000.0f) {
            TerI->curpos.y = TerITemp2->curpos.y;
        } else {
            ty = ((TerITemp2->origpos.y + TerITemp2->size) - TerITemp2->curpos.y) + 0.0025f;
            if (ty < -0.008f) {
            } else if ((ty < 0.009f)) {
                TerITemp2->curpos.y = TerITemp2->curpos.y + ty;
                flags[0] = 1;
            }
            else if ((ty - 0.0025f) * (ty - 0.0025f)
                        < (TerITemp2->curvel).x * (TerITemp2->curvel).x + (TerITemp2->curvel).z * (TerITemp2->curvel).z)
            {
                TerITemp2->curpos.y = TerITemp2->curpos.y + ty;
                flags[0] = 1;
            }
        }
        if (TerrainPlatformMoveCheck(&TerITemp2->curpos, NULL, platid, 1, 1) == 0) {
            flags[0] = 0;
        }
        TerI->curpos.x = TerITemp2->curpos.x;
        TerI->curpos.y = TerITemp2->curpos.y;
        TerI->curpos.z = TerITemp2->curpos.z;
        if ((flags[0] != 0) && (CurTerr->terr[castnum].type == TERR_TYPE_PLATFORM)) {
            TerI->flags[1] = 1;
            PlatformConnect((char*)TerITemp->flags, &TerITemp->curvel, vvel, (s32)CurTerr->terr[castnum].info);
        } else {
            if (CurTrackInfo != NULL) {
                CurTrackInfo->platinf = CurTrackInfo->platinf & 0xfffe;
            }
        }
        NuScratchRelease();
        ScaleTerrain = ScaleTerrainT1;
        curSphereter = oldcurSphereter;
        TerI = TerITemp;
        return 0;
    } else {
        return 1;
    }
}

//NGC MATCH
void StorePlatImpact(void) {
  struct nuvec_s hitnorm;
  float rad;
  
  if (TerI->hittype == 0 || TerI->hitterrno == -1) {
      return;
  } 
  if (CurTerr->terr[TerI->hitterrno].type != TERR_TYPE_PLATFORM) {
    return;
  }
  hitnorm = (TerI->hitnorm);
  switch (TerI->hittype) {
    default:
      break;
    case 0x12:
    case 0x13:
    case 0x14:
    case 2:
    case 3:
    case 4:
         RotateVec(&hitnorm,&hitnorm);
          if ((TerI->hittype & 0x10U) == 0) {
            hitnorm.x = hitnorm.x * TerI->sizediv;
            hitnorm.y = hitnorm.y * TerI->sizediv;
            hitnorm.z = hitnorm.z * TerI->sizediv;
          }
    case 1:
    case 0x11:
      if (TerI->yscale != 1.0f) {
        rad = 1.0f / NuFsqrt(hitnorm.x * hitnorm.x + hitnorm.y * hitnorm.y * TerI->inyscalesq + hitnorm.z * hitnorm.z);
        hitnorm.x = hitnorm.x * rad;
        hitnorm.y = TerI->inyscale * rad * hitnorm.y;
        hitnorm.z = hitnorm.z * rad;
      }
      PlatImpactId = (int)CurTerr->terr[TerI->hitterrno].info;
      PlatImpactNorm = hitnorm;
      PlatImpactTer = *TerI->hitter;
      break;
  }
}

//NGC MATCH
s32 PlatformChecks(s32 itterationcnt, struct nuvec_s* vvel) {
    struct teri_s* TerITemp;
    struct teri_s* TerITemp2;
    short* CurData;
    short* WriteData;
    s32 lp;
    s32 platid;
    s32 result;
    struct nuvec4_s pnts[2];
    float dist;
    float dist2;
    char flags[2];

    if (TerI->PlatScanStart == NULL) {
        return itterationcnt;
    }
    if (*TerI->PlatScanStart == 0) {
        return itterationcnt;
    }
    if (CurTrackInfo == NULL) {
        return itterationcnt;
    }
    TerITemp = TerI;
    TerI = TerITemp2 = (struct teri_s*)NuScratchAlloc32(0x930);
    CurData = TerITemp->PlatScanStart;
    WriteData = (short*)&TerI->hitdata[0];

    while (*CurData > 0) {
        lp = *WriteData = *CurData;
        WriteData[1] = CurData[1];
        WriteData += 2;
        CurData += 2;

        while (lp > 0) {
            *WriteData++ = *CurData++;
            *WriteData++ = *CurData++;
            lp--;
        }
    }
    WriteData[0] = 0;
    WriteData[1] = 0;
    TerI->size = TerITemp->size;
    TerI->yscale = TerITemp->yscale;
    TerI->inyscalesq = TerITemp->inyscalesq;
    TerI->sizediv = TerITemp->sizediv;
    TerI->timeadj = TerITemp->timeadj;
    TerI->inyscale = TerITemp->inyscale;
    TerI->impactadj = TerITemp->impactadj;
    TerI->tempvec[0] = TerITemp->curpos;
    if ((CurTrackInfo != NULL) && ((CurTrackInfo->platinf & 1U) != 0)) {
        if (CurTerr->platdata[CurTrackInfo->platid].status.rotate != 0) {
            tertempmat = CurTerr->platdata[CurTrackInfo->platid].oldmtx;
            tertempvec4.x = TerITemp->curpos.x;
            tertempvec4.y = TerITemp->curpos.y * TerI->yscale;
            tertempvec4.z = TerITemp->curpos.z;
            tertempvec4.w = 1.0f;
            NuVec4MtxInvTransform(&tertempvec4, (struct nuvec_s*)&tertempvec4, &tertempmat);
            tertempmat = *CurTerr->platdata[CurTrackInfo->platid].curmtx;
            NuVec4MtxTransformVU0(&tertempvec4, &tertempvec4, &tertempmat);
            tertempvec4.y = tertempvec4.y * TerI->inyscale;
            TerI->tempvec[0].x += (tertempvec4.x - TerITemp->curpos.x);
            TerI->tempvec[0].y += (tertempvec4.y - TerITemp->curpos.y);
            TerI->tempvec[0].z += (tertempvec4.z - TerITemp->curpos.z);
        } else {
            TerI->tempvec[0].x +=
                ((CurTerr->platdata[CurTrackInfo->platid].curmtx)->_30
                 - CurTerr->platdata[CurTrackInfo->platid].oldmtx._30);
            TerI->tempvec[0].y += ((CurTerr->platdata[CurTrackInfo->platid].curmtx)->_31
                                   - CurTerr->platdata[CurTrackInfo->platid].oldmtx._31)
                * TerITemp->inyscale;
            TerI->tempvec[0].z +=
                ((CurTerr->platdata[CurTrackInfo->platid].curmtx)->_32
                 - CurTerr->platdata[CurTrackInfo->platid].oldmtx._32);
        }
    }
    TerI->curpos = TerI->tempvec[1] = TerI->tempvec[0];
    TerI->curvel.x = 0.0f;
    TerI->curvel.z = 0.0f;
    TerI->curpos.y += 0.2f;
    TerI->curvel.y = -0.4f;
    DerotateMovementVector();
    HitTerrain();
    StorePlatImpact();
    TerI->hittime = TerI->hittime + TerI->timeadj;
    TerrainImpactNorm();
    TerrainImpactPlatform((unsigned char*)&flags[0]);
    TerI->tempvec[0].y = TerI->curpos.y - TerI->size;
    TerI = TerITemp;
    if (TerITemp2->hittype == 0) {
        NuScratchRelease();
        return itterationcnt;
    }
    if (NuFabs(((TerITemp2->tempvec[1].y - TerITemp2->tempvec[0].y) - TerI->size)) > 0.05f) {
        NuScratchRelease();
        return itterationcnt;
    }
    if (TerITemp2->hitnorm.y < 0.707f) {
        NuScratchRelease();
        return itterationcnt;
    }

    platid = (s32)CurTerr->terr[TerITemp2->hitterrno].info;
    if ((vvel->y > 0.04f)
        && (vvel->y > ((CurTerr->platdata[platid].curmtx)->_31 + 0.04f) - CurTerr->platdata[platid].oldmtx._31))
    {
        NuScratchRelease();
        return itterationcnt;
    }
    if ((TerITemp2->hittype != 0)
        && ((
            (TerITemp2->hitnorm.y > 0.707f && (TerITemp2->hitterrno >= 0))
            && (CurTerr->terr[TerITemp2->hitterrno].type == TERR_TYPE_PLATFORM)
        )))
    {
        CurTerr->platdata[CurTerr->terr[TerITemp2->hitterrno].info].status.hit = 1;
    }
    TerI->tempvec[0].x = TerITemp2->tempvec[0].x;
    if (CurTerr->platdata[CurTrackInfo->platid].status.rotate != 0) {
        TerI->tempvec[0].y = TerITemp2->tempvec[0].y + TerI->size;
    } else {
        TerI->tempvec[0].y = TerITemp2->tempvec[0].y + TerI->size + 0.0025f;
    }
    TerI->tempvec[0].z = TerITemp2->tempvec[0].z;
    pnts[0].x = TerI->tempvec[0].x - TerI->curpos.x;
    pnts[0].y = TerI->tempvec[0].y - TerI->curpos.y;
    pnts[0].z = TerI->tempvec[0].z - TerI->curpos.z;
    pnts[0].w = NuFsqrt(pnts[0].x * pnts[0].x + pnts[0].y * pnts[0].y + pnts[0].z * pnts[0].z);
    pnts[0].x = pnts[0].x / pnts[0].w;
    pnts[0].y = pnts[0].y / pnts[0].w;
    pnts[0].z = pnts[0].z / pnts[0].w;
    pnts[1].x = TerI->curpos.x;
    pnts[1].y = TerI->curpos.y;
    pnts[1].z = TerI->curpos.z;
    do {
        result = TerrainPlatformMoveCheck(&TerI->tempvec[0], &TerI->tempvec[1], platid, 1, 0);
        if (((TerI->hittype != 0) && (TerI->hitterrno >= 0))
            && (CurTerr->terr[TerI->hitterrno].type == TERR_TYPE_PLATFORM))
        {
            CurTrackInfo->platid = CurTerr->terr[TerI->hitterrno].info;
        }
        itterationcnt--;
        if (pnts[0].x * TerI->tempvec[1].x + pnts[0].y * TerI->tempvec[1].y + pnts[0].z * TerI->tempvec[1].z < -0.7f) {
            result = 1;
        }
        if ((result == 0) && (itterationcnt > 0)) {
            dist = (TerI->curpos.x - pnts[1].x) * pnts[0].x + (TerI->curpos.y - pnts[1].y) * pnts[0].y
                + (TerI->curpos.z - pnts[1].z) * pnts[0].z;
            if (dist < pnts[0].w) {
                if (FullDeflectTest(&TerI->tempvec[1], (struct nuvec_s*)&pnts[0], &TerI->tempvec[0]) != 0) {
                    dist2 = pnts[0].x * TerI->tempvec[0].x + pnts[0].y * TerI->tempvec[0].y
                        + pnts[0].z * TerI->tempvec[0].z;
                    if (dist2 > 0.0f) {
                        dist2 = (pnts[0].w - dist) / dist2;
                        if (dist2 < 0.8f) {
                            TerI->tempvec[0].x = TerI->tempvec[0].x * dist2 + TerI->curpos.x;
                            TerI->tempvec[0].y = TerI->tempvec[0].y * dist2 + TerI->curpos.y;
                            TerI->tempvec[0].z = TerI->tempvec[0].z * dist2 + TerI->curpos.z;
                            dist = 1.0f;
                        } else {
                            dist = 0.0f;
                        }
                    } else {
                        dist = 0.0f;
                    }
                } else {
                    dist = 0.0f;
                }
            } else {
                dist = 0.0f;
            }
        }
    } while (((result == 0) && (itterationcnt > 0)) && (dist != 0.0f));
    if (result != 0) {
        TerrPoly = TerITemp2->hitter;
        TerI->curpos = TerI->tempvec[0];
        if (vvel->y < 0.0f) {
            FullDeflect(&ShadNorm, &TerI->curvel, &TerI->curvel);
            FullDeflect(&ShadNorm, vvel, vvel);
        }
        TerI->flags[1] = 1;
        PlatformConnect((char*)TerITemp->flags, &TerITemp->curvel, vvel, (s32)CurTerr->terr[TerITemp2->hitterrno].info);
    }
    NuScratchRelease();
    TerI = TerITemp;
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

//NGC MATCH
int TerrShapeSideStep(struct nuvec_s* vpos, struct nuvec_s* vvel, u8* flags) {
    struct nuvec_s slide;
    struct nuvec_s temp;
    float dotp;
    int doslide;
    u8 tflags[2];

    switch (TerI->hittype) {
        case 0:
            return 1;
        default:
            if ((TerI->uhitnorm.y > 0.707f) || (TerI->uhitnorm.y < -0.8f)) {
                return 1;
            }
            doslide = 0;
            break;
        case 2:
        case 3:
            if (TerI->uhitnorm.y < 0.5f) {
                doslide = 0;
            } else {
                doslide = 1;
            }
            break;
    }
    slide.x = NuTrigTable[(int)(TerrShape->ang + 16384.0f) & 0xffff];
    slide.y = 0.0f;
    slide.z = -NuTrigTable[(int)TerrShape->ang & 0xffff];
    TerI->origpos.x = TerI->curpos.x;
    TerI->origpos.y = TerI->curpos.y;
    TerI->origpos.z = TerI->curpos.z;
    TerI->origvel.x = TerI->curvel.x;
    TerI->origvel.y = TerI->curvel.y;
    TerI->origvel.z = TerI->curvel.z;
    TerI->curvel.y = 0.0f;
    if (doslide == 1) {
        TerI->curvel.x = TerI->hitnorm.x * TerrShape->size * 0.15f;
        TerI->curvel.z = TerI->hitnorm.z * TerrShape->size * 0.15f;
        TerrShape->offset.x = TerrShape->offset.x + TerI->hitnorm.x * TerrShape->size * 0.05f;
        TerrShape->offset.z = TerrShape->offset.z + TerI->hitnorm.z * TerrShape->size * 0.05f;
    } else {
        dotp = slide.x * TerI->uhitnorm.x + slide.z * TerI->uhitnorm.z;
        if ((dotp == 0.0f)
            && (dotp = (TerrShape->offset.x * slide.x + TerrShape->offset.z * slide.z) / TerrShape->size, dotp == 0.0f))
        {
            return 1;
        }
        if (0.0f > dotp) {
            dotp = -NuFsqrt(-dotp);
        } else {
            dotp = NuFsqrt(dotp);
        }
        TerI->curvel.x = dotp * slide.x * TerrShape->size + TerrShape->offset.x;
        TerI->curvel.z = dotp * slide.z * TerrShape->size + TerrShape->offset.z;
    }
    do {
        DerotateMovementVector();
        HitTerrain();
        TerrainImpactNorm();
        TerrainImpact(&temp, &temp, tflags);
        TerrShapeAdjCnt--;
    } while (TerrShapeAdjCnt > 0 && TerI->hittype != 0);
    if (TerI->hittype == 0) {
        TerI->curpos.x = TerI->curpos.x + TerI->curvel.x;
        TerI->curpos.y = TerI->curpos.y + TerI->curvel.y;
        TerI->curpos.z = TerI->curpos.z + TerI->curvel.z;
    }
    TerrShape->offset.x = TerrShape->offset.x + (TerI->origpos.x - TerI->curpos.x);
    TerrShape->offset.z = TerrShape->offset.z + (TerI->origpos.z - TerI->curpos.z);
    TerI->hittype = 1;
    TerI->curvel.x = TerI->origvel.x;
    TerI->curvel.y = TerI->origvel.y;
    TerI->curvel.z = TerI->origvel.z;
    TerrainSideClamp(&slide, &TerI->curpos);
    return 0;
}

//PS2 MATCH //check Gamecube
void CubeImpact(struct numtx_s *mat,struct numtx_s *nmat,struct nuvec_s *norm,float size,struct nuvec_s *impact) {
  s32 lp;
  struct nuvec_s cube[8] = {1.0f, 1.0f, 1.0f};
  struct nuvec4_s pnts[10];
  float best;
  s32 bestnum;
  float dota;
  float dotb;
  
    best = 10000.0f;
    bestnum = 0.0f;
  for (lp = 0; (s32)lp < 8; lp++) {
    pnts[lp].x = cube[lp].x;
    pnts[lp].y = cube[lp].y;
    pnts[lp].z = cube[lp].z;
    pnts[lp].w = 0.0f;
    NuVec4MtxTransformVU0(&pnts[8],&pnts[lp],nmat);
    NuVec4MtxTransformVU0(&pnts[lp],&pnts[lp],mat);
    dota = pnts[lp].x * norm->x + pnts[lp].y * norm->y + pnts[lp].z * norm->z;
    dotb = pnts[8].x * norm->x + pnts[8].y * norm->y + pnts[8].z * norm->z;
    if ((dota > dotb) && (dota < best)) {
      best = dota;
      bestnum = lp;
    }
  }
  impact->x = mat->_30 + pnts[bestnum].x * size;
  impact->y = mat->_31 + pnts[bestnum].y * size;
  impact->z = mat->_32 + pnts[bestnum].z * size;
}

//NGC MATCH
void NewTerrainScaleY(
    struct nuvec_s* vpos, struct nuvec_s* vvel, u8* flags, s32 terid, float stopflag, float size, float yscale
) {
    s32 cnt;
    s32 normhit;
    s32 plathitcnt;

    if (CurTerr == NULL) {
        return;
    }
    plathitid = -1;
    TerrPolyObj = -1;
    TerrPoly = NULL;
    PlatCrush = 0;
    terrhitflags = 0;
    CurTrackInfo = ScanTerrId(flags);
    TerI = (struct teri_s*)NuScratchAlloc32(0x930);
    TerI->yscale = yscale;
    TerI->yscalesq = TerI->yscale * TerI->yscale;
    TerI->inyscale = 1.0f / yscale;
    TerI->inyscalesq = TerI->inyscale * TerI->inyscale;
    TerI->size = size;
    TerI->sizediv = 1.0f / TerI->size;
    TerI->sizesq = TerI->size * TerI->size;
    TerI->origpos.x = TerI->curpos.x = vpos->x;
    TerI->origpos.y = TerI->curpos.y = vpos->y + (TerI->size * yscale);
    TerI->origpos.z = TerI->curpos.z = vpos->z;
    TerI->origvel.x = TerI->curvel.x = vvel->x;
    TerI->origvel.y = TerI->curvel.y = vvel->y;
    TerI->origvel.z = TerI->curvel.z = vvel->z;
    TerI->id = terid;
    TerI->stopflag = stopflag;
    TerI->flags = flags;
    TerI->scanmode = 0;
    TerI->timeadj = 0.01f;
    TerI->impactadj = 0.0000099999997f;
    ScanTerrain(1, 0);
    if ((((flags[1] != 0) && (NuFabs(vvel->x) < stopflag)) && (NuFabs(vvel->y) < stopflag))
        && ((NuFabs(vvel->z) < stopflag && (platinrange == 0))))
    {
        NuScratchRelease();
        TerrFlush();
        return;
    }
    TerI->curpos.y = TerI->curpos.y * TerI->inyscale;
    TerI->curvel.y = TerI->curvel.y * TerI->inyscale;
    *flags = 0;
    flags[1] = 0;
    cnt = PlatformChecks(4, vvel);
    plathitcnt = 0;
    do {
        DerotateMovementVector();
        HitTerrain();
        StorePlatImpact();
        if (((TerI->hittype > 0x10) && (TerI->hitterrno != -1))
            && (CurTerr->terr[TerI->hitterrno].type == TERR_TYPE_PLATFORM))
        {
            cnt--;
            normhit = TerrainPlatformEmbedded(vvel);
        } else {
            normhit = 1;
        }
        if (normhit != 0) {
            cnt--;
            TerrainImpactNorm();
            if (TerI->hittype != 0) {
                ShadNorm = TerI->uhitnorm;
                if (((TerI->uhitnorm.y > 0.707f) && (TerI->hitterrno >= 0))
                    && (CurTerr->terr[TerI->hitterrno].type == TERR_TYPE_PLATFORM))
                {
                    CurTerr->platdata[CurTerr->terr[TerI->hitterrno].info].status.hit = 1;
                }
            }
            if (TerrShapeAdjCnt != 0) {
                if (TerrShapeSideStep(vpos, vvel, flags) != NULL) {
                    TerrainImpact(vpos, vvel, flags);
                }
            } else {
                TerrainImpact(vpos, vvel, flags);
            }
        }
    } while (((TerI->hittype != 0) && (cnt > 0))
             && (TerI->hitnorm.x * TerI->hitnorm.x + TerI->hitnorm.y * TerI->hitnorm.y
                     + TerI->hitnorm.z * TerI->hitnorm.z
                 <= 1.5f));
    if (TerI->hittype != 0) {
        vpos->x = TerI->curpos.x;
        vpos->y = TerI->curpos.y * TerI->yscale - TerI->size * TerI->yscale;
        vpos->z = TerI->curpos.z;
    }
    if (testlock != 0) {
        vpos->x = TerI->origpos.x;
        vpos->z = TerI->origpos.z;
    }
    NuScratchRelease();
    TerrFlush();
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
s32 NewRayCastMask(struct nuvec_s* vpos, struct nuvec_s* vvel, float size, s32 mask) {
    s32 hit;

    if (CurTerr == NULL) {
        return 0;
    }
    plathitid = -1;
    TerrPolyObj = -1;
    TerrPoly = NULL;
    TerI = (struct teri_s*)NuScratchAlloc32(0x930);
    TerI->inyscalesq = TerI->inyscale = TerI->yscalesq = TerI->yscale = 1.0f;
    TerI->size = size;
    TerI->sizediv = 1.0f / TerI->size;
    TerI->sizesq = TerI->size * TerI->size;
    TerI->origpos.x = TerI->curpos.x = vpos->x;
    TerI->origpos.y = TerI->curpos.y = vpos->y;
    TerI->origpos.z = TerI->curpos.z = vpos->z;
    TerI->origvel = TerI->curvel = *vvel;
    TerI->id = -1;
    TerI->flags = NULL;
    TerI->scanmode = 1;
    TerI->timeadj = 0.01f;
    TerI->impactadj = 0.0000099999997f;
    ScanTerrain(0, mask);
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

//NGC MATCH
short* NewScanHandelFull(struct nuvec_s* vpos, struct nuvec_s* vvel, float size, s32 platscan, s32 extramask) {
    s32 a;
    s32 b;
    s32 c;
    s32 HitCnt;
    struct scaleterrain_s** HitData;
    struct scaleterrain_s** MaxData;
    short* LastWrite;
    short* ttemp;
    short* scandata;
    struct wallspl_s* WallSpl;
    struct nuvec_s** walldata;
    float maxx;
    float maxy;
    float maxz;
    float tmaxx;
    float tmaxy;
    float tmaxz;
    float minx;
    float miny;
    float minz;
    float tminx;
    float tminy;
    float tminz;
    struct terr_s* mbuf2;
    struct scaleterrain_s* ter;
    short* modp;
    struct nuvec4_s pnts[4];
    struct nuvec4_s norm[2];
    float radmov;
    float tn;
    s32 curscltemp;

    curscltemp = 0;
    ScaleTerrain = ScaleTerrainT1;
    scandata = LastWrite = (short*)TempStackPtr;
    HitData = (struct scaleterrain_s**)(LastWrite + 2);
    MaxData = (struct scaleterrain_s**)(HitData + 0x1fc);
    if (HitData > (struct scaleterrain_s**)debkeydata + 0x2d931) {
        return 0;
    }
    if ((struct scaleterrain_s**)(debkeydata + 0x2dac0) < MaxData) {
        MaxData = (struct scaleterrain_s**)(debkeydata + 0x2dac0);
    }
    HitCnt = 0;
    platinrange = 0;
    if (vvel->x > 0.0f) {
        minx = vpos->x - 0.02f - size;
        maxx = vpos->x + vvel->x + 0.02f + size;
    } else {
        minx = vpos->x + vvel->x - 0.02f - size;
        maxx = vpos->x + 0.02f + size;
    }
    if (vvel->y > 0.0f) {
        miny = (vpos->y - 0.02f) - size;
        maxy = vpos->y + vvel->y + 0.02f + size;
    } else {
        miny = (vpos->y + vvel->y - 0.02f) - size;
        maxy = vpos->y + 0.02f + size;
    }
    if (vvel->z > 0.0f) {
        minz = vpos->z - 0.02f - size;
        maxz = vpos->z + vvel->z + 0.02f + size;
    } else {
        minz = vpos->z + vvel->z - 0.02f - size;
        maxz = (vpos->z + 0.02f) + size;
    }
    radmov = (minx - maxx) * (minx - maxx) + (miny - maxy) * (miny - maxy) + (minz - maxz) * (minz - maxz);
    radmov += 0.02f;
    for (a = 0; a < CurTerr->terrgcnt; a++) {
        if ((((maxx >= CurTerr->terrgroup[a].minx) && (maxz >= CurTerr->terrgroup[a].minz))
             && (minx <= CurTerr->terrgroup[a].maxx))
            && (minz <= CurTerr->terrgroup[a].maxz))
        {
            ttemp = CurTerr->terrlist + CurTerr->terrgroup[a].tabindex;
            for (b = 0; b < CurTerr->terrgroup[a].count; b++, ttemp++) {
                mbuf2 = CurTerr->terr + *ttemp;
                if (((((maxx >= mbuf2->min.x) && (maxy >= mbuf2->min.y))
                      && ((maxz >= mbuf2->min.z && ((minx <= mbuf2->max.x && (miny < mbuf2->max.y))))))
                     && (minz < mbuf2->max.z))
                    && (mbuf2->type != ~TERR_TYPE_NORMAL))
                {
                    tmaxx = maxx - mbuf2->Location.x;
                    tmaxy = maxy - mbuf2->Location.y;
                    tmaxz = maxz - mbuf2->Location.z;
                    tminx = minx - mbuf2->Location.x;
                    tminy = miny - mbuf2->Location.y;
                    tminz = minz - mbuf2->Location.z;
                    modp = (short*)mbuf2->model;
                    while (*(short*)modp >= 0) {
                        c = (u32) * (short*)((s32)modp + 2);
                        ter = (struct scaleterrain_s*)(short*)(modp + 10);
                        if ((tmaxx >= *(float*)(modp + 2)) && (tminx < *(float*)(modp + 4))) {
                            if ((tmaxz >= *(float*)(modp + 6)) && (tminz < *(float*)(modp + 8))) {
                                for (; (s32)c > 0; c--) {
                                    if (((((tmaxx >= ter->minx) && (tminx < ter->maxx)) && (tmaxz >= ter->minz))
                                         && ((
                                             (tminz < ter->maxz && (tmaxy >= ter->miny))
                                             && ((tminy < ter->maxy && (HitData < MaxData)))
                                         )))
                                        && ((ter->info[1] == 0 || (((u32)ter->info[1] & extramask) != 0))))
                                    {
                                        *HitData = ter;
                                        HitData++;
                                        HitCnt++;
                                    }
                                    ter++;
                                }
                                modp = (short*)ter;
                            } else {
                                modp = (short*)&ter[c];
                            }
                        } else {
                            modp = (short*)&ter[c];
                        }
                    }
                    if (HitCnt != 0) {
                        *(short*)LastWrite = HitCnt;
                        *(short*)(LastWrite + 1) = *ttemp;
                        LastWrite = (short*)HitData;
                        HitData = HitData + 1;
                        HitCnt = 0;
                    }
                }
            }
        }
    }
    TerI->PlatScanStart = (short*)LastWrite;
    a = 0x100;
    minx = minx - 0.05f;
    maxx = maxx + 0.05f;
    miny = miny - 0.05f;
    maxy = maxy + 0.05f;
    minz = minz - 0.05f;
    maxz = maxz + 0.05f;
    if (platscan != 0) {
        ttemp = &CurTerr->terrlist[CurTerr->terrgroup[a].tabindex];
        for (b = 0; b < CurTerr->terrgroup[a].count; b++, ttemp++) {
            mbuf2 = CurTerr->terr + *ttemp;
            if (CurTerr->platdata[mbuf2->info].curmtx != NULL) {
                if ((CurTerr->platdata[mbuf2->info].curmtx)->_30 > CurTerr->platdata[mbuf2->info].oldmtx._30) {
                    tminx = minx - mbuf2->Location.x;
                    tmaxx = (((CurTerr->platdata[mbuf2->info].curmtx)->_30 - CurTerr->platdata[mbuf2->info].oldmtx._30)
                                 * 1.5f
                             + maxx)
                        - mbuf2->Location.x;
                } else {
                    tmaxx = maxx - mbuf2->Location.x;
                    tminx = (((CurTerr->platdata[mbuf2->info].curmtx)->_30 - CurTerr->platdata[mbuf2->info].oldmtx._30)
                                 * 1.5f
                             + minx)
                        - mbuf2->Location.x;
                }
                if ((CurTerr->platdata[mbuf2->info].curmtx)->_31 > CurTerr->platdata[mbuf2->info].oldmtx._31) {
                    tminy = miny - mbuf2->Location.y;
                    tmaxy = (((CurTerr->platdata[mbuf2->info].curmtx)->_31 - CurTerr->platdata[mbuf2->info].oldmtx._31)
                                 * 1.5f
                             + maxy)
                        - mbuf2->Location.y;
                } else {
                    tmaxy = maxy - mbuf2->Location.y;
                    tminy = (((CurTerr->platdata[mbuf2->info].curmtx)->_31 - CurTerr->platdata[mbuf2->info].oldmtx._31)
                                 * 1.5f
                             + miny)
                        - mbuf2->Location.y;
                }
                if ((CurTerr->platdata[mbuf2->info].curmtx)->_32 > CurTerr->platdata[mbuf2->info].oldmtx._32) {
                    tminz = minz - mbuf2->Location.z;
                    tmaxz =
                        (((CurTerr->platdata[mbuf2->info].curmtx)->_32 - CurTerr->platdata[mbuf2->info].oldmtx._32)
                             * 1.5f
                         + maxz)
                        - mbuf2->Location.z;
                } else {
                    tmaxz = maxz - mbuf2->Location.z;
                    tminz = (((CurTerr->platdata[mbuf2->info].curmtx)->_32 - CurTerr->platdata[mbuf2->info].oldmtx._32)
                                 * 1.5f
                             + minz)
                        - mbuf2->Location.z;
                }
            } else {
                tmaxx = maxx - mbuf2->Location.x;
                tminx = minx - mbuf2->Location.x;
                tmaxy = maxy - mbuf2->Location.y;
                tminy = miny - mbuf2->Location.y;
                tmaxz = maxz - mbuf2->Location.z;
                tminz = minz - mbuf2->Location.z;
            }
            if ((s32)CurTerr->platdata[mbuf2->info].status.rotate == 0) {
                if ((((tmaxx >= mbuf2->min.x) && (tmaxy >= mbuf2->min.y)) && (tmaxz >= mbuf2->min.z))
                    && ((
                        (tminx <= mbuf2->max.x && (tminy < mbuf2->max.y))
                        && ((tminz < mbuf2->max.z && (mbuf2->type != ~TERR_TYPE_NORMAL)))
                    )))
                {
                    modp = (short*)mbuf2->model;
                    while (*(short*)modp >= 0) {
                        c = (u32) * (short*)((s32)modp + 2);
                        ter = (struct scaleterrain_s*)(modp + 10);
                        if (tmaxx >= *(float*)(modp + 2) && (tminx < *(float*)(modp + 4))) {
                            if ((tmaxz >= *(float*)(modp + 6)) && (tminz < *(float*)(modp + 8))) {
                                for (; 0 < (s32)c; c--) {
                                    if ((((((tmaxx >= ter->minx) && (tminx < ter->maxx)) && (tmaxz >= ter->minz))
                                          && ((tminz < ter->maxz && (tmaxy >= ter->miny))))
                                         && (tminy < ter->maxy))
                                        && ((
                                            platinrange = 1,
                                            HitData < MaxData
                                                && ((ter->info[1] == 0 || (((u32)ter->info[1] & extramask) != 0)))
                                        )))
                                    {
                                        *HitData = ter;
                                        HitData++;
                                        HitCnt++;
                                    }
                                    ter++;
                                }
                                modp = (short*)ter;
                            } else {
                                modp = (short*)&ter[c];
                            }

                        } else {
                            modp = (short*)&ter[c];
                        }
                    }
                }
            } else {
                tn = ((maxx + minx) * 0.5f - mbuf2->Location.x) * ((maxx + minx) * 0.5f - mbuf2->Location.x)
                    + (((maxy + miny) * 0.5f - mbuf2->Location.y) * TerI->inyscale)
                        * (((maxy + miny) * 0.5f - mbuf2->Location.y) * TerI->inyscale)
                    + ((maxz + minz) * 0.5f - mbuf2->Location.z) * ((maxz + minz) * 0.5f - mbuf2->Location.z);
                if ((mbuf2->type != ~TERR_TYPE_NORMAL) && (tn < mbuf2->radius + radmov)) {
                    modp = (short*)mbuf2->model;
                    while (*(short*)modp >= 0) {
                        c = *(short*)(modp + 1);
                        ter = (struct scaleterrain_s*)(modp + 10);
                        for (; c > 0; c--) {
                            platinrange = 1;
                            if ((HitData < MaxData) && ((ter->info[1] == 0 || ((ter->info[1] & extramask) != 0)))) {
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
                                NuVec4MtxTransformVU0(pnts, pnts, CurTerr->platdata[mbuf2->info].curmtx);
                                NuVec4MtxTransformVU0(pnts + 1, pnts + 1, CurTerr->platdata[mbuf2->info].curmtx);
                                NuVec4MtxTransformVU0(pnts + 2, pnts + 2, CurTerr->platdata[mbuf2->info].curmtx);
                                norm[0].x = ter->norm[0].x;
                                norm[0].y = ter->norm[0].y;
                                norm[0].z = ter->norm[0].z;
                                norm[0].w = 0.0f;
                                NuVec4MtxTransformVU0(norm, norm, CurTerr->platdata[mbuf2->info].curmtx);
                                if (ter->norm[1].y < 65535.0f) {
                                    pnts[3].x = ter->pnts[3].x;
                                    pnts[3].y = ter->pnts[3].y;
                                    pnts[3].z = ter->pnts[3].z;
                                    pnts[3].w = 0.0f;
                                    NuVec4MtxTransformVU0(pnts + 3, pnts + 3, CurTerr->platdata[mbuf2->info].curmtx);
                                    norm[1].x = ter->norm[1].x;
                                    norm[1].y = ter->norm[1].y;
                                    norm[1].z = ter->norm[1].z;
                                    norm[1].w = 0.0f;
                                    NuVec4MtxTransformVU0(&norm[1], &norm[1], CurTerr->platdata[mbuf2->info].curmtx);
                                } else {
                                    pnts[3] = pnts[2];
                                }
                                ScaleTerrain[curscltemp].info[0] = ter->info[0];
                                ScaleTerrain[curscltemp].info[1] = ter->info[1];
                                ScaleTerrain[curscltemp].info[2] = ter->info[2];
                                ScaleTerrain[curscltemp].info[3] = ter->info[3];
                                ScaleTerrain[curscltemp].pnts[0].x = pnts[0].x;
                                ScaleTerrain[curscltemp].pnts[0].z = pnts[0].z;
                                ScaleTerrain[curscltemp].pnts[0].y = pnts[0].y;
                                ScaleTerrain[curscltemp].pnts[1].x = pnts[1].x;
                                ScaleTerrain[curscltemp].pnts[1].z = pnts[1].z;
                                ScaleTerrain[curscltemp].pnts[1].y = pnts[1].y;
                                ScaleTerrain[curscltemp].pnts[2].x = pnts[2].x;
                                ScaleTerrain[curscltemp].pnts[2].z = pnts[2].z;
                                ScaleTerrain[curscltemp].pnts[2].y = pnts[2].y;
                                if (ter->norm[1].y < 65535.0f) {
                                    ScaleTerrain[curscltemp].pnts[3].x = pnts[3].x;
                                    ScaleTerrain[curscltemp].pnts[3].z = pnts[3].z;
                                    ScaleTerrain[curscltemp].pnts[3].y = pnts[3].y;
                                    tn = 1.0f
                                        / NuFsqrt(
                                                norm[1].x * norm[1].x + norm[1].y * norm[1].y + norm[1].z * norm[1].z
                                        );
                                    ScaleTerrain[curscltemp].norm[1].x = norm[1].x * tn;
                                    ScaleTerrain[curscltemp].norm[1].y = norm[1].y * tn;
                                    ScaleTerrain[curscltemp].norm[1].z = norm[1].z * tn;
                                } else {
                                    ScaleTerrain[curscltemp].norm[1].y = 65536.0f;
                                }
                                tn = 1.0f
                                    / NuFsqrt(norm[0].x * norm[0].x + norm[0].y * norm[0].y + norm[0].z * norm[0].z);
                                ScaleTerrain[curscltemp].norm[0].x = norm[0].x * tn;
                                ScaleTerrain[curscltemp].norm[0].y = norm[0].y * tn;
                                ScaleTerrain[curscltemp].norm[0].z = norm[0].z * tn;
                                *HitData = &ScaleTerrain[curscltemp];
                                HitData++;
                                HitCnt++;
                                curscltemp++;
                            }
                            ter++;
                        }
                        modp = (short*)ter;
                    }
                }
            }
            if (HitCnt != 0) {
                *(short*)LastWrite = HitCnt;
                *(short*)(LastWrite + 1) = *ttemp;
                LastWrite = (short*)HitData;
                HitData++;
                HitCnt = 0;
            }
        }
    }
    *(short*)LastWrite = 0;
    *(short*)(LastWrite + 1) = 0;
    walldata = (struct nuvec_s **)(LastWrite + 2);
    minx = minx - 0.02f;
    minz = minz - 0.02f;
    maxx = maxx + 0.02f;
    maxz = maxz + 0.02f;
    for (WallSpl = (struct wallspl_s*)CurTerr->wallinfo; WallSpl != NULL;
         WallSpl = (*(struct wallspl_s**)((s32)WallSpl - 4)))
    {
        for (a = 0; a < *(unsigned short*)WallSpl; a += 0x10) {
            if (WallSpl->spl[a].y != 2.1474836E+9f) {
                if ((((WallSpl->spl[a + 1].y >= minx) && (WallSpl->spl[a].y <= maxx))
                     && (WallSpl->spl[a + 3].y >= minz))
                    && (WallSpl->spl[a + 2].y <= maxz))
                {
                    c = a + 0x10;
                    if (c > *(unsigned short*)WallSpl) {
                        c = *(unsigned short*)WallSpl;
                    }
                } else {
                    c = 0;
                }
            } else {
                c = *(unsigned short*)WallSpl;
            }
            asm("nop");
            for (b = a; b < (s32)c; b++) {
                if (((((WallSpl->spl[b].x >= minx) && (WallSpl->spl[b + 1].x <= maxx))
                      || ((WallSpl->spl[b + 1].x >= minx && (WallSpl->spl[b].x <= maxx))))
                     && ((
                         (WallSpl->spl[b].z >= minz && (WallSpl->spl[b + 1].z <= maxz))
                         || ((WallSpl->spl[b + 1].z >= minz && (WallSpl->spl[b].z <= maxz)))
                     )))
                    && (WallSplCount < 0x40))
                {
                    *walldata++ = &WallSpl->spl[b];
                }
            }
        }
    }
    LastWrite = (short*)walldata;
    *(short*)LastWrite = 0;
    *(short*)(LastWrite + 1) = 0;
    TempStackPtr = LastWrite + 2;
    return scandata;
}

//NGC MATCH
short* NewScanHandelSubset(short* handel, struct nuvec_s* vpos, struct nuvec_s* vvel, float size, s32 extramask) {
    s32 HitCnt;
    s32 lp;
    s32 terrid;
    struct scaleterrain_s** HitData;
    struct scaleterrain_s** MaxData;
    short* LastWrite;
    short* scandata;
    struct nuvec_s** walldata;
    struct nuvec_s** spl;
    short* CurData;
    float maxx;
    float maxy;
    float maxz;
    float tmaxx;
    float tmaxy;
    float tmaxz;
    float radmov;
    float minx;
    float miny;
    float minz;
    float tminx;
    float tminy;
    float tminz;
    float tn;
    struct terr_s* mbuf2;
    struct scaleterrain_s** curter;

    if (handel == NULL) {
        return NULL;
    }
    scandata = LastWrite = (short*)TempStackPtr;
    HitData = (struct scaleterrain_s**)(LastWrite + 2);
    MaxData = (struct scaleterrain_s**)(HitData + 0x1fc);
    if (HitData > (struct scaleterrain_s**)debkeydata + 0x2d931) {
        return NULL;
    }
    if ((struct scaleterrain_s**)(debkeydata + 0x2dac0) < MaxData) {
        MaxData = (struct scaleterrain_s**)(debkeydata + 0x2dac0);
    }
    if (vvel->x > 0.0f) {
        minx = vpos->x - 0.02f - size;
        maxx = vpos->x + vvel->x + 0.02f + size;
    } else {
        minx = vpos->x + vvel->x - 0.02f - size;
        maxx = vpos->x + 0.02f + size;
    }
    if (vvel->y > 0.0f) {
        miny = vpos->y - 0.02f - size;
        maxy = vpos->y + vvel->y + 0.02f + size;
    } else {
        miny = vpos->y + vvel->y - 0.02f - size;
        maxy = vpos->y + 0.02f + size;
    }
    if (vvel->z > 0.0f) {
        minz = vpos->z - 0.02f - size;
        maxz = vpos->z + vvel->z + 0.02f + size;
    } else {
        minz = vpos->z + vvel->z - 0.02f - size;
        maxz = vpos->z + 0.02f + size;
    }
    radmov = (minx - maxx) * (minx - maxx) + (miny - maxy) * (miny - maxy) + (minz - maxz) * (minz - maxz);
    radmov += 0.02f;
    HitCnt = 0;
    CurData = (short*)handel;
    while (*(short*)CurData > 0) {
        terrid = *(short*)(CurData + 1);
        mbuf2 = CurTerr->terr + *(short*)(CurData + 1);
        if ((mbuf2->type == TERR_TYPE_PLATFORM) && (CurTerr->platdata[mbuf2->info].curmtx != NULL)) {
            if ((CurTerr->platdata[mbuf2->info].curmtx)->_30 > CurTerr->platdata[mbuf2->info].oldmtx._30) {
                tminx = minx - mbuf2->Location.x;
                tmaxx =
                    (((CurTerr->platdata[mbuf2->info].curmtx)->_30 - CurTerr->platdata[mbuf2->info].oldmtx._30) * 1.5f
                     + maxx)
                    - mbuf2->Location.x;
            } else {
                tmaxx = maxx - mbuf2->Location.x;
                tminx =
                    (((CurTerr->platdata[mbuf2->info].curmtx)->_30 - CurTerr->platdata[mbuf2->info].oldmtx._30) * 1.5f
                     + minx)
                    - mbuf2->Location.x;
            }
            if ((CurTerr->platdata[mbuf2->info].curmtx)->_31 > CurTerr->platdata[mbuf2->info].oldmtx._31) {
                tminy = miny - mbuf2->Location.y;
                tmaxy =
                    (((CurTerr->platdata[mbuf2->info].curmtx)->_31 - CurTerr->platdata[mbuf2->info].oldmtx._31) * 1.5f
                     + maxy)
                    - mbuf2->Location.y;
            } else {
                tmaxy = maxy - mbuf2->Location.y;
                tminy =
                    (((CurTerr->platdata[mbuf2->info].curmtx)->_31 - CurTerr->platdata[mbuf2->info].oldmtx._31) * 1.5f
                     + miny)
                    - mbuf2->Location.y;
            }
            if ((CurTerr->platdata[mbuf2->info].curmtx)->_32 > CurTerr->platdata[mbuf2->info].oldmtx._32) {
                tminz = minz - mbuf2->Location.z;
                tmaxz =
                    (((CurTerr->platdata[mbuf2->info].curmtx)->_32 - CurTerr->platdata[mbuf2->info].oldmtx._32) * 1.5f
                     + maxz)
                    - mbuf2->Location.z;
            } else {
                tmaxz = maxz - mbuf2->Location.z;
                tminz =
                    (((CurTerr->platdata[mbuf2->info].curmtx)->_32 - CurTerr->platdata[mbuf2->info].oldmtx._32) * 1.5f
                     + minz)
                    - mbuf2->Location.z;
            }
            if ((s32)CurTerr->platdata[mbuf2->info].status.rotate == 0) {
                if ((((tmaxx >= mbuf2->min.x) && (tmaxy >= mbuf2->min.y)) && (tmaxz >= mbuf2->min.z))
                    && (((tminx <= mbuf2->max.x && (tminy < mbuf2->max.y)) && ((tminz < mbuf2->max.z)))))
                {
                    curter = (struct scaleterrain_s **)CurData + 1;
                    for (lp = *(short*)CurData; lp > 0; lp--, curter++) {
                        if ((((((tmaxx >= (*curter)->minx) && (tminx < (*curter)->maxx)) && (tmaxz >= (*curter)->minz))
                              && ((tminz < (*curter)->maxz && (tmaxy >= (*curter)->miny))))
                             && (tminy < (*curter)->maxy))
                            && ((
                                HitData < MaxData
                                && (((*curter)->info[1] == 0 || (((u32)(*curter)->info[1] & extramask) != 0)))
                            )))
                        {
                            *HitData = *curter;
                            HitData++;
                            HitCnt++;
                        }
                    }
                } else {
                    curter = (struct scaleterrain_s **)(short*)CurData + 1;
                    curter += *(short*)CurData;
                }
            } else {
                tn = ((maxx + minx) * 0.5f - mbuf2->Location.x) * ((maxx + minx) * 0.5f - mbuf2->Location.x)
                    + (((maxy + miny) * 0.5f - mbuf2->Location.y) * TerI->inyscale)
                        * (((maxy + miny) * 0.5f - mbuf2->Location.y) * TerI->inyscale)
                    + ((maxz + minz) * 0.5f - mbuf2->Location.z) * ((maxz + minz) * 0.5f - mbuf2->Location.z);
                if (tn < mbuf2->radius + radmov) {
                    curter = (struct scaleterrain_s **)CurData + 1;
                    for (lp = (s32) * (short*)CurData; lp > 0; lp--, curter++) {
                        if (((((HitData < MaxData)
                               && (((*curter)->info[1] == 0 || (((*curter)->info[1] & extramask) != 0))))
                              && ((
                                  (*curter)->pnts[0].x > tminx
                                  || ((
                                      ((*curter)->pnts[1].x > tminx || ((*curter)->pnts[2].x > tminx))
                                      || ((*curter)->pnts[3].x > tminx)
                                  ))
                              )))
                             && ((
                                 ((*curter)->pnts[0].x < tmaxx || ((*curter)->pnts[1].x < tmaxx))
                                 || (((*curter)->pnts[2].x < tmaxx || ((*curter)->pnts[3].x < tmaxx)))
                             )))
                            && ((
                                ((((*curter)->pnts[0].z > tminz || ((*curter)->pnts[1].z > tminz)) || ((*curter)->pnts[2].z > tminz))
                                 || ((*curter)->pnts[3].z > tminz))
                                && ((
                                    (((*curter)->pnts[0].z < tmaxz || ((*curter)->pnts[1].z < tmaxz)) || ((*curter)->pnts[2].z < tmaxz)
                                    )
                                    || ((*curter)->pnts[3].z < tmaxz)
                                ))
                            )))
                        {
                            *HitData = *curter;
                            HitData++;
                            HitCnt++;
                        }
                    }
                } else {
                    curter = (struct scaleterrain_s **)(CurData + 2);
                    curter += *(short*)CurData;
                }
            }
        } else {
            tmaxx = maxx - mbuf2->Location.x;
            tminx = minx - mbuf2->Location.x;
            tmaxy = maxy - mbuf2->Location.y;
            tminy = miny - mbuf2->Location.y;
            tmaxz = maxz - mbuf2->Location.z;
            tminz = minz - mbuf2->Location.z;
            if ((maxx >= mbuf2->min.x)
                && ((
                    ((maxy >= mbuf2->min.y && (maxz >= mbuf2->min.z)) && (minx <= mbuf2->max.x))
                    && ((miny < mbuf2->max.y && (minz < mbuf2->max.z)))
                )))
            {
                curter = (struct scaleterrain_s **)(CurData + 2);
                for (lp = (s32) * (short*)CurData; lp > 0; lp--, curter++) {
                    if ((((((tmaxx >= (*curter)->minx) && (tminx < (*curter)->maxx)) && (tmaxz >= (*curter)->minz))
                          && ((tminz < (*curter)->maxz && (tmaxy >= (*curter)->miny))))
                         && (tminy < (*curter)->maxy))
                        && ((
                            HitData < MaxData
                            && (((*curter)->info[1] == 0 || (((u32)(*curter)->info[1] & extramask) != 0)))
                        )))
                    {
                        *HitData = *curter;
                        HitData++;
                        HitCnt++;
                    }
                }
            } else {
                curter = (struct scaleterrain_s **)(CurData + 2);
                curter += *(short*)CurData;
            }
        }
        if (HitCnt != 0) {
            *(short*)LastWrite = HitCnt;
            *(short*)(LastWrite + 1) = terrid;
            LastWrite = (short*)HitData;
            HitData++;
            HitCnt = 0;
        }
        CurData = (short*)curter;
    }
    *(short*)LastWrite = 0;
    *(short*)(LastWrite + 1) = 0;
    walldata = (struct nuvec_s **)(LastWrite + 2);
    minx = minx - 0.02f;
    minz = minz - 0.02f;
    maxx = maxx + 0.02f;
    maxz = maxz + 0.02f;
    for (CurData = CurData + 2; *(s32*)CurData > 0;) {
        spl = (struct nuvec_s**)CurData;
        if (((((*spl)->x >= minx) && ((*spl + 1)->x <= maxx)) || (((*spl + 1)->x >= minx && ((*spl)->x <= maxx))))
            && ((((*spl)->z >= minz && ((*spl + 1)->z <= maxz)) || (((*spl + 1)->z >= minz && ((*spl)->z <= maxz))))))
        {
            *walldata++ = *spl;
        }
        spl++;
        CurData = (short*)spl;
    }
    LastWrite = (short*)walldata;
    *(short*)LastWrite = 0;
    *(short*)((s32)LastWrite + 2) = 0;
    TempStackPtr = (short*)(LastWrite + 2);
    return scandata;
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

//NGC MATCH
void ScanTerrainHandel(s32 extramask, short* Handel) {
    s32 HitCnt;
    s32 lp;
    s32 terrid;
    struct scaleterrain_s** HitData;
    struct scaleterrain_s** MaxData;
    short* LastWrite;
    struct nuvec_s** spl;
    short* CurData;
    float maxx;
    float maxy;
    float maxz;
    float tmaxx;
    float tmaxy;
    float tmaxz;
    float radmov;
    float minx;
    float miny;
    float minz;
    float tminx;
    float tminy;
    float tminz;
    float tn;
    struct terr_s* mbuf2;
    struct scaleterrain_s** curter;

    if (Handel == NULL) {
        return;
    }
    TerI->hitcnt = 0;
    LastWrite = (short*)TerI->hitdata;
    HitData = TerI->hitdata + 1;
    MaxData = HitData + 0x1fc;
    HitCnt = 0;
    platinrange = 0;
    TerI->plathit = -1;
    if (TerI->scanmode != 1) {
        radmov = TerI->curvel.x * TerI->curvel.x + TerI->curvel.y * TerI->curvel.y + TerI->curvel.z * TerI->curvel.z;
        radmov += TerI->sizesq + 0.02f;
        maxx = TerI->curpos.x + radmov;
        maxy = TerI->curpos.y + radmov * TerI->yscale;
        maxz = TerI->curpos.z + radmov;
        minx = TerI->curpos.x - radmov;
        miny = TerI->curpos.y - radmov * TerI->yscale;
        minz = TerI->curpos.z - radmov;
    } else {
        if (TerI->curvel.x > 0.0f) {
            minx = (TerI->curpos.x - 0.02f) - TerI->size;
            maxx = TerI->curpos.x + TerI->curvel.x + 0.02f + TerI->size;
        } else {
            minx = ((TerI->curpos.x + TerI->curvel.x) - 0.02f) - TerI->size;
            maxx = TerI->curpos.x + 0.02f + TerI->size;
        }
        if (TerI->curvel.y > 0.0f) {
            miny = TerI->curpos.y - 0.02f - TerI->size;
            maxy = ((TerI->curpos.y + TerI->curvel.y) + 0.02f) + TerI->size;
        } else {
            miny = ((TerI->curpos.y + TerI->curvel.y) - 0.02f) - TerI->size;
            maxy = (TerI->curpos.y + 0.02f) + TerI->size;
        }
        if (TerI->curvel.z > 0.0f) {
            minz = (TerI->curpos.z - 0.02f) - TerI->size;
            maxz = TerI->curpos.z + TerI->curvel.z + 0.02f + TerI->size;
        } else {
            minz = ((TerI->curpos.z + TerI->curvel.z) - 0.02f) - TerI->size;
            maxz = TerI->curpos.z + 0.02f + TerI->size;
        }
        radmov = (minx - maxx) * (minx - maxx) + (miny - maxy) * (miny - maxy) + (minz - maxz) * (minz - maxz);
        radmov += +0.02f;
    }
    HitCnt = 0;
    CurData = Handel;
    while (*(short*)CurData > 0) {
        terrid = *(short*)((s32)CurData + 2);
        mbuf2 = CurTerr->terr + *(short*)((s32)CurData + 2);
        curter = (struct scaleterrain_s**)CurData + 1;
        if ((mbuf2->type == TERR_TYPE_PLATFORM) && (CurTerr->platdata[mbuf2->info].curmtx != NULL)) {
            if ((CurTerr->platdata[mbuf2->info].curmtx)->_30 > CurTerr->platdata[mbuf2->info].oldmtx._30) {
                tminx = minx - mbuf2->Location.x;
                tmaxx =
                    (((CurTerr->platdata[mbuf2->info].curmtx)->_30 - CurTerr->platdata[mbuf2->info].oldmtx._30) * 1.5f
                     + maxx)
                    - mbuf2->Location.x;
            } else {
                tmaxx = maxx - mbuf2->Location.x;
                tminx =
                    (((CurTerr->platdata[mbuf2->info].curmtx)->_30 - CurTerr->platdata[mbuf2->info].oldmtx._30) * 1.5f
                     + minx)
                    - mbuf2->Location.x;
            }
            if ((CurTerr->platdata[mbuf2->info].curmtx)->_31 > CurTerr->platdata[mbuf2->info].oldmtx._31) {
                tminy = miny - mbuf2->Location.y;
                tmaxy =
                    (((CurTerr->platdata[mbuf2->info].curmtx)->_31 - CurTerr->platdata[mbuf2->info].oldmtx._31) * 1.5f
                     + maxy)
                    - mbuf2->Location.y;
            } else {
                tmaxy = maxy - mbuf2->Location.y;
                tminy =
                    (((CurTerr->platdata[mbuf2->info].curmtx)->_31 - CurTerr->platdata[mbuf2->info].oldmtx._31) * 1.5f
                     + miny)
                    - mbuf2->Location.y;
            }
            if ((CurTerr->platdata[mbuf2->info].curmtx)->_32 > CurTerr->platdata[mbuf2->info].oldmtx._32) {
                tminz = minz - mbuf2->Location.z;
                tmaxz =
                    (((CurTerr->platdata[mbuf2->info].curmtx)->_32 - CurTerr->platdata[mbuf2->info].oldmtx._32) * 1.5f
                     + maxz)
                    - mbuf2->Location.z;
            } else {
                tmaxz = maxz - mbuf2->Location.z;
                tminz =
                    (((CurTerr->platdata[mbuf2->info].curmtx)->_32 - CurTerr->platdata[mbuf2->info].oldmtx._32) * 1.5f
                     + minz)
                    - mbuf2->Location.z;
            }
            if (mbuf2->info == TerrPlatDis) {
                curter = (struct scaleterrain_s**)CurData + 1;
                curter = curter + *(short*)CurData;
            } else if ((s32)CurTerr->platdata[mbuf2->info].status.rotate == 0) {
                if ((((tmaxx >= mbuf2->min.x) && (tmaxy >= mbuf2->min.y))
                     && ((tmaxz >= mbuf2->min.z && ((tminx <= mbuf2->max.x && (tminy < mbuf2->max.y))))))
                    && (tminz < mbuf2->max.z))
                {
                    for (lp = (s32) *(short*)CurData; lp > 0; lp--, curter++) {
                        if (((((tmaxx >= (*curter)->minx) && (tminx < (*curter)->maxx))
                              && (tmaxz >= (*curter)->minz))
                             && ((tminz < (*curter)->maxz && (tmaxy >= (*curter)->miny))))
                            && ((
                                (tminy < (*curter)->maxy && (HitData < MaxData))
                                && (((*curter)->info[1] == 0 || (((uint)(*curter)->info[1] & extramask) != 0)))
                            )))
                        {
                            *HitData = *curter;
                            HitData++;
                            HitCnt++;
                        }
                    }
                } else {
                    curter = (struct scaleterrain_s**)CurData + 1;
                    curter = curter + *(short*)CurData;
                }
            } else {
                tn = ((maxx + minx) * 0.5f - mbuf2->Location.x) * ((maxx + minx) * 0.5f - mbuf2->Location.x)
                    + (((maxy + miny) * 0.5f - mbuf2->Location.y) * TerI->inyscale)
                        * (((maxy + miny) * 0.5f - mbuf2->Location.y) * TerI->inyscale)
                    + ((maxz + minz) * 0.5f - mbuf2->Location.z) * ((maxz + minz) * 0.5f - mbuf2->Location.z);
                if (tn < mbuf2->radius + radmov) {
                    for (lp = (s32) * (short*)CurData; 0 < lp; lp--, curter++) {
                        if (((((HitData < MaxData)
                               && (((*curter)->info[1] == 0 || (((uint)(*curter)->info[1] & extramask) != 0))))
                              && ((
                                  (*curter)->pnts[0].x > tminx
                                  || ((
                                      ((*curter)->pnts[1].x > tminx || ((*curter)->pnts[2].x > tminx))
                                      || ((*curter)->pnts[3].x) > tminx
                                  ))
                              )))
                             && ((
                                 ((*curter)->pnts[0].x < tmaxx || ((*curter)->pnts[1].x < tmaxx))
                                 || (((*curter)->pnts[2].x < tmaxx || ((*curter)->pnts[3].x < tmaxx)))
                             )))
                            && ((
                                ((((*curter)->pnts[0].z > tminz || ((*curter)->pnts[1].z > tminz))
                                  || ((*curter)->pnts[2].z > tminz))
                                 || ((*curter)->pnts[3].z > tminz))
                                && ((
                                    (((*curter)->pnts[0].z < tmaxz || ((*curter)->pnts[1].z < tmaxz))
                                     || ((*curter)->pnts[2].z < tmaxz))
                                    || ((*curter)->pnts[3].z < tmaxz)
                                ))
                            )))
                        {
                            *HitData = *curter;
                            HitData++;
                            HitCnt++;
                        }
                    }
                } else {
                    curter = (struct scaleterrain_s**)CurData + 1;
                    curter = curter + *(short*)CurData;
                }
            }
        } else if ((maxx >= mbuf2->min.x) &&
              ((((maxy >= mbuf2->min.y && (maxz >= mbuf2->min.z)) && (minx <= mbuf2->max.x)) &&
               ((miny < mbuf2->max.y && (minz < mbuf2->max.z))))))
        {
            tmaxx = maxx - mbuf2->Location.x;
            tminx = minx - mbuf2->Location.x;
            tmaxy = maxy - mbuf2->Location.y;
            tminy = miny - mbuf2->Location.y;
            tmaxz = maxz - mbuf2->Location.z;
            tminz = minz - mbuf2->Location.z;
            for (lp = (s32) * (short*)CurData; 0 < lp; lp--, curter++) {
                if ((((tmaxx >= (*curter)->minx) && (tminx < (*curter)->maxx))
                     && ((
                         tmaxz >= (*curter)->minz
                         && ((
                             (tminz < (*curter)->maxz
                              && (tmaxy >= (*curter)->miny ))
                             && (tminy < (*curter)->maxy)
                         ))
                     )))
                    && ((
                        HitData < MaxData
                        && (((*curter)->info[1] == 0 || (((uint)(*curter)->info[1] & extramask) != 0)))
                    )))
                {
                    *HitData = *curter;
                    HitData++;
                    HitCnt++;
                }
            }
        } else {
            curter = (struct scaleterrain_s**)CurData + 1;
            curter = curter + *(short*)CurData;
        }
        if (HitCnt != 0) {
            *(short*)LastWrite = HitCnt;
            *(short*)(LastWrite + 1) = terrid;
            LastWrite = (short*)HitData;
            HitData++;
            HitCnt = 0;
        }
        CurData = (short*)curter;
    }
    *(short*)LastWrite = 0;
    *(short*)((s32)LastWrite + 2) = 0;
    minx -= 0.02f;
    minz -= 0.02f;
    maxx += 0.02f;
    maxz += 0.02f;
    CurData += 2;
    WallSplCount = 0;
    while (*(s32*)CurData != NULL) {
        spl = (struct nuvec_s**)CurData;
        if ((((((*spl)->x >= minx) && ((*spl + 1)->x <= maxx)) || (((*spl + 1)->x >= minx && ((*spl)->x <= maxx))))
             && ((((*spl)->z >= minz && ((*spl + 1)->z <= maxz)) || (((*spl + 1)->z >= minz && ((*spl)->z <= maxz))))))
            && (WallSplCount < 0x40))
        {
            WallSplList[WallSplCount] = *(*spl);
            WallSplCount++;
            WallSplList[WallSplCount] = *(*spl + 1);
            WallSplCount++;
        }
        spl = (struct nuvec_s**)spl + 1;
        CurData = (short*)spl;
    }
    TerrPlatDis = -1;
    return;
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


//NGC MATCH
void ScanTerrainPlatform(s32 msituid, s32 extramask) {
    s32 a;
    s32 c;
    s32 HitCnt;
    struct scaleterrain_s** HitData;
    struct scaleterrain_s** MaxData;
    short* LastWrite;
    float maxx;
    float maxy;
    float maxz;
    float tmaxx;
    float tmaxy;
    float tmaxz;
    float radmov;
    float minx;
    float miny;
    float minz;
    float tminx;
    float tminy;
    float tminz;
    float tn;
    struct terr_s* mbuf2;
    struct scaleterrain_s* ter;
    short* modp;
    s32 curscltemp;
    struct nuvec4_s pnts[4];
    struct nuvec4_s norm[2];

    curscltemp = 0;
    ScaleTerrain = ScaleTerrainT1;
    TerI->hitcnt = 0;
    LastWrite = (short*)TerI->hitdata;
    HitData = TerI->hitdata + 1;
    MaxData = HitData + 0x1fc;
    HitCnt = 0;
    platinrange = 0;
    TerI->plathit = -1;
    if (TerI->scanmode != 1) {
        radmov = TerI->curvel.x * TerI->curvel.x + TerI->curvel.y * TerI->curvel.y + TerI->curvel.z * TerI->curvel.z;
        radmov += TerI->sizesq + 0.02f;
        maxx = TerI->curpos.x + radmov;
        maxy = TerI->curpos.y + radmov * TerI->yscale;
        maxz = TerI->curpos.z + radmov;
        minx = TerI->curpos.x - radmov;
        miny = TerI->curpos.y - radmov * TerI->yscale;
        minz = TerI->curpos.z - radmov;
    } else {
        if (TerI->curvel.x > 0.0f) {
            minx = (TerI->curpos.x - 0.02f) - TerI->size;
            maxx = TerI->curpos.x + TerI->curvel.x + 0.02f + TerI->size;
        } else {
            minx = ((TerI->curpos.x + TerI->curvel.x) - 0.02f) - TerI->size;
            maxx = TerI->curpos.x + 0.02f + TerI->size;
        }
        if (TerI->curvel.y > 0.0f) {
            miny = (TerI->curpos.y - 0.02f) - TerI->size;
            maxy = TerI->curpos.y + TerI->curvel.y + 0.02f + TerI->size;
        } else {
            miny = ((TerI->curpos.y + TerI->curvel.y) - 0.02f) - TerI->size;
            maxy = TerI->curpos.y + 0.02f + TerI->size;
        }
        if (TerI->curvel.z > 0.0f) {
            minz = (TerI->curpos.z - 0.02f) - TerI->size;
            maxz = TerI->curpos.z + TerI->curvel.z + 0.02f + TerI->size;
        } else {
            minz = ((TerI->curpos.z + TerI->curvel.z) - 0.02f) - TerI->size;
            maxz = TerI->curpos.z + 0.02f + TerI->size;
        }
        radmov = (minx - maxx) * (minx - maxx) + (miny - maxy) * (miny - maxy) + (minz - maxz) * (minz - maxz);
        radmov += TerI->size + 0.02f;
    }
    TerI->PlatScanStart = LastWrite;
    a = 0x100;
    minx = minx - 0.05f;
    maxx = maxx + 0.05f;
    miny = miny - 0.05f;
    maxy = maxy + 0.05f;
    minz = minz - 0.05f;
    maxz = maxz + 0.05f;
    mbuf2 = CurTerr->terr + msituid;
    if (CurTerr->platdata[mbuf2->info].curmtx != NULL) {
        if ((CurTerr->platdata[mbuf2->info].curmtx)->_30 > CurTerr->platdata[mbuf2->info].oldmtx._30) {
            tminx = minx - mbuf2->Location.x;
            tmaxx = (((CurTerr->platdata[mbuf2->info].curmtx)->_30 - CurTerr->platdata[mbuf2->info].oldmtx._30) * 1.5f
                     + maxx)
                - mbuf2->Location.x;
        } else {
            tmaxx = maxx - mbuf2->Location.x;
            tminx = (((CurTerr->platdata[mbuf2->info].curmtx)->_30 - CurTerr->platdata[mbuf2->info].oldmtx._30) * 1.5f
                     + minx)
                - mbuf2->Location.x;
        }
        if ((CurTerr->platdata[mbuf2->info].curmtx)->_31 > CurTerr->platdata[mbuf2->info].oldmtx._31) {
            tminy = miny - mbuf2->Location.y;
            tmaxy = (((CurTerr->platdata[mbuf2->info].curmtx)->_31 - CurTerr->platdata[mbuf2->info].oldmtx._31) * 1.5f
                     + maxy)
                - mbuf2->Location.y;
        } else {
            tmaxy = maxy - mbuf2->Location.y;
            tminy = (((CurTerr->platdata[mbuf2->info].curmtx)->_31 - CurTerr->platdata[mbuf2->info].oldmtx._31) * 1.5f
                     + miny)
                - mbuf2->Location.y;
        }
        if ((CurTerr->platdata[mbuf2->info].curmtx)->_32 > CurTerr->platdata[mbuf2->info].oldmtx._32) {
            tminz = minz - mbuf2->Location.z;
            tmaxz = (((CurTerr->platdata[mbuf2->info].curmtx)->_32 - CurTerr->platdata[mbuf2->info].oldmtx._32) * 1.5f
                     + maxz)
                - mbuf2->Location.z;
        } else {
            tmaxz = maxz - mbuf2->Location.z;
            tminz = (((CurTerr->platdata[mbuf2->info].curmtx)->_32 - CurTerr->platdata[mbuf2->info].oldmtx._32) * 1.5f
                     + minz)
                - mbuf2->Location.z;
        }
    } else {
        tmaxx = maxx - mbuf2->Location.x;
        tminx = minx - mbuf2->Location.x;
        tmaxy = maxy - mbuf2->Location.y;
        tminy = miny - mbuf2->Location.y;
        tmaxz = maxz - mbuf2->Location.z;
        tminz = minz - mbuf2->Location.z;
    }
    // a = msituid;
    if ((s32)CurTerr->platdata[mbuf2->info].status.rotate != 0) {
        tminx = ((maxx + minx) * 0.5f - mbuf2->Location.x) * ((maxx + minx) * 0.5f - mbuf2->Location.x)
            + (((maxy + miny) * 0.5f - mbuf2->Location.y) * TerI->inyscale)
                * (((maxy + miny) * 0.5f - mbuf2->Location.y) * TerI->inyscale)
            + ((maxz + minz) * 0.5f - mbuf2->Location.z) * ((maxz + minz) * 0.5f - mbuf2->Location.z);
        if ((mbuf2->type != ~TERR_TYPE_NORMAL) && (tminx < mbuf2->radius + radmov)) {
            modp = (short*)mbuf2->model;
            while (*(short*)modp >= 0) {
                for (c = *(short*)(modp + 1), ter = (struct scaleterrain_s*)(modp + 10); 0 < c; c--) {
                    platinrange = 1;
                    if ((HitData < MaxData) && ((ter->info[1] == 0 || ((ter->info[1] & extramask) != 0)))) {
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
                        pnts[3].x = ter->pnts[3].x;
                        pnts[3].y = ter->pnts[3].y;
                        pnts[3].z = ter->pnts[3].z;
                        pnts[3].w = 0.0f;
                        NuVec4MtxTransformVU0(&pnts[0], &pnts[0], CurTerr->platdata[mbuf2->info].curmtx);
                        NuVec4MtxTransformVU0(&pnts[1], &pnts[1], CurTerr->platdata[mbuf2->info].curmtx);
                        NuVec4MtxTransformVU0(&pnts[2], &pnts[2], CurTerr->platdata[mbuf2->info].curmtx);
                        NuVec4MtxTransformVU0(&pnts[3], &pnts[3], CurTerr->platdata[mbuf2->info].curmtx);
                        norm[0].x = ter->norm[0].x;
                        norm[0].y = ter->norm[0].y;
                        norm[0].z = ter->norm[0].z;
                        norm[0].w = 0.0f;
                        NuVec4MtxTransformVU0(&norm[0], &norm[0], CurTerr->platdata[mbuf2->info].curmtx);
                        if (ter->norm[1].y < 65535.0f) {
                            norm[1].x = ter->norm[1].x;
                            norm[1].y = ter->norm[1].y;
                            norm[1].z = ter->norm[1].z;
                            norm[1].w = 0.0f;
                            NuVec4MtxTransformVU0(&norm[1], &norm[1], CurTerr->platdata[mbuf2->info].curmtx);
                        }
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
                        if (ter->norm[1].y < 65535.0f) {
                            ScaleTerrain[curscltemp].pnts[3].x = pnts[3].x;
                            ScaleTerrain[curscltemp].pnts[3].z = pnts[3].z;
                            ScaleTerrain[curscltemp].pnts[3].y =
                                (pnts[3].y + mbuf2->Location.y) * TerI->inyscale - mbuf2->Location.y;
                            tn = 1.0f
                                / NuFsqrt(
                                     norm[1].x * norm[1].x + norm[1].y * norm[1].y * TerI->yscalesq
                                     + norm[1].z * norm[1].z
                                );
                            ScaleTerrain[curscltemp].norm[1].x = norm[1].x * tn;
                            ScaleTerrain[curscltemp].norm[1].y = norm[1].y * TerI->yscale * tn;
                            ScaleTerrain[curscltemp].norm[1].z = norm[1].z * tn;
                        } else {
                            ScaleTerrain[curscltemp].norm[1].y = 65536.0f;
                        }
                        tn = 1.0f
                            / NuFsqrt(
                                 norm[0].x * norm[0].x + norm[0].y * norm[0].y * TerI->yscalesq + norm[0].z * norm[0].z
                            );
                        ScaleTerrain[curscltemp].norm[0].x = norm[0].x * tn;
                        ScaleTerrain[curscltemp].norm[0].y = norm[0].y * TerI->yscale * tn;
                        ScaleTerrain[curscltemp].norm[0].z = norm[0].z * tn;
                        *HitData = ScaleTerrain + curscltemp;
                        HitData = HitData + 1;
                        HitCnt = HitCnt + 1;
                        curscltemp++;
                    }
                    ter++;
                }
                modp = (short*)ter;
            }
            if (HitCnt != 0) {
                *(short*)LastWrite = HitCnt;
                *(short*)(LastWrite + 1) = msituid;
                LastWrite = (short*)HitData;
                HitData++;
                HitCnt = 0;
            }
        }
    } else if (((((tmaxx >= mbuf2->min.x) && (tmaxy >= mbuf2->min.y)) &&
            (tmaxz >= mbuf2->min.z)) &&
           ((tminx <= mbuf2->max.x && (tminy < mbuf2->max.y)))) &&
          ((tminz < mbuf2->max.z && (mbuf2->type != ~TERR_TYPE_NORMAL))))
    {
        modp = (short*)mbuf2->model;
        while (*(short*)modp >= 0) {
            c = *(short*)(modp + 1);
            ter = (struct scaleterrain_s*)(modp + 10);
            if ((tmaxx >= *(float*)(modp + 2)) && (tminx < *(float*)(modp + 4))) {
                if ((tmaxz >= *(float*)(modp + 6)) && (tminz < *(float*)(modp + 8))) {
                    for (; c > 0; c--) {
                        if (((((tmaxx >= ter->minx) && (tminx < ter->maxx)) && (tmaxz >= ter->minz))
                             && ((
                                 (tminz < ter->maxz && (tmaxy >= ter->miny))
                                 && ((tminy < ter->maxy && (platinrange = 1, HitData < MaxData)))
                             )))
                            && ((ter->info[1] == 0 || (((uint)ter->info[1] & extramask) != 0))))
                        {
                            if (TerI->yscale == 1.0f) {
                                *HitData = ter;
                                HitData = HitData + 1;
                                HitCnt = HitCnt + 1;
                            } else {
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
                                if (ter->norm[1].y < 65535.0f) {
                                    ScaleTerrain[curscltemp].pnts[3].x = ter->pnts[3].x;
                                    ScaleTerrain[curscltemp].pnts[3].z = ter->pnts[3].z;
                                    ScaleTerrain[curscltemp].pnts[3].y =
                                        (ter->pnts[3].y + mbuf2->Location.y) * TerI->inyscale - mbuf2->Location.y;
                                    tn = 1.0f
                                        / NuFsqrt(
                                             ter->norm[1].x * ter->norm[1].x
                                             + ter->norm[1].y * ter->norm[1].y * TerI->yscalesq
                                             + ter->norm[1].z * ter->norm[1].z
                                        );
                                    ScaleTerrain[curscltemp].norm[1].x = ter->norm[1].x * tn;
                                    ScaleTerrain[curscltemp].norm[1].y = ter->norm[1].y * TerI->yscale * tn;
                                    ScaleTerrain[curscltemp].norm[1].z = ter->norm[1].z * tn;
                                } else {
                                    ScaleTerrain[curscltemp].norm[1].y = 65536.0f;
                                }
                                tn = 1.0f
                                    / NuFsqrt(
                                         ter->norm[0].x * ter->norm[0].x
                                         + ter->norm[0].y * ter->norm[0].y * TerI->yscalesq
                                         + ter->norm[0].z * ter->norm[0].z
                                    );
                                ScaleTerrain[curscltemp].norm[0].x = ter->norm[0].x * tn;
                                ScaleTerrain[curscltemp].norm[0].y = ter->norm[0].y * TerI->yscale * tn;
                                ScaleTerrain[curscltemp].norm[0].z = ter->norm[0].z * tn;
                                *HitData = &ScaleTerrain[curscltemp];
                                HitData++;
                                HitCnt++;
                                curscltemp++;
                            }
                        }
                        ter++;
                    }
                    modp = (short*)ter;
                } else {
                    modp = (short*)&ter[c];
                }
            } else {
                modp = (short*)&ter[c];
            }
        }
        if (HitCnt != 0) {
            *(short*)LastWrite = HitCnt;
            *(short*)(LastWrite + 1) = msituid;
            LastWrite = (short*)HitData;
            HitData++;
            HitCnt = 0;
        }
    }
    *(short*)LastWrite = 0;
    *(short*)(LastWrite + 1) = 0;
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