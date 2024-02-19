#ifndef CREATURE_H
#define CREATURE_H

#include "../types.h"
#include "../nu.h"
#include "nu3dx/nu3dxtypes.h"
#include "nuxbox/nuxboxtypes.h"
#include "gamecode/font3d.h"

void* SpaceGameCutTab[2][26];
unsigned char CLetter[191];
float temp_time;
s32 temp_action;

struct MoveInfo {
    // total size: 0x48
    float IDLESPEED; // offset 0x0, size 0x4
    float TIPTOESPEED; // offset 0x4, size 0x4
    float WALKSPEED; // offset 0x8, size 0x4
    float RUNSPEED; // offset 0xC, size 0x4
    float SPRINTSPEED; // offset 0x10, size 0x4
    float SLIDESPEED; // offset 0x14, size 0x4
    float CRAWLSPEED; // offset 0x18, size 0x4
    float DANGLESPEED; // offset 0x1C, size 0x4
    float WADESPEED; // offset 0x20, size 0x4
    float JUMPHEIGHT; // offset 0x24, size 0x4
    float DANGLEGAP; // offset 0x28, size 0x4
    short JUMPFRAMES0; // offset 0x2C, size 0x2
    short JUMPFRAMES1; // offset 0x2E, size 0x2
    short JUMPFRAMES2; // offset 0x30, size 0x2
    short STARJUMPFRAMES; // offset 0x32, size 0x2
    short SOMERSAULTFRAMES; // offset 0x34, size 0x2
    short SPINFRAMES; // offset 0x36, size 0x2
    short SPINRESETFRAMES; // offset 0x38, size 0x2
    short SUPERSPINFRAMES; // offset 0x3A, size 0x2
    short SUPERSPINWAITFRAMES; // offset 0x3C, size 0x2
    short SLAMWAITFRAMES; // offset 0x3E, size 0x2
    short SLIDEFRAMES; // offset 0x40, size 0x2
    short CROUCHINGFRAMES; // offset 0x42, size 0x2
    short JUMPLANDFRAMES; // offset 0x44, size 0x2
    short spad; // offset 0x46, size 0x2
};

struct MoveInfo CrashMoveInfo;
struct MoveInfo CocoMoveInfo;
struct MoveInfo MineCartMoveInfo;

struct animlist {
    char * file;
    short action;
    u8 blend_in_frames;
    u8 blend_out_frames;
    f32 speed;
    u16 flags;
    char pad1;
    char pad2;
    unsigned long long levbits;
};

    struct animlist *CrashAnim_CREDITS[2];
    struct animlist *CrashAnim_MINETUB[5];
    struct animlist *CrashAnim_ATLASPHERE[11];
    struct animlist *CrashAnim_MOSQUITO[3];
    struct animlist *CrashAnim_GLIDER[5];
    struct animlist *CocoAnim_DROPSHIP[3];
    struct animlist *CrashAnim_GYRO[7];

struct space_s{
    s32 character;
    struct animlist animlist[5];
};

typedef struct {
    // total size: 0x34
    char * path; // offset 0x0, size 0x4
    char * file; // offset 0x4, size 0x4
    char * name; // offset 0x8, size 0x4
    struct animlist * anim; // offset 0xC, size 0x4
    float radius; // offset 0x10, size 0x4
    struct nuvec_s min; // offset 0x14, size 0xC
    struct nuvec_s max; // offset 0x20, size 0xC
    float scale; // offset 0x2C, size 0x4
    float shadow_scale; // offset 0x30, size 0x4
} CharacterData;

CharacterData CData[191];

struct RPos_s {
    char iRAIL;
    char vertical;
    short iALONG;
    short i1;
    short i2;
    float fALONG;
    float fACROSS;
    u16 angle;
    u16 cam_angle;
    u8 mode;
    char pad1;
    char pad2;
    char pad3;
    struct nuvec_s pos;
};

struct CharacterModel {
    struct NUHGOBJ_s * hobj;
    struct nuanimdata_s * anmdata[118];
    struct animlist * animlist[118];
    struct nuanimdata_s * fanmdata[118];
    struct animlist * fanimlist[118];
    int shadhdr;
    short character;
    char pad1;
    char pad2;
    struct NUPOINTOFINTEREST_s * pLOCATOR[16];
};


union type_s_name {
    u8 chrs;
    u16 all;
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
    u32 flags;
    u32 frame;
    u32 draw_frame;
    u32 vehicle_frame;
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
    u16 pad_angle;
    u16 attack;
    u16 vulnerable;
    short die_action;
    char old_ground;
    char finished;
    u16 xrot;
    u16 yrot;
    u16 zrot;
    u16 surface_xrot;
    u16 surface_zrot;
    u16 layer_xrot;
    u16 layer_zrot;
    u16 roof_xrot;
    u16 roof_zrot;
    short target_xrot;
    short target_yrot;
    short dyrot;
    union type_s_name gndflags;
    u16 hdg;
    u16 thdg;
    char ground;
    char surface_type;
    char layer_type;
    char roof_type;
    char invisible;
    u8 submerged;
    char transporting;
    char got_shadow;
    u8 boing;
    u8 contact_type;
    char die_model[2];
    u8 invincible;
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
    u8 touch;
};

struct AI_s {
    struct nuvec_s oldpos;
    struct nuvec_s newpos;
    struct nuvec_s wobblepos;
    f32 time;
    f32 duration;
    f32 spins;
    f32 speed;
    f32 radius;
    f32 distance;
    f32 height;
    f32 lateral;
    f32 scale;
    f32 wobble;
    u16 oldangle;
    u16 angle;
    u16 old_yrot;
    u16 movflags;
    u16 terflags;
    u16 rotflags;
    u16 rotspeed;
    u16 rotadjust;
    u16 wobble_ang;
    u16 wobble_speed;
    char direction;
    char i0;
    char i1;
    char idle_update;
    void * event_function[2];
    f32 event_distance[2];
    short event_branch[2];
    char event[2];
    char event_index[2];
    u8 event_flags[2];
    u8 event_occured[2];
    u16 attack_locator_bits;
    char pad1;
    char pad2;
    f32 attack_locator_radius;
    struct nuhspecial_s obj;
    char kill;
    char locator_kill;
    short force_action;
    short look_creature;
    u8 count;
    u8 hits;
};

struct rumble_s {
    u8 buzz;
    u8 power;
    u8 frame;
    u8 frames;
};

// Size: 0xCE4
struct creature_s {
    char used;
    char on;
    char off_wait;
    char i_aitab;
    struct obj_s obj;
    struct AI_s ai;
    struct NEWBUGGY * Buggy;
    struct creatcmd_s * cmdtable;
    struct creatcmd_s * cmdcurr;
    struct MoveInfo * OnFootMoveInfo;
    struct numtx_s m;
    struct numtx_s mtxLOCATOR[16][2];
    struct nuvec_s momLOCATOR[16][2];
    struct Nearest_Light_s lights;
    struct rumble_s rumble;
    f32 idle_time;
    f32 idle_wait;
    short idle_action;
    short old_idle_action;
    char idle_mode;
    char idle_repeat;
    char jump;
    char jump_type;
    char jump_subtype;
    char ok_slam;
    char slam;
    char spin;
    char crawl;
    char crawl_lock;
    char tiptoe;
    char sprint;
    u8 somersault;
    u8 land;
    char pad_type;
    char jump_hack;
    u8 jump_hold;
    u8 allow_jump;
    short jump_frames;
    short jump_frame;
    short slam_wait;
    short spin_frames;
    short spin_frame;
    short spin_wait;
    short slide;
    short crouch_pos;
    u16 slam_frame;
    short fire_action;
    u8 fire;
    u8 tap;
    char target;
    char target_wait;
    char fire_lock;
    char idle_sigh;
    u8 hit_type;
    u8 freeze;
    char anim_processed;
    char pad1;
};


struct creature_s* player;
struct creature_s Character[9];
struct CharacterModel CModel[49];
signed char CRemap[191];

#endif // !CREATURE_H
