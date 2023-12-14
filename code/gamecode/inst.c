#include "gamecode/inst.h"

static struct nulsthdr_s* sceneinst_pool;
static struct nulsthdr_s* animdatainst_pool;

//PS2
struct nuscene_s * InstSceneLoad(char *name)
{
  struct sceneinst_s *sc;

    sc = (struct sceneinst_s *)NuLstGetNext(sceneinst_pool,NULL);
    while(sc != NULL) {
        if (strcasecmp(name, sc->name) == 0) {
            sc->inst_cnt++;
            return sc->scene;
        }
        sc = (struct sceneinst_s *)NuLstGetNext(sceneinst_pool,(struct nulnkhdr_s *)sc);
    }

    sc = (struct sceneinst_s *)NuLstAlloc(sceneinst_pool);
    if (sc != NULL) {
        sc->scene = NuSceneLoad(name);
        if (sc->scene != NULL) {
            strcpy(sc->name, name);
            sc->inst_cnt = 1;
            return sc->scene;
        } else {
            NuLstFree((struct nulnkhdr_s *)sc);
        }
    }

    return NULL;
}

//PS2
struct nuanimdata_s * InstAnimDataLoad(char *name)
{
    struct animdatainst_s *lst;
    struct nuanimdata_s *adat;
    struct animdatainst_s *sc;

    // can also be a for loop
    // for (sc =NuLstGetNext(animdatainst_pool, NULL); sc != 0; sc = NuLstGetNext(animdatainst_pool,sc))
    sc = (struct animdatainst_s *)NuLstGetNext(animdatainst_pool, NULL);
    while(sc != 0) {
        if (strcasecmp(name, sc->name) == 0) {
            sc->inst_cnt++;
            return sc->ad;
        }
        sc = (struct animdatainst_s *)NuLstGetNext(animdatainst_pool, (struct nulnkhdr_s *)sc);
    }

    lst = (struct animdatainst_s *)NuLstAlloc(animdatainst_pool);
    if (lst != NULL) {
        adat = NuAnimDataLoadBuff(name, &superbuffer_ptr, &superbuffer_end);
        lst->ad = adat;
        if (adat != NULL) {
            strcpy(lst->name, name);
            lst->inst_cnt = 1;
            return lst->ad;
        } else {
            NuLstFree((struct nulnkhdr_s *)lst);
        }
    }

    return NULL;
}


void InstInit(void)

{
  sceneinst_pool = NuLstCreate(0x10,0x108);
  animdatainst_pool = NuLstCreate(0xc0,0x108);
  return;
}

//PS2
void InstClose(void)
{
  //struct shadinst_s *sdi;
  struct animdatainst_s *adi;
  struct sceneinst_s *si;

  /*  if (shaddatainst_pool != NULL) {
    sdi = (struct shadinst_s *)NuLstGetNext(shaddatainst_pool,NULL);
    while (sdi != NULL) {
      ShadDataDestroy(sdi->shad);
     sdi = NuLstGetNext(shaddatainst_pool,(struct nulnkhdr_s *)sdi);
    }
    NuLstDestroy(shaddatainst_pool);
    shaddatainst_pool = NULL;
  }	*/
  if (animdatainst_pool != NULL) {
    adi = (struct animdatainst_s *)NuLstGetNext(animdatainst_pool,NULL);
    while (adi != NULL)
    {
        adi = (struct animdatainst_s *)NuLstGetNext(animdatainst_pool,(struct nulnkhdr_s *)adi);
    }
    NuLstDestroy(animdatainst_pool);
    animdatainst_pool = NULL;
  }

  if (sceneinst_pool != NULL) {
    si = (struct sceneinst_s *)NuLstGetNext(sceneinst_pool,NULL);
    while (si != NULL) {
      NuSceneDestroy(si->scene);
      si = (struct sceneinst_s *)NuLstGetNext(sceneinst_pool,(struct nulnkhdr_s *)si);
    }
    NuLstDestroy(sceneinst_pool);
    sceneinst_pool = NULL;
  }
  return;
}
