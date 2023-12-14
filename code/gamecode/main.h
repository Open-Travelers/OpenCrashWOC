#ifndef MAIN_H
#define MAIN_H

#include "../types.h"
#include "../nu.h"
#include "gamecode/creature.h"
#include "gamecode/game.h"
#include "gamecode/font3d.h"
#include "gamecode/camera.h"
#include "nuxbox/nuxboxtypes.h"

s32 SWIDTH;
s32 SHEIGHT;
struct nugscn_s* world_scene[32];
struct nuscene_s* world_scene2[32];
s32 iss3cmp;

void Text3D2 (char * txt, float x, float y, float z, float scalex, float scaley, float scalez, int align, int colour);
void Text3D (char * txt, float x, float y, float z, float scalex, float scaley, float scalez, int align, int colour);
void DrawPanel(void);
void PlayCutMovie (int movie);
void ResetSuperBuffer (void);
void ResetSuperBuffer2 (void);

// Size: 0x10
static struct txanmlist
{
    char* path; // Offset: 0x0, DWARF: 0x40B859
    int pad1; // Offset: 0x4, DWARF: 0x40B883
    unsigned long long levbits; // Offset: 0x8, DWARF: 0x40B8AA
};

typedef struct pCHASE pCHASE, *PpCHASE;

typedef struct pSFX pSFX, *PpSFX;

struct pSFX {
    char name[16];
    u16 pitch;
    u16 volume;
    u8 buzz;
    u8 rumble;
    u8 delay;
    u8 wait;
    char * path;
    u16 frequency;
    u16 stream;
    char type;
    char pad1;
    u16 id;
    struct nuvec_s Pos;
};

struct pCHASE {
    char i;
    char i_last;
    char i_next;
    u8 cuboid;
    short character[6];
    short action[6];
    float scale[6];
    float duration;
};

struct ldata_s {
    char * filepath;
    u8 * cList; /* modelList */
    struct pCHASE * pChase;
    u32 time[3];
    short music[2];
    struct pSFX * pSFX;
    short nSFX;
    char pad1;
    char hub;
    u16 flags;
    short character;
    short vehicle;
    u16 farclip;
    struct nuvec_s vSTART;
    struct nuvec_s vBONUS;
    float fognear;
    float fogfar;
    u8 fogr;
    u8 fogg;
    u8 fogb;
    u8 foga;
    u8 hazer;
    u8 hazeg;
    u8 hazeb;
    u8 hazea;
    int totalsize;
};



typedef struct ghg_inst_s ghg_inst_s, *Pghg_inst_s;

struct ghg_inst_s {
    char * name;
    struct NUHGOBJ_s * scene;
};

typedef enum Actions_ {
    ACT_ATTACK=0,
    ACT_ATTACK2=1,
    ACT_CRAWL=2,
    ACT_CROUCHDOWN=3,
    ACT_CROUCHIDLE=4,
    ACT_CROUCHUP=5,
    ACT_DIE=6,
    ACT_DIE2=7,
    ACT_DOWN=8,
    ACT_DROWN=9,
    ACT_EXPLODE=10,
    ACT_FALL=11,
    ACT_FILL=12,
    ACT_FLY=13,
    ACT_GETUP=14,
    ACT_HANG=15,
    ACT_HOP=16,
    ACT_IDLE=17,
    ACT_IDLE00=18,
    ACT_IDLE01=19,
    ACT_IDLE02=20,
    ACT_IDLE03=21,
    ACT_IDLE04=22,
    ACT_IDLE05=23,
    ACT_IDLE06=24,
    ACT_IDLE07=25,
    ACT_IN=26,
    ACT_JUMP=27,
    ACT_JUMP2=28,
    ACT_LAND=29,
    ACT_LAND2=30,
    ACT_LEFT=31,
    ACT_LOSE=32,
    ACT_OPEN=33,
    ACT_OUT=34,
    ACT_PULL=35,
    ACT_PUSH=36,
    ACT_RIGHT=37,
    ACT_RUN=38,
    ACT_RUN2=39,
    ACT_RUNNINGJUMP=40,
    ACT_SHAKE=41,
    ACT_SIT=42,
    ACT_SKATE=43,
    ACT_SKID=44,
    ACT_SLAM=45,
    ACT_SLEEP=46,
    ACT_SLIDE=47,
    ACT_SOMERSAULT=48,
    ACT_SPENT=49,
    ACT_SPIN=50,
    ACT_SPIN2=51,
    ACT_SPRINT=52,
    ACT_STARJUMP=53,
    ACT_SUPERSLAM=54,
    ACT_SWIM=55,
    ACT_SWING=56,
    ACT_TIPTOE=57,
    ACT_TRAPPED=58,
    ACT_TRAPPEDSHAKE=59,
    ACT_TURN=60,
    ACT_UP=61,
    ACT_WADE=62,
    ACT_WAIT=63,
    ACT_WALK=64,
    ACT_WIN=65,
    ACT_FLYZOFFA=66,
    MAXACTIONS=67
} Actions_;

typedef struct cdata_s cdata_s, *Pcdata_s;

struct cdata_s { /* CharacterData */
    char * path;
    char * file;
    char * name;
    struct animlist * anim;
    float radius;
    struct nuvec_s min;
    struct nuvec_s max;
    float scale;
    float shadow_scale;
};


struct mask {
    struct numtx_s mM;
    struct numtx_s mS;
    struct nuvec_s pos;
    struct nuvec_s newpos;
    struct Nearest_Light_s lights;
    struct anim_s anim;
    float scale;
    float shadow;
    short character;
    short active;
    short sfx;
    char pad1;
    char pad2;
    u16 xrot;
    u16 yrot;
    u16 angle;
    u16 surface_xrot;
    u16 surface_zrot;
    u16 wibble_ang[3];
    float idle_time;
    float idle_duration;
    char idle_mode;
    char reflect;
    u8 offset_frame;
    char hold;
};



typedef struct csc_s csc_s, *Pcsc_s;

struct csc_s {
    struct NUHGOBJ_s * obj;
    char * path;
    char * name;
};

typedef struct PADStatus PADStatus, *PPADStatus;

struct PADStatus {
    u32 button;
    short stickX;
    short stickY;
    short substickX;
    short substickY;
    u32 triggerLeft;
    u32 triggerRight;
    u32 analogA;
    u32 analogB;
    short err;
};

typedef struct PData PData, *PPData;

struct PData {
    u8 character;
    u8 font3d_letter;
    char pad1;
    char pad2;
    char * name;
    int * description;
};

typedef struct PlrEvent PlrEvent, *PPlrEvent;


struct PlrEvent {
    struct nugspline_s * spl;
    struct nuhspecial_s obj[16];
    struct nuvec_s pos;
    char played;
    char iRAIL;
    short iALONG;
    float fALONG;
};

typedef struct pVTog pVTog, *PpVTog;

struct pVTog {
    struct nugspline_s * pTRIGGER;
    struct nugspline_s * pCAM;
    struct nugspline_s * pLOOK;
    struct numtx_s m;
    struct nuvec_s pos;
    struct anim_s anim;
    char type;
    char iRAIL;
    short iALONG;
    float fALONG;
    u16 xrot;
    u16 yrot;
    u16 zrot;
    char pad1;
    char pad2;
};
// Size: 0x10, DWARF: 0x1464F0
struct nulnkhdr_s
{
    struct nulsthdr_s* owner; // Offset: 0x0, DWARF: 0x14650F
    struct nulnkhdr_s* succ; // Offset: 0x4, DWARF: 0x14653C
    struct nulnkhdr_s* prev; // Offset: 0x8, DWARF: 0x146568
    unsigned short id; // Offset: 0xC, DWARF: 0x146594
    unsigned short used : 1; // Offset: 0xE, DWARF: 0x1465B9, Bit Offset: 0, Bit Size: 1
};

// Size: 0x10, DWARF: 0x1463F8
struct nulsthdr_s
{
    struct nulnkhdr_s* free; // Offset: 0x0, DWARF: 0x146417
    struct nulnkhdr_s* head; // Offset: 0x4, DWARF: 0x146443
    struct nulnkhdr_s* tail; // Offset: 0x8, DWARF: 0x14646F
    short elcnt; // Offset: 0xC, DWARF: 0x14649B
    short elsize; // Offset: 0xE, DWARF: 0x1464C3
};



#endif // !MAIN_H
