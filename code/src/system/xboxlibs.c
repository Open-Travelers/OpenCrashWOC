#include "system/xboxlibs.h"

s32 GetLastError_NGC(void) {
  return 0;
}

//NGC MATCH
void* malloc_NGC(int __size) {
  void *heap;
  int totfree;

  if ((__size == 0x40) || (__size == 0x60)) {
    mah = mah + 1;
  }
  //heap = OSAllocFromHeap(__OSCurrHeap,__size);
  if (((void *)0x814756ff < heap) && ((u32)__size < 0x80)) {
    mah = mah + 1;
  }
  if (heap == NULL) {
   // totfree = OSCheckHeap(__OSCurrHeap);
    sprintf(DebugText,"Out of Memory allocating %d bytes, %d bytes remaining",__size,totfree);
    DisplayErrorAndLockup("C:/source/crashwoc/code/system/xboxlibs.c",0x1a8,DebugText);
  }
 // msize = OSReferentSize(heap);
  malloctotalram = malloctotalram + msize;
  return heap;
}
