#include "nuscene.h"
#include"nu3dxtypes.h"
#include"types.h"

// Since this project isn't trying to match the original code,
// I figured it would be nice to try and have the errors show the correct file/line,
// rather than manually entering it


void ReadNuIFFTextureSet(fileHandle handle,struct nuscene_s *scene)
{
  s32 count;
  s16 *tids;
  s32 imsize;
  s32 numtids;
  struct nutex_s nuTex;

  NuFileBeginBlkRead(handle,"0HST");
  count = NuFileReadInt(handle);
  NuFileEndBlkRead(handle);
  scene->numtids = 0;
  if (count == 0) {
    scene->tids = (s16 *)0x0;
  }
  else {
    tids = (s16 *)NuMemAlloc(count << 1);
    scene->tids = tids;
    *tids = 0;
  }
  if (0 < count) {
    do {
      NuFileBeginBlkRead(handle,"0MXT");
      nuTex.type = NuFileReadInt(handle);
      nuTex.width = NuFileReadInt(handle);
      nuTex.height = NuFileReadInt(handle);
      imsize = NuFileReadInt(handle);
      nuTex.pal = (int *)0x0;
      nuTex.bits = (void *)0x0;
      nuTex.decal = 0;
      nuTex.mmcnt = imsize;
      if ((nuTex.type & 0x80) == 0) {
        imsize = NuTexImgSize(nuTex.type,nuTex.width,nuTex.height);
      }
      nuTex.bits = malloc_x(imsize);
      NuFileRead(handle,nuTex.bits,imsize);
      imsize = NuTexPalSize(nuTex.type);
      if (imsize != 0) {
        nuTex.pal = (int *)malloc_x(imsize);
        NuFileRead(handle,nuTex.pal,imsize);
      }
      imsize = NuTexCreate(&nuTex);
      numtids = scene->numtids;
      scene->tids[numtids] = (s16)imsize;
      scene->numtids = numtids + 1;
      if (nuTex.pal != (int *)0x0) {
        free_x(nuTex.pal);
        nuTex.pal = (int *)0x0;
      }
      if (nuTex.bits != (void *)0x0) {
        free_x(nuTex.bits);
        nuTex.bits = (void *)0x0;
      }
      NuFileEndBlkRead(handle);
      count = count + -1;
    } while (count != 0);
  }
  return;
}

void ReadNuIFFMaterialSet(fileHandle fh,struct nuscene_s *sc)	//CHECK
{
  struct numtl_s* attr;
  s32 nmtl;
  struct numtl_s **mtl;
  struct numtl_s *mtlR;
  struct numtl_s *nextmtl;
  s32 i;
  s32 j;

  i = 0;
  NuDebugMsgProlog("C:/source/crashwoc/code/nu3dx/nuscene.c",0xe6)("Reading IFF Material set...");
  nmtl = NuFileReadInt(fh);
  mtl = (struct numtl_s **)NuMemAlloc(nmtl << 2);
  sc->nummtls = nmtl;
  sc->mtls = mtl;
  if (0 < nmtl) {
    do {
      mtlR = (struct numtl_s *)NuMtlRead(fh);
      j = i + 1;
      sc->mtls[i] = mtlR;
      attr = sc->mtls[i];
      if ((attr->attrib)._word >> 0x1e != 0) {
        (attr->attrib)._word = (attr->attrib)._word & 0xfff3ffff | 0x40000;	//could be? --> sc->mtls[i]->attrib).aref = 0xffffffff
      }
      i = j;
    } while (j < nmtl);
  }
  i = 0;
  if (0 < nmtl) {
    do {
      mtlR = sc->mtls[i];
      if ((s32)mtlR->next < 1) {
        mtlR->next = NULL;
      }
      else {
        nextmtl = sc->mtls[(s32)((s32)&mtlR->next[-1].sv + 3)];
        mtlR->next = nextmtl;
        sc->mtls[i]->fxid = nextmtl->fxid;
      }
      i = i + 1;
      nmtl = nmtl + -1;
    } while (nmtl != 0);
  }
  return;
}


void NuSceneMtlUpdate(struct nuscene_s *nus)
{
  u8 decal;
  struct numtl_s *mtl;
  s32 i;
  s32 n;

  i = 0;
  if (0 < nus->nummtls) {
    do {
      mtl = nus->mtls[i];
      if (mtl->tid == -1) {
        mtl->tid = 0;
        mtl->L = '\0';
      }
      else {
        mtl->tid = (s32)nus->tids[mtl->tid];
        mtl = nus->mtls[i];
        decal = NuTexGetDecalInfo(mtl->tid);
        mtl->L = decal;
      }
      n = i + 1;
      NuMtlUpdate((s8)nus->mtls[i]);
      i = n;
    } while (n < nus->nummtls);
  }
  return;
}




void ReadNuIFFAnimationLibrary(fileHandle handle, struct nugscn_s* scene)
{
    u8 i;

    u8 numAnim = NuFileReadChar(handle);
    scene->numinstanims = numAnim;

    if (numAnim > 0)
    {
        scene->instanimdata = NuMemAlloc(numAnim * sizeof(struct nuanimdata_s**));
        memset(scene->instanimdata, 0, numAnim * sizeof(struct nuanimdata_s**));

        for (i = 0; i < numAnim; i++)
        {
            if (NuFileReadChar(handle) != 0)
            {
                scene->instanimdata[i] = NuAnimDataRead(handle);
            }
        }
    }
}

/*void ReadNuIFFGSplineSet(fileHandle fh,struct nugscn_s *gsc)
{
  s16 *psVar1;
  s32 index;
  void *spline;
  struct nugspline_s *splineDataIndex;
  s32 iVar2;

  index = NuFileReadInt(fh);
  gsc->numsplines = index;
  index = NuFileReadInt(fh);
  if (gsc->numsplines != 0) {
    spline = NuMemAlloc(index);
    gsc->splinedata = spline;
    NuFileRead(fh,spline,index);
    splineDataIndex = (struct nugspline_s *)NuMemAlloc(gsc->numsplines * 0xc);
    index = 0;
    gsc->splines = splineDataIndex;
    if (0 < gsc->numsplines) {
      iVar2 = 0;
      do {
        index = index + 1;
        *(undefined2 *)((s32)&gsc->splines->len + iVar2) = *(undefined2 *)((s32)spline + 2);
        *(s8 **)((s32)&gsc->splines->name + iVar2) = gsc->nametable + *(s16 *)((s32)spline + 6 );
        *(s32 *)((s32)&gsc->splines->pts + iVar2) = (s32)spline + 8;
        *(undefined2 *)((s32)&gsc->splines->ptsize + iVar2) = 0xc;
        psVar1 = (s16 *)((s32)&gsc->splines->len + iVar2);
        iVar2 = iVar2 + 0xc;
        spline = (void *)((s32)spline + 8 + *psVar1 * 0xc);
      } while (index < gsc->numsplines);
    }
  }
  return;
}*/


void ReadNuIFFGobjSet(fileHandle handle, struct nuscene_s* nus)
{
    s32 numGobj = NuFileReadInt(handle);

    if (nus->gobjs != NULL)
    {
        NuError("ReadNuIFFGobjSet : Object already defined!");
    }

    nus->gobjs = NuMemAlloc(numGobj * sizeof(struct nugobj_s*));
    memset(nus->gobjs, 0, numGobj * sizeof(struct nugobj_s*));
    nus->numgobjs = 0;

    while (numGobj-- > 0)
    {
        ReadNuIFFGeomDef(handle, nus);
    }
}

struct nugobj_s * ReadNuIFFGeom(fileHandle handle,struct numtl_s **mtls)

{
  s32 ngobjs;
  struct nugobj_s *gobject;
  enum gobjtype_s type;
  s32 mtlid;
  struct nufaceongeom_s *face;
  struct nugobj_s *first;
  struct nugobj_s *last;
  struct nugeom_s *geom;
  s32 i;
  s32 count;
  struct nuvec_s zero;

  memset(&zero,0,0xc);
  i = 0;
  last = NULL;
  ngobjs = NuFileReadInt(handle);
  if (0 < ngobjs) {
    do {
      gobject = NuGobjCreate();
      gobject->ignore = 0;
      if (i == 0) {
        gobject->ngobjs = ngobjs;
        first = gobject;
      }
      else {
        gobject->ngobjs = 0;
      }
      type = NuFileReadInt(handle);
      gobject->type = type;
      if (type == NUGOBJ_MESH) {
        NuFileRead(handle,&gobject->origin,0xc);
        count = NuFileReadInt(handle);
        if (0 < count) {
          do {
            geom = NuGeomCreate();
            mtlid = NuFileReadInt(handle);
            geom->mtl_id = mtlid;
            geom->mtl = mtls[mtlid];
            ReadNuIFFGeomVtx(handle,geom);
            ReadNuIFFGeomCntrl(handle,geom);
            ReadNuIFFGeomPrim(handle,geom);
            ReadNuIFFGeomSkin(handle,geom);
            ReadNuIFFBlendShape(handle,geom);
            NuGobjAddGeom(gobject,geom);
            count = count + -1;
          } while (count != 0);
        }
      }
      else if ((type == NUGOBJ_FACEON) && (count = NuFileReadInt(handle), 0 < count)) {
        do {
          (gobject->origin).x = zero.x;
          (gobject->origin).y = zero.y;
          (gobject->origin).z = zero.z;
          face = NuFaceOnGeomCreate();
          mtlid = NuFileReadInt(handle);
          face->mtl_id = mtlid;
          face->mtl = mtls[mtlid];
          ReadNuIFFFaceOnGeom(handle,face);
          NuGobjAddFaceOnGeom(gobject,face);
          count = count + -1;
        } while (count != 0);
      }
      i = i + 1;
      NuPs2CreateSkin(gobject);   //finish cvtskin file
      NuGobjCalcDims(gobject);
      for (geom = gobject->geom; geom != NULL; geom = geom->next) {
      }
      if (last != NULL) {
        last->next_gobj = gobject;
      }
      last = gobject;
    } while (i < ngobjs);
  }
  return first;
}


void ReadNuIFFGeomDef(fileHandle handle, struct nuscene_s* nus)
{
    struct nugobj_s* gobj;
    s32 numgobjs;

    gobj = ReadNuIFFGeom(handle, &nus->mtls);
    numgobjs = nus->numgobjs;
    nus->gobjs[numgobjs] = gobj;
    nus->numgobjs = numgobjs + 1;
    return;
}

void ReadNuIFFGeomVtx(fileHandle handle, struct nugeom_s* geom)
{

    s32 vtxCount = NuFileReadInt(handle);
    geom->vtxcnt = vtxCount;

    if (vtxCount != 0)
    {
        NuGeomCreateVB(geom, vtxCount, NUVT_TC1, 0);

        if (geom->hVB == NULL)
        {
            NuError("ReadNuIFFGeomVtx : Lock VB failed!");
        }

        NuFileRead(handle, geom->hVB, vtxCount * 0x24);
        //GS_ARGBTORGBA(geom->hVB, vtxCount);   add
    }
}

void ReadNuIFFGeomCntrl(fileHandle handle, struct nugeom_s* geom)
{
    // Removed code???
    NuFileReadInt(handle);
    return;
}

void ReadNuIFFFaceOnGeom(fileHandle handle,struct nufaceongeom_s *face)

{
  s32 nfaceons;
  enum nufaceontype_s type;
  struct nufaceon_s *faceon;
  f32 fVar1;

  nfaceons = NuFileReadInt(handle);
  face->nfaceons = nfaceons;
  type = NuFileReadInt(handle);
  face->faceon_type = type;
  fVar1 = NuFileReadFloat(handle);
  face->faceon_radius = fVar1;
  nfaceons = face->nfaceons * 0x18;
  faceon = (struct nufaceon_s *)NuMemAlloc(nfaceons);
  face->faceons = faceon;
  NuFileRead(handle,faceon,nfaceons);
  return;
}

void ReadNuIFFGeomPrim(fileHandle handle, struct nugeom_s* geom)
{
    s32 counter;
    enum nuprimtype_e type;

    counter = NuFileReadInt(handle);
    if ((counter != 0) && (0 < counter)) {
        do {
            type = NuFileReadInt(handle);
            if (type == NUPT_NDXTRI) {
                ReadNuIFFGeomTri(handle, geom, NUPT_NDXTRI);
            }
            else if (type == NUPT_NDXTRISTRIP) {
                ReadNuIFFGeomTri(handle, geom, NUPT_NDXTRISTRIP);
            }
            else {
                //e = NuErrorProlog("C:/source/crashwoc/code/nu3dx/nuscene.c", 0x2a7);
                //(*e)("Unknown primitive type");
            }
            counter = counter + -1;
        } while (counter != 0);
    }
    return;
}

void ReadNuIFFGeomTri(fileHandle handle, struct nugeom_s* geom, enum nuprimtype_e type)
{
    struct nuprim_s *prim;
    s32 itemCount;

    itemCount = NuFileReadInt(handle);
    prim = NuPrimCreate(itemCount, type);
    NuFileRead(handle, (void*)prim->idxbuff, itemCount * 2);
    NuGeomAddPrim(geom, prim);
}

void ReadNuIFFGeomSkin(fileHandle handle,struct nugeom_s *geom)

{
  s32 count;
  s8 index;
  struct NUVTXSKININFO_s *vtxskin_info;
  struct nuskin_s *skin;
  s32 byteIndex;
  s32 *mtxid;
  s32 iVar1;
  f32 *data;
  s32 indx_;
  s32 cycleIndex;

  count = NuFileReadInt(handle);
  if (count != 0) {
    index = NuFileReadChar(handle);
    if (index == '\0') {
      indx_ = 0;
      if (0 < count) {
        do {
          cycleIndex = 0;
          skin = (struct nuskin_s *)NuMemAlloc(0x18);
          indx_ = indx_ + 1;
          memset(skin,0,0x18);
          byteIndex = NuFileReadInt(handle);
          skin->vtxoffset = byteIndex;
          byteIndex = NuFileReadInt(handle);
          skin->vtxcnt = byteIndex;
          byteIndex = NuFileReadInt(handle);
          skin->mtxcnt = byteIndex;
          mtxid = (s32 *)NuMemAlloc(byteIndex << 2);
          skin->mtxid = mtxid;
          if (0 < skin->mtxcnt) {
            byteIndex = 0;
            do {
              cycleIndex = cycleIndex + 1;
              iVar1 = NuFileReadInt(handle);
              *(s32 *)(byteIndex + (s32)skin->mtxid) = iVar1;
              byteIndex = byteIndex + 4;
            } while (cycleIndex < skin->mtxcnt);
          }
          byteIndex = skin->vtxcnt * skin->mtxcnt * 4;
          data = (f32 *)NuMemAlloc(byteIndex);
          skin->weights = data;
          NuFileRead(handle,data,byteIndex);
          NuGeomAddSkin(geom,skin);
        } while (indx_ < count);
      }
    }
    else {
      count = geom->vtxcnt << 4;
      vtxskin_info = (struct NUVTXSKININFO_s *)NuMemAlloc(count);
      geom->vtxskininfo = vtxskin_info;
      NuFileRead(handle,vtxskin_info,count);
    }
  }
  return;
}


void ReadNuIFFBlendShape(fileHandle handle,struct nugeom_s *geom)

{
  f32 fVar1;
  s32 vtx_count;
  s32 nblends;
  struct NUBLENDGEOM_s *blendgeom;
  struct nuvec_s **offsets;
  s32 next_ix;
  s32 size;
  struct nuvec_s *pnVar2;
  char cVar3;
  struct GS_Buffer *vtxBuf;
  s32 iVar4;
  s32 iVar5;
  s32 iVar6;
  double dVar7;
  double dVar8;
  double dVar9;
  s32 __n;

  nblends = NuFileReadInt(handle);
  if (nblends != 0) {
    __n = nblends << 2;
    blendgeom = (struct NUBLENDGEOM_s *)NuMemAlloc(0x40);
    geom->blendgeom = blendgeom;
    vtx_count = 0;
    memset(blendgeom,0,0x40);
    geom->blendgeom->nblends = nblends;
    offsets = (struct nuvec_s **)NuMemAlloc(__n);
    geom->blendgeom->blend_offsets = offsets;
    memset(geom->blendgeom->blend_offsets,0,__n);
    next_ix = (s32)NuMemAlloc(__n);
    geom->blendgeom->ix = (s32 *)next_ix;
    NuFileRead(handle,geom->blendgeom->ix,__n);
    size = NuFileReadInt(handle);
    pnVar2 = (struct nuvec_s *)NuMemAlloc(size);
    geom->blendgeom->offsets = pnVar2;
    NuFileRead(handle,geom->blendgeom->offsets,size);
    if (0 < nblends) {
      iVar5 = 0;
      iVar4 = nblends;
      do {
        cVar3 = NuFileReadChar(handle);
        if (cVar3 != '\0') {
          *(struct nuvec_s **)(iVar5 + (int)geom->blendgeom->blend_offsets) =
               geom->blendgeom->offsets + vtx_count;
          vtx_count = vtx_count + geom->vtxcnt;
        }
        iVar5 = iVar5 + 4;
        iVar4 = iVar4 + -1;
      } while (iVar4 != 0);
    }
    if (size == 0) {
      geom->blendgeom = NULL;
    }
    blendgeom = geom->blendgeom;
    if (blendgeom != NULL) {
      vtx_count = geom->vtxcnt;
      size = 0;
      while (size < vtx_count) {
        if (0 < nblends) {
          iVar4 = 0;
          dVar8 = 0.0;
          vtx_count = nblends;
          do {
            iVar5 = *(s32 *)(iVar4 + (s32)blendgeom->blend_offsets);
            if (iVar5 != 0) {
              iVar6 = size * 0xc;
              fVar1 = *(f32*)(iVar6 + iVar5);
              dVar7 = (double)fVar1;
              NuFabs(fVar1);
              dVar9 = 9.999999747378752e-06;
              if (dVar7 < 9.999999747378752e-06) {
                *(f32 *)(iVar6 + *(s32 *)(iVar4 + (s32)geom->blendgeom->blend_offsets)) =
                     (f32)dVar8;
              }
              fVar1 = *(f32 *)(iVar6 + *(s32 *)(iVar4 + (s32)geom->blendgeom->blend_offsets) + 4 );
              dVar7 = (double)fVar1;
              NuFabs(fVar1);
              if (dVar7 < dVar9) {
                *(f32 *)(iVar6 + *(s32 *)(iVar4 + (s32)geom->blendgeom->blend_offsets) + 4) =
                     (f32)dVar8;
              }
              fVar1 = *(f32 *)(iVar6 + *(s32 *)(iVar4 + (s32)geom->blendgeom->blend_offsets) + 8 );
              dVar7 = (double)fVar1;
              NuFabs(fVar1);
              if (dVar7 < dVar9) {
                *(f32 *)(iVar6 + *(s32 *)(iVar4 + (s32)geom->blendgeom->blend_offsets) + 8) =
                     (f32)dVar8;
              }
              blendgeom = geom->blendgeom;
            }
            iVar4 = iVar4 + 4;
            vtx_count = vtx_count + -1;
          } while (vtx_count != 0);
        }
        size = size + 1;
        vtx_count = geom->vtxcnt;
      }
      vtxBuf = GS_CreateBuffer(vtx_count * 0xc,3);
      geom->blendgeom->hVB = (s32)vtxBuf;
    }
  }
  return;
}


void ReadNuIFFUnknown(fileHandle handle, u32 blockMagic)
{

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


void ReadNuIFFSpecialObjects(fileHandle fh,struct nugscn_s *gsc)

{
  s32 numspec;
  struct nufspecial_s *file_specials;
  struct nuspecial_s *pnVar1;
  s32 cnt;
  s32 i;
  //struct nuinstance_s *inst;
  struct nuinstance_s *pnVar2;
  //struct nuspecial_s *spec;

  numspec = NuFileReadInt(fh);
  gsc->numspecial = numspec;
  file_specials = (struct nufspecial_s *)malloc_x(numspec * 0x50);
  NuFileRead(fh,file_specials,gsc->numspecial * 0x50);
  pnVar1 = (struct nuspecial_s *)NuMemAlloc(gsc->numspecial * 0x50);
  numspec = 0;
  gsc->specials = pnVar1;
  if (0 < gsc->numspecial) {
    do {
      cnt = numspec + 1;
      i = 0x30;
      gsc->specials[numspec].instance = gsc->instances + file_specials[numspec].instanceix;
      pnVar2 = gsc->specials[numspec].instance;
      pnVar1 = gsc->specials + numspec;

      pnVar1->mtx = pnVar2->mtx;
      /*do {
        spec = gsc->specials + numspec;
        inst = gsc->specials[numspec].instance;
        i = i + -0x18;
        (spec->mtx)._00 = (inst->mtx)._00;
        (spec->mtx)._01 = (inst->mtx)._01;
        (spec->mtx)._02 = (inst->mtx)._02;
        (spec->mtx)._03 = (inst->mtx)._03;
        (spec->mtx)._10 = (inst->mtx)._10;
        pnVar2 = (struct nuinstance_s *)&(inst->mtx)._12;
        (spec->mtx)._11 = (inst->mtx)._11;
        pnVar1 = (struct nuspecial_s *)&(spec->mtx)._12;
      } while (i != 0);
      *(f32 *)pnVar1 = *(f32 *)pnVar2;
      (spec->mtx)._13 = (inst->mtx)._13;
      (spec->mtx)._20 = (inst->mtx)._20;
      (spec->mtx)._21 = (inst->mtx)._21;*/
      gsc->specials[numspec].name = gsc->nametable + file_specials[numspec].nameix;
      (gsc->specials[numspec].instance)->special_flag = '\x01';
      numspec = cnt;
    } while (cnt < gsc->numspecial);
  }
  free_x(file_specials);
  return;
}

void NuSceneCalcCulling(struct nugscn_s *scene)

{
  float *pfVar1;
  int iVar2;
  float fVar3;
  float fVar4;
  int iVar5;
  void *allc;
  struct nuvec_s *v;
  int numgobj;
  int ninst;
  int iVar6;
  float local_30;
  float local_2c;
  float local_28;

  numgobj = scene->numgobj;
  ninst = scene->numinstance;
  allc = malloc_x(numgobj << 2);
  memset(allc,0,numgobj << 2);
  if (0 < ninst) {
    iVar6 = 0;
    do {
      v = (struct nuvec_s *)(scene->instances->pad + iVar6 + -0x4f);
      if (*(char *)((int)&v[6].y + 2) == '\0') {
        if (v[6].x != 0.0) goto LAB_800b9214;
        NuMtxGetScale((struct numtx_s *)&local_30,v);
        iVar5 = *(int *)(scene->instances->pad + iVar6 + -0xf);
        iVar2 = iVar5 * 4;
        fVar3 = local_30;
        if (local_30 < *(float *)((int)allc + iVar2)) {
          fVar3 = *(float *)((int)allc + iVar2);
        }
        *(float *)((int)allc + iVar2) = fVar3;
        fVar4 = local_2c;
        if (local_2c < fVar3) {
          fVar4 = fVar3;
        }
        *(float *)(iVar2 + (int)allc) = fVar4;
        fVar3 = local_28;
        if (local_28 < fVar4) {
          fVar3 = fVar4;
        }
        *(float *)((int)allc + iVar2) = fVar3;
        scene->gobjs[iVar5]->culltype = 0;
      }
      else {
LAB_800b9214:
        scene->gobjs[(int)v[5].y]->culltype = 1;
      }
      iVar6 = iVar6 + 0x50;
      ninst = ninst + -1;
    } while (ninst != 0);
  }
  if (0 < numgobj) {
    ninst = 0;
    do {
      numgobj = numgobj + -1;
      pfVar1 = (float *)(ninst + (int)allc);
      iVar6 = *(int *)(ninst + (int)scene->gobjs);
      ninst = ninst + 4;
      *(float *)(iVar6 + 0x40) = *(float *)(iVar6 + 0x40) * *pfVar1;
    } while (numgobj != 0);
  }
  free_x(allc);
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


    NuDebugMsgProlog("..\\nu2.ps2\\nu3d\\nugscn.c", 0xC61)("Reading Animated texture set...");
    gsc->numtexanims = NuFileReadInt(fh);
    NuFileReadInt(fh);
    gsc->texanims = NuMemAllocFn(gsc->numtexanims * sizeof(struct nutexanim_s), "..\\nu2.ps2\\nu3d\\nugscn.c", 0xC68);
    NuFileRead(fh, gsc->texanims, gsc->numtexanims * sizeof(struct nutexanim_s));
    cnt = NuFileReadInt(fh) * 2;
    gsc->texanim_tids = (s16*)NuMemAllocFn(cnt, "..\\nu2.ps2\\nu3d\\nugscn.c", 0xC6D);
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

void ReadNuIFFGScene(fileHandle handle,struct nugscn_s *gscene)
{
  s32 iVar1;
  s8 *NTBL_;
  s32 currentBlkType;
  s32 tasFileOffset;
  struct nuscene_s nus;

  tasFileOffset = -1;
  memset(&nus,0,0x3c);
  do {
    currentBlkType = NuFileBeginBlkRead(handle,0);
    if (currentBlkType == 0) {
      NuSceneMtlUpdate(&nus);
      gscene->mtls = nus.mtls;
      gscene->nummtl = nus.nummtls;
      gscene->numgobj = nus.numgobjs;
      gscene->gobjs = nus.gobjs;
      gscene->numtid = nus.numtids;
      gscene->tids = nus.tids;
      NuSceneCalcCulling(gscene);
      if (tasFileOffset != -1) {
        NuFileSeek(handle,tasFileOffset,0);
        ReadNuIFFTexAnimSet(handle,gscene,nus.tids);
      }
      return;
    }
    if (currentBlkType == "0TST") {
      ReadNuIFFTextureSet(handle,&nus);
    }
    else if (currentBlkType < "0TSU") {
      if (currentBlkType == "0SAT") {
        tasFileOffset = NuFilePos(handle);
      }
      else if (currentBlkType < "0SAU") {
        if (currentBlkType == "00SM") {
          ReadNuIFFMaterialSet(handle,&nus);
        }
        else {
LAB_800b9478:
          ReadNuIFFUnknown(handle,currentBlkType);
        }
      }
      else if (currentBlkType == "0TSG") {
        ReadNuIFFGobjSet(handle,&nus);
      }
      else {
        if (currentBlkType != "0TSS") goto LAB_800b9478;
        //ReadNuIFFGSplineSet(handle,gscene);
      }
    }
    else if (currentBlkType == "CEPS") {
      ReadNuIFFSpecialObjects(handle,gscene);
    }
    else if (currentBlkType < "CEPT") {
      if (currentBlkType != "BILA") goto LAB_800b9478;
      ReadNuIFFAnimationLibrary(handle,gscene);
    }
    else if (currentBlkType == "LBTN") {
      NTBL_ = ReadNuIFFNameTable(handle);
      gscene->nametable = NTBL_;
    }
    else {
      if (currentBlkType != "TSNI") goto LAB_800b9478;
      iVar1 = ReadNuIFFInstSet(handle,&gscene->instances,&gscene->instanimblock);
      gscene->numinstance = iVar1;
    }
    NuFileEndBlkRead(handle);
  } while( true );
}

struct nuscene_s * NuSceneLoad(s8 *filename)
{
  s32 len;
  fileHandle handle;
  s32 index;
  struct nugscn_s *gscn;
  s32 i;
  struct nugeom_s *list_item;
  struct nugobj_s *gobj;
  struct nuscene_s *scene;
  s8 *filen;
  struct nuscene_s nus;
  struct nugeom_s **g;

  filen = (s8 *)&nus.names;
  strcpy(filen,filename);
  scene = NULL;
  len = strlen((s8 *)&nus.names);
  filen[len + -1] = 'x';
  strcpy(filen,filename);
  handle = NuFileOpen(filen,NUFILE_READ);
  if (handle == 0) {
    strlwr(filename);
  }
  else {
    strlwr(filename);
    scene = (struct nuscene_s *)NuMemAlloc(0x3c);
    memset(scene,0,0x3c);
    len = blkcnt;
    index = NuFileBeginBlkRead(handle,0);
    if (index == "0CSG") {
      gscn = (struct nugscn_s *)NuMemAlloc(0x74);
      scene->gscene = gscn;
      memset(gscn,0,0x74);
      ReadNuIFFGScene(handle,scene->gscene);
      gscn = scene->gscene;
      index = 0;
      if (0 < gscn->numinstance) {
        do {
          i = index + 1;
          for (list_item = gscn->gobjs[gscn->instances[index].objid]->geom;
              list_item != NULL; list_item = list_item->next) {
          }
          index = i;
        } while (i < gscn->numinstance);
      }
      //NuSceneAverageTextureSpaceVerts(scene);     //todo
      gscn = scene->gscene;
      index = 0;
      if (0 < gscn->numgobj) {
        do {
          gobj = gscn->gobjs[index];
          index = index + 1;
          while (gobj != NULL) {
            g = &gobj->geom;
            gobj = gobj->next_gobj;
            for (list_item = *g; list_item != NULL; list_item = list_item->next) {
            }
          }
        } while (index < gscn->numgobj);
      }
      NuFileEndBlkRead(handle);
      NuFileClose(handle);
      if (len != blkcnt) {
        blkcnt = len;
      }
    }
    else {
      NuMemFree(scene);
      //e = NuErrorProlog("C:/source/crashwoc/code/nu3dx/nuscene.c",0x718);
      //(*e)("Baaaaad file <%s>",filename);
      scene = NULL;
    }
  }
  return scene;
}


s32 ReadNuIFFNameTable(fileHandle handle)
{
    s32 size = NuFileReadInt(handle);
    return NuFileRead(handle, NuMemAlloc(size), size);
}


void NuGSceneDestroy(struct nugscn_s *gsc)

{
  struct nuAnimData_s **ianm;
  s32 i;
  s32 j;

  for (i = 0; i < gsc->numtid; i = i + 1) {
    NuTexDestroy((s32)gsc->tids[i]);
  }
  i = 0;
  if (0 < gsc->nummtl) {
    do {
      j = i + 1;
      NuMtlDestroy(gsc->mtls[i]);
      i = j;
    } while (j < gsc->nummtl);
  }
  i = 0;
  if (0 < gsc->numgobj) {
    do {
      j = i + 1;
      NuGobjDestroy(gsc->gobjs[i]);
      i = j;
    } while (j < gsc->numgobj);
  }
  ianm = gsc->instanimdata;
  if ((ianm != NULL) && (i = 0, 0 < gsc->numinstanims)) {
    while( true ) {
      //NuAnimDataDestroy(ianm[i]);     //finish nuanim
      if (gsc->numinstanims <= i + 1) break;
      ianm = gsc->instanimdata;
      i = i + 1;
    }
  }
  return;
}


void NuSceneDestroy(struct nuscene_s *sc)

{
  s32 i;
  s32 j;

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
    i = 0;
    if (0 < sc->numtids) {
      do {
        j = i + 1;
        NuTexDestroy((int)sc->tids[i]);
        i = j;
      } while (j < sc->numtids);
    }
    i = 0;
    if (0 < sc->nummtls) {
      do {
        j = i + 1;
        NuMtlDestroy(sc->mtls[i]);
        i = j;
      } while (j < sc->nummtls);
    }
    i = 0;
    if (0 < sc->numgobjs) {
      do {
        j = i + 1;
        NuGobjDestroy(sc->gobjs[i]);
        i = j;
      } while (j < sc->numgobjs);
    }
    if (sc->gscene != NULL) {
      NuGSceneDestroy(sc->gscene);
    }
  }
  return;
}
