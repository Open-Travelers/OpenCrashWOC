#include "nuplane.h"

u32 NuPlnLine(Vec4* p1, Vec* p2, Vec* p3, Vec* p4)
{
  Vec tmp;
  f32 t1 = NuVecDot(p2,(Vec *)p1);
  f32 t2 = (t1 + p1->w);
  t1 = NuVecDot(p3,(Vec *)p1);
  f32 t3 = (t1 + p1->w);
  t1 = NuFsign(t2);
  f32 t4 = NuFsign(t3);
  if (t1 != t4) {
    NuVecSub(&tmp, p3, p2);
    NuVecScale((-t2 / (t3 - t2)), p4, &tmp);
    NuVecAdd(p4, p4, p2);
  }
  return t1 != t4;
}