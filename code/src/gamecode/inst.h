#ifndef INST_H
#define INST_H

#include "../types.h"
#include "../nu.h"
#include "gamecode/main.h"
#include "nu3dx/nu3dxtypes.h"
#include "nu3dx/nuanim.h"


// Size: 0x108
struct sceneinst_s
{
    struct nuscene_s* scene; // Offset: 0x0
    char name[256]; // Offset: 0x4
    int inst_cnt; // Offset: 0x104
};

// Size: 0x108, DWARF: 0x31B3F7
struct animdatainst_s
{
    struct nuanimdata_s* ad; // Offset: 0x0, DWARF: 0x31B41A
    char name[256]; // Offset: 0x4, DWARF: 0x31B444
    int inst_cnt; // Offset: 0x104, DWARF: 0x31B46D
};


#endif // !INST_H
