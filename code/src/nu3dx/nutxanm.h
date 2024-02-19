#ifndef NUTEXANM_H
#define NUTEXANM_H

#include "../types.h"
#include "nu3dxtypes.h"
#include "nucore/nucoretypes.h"

/*
  800ba124 000064 800ba124  4 NuTexAnimProgSysInit 	Global
  800ba188 000054 800ba188  4 NuTexAnimProgFind 	Global
  800ba1dc 00005c 800ba1dc  4 NuTexAnimProgInit 	Global
  800ba238 00012c 800ba238  4 NuTexAnimProgAssembleEnd 	Global
  800ba364 0000ac 800ba364  4 NuTexAnimEnvCreate 	Global
  800ba410 0000f0 800ba410  4 EvalVars 	Global
  800ba500 0000e4 800ba500  4 ParGetCC 	Global
  800ba5e4 0000ec 800ba5e4  4 LabTabFind 	Global
  800ba6d0 0000ec 800ba6d0  4 XDefLabTabFind 	Global
  800ba7bc 00005c 800ba7bc  4 pftaTex 	Global
  800ba818 0000b0 800ba818  4 pftaTexAdj 	Global
  800ba8c8 00008c 800ba8c8  4 pftaWait 	Global
  800ba954 00008c 800ba954  4 pftaRate 	Global
  800ba9e0 000048 800ba9e0  4 pftaOn 	Global
  800baa28 000048 800baa28  4 pftaOff 	Global
  800baa70 00004c 800baa70  4 pftaLabel 	Global
  800baabc 000068 800baabc  4 pftaXDef 	Global
  800bab24 000070 800bab24  4 pftaGoto 	Global
  800bab94 000070 800bab94  4 pftaXRef 	Global
  800bac04 0000b8 800bac04  4 pftaBtex 	Global
  800bacbc 000070 800bacbc  4 pftaGosub 	Global
  800bad2c 000028 800bad2c  4 pftaRet 	Global
  800bad54 000098 800bad54  4 pftaRepeat 	Global
  800badec 000028 800badec  4 pftaRepend 	Global
  800bae14 00008c 800bae14  4 pftaUntiltex 	Global
  800baea0 000028 800baea0  4 pftaEnd 	Global
  800baec8 000044 800baec8  4 pftaScriptname 	Global
  800baf0c 000170 800baf0c  4 NuTexAnimProgReadScript 	Global
  800bb07c 0000bc 800bb07c  4 NuTexAnimXCall 	Global
  800bb138 000654 800bb138  4 NuTexAnimEnvProc 	Global
  800bb78c 000020 800bb78c  4 NuTexAnimSetSignals 	Global
  800bb7ac 000048 800bb7ac  4 NuTexAnimProcessList 	Global
  800bb7f4 00003c 800bb7f4  4 NuTexAnimAddList 	Global
  800bb830 000074 800bb830  4 NuTexAnimRemoveList 	Global
  800bb8a4 000044 800bb8a4  4 NuTexAnimProcess 	Global
*/


static u32 nta_sig_on;
static u32 nta_sig_off;
static u32 nta_sig_old;
s32 nta_labels[64];
char texanimbuff[32768];
u32 texanimbits;
static char xdeflabtab[21][256];
static s32 xdeflabtabcnt;
static s32 labtabcnt;


// Size: 0x10
struct texanimscripts_s
{
    char* path;
    int pad1;
    unsigned long long levbits; //UNK TYPE
};



// Size: 0x20
struct nutexanim_s
{
    struct nutexanim_s* succ;
    struct nutexanim_s* prev;
    short* tids;
    short numtids;
    short dynalloc : 1; // Offset: 0xE, Bit Offset: 0, Bit Size: 1
    struct numtl_s* mtl; // Offset: 0x10
    struct nutexanimenv_s* env;
    char* ntaname;
    char* scriptname;
};

// Size: 0xEC
struct nutexanimenv_s
{
    struct nutexanimprog_s* prog;
    int pc;
    int rep_count[16];
    int rep_start[16];
    int rep_ix;
    int ra[16];
    int ra_ix;
    int pause;
    int pause_r;
    int pause_cnt;
    struct numtl_s* mtl;
    short* tids;
    int tex_ix;
    int dynalloc : 1; // Offset: 0xE8, Bit Offset: 0, Bit Size: 1
};


// Size: 0x1BC
struct nutexanimprog_s
{
    struct nutexanimprog_s* succ;
    struct nutexanimprog_s* prev;
    char name[32];
    int on_sig[32];
    int off_sig[32];
    unsigned int on_mask;
    unsigned int off_mask;
    short xdef_ids[32];
    short xdef_addrs[32];
    int xdef_cnt;
    short eop;
    short dynalloc : 1; // Offset: 0x1B6, Bit Offset: 0, Bit Size: 1
    short code[1]; // Offset: 0x1B8
};


// Size: 0x20, nutexanimFile
struct nutexanimf_s
{
    struct nutexanim_s* succ;
    struct nutexanim_s* prev;
    int tids;
    short numtids; // Offset: 0xC
    short dynalloc : 1; // Offset: 0xE, DWARF: 0x1DFFB, Bit Offset: 0, Bit Size: 1
    int mtl; // Offset: 0x10
    struct nutexanimenv_s* env;
    int ntaname;
    int scriptname;
};


// Size: 0xC
struct nutexanimlist_s
{
    struct nutexanim_s* nta;
    struct nutexanimlist_s* succ;
    struct nutexanimlist_s* prev;
};

struct nutexanimprog_s* sys_progs;
struct nutexanimlist_s ntalsysbuff[64];
struct nutexanimlist_s* ntal_first;
struct nutexanimlist_s* ntal_free;
static struct nutexanimprog_s* parprog;
static struct texanimscripts_s texanmscripts[24];
static struct nufpcomjmp_s nutexanimcomtab[19];

#endif // !NUTEXANM_H
