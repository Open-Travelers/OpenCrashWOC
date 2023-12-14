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

struct time {
    char name[4];
    u32 itime;
};

struct level_s {
    u16 flags;
    char pad1;
    char pad2;
    struct time time[3];
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

// Size: 0x14
struct GTimer
{
    unsigned int frame; // Offset: 0x0, DWARF: 0x273E8F
    unsigned int itime; // Offset: 0x4, DWARF: 0x273EB7
    unsigned int isec; // Offset: 0x8, DWARF: 0x273EDF
    float ftime; // Offset: 0xC, DWARF: 0x273F06
    float fsec; // Offset: 0x10, DWARF: 0x273F2E
};

struct GTimer PauseTimer;

#endif // !GAME_H
