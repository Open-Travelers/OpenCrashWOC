#ifndef CUT_H
#define CUT_H

#include "../types.h"
#include "../nu.h"
#include "gamecode/main.h"
#include "nu3dx/nu3dxtypes.h"
#include "nu3dx/nuanim.h"


// Size: 0x8
struct pcutanm_s
{
    short character; // Offset: 0x0, DWARF: 0x1ED581
    short action; // Offset: 0x2, DWARF: 0x1ED5AD
    short sfx; // Offset: 0x4, DWARF: 0x1ED5D6
    signed char i; // Offset: 0x6, DWARF: 0x1ED5FC
    char pad1; // Offset: 0x7, DWARF: 0x1ED620
};

struct anim_s CutAnim;
struct timer_s CutTimer;
s32 CutAudio[32];
static struct NUGCUTLOCFNDATA_s cutscene_locatorfns[2];
static struct instNUGCUTSCENE_s* CutInst[32];
static struct NUGCUTSCENE_s* CutScene[32];
struct anim_s CutVortexAnim;
struct nuvec_s  campos_SPACE;
unsigned short cutang_FRONTEND[2];
unsigned short cutang_SPACE[3]; 
struct nuvec_s cutpos_CRASH;
struct nuvec_s cutpos_FRONTEND[2];
struct nuvec_s cutpos_SPACE[3];
float SPACEMASKADJUSTY;
float HUBHOLOSCALE;
float FRONTENDCORTEXADJUSTY;
struct pcutanm_s* pCutAnim;
s32 gamecut_finished;
s32 gamecut_hold;
s32 gamecut_sfx;
s32 gamecut_hack;
s32 gamecut_newlevel;
s32 gamecut_start;
s32 gamecut;
s32 cutmovie_hold;
s32 cutmovie;
s32 cutworldix;
static float cutratefrig;
s32 logos_played;
void* SpaceGameCutTab[2][26];
struct nucolour3_s cutdircol[3];
struct nuvec_s cutambcol; 
struct nuvec_s cutdir[3];

#endif // !CUT_H
