#include "nuhgobj.h"
#include <nu.h>


struct numtx_s mtx_arrayHGobj[256];

struct NUHGOBJ_s* NuHGobjCreate(void)
{
    struct NUHGOBJ_s *hgobj = (struct NUHGOBJ_s *)NuMemAlloc(sizeof(struct NUHGOBJ_s));
    memset(hgobj, 0, sizeof(struct NUHGOBJ_s));
    hgobj->tbrdist = 1.0f;

    return hgobj;
}

struct NUHGOBJ_s* NuHGobjLoad(const char* name)
{
    struct NUHGOBJ_s *hgobj = NULL;
    fileHandle handle = NuFileOpen(name, NUFILE_READ);

    if (handle != 0)
    {
        NuFileBeginBlkRead(handle, 'FOGH');
        struct NUHGOBJ_s* hgobj = NuHGobjCreate();
        ReadNuIFFHGobj(handle, hgobj);
        NuFileEndBlkRead(handle);
        NuFileClose(handle);
    }
    else
    {
        strlwr(name);
    }

    return hgobj;
}

struct NUHGOBJ_s* NuHGobjRead(union variptr_u* superbuf_ptr, char* name)

{
    struct NUHGOBJ_s* hgobj;

    if (superbuf_ptr != NULL) {
        NuMemSetExternal(superbuf_ptr, NULL);
    }
    hgobj = NuHGobjLoad(name);
    if (superbuf_ptr != NULL) {
        NuMemSetExternal(NULL, NULL);
    }
    return hgobj;
}

void NuHGobjDestroy(struct NUHGOBJ_s* hgobj)

{
    if (hgobj != NULL) {
        NuHGobjDestroyDynamic(hgobj);
    }
    return;
}

//need clean
void NuHGobjDestroyDynamic(struct NUHGOBJ_s *hgobj)
{
  struct NULAYERDATA_s *layers;
  u32 nJoints;
  struct numtl_s **mtls;
  s16 *tids;
  s32 i;
  s32 j;
  s32 k;
  struct nugobj_s **gobjs;

  if (hgobj->texanims != NULL) {
    //NuTexAnimRemoveList(hgobj->texanims);
  }
  layers = hgobj->layers;
  if ((layers != NULL) && (i = 0, hgobj->num_layers != '\0')) {
    while( true ) {
      if (layers[i].gobjs != NULL) {
        nJoints = (u32)hgobj->num_joints;
        j = 0;
        while (j < (s32)nJoints) {
          k = j + 1;
          gobjs = layers[i].gobjs + j;
          j = k;
          if (*gobjs != NULL) {
            NuGobjDestroy(*gobjs);
            layers = hgobj->layers;
            nJoints = (u32)hgobj->num_joints;
          }
        }
      }
      if (layers[i].blend_gobjs != NULL) {
        nJoints = (u32)hgobj->num_joints;
        j = 0;
        while (j < (s32)nJoints) {
          k = j + 1;
          gobjs = layers[i].blend_gobjs + j;
          j = k;
          if (*gobjs != NULL) {
            NuGobjDestroy(*gobjs);
            layers = hgobj->layers;
            nJoints = (u32)hgobj->num_joints;
          }
        }
      }
      if (layers[i].skin_gobj != NULL) {
        NuGobjDestroy(layers[i].skin_gobj);
        layers = hgobj->layers;
      }
      if (layers[i].blend_skin_gobj != NULL) {
        NuGobjDestroy(layers[i].blend_skin_gobj);
      }
      i = i + 1;
      if ((s32)(u32)hgobj->num_layers <= i) break;
      layers = hgobj->layers;
    }
  }
  mtls = hgobj->mtls;
  if ((mtls != NULL) && (i = 0, 0 < hgobj->nummtl)) {
    while( true ) {
      NuMtlDestroy(mtls[i]);
      if (hgobj->nummtl <= i + 1) break;
      mtls = hgobj->mtls;
      i = i + 1;
    }
  }
  tids = hgobj->tids;
  if ((tids != NULL) && (i = 0, 0 < hgobj->numtid)) {
    while( true ) {
      NuTexDestroy((s32)tids[i]);
      if (hgobj->numtid <= i + 1) break;
      tids = hgobj->tids;
      i = i + 1;
    }
  }
  if (hgobj->texanims != NULL) {
    //NuTexAnimRemoveList(hgobj->texanims);
  }
  return;
}

//Check
void NuHGobjPOIMtx(struct NUHGOBJ_s* hgobj, u8 poi_id, struct numtx_s* world_mtx, struct numtx_s* mtx_array, struct numtx_s* mtx)

{

     NuMtxMul(mtx,&hgobj->points_of_interest[hgobj->poi_ixs[poi_id]].offset, mtx_array + hgobj->points_of_interest[hgobj->poi_ixs[poi_id]].parent_joint_ix);
     NuMtxMul(mtx,mtx,world_mtx);
     return;
}


struct NUPOINTOFINTEREST_s* NuHGobjGetPOI(struct NUHGOBJ_s* hgobj, u8 poi_id) {
    u8 cindex;

    if (poi_id < (u8) hgobj->num_poi_ixs) {
        cindex = hgobj->poi_ixs[poi_id];
        if ((s32) cindex != 0xFF) {
            return &hgobj->points_of_interest[cindex];
        }
    }
    return NULL;
}


void ReadNuIFFHGobj(fileHandle handle, struct NUHGOBJ_s *hgobj)
{
    u32 magic;
    struct nuscene_s scene;
    struct nugscn_s gscn;

    memset(&scene, 0, sizeof(struct nuscene_s));
    memset(&gscn, 0, sizeof(struct nugscn_s));

    scene.mtls = NULL;
    scene.numtids = 0;
    scene.tids = 0;
    scene.nummtls = 0;

    while ((magic = NuFileBeginBlkRead(handle, 0)) != 0)
    {
        switch(magic)
        {
            case '0SAT':
                gscn.nametable = hgobj->string_table;
                //ReadNuIFFTexAnimSet(handle, &gscn, scene.tids);
                hgobj->texanim_tids = gscn.texanim_tids;
                hgobj->texanims = gscn.texanims;
                hgobj->numtexanims = gscn.numtexanims;
                break;
            case '00SM':
                //ReadNuIFFMaterialSet(handle, &scene);
                gscn.mtls = scene.mtls;
                gscn.nummtl = scene.nummtls;
                hgobj->mtls = scene.mtls;
                hgobj->nummtl = scene.nummtls;
                break;
            case '0OGH':
                ReadNuIFFHGobjSet(handle, hgobj);
                break;
            case '0TST':
                //ReadNuIFFTextureSet(handle, &scene);
				hgobj->numtid = scene.numtids;
				s16 tids = (s16 *)NuMemAlloc(scene.numtids << 2);
				hgobj->tids = tids;
				s32 i = 0;
				if (scene.numtids > 0)
				{
				do {
						i++;
						hgobj->tids[i] = scene.tids[i];
				} while (i < scene.numtids);
				}
                break;
            case 'LBTN':
                //hgobj->string_table = ReadNuIFFNameTable(handle);
                break;
            default:
                //ReadNuIFFUnknown(handle, magic);
                break;
        }

        NuFileEndBlkRead(handle);
    }

    //NuSceneMtlUpdate(&scene);
	return;
}

void ReadNuIFFHGobjSet(fileHandle fh, struct NUHGOBJ_s* hgobj) {
    s32 i;
    s32 j;
    s32 str_ix;
    s32 nshadow_data;

    hgobj->num_joints = NuFileReadChar(fh);
    if (hgobj->num_joints != 0) {
        hgobj->joints = NuMemAlloc(hgobj->num_joints * sizeof(struct NUJOINTDATA_s));
        memset(hgobj->joints, 0, hgobj->num_joints * sizeof(struct NUJOINTDATA_s));
        hgobj->T = NuMemAlloc(hgobj->num_joints * sizeof(struct numtx_s));
        memset(hgobj->T, 0, hgobj->num_joints * sizeof(struct numtx_s));
        hgobj->INV_WT = NuMemAlloc(hgobj->num_joints * sizeof(struct numtx_s));
        memset(hgobj->INV_WT, 0, hgobj->num_joints * sizeof(struct numtx_s));
        for (i = 0; i < hgobj->num_joints; i++) {
            NuFileRead(fh, &hgobj->joints[i].flags, sizeof(char));
            NuFileRead(fh, &hgobj->T[i], sizeof(struct numtx_s));
            NuFileRead(fh, &hgobj->INV_WT[i], sizeof(struct numtx_s));
            NuFileRead(fh, &hgobj->joints[i].orient, sizeof(struct numtx_s));
            NuFileRead(fh, &hgobj->joints[i].locator_offset, sizeof(struct nuvec_s));
            hgobj->joints[i].parent_ix = NuFileReadChar(fh);
            str_ix = NuFileReadInt(fh);
            if (str_ix != 0) {
                hgobj->joints[i].name = &hgobj->string_table[str_ix-1];
            }
        }
        hgobj->num_joint_ixs = NuFileReadChar(fh);
        if (hgobj->num_joint_ixs != 0) {
            hgobj->joint_ixs = NuMemAlloc(hgobj->num_joint_ixs);
            NuFileRead(fh, hgobj->joint_ixs, (s32) hgobj->num_joint_ixs);
        }
        hgobj->num_poi_ixs = NuFileReadChar(fh);
        if (hgobj->num_poi_ixs != 0) {
            hgobj->poi_ixs = NuMemAlloc(hgobj->num_poi_ixs);
            NuFileRead(fh, hgobj->poi_ixs, (s32) hgobj->num_poi_ixs);
        }
        hgobj->num_layers = NuFileReadChar(fh);
        if (hgobj->num_layers != 0) {
            hgobj->layers = NuMemAlloc(hgobj->num_layers * sizeof(struct NULAYERDATA_s));
            memset(hgobj->layers, 0, hgobj->num_layers * sizeof(struct NULAYERDATA_s));
            for (i = 0; i < hgobj->num_layers; i++) {
                str_ix = NuFileReadInt(fh);
                if (str_ix != 0) {
                    hgobj->layers[i].name = &hgobj->string_table[str_ix-1];
                }
                if (NuFileReadChar(fh) != 0) {
                    hgobj->layers[i].gobjs = NuMemAlloc(hgobj->num_joints * 4);
                    memset(hgobj->layers[i].gobjs, 0, hgobj->num_joints * 4);
                    for (j = 0; j < hgobj->num_joints; j++) {
                        if (NuFileReadChar(fh) != 0) {
                            //hgobj->layers[i].gobjs[j] = ReadNuIFFGeom(fh, hgobj->mtls);
                        }
                    }
                }
                if (NuFileReadChar(fh) != 0) {
                    //hgobj->layers[i].skin_gobj = ReadNuIFFGeom(fh, hgobj->mtls);
                }
                if (NuFileReadChar(fh) != 0) {
                    hgobj->layers[i].blend_gobjs = NuMemAlloc(hgobj->num_joints * 4);
                    memset(hgobj->layers[i].blend_gobjs, 0, hgobj->num_joints * 4);
                    for (j = 0; j < hgobj->num_joints; j++) {
                        if (NuFileReadChar(fh) != 0) {
                            //hgobj->layers[i].blend_gobjs[j] = ReadNuIFFGeom(fh, hgobj->mtls);
                        }
                    }
                }
                if (NuFileReadChar(fh) != 0) {
                    //hgobj->layers[i].blend_skin_gobj = ReadNuIFFGeom(fh, hgobj->mtls);
                }
            }
        }
        hgobj->num_points_of_interest = NuFileReadChar(fh);
        if (hgobj->num_points_of_interest != 0) {
            hgobj->points_of_interest = NuMemAlloc(hgobj->num_points_of_interest * sizeof(struct NUPOINTOFINTEREST_s));
            memset(hgobj->points_of_interest, 0, hgobj->num_points_of_interest * sizeof(struct NUPOINTOFINTEREST_s));
            for (i = 0; i < hgobj->num_points_of_interest; i++) {
                hgobj->points_of_interest[i].parent_joint_ix = NuFileReadChar(fh);
                NuFileRead(fh, &hgobj->points_of_interest[i].offset, sizeof(struct numtx_s));
                str_ix = NuFileReadInt(fh);
                if (str_ix != 0) {
                    hgobj->points_of_interest[i].name = &hgobj->string_table[str_ix-1];
                }
            }
        }
        nshadow_data = NuFileReadChar(fh);
        if (nshadow_data != 0) {
            hgobj->shadow_data = NuMemAlloc((nshadow_data + 1) * sizeof(struct NUSHADOWDATA_s));
            if (hgobj->shadow_data == NULL) {
                NuErrorProlog("C:/source/crashwoc/code/nu3dx/nuhgobj.c", 0x274)("failed to alloc shadow data");
            }
            memset(hgobj->shadow_data, 0, (nshadow_data + 1) * sizeof(struct NUSHADOWDATA_s));
            hgobj->shadow_data[nshadow_data].joint = 0xFF;
            for (i = 0; i < nshadow_data; i++) {
                hgobj->shadow_data[i].ellipsoids = NULL;
                hgobj->shadow_data[i].shadow_meshes = (struct NUSHADOWMESH_s* ) NULL;
                hgobj->shadow_data[i].cylinders = (struct NUCYLINDER_s* ) NULL;
                hgobj->shadow_data[i].nellipsoids = NuFileReadChar(fh);
                if (hgobj->shadow_data[i].nellipsoids != 0) {
                    hgobj->shadow_data[i].ellipsoids = NuMemAlloc(hgobj->shadow_data[i].nellipsoids * sizeof(struct NUELLIPSOID_s));
                    if (hgobj->shadow_data[i].ellipsoids == NULL) {
                        NuErrorProlog("C:/source/crashwoc/code/nu3dx/nuhgobj.c", 0x289)("failed to alloc shadow ellipsoids");
                    }
                    NuFileRead(fh, hgobj->shadow_data[i].ellipsoids, hgobj->shadow_data[i].nellipsoids * sizeof(struct NUELLIPSOID_s));
                }
                hgobj->shadow_data[i].ncylinders = NuFileReadChar(fh);
                if (hgobj->shadow_data[i].ncylinders != 0) {
                    hgobj->shadow_data[i].cylinders = NuMemAlloc(hgobj->shadow_data[i].ncylinders << 6);
                    if (hgobj->shadow_data[i].cylinders == NULL) {
                        NuErrorProlog("C:/source/crashwoc/code/nu3dx/nuhgobj.c", 0x294)("failed to alloc shadow cylinders");
                    }
                    NuFileRead(fh, hgobj->shadow_data[i].cylinders, hgobj->shadow_data[i].ncylinders << 6);
                }
                hgobj->shadow_data[i].nshadow_meshes = NuFileReadChar(fh);
                if (hgobj->shadow_data[i].nshadow_meshes != 0) {
                    hgobj->shadow_data[i].shadow_meshes = NuMemAlloc(hgobj->shadow_data[i].nshadow_meshes * 8);
                    if (hgobj->shadow_data[i].shadow_meshes == NULL) {
                        NuErrorProlog("C:/source/crashwoc/code/nu3dx/nuhgobj.c", 0x29F)("failed to alloc shadow_meshes");
                    }
                    memset(hgobj->shadow_data[i].shadow_meshes, 0, hgobj->shadow_data[i].nshadow_meshes * 8);
                    for (j = 0; j < hgobj->shadow_data[i].nshadow_meshes; j++) {
                        {
                            s32 bytes = NuFileReadInt(fh) * sizeof(struct Vec4);
                            hgobj->shadow_data[i].shadow_meshes[j].normals = NuMemAlloc(bytes);
                            NuFileRead(fh, &hgobj->shadow_data[i].shadow_meshes[j].normals, bytes);
                        }
                        {
                            s32 bytes = NuFileReadInt(fh) * sizeof(struct Vec4);
                            hgobj->shadow_data[i].shadow_meshes[j].verts = NuMemAlloc(bytes);
                            NuFileRead(fh, hgobj->shadow_data[i].shadow_meshes[j].verts, bytes);
                        }
                    }
                }
                hgobj->shadow_data[i].joint = NuFileReadChar(fh);
            }
        }
    }
    hgobj->sphere_radius = NuFileReadFloat(fh);
    hgobj->sphere_yoff = NuFileReadFloat(fh);
    hgobj->min.x = NuFileReadFloat(fh);
    hgobj->min.y = NuFileReadFloat(fh);
    hgobj->min.z = NuFileReadFloat(fh);
    hgobj->max.x = NuFileReadFloat(fh);
    hgobj->max.y = NuFileReadFloat(fh);
    hgobj->max.z = NuFileReadFloat(fh);
    hgobj->cylinder_yoff = NuFileReadFloat(fh);
    hgobj->cylinder_height = NuFileReadFloat(fh);
    hgobj->cylinder_radius = NuFileReadFloat(fh);
    if ((hgobj->min.x == 0.0) && (hgobj->min.y == 0.0) && (hgobj->min.z == 0.0)) {
        hgobj->min.x = -1.0;
        hgobj->min.z = -1.0;
        hgobj->min.y = -1.0;
    }
    if ((hgobj->max.x == 0.0) && (hgobj->max.y == 0.0) && (hgobj->max.z == 0.0)) {
        hgobj->max.x = 1.0;
        hgobj->max.z = 1.0;
        hgobj->max.y = 1.0;
    }
}

