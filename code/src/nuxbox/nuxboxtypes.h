#ifndef NUXBOXTYPES_H
#define NUXBOXTYPES_H

#include"types.h"
#include "../nu.h"
#include "nu3dx/nu3dxtypes.h"

const struct numtx_s numtx_identity;
s32 CausticTextures[32];

struct pdir_s {
    int Index;
    struct nuvec_s Direction;
    struct nucolour3_s Colour;
    float Distance;
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

struct anim_s {
    float anim_time;
    float blend_src_time;
    float blend_dst_time;
    short action;
    short oldaction;
    short newaction;
    short blend_src_action;
    short blend_dst_action;
    short blend_frame;
    short blend_frames;
    char blend;
    u8 flags;
};

struct mask_s {
    struct numtx_s mM;
    struct numtx_s mS;
    struct nuvec_s pos;
    struct nuvec_s newpos;
    struct Nearest_Light_s lights;
    struct anim_s anim;
    float scale; // Offset: 0x164, DWARF: 0x268BCC
    float shadow; // Offset: 0x168, DWARF: 0x268BF4
    short character; // Offset: 0x16C, DWARF: 0x268C1D
    short active; // Offset: 0x16E, DWARF: 0x268C49
    short sfx; // Offset: 0x170, DWARF: 0x268C72
    char pad1; // Offset: 0x172, DWARF: 0x268C98
    char pad2; // Offset: 0x173, DWARF: 0x268CBF
    unsigned short xrot; // Offset: 0x174, DWARF: 0x268CE6
    unsigned short yrot; // Offset: 0x176, DWARF: 0x268D0D
    unsigned short angle; // Offset: 0x178, DWARF: 0x268D34
    unsigned short surface_xrot; // Offset: 0x17A, DWARF: 0x268D5C
    unsigned short surface_zrot; // Offset: 0x17C, DWARF: 0x268D8B
    unsigned short wibble_ang[3]; // Offset: 0x17E, DWARF: 0x268DBA
    float idle_time; // Offset: 0x184, DWARF: 0x268DE9
    float idle_duration; // Offset: 0x188, DWARF: 0x268E15
    signed char idle_mode; // Offset: 0x18C, DWARF: 0x268E45
    signed char reflect; // Offset: 0x18D, DWARF: 0x268E71
    unsigned char offset_frame; // Offset: 0x18E, DWARF: 0x268E9B
    signed char hold; // Offset: 0x18F, DWARF: 0x268ECA
};

// Size: 0x12
struct _XINPUT_GAMEPAD
{
    unsigned short wButtons; // Offset: 0x0, DWARF: 0x11CFD
    unsigned char bAnalogButtons[8]; // Offset: 0x2, DWARF: 0x11D28
    short sThumbLX; // Offset: 0xA, DWARF: 0x11D5B
    short sThumbLY; // Offset: 0xC, DWARF: 0x11D86
    short sThumbRX; // Offset: 0xE, DWARF: 0x11DB1
    short sThumbRY; // Offset: 0x10, DWARF: 0x11DDC
};

// Size: 0x16, DWARF: 0x11F49
struct _XINPUT_STATE
{
    unsigned int dwPacketNumber; // Offset: 0x0, DWARF: 0x11F6B
    struct _XINPUT_GAMEPAD gamepad;
};

// Size: 0x44, DWARF: 0x11FE6
struct _XINPUT_FEEDBACK_HEADER
{
    unsigned int dwStatus; // Offset: 0x0, DWARF: 0x12012
    void* hEvent; // Offset: 0x4, DWARF: 0x1203D
    unsigned char Reserved[58]; // Offset: 0x8, DWARF: 0x12069
};

// Size: 0x4, DWARF: 0x11E75
struct _XINPUT_RUMBLE
{
    unsigned short wLeftMotorSpeed; // Offset: 0x0, DWARF: 0x11E98
    unsigned short wRightMotorSpeed; // Offset: 0x2, DWARF: 0x11ECA
};

// Size: 0x48, DWARF: 0x12114
struct _XINPUT_FEEDBACK
{
    struct _XINPUT_FEEDBACK_HEADER Header; // Offset: 0x0, DWARF: 0x12139
    struct _XINPUT_RUMBLE rumble;
};

// Size: 0xF0, DWARF: 0x1F8DD
struct nupad_s
{
    void* padhandle; // Offset: 0x0, DWARF: 0x1F8F9
    struct _XINPUT_STATE xinputs; // Offset: 0x4, DWARF: 0x1F928
    struct _XINPUT_STATE old_xinputs; // Offset: 0x1C, DWARF: 0x1F954
    struct _XINPUT_FEEDBACK feedback; // Offset: 0x34, DWARF: 0x1F984
    int port; // Offset: 0x7C, DWARF: 0x1F9B1
    int slot; // Offset: 0x80, DWARF: 0x1F9D8
    int stage; // Offset: 0x84, DWARF: 0x1F9FF
    int id; // Offset: 0x88, DWARF: 0x1FA27
    int ldx; // Offset: 0x8C, DWARF: 0x1FA4C
    int ldy; // Offset: 0x90, DWARF: 0x1FA72
    int rdx; // Offset: 0x94, DWARF: 0x1FA98
    int rdy; // Offset: 0x98, DWARF: 0x1FABE
    float l_nx; // Offset: 0x9C, DWARF: 0x1FAE4
    float l_ny; // Offset: 0xA0, DWARF: 0x1FB0B
    float l_rx; // Offset: 0xA4, DWARF: 0x1FB32
    float l_ry; // Offset: 0xA8, DWARF: 0x1FB59
    float l_dx; // Offset: 0xAC, DWARF: 0x1FB80
    float l_dy; // Offset: 0xB0, DWARF: 0x1FBA7
    float r_nx; // Offset: 0xB4, DWARF: 0x1FBCE
    float r_ny; // Offset: 0xB8, DWARF: 0x1FBF5
    float r_rx; // Offset: 0xBC, DWARF: 0x1FC1C
    float r_ry; // Offset: 0xC0, DWARF: 0x1FC43
    float r_dx; // Offset: 0xC4, DWARF: 0x1FC6A
    float r_dy; // Offset: 0xC8, DWARF: 0x1FC91
    unsigned int paddata; // Offset: 0xCC, DWARF: 0x1FCB8
    unsigned int oldpaddata; // Offset: 0xD0, DWARF: 0x1FCE2
    unsigned int paddata_db; // Offset: 0xD4, DWARF: 0x1FD0F
    int analogue_sticks : 1; // Offset: 0xD8, DWARF: 0x1FD3C, Bit Offset: 0, Bit Size: 1
    int analogue_buttons : 1; // Offset: 0xD8, DWARF: 0x1FD7E, Bit Offset: 1, Bit Size: 1
    int valid : 1; // Offset: 0xD8, DWARF: 0x1FDC1, Bit Offset: 2, Bit Size: 1
    int deadzone : 1; // Offset: 0xD8, DWARF: 0x1FDF9, Bit Offset: 3, Bit Size: 1
    unsigned int padvalue; // Offset: 0xDC, DWARF: 0x1FE34
    unsigned char ok; // Offset: 0xE0, DWARF: 0x1FE5F
    unsigned char read_id; // Offset: 0xE1, DWARF: 0x1FE84
    unsigned char buttons_hi; // Offset: 0xE2, DWARF: 0x1FEAE
    unsigned char buttons_lo; // Offset: 0xE3, DWARF: 0x1FEDB
    unsigned char r_alg_x; // Offset: 0xE4, DWARF: 0x1FF08
    unsigned char r_alg_y; // Offset: 0xE5, DWARF: 0x1FF32
    unsigned char l_alg_x; // Offset: 0xE6, DWARF: 0x1FF5C
    unsigned char l_alg_y; // Offset: 0xE7, DWARF: 0x1FF86
    unsigned char l_algpad_r; // Offset: 0xE8, DWARF: 0x1FFB0
    unsigned char l_algpad_l; // Offset: 0xE9, DWARF: 0x1FFDD
    unsigned char l_algpad_u; // Offset: 0xEA, DWARF: 0x2000A
    unsigned char l_algpad_d; // Offset: 0xEB, DWARF: 0x20037
    unsigned char r_algpad_u; // Offset: 0xEC, DWARF: 0x20064
    unsigned char r_algpad_r; // Offset: 0xED, DWARF: 0x20091
    unsigned char r_algpad_d; // Offset: 0xEE, DWARF: 0x200BE
    unsigned char r_algpad_l; // Offset: 0xEF, DWARF: 0x200EB
    unsigned char l1_alg; // Offset: 0xF0, DWARF: 0x20118
    unsigned char r1_alg; // Offset: 0xF1, DWARF: 0x20141
    unsigned char l2_alg; // Offset: 0xF2, DWARF: 0x2016A
    unsigned char r2_alg; // Offset: 0xF3, DWARF: 0x20193
};

#endif // !NUXBOXTYPES_H
