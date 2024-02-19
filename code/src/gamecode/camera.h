#ifndef CAMERA_H
#define CAMERA_H

#include "../types.h"
#include "../nu.h"
#include "gamecode/main.h"

struct nucamera_s* pNuCam;
struct RPos_s TempRPos;
int temp_iRAIL;
int temp_iALONG;
int nRAILS;

// Size: 0x11C
struct cammtx_s
{
    struct numtx_s m; // Offset: 0x0, DWARF: 0x7250A
    struct numtx_s m0; // Offset: 0x40, DWARF: 0x72530
    struct nuvec_s vX; // Offset: 0x80, DWARF: 0x72557
    struct nuvec_s vY; // Offset: 0x8C, DWARF: 0x7257E
    struct nuvec_s vZ; // Offset: 0x98, DWARF: 0x725A5
    struct nuvec_s pos; // Offset: 0xA4, DWARF: 0x725CC
    struct nuvec_s oldpos; // Offset: 0xB0, DWARF: 0x725F4
    struct nuvec_s newpos; // Offset: 0xBC, DWARF: 0x7261F
    struct nuvec_s pos2; // Offset: 0xC8, DWARF: 0x7264A
    struct nuvec_s seek; // Offset: 0xD4, DWARF: 0x72673
    float distance; // Offset: 0xE0, DWARF: 0x7269C
    float ahead; // Offset: 0xE4, DWARF: 0x726C7
    float judder; // Offset: 0xE8, DWARF: 0x726EF
    float blend_time; // Offset: 0xEC, DWARF: 0x72718
    float blend_duration; // Offset: 0xF0, DWARF: 0x72745
    int xrot; // Offset: 0xF4, DWARF: 0x72776
    int yrot; // Offset: 0xF8, DWARF: 0x7279D
    int zrot; // Offset: 0xFC, DWARF: 0x727C4
    unsigned short old_xrot; // Offset: 0x100, DWARF: 0x727EB
    unsigned short new_xrot; // Offset: 0x102, DWARF: 0x72816
    unsigned short old_yrot; // Offset: 0x104, DWARF: 0x72841
    unsigned short new_yrot; // Offset: 0x106, DWARF: 0x7286C
    unsigned short old_zrot; // Offset: 0x108, DWARF: 0x72897
    unsigned short new_zrot; // Offset: 0x10A, DWARF: 0x728C2
    unsigned short hdg_to_player; // Offset: 0x10C, DWARF: 0x728ED
    short route; // Offset: 0x10E, DWARF: 0x7291D
    short i0; // Offset: 0x110, DWARF: 0x72945
    short i1; // Offset: 0x112, DWARF: 0x7296A
    signed char mode; // Offset: 0x114, DWARF: 0x7298F
    signed char iRAIL; // Offset: 0x115, DWARF: 0x729B6
    short iALONG; // Offset: 0x116, DWARF: 0x729DE
    unsigned short angle; // Offset: 0x118, DWARF: 0x72A07
    signed char vertical; // Offset: 0x11A, DWARF: 0x72A2F
    char pad1; // Offset: 0x11B, DWARF: 0x72A5A
};

struct cammtx_s GameCam[1];

// Size: 0x28, DWARF: 0x72FC7
struct rail_s
{
    struct nugspline_s* pINPLAT; // Offset: 0x0, DWARF: 0x72FD9
    struct nugspline_s* pINCAM; // Offset: 0x4, DWARF: 0x73008
    struct nugspline_s* pLEFT; // Offset: 0x8, DWARF: 0x73036
    struct nugspline_s* pCAM; // Offset: 0xC, DWARF: 0x73063
    struct nugspline_s* pRIGHT; // Offset: 0x10, DWARF: 0x7308F
    struct nugspline_s* pOUTCAM; // Offset: 0x14, DWARF: 0x730BD
    struct nugspline_s* pOUTPLAT; // Offset: 0x18, DWARF: 0x730EC
    float in_distance; // Offset: 0x1C, DWARF: 0x7311C
    float out_distance; // Offset: 0x20, DWARF: 0x7314A
    short edges; // Offset: 0x24, DWARF: 0x73179
    signed char type; // Offset: 0x26, DWARF: 0x731A1
    signed char circuit; // Offset: 0x27, DWARF: 0x731C8
};

struct rail_s Rail[8];

#endif // !CAMERA_H
