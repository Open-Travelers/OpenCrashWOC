#include "../system.h"
#include "nu.h"

//PS2 MATCH
void LinkDmaParticalSets(int **table,int count) {
  u32 *ptr;
  s32 lp;
  
  ptr = (u32 *)table[count - 1];
  lp = count - 2;
  ptr[1] = 0;
  *ptr = (*ptr & 0x8fffffff) + 0x60000000;
  while (lp >= 0) {
      ptr = *(table + lp);
      *ptr = (*ptr & 0x8fffffff) + 0x20000000;
      ptr[1] = (ptr[1] & 0x80000000) + (*(u32*)((table + lp) + 1) & 0x7fffffff);
      lp--;
      ptr--;
  }
  return;
}