#include "../types.h"
#include "../nu.h"
#include "gamecode/main.h"
#include "nu3dx/nu3dxtypes.h"


union Lst
{
    char* s8; // Offset: 0x0
    struct nulnkhdr_s* lhdr; // Offset: 0x0
};


//GCN MATCH
struct nulsthdr_s * NuLstCreate(s32 elcnt,s32 elsize)
{
    union Lst prev;
    struct nulsthdr_s *list;
    union Lst curr;
    struct nulnkhdr_s* start;
    s32 n;

    list = (struct nulsthdr_s *)NuMemAlloc(elcnt * (elsize + 0x10) + 0x10);
    if (list != NULL) {
        list->head = NULL;
        // start = (struct nulnkhdr_s *)list + 1;
        list->free = list + 1;
        curr.lhdr = (struct nulnkhdr_s *)list + 1;
        list->elcnt = (short)elcnt;
        list->elsize = (short)elsize;
        start = (s8*)curr.lhdr + elsize + 0x10;
        for(n = 1; n < elcnt; n++)
        {
            (curr.lhdr)->succ = start;
            (curr.lhdr)->id = n - 1;
            (curr.lhdr)->owner = list;
            curr.lhdr = start;
            start = (s8*)start + (elsize + 0x10);

        }
        prev.s8 = (s8*)curr.lhdr + (elsize + 0x10);

        curr.lhdr->succ = NULL;
        curr.lhdr->id = n - 1;
        curr.lhdr->owner = list;
    }
    return list;
}

void NuLstDestroy(struct nulsthdr_s *hdr) {
  NuMemFree(hdr);
  return;
}

//PS2
struct nulnkhdr_s * NuLstAlloc(struct nulsthdr_s *hdr)
{
  struct nulnkhdr_s *rv;

  rv = hdr->free;
  if (rv != NULL) {
    hdr->free = rv->succ;
    rv->succ = hdr->head;
    if (hdr->head != NULL) {
      hdr->head->prev = rv;
    }
    else {
      hdr->tail = rv;
    }
    rv->prev = NULL;
    hdr->head = rv;
    *(u32 *)&rv->id = *(u32 *)&rv->id | 0x10000;
    return rv + 1;
  }
  return NULL;
}


//PS2
void NuLstFree(struct nulnkhdr_s *lnk)
{
  struct nulsthdr_s *hdr;
    // struct nulnkhdr_s* prev = lnk - 1;
    lnk -= 1;

  hdr = lnk->owner;
  if (lnk->succ != NULL) {
    (lnk->succ)->prev = lnk->prev;
  }
  else {
    hdr->tail = lnk->prev;
  }
  if (lnk->prev != NULL) {
    (lnk->prev)->succ = lnk->succ;
  }
  else {
    hdr->head = lnk->succ;
  }
  lnk->succ = hdr->free;
  hdr->free = lnk;
  *(u32 *)&lnk->id = *(u32 *)&lnk->id & 0xfffeffff;
  return;
}

//PS2
struct nulnkhdr_s * NuLstGetNext(struct nulsthdr_s *hdr,struct nulnkhdr_s *lnk)
{
  struct nulnkhdr_s *rv;

  if (lnk != NULL) {
    lnk -= 1;
    rv = lnk->succ;
  }
  else {
    rv = hdr->head;
  }
  if (rv == NULL) {
    return NULL;
  }
  return rv + 1;
}


