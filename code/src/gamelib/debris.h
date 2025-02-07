#ifndef DEBRIS_H
#define DEBRIS_H

#include "../types.h"
#include "../nu.h"

// Size: 0x28
struct PartList_s {
    struct nuvec_s vt[3]; // Offset: 0x0, DWARF: 0x617C1B
    s32 colour; // Offset: 0x24, DWARF: 0x617C42
};

// Size: 0xA48
struct PartHeader_testretail
{
    s32 DmaHeader[2]; // Offset: 0x0, DWARF: 0x61787D
    float grav; // Offset: 0x10, DWARF: 0x6178AB
    float gtime; // Offset: 0x14, DWARF: 0x6178D2
    float u0; // Offset: 0x28, DWARF: 0x617926
    float v0; // Offset: 0x2C, DWARF: 0x61794B
    float u1; // Offset: 0x30, DWARF: 0x617970
    float v1; // Offset: 0x34, DWARF: 0x617995
    float u2; // Offset: 0x38, DWARF: 0x6179BA
    float v2; // Offset: 0x3C, DWARF: 0x6179DF
    float u3; // Offset: 0x40, DWARF: 0x617A04
    float v3; // Offset: 0x44, DWARF: 0x617A29
    struct PartList_s Data[64]; // Offset: 0x48, DWARF: 0x617A4E
    s32 DmaBody[6]; // Offset: 0x18, DWARF: 0x6178FA
};

typedef struct { // 0x10
	/* 0x0 */ float t;
	/* 0x4 */ float r;
	/* 0x8 */ float g;
	/* 0xc */ float b;
} DEBRIS_COLOUR_SLOT;

typedef struct { // 0x8
	/* 0x0 */ float t;
	/* 0x4 */ float a;
} DEBRIS_ALPHA_SLOT;

typedef struct { // 0x8
	/* 0x0 */ float t;
	/* 0x4 */ float w;
} DEBRIS_WIDTH_SLOT;

typedef struct { // 0x8
	/* 0x0 */ float t;
	/* 0x4 */ float h;
} DEBRIS_HEIGHT_SLOT;

typedef struct { // 0x8
	/* 0x0 */ float t;
	/* 0x4 */ float r;
} DEBRIS_ROTATION_SLOT;

typedef struct { // 0x8
	/* 0x0 */ float t;
	/* 0x4 */ float x;
} DEBRIS_JIBBER_X_SLOT;

typedef struct { // 0x8
	/* 0x0 */ float t;
	/* 0x4 */ float y;
} DEBRIS_JIBBER_Y_SLOT;

typedef struct { // 0x8
	/* 0x0 */ float t;
	/* 0x4 */ float r;
} DEBRIS_COLLISION_SPHERE_SLOT;

typedef struct { // 0xc
	/* 0x0 */ int id;
	/* 0x4 */ int type;
	/* 0x8 */ int delay;
} DEBRIS_SOUND;

// 0x354
struct debinftype {
	/* 0x000 */ char id[16];
	/* 0x010 */ char listid;
	/* 0x011 */ char pageid; //unk
	/* 0x012 */ short generate;
	/* 0x014 */ short debnum;
	/* 0x016 */ short ival_offset;
	/* 0x018 */ short ival_on;
	/* 0x01a */ short ival_on_ran;
	/* 0x01c */ short ival_off;
	/* 0x01e */ short ival_off_ran;
	/* 0x020 */ char gensort;
	/* 0x021 */ char gencode;
	/* 0x022 */ char mat;
	/* 0x023 */ char efftab;
	/* 0x024 */ float radius;
	/* 0x028 */ float cuton;
	/* 0x02c */ float cutoff;
	/* 0x030 */ float drawcutoff;
	/* 0x034 */  float emitmag;
	/* 0x038 */ struct nuvec_s variable_emit;
	/* 0x044 */ struct nuvec_s variable_emit_ranscale;
	/* 0x050 */ struct nuvec_s variable_start;
	/* 0x05c */ struct nuvec_s variable_start_ranscale;
	/* 0x068 */ float rsx;
	/* 0x06c */ float rsy;
	/* 0x070 */ float rsz;
	/* 0x074 */ float osx;
	/* 0x078 */ float osy;
	/* 0x07c */ float osz;
	/* 0x080 */ float rvx;
	/* 0x084 */ float rvy;
	/* 0x088 */ float rvz;
	/* 0x08c */ float ovx;
	/* 0x090 */ float ovy;
	/* 0x094 */ float ovz;
	/* 0x098 */ float grav;
	/* 0x09c */ float etime;
	/* 0x0a0 */ short uw;
	/* 0x0a2 */ char frames;
	/* 0x0a3 */ char rate;
	/* 0x0a4 */ float uvanim;
	/* 0x0a8 */ float jibxfreq;
	/* 0x0ac */ float jibxamp;
	/* 0x0b0 */ float jibyfreq;
	/* 0x0b4 */ float jibyamp;
	/* 0x0b8 */ DEBRIS_COLOUR_SLOT colslot[8];
	/* 0x138 */ DEBRIS_ALPHA_SLOT alpslot[8];
	/* 0x178 */ float minsize;
	/* 0x17c */ float maxsize;
	/* 0x180 */ DEBRIS_WIDTH_SLOT wslot[8];
	/* 0x1c0 */ DEBRIS_HEIGHT_SLOT hslot[8];
	/* 0x200 */ float minrot;
	/* 0x204 */ float maxrot;
	/* 0x208 */ DEBRIS_ROTATION_SLOT rotslot[8];
	/* 0x248 */ DEBRIS_JIBBER_X_SLOT jibxslot[8];
	/* 0x288 */ DEBRIS_JIBBER_Y_SLOT jibyslot[8];
	/* 0x2c8 */ float u1;
	/* 0x2cc */ float v1;
	/* 0x2d0 */ float u2;
	/* 0x2d4 */ float v2;
	/* 0x2d8 */ struct PartHeader_testretail *DmaDebTypePointer;
	/* 0x2dc */ DEBRIS_COLLISION_SPHERE_SLOT sphereslot[8];
	/* 0x31c */ char numspheres;
	/* 0x31d */ char drawflag; //unk
	/* 0x31e */ unsigned char damageflags;    //unk
	/* 0x31f */ char repeatflag;    //unk
	/* 0x320 */ s32 variable_key;
	/* 0x324 */ DEBRIS_SOUND sounds[4];
};

// Size: 0x20
struct uv1deb {
        float x; // Offset: 0x0, DWARF: 0x615C85
        float y; // Offset: 0x4, DWARF: 0x615CA9
        float z; // Offset: 0x8, DWARF: 0x615CCD
        float time; // Offset: 0xC, DWARF: 0x615CF1
        float mx; // Offset: 0x10, DWARF: 0x615D18
        float my; // Offset: 0x14, DWARF: 0x615D3D
        float mz; // Offset: 0x18, DWARF: 0x615D62
        float etime; // Offset: 0x1C, DWARF: 0x615D87
};

// Size: 0x410, DWARF: 0x61704B
struct rdata_s
{
    s32 dmadata[2]; // Offset: 0x0, DWARF: 0x61705D
    // s32 unpackdata[2]; // Offset: 0x8, DWARF: 0x617089
/* 0x010 */ //s32 giftaginfo[4];
    struct uv1deb debris[32]; // Offset: 0x10, DWARF: 0x6170B8
};

// Size: 0x10, DWARF: 0x3719E
struct debrissphere_s
{
    struct nuvec_s emit; // Offset: 0x0, DWARF: 0x371C1
    float t; // Offset: 0xC, DWARF: 0x371EA
};

// Size: 0x56c, DWARF: 0x37233
struct debkeydatatype_s
{
    // Size: 0x410, DWARF: 0x36421
    struct rdata_s* chunks[256]; // Offset: 0x0, DWARF: 0x37258
    struct numtx_s rotmtx; // Offset: 0x200, DWARF: 0x37283
    struct numtx_s emitrotmtx; // Offset: 0x240, DWARF: 0x372AE
    short count; // Offset: 0x280, DWARF: 0x372DD
    short type; // Offset: 0x282, DWARF: 0x37305
    short active; // Offset: 0x284, DWARF: 0x3732C
    short debcount; // Offset: 0x286, DWARF: 0x37355
    short disposed; // Offset: 0x288, DWARF: 0x37380
    short pointer; // Offset: 0x28A, DWARF: 0x373AB
    short reqcount; // Offset: 0x28C, DWARF: 0x373D5
    short reqdebcount; // Offset: 0x28E, DWARF: 0x37400
    float x; // Offset: 0x290, DWARF: 0x3742E
    float y; // Offset: 0x294, DWARF: 0x37452
    float z; // Offset: 0x298, DWARF: 0x37476
    float toffx; // Offset: 0x29C, DWARF: 0x3749A
    float toffy; // Offset: 0x2A0, DWARF: 0x374C2
    float toffz; // Offset: 0x2A4, DWARF: 0x374EA
    // Size: 0x20, DWARF: 0x1EBCB
    struct uv1deb*(*genptr)(); // Offset: 0x2A8, DWARF: 0x37512
    void* gencode; // Offset: 0x2AC, DWARF: 0x37540
    struct debkeydatatype_s* next; // Offset: 0x2B0, DWARF: 0x3756F
    struct debkeydatatype_s* prev; // Offset: 0x2B4, DWARF: 0x3759B
    short rotory; // Offset: 0x2B8, DWARF: 0x375C7
    short rotorz; // Offset: 0x2BA, DWARF: 0x375F0
    s32 instances; // Offset: 0x2BC, DWARF: 0x37619
    short oncount; // Offset: 0x2C0, DWARF: 0x37645
    short delay; // Offset: 0x2C2, DWARF: 0x3766F
    struct debrissphere_s spheres[8]; // Offset: 0x2C4, DWARF: 0x37697
    short sphere_next; // Offset: 0x344, DWARF: 0x376C3
    short sphere_next_emit; // Offset: 0x346, DWARF: 0x376F1
    s32 trigger_type; // Offset: 0x348, DWARF: 0x37724
    s32 trigger_id; // Offset: 0x34C, DWARF: 0x37753
    float trigger_var; // Offset: 0x350, DWARF: 0x37780
    short refrotz; // Offset: 0x354, DWARF: 0x377AE
    short refroty; // Offset: 0x356, DWARF: 0x377D8
    float refoff; // Offset: 0x358, DWARF: 0x37802
    float refbounce; // Offset: 0x35C, DWARF: 0x3782B
    short sound_next[4]; // Offset: 0x360, DWARF: 0x37857
    short group_id; // Offset: 0x368, DWARF: 0x37886
};

// Size: 0x58, DWARF: 0x6F9899
struct particlechunkrendertype_s
{
    struct rdata_s* chunk; // Offset: 0x0
    struct debinftype* debinfo; // Offset: 0x4
    struct debkeydatatype_s* debdata; // Offset: 0x8, DWARF: 0x6F9907
    struct numtx_s rotmtx; // Offset: 0xC, DWARF: 0x6F9936
    float x; // Offset: 0x4C, DWARF: 0x6F9961
    float y; // Offset: 0x50, DWARF: 0x6F9985
    float z; // Offset: 0x54, DWARF: 0x6F99A9
};

enum debris_chunk_control_e
{
    DEBRIS_CHUNK_CONTROL_DO_BOUNCEXZ = 6,
    DEBRIS_CHUNK_CONTROL_DO_BOUNCEY = 5,
    DEBRIS_CHUNK_CONTROL_CHANGE_COLOUR = 4,
    DEBRIS_CHUNK_CONTROL_RETURN_TO_STACK = 3,
    DEBRIS_CHUNK_CONTROL_SORT_DMA = 2,
    DEBRIS_CHUNK_CONTROL_FREE_AND_UNLINK = 1,
    DEBRIS_CHUNK_CONTROL_FREE = 0
};

// Size: 0x2C, DWARF: 0x619A49
struct debris_chunk_control_s
{
    struct rdata_s* chunk; // Offset: 0x0, DWARF: 0x619A74
    s32 delay; // Offset: 0x4, DWARF: 0x619AA1
    enum debris_chunk_control_e action; // Offset: 0x8, DWARF: 0x619AC9
    struct debkeydatatype_s* owner; // Offset: 0xC, DWARF: 0x619AF4
    struct debris_chunk_control_s* next; // Offset: 0x10, DWARF: 0x619B21
    s32 ivariable; // Offset: 0x14, DWARF: 0x619B4D
    float fvariable; // Offset: 0x18, DWARF: 0x619B79
    short type; // Offset: 0x1C, DWARF: 0x619BA5
    short refrotz; // Offset: 0x1E, DWARF: 0x619BCC
    short refroty; // Offset: 0x20, DWARF: 0x619BF6
    float refoff; // Offset: 0x24, DWARF: 0x619C20
    float refbounce; // Offset: 0x28, DWARF: 0x619C49
};

static struct nuvec_s lbl_80119E30 = { 1.0f, 0.0f, 0.0f };
char* debbuffer;
struct rdata_s* freedebchunks[256];
struct debkeydatatype_s debkeydata[256];
struct particlechunkrendertype_s ParticleChunkRenderStack[256];
s32 debris_emitter_stack_index;
struct debkeydatatype_s* debris_emitter_stack[32];
struct debinftype* debtab[128];
short freedebkeys[256];
s32 freeDmaDebType;
s32 debris_chunk_control_stack_index;
s32 freedebkeyptr;
s32 freechunkcontrolsptr;
s32 mydebbuffersize;
struct debris_chunk_control_s* freechunkcontrols[512];
struct debris_chunk_control_s debris_chunk_controls[512];
s32 freedebchkptr;
struct numtl_s* DebMat[8];
struct PartHeader* DmaDebTypes[128];
struct debris_chunk_control_s* debris_chunk_control_stack[32];
struct debris_chunk_control_s* debris_chunk_control_stack[32];
extern s32 debris_render_group;
static s32 render_debris_enabled;
s32 globalframes;
float globaltime;

#endif // !DEBRIS_H