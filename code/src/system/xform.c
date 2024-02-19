#include "system/gs.h"

void GS_InitXForm(void) {
  if (TVertices == NULL) {
    TVertices = (struct _GS_VERTEXTL *)malloc_x(0x2c300);
  }
  CV_SKINMTX = (struct _GSMATRIX *)malloc_x(0x400);
  return;
}