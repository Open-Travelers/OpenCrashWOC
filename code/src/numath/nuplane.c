#include "nuplane.h"

u32 NuPlnLine(struct nuvec4_s* p1, struct nuvec_s* p2, struct nuvec_s* p3, struct nuvec_s* p4)
{
  struct nuvec_s tmp;
  f32 t1 = NuVecDot(p2,(struct nuvec_s *)p1);
  f32 t2 = (t1 + p1->w);
  t1 = NuVecDot(p3,(struct nuvec_s *)p1);
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
