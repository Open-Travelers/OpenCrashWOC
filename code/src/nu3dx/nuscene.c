#include "nuscene.h"
#include"nu3dxtypes.h"
#include"types.h"

#define MAX(x, y) ((x) > (y) ? (x) : (y))

//NGC 97%
void ReadNuIFFTextureSet(s32 handle,struct nuscene_s *scene) {
  s32 count;
  s32 i;
  s32 size;
  s32 pal_size;
  s32 imsize = 0;
  struct nutex_s nuTex;
  s32* new_bits;
  u8* bits8;

  NuFileBeginBlkRead(handle,0x30485354);
  count = NuFileReadInt(handle);
  NuFileEndBlkRead(handle);
  scene->numtids = bits8;
  if (count != 0) {
    scene->tids = (s16 *)NuMemAlloc(count << 1);
    scene->tids[0] = 0;
  }
  else {
    scene->tids = NULL;
  }
    for(i = 0; i < count; i++) {
      NuFileBeginBlkRead(handle,0x304d5854);
      nuTex.type = NuFileReadInt(handle);
      nuTex.width = NuFileReadInt(handle);
      nuTex.height = NuFileReadInt(handle);
      nuTex.mmcnt = NuFileReadInt(handle);
      nuTex.pal = NULL;
      nuTex.bits = NULL;
      nuTex.decal = 0;
      if ((nuTex.type & 0x80) == 0) {
        imsize = NuTexImgSize(nuTex.type,nuTex.width,nuTex.height);
      }
      nuTex.bits = malloc_x(imsize);
      NuFileRead(handle,nuTex.bits,imsize);
      pal_size = NuTexPalSize(nuTex.type);
      if (pal_size != 0) {
        nuTex.pal = (int *)malloc_x(pal_size);
        NuFileRead(handle,nuTex.pal,pal_size);
      }
      size = NuTexCreate(&nuTex);
      scene->tids[scene->numtids++] = (s16)size;

      if (nuTex.pal != NULL) {
        free_x(nuTex.pal);
        nuTex.pal = NULL;
      }
      if (nuTex.bits != NULL) {
        free_x(nuTex.bits);
        nuTex.bits = NULL;
      }
      NuFileEndBlkRead(handle);
    }
  return;
}

//NGC MATCH
void ReadNuIFFMaterialSet(s32 fh,struct nuscene_s *sc) {
    struct numtl_s *nextmtl;
    int i;
    int cnt;
    int tmp;

    //NuDebugMsgProlog("C:/source/crashwoc/code/nu3dx/nuscene.c",0xe6)("Reading IFF Material set...");

    cnt = NuFileReadInt(fh);
    sc->mtls = (struct numtl_s **)NuMemAlloc(cnt << 2);
        sc->nummtls = cnt;
        for(i = 0; i < cnt; i++) {
            sc->mtls[i] = NuMtlRead(fh);
            if (sc->mtls[i]->attrib.alpha != 0) {
                sc->mtls[i]->attrib.zmode = 1;
            }
        }
        for(i = 0; i < cnt; i++) {
            tmp = (int)sc->mtls[i]->next; //check this
            if (tmp > 0) {
                nextmtl = sc->mtls[tmp - 1];
                sc->mtls[i]->next = nextmtl;
                sc->mtls[i]->fxid = nextmtl->fxid;
            }
            else {
                sc->mtls[i]->next = NULL;
            }
        }
    return;
}


//NGC MATCH
void NuSceneMtlUpdate(struct nuscene_s *nus) {
  s32 i;

    for(i = 0; i < nus->nummtls; i++) {
      if (nus->mtls[i]->tid == -1) {
        nus->mtls[i]->tid = 0;
        nus->mtls[i]->L = '\0';
      }
      else {
        nus->mtls[i]->tid = (s32)nus->tids[nus->mtls[i]->tid];
        nus->mtls[i]->L = NuTexGetDecalInfo(nus->mtls[i]->tid);
      }
      NuMtlUpdate(nus->mtls[i]);
    }
  return;
}

//MATCH NGC
void ReadNuIFFAnimationLibrary(s32 fh, struct nugscn_s* gsc)
{
    u8 i;
    u8 narrayelements;

    narrayelements = gsc->numinstanims = NuFileReadChar(fh);

    if (narrayelements > 0)
    {
        gsc->instanimdata = NuMemAlloc(narrayelements * sizeof(struct nuanimdata_s**));
        memset(gsc->instanimdata, 0, narrayelements * sizeof(struct nuanimdata_s**));

        for (i = 0; i < narrayelements; i++)
        {
            if (NuFileReadChar(fh) != 0)
            {
                gsc->instanimdata[i] = NuAnimDataRead(fh);
            }
        }
    }
}

//MATCH NGC
static void ReadNuIFFGSplineSet(s32 fh, struct nugscn_s *gsc) {
    s32 name_ix;
    s32 size;
    char *temp;
    char *temp2;
    s32 i;

    size = NuFileReadInt(fh);
    gsc->numsplines = size;
    name_ix = NuFileReadInt(fh);
    if (gsc->numsplines != 0) {
        temp2 = (char *)NuMemAlloc(name_ix);
        gsc->splinedata = temp2;
        temp = temp2;
        NuFileRead(fh,temp,name_ix);
        gsc->splines = (struct nugspline_s *)NuMemAlloc(gsc->numsplines * sizeof(struct nugspline_s));
            for(i = 0; i < gsc->numsplines; i++) {
               gsc->splines[i].len = *(s16*)(temp + 2);
                temp += 4;
               gsc->splines[i].name = gsc->nametable + *(s16*)(temp + 2);
                temp += 4;
               gsc->splines[i].pts = temp;
               gsc->splines[i].ptsize = 0xc;
                temp += gsc->splines[i].len * 0xc;
            }
    }
    return;
}

//MATCH NGC
void ReadNuIFFGobjSet(s32 fh, struct nuscene_s *sc) {
    s32 cnt;
    s32 i;

    cnt = NuFileReadInt(fh);
    if (sc->gobjs != NULL) {
        NuErrorProlog("C:/source/crashwoc/code/nu3dx/nuscene.c",0x1a8)
        ("ReadNuIFFGobjSet : Object already defined!");
    }
    sc->gobjs = NuMemAlloc(cnt << 2);
    memset(sc->gobjs,0,cnt << 2);
    sc->numgobjs = 0;

    for (i = 0; i < cnt; i++) {

        ReadNuIFFGeomDef(fh,sc);
    }

    return;
}

//MATCH NGC
struct nugobj_s* ReadNuIFFGeom(s32 handle, struct numtl_s** mtls) {

    struct nugobj_s* gobject;
    struct nugobj_s* first;
    struct nugobj_s* last;
    struct nugeom_s* geom;
    struct nufaceongeom_s* fgeom;
    s32 count;
    s32 i;
    s32 j;
    s32 ngobjs;
    struct nuvec_s zero;

    memset(&zero, 0, 0xc); // or struct nuvec_s zero = {};
    last = NULL;
    ngobjs = NuFileReadInt(handle);
    for (i = 0; i < ngobjs; i++) {
        gobject = NuGobjCreate();
        gobject->ignore = 0;
        if (i == 0) {
            first = gobject;
            gobject->ngobjs = ngobjs;
        } else {
            gobject->ngobjs = 0;
        }
        gobject->type = NuFileReadInt(handle);
        switch (gobject->type) {
            case NUGOBJ_MESH:
                NuFileRead(handle, &gobject->origin, 0xc);
                count = NuFileReadInt(handle);
                for (j = 0; j < count; j++) {
                    geom = NuGeomCreate();
                    geom->mtl_id = NuFileReadInt(handle);
                    geom->mtl = mtls[geom->mtl_id];
                    ReadNuIFFGeomVtx(handle, geom);
                    ReadNuIFFGeomCntrl(handle, geom);
                    ReadNuIFFGeomPrim(handle, geom);
                    ReadNuIFFGeomSkin(handle, geom);
                    ReadNuIFFBlendShape(handle, geom);
                    NuGobjAddGeom(gobject, geom);
                }
                break;
            case NUGOBJ_FACEON:
                count = NuFileReadInt(handle);
                for (j = 0; j < count; j++) {
                    gobject->origin = zero;
                    fgeom = NuFaceOnGeomCreate();
                    fgeom->mtl_id = NuFileReadInt(handle);
                    fgeom->mtl = mtls[fgeom->mtl_id];
                    ReadNuIFFFaceOnGeom(handle, fgeom);
                    NuGobjAddFaceOnGeom(gobject, fgeom);
                }
                break;
        }
        NuPs2CreateSkin(gobject);
        NuGobjCalcDims(gobject);
        for (geom = gobject->geom; geom != NULL; geom = geom->next) {
        }
        if (last != NULL) {
            last->next_gobj = gobject;
        }
        last = gobject;
    }
    return first;
}

//MATCH NGC
void ReadNuIFFGeomDef(s32 fh, struct nuscene_s* sc) {
    struct nugobj_s* gobj;
    s32 numgobjs;

    gobj = ReadNuIFFGeom(fh, sc->mtls);
    numgobjs = sc->numgobjs;
    sc->gobjs[sc->numgobjs++] = gobj;
    return;
}

//MATCH NGC
void ReadNuIFFGeomVtx(s32 fh, struct nugeom_s* geom)
{
    s32 cnt;
    struct nuvtx_tc1_s *vtx;

    cnt = NuFileReadInt(fh);
    geom->vtxcnt = cnt;

    if (cnt != 0) {
        NuGeomCreateVB(geom, cnt, NUVT_TC1, 0);
	vtx = (struct nuvtx_tc1_s *)geom->hVB;
        if (vtx == NULL)
        {
            NuErrorProlog("C:/source/crashwoc/code/nu3dx/nuscene.c",599)("ReadNuIFFGeomVtx : Lock VB failed!");
        }

        NuFileRead(fh, vtx, cnt * 0x24);
        GS_ARGBTORGBA(vtx, cnt);
    }
}

//MATCH NGC
void ReadNuIFFGeomCntrl(s32 handle, struct nugeom_s* geom)
{
    // Removed code???
    NuFileReadInt(handle);
    return;
}

//MATCH NGC
static void ReadNuIFFFaceOnGeom(s32 handle,struct nufaceongeom_s *face) {
  s32 bytes;

  bytes = NuFileReadInt(handle);
  face->nfaceons = bytes;
  face->faceon_type = NuFileReadInt(handle);
  face->faceon_radius = NuFileReadFloat(handle);
  bytes = face->nfaceons * 0x18;
  face->faceons = (struct nufaceon_s *)NuMemAlloc(bytes);
  NuFileRead(handle,face->faceons,bytes);
  return;
}

//MATCH NGC
static void ReadNuIFFGeomPrim(s32 fh, struct nugeom_s* geom) {
    s32 cnt;
    s32 type;
    s32 i;

    cnt = NuFileReadInt(fh);
    if (cnt != 0) {
        for (i = 0; i < cnt; i++) {
            type = NuFileReadInt(fh);
            switch (type) {
            case NUPT_NDXTRI:
                ReadNuIFFGeomTri(fh, geom, 5);
                break;
            case NUPT_NDXTRISTRIP:
                ReadNuIFFGeomTri(fh, geom, 6);
                break;
            default:
                NuErrorProlog("C:/source/crashwoc/code/nu3dx/nuscene.c", 0x2a7)("Unknown primitive type");
                break;
            }
        }
    }
}

//MATCH NGC
void ReadNuIFFGeomTri(fileHandle handle, struct nugeom_s* geom, enum nuprimtype_e type)
{
    struct nuprim_s *prim;
    s32 cnt;
    u16* idxptr;

    cnt = NuFileReadInt(handle);
    prim = NuPrimCreate(cnt, type);
    idxptr = (u16*)prim->idxbuff;
    NuFileRead(handle, idxptr, cnt * 2);
    NuGeomAddPrim(geom, prim);
}

//MATCH NGC
static void ReadNuIFFGeomSkin(s32 handle,struct nugeom_s *geom) {
  s32 count;
  struct nuskin_s *skin;
  s32 bytes;
  s32 i;
  s32 j;

  count = NuFileReadInt(handle);
  if (count != 0) {
    if (NuFileReadChar(handle) != '\0') {
      bytes = geom->vtxcnt << 4;
      geom->vtxskininfo = NuMemAlloc(bytes);
      NuFileRead(handle,geom->vtxskininfo,bytes);
    }
    else {
        for (i = 0; i < count; i++) {
          skin = (struct nuskin_s *)NuMemAlloc(0x18);
          memset(skin,0,0x18);
          skin->vtxoffset = NuFileReadInt(handle);
          skin->vtxcnt = NuFileReadInt(handle);
          skin->mtxcnt = NuFileReadInt(handle);
          skin->mtxid = (s32 *)NuMemAlloc(skin->mtxcnt << 2);
            for (j = 0; j < skin->mtxcnt; j++) {
              skin->mtxid[j] = NuFileReadInt(handle);
            }
          bytes = (skin->vtxcnt * 4) * skin->mtxcnt;
          skin->weights = (f32 *)NuMemAlloc(bytes);
          NuFileRead(handle,skin->weights,bytes);
          NuGeomAddSkin(geom,skin);
        }
    }
  }
  return;
}

//84%
static void ReadNuIFFBlendShape(s32 fh,struct nugeom_s *geom) {
    s32 i;
    s32 nblends;
    s32 next_ix;
    s32 j;
    s32 numblends;
    s32 nbytes;

    nblends = NuFileReadInt(fh);
    if (nblends == 0) {
        return;
    }

    // Get size of memory to allocate for nblends of nuvec_s pointers
    nbytes = nblends * sizeof(struct nuvec_s*);

    // allocate NUBLENDGEOM_s
    geom->blendgeom = (struct NUBLENDGEOM_s *)NuMemAlloc(sizeof(struct NUBLENDGEOM_s));

    memset(geom->blendgeom, 0, sizeof(struct NUBLENDGEOM_s));

    geom->blendgeom->nblends = nblends;

    geom->blendgeom->blend_offsets = (struct nuvec_s **)NuMemAlloc(nblends * sizeof(struct nuvec_s*));
    memset(geom->blendgeom->blend_offsets, 0, nbytes);

    // next_ix = (s32)NuMemAlloc(nbytes);
    geom->blendgeom->ix = (s32 *)NuMemAlloc(nbytes);
    NuFileRead(fh, geom->blendgeom->ix, nbytes);

    numblends = NuFileReadInt(fh);
    geom->blendgeom->offsets = (struct nuvec_s *)NuMemAlloc(numblends);
    NuFileRead(fh, geom->blendgeom->offsets, numblends);

    j = 0;
    for (i = 0; i < nblends; i++) {
        if (NuFileReadChar(fh) != '\0') {
            geom->blendgeom->blend_offsets[i] = &geom->blendgeom->offsets[j];
            j += geom->vtxcnt;
        }
    }

    if (numblends == 0) {
        geom->blendgeom = NULL;
    }

    if (geom->blendgeom) {
        for (i = 0; i < geom->vtxcnt; i++) {
            for (j = 0; j < nblends; j++) {
                if (geom->blendgeom->blend_offsets[j] != NULL) {
                    if (NuFabs(geom->blendgeom->blend_offsets[j][i].x) < 0.000001f) {
                        geom->blendgeom->blend_offsets[j][i].x = 0.0f;
                    }
                    if (NuFabs(geom->blendgeom->blend_offsets[j][i].y) < 0.000001f) {
                        geom->blendgeom->blend_offsets[j][i].y = 0.0f;
                    }
                    if (NuFabs(geom->blendgeom->blend_offsets[j][i].z) < 0.000001f) {
                        geom->blendgeom->blend_offsets[j][i].z = 0.0f;
                    }
                }
            }
        }
        geom->blendgeom->hVB =  GS_CreateBuffer(geom->vtxcnt * 0xc,3);
    }
}


void ReadNuIFFUnknown(s32 fh, s32 blk) {

	//NuDebugMsgProlog("C:/source/crashwoc/code/nu3dx/nuscene.c",0x4d9)("Unknown block type %c%c%c%c",blk & 0xff,blk >> 8 & 0xff, blk >> 0x10 & 0xff,blk >> 0x18);
        return;
}

//PS2
s32 ReadNuIFFInstSet(s32 fh,struct nuinstance_s **instances,struct nuinstanim_s **instanims)
{
  s32 i;
  s32 num_instanims;
  s32 bytes;
  s32 num_instances;
  struct nuinstance_s *instance;
  struct nuinstanim_s *linstanims;

  num_instances = NuFileReadInt(fh);
  *instances = (struct nuinstance_s *)NuMemAlloc(num_instances * sizeof(struct nuinstance_s)); //,"..\\nu2.ps2\\nu3d\\nuscene.c",0x5a3
  NuFileRead(fh,*instances,num_instances * sizeof(struct nuinstance_s));
  num_instanims = NuFileReadInt(fh);
  if (num_instanims != 0) {
      bytes = num_instanims * sizeof(struct nuinstanim_s);
    linstanims = (struct nuinstanim_s *)NuMemAlloc(bytes); //,"..\\nu2.ps2\\nu3d\\nuscene.c",0x5aa
    NuFileRead(fh,linstanims,bytes);
    if (instanims != NULL) {
      *instanims = linstanims;
    }
      for(i = 0; i < num_instances; i++) {
          instance = &instances[0][i];
          if (instance->anim != 0) {
              instance->anim = linstanims++;
              instance->anim->mtx = instance->mtx;
          }
      }
  }
  return num_instances;
}

//NGC MATCH
static void ReadNuIFFSpecialObjects(s32 fh,struct nugscn_s *gsc) {
  struct nufspecial_s *file_specials;
  s32 i;

  gsc->numspecial = NuFileReadInt(fh);
  file_specials = (struct nufspecial_s *)malloc_x(gsc->numspecial * 0x50);
  NuFileRead(fh,file_specials,gsc->numspecial * 0x50);
  gsc->specials = (struct nuspecial_s *)NuMemAlloc(gsc->numspecial * 0x50);

  for(i = 0; i < gsc->numspecial; i++) {
      gsc->specials[i].instance = gsc->instances + file_specials[i].instanceix;
      gsc->specials[i].mtx = gsc->specials[i].instance->mtx;
      gsc->specials[i].name = gsc->nametable + file_specials[i].nameix;
      gsc->specials[i].instance->special_flag = '\x01';
    }
  free_x(file_specials);
  return;
}

//NGC MATCH
static void NuSceneCalcCulling(struct nugscn_s *scene) {
    s32 num_gobjs;
    s32 num_instances;
    s32 objid;
    s32 i;
    float *max_scales_array;
    struct nuvec_s scale;

    num_gobjs = scene->numgobj;
    num_instances = scene->numinstance;
    max_scales_array = (float*)malloc_x(num_gobjs * sizeof(float));
    memset(max_scales_array, 0, num_gobjs * sizeof(float));

    for(i = 0; i < num_instances; i++) {
        if (scene->instances[i].special_flag != '\0' || scene->instances[i].anim != NULL) {
                scene->gobjs[scene->instances[i].objid]->culltype = 1;
        }
        else {
                NuMtxGetScale(&scale, &scene->instances[i].mtx);
                objid = scene->instances[i].objid;

                max_scales_array[objid] = MAX(max_scales_array[objid], scale.x);
                max_scales_array[objid] = MAX(max_scales_array[objid], scale.y);
                max_scales_array[objid] = MAX(max_scales_array[objid], scale.z);

                scene->gobjs[objid]->culltype = 0;
        }
    }

    for(i = 0; i < num_gobjs; i++) {
        scene->gobjs[i]->bounding_radius_from_center *= max_scales_array[i];
    }

    free_x(max_scales_array);
    return;
}

//PS2
void ReadNuIFFTexAnimSet(s32 fh, struct nugscn_s* gsc, s16* tids)
{
    struct nutexanimf_s *ftex;
    struct nutexanim_s *tex;
    struct nutexanimprog_s *prog;
    s32 j;
    s32 i;
    s32 cnt;


    //NuDebugMsgProlog("..\\nu2.ps2\\nu3d\\nugscn.c", 0xC61)("Reading Animated texture set...");
    gsc->numtexanims = NuFileReadInt(fh);
    NuFileReadInt(fh);
    gsc->texanims = NuMemAlloc(gsc->numtexanims * sizeof(struct nutexanim_s)); //, "..\\nu2.ps2\\nu3d\\nugscn.c", 0xC68
    NuFileRead(fh, gsc->texanims, gsc->numtexanims * sizeof(struct nutexanim_s));
    cnt = NuFileReadInt(fh) * 2;
    gsc->texanim_tids = (s16*)NuMemAlloc(cnt); //, "..\\nu2.ps2\\nu3d\\nugscn.c", 0xC6D
    NuFileRead(fh, gsc->texanim_tids, cnt);

    for (i = 0; i < gsc->numtexanims; i++){
        tex = &gsc->texanims[i]; //tex = gsc->texanims + (cnt << 5);
        tex->succ = 0;
        tex->prev = 0;
        tex->tids = (gsc->texanim_tids + ((s32)tex->tids));
        tex->dynalloc = 0;
        for(j = 0;  j < tex->numtids; j++)
        {
            tex->tids[j] = tids[tex->tids[j]];
        }
        tex->ntaname = (char*)(&gsc->nametable[(s32)tex->ntaname]);
        tex->scriptname = (char*)(&gsc->nametable[(s32)tex->scriptname]);
        tex->mtl = gsc->mtls[(s32)tex->mtl];
        tex->env = NuTexAnimEnvCreate(0, tex->mtl, tex->tids, NuTexAnimProgFind((s32)tex->scriptname));
    }

    for (i = 0;  i < (gsc->numtexanims - 1); i++)
    {
        gsc->texanims[i].succ = &gsc->texanims[i] + 1;
        gsc->texanims[i + 1].prev = &gsc->texanims[i];

    }
    NuTexAnimAddList(gsc->texanims);
    return;
}

//NGC MATCH
void ReadNuIFFGScene(s32 handle,struct nugscn_s *gscene) {
  s32 blk;
  struct nuscene_s sc;
  s32 texanim;

  texanim = -1;
  memset(&sc,0,0x3c);
    while (blk = NuFileBeginBlkRead(handle, 0)) {
                    switch(blk)
                    {
                        case 0x30534154: //0SAT
                            texanim = NuFilePos(handle);
                        break;
                        case 0x30545354:    //0TST
                            ReadNuIFFTextureSet(handle,&sc);
                        break;
                        case 0x3030534D: //"00SM"
                            ReadNuIFFMaterialSet(handle,&sc);
                        break;
                        case 0x30545347: //"0TSG"
                          ReadNuIFFGobjSet(handle,&sc);
                        break;
                        case 0x54534E49: //"TSNI"
                            gscene->numinstance = ReadNuIFFInstSet(handle,&gscene->instances,&gscene->instanimblock);
                        break;
                        case 0x4C42544E: //"LBTN"
                            gscene->nametable = ReadNuIFFNameTable(handle);
                        break;
                        case 0x43455053: //"CEPS"
                            ReadNuIFFSpecialObjects(handle,gscene);
                        break;
                        case 0x30545353: //"0TSS"
                            ReadNuIFFGSplineSet(handle,gscene);
                        break;
                        case 0x42494C41: //"BILA"
                            ReadNuIFFAnimationLibrary(handle,gscene);
                        break;
                        default:
                             ReadNuIFFUnknown(handle,blk);
                        break;
                    }
              NuFileEndBlkRead(handle);
    }
    NuSceneMtlUpdate(&sc);
    gscene->mtls = sc.mtls;
    gscene->nummtl = sc.nummtls;
    gscene->numgobj = sc.numgobjs;
    gscene->gobjs = sc.gobjs;
    gscene->numtid = sc.numtids;
    gscene->tids = sc.tids;
    NuSceneCalcCulling(gscene);
    if (texanim != -1) {
        NuFileSeek(handle,texanim,0);
        ReadNuIFFTexAnimSet(handle,gscene,sc.tids);
    }

    return;
}

//NGC MATCH
struct nuscene_s * NuSceneLoad(char *filename) {
  s32 fh;
  struct nuscene_s *scene;
  s32 blk;
  s32 len;
  char file2[100];
  s32 i;
  //s32 j;
  struct nugeom_s *geom1;
  struct nugobj_s *gobj1;


  strcpy(file2,filename);
  scene = NULL;
  len = strlen(file2);
  file2[len + -1] = 0x78;
  strcpy(file2,filename);
  fh = NuFileOpen(file2,NUFILE_READ);
  if (fh != 0) {
            strlwr(filename);
            scene = (struct nuscene_s *)NuMemAlloc(0x3c);
            memset(scene,0,0x3c);
            blk = blkcnt;
            if (NuFileBeginBlkRead(fh,0) == 0x30435347) { //"0CSG"
              scene->gscene = (struct nugscn_s *)NuMemAlloc(0x74);
              memset(scene->gscene,0,0x74);
              ReadNuIFFGScene(fh,scene->gscene);
                for (i = 0; i < scene->gscene->numinstance; i++) {
                      for (geom1 = scene->gscene->gobjs[scene->gscene->instances[i].objid]->geom;
                          geom1 != NULL; geom1 = geom1->next) {
                      }
                }
              NuSceneAverageTextureSpaceVerts(scene);
                for (i = 0; i < scene->gscene->numgobj; i++) {
                      for (gobj1 = scene->gscene->gobjs[i]; gobj1 != NULL; gobj1 = gobj1->next_gobj) {
                            for (geom1 = *(&gobj1->geom); geom1 != NULL; geom1 = geom1->next) {
                            }
                      }
                }
            }
            else {
              NuMemFree(scene);
              NuErrorProlog("C:/source/crashwoc/code/nu3dx/nuscene.c",0x718,"Baaaaad file <%s>",filename);
              return scene = NULL;
            }
              NuFileEndBlkRead(fh);
              NuFileClose(fh);
            if (blk != blkcnt) {
                blkcnt = blk;
            }
  }
  else {
    strlwr(filename);
  }
  return scene;
}

//NGC MATCH
s8* ReadNuIFFNameTable(s32 handle) {
    s8* str;
    s32 bytes;

    bytes = NuFileReadInt(handle);
    str = (s8*)NuMemAlloc(bytes);
    NuFileRead(handle, str, bytes);
    return str;
}

//NGC MATCH
void NuGSceneDestroy(struct nugscn_s *gsc) {
  s32 i;

  for (i = 0; i < gsc->numtid; i++) {
    NuTexDestroy((s32)gsc->tids[i]);
  }
  for (i = 0; i < gsc->nummtl; i++) {
      NuMtlDestroy(gsc->mtls[i]);
  }
  for (i = 0; i < gsc->numgobj; i++) {
      NuGobjDestroy(gsc->gobjs[i]);
  }
  if ((gsc->instanimdata != NULL)) {
        for (i = 0; i < gsc->numinstanims; i++) {
          NuAnimDataDestroy(gsc->instanimdata[i]);
        }
  }

  return;
}

//NGC MATCH
void NuSceneDestroy(struct nuscene_s *sc) {
    s32 i;

    if (sc != NULL) {
        if (sc->names != NULL) {
            NuMemFree(sc->names);
        }
        if (sc->nodes != NULL) {
            NuMemFree(sc->nodes);
        }
        if (sc->strings != NULL) {
            NuMemFree(sc->strings);
        }

        for (i = 0; i < sc->numtids; i++) {
                NuTexDestroy((int)sc->tids[i]);
        }
        for (i = 0; i < sc->nummtls; i++) {
                NuMtlDestroy(sc->mtls[i]);
        }
        for (i = 0; i < sc->numgobjs; i++) {
                NuGobjDestroy(sc->gobjs[i]);
        }

        if (sc->gscene != NULL) {
            NuGSceneDestroy(sc->gscene);
        }
    }
    return;
}
