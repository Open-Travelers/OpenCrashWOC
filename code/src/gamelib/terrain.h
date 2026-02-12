#ifndef TERRAIN_H
#define TERRAIN_H

#include "../types.h"
#include "../nu.h"

typedef struct {
    // total size: 0x14
    NUVEC offset; // offset 0x0, size 0xC
    float ang; // offset 0xC, size 0x4
    float size; // offset 0x10, size 0x4
} TerrShapeType;
NUVEC ShadNorm : 0x802F2108; // size: 0xC
NUVEC EShadNorm : 0x8031F704; // size: 0xC
float EShadY : 0x803C3C68; // size: 0x4
int terrhitflags : 0x803C42FC; // size: 0x4
NUVEC ShadRoofNorm : 0x8031F6F8; // size: 0xC
NUVEC EShadRoofNorm : 0x80381D00; // size: 0xC
float ShadRoofY : 0x803C3DC0; // size: 0x4
float EShadRoofY : 0x803C42D0; // size: 0x4
typedef enum terr_type TERR_TYPE;
typedef struct TERRAINFLAGS_s TERRAINFLAGS;
typedef struct {
    // total size: 0x34
    int offset; // offset 0x0, size 0x4
    NUVEC translation; // offset 0x4, size 0xC
    short type; // offset 0x10, size 0x2
    short info; // offset 0x12, size 0x2
    short rx; // offset 0x14, size 0x2
    short ry; // offset 0x16, size 0x2
    short rz; // offset 0x18, size 0x2
    short pad; // offset 0x1A, size 0x2
    NUVEC rotation; // offset 0x1C, size 0xC
    TERRAINFLAGS flags; // offset 0x28, size 0x4
    short prim; // offset 0x2C, size 0x2
    short id; // offset 0x2E, size 0x2
    int datapos; // offset 0x30, size 0x4
} OffType;
typedef struct {
    // total size: 0x6804
    int count; // offset 0x0, size 0x4
    OffType offlist[512]; // offset 0x4, size 0x6800
} OffFileType;
typedef struct {
    // total size: 0x64
    float minx; // offset 0x0, size 0x4
    float maxx; // offset 0x4, size 0x4
    float miny; // offset 0x8, size 0x4
    float maxy; // offset 0xC, size 0x4
    float minz; // offset 0x10, size 0x4
    float maxz; // offset 0x14, size 0x4
    NUVEC pnts[4]; // offset 0x18, size 0x30
    NUVEC norm[2]; // offset 0x48, size 0x18
    unsigned char info[4]; // offset 0x60, size 0x4
} tertype;
typedef struct {
    // total size: 0x38
    NUVEC Location; // offset 0x0, size 0xC
    short * model; // offset 0xC, size 0x4
    NUVEC min; // offset 0x10, size 0xC
    NUVEC max; // offset 0x1C, size 0xC
    TERRAINFLAGS flags; // offset 0x28, size 0x4
    TERR_TYPE type; // offset 0x2C, size 0x4
    short info; // offset 0x30, size 0x2
    short id; // offset 0x32, size 0x2
    float radius; // offset 0x34, size 0x4
} terrsitu;
typedef struct {
    // total size: 0xC004
    unsigned short count; // offset 0x0, size 0x2
    unsigned short val; // offset 0x2, size 0x2
    NUVEC spl[4096]; // offset 0x4, size 0xC000
} WallSpline;
typedef struct {
    // total size: 0x14
    short tabindex; // offset 0x0, size 0x2
    short count; // offset 0x2, size 0x2
    float minx; // offset 0x4, size 0x4
    float minz; // offset 0x8, size 0x4
    float maxx; // offset 0xC, size 0x4
    float maxz; // offset 0x10, size 0x4
} terrgrouptype;
typedef struct {
    // total size: 0x930
    NUVEC origpos; // offset 0x0, size 0xC
    NUVEC origvel; // offset 0xC, size 0xC
    NUVEC curpos; // offset 0x18, size 0xC
    NUVEC curvel; // offset 0x24, size 0xC
    short id; // offset 0x30, size 0x2
    short scanmode; // offset 0x32, size 0x2
    float stopflag; // offset 0x34, size 0x4
    float vellen; // offset 0x38, size 0x4
    unsigned char * flags; // offset 0x3C, size 0x4
    float ax; // offset 0x40, size 0x4
    float ay; // offset 0x44, size 0x4
    float len; // offset 0x48, size 0x4
    float size; // offset 0x4C, size 0x4
    float sizesq; // offset 0x50, size 0x4
    float sizediv; // offset 0x54, size 0x4
    float yscale; // offset 0x58, size 0x4
    float yscalesq; // offset 0x5C, size 0x4
    float inyscale; // offset 0x60, size 0x4
    float inyscalesq; // offset 0x64, size 0x4
    short hitcnt; // offset 0x68, size 0x2
    short hitterrno; // offset 0x6A, size 0x2
    float csx; // offset 0x6C, size 0x4
    float csy; // offset 0x70, size 0x4
    float csz; // offset 0x74, size 0x4
    float cex; // offset 0x78, size 0x4
    float cey; // offset 0x7C, size 0x4
    float cez; // offset 0x80, size 0x4
    short hittype; // offset 0x84, size 0x2
    short plathit; // offset 0x86, size 0x2
    short * PlatScanStart; // offset 0x88, size 0x4
    tertype * hitter; // offset 0x8C, size 0x4
    float hittime; // offset 0x90, size 0x4
    float timeadj; // offset 0x94, size 0x4
    float impactadj; // offset 0x98, size 0x4
    NUVEC hitnorm; // offset 0x9C, size 0xC
    NUVEC uhitnorm; // offset 0xA8, size 0xC
    NUVEC tempvec[2]; // offset 0xB4, size 0x18
    tertype rotter; // offset 0xCC, size 0x64
    tertype * hitdata[512]; // offset 0x130, size 0x800
} TerTempInfoType;
typedef struct {
    // total size: 0x81C
    float x; // offset 0x0, size 0x4
    float y; // offset 0x4, size 0x4
    float z; // offset 0x8, size 0x4
    float x2; // offset 0xC, size 0x4
    float y2; // offset 0x10, size 0x4
    float z2; // offset 0x14, size 0x4
    short timer; // offset 0x18, size 0x2
    short pad; // offset 0x1A, size 0x2
    tertype * hitdata[512]; // offset 0x1C, size 0x800
} pollisttype;
typedef struct {
    // total size: 0x10
    NUVEC pos; // offset 0x0, size 0xC
    float radius; // offset 0xC, size 0x4
} Shperetype;
typedef struct platattrib PLATATTRIB;
typedef struct {
    // total size: 0x64
    NUMTX oldmtx; // offset 0x0, size 0x40
    NUMTX * curmtx; // offset 0x40, size 0x4
    short terrno; // offset 0x44, size 0x2
    short instance; // offset 0x46, size 0x2
    PLATATTRIB status; // offset 0x48, size 0x4
    short hitcnt; // offset 0x4C, size 0x2
    float plrgrav; // offset 0x50, size 0x4
    float ypos; // offset 0x54, size 0x4
    float yvel; // offset 0x58, size 0x4
    float tension; // offset 0x5C, size 0x4
    float damp; // offset 0x60, size 0x4
} TERRPLATTYPE;
typedef struct {
    // total size: 0xC
    void * ptrid; // offset 0x0, size 0x4
    short platid; // offset 0x4, size 0x2
    short platinf; // offset 0x6, size 0x2
    short timer; // offset 0x8, size 0x2
    short pad; // offset 0xA, size 0x2
} TERRINFO;
typedef struct {
    // total size: 0xAF50
    terrsitu * terr; // offset 0x0, size 0x4
    TERRPLATTYPE platdata[128]; // offset 0x4, size 0x3200
    void * wallinfo; // offset 0x3204, size 0x4
    TERRINFO TrackInfo[4]; // offset 0x3208, size 0x30
    terrgrouptype terrgroup[257]; // offset 0x3238, size 0x1414
    int terrainlow; // offset 0x464C, size 0x4
    int terrgcnt; // offset 0x4650, size 0x4
    short terrlist[4096]; // offset 0x4654, size 0x2000
    pollisttype pollist[9]; // offset 0x6654, size 0x48FC
} TERRSET;
typedef struct {
    // total size: 0x330C
    terrsitu * terr; // offset 0x0, size 0x4
    terrsitu * curlist; // offset 0x4, size 0x4
    short idlookup[128]; // offset 0x8, size 0x100
    TERRPLATTYPE platdata[128]; // offset 0x108, size 0x3200
    void * wallinfo; // offset 0x3308, size 0x4
} TERRPICKUPSET;


TERRSET * CurTerr : 0x803C42BC; // size: 0x4
TERRPICKUPSET * PickupTerr : 0x803C4310; // size: 0x4
Shperetype SphereData[16] : 0x8038230C; // size: 0x100
int curSphereter : 0x803C4294; // size: 0x4
int curPickInst : 0x803C429C; // size: 0x4
NUMTX tertempmat : 0x8039B470; // size: 0x40
NUVEC4 tertempvec4 : 0x8039B4B0; // size: 0x10
TERRINFO * CurTrackInfo : 0x803C4300; // size: 0x4
TerTempInfoType * TerI : 0x803C42C0; // size: 0x4
tertype * TerrPoly : 0x803C42AC; // size: 0x4
tertype TerrPolyInfo : 0x8038EC0C; // size: 0x64
int TerrPolyObj : 0x803C42F8; // size: 0x4
tertype * ShadPoly : 0x803C42A8; // size: 0x4
tertype * EShadPoly : 0x803C42DC; // size: 0x4
tertype * ShadRoofPoly : 0x803C42E4; // size: 0x4
tertype * EShadRoofPoly : 0x803C42D4; // size: 0x4
tertype ScaleTerrainT1[512] : 0x8038240C; // size: 0xC800
tertype ScaleTerrainT2[512] : 0x8038EC70; // size: 0xC800
NUVEC WallSplList[128] : 0x80381D0C; // size: 0x600
int WallSplCount : 0x803C42EC; // size: 0x4
tertype * ScaleTerrain : 0x803C42C4; // size: 0x4
TerrShapeType * TerrShape : 0x803C4304; // size: 0x4
int TerrShapeAdjCnt : 0x803C4298; // size: 0x4
int PlatImpactId : 0x803C42B0; // size: 0x4
NUVEC PlatImpactNorm : 0x8039B4C0; // size: 0xC
tertype PlatImpactTer : 0x8039B4CC; // size: 0x64
int PlatCrush : 0x803C42F0; // size: 0x4
int plathitid : 0x803C42F4; // size: 0x4
short castnum : 0x803C42C8; // size: 0x2
short ecastnum : 0x803C42CA; // size: 0x2
short castroofnum : 0x803C42E8; // size: 0x2
short ecastroofnum : 0x803C42D8; // size: 0x2
short shadhit : 0x803C42EA; // size: 0x2
short eshadhit : 0x803C42DA; // size: 0x2
short shadroofhit : 0x803C42E0; // size: 0x2
short eshadroofhit : 0x803C42CC; // size: 0x2
int terraincnt : 0x803C42A0; // size: 0x4
int situtotal : 0x803C42B4; // size: 0x4
int platinrange : 0x803C42A4; // size: 0x4
void * crashdata : 0x803C4290; // size: 0x4
int terlistptr : 0x803C42B8; // size: 0x4
short TempScanStack[4096] : 0x8039B530; // size: 0x2000
short * TempStackPtr : 0x803C430C; // size: 0x4
int TerrPlatDis : 0x803C2748; // size: 0x4
int testlock : 0x803C4308; // size: 0x4

#endif // !NUWIND_H