//NGC MATCH
static s32 PtInsideSpline(struct nuvec_s *wpos,struct nugspline_s *sp) {
  struct nuvec_s *curr;
  struct nuvec_s *prev;
  struct nuvec_s *pts;
  struct nuvec_s ipnt;
  s32 n;
  s32 icnt;
  s32 len;
  struct nuplane_s p;

  pts = (struct nuvec_s *)sp->pts;
  prev = &pts[sp->len - 1];
  curr = pts;
  
  p.a = 0.0f;
  p.b = 0.0f;
  p.c = 1.0f;
  p.d = -((wpos->x * 0.0f) + (wpos->y * 0.0f) + wpos->z);
  for (icnt = 0, n = 0; icnt < sp->len; icnt++, curr++) {
      if ((NuPlnLine(&p,prev,curr,&ipnt) != 0) && (ipnt.x >= wpos->x)) {
        n++;
      }
      prev = curr;
  }
  return n & 1;
}

//NGC MATCH
void ApplyVisiTable(struct nugscn_s *sc,struct nuvec_s *pos) {
  s32 n;
  s32 m;
  struct visidat_s *vd;
  
  if (vscnt != 0) {
    if (pos == NULL) {
        for (m = 0, n = 0; m < sc->numinstance; m++, n++) {
          sc->instances[n].flags.visitest = 0x20000000 | 1;
        }
    }
    else {
        for (m = 0, n = 0; m < sc->numinstance; m++, n++) {
              sc->instances[n].flags.visitest = 0xdfffffff * 2;
        }
        for (m = 0; m < vscnt; m++) {
          vd = visidat[m];
          if ((PtInsideSpline(pos,vd->sp) != 0)) {
            for (n = 0; n < vd->numinstances; n++) {
              vd->i[n]->flags.visitest = 0x20000000 | 1;
            }
          }
        }
    }
  }
  return;
}

//NGC MATCH
static s32 VSAddObjs(struct nuinstance_s** op, struct nugscn_s* gsc, struct nugspline_s* sp) {
    s32 n;
    s32 m;
    s32 gcnt;
    s32 icnt;
    struct nugobj_s* gobj;
    struct nuvec_s wpos;

    for (n = 0, icnt = 0; n < gsc->numinstance; n++) {
        gobj = gsc->gobjs[gsc->instances[n].objid];
        gcnt = gobj->ngobjs;
        for (m = 0; m < gcnt; m++) {
            NuVecMtxTransform(&wpos, &gobj->bounding_box_center, &gsc->instances[n].mtx);
            if (PtInsideSpline(&wpos, sp) != 0) {
                if (op != NULL) {
                    op[icnt] = &gsc->instances[n];
                }
                icnt++;
                break;
            }
            gobj++;
        }
    }
    return icnt;
}

//NGC MATCH
void BuildVisiTable(struct nugscn_s *gsc) {
  s32 n;
  s32 ocnt;
  char *ptr;
  struct visidat_s *vd;
  
  vscnt = 0;
  for (n = 0, ocnt = 0; n < gsc->numsplines; n++) {
      if (strstr(gsc->splines[n].name,"vis") != NULL) {
        vscnt++;
        ocnt += VSAddObjs(NULL,gsc,&gsc->splines[n]);
      }
  }
    
  vsdata = NULL;
  if (ocnt != 0) {
      vsdata = (struct visidat_s *)NuMemAlloc(vscnt * 0xc + ocnt * 4);
        if (vsdata != NULL) {
            vd = vsdata;
            vscnt = 0;
            for (n = 0; n < gsc->numsplines; n++) {
                if (strstr(gsc->splines[n].name,"vis") != NULL) {
                  visidat[vscnt] = vd;
                  vscnt++;
                  ocnt = VSAddObjs(vd->i,gsc,&gsc->splines[n]);
                  vd->sp = &gsc->splines[n];
                  vd->numinstances = ocnt;
                  ptr = (ocnt * 4);
                  ptr += 0xc;
                  (char*)vd +=  (s32)ptr;
                }
            }
            
       }
  }
  return;
}

//NGC MATCH
void ReleaseVisiTable(void) {
    if (vsdata != NULL) {
        NuMemFree(vsdata);
    }
    vsdata = NULL;
    vscnt = 0;
    return;
}