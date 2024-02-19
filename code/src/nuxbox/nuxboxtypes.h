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

#endif // !NUXBOXTYPES_H
