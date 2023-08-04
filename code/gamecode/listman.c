//PS2
struct nulsthdr_s * NuLstCreate(s32 elcnt,s32 elsize)
{
    union Lst prev;
    struct nulsthdr_s *list;
    union Lst curr;
    struct nulnkhdr_s* start;
    s32 n;
    
    list = (struct nulsthdr_s *)NuMemAllocFn(elcnt * (elsize + 0x10) + 0x10, ".\\listman.c", 0x24);
    if (list != NULL) {
        start = (struct nulnkhdr_s *)list + 1;
        list->free = start;
        list->head = NULL;
        list->elcnt = (short)elcnt;
        list->elsize = (short)elsize;
        curr.lhdr = start;
        start = (s8*)curr.lhdr + elsize + 0x10;
        
        for(n = 1;  n < elsize; n++)
        {
            (curr.lhdr)->succ = start;
            (curr.lhdr)->id = n - 1;
            (curr.lhdr)->owner = list;
            curr.lhdr = start;
            start = (s8*)curr.lhdr + (elsize + 0x10);
        
        }
        prev.s8 = (s8*)curr.lhdr + (elsize + 0x10);
        start->id = n - 1;
        start->succ = NULL;
        start->owner = list;
    }
    return list;
}

void NuLstDestroy(nulsthdr_s *hdr)

{
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
    *(uint *)&rv->id = *(uint *)&rv->id | 0x10000;
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


