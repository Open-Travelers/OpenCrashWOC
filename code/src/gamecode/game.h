#ifndef GAME_H
#define GAME_H

#include "../types.h"
#include "../nu.h"
#include "gamecode/main.h"

s32 Level;
s32 PLAYERCOUNT;
struct ldata_s* LDATA;
char tbuf[128];
char LevelFileName[32];
s32 pause_dir;
s32 LIGHTMASK;
unsigned int pausestats_frame;

struct remember {
    char x;
    char y;
};

struct cursor_s {
    struct remember remember[48];
    u32 menu_frame;
    u32 item_frame;
    char x;
    char y;
    char x_min;
    char y_min;
    char x_max;
    char y_max;
    char menu;
    char new_menu;
    char new_level;
    u8 wait;
    u8 wait_frames;
    char wait_hack;
    u8 button_lock;
    char pad1;
    char pad2;
    char pad3;
};

struct cursor_s Cursor;


struct hub_s {
    u8 flags;
    u8 crystals;
    char pad1;
    char pad2;
};

// Size: 0x8
struct time_s {
    char name[4]; // offset 0x0
    u32 itime; // offset 0x4
};

struct level_s {
    u16 flags;
    char pad1;
    char pad2;
    struct time_s time[3];
};

// Size: 0x414
struct game_s {
    char name[9];
    u8 vibration;
    u8 surround;
    u8 sfx_volume;
    u8 music_volume;
    char screen_x;
    char screen_y;
    u8 language;
    struct hub_s hub[6];
    struct level_s level[35];
    u8 lives;
    u8 wumpas;
    u8 mask;
    u8 percent;
    u8 crystals;
    u8 relics;
    u8 crate_gems;
    u8 bonus_gems;
    u8 gems;
    u8 gembits;
    u8 powerbits;
    u8 empty;
    u32 cutbits;
    u8 year;
    u8 month;
    u8 day;
    u8 hours;
    u8 mins;
    u8 pad_[3];
};

struct game_s Game;

// Size: 0x58
struct ldata_s {
    char * filepath;
    u8 * clist; /* modelList */
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

struct ldata_s* LDATA;

// Size: 0x14
struct GTimer
{
    unsigned int frame; // Offset: 0x0, DWARF: 0x273E8F
    unsigned int itime; // Offset: 0x4, DWARF: 0x273EB7
    unsigned int isec; // Offset: 0x8, DWARF: 0x273EDF
    float ftime; // Offset: 0xC, DWARF: 0x273F06
    float fsec; // Offset: 0x10, DWARF: 0x273F2E
};

// Size: 0xC
struct hdata_s
{
    signed char level[6]; // Offset: 0x0
    signed char i_spl[2]; // Offset: 0x6
    unsigned char barrier; // Offset: 0x8
    unsigned char i_gdeb; // Offset: 0x9
    short sfx; // Offset: 0xA
};

// Size: 0x30
struct award_s
{
    float time; // Offset: 0x0
    unsigned short yrot; // Offset: 0x4
    unsigned short got; // Offset: 0x6
    signed char level; // Offset: 0x8
    signed char wait; // Offset: 0x9
    char pad1; // Offset: 0xA
    char pad2; // Offset: 0xB
    struct nuvec_s oldpos0; // Offset: 0xC
    struct nuvec_s oldpos1; // Offset: 0x18
    struct nuvec_s newpos; // Offset: 0x24
};

// Size: 0x18
struct spltab_s
{
    struct nugspline_s* spl; // Offset: 0x0
    char* name; // Offset: 0x4
    short min; // Offset: 0x8
    short max; // Offset: 0xA
    unsigned long long levbits; // Offset: 0x10
};

// Size: 0x90
struct mfeathers_s
{
    struct numtx_s mM; // Offset: 0x0, DWARF: 0x27115A
    struct numtx_s mS; // Offset: 0x40, DWARF: 0x271181
    float time; // Offset: 0x80, DWARF: 0x2711A8
    float duration; // Offset: 0x84, DWARF: 0x2711CF
    float shadow; // Offset: 0x88, DWARF: 0x2711FA
    char pad1; // Offset: 0x8C, DWARF: 0x271223
    char pad2; // Offset: 0x8D, DWARF: 0x27124A
    char pad3; // Offset: 0x8E, DWARF: 0x271271
    char pad4; // Offset: 0x8F, DWARF: 0x271298
};

// Size: 0x4C
struct wumpa_s
{
    struct nuvec_s pos0; // Offset: 0x0, DWARF: 0x26E631
    struct nuvec_s pos1; // Offset: 0xC, DWARF: 0x26E65A
    struct nuvec_s pos; // Offset: 0x18, DWARF: 0x26E683
    struct nuvec_s mom; // Offset: 0x24, DWARF: 0x26E6AB
    float shadow; // Offset: 0x30, DWARF: 0x26E6D3
    unsigned short surface_xrot; // Offset: 0x34, DWARF: 0x26E6FC
    unsigned short surface_zrot; // Offset: 0x36, DWARF: 0x26E72B
    float time; // Offset: 0x38, DWARF: 0x26E75A
    float duration; // Offset: 0x3C, DWARF: 0x26E781
    signed char active; // Offset: 0x40, DWARF: 0x26E7AC
    signed char iRAIL; // Offset: 0x41, DWARF: 0x26E7D5
    short iALONG; // Offset: 0x42, DWARF: 0x26E7FD
    float fALONG; // Offset: 0x44, DWARF: 0x26E826
    signed char destroy; // Offset: 0x48, DWARF: 0x26E84F
    signed char fired; // Offset: 0x49, DWARF: 0x26E879
    signed char in_range; // Offset: 0x4A, DWARF: 0x26E8A1
    signed char surface_type; // Offset: 0x4B, DWARF: 0x26E8CC
};

// Size: 0x50
struct winfo_s
{
    struct numtx_s m; // Offset: 0x0, DWARF: 0x26F9E7
    float scale; // Offset: 0x40, DWARF: 0x26FA0D
    float dy; // Offset: 0x44, DWARF: 0x26FA35
    unsigned short angle; // Offset: 0x48, DWARF: 0x26FA5A
    unsigned short spin; // Offset: 0x4A, DWARF: 0x26FA82
    char pad1; // Offset: 0x4C, DWARF: 0x26FAA9
    char pad2; // Offset: 0x4D, DWARF: 0x26FAD0
    char pad3; // Offset: 0x4E, DWARF: 0x26FAF7
    char pad4; // Offset: 0x4F, DWARF: 0x26FB1E
};

// Size: 0x24
struct newwumpa_s
{
    struct nuvec_s world_pos; // Offset: 0x0, DWARF: 0x27004A
    struct nuvec_s screen_pos; // Offset: 0xC, DWARF: 0x270078
    float screen_scale; // Offset: 0x18, DWARF: 0x2700A7
    unsigned char count; // Offset: 0x1C, DWARF: 0x2700D6
    unsigned char delay; // Offset: 0x1D, DWARF: 0x2700FE
    signed char transformed; // Offset: 0x1E, DWARF: 0x270126
    signed char bonus; // Offset: 0x1F, DWARF: 0x270154
    signed char active; // Offset: 0x20, DWARF: 0x27017C
    char pad1; // Offset: 0x21, DWARF: 0x2701A5
    char pad2; // Offset: 0x22, DWARF: 0x2701CC
    char pad3; // Offset: 0x23, DWARF: 0x2701F3
};

struct mfeathers_s MaskFeathers[4];
struct spltab_s SplTab[73];
struct award_s Award[3];
struct hdata_s HData[6];
struct newwumpa_s NewWumpa[32];
s32 i_newwumpa;
struct GTimer PauseTimer;
struct GTimer GameTimer;
s32 probecol;
s32 probeon;
struct nuangvec_s proberot;
s32 probetime;
s32 probey;
struct anim_s TempAnim;
struct anim_s TempAnim2;
struct Nearest_Light_s TempLights;
struct Nearest_Light_s TempLights2;
s32 temp_character;
s32 temp_character_action;
s32 temp_character2;
s32 temp_character2_action;
s32 temp_hub;
s32 temp_hublevel;
s32 tumble_character;
f32 tumble_item_addtime;
s32 i_award;
s32 qseed;
char akuaku_advice_played[13];
s32 temp_minutes;
s32 temp_hours;
s32 temp_tenths;
s32 temp_hundredths;
s32 temp_seconds; 

#endif // !GAME_H
