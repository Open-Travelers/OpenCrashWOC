#include "nutxanm.h"
#include "../system.h"

#define CONDITION_CODE_EQUAL 0
#define CONDITION_CODE_LESS_THAN 1
#define CONDITION_CODE_GREATER_THAN 2
#define CONDITION_CODE_LESS_THAN_OR_EQUAL 3
#define CONDITION_CODE_GREATER_THAN_OR_EQUAL 4
#define CONDITION_CODE_NOT_EQUAL 5

s32 nta_labels[64];
static char labtab[21][64];
static int labtabcnt;
static char xdeflabtab[21][256];
static int xdeflabtabcnt;
static struct nutexanimprog_s* parprog;


//PS2
void NuTexAnimProgSysInit(void)
{
  struct nutexanimlist_s **succ;
  struct nutexanimlist_s *nta;
  s32 n;

  sys_progs = NULL;
    for(n = 0; n < 63; n++) {
        ntalsysbuff[n].succ = &ntalsysbuff[n + 1].nta;
    }
  ntal_first = NULL;
  ntalsysbuff[63].succ = NULL; //ntalsysbuff->succ = NULL;
  ntal_free = ntalsysbuff;
  xdeflabtabcnt = 0;
  nta_sig_old = 0;
  nta_sig_off = 0;
  nta_sig_on = 0;
  return;
}

//PS2
struct nutexanimprog_s * NuTexAnimProgFind(char *name)
{
  struct nutexanimprog_s *rv;

  rv = sys_progs;
  while ((rv != NULL && (strcasecmp(name,rv->name) != 0))) {
    rv = rv->succ;
  }
  return rv;
}


//PS2
void NuTexAnimProgInit(struct nutexanimprog_s *rv)
{

if (rv != 0) {
        s32 i;
        for(i = 0; i < 32; i++) {
            rv->off_sig[i] = -1;
            rv->on_sig[i] = -1;
        }
      rv->eop = 0;	//*(uint *)&rv->eop = *(uint *)&rv->eop & 0xffff7fff;
      rv->on_mask = 0;
      rv->off_mask = 0;
      rv->name[0] = '\0';
      rv->dynalloc = 0;
      rv->xdef_cnt = 0;
    }
  return;
}


//PS2
void NuTexAnimProgAssembleEnd(struct nutexanimprog_s *p)
{
    s16 ix = 0;

    while (ix < p->eop) {
        switch(p->code[ix]) {
            case 1: ix += 4; break;
            case 2: ix += 3; break;
            case 5: ix += 3; break;
            case 9:
                p->code[ix + 3] = nta_labels[p->code[ix + 3]];
                ix += 4;
                break;
            case 7:
            case 8:
                p->code[ix + 1] = nta_labels[p->code[ix + 1]];
                ix += 2;
                break;
            case 10: ix += 1; break;
            case 11: ix += 3; break;
            case 12: ix += 1; break;
            case 13: ix += 3; break;
            case 14: ix += 1; break;
            case 0:
            case 15: ix += 2; break;
        }
    }
    return;
}

//PS2
struct nutexanimenv_s * NuTexAnimEnvCreate(union variptr_u *buff,struct numtl_s *mtl,s16 *tids, struct nutexanimprog_s *p)
{
  struct nutexanimenv_s *rv;

  if (buff != NULL) {
    rv = (struct nutexanimenv_s *)(((s32)buff->voidptr + 3) & 0xfffffffc);
    buff->voidptr = rv + 1;
  }
  else {
      rv= (struct nutexanimenv_s *)NuMemAlloc(0xEC); //rv = (struct nutexanimenv_s *)NuMemAllocFn(0xec,"..\\nu2.ps2\\nu3d\\nutexanm.c",0x17a);
  }
  if (rv != NULL) {
    rv->prog = p;
    rv->mtl = mtl;
    rv->tids = tids;
    rv->pc = 0;
    rv->rep_ix = 0;
    rv->ra_ix = 0;
    rv->pause = 0;
    rv->pause_r = 0;
    rv->pause_cnt = 0;
    rv->tex_ix = 0;
    if (buff != NULL) {
      rv->dynalloc = rv->dynalloc & 0xfffffffe;	//0x7fffffff
    }
    else {
     rv->dynalloc = rv->dynalloc | 1;	// 0x80000000
    }
  }
  return rv;
}


//PS2
struct nutexanimprog_s * NuTexAnimProgReadScript(union variptr_u *buff,char *fname)
{
  struct nutexanimprog_s *rv;
  struct nufpar_s *fp;
  union variptr_u *ptr;

  if (buff != NULL) {
    ptr = (union variptr_u *)(s32)((buff->intaddr + 3) & ~3);
  } else {
    ptr = NuMemAlloc(0x400); //NuMemAllocFn(0x400,"..\\nu2.ps2\\nu3d\\nutexanm.c",0x2f9);
  }
  memset(labtab,0,0x540);
  labtabcnt = 0;
  fp = NuFParCreate(fname);
    rv = NULL;
  if (fp != NULL) {
    NuFParPushCom(fp,nutexanimcomtab);
    rv = (struct nutexanimprog_s*)ptr;
    NuTexAnimProgInit(rv);
      parprog = rv;
    while (NuFParGetLine(fp)) {
        int n = NuFParGetWord(fp);
      if (n && !NuFParInterpretWord(fp) && fp->wbuff[0]) {
          if (fp->wbuff[n - 1] == ':') {
              fp->wbuff[n - 1] = '\0';
              nta_labels[LabTabFind(fp->wbuff)] = parprog->eop;
          }
      }
    }
    if (buff != NULL) {
      buff->voidptr = rv->code + rv->eop;
    }
    NuFParDestroy(fp);
    NuTexAnimProgAssembleEnd(rv);
    rv->succ = sys_progs;
    if (sys_progs != NULL) {
      sys_progs->prev = rv;
    }
    rv->prev = NULL;
    sys_progs = rv;
  }
  return rv;
}

//PS2
void NuTexAnimSetSignals(u32 sig)
{
    nta_sig_on =  (sig ^ nta_sig_old) & sig;
    nta_sig_off = (sig ^ nta_sig_old) & ~sig;
    nta_sig_old = sig;
    return;
}

//PS2
void NuTexAnimProcessList(struct nutexanim_s *nta)
{
  if (nta != NULL) {
    do {
      if (nta->env != NULL) {
        NuTexAnimEnvProc(nta->env);
      }
      nta = nta->succ;
    } while (nta != NULL);
  }
  return;
}

//PS2
void NuTexAnimAddList(struct nutexanim_s *nta)
{
  struct nutexanimlist_s *rv;

  //NuDisableVBlank();
  rv = ntal_free;
  if (ntal_free != NULL) {
      ntal_free = ntal_free->succ;
      rv->nta = nta;
      rv->succ = ntal_first;
      rv->prev = NULL;
      if (ntal_first != NULL) {
        ntal_first->prev = rv;
      }
      ntal_first = rv;
  }
  //NuEnableVBlank();
  return;
}

//PS2
void NuTexAnimRemoveList(struct nutexanim_s *nta)
{
  struct nutexanimlist_s *rv;

  //NuDisableVBlank();
  rv = ntal_first;
  if (ntal_first != NULL) {
          do {
                if (rv->nta == nta) {
                      if (rv->succ != NULL) {
                        rv->succ->prev = rv->prev;
                      }
                      if (rv->prev != NULL) {
                        rv->prev->succ = rv->succ;
                      }
                      else {
                        ntal_first = rv->succ;
                      }
                      rv->succ = ntal_free;
                      ntal_free = rv;
                    //NuEnableVBlank();
                    return;
                }
                rv = rv->succ;

          } while (rv != NULL);
   }

    //NuEnableVBlank();
  return;
}


//PS2
void NuTexAnimProcess(void)
{
  struct nutexanimlist_s *rv;

  rv = ntal_first;
  if (ntal_first != NULL) {
    do {
      NuTexAnimProcessList(rv->nta);
      rv = rv->succ;
    } while (rv != NULL);
  }
  return;
}


//PS2
inline static int EvalVars (int cc, int v0, int v1)
{
    switch(cc) {
        case CONDITION_CODE_EQUAL:
            if (v0 == v1) {
                return 1;
            }
            break;
        case CONDITION_CODE_LESS_THAN:
            if (v0 < v1) {
                return 1;
            }
            break;
        case CONDITION_CODE_GREATER_THAN:
            if (v0 > v1) {
                return 1;
            }
            break;
        case CONDITION_CODE_LESS_THAN_OR_EQUAL:
            if (v0 <= v1) {
                return 1;
            }
            break;
        case CONDITION_CODE_GREATER_THAN_OR_EQUAL:
            if (v0 >= v1) {
                return 1;
            }
            break;
        case CONDITION_CODE_NOT_EQUAL:
            if (v0 != v1) {
                return 1;
            }
            break;
        default:
            NuErrorProlog("..\\nu2.ps2\\nu3d\\nutexanm.c", 0x1bb)("unknown condition code %d", cc);
            break;
    }
    return 0;
}

//PS2
inline static void NuTexAnimXCall (s32 lid, struct nutexanimenv_s * ignore)
{
    struct nutexanimlist_s* rv;
    struct nutexanim_s* nta;
    struct nutexanimenv_s *e;
    struct nutexanimprog_s *p;
    s32 n;


    for (rv = ntal_first; rv != NULL; rv = rv->succ) {
        for (nta = rv->nta; nta != NULL; nta = nta->succ) {
            e = nta->env;
            if ((e == NULL) || (e == ignore)) {
                continue;
            }

            p = e->prog;
            if ((p == NULL) || (p->xdef_cnt == 0)) {
                continue;
            }

            for (n = 0; n < p->xdef_cnt; n++) {
                if (p->xdef_ids[n] == lid) {
                    e->pc = p->xdef_addrs[n];
                    e->pause_cnt = 0;
                    e->ra_ix = 0;
                    e->rep_ix = 0;
                    break;
                }
            }
        }
    }
}

//PS2
void NuTexAnimEnvProc(struct nutexanimenv_s *e)
{
/*
DWARF
    nutexanimprog_s* p; //
    int done; //
    int n;
*/
    struct nutexanimprog_s *p;
    s32 done;
    s32 n;
    s32 lVar14;
    s16 *cod;

    p = e->prog;
    done = 0;
    if (p == NULL) {
        return;
    }

    if ((nta_sig_off & p->off_mask) != 0) {
        for (n = 0; n < 0x20; n++) {
            if ((nta_sig_off & 1 << n & p->off_mask) != 0) {
                e->pc = p->off_sig[n];
                e->pause_cnt = 0;
                e->ra_ix = 0;
                e->rep_ix = 0;
                break;
            }
        }
    }

    if ((nta_sig_on & p->on_mask) != 0) {
        for (n = 0; n < 0x20; n++) {
            if ((nta_sig_on & 1 << n & p->on_mask) != 0) {
                e->pc = p->on_sig[n];
                e->pause_cnt = 0;
                e->ra_ix = 0;
                e->rep_ix = 0;
                break;
            }
        }
    }

    if (e->pause_cnt != 0) {
        e->pause_cnt--;
        return;
    }
    cod = p->code;
    while (!done) {
        switch(cod[e->pc]) {
            case 0:
                e->tex_ix = p->code[e->pc + 1];
                e->mtl->tid = e->tids[e->tex_ix];
                e->pc += 2;
                e->pause_cnt = e->pause;
                if (e->pause_r != 0) {
                    e->pause_cnt += (long)NuRand(0) % e->pause_r;
                }
                break;
            case 1:
                lVar14 = e->tex_ix + p->code[e->pc + 1];
                if (lVar14 < p->code[e->pc+2]) {
                    lVar14 = p->code[e->pc+2];
                }
                if (p->code[e->pc+3] < lVar14) {
                    lVar14 = p->code[e->pc+3];
                }
                e->tex_ix = lVar14;
                e->mtl->tid = e->tids[e->tex_ix];
                e->pc += 4;
                e->pause_cnt = e->pause;
                if (e->pause_r != 0) {
                    e->pause_cnt += (long)NuRand(0) % e->pause_r;
                }
                break;
            case 2:
                e->pause_cnt = p->code[e->pc + 1];
                if (p->code[e->pc + 2] != 0) {
                    e->pause_cnt += (long)NuRand(0) % p->code[e->pc + 2];
                }
                done = 1;
                e->pc += 3;
                break;
            case 5:
                e->pause = p->code[e->pc + 1];
                e->pause_r = p->code[e->pc + 2];
                e->pc += 3;
                break;
            case 7:
                e->pc = p->code[e->pc + 1];
                break;
            case 9:
                if (EvalVars(p->code[e->pc + 1], e->tex_ix, p->code[e->pc + 2])) {
                    e->pc = p->code[e->pc + 3];
                } else {
                    e->pc += 4;
                }
                break;
            case 8:
                if (e->ra_ix >= 0x10) {
                    NuErrorProlog("..\\nu2.ps2\\nu3d\\nutexanm.c", 0x3c5)("TexAnim Processor Alert: Call Stack Overflow at (%d)", e->pc);
                }
                e->ra[e->ra_ix++] = e->pc + 2;
                e->pc = p->code[e->pc + 1];
                break;
            case 10:
                if (e->ra_ix == 0) {
                    NuErrorProlog("..\\nu2.ps2\\nu3d\\nutexanm.c", 0x3cd)("TexAnim Processor Alert: Call Stack Underflow at (%d)", e->pc);
                }
                e->ra_ix--;
                e->pc = e->ra[e->ra_ix];
                break;
            case 0xb:
                if (e->rep_ix >= 0x10) {
                    NuErrorProlog("..\\nu2.ps2\\nu3d\\nutexanm.c", 0x3d5)("TexAnim Processor Alert: Too Many Nested Repeat Loops at (%d)", e->pc);
                }
                e->rep_count[e->rep_ix] = p->code[e->pc + 1];
                if (p->code[e->pc + 2] != 0) {
                    e->rep_count[e->rep_ix] += (long)NuRand(0) % p->code[e->pc + 2];
                }
                e->pc += 3;
                e->rep_start[e->rep_ix++] = e->pc;
                break;
            case 0xc:
                if (e->rep_ix == 0) {
                    NuErrorProlog("..\\nu2.ps2\\nu3d\\nutexanm.c", 0x3df)("TexAnim Processor Alert: REPEND without REPEAT at (%d)", e->pc);
                }

                if (e->rep_count[e->rep_ix - 1] == 0) {
                    e->rep_ix--;
                    e->pc++;
                    break;
                }
                e->pc = e->rep_start[e->rep_ix - 1];
                e->rep_count[e->rep_ix - 1]--;
                break;
            case 0xd:
                if (e->rep_ix == 0) {
                    NuErrorProlog("..\\nu2.ps2\\nu3d\\nutexanm.c", 0x3ed)("TexAnim Processor Alert: UNTILTEX without REPEAT at (%d)", e->pc);
                }

                if (EvalVars(p->code[e->pc + 1], e->tex_ix, p->code[e->pc + 2]) || (e->rep_count[e->rep_ix - 1] == 0)) {
                    e->pc += 3;
                    e->rep_ix--;
                } else{
                    e->pc = e->rep_start[e->rep_ix - 1];
                    e->rep_count[e->rep_ix - 1]--;
                }
                break;
            case 0xe:
                done = 1;
                break;
            case 0xf:
                NuTexAnimXCall(p->code[e->pc + 1], e);
                e->pc += 2;
                break;
        }
    }

    return;
}


//PS2
static s32 ParGetCC(struct nufpar_s *pf)
{

    NuFParGetWord(pf);
    switch (pf->wbuff[0]){
        case '=':
            return CONDITION_CODE_EQUAL;
        case '<':
            switch (pf->wbuff[1]){
                case '>': return CONDITION_CODE_NOT_EQUAL;
                case '=': return CONDITION_CODE_LESS_THAN_OR_EQUAL;
            }
            return CONDITION_CODE_LESS_THAN;
        case '>':
            switch (pf->wbuff[1]){
                case '=': return CONDITION_CODE_GREATER_THAN_OR_EQUAL;
            }
            return CONDITION_CODE_GREATER_THAN;
        case '!':
            return CONDITION_CODE_NOT_EQUAL;
        default:
            NuErrorProlog("C:/source/crashwoc/code/nu3dx/nutexanm.c", 0x1E9)
                ("unknown condition \'%s\' at line %d", pf->wbuff, pf->line_num);
            break;
    }
}

//PS2
s32 LabTabFind(char* buf) //static inline on ps2
{
    s32 i;

    if (strlen(buf) > 0x14) {
        buf[0x14] = '\0';
    }

    for(i = 0; i < labtabcnt; i++) {
        if (strcasecmp(&labtab[i],buf) == 0) {
            return i;
        }
    }

    if (labtabcnt >= 0x40) {
        NuErrorProlog("..\\nu2.ps2\\nu3d\\nutexanm.c", 0x206)("Tex Anim Assembler Fatal Error: too many labels");
    }

    strcpy(&labtab[labtabcnt++], buf);
    return labtabcnt - 1;
}

//PS2
static s32 XDefLabTabFind(char* buf)
{
   s32 i;
    if (strlen(buf) > 0x14) {
        buf[0x14] = '\0';
      }
    for(i = 0; i < xdeflabtabcnt; i++) {
      if (strcasecmp(&xdeflabtab[i],buf) == 0) {
          return i;
      }
    }
  if (0xff < xdeflabtabcnt) {
    NuErrorProlog("C:/source/crashwoc/code/nu3dx/nutexanm.c",0x21b)("Tex Anim Assembler Fatal Error: too many global labels");
  }
  strcpy(&xdeflabtab[xdeflabtabcnt++],buf); //NuStrCpy
  return xdeflabtabcnt + -1;
}


//PS2
void pftaTex(struct nufpar_s *fp)
{
  s16 tid;

  tid = NuFParGetInt(fp);
  parprog->code[parprog->eop++] = 0;
  parprog->code[parprog->eop++] = tid;
  return;
}

//PS2
void pftaTexAdj(struct nufpar_s *fpar)
{
  s32 tid;
  s32 mi;
  s32 ma;

  tid = NuFParGetInt(fpar);
  mi = NuFParGetInt(fpar);
  ma = NuFParGetInt(fpar);
  parprog->code[parprog->eop++] = 1;
  parprog->code[parprog->eop++] = (short)tid;
  parprog->code[ parprog->eop++] = (short)mi;
  parprog->code[ parprog->eop++] = (short)ma;
  return;
}

//PS2
void pftaWait(struct nufpar_s *fpar)
{
  s32 fcnt;
  s32 rcnt;

  fcnt = NuFParGetInt(fpar);
  rcnt = NuFParGetInt(fpar);
  parprog->code[parprog->eop++] = 2;
  parprog->code[parprog->eop++] = (short)fcnt;
  parprog->code[parprog->eop++] = (short)rcnt;

  return;
}

//PS2
void pftaRate(struct nufpar_s *fpar)
{
  s32 fcnt;
  s32 rcnt;

  fcnt = NuFParGetInt(fpar);
  rcnt = NuFParGetInt(fpar);
  parprog->code[parprog->eop++] = 5;
  parprog->code[parprog->eop++] = (short)fcnt;
  parprog->code[parprog->eop++] = (short)rcnt;
  return;
}

//PS2
static void pftaOn(struct nufpar_s *fp)
{
  s32 sig;

  sig = NuFParGetInt(fp);
  parprog->on_sig[sig] = (int)parprog->eop;
  parprog->on_mask = parprog->on_mask | 1 << sig;
  return;
}

//PS2
static void pftaOff(struct nufpar_s *fp)
{
  s32 sig;

  sig = NuFParGetInt(fp);
  parprog->off_sig[sig] = (int)parprog->eop;
  parprog->off_mask = parprog->off_mask | 1 << sig;
  return;
}

//PS2
static void pftaLabel(struct nufpar_s *fp) {
    NuFParGetWord(fp);
    nta_labels[LabTabFind(fp->wbuff)] = parprog->eop;
}

//PS2
static void pftaXDef(struct nufpar_s *fpar)
{
  s32 lab;

  NuFParGetWord(fpar);
  lab = XDefLabTabFind(fpar->wbuff);
  parprog->xdef_ids[parprog->xdef_cnt] = (short)lab;
  parprog->xdef_addrs[parprog->xdef_cnt] = parprog->eop;
  parprog->xdef_cnt = parprog->xdef_cnt + 1;
  return;
}

//PS2
static void pftaGoto(struct nufpar_s *fpar)
{
  s32 lab;

  NuFParGetWord(fpar);
  lab = LabTabFind(fpar->wbuff);
  parprog->code[parprog->eop++] = 7;
  parprog->code[parprog->eop++] = (short)lab;
  return;
}

//PS2
void pftaXRef(struct nufpar_s *fpar)
{
  s32 lab;

  NuFParGetWord(fpar);
  lab = XDefLabTabFind(fpar->wbuff);
  parprog->code[parprog->eop++] = 0xf;
  parprog->code[parprog->eop++] = (short)lab;
  return;
}

//PS2
static void pftaBtex(struct nufpar_s *fpar)
{
  s32 cc;
  s32 tid;
  s32 lab;

  cc = ParGetCC(fpar);
  tid = NuFParGetInt(fpar);
  NuFParGetWord(fpar);
  lab = LabTabFind(fpar->wbuff);
  parprog->code[parprog->eop++] = 9;
  parprog->code[parprog->eop++] = cc;
  parprog->code[parprog->eop++] = tid;
  parprog->code[parprog->eop++] = lab;
  return;
}

//PS2
void pftaGosub(struct nufpar_s *fpar)
{
  s32 lab;

  NuFParGetWord(fpar);
  lab = LabTabFind(fpar->wbuff);
  parprog->code[parprog->eop++] = 8;
  parprog->code[parprog->eop++] = (short)lab;
  return;
}

//PS2
void pftaRet(struct nufpar_s *fp)
{
  parprog->code[parprog->eop++] = 10;
  return;
}

//PS2
void pftaRepeat(struct nufpar_s *fpar)
{
  s32 randval;
  s32 cnt;

  randval = NuFParGetInt(fpar);
  if (randval == 0) {
    randval = 0xffff; //0x7fffffff
  }
  cnt = NuFParGetInt(fpar);
  parprog->code[parprog->eop++] = 0xb;
  parprog->code[parprog->eop++] = (short)randval;
  parprog->code[parprog->eop++] = (short)cnt;
  return;
}

//PS2
void pftaRepend(struct nufpar_s *fp)
{
  parprog->code[parprog->eop++] = 0xc;
  return;
}

//PS2
static void pftaUntiltex(struct nufpar_s *fpar)
{
  s32 cc;
  s32 tid;

  cc = ParGetCC(fpar);
  tid = NuFParGetInt(fpar);
  parprog->code[parprog->eop++] = 0xd;
  parprog->code[parprog->eop++] = cc;
  parprog->code[parprog->eop++] = tid;
  return;
}

//PS2
static void pftaEnd(struct nufpar_s *fp)
{
  parprog->code[parprog->eop++] = 0xe;
  return;
}

//PS2
static void pftaScriptname(struct nufpar_s *fpar)
{
  NuFParGetWord(fpar);
  fpar->wbuff[0x20] = '\0';
  strcpy(parprog->name,fpar->wbuff);
  return;
}
