#ifndef FONT3D_H
#define FONT3D_H

#include "../types.h"
#include "../nu.h"
#include "gamecode/main.h"


s32 font3d_initialised;


// Size: 0x8
typedef struct nuhspecial_s {
    struct nugscn_s* scene; // Offset: 0x0, DWARF: 0x840B
    struct nuspecial_s* special; // Offset: 0x4, DWARF: 0x8438
};


typedef struct {
    // total size: 0x10
    char ascii; // offset 0x0, size 0x1
    char pad1; // offset 0x1, size 0x1
    char pad2; // offset 0x2, size 0x1
    char pad3; // offset 0x3, size 0x1
    struct nuhspecial_s obj; // offset 0x4, size 0x8
    char * name; // offset 0xC, size 0x4
} FONT3DCHARACTER;
typedef struct {
    // total size: 0xC
    struct nuhspecial_s obj; // offset 0x0, size 0x8
    char * name; // offset 0x8, size 0x4
} FONT3DJCHARACTER;
typedef struct {
    // total size: 0xC
    short i; // offset 0x0, size 0x2
    unsigned char flags; // offset 0x2, size 0x1
    signed char action; // offset 0x3, size 0x1
    float anim_time; // offset 0x4, size 0x4
    float scale; // offset 0x8, size 0x4
} FONT3DOBJECT;

volatile FONT3DOBJECT Font3DObjTab[26];
volatile FONT3DCHARACTER Font3DTab[62];

#endif // !FONT3D_H
