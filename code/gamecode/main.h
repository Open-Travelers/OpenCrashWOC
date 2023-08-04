#ifndef MAIN_H
#define MAIN_H

#include "../types.h"
#include "../nu.h"

void Text3D2 (char * txt, float x, float y, float z, float scalex, float scaley, float scalez, int align, int colour);
void Text3D (char * txt, float x, float y, float z, float scalex, float scaley, float scalez, int align, int colour);
void DrawPanel(void);
void PlayCutMovie (int movie);
void ResetSuperBuffer (void);
void ResetSuperBuffer2 (void);


typedef struct Cursor Cursor, *PCursor;

typedef struct remember remember, *Premember;

struct remember {
    char x;
    char y;
};

struct Cursor {
    struct remember remember[48];
    uint menu_frame;
    uint item_frame;
    char x;
    char y;
    char x_min;
    char y_min;
    char x_max;
    char y_max;
    char menu;
    char new_menu;
    char new_level;
    uchar wait;
    uchar wait_frames;
    char wait_hack;
    uchar button_lock;
    char pad1;
    char pad2;
    char pad3;
};

struct Hub {
    uchar flags;
    uchar crystals;
    char pad1;
    char pad2;
};

struct time {
    char name[4];
    uint itime;
};

struct level {
    ushort flags;
    char pad1;
    char pad2;
    struct time time[3];
};

struct Game_s {
    char name[9];
    uchar vibration;
    uchar surround;
    uchar sfx_volume;
    uchar music_volume;
    char screen_x;
    char screen_y;
    uchar language;
    struct Hub hub[6];
    struct level level[35];
    uchar lives;
    uchar wumpas;
    uchar mask;
    uchar percent;
    uchar crystals;
    uchar relics;
    uchar crate_gems;
    uchar bonus_gems;
    uchar gems;
    uchar gembits;
    uchar powerbits;
    uchar empty;
    uint cutbits;
    uchar year;
    uchar month;
    uchar day;
    uchar hours;
    uchar mins;
    uchar pad_[3];
};

typedef struct LDATA LDATA, *PLDATA;

typedef struct pCHASE pCHASE, *PpCHASE;

typedef struct pSFX pSFX, *PpSFX;

struct pSFX {
    char name[16];
    ushort pitch;
    ushort volume;
    uchar buzz;
    uchar rumble;
    uchar delay;
    uchar wait;
    char * path;
    ushort frequency;
    ushort stream;
    char type;
    char pad1;
    ushort id;
    struct nuvec_s Pos;
};

struct pCHASE {
    char i;
    char i_last;
    char i_next;
    uchar cuboid;
    short character[6];
    short action[6];
    float scale[6];
    float duration;
};

struct LDATA {
    char * filepath;
    uchar * cList; /* modelList */
    struct pCHASE * pChase;
    uint time[3];
    short music[2];
    struct pSFX * pSFX;
    short nSFX;
    char pad1;
    char hub;
    ushort flags;
    short character;
    short vehicle;
    ushort farclip;
    struct nuvec_s vSTART;
    struct nuvec_s vBONUS;
    float fognear;
    float fogfar;
    uchar fogr;
    uchar fogg;
    uchar fogb;
    uchar foga;
    uchar hazer;
    uchar hazeg;
    uchar hazeb;
    uchar hazea;
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
    struct AnimList * anim;
    float radius;
    struct nuvec_s min;
    struct nuvec_s max;
    float scale;
    float shadow_scale;
};


typedef struct CharacterModel CharacterModel, *PCharacterModel;

struct CharacterModel {
    struct NUHGOBJ_s * hobj;
    struct NuAnimData_s * anmdata[118];
    struct AnimList * animlist[118];
    struct NuAnimData_s * fanmdata[118];
    struct AnimList * fanimlist[118];
    int shadhdr;
    short character;
    char pad1;
    char pad2;
    struct NUPOINTOFINTEREST_s * pLOCATOR[16];
};

typedef struct CocoMoveInfo CocoMoveInfo, *PCocoMoveInfo;

struct CocoMoveInfo {
    float IDLESPEED;
    float TIPTOESPEED;
    float WALKSPEED;
    float RUNSPEED;
    float SPRINTSPEED;
    float SLIDESPEED;
    float CRAWLSPEED;
    float DANGLESPEED;
    float WADESPEED;
    float JUMPHEIGHT;
    float DANGLEGAP;
    short JUMPFRAMES0;
    short JUMPFRAMES1;
    short JUMPFRAMES2;
    short STARJUMPFRAMES;
    short SOMERSAULTFRAMES;
    short SPINFRAMES;
    short SPINRESETFRAMES;
    short SUPERSPINFRAMES;
    short SUPERSPINWAITFRAMES;
    short SLAMWAITFRAMES;
    short SLIDEFRAMES;
    short CROUCHINGFRAMES;
    short JUMPLANDFRAMES;
    short spad;
};

typedef struct CrashMoveInfo CrashMoveInfo, *PCrashMoveInfo;

struct CrashMoveInfo {
    float IDLESPEED;
    float TIPTOESPEED;
    float WALKSPEED;
    float RUNSPEED;
    float SPRINTSPEED;
    float SLIDESPEED;
    float CRAWLSPEED;
    float DANGLESPEED;
    float WADESPEED;
    float JUMPHEIGHT;
    float DANGLEGAP;
    short JUMPFRAMES0;
    short JUMPFRAMES1;
    short JUMPFRAMES2;
    short STARJUMPFRAMES;
    short SOMERSAULTFRAMES;
    short SPINFRAMES;
    short SPINRESETFRAMES;
    short SUPERSPINFRAMES;
    short SUPERSPINWAITFRAMES;
    short SLAMWAITFRAMES;
    short SLIDEFRAMES;
    short CROUCHINGFRAMES;
    short JUMPLANDFRAMES;
    short spad;
};

struct Moveinfo_s {
    float IDLESPEED;
    float TIPTOESPEED;
    float WALKSPEED;
    float RUNSPEED;
    float SPRINTSPEED;
    float SLIDESPEED;
    float CRAWLSPEED;
    float DANGLESPEED;
    float WADESPEED;
    float JUMPHEIGHT;
    float DANGLEGAP;
    short JUMPFRAMES0;
    short JUMPFRAMES1;
    short JUMPFRAMES2;
    short STARJUMPFRAMES;
    short SOMERSAULTFRAMES;
    short SPINFRAMES;
    short SPINRESETFRAMES;
    short SUPERSPINFRAMES;
    short SUPERSPINWAITFRAMES;
    short SLAMWAITFRAMES;
    short SLIDEFRAMES;
    short CROUCHINGFRAMES;
    short JUMPLANDFRAMES;
    short spad;
};

struct RPos_s {
    char iRAIL;
    char vertical;
    short iALONG;
    short i1;
    short i2;
    float fALONG;
    float fACROSS;
    ushort angle;
    ushort cam_angle;
    uchar mode;
    char pad1;
    char pad2;
    char pad3;
    struct nuvec_s pos;
};

struct Nearest_Light_s {
    int AmbIndex;
    struct nuvec_s AmbCol;
    float ambientdist;
    int CurLoopIndex;
    struct pdir_s dir1;
    struct pdir_s dir2;
    struct pdir_s dir3;
    struct pdir_s * pDir1st;
    struct pdir_s * pDir2nd;
    struct pdir_s * pDir3rd;
    int glbambindex;
    int negativeindex;
    float negativedist;
    struct pdir_s glbdirectional;
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
    ushort xrot;
    ushort yrot;
    ushort angle;
    ushort surface_xrot;
    ushort surface_zrot;
    ushort wibble_ang[3];
    float idle_time;
    float idle_duration;
    char idle_mode;
    char reflect;
    uchar offset_frame;
    char hold;
};

struct obj_s {
    struct obj_s * parent;
    struct CharacterModel * model;
    struct mask * mask;
    void * contact;
    struct numtx_s * pLOCATOR;
    struct anim_s anim;
    short character;
    short vehicle;
    uint flags;
    uint frame;
    uint draw_frame;
    uint vehicle_frame;
    struct RPos_s RPos;
    struct nuvec_s pos;
    struct nuvec_s mom;
    struct nuvec_s oldpos;
    struct nuvec_s startpos;
    struct nuvec_s vSN;
    struct nuvec_s vLN;
    struct nuvec_s vRN;
    float shadow;
    float layer_shadow;
    float roof_y;
    float clearance;
    float forward;
    float abs_forward;
    float side;
    float abs_side;
    float xyz_distance;
    float xz_distance;
    float radius;
    struct nuvec_s min;
    struct nuvec_s max;
    float scale;
    float SCALE;
    float RADIUS;
    float old_SCALE;
    float objbot;
    float objtop;
    float bot;
    float top;
    float oldobjbot;
    float oldobjtop;
    float die_time;
    float die_duration;
    float reflect_y;
    float idle_gametime;
    float pad_speed;
    float pad_dx;
    float pad_dz;
    char i;
    char dead;
    ushort pad_angle;
    ushort attack;
    ushort vulnerable;
    short die_action;
    char old_ground;
    char finished;
    ushort xrot;
    ushort yrot;
    ushort zrot;
    ushort surface_xrot;
    ushort surface_zrot;
    ushort layer_xrot;
    ushort layer_zrot;
    ushort roof_xrot;
    ushort roof_zrot;
    short target_xrot;
    short target_yrot;
    short dyrot;
    union type_s_name gndflags;
    ushort hdg;
    ushort thdg;
    char ground;
    char surface_type;
    char layer_type;
    char roof_type;
    char invisible;
    uchar submerged;
    char transporting;
    char got_shadow;
    uchar boing;
    uchar contact_type;
    char die_model[2];
    uchar invincible;
    char pos_adjusted;
    char wade;
    char dangle;
    char ddsand;
    char ddsnow;
    char ddwater;
    char ddr;
    char ddg;
    char ddb;
    char last_ground;
    char direction;
    char kill_contact;
    uchar touch;
};

typedef struct csc_s csc_s, *Pcsc_s;

struct csc_s {
    struct NUHGOBJ_s * obj;
    char * path;
    char * name;
};



typedef struct creature_s creature_s, *Pcreature_s;

typedef struct obj_s obj_s, *Pobj_s;

typedef struct PADStatus PADStatus, *PPADStatus;

struct PADStatus {
    uint button;
    short stickX;
    short stickY;
    short substickX;
    short substickY;
    uint triggerLeft;
    uint triggerRight;
    uint analogA;
    uint analogB;
    short err;
};

typedef struct PData PData, *PPData;

struct PData {
    uchar character;
    uchar font3d_letter;
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
    ushort xrot;
    ushort yrot;
    ushort zrot;
    char pad1;
    char pad2;
};


// Size: 0xCE4, DWARF: 0x4B20
struct creature_s
{
    signed char used; // Offset: 0x0, DWARF: 0x4B3F
    signed char on; // Offset: 0x1, DWARF: 0x4B66
    signed char off_wait; // Offset: 0x2, DWARF: 0x4B8B
    signed char i_aitab; // Offset: 0x3, DWARF: 0x4BB6
    // Size: 0x188, DWARF: 0x54E3
    struct
    {
        void* parent; // Offset: 0x0, DWARF: 0x54F5
        // Size: 0x7AC, DWARF: 0x786D
        struct
        {
            NUHGOBJ_s* obj; // Offset: 0x0, DWARF: 0x787F
            nuanimdata_s* anmdata[118]; // Offset: 0x4, DWARF: 0x78AA
            // Size: 0x18, DWARF: 0x8C4A
            struct
            {
                char* file; // Offset: 0x0, DWARF: 0x8C5C
                short action; // Offset: 0x4, DWARF: 0x8C86
                unsigned char blend_in_frames; // Offset: 0x6, DWARF: 0x8CAF
                unsigned char blend_out_frames; // Offset: 0x7, DWARF: 0x8CE1
                float speed; // Offset: 0x8, DWARF: 0x8D14
                unsigned short flags; // Offset: 0xC, DWARF: 0x8D3C
                char pad1; // Offset: 0xE, DWARF: 0x8D64
                char pad2; // Offset: 0xF, DWARF: 0x8D8B
                <unknown type 0x8208> levbits; // Offset: 0x10, DWARF: 0x8DB2
            }* animlist[118]; // Offset: 0x1DC, DWARF: 0x78D6
            nuanimdata_s* fanmdata[118]; // Offset: 0x3B4, DWARF: 0x7903
            // Size: 0x18, DWARF: 0x8C4A
            struct
            {
                char* file; // Offset: 0x0, DWARF: 0x8C5C
                short action; // Offset: 0x4, DWARF: 0x8C86
                unsigned char blend_in_frames; // Offset: 0x6, DWARF: 0x8CAF
                unsigned char blend_out_frames; // Offset: 0x7, DWARF: 0x8CE1
                float speed; // Offset: 0x8, DWARF: 0x8D14
                unsigned short flags; // Offset: 0xC, DWARF: 0x8D3C
                char pad1; // Offset: 0xE, DWARF: 0x8D64
                char pad2; // Offset: 0xF, DWARF: 0x8D8B
                <unknown type 0x8208> levbits; // Offset: 0x10, DWARF: 0x8DB2
            }* fanimlist[118]; // Offset: 0x58C, DWARF: 0x7930
            int shadhdr; // Offset: 0x764, DWARF: 0x795E
            short character; // Offset: 0x768, DWARF: 0x7988
            char pad1; // Offset: 0x76A, DWARF: 0x79B4
            char pad2; // Offset: 0x76B, DWARF: 0x79DB
            NUPOINTOFINTEREST_s* pLOCATOR[16]; // Offset: 0x76C, DWARF: 0x7A02
        }* model; // Offset: 0x4, DWARF: 0x5521
        // Size: 0x190, DWARF: 0x7A33
        struct
        {
            numtx_s mM; // Offset: 0x0, DWARF: 0x7A45
            numtx_s mS; // Offset: 0x40, DWARF: 0x7A6C
            nuvec_s pos; // Offset: 0x80, DWARF: 0x7A93
            nuvec_s newpos; // Offset: 0x8C, DWARF: 0x7ABB
            // Size: 0xB0, DWARF: 0x7526
            struct
            {
                int AmbIndex; // Offset: 0x0, DWARF: 0x7538
                nuvec_s AmbCol; // Offset: 0x4, DWARF: 0x7563
                float ambientdist; // Offset: 0x10, DWARF: 0x758E
                int CurLoopIndex; // Offset: 0x14, DWARF: 0x75BC
                // Size: 0x20, DWARF: 0x846B
                struct
                {
                    int Index; // Offset: 0x0, DWARF: 0x847D
                    nuvec_s Direction; // Offset: 0x4, DWARF: 0x84A5
                    nucolour3_s Colour; // Offset: 0x10, DWARF: 0x84D3
                    float Distance; // Offset: 0x1C, DWARF: 0x84FE
                } dir1; // Offset: 0x18, DWARF: 0x75EB
                // Size: 0x20, DWARF: 0x846B
                struct
                {
                    int Index; // Offset: 0x0, DWARF: 0x847D
                    nuvec_s Direction; // Offset: 0x4, DWARF: 0x84A5
                    nucolour3_s Colour; // Offset: 0x10, DWARF: 0x84D3
                    float Distance; // Offset: 0x1C, DWARF: 0x84FE
                } dir2; // Offset: 0x38, DWARF: 0x7614
                // Size: 0x20, DWARF: 0x846B
                struct
                {
                    int Index; // Offset: 0x0, DWARF: 0x847D
                    nuvec_s Direction; // Offset: 0x4, DWARF: 0x84A5
                    nucolour3_s Colour; // Offset: 0x10, DWARF: 0x84D3
                    float Distance; // Offset: 0x1C, DWARF: 0x84FE
                } dir3; // Offset: 0x58, DWARF: 0x763D
                // Size: 0x20, DWARF: 0x846B
                struct
                {
                    int Index; // Offset: 0x0, DWARF: 0x847D
                    nuvec_s Direction; // Offset: 0x4, DWARF: 0x84A5
                    nucolour3_s Colour; // Offset: 0x10, DWARF: 0x84D3
                    float Distance; // Offset: 0x1C, DWARF: 0x84FE
                }* pDir1st; // Offset: 0x78, DWARF: 0x7666
                // Size: 0x20, DWARF: 0x846B
                struct
                {
                    int Index; // Offset: 0x0, DWARF: 0x847D
                    nuvec_s Direction; // Offset: 0x4, DWARF: 0x84A5
                    nucolour3_s Colour; // Offset: 0x10, DWARF: 0x84D3
                    float Distance; // Offset: 0x1C, DWARF: 0x84FE
                }* pDir2nd; // Offset: 0x7C, DWARF: 0x7695
                // Size: 0x20, DWARF: 0x846B
                struct
                {
                    int Index; // Offset: 0x0, DWARF: 0x847D
                    nuvec_s Direction; // Offset: 0x4, DWARF: 0x84A5
                    nucolour3_s Colour; // Offset: 0x10, DWARF: 0x84D3
                    float Distance; // Offset: 0x1C, DWARF: 0x84FE
                }* pDir3rd; // Offset: 0x80, DWARF: 0x76C4
                int glbambindex; // Offset: 0x84, DWARF: 0x76F3
                int negativeindex; // Offset: 0x88, DWARF: 0x7721
                float negativedist; // Offset: 0x8C, DWARF: 0x7751
                // Size: 0x20, DWARF: 0x846B
                struct
                {
                    int Index; // Offset: 0x0, DWARF: 0x847D
                    nuvec_s Direction; // Offset: 0x4, DWARF: 0x84A5
                    nucolour3_s Colour; // Offset: 0x10, DWARF: 0x84D3
                    float Distance; // Offset: 0x1C, DWARF: 0x84FE
                } glbdirectional; // Offset: 0x90, DWARF: 0x7780
            } lights; // Offset: 0x98, DWARF: 0x7AE6
            // Size: 0x1C, DWARF: 0x7E63
            struct
            {
                float anim_time; // Offset: 0x0, DWARF: 0x7E75
                float blend_src_time; // Offset: 0x4, DWARF: 0x7EA1
                float blend_dst_time; // Offset: 0x8, DWARF: 0x7ED2
                short action; // Offset: 0xC, DWARF: 0x7F03
                short oldaction; // Offset: 0xE, DWARF: 0x7F2C
                short newaction; // Offset: 0x10, DWARF: 0x7F58
                short blend_src_action; // Offset: 0x12, DWARF: 0x7F84
                short blend_dst_action; // Offset: 0x14, DWARF: 0x7FB7
                short blend_frame; // Offset: 0x16, DWARF: 0x7FEA
                short blend_frames; // Offset: 0x18, DWARF: 0x8018
                signed char blend; // Offset: 0x1A, DWARF: 0x8047
                unsigned char flags; // Offset: 0x1B, DWARF: 0x806F
            } anim; // Offset: 0x148, DWARF: 0x7B11
            float scale; // Offset: 0x164, DWARF: 0x7B3A
            float shadow; // Offset: 0x168, DWARF: 0x7B62
            short character; // Offset: 0x16C, DWARF: 0x7B8B
            short active; // Offset: 0x16E, DWARF: 0x7BB7
            short sfx; // Offset: 0x170, DWARF: 0x7BE0
            char pad1; // Offset: 0x172, DWARF: 0x7C06
            char pad2; // Offset: 0x173, DWARF: 0x7C2D
            unsigned short xrot; // Offset: 0x174, DWARF: 0x7C54
            unsigned short yrot; // Offset: 0x176, DWARF: 0x7C7B
            unsigned short angle; // Offset: 0x178, DWARF: 0x7CA2
            unsigned short surface_xrot; // Offset: 0x17A, DWARF: 0x7CCA
            unsigned short surface_zrot; // Offset: 0x17C, DWARF: 0x7CF9
            unsigned short wibble_ang[3]; // Offset: 0x17E, DWARF: 0x7D28
            float idle_time; // Offset: 0x184, DWARF: 0x7D57
            float idle_duration; // Offset: 0x188, DWARF: 0x7D83
            signed char idle_mode; // Offset: 0x18C, DWARF: 0x7DB3
            signed char reflect; // Offset: 0x18D, DWARF: 0x7DDF
            unsigned char offset_frame; // Offset: 0x18E, DWARF: 0x7E09
            signed char hold; // Offset: 0x18F, DWARF: 0x7E38
        }* mask; // Offset: 0x8, DWARF: 0x554E
        void* contact; // Offset: 0xC, DWARF: 0x557A
        numtx_s* pLOCATOR; // Offset: 0x10, DWARF: 0x55A7
        // Size: 0x1C, DWARF: 0x7E63
        struct
        {
            float anim_time; // Offset: 0x0, DWARF: 0x7E75
            float blend_src_time; // Offset: 0x4, DWARF: 0x7EA1
            float blend_dst_time; // Offset: 0x8, DWARF: 0x7ED2
            short action; // Offset: 0xC, DWARF: 0x7F03
            short oldaction; // Offset: 0xE, DWARF: 0x7F2C
            short newaction; // Offset: 0x10, DWARF: 0x7F58
            short blend_src_action; // Offset: 0x12, DWARF: 0x7F84
            short blend_dst_action; // Offset: 0x14, DWARF: 0x7FB7
            short blend_frame; // Offset: 0x16, DWARF: 0x7FEA
            short blend_frames; // Offset: 0x18, DWARF: 0x8018
            signed char blend; // Offset: 0x1A, DWARF: 0x8047
            unsigned char flags; // Offset: 0x1B, DWARF: 0x806F
        } anim; // Offset: 0x14, DWARF: 0x55D7
        short character; // Offset: 0x30, DWARF: 0x5600
        short vehicle; // Offset: 0x32, DWARF: 0x562C
        unsigned int flags; // Offset: 0x34, DWARF: 0x5656
        unsigned int frame; // Offset: 0x38, DWARF: 0x567E
        unsigned int draw_frame; // Offset: 0x3C, DWARF: 0x56A6
        unsigned int vehicle_frame; // Offset: 0x40, DWARF: 0x56D3
        // Size: 0x24, DWARF: 0x809B
        struct
        {
            signed char iRAIL; // Offset: 0x0, DWARF: 0x80AD
            signed char vertical; // Offset: 0x1, DWARF: 0x80D5
            short iALONG; // Offset: 0x2, DWARF: 0x8100
            short i1; // Offset: 0x4, DWARF: 0x8129
            short i2; // Offset: 0x6, DWARF: 0x814E
            float fALONG; // Offset: 0x8, DWARF: 0x8173
            float fACROSS; // Offset: 0xC, DWARF: 0x819C
            unsigned short angle; // Offset: 0x10, DWARF: 0x81C6
            unsigned short cam_angle; // Offset: 0x12, DWARF: 0x81EE
            unsigned char mode; // Offset: 0x14, DWARF: 0x821A
            char pad1; // Offset: 0x15, DWARF: 0x8241
            char pad2; // Offset: 0x16, DWARF: 0x8268
            char pad3; // Offset: 0x17, DWARF: 0x828F
            nuvec_s pos; // Offset: 0x18, DWARF: 0x82B6
        } RPos; // Offset: 0x44, DWARF: 0x5703
        nuvec_s pos; // Offset: 0x68, DWARF: 0x572C
        nuvec_s mom; // Offset: 0x74, DWARF: 0x5754
        nuvec_s oldpos; // Offset: 0x80, DWARF: 0x577C
        nuvec_s startpos; // Offset: 0x8C, DWARF: 0x57A7
        nuvec_s vSN; // Offset: 0x98, DWARF: 0x57D4
        nuvec_s vLN; // Offset: 0xA4, DWARF: 0x57FC
        nuvec_s vRN; // Offset: 0xB0, DWARF: 0x5824
        float shadow; // Offset: 0xBC, DWARF: 0x584C
        float layer_shadow; // Offset: 0xC0, DWARF: 0x5875
        float roof_y; // Offset: 0xC4, DWARF: 0x58A4
        float clearance; // Offset: 0xC8, DWARF: 0x58CD
        float forward; // Offset: 0xCC, DWARF: 0x58F9
        float abs_forward; // Offset: 0xD0, DWARF: 0x5923
        float side; // Offset: 0xD4, DWARF: 0x5951
        float abs_side; // Offset: 0xD8, DWARF: 0x5978
        float xyz_distance; // Offset: 0xDC, DWARF: 0x59A3
        float xz_distance; // Offset: 0xE0, DWARF: 0x59D2
        float radius; // Offset: 0xE4, DWARF: 0x5A00
        nuvec_s min; // Offset: 0xE8, DWARF: 0x5A29
        nuvec_s max; // Offset: 0xF4, DWARF: 0x5A51
        float scale; // Offset: 0x100, DWARF: 0x5A79
        float SCALE; // Offset: 0x104, DWARF: 0x5AA1
        float RADIUS; // Offset: 0x108, DWARF: 0x5AC9
        float old_SCALE; // Offset: 0x10C, DWARF: 0x5AF2
        float objbot; // Offset: 0x110, DWARF: 0x5B1E
        float objtop; // Offset: 0x114, DWARF: 0x5B47
        float bot; // Offset: 0x118, DWARF: 0x5B70
        float top; // Offset: 0x11C, DWARF: 0x5B96
        float oldobjbot; // Offset: 0x120, DWARF: 0x5BBC
        float oldobjtop; // Offset: 0x124, DWARF: 0x5BE8
        float die_time; // Offset: 0x128, DWARF: 0x5C14
        float die_duration; // Offset: 0x12C, DWARF: 0x5C3F
        float reflect_y; // Offset: 0x130, DWARF: 0x5C6E
        float idle_gametime; // Offset: 0x134, DWARF: 0x5C9A
        float pad_speed; // Offset: 0x138, DWARF: 0x5CCA
        float pad_dx; // Offset: 0x13C, DWARF: 0x5CF6
        float pad_dz; // Offset: 0x140, DWARF: 0x5D1F
        signed char i; // Offset: 0x144, DWARF: 0x5D48
        signed char dead; // Offset: 0x145, DWARF: 0x5D6C
        unsigned short pad_angle; // Offset: 0x146, DWARF: 0x5D93
        unsigned short attack; // Offset: 0x148, DWARF: 0x5DBF
        unsigned short vulnerable; // Offset: 0x14A, DWARF: 0x5DE8
        short die_action; // Offset: 0x14C, DWARF: 0x5E15
        signed char old_ground; // Offset: 0x14E, DWARF: 0x5E42
        signed char finished; // Offset: 0x14F, DWARF: 0x5E6F
        unsigned short xrot; // Offset: 0x150, DWARF: 0x5E9A
        unsigned short yrot; // Offset: 0x152, DWARF: 0x5EC1
        unsigned short zrot; // Offset: 0x154, DWARF: 0x5EE8
        unsigned short surface_xrot; // Offset: 0x156, DWARF: 0x5F0F
        unsigned short surface_zrot; // Offset: 0x158, DWARF: 0x5F3E
        unsigned short layer_xrot; // Offset: 0x15A, DWARF: 0x5F6D
        unsigned short layer_zrot; // Offset: 0x15C, DWARF: 0x5F9A
        unsigned short roof_xrot; // Offset: 0x15E, DWARF: 0x5FC7
        unsigned short roof_zrot; // Offset: 0x160, DWARF: 0x5FF3
        short target_xrot; // Offset: 0x162, DWARF: 0x601F
        short target_yrot; // Offset: 0x164, DWARF: 0x604D
        short dyrot; // Offset: 0x166, DWARF: 0x607B
        // Size: 0x2, DWARF: 0x82E2
        union
        {
            unsigned char chrs[2]; // Offset: 0x0, DWARF: 0x82F4
            unsigned short all; // Offset: 0x0, DWARF: 0x831D
        } gndflags; // Offset: 0x168, DWARF: 0x60A3
        unsigned short hdg; // Offset: 0x16A, DWARF: 0x60D0
        unsigned short thdg; // Offset: 0x16C, DWARF: 0x60F6
        signed char ground; // Offset: 0x16E, DWARF: 0x611D
        signed char surface_type; // Offset: 0x16F, DWARF: 0x6146
        signed char layer_type; // Offset: 0x170, DWARF: 0x6175
        signed char roof_type; // Offset: 0x171, DWARF: 0x61A2
        signed char invisible; // Offset: 0x172, DWARF: 0x61CE
        unsigned char submerged; // Offset: 0x173, DWARF: 0x61FA
        signed char transporting; // Offset: 0x174, DWARF: 0x6226
        signed char got_shadow; // Offset: 0x175, DWARF: 0x6255
        unsigned char boing; // Offset: 0x176, DWARF: 0x6282
        unsigned char contact_type; // Offset: 0x177, DWARF: 0x62AA
        signed char die_model[2]; // Offset: 0x178, DWARF: 0x62D9
        unsigned char invincible; // Offset: 0x17A, DWARF: 0x6307
        signed char pos_adjusted; // Offset: 0x17B, DWARF: 0x6334
        signed char wade; // Offset: 0x17C, DWARF: 0x6363
        signed char dangle; // Offset: 0x17D, DWARF: 0x638A
        char ddsand; // Offset: 0x17E, DWARF: 0x63B3
        char ddsnow; // Offset: 0x17F, DWARF: 0x63DC
        char ddwater; // Offset: 0x180, DWARF: 0x6405
        char ddr; // Offset: 0x181, DWARF: 0x642F
        char ddg; // Offset: 0x182, DWARF: 0x6455
        char ddb; // Offset: 0x183, DWARF: 0x647B
        signed char last_ground; // Offset: 0x184, DWARF: 0x64A1
        signed char direction; // Offset: 0x185, DWARF: 0x64CF
        signed char kill_contact; // Offset: 0x186, DWARF: 0x64FB
        unsigned char touch; // Offset: 0x187, DWARF: 0x652A
    } obj; // Offset: 0x4, DWARF: 0x4BE0
    // Size: 0x98, DWARF: 0x6556
    struct
    {
        nuvec_s oldpos; // Offset: 0x0, DWARF: 0x6568
        nuvec_s newpos; // Offset: 0xC, DWARF: 0x6593
        nuvec_s wobblepos; // Offset: 0x18, DWARF: 0x65BE
        float time; // Offset: 0x24, DWARF: 0x65EC
        float duration; // Offset: 0x28, DWARF: 0x6613
        float spins; // Offset: 0x2C, DWARF: 0x663E
        float speed; // Offset: 0x30, DWARF: 0x6666
        float radius; // Offset: 0x34, DWARF: 0x668E
        float distance; // Offset: 0x38, DWARF: 0x66B7
        float height; // Offset: 0x3C, DWARF: 0x66E2
        float lateral; // Offset: 0x40, DWARF: 0x670B
        float scale; // Offset: 0x44, DWARF: 0x6735
        float wobble; // Offset: 0x48, DWARF: 0x675D
        unsigned short oldangle; // Offset: 0x4C, DWARF: 0x6786
        unsigned short angle; // Offset: 0x4E, DWARF: 0x67B1
        unsigned short old_yrot; // Offset: 0x50, DWARF: 0x67D9
        unsigned short movflags; // Offset: 0x52, DWARF: 0x6804
        unsigned short terflags; // Offset: 0x54, DWARF: 0x682F
        unsigned short rotflags; // Offset: 0x56, DWARF: 0x685A
        unsigned short rotspeed; // Offset: 0x58, DWARF: 0x6885
        unsigned short rotadjust; // Offset: 0x5A, DWARF: 0x68B0
        unsigned short wobble_ang; // Offset: 0x5C, DWARF: 0x68DC
        unsigned short wobble_speed; // Offset: 0x5E, DWARF: 0x6909
        signed char direction; // Offset: 0x60, DWARF: 0x6938
        signed char i0; // Offset: 0x61, DWARF: 0x6964
        signed char i1; // Offset: 0x62, DWARF: 0x6989
        signed char idle_update; // Offset: 0x63, DWARF: 0x69AE
        void* event_function[2]; // Offset: 0x64, DWARF: 0x69DC
        float event_distance[2]; // Offset: 0x6C, DWARF: 0x6A0F
        short event_branch[2]; // Offset: 0x74, DWARF: 0x6A42
        signed char event[2]; // Offset: 0x78, DWARF: 0x6A73
        signed char event_index[2]; // Offset: 0x7A, DWARF: 0x6A9D
        unsigned char event_flags[2]; // Offset: 0x7C, DWARF: 0x6ACD
        unsigned char event_occured[2]; // Offset: 0x7E, DWARF: 0x6AFD
        unsigned short attack_locator_bits; // Offset: 0x80, DWARF: 0x6B2F
        char pad1; // Offset: 0x82, DWARF: 0x6B65
        char pad2; // Offset: 0x83, DWARF: 0x6B8C
        float attack_locator_radius; // Offset: 0x84, DWARF: 0x6BB3
        nuhspecial_s obj; // Offset: 0x88, DWARF: 0x6BEB
        signed char kill; // Offset: 0x90, DWARF: 0x6C13
        signed char locator_kill; // Offset: 0x91, DWARF: 0x6C3A
        short force_action; // Offset: 0x92, DWARF: 0x6C69
        short look_creature; // Offset: 0x94, DWARF: 0x6C98
        unsigned char count; // Offset: 0x96, DWARF: 0x6CC8
        unsigned char hits; // Offset: 0x97, DWARF: 0x6CF0
    } ai; // Offset: 0x18C, DWARF: 0x4C08
    NEWBUGGY* Buggy; // Offset: 0x224, DWARF: 0x4C2F
    creatcmd_s* cmdtable; // Offset: 0x228, DWARF: 0x4C5C
    creatcmd_s* cmdcurr; // Offset: 0x22C, DWARF: 0x4C8C
    // Size: 0x48, DWARF: 0x6DAC
    struct
    {
        float IDLESPEED; // Offset: 0x0, DWARF: 0x6DBE
        float TIPTOESPEED; // Offset: 0x4, DWARF: 0x6DEA
        float WALKSPEED; // Offset: 0x8, DWARF: 0x6E18
        float RUNSPEED; // Offset: 0xC, DWARF: 0x6E44
        float SPRINTSPEED; // Offset: 0x10, DWARF: 0x6E6F
        float SLIDESPEED; // Offset: 0x14, DWARF: 0x6E9D
        float CRAWLSPEED; // Offset: 0x18, DWARF: 0x6ECA
        float DANGLESPEED; // Offset: 0x1C, DWARF: 0x6EF7
        float WADESPEED; // Offset: 0x20, DWARF: 0x6F25
        float JUMPHEIGHT; // Offset: 0x24, DWARF: 0x6F51
        float DANGLEGAP; // Offset: 0x28, DWARF: 0x6F7E
        short JUMPFRAMES0; // Offset: 0x2C, DWARF: 0x6FAA
        short JUMPFRAMES1; // Offset: 0x2E, DWARF: 0x6FD8
        short JUMPFRAMES2; // Offset: 0x30, DWARF: 0x7006
        short STARJUMPFRAMES; // Offset: 0x32, DWARF: 0x7034
        short SOMERSAULTFRAMES; // Offset: 0x34, DWARF: 0x7065
        short SPINFRAMES; // Offset: 0x36, DWARF: 0x7098
        short SPINRESETFRAMES; // Offset: 0x38, DWARF: 0x70C5
        short SUPERSPINFRAMES; // Offset: 0x3A, DWARF: 0x70F7
        short SUPERSPINWAITFRAMES; // Offset: 0x3C, DWARF: 0x7129
        short SLAMWAITFRAMES; // Offset: 0x3E, DWARF: 0x715F
        short SLIDEFRAMES; // Offset: 0x40, DWARF: 0x7190
        short CROUCHINGFRAMES; // Offset: 0x42, DWARF: 0x71BE
        short JUMPLANDFRAMES; // Offset: 0x44, DWARF: 0x71F0
        short spad; // Offset: 0x46, DWARF: 0x7221
    }* minfo; // Offset: 0x230, DWARF: 0x4CBB
    numtx_s m; // Offset: 0x234, DWARF: 0x4CE8
    numtx_s mtxLOCATOR[16][2]; // Offset: 0x274, DWARF: 0x4D0E
    nuvec_s momLOCATOR[16][2]; // Offset: 0xA74, DWARF: 0x4D3D
    // Size: 0xB0, DWARF: 0x7526
    struct
    {
        int AmbIndex; // Offset: 0x0, DWARF: 0x7538
        nuvec_s AmbCol; // Offset: 0x4, DWARF: 0x7563
        float ambientdist; // Offset: 0x10, DWARF: 0x758E
        int CurLoopIndex; // Offset: 0x14, DWARF: 0x75BC
        // Size: 0x20, DWARF: 0x846B
        struct
        {
            int Index; // Offset: 0x0, DWARF: 0x847D
            nuvec_s Direction; // Offset: 0x4, DWARF: 0x84A5
            nucolour3_s Colour; // Offset: 0x10, DWARF: 0x84D3
            float Distance; // Offset: 0x1C, DWARF: 0x84FE
        } dir1; // Offset: 0x18, DWARF: 0x75EB
        // Size: 0x20, DWARF: 0x846B
        struct
        {
            int Index; // Offset: 0x0, DWARF: 0x847D
            nuvec_s Direction; // Offset: 0x4, DWARF: 0x84A5
            nucolour3_s Colour; // Offset: 0x10, DWARF: 0x84D3
            float Distance; // Offset: 0x1C, DWARF: 0x84FE
        } dir2; // Offset: 0x38, DWARF: 0x7614
        // Size: 0x20, DWARF: 0x846B
        struct
        {
            int Index; // Offset: 0x0, DWARF: 0x847D
            nuvec_s Direction; // Offset: 0x4, DWARF: 0x84A5
            nucolour3_s Colour; // Offset: 0x10, DWARF: 0x84D3
            float Distance; // Offset: 0x1C, DWARF: 0x84FE
        } dir3; // Offset: 0x58, DWARF: 0x763D
        // Size: 0x20, DWARF: 0x846B
        struct
        {
            int Index; // Offset: 0x0, DWARF: 0x847D
            nuvec_s Direction; // Offset: 0x4, DWARF: 0x84A5
            nucolour3_s Colour; // Offset: 0x10, DWARF: 0x84D3
            float Distance; // Offset: 0x1C, DWARF: 0x84FE
        }* pDir1st; // Offset: 0x78, DWARF: 0x7666
        // Size: 0x20, DWARF: 0x846B
        struct
        {
            int Index; // Offset: 0x0, DWARF: 0x847D
            nuvec_s Direction; // Offset: 0x4, DWARF: 0x84A5
            nucolour3_s Colour; // Offset: 0x10, DWARF: 0x84D3
            float Distance; // Offset: 0x1C, DWARF: 0x84FE
        }* pDir2nd; // Offset: 0x7C, DWARF: 0x7695
        // Size: 0x20, DWARF: 0x846B
        struct
        {
            int Index; // Offset: 0x0, DWARF: 0x847D
            nuvec_s Direction; // Offset: 0x4, DWARF: 0x84A5
            nucolour3_s Colour; // Offset: 0x10, DWARF: 0x84D3
            float Distance; // Offset: 0x1C, DWARF: 0x84FE
        }* pDir3rd; // Offset: 0x80, DWARF: 0x76C4
        int glbambindex; // Offset: 0x84, DWARF: 0x76F3
        int negativeindex; // Offset: 0x88, DWARF: 0x7721
        float negativedist; // Offset: 0x8C, DWARF: 0x7751
        // Size: 0x20, DWARF: 0x846B
        struct
        {
            int Index; // Offset: 0x0, DWARF: 0x847D
            nuvec_s Direction; // Offset: 0x4, DWARF: 0x84A5
            nucolour3_s Colour; // Offset: 0x10, DWARF: 0x84D3
            float Distance; // Offset: 0x1C, DWARF: 0x84FE
        } glbdirectional; // Offset: 0x90, DWARF: 0x7780
    } lights; // Offset: 0xBF4, DWARF: 0x4D6C
    // Size: 0x4, DWARF: 0x77B7
    struct
    {
        unsigned char buzz; // Offset: 0x0, DWARF: 0x77C9
        unsigned char power; // Offset: 0x1, DWARF: 0x77F0
        unsigned char frame; // Offset: 0x2, DWARF: 0x7818
        unsigned char frames; // Offset: 0x3, DWARF: 0x7840
    } rumble; // Offset: 0xCA4, DWARF: 0x4D97
    float idle_time; // Offset: 0xCA8, DWARF: 0x4DC2
    float idle_wait; // Offset: 0xCAC, DWARF: 0x4DEE
    short idle_action; // Offset: 0xCB0, DWARF: 0x4E1A
    short old_idle_action; // Offset: 0xCB2, DWARF: 0x4E48
    signed char idle_mode; // Offset: 0xCB4, DWARF: 0x4E7A
    signed char idle_repeat; // Offset: 0xCB5, DWARF: 0x4EA6
    signed char jump; // Offset: 0xCB6, DWARF: 0x4ED4
    signed char jump_type; // Offset: 0xCB7, DWARF: 0x4EFB
    signed char jump_subtype; // Offset: 0xCB8, DWARF: 0x4F27
    signed char ok_slam; // Offset: 0xCB9, DWARF: 0x4F56
    signed char slam; // Offset: 0xCBA, DWARF: 0x4F80
    signed char spin; // Offset: 0xCBB, DWARF: 0x4FA7
    signed char crawl; // Offset: 0xCBC, DWARF: 0x4FCE
    signed char crawl_lock; // Offset: 0xCBD, DWARF: 0x4FF6
    signed char tiptoe; // Offset: 0xCBE, DWARF: 0x5023
    signed char sprint; // Offset: 0xCBF, DWARF: 0x504C
    unsigned char somersault; // Offset: 0xCC0, DWARF: 0x5075
    unsigned char land; // Offset: 0xCC1, DWARF: 0x50A2
    signed char pad_type; // Offset: 0xCC2, DWARF: 0x50C9
    signed char jump_hack; // Offset: 0xCC3, DWARF: 0x50F4
    unsigned char jump_hold; // Offset: 0xCC4, DWARF: 0x5120
    unsigned char allow_jump; // Offset: 0xCC5, DWARF: 0x514C
    short jump_frames; // Offset: 0xCC6, DWARF: 0x5179
    short jump_frame; // Offset: 0xCC8, DWARF: 0x51A7
    short slam_wait; // Offset: 0xCCA, DWARF: 0x51D4
    short spin_frames; // Offset: 0xCCC, DWARF: 0x5200
    short spin_frame; // Offset: 0xCCE, DWARF: 0x522E
    short spin_wait; // Offset: 0xCD0, DWARF: 0x525B
    short slide; // Offset: 0xCD2, DWARF: 0x5287
    short crouch_pos; // Offset: 0xCD4, DWARF: 0x52AF
    unsigned short slam_frame; // Offset: 0xCD6, DWARF: 0x52DC
    short fire_action; // Offset: 0xCD8, DWARF: 0x5309
    unsigned char fire; // Offset: 0xCDA, DWARF: 0x5337
    unsigned char tap; // Offset: 0xCDB, DWARF: 0x535E
    signed char target; // Offset: 0xCDC, DWARF: 0x5384
    signed char target_wait; // Offset: 0xCDD, DWARF: 0x53AD
    signed char fire_lock; // Offset: 0xCDE, DWARF: 0x53DB
    signed char idle_sigh; // Offset: 0xCDF, DWARF: 0x5407
    unsigned char hit_type; // Offset: 0xCE0, DWARF: 0x5433
    unsigned char freeze; // Offset: 0xCE1, DWARF: 0x545E
    signed char anim_processed; // Offset: 0xCE2, DWARF: 0x5487
    char pad1; // Offset: 0xCE3, DWARF: 0x54B8
};


typedef struct nupad_s nupad_s, *Pnupad_s;

struct nupad_s {
    void * padhandle;
    struct _XINPUT_STATE xinputs;
    struct _XINPUT_STATE old_xinputs;
    struct _XINPUT_FEEDBACK feedback;
    s32 port;
    s32 slot;
    s32 stage;
    s32 id;
    s32 ldx;
    s32 ldy;
    s32 rdx;
    s32 rdy;
    float l_nx;
    float l_ny;
    float l_rx;
    float l_ry;
    float l_dx;
    float l_dy;
    float r_nx;
    float r_ny;
    float r_rx;
    float r_ry;
    float r_dx;
    float r_dy;
    u32 paddata;
    u32 old_paddata;
    u32 paddata_db;
    int bit_;
    u32 padvalue;
    u8 ok;
    u8 read_id;
    u8 buttons_hi;
    u8 buttons_lo;
    u8 r_alg_x;
    u8 r_alg_y;
    u8 l_alg_x;
    u8 l_alg_y;
    u8 l_algpad_r;
    u8 l_algpad_l;
    u8 l_algpad_u;
    u8 l_algpad_d;
    u8 r_algpad_u;
    u8 r_algpad_r;
    u8 r_algpad_d;
    u8 r_algpad_l;
    u8 l1_alg;
    u8 r1_alg;
    u8 l2_alg;
    u8 r2_alg;
};

#endif // !MAIN_H
