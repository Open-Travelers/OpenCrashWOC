
//NGC MATCH
void visiSetSplineKnot(struct visidata_s* vd, s32 spix, s32 knotix) {
    s32 n;
    s32 i;
    s32 ix;
    s32 len;
    unsigned long long mask; // r7

    if (vd == NULL) {
        return;
    }
    if ((spix == vd->curspline) && (knotix == vd->curknot)) {
        return;
    }
    if (spix < 0) {
        return;
    }
    if (spix >= vd->sc->numsplines) {
        return;
    }
    if (vd->vspline[spix] == NULL) {
        return;
    }
    
    len = vd->sc->splines[spix].len;
    
    knotix = (knotix < len - 1) ? knotix : len - 1;
    
    if (knotix < 0) {
        return;
    } else if (knotix >= len) {
        return;
    }
    vd->curspline = spix;
    vd->curknot = knotix;

    for (knotix; knotix >= 0; knotix--) {
        if (vd->vspline[spix][knotix] != NULL) {
            i = 0; // r31 set here and used in the most inner loop
            for (ix = 0; ix < vd->binfosize; ix++) {
                mask = vd->vspline[spix][knotix][ix];
                for (n = 0; n < 64; n++) {
                    vd->sc->instances[i++].flags.visitest = mask & 1;
                    mask >>= 1;
                    if (i >= vd->sc->numinstance)
                        break;
                }
                if (i >= vd->sc->numinstance) {
                    return;
                }
            }
            return;
        }
    }
}

//NGC MATCH
static s32 PtInsideFBound(struct nuvec_s *wpos,s32 spcnt,struct nuvec_s *pts) {
  struct nuvec_s* curr;
  struct nuvec_s* prev;
  struct nuvec_s ipnt;
  s32 icnt;
  s32 n;
  struct nuplane_s p;

  prev = &pts[spcnt - 1];
  curr = pts;
  p.a = 0.0f;
  p.b = 0.0f;
  p.c = 1.0f;
  p.d = -((wpos->x * p.a) + (wpos->y * p.b) + (wpos->z * p.c));
  for (icnt = 0, n = 0; icnt < spcnt; icnt++, curr++) {
      if ((NuPlnLine(&p,prev,curr,&ipnt) != 0) && (ipnt.x >= wpos->x)) {
        n++;
      }
      prev = curr;
 }
  return n & 1;
}

struct visidata_s* visiLoadData(char* fname, struct nugscn_s* sc, union variptr_u* buff) {

	//TODO

}
