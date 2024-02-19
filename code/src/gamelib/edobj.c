#include "edobj.h"
#include "../nu.h"

s32 edobjRenderCutoffTest(struct nuvec_s *pos) {
  float x;
  float y;
  float z;
  
  x = pos->x - global_camera.mtx._30;
  y = pos->y - global_camera.mtx._31;
  z = pos->z - global_camera.mtx._32;
  if (NuFsqrt(x * x + y * y + z * z) > 80.0f) {
      return 0;
  }
  return 1;
}