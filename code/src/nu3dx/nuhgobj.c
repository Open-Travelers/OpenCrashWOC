#include "nuhgobj.h"
#include <nu.h>


struct numtx_s mtx_arrayHGobj[256];

//MATCH GCN
static struct NUHGOBJ_s* NuHGobjCreate(void) {
    struct NUHGOBJ_s *hgobj = (struct NUHGOBJ_s *)NuMemAlloc(sizeof(struct NUHGOBJ_s));
    memset(hgobj, 0, sizeof(struct NUHGOBJ_s));
    hgobj->tbrdist = 1.0f;

    return hgobj;
}

//MATCH GCN
struct NUHGOBJ_s* NuHGobjLoad(char* file) {
    struct NUHGOBJ_s *hgobj = NULL;
    s32 fh = NuFileOpen(file, NUFILE_READ);

    if (fh != 0)
    {
        NuFileBeginBlkRead(fh, 'FOGH');
        hgobj = NuHGobjCreate();
        ReadNuIFFHGobj(fh, hgobj);
        NuFileEndBlkRead(fh);
        NuFileClose(fh);
    }
    else
    {
        strlwr(file);
    }

    return hgobj;
}

//MATCH GCN
struct NUHGOBJ_s* NuHGobjRead(union variptr_u* opbuff, char* fname) {
    struct NUHGOBJ_s* hgobj;

    if (opbuff != NULL) {
        NuMemSetExternal(opbuff, NULL);
    }
    hgobj = NuHGobjLoad(fname);
    if (opbuff != NULL) {
        NuMemSetExternal(NULL, NULL);
    }
    return hgobj;
}


//MATCH GCN
static void NuHGobjDestroyDynamic(struct NUHGOBJ_s *hgobj) {
  s32 i;
  s32 j;

  if (hgobj->texanims != NULL) {
    NuTexAnimRemoveList(hgobj->texanims);
  }
  if ((hgobj->layers != NULL)) {
    for (i = 0; i < hgobj->num_layers; i++) {
      if (hgobj->layers[i].gobjs != NULL) {
        for (j = 0; j < (s32)hgobj->num_joints; j++) {
          if (hgobj->layers[i].gobjs[j] != NULL) {
            NuGobjDestroy(hgobj->layers[i].gobjs[j]);
          }
        }
      }
      if (hgobj->layers[i].blend_gobjs != NULL) {
        for (j = 0; j < (s32)hgobj->num_joints; j++) {
          if (hgobj->layers[i].blend_gobjs[j] != NULL) {
            NuGobjDestroy(hgobj->layers[i].blend_gobjs[j]);
          }
        }
      }
      if (hgobj->layers[i].skin_gobj != NULL) {
        NuGobjDestroy(hgobj->layers[i].skin_gobj);
      }
      if (hgobj->layers[i].blend_skin_gobj != NULL) {
        NuGobjDestroy(hgobj->layers[i].blend_skin_gobj);
      }
    }
  }
  if ((hgobj->mtls != NULL)) {
    for (i = 0; i < hgobj->nummtl; i++) {
      NuMtlDestroy(hgobj->mtls[i]);
    }
  }
  if ((hgobj->tids != NULL)) {
    for (i = 0; i < hgobj->numtid; i++) {
      NuTexDestroy(hgobj->tids[i]);
    }
  }
  if (hgobj->texanims != NULL) {
    NuTexAnimRemoveList(hgobj->texanims);
  }
  return;
}

//MATCH GCN
void NuHGobjDestroy(struct NUHGOBJ_s* hgobj) {
    if (hgobj != NULL) {
        NuHGobjDestroyDynamic(hgobj);
    }
    return;
}


//MATCH GCN
void NuHGobjPOIMtx(struct NUHGOBJ_s* hgobj, u8 poi_id, struct numtx_s* world_mtx, struct numtx_s* mtx_array, struct numtx_s* mtx) {

     NuMtxMul(mtx,&hgobj->points_of_interest[hgobj->poi_ixs[poi_id]].offset, mtx_array + hgobj->points_of_interest[hgobj->poi_ixs[poi_id]].parent_joint_ix);
     NuMtxMul(mtx,mtx,world_mtx);
     return;
}

//MATCH GCN
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

//MATCH GCN
static void ReadNuIFFHGobj(s32 handle, struct NUHGOBJ_s *hgobj) {
    s32 magic;
    s32 i;
    struct nuscene_s scene;
    struct nugscn_s gscn;


    memset(&scene, 0, sizeof(struct nuscene_s));
    memset(&gscn, 0, sizeof(struct nugscn_s));

    scene.numtids = 0;
    scene.tids = 0;
    scene.nummtls = 0;
    scene.mtls = NULL;

    while (magic = NuFileBeginBlkRead(handle, 0))
    {
        switch(magic)
        {
            case 0x4C42544E: //LBTN
                hgobj->string_table = ReadNuIFFNameTable(handle);
                break;
            case 0x30545354: //0TST
                ReadNuIFFTextureSet(handle, &scene);
				hgobj->numtid = scene.numtids;
				hgobj->tids = (s16 *)NuMemAlloc(scene.numtids << 2);
				for (i = 0; i < scene.numtids; i++) {
						hgobj->tids[i] = scene.tids[i];
				}
                break;
            case 0x3030534D: //00SM
                ReadNuIFFMaterialSet(handle, &scene);
                hgobj->mtls = scene.mtls;
                hgobj->nummtl = scene.nummtls;
                gscn.mtls = scene.mtls;
                gscn.nummtl = scene.nummtls;
                break;
            case 0x304F4748: //0OGH
                ReadNuIFFHGobjSet(handle, hgobj);
                break;
            case 0x30534154: //0SAT
                gscn.nametable = hgobj->string_table;
                ReadNuIFFTexAnimSet(handle, &gscn, scene.tids);
                hgobj->texanim_tids = gscn.texanim_tids;
                hgobj->texanims = gscn.texanims;
                hgobj->numtexanims = gscn.numtexanims;
                break;
            default:
                ReadNuIFFUnknown(handle, magic);
                break;
        }
        NuFileEndBlkRead(handle);
    }
    NuSceneMtlUpdate(&scene);
    scene.tids = 0;
	return;
}

//MATCH GCN
static void ReadNuIFFHGobjSet(s32 fh, struct NUHGOBJ_s* hgobj) {
    s32 i;
    s32 j;
    s32 str_ix;
    s32 nshadow_data;
    s32 bytes;
    struct NUSHADOWDATA_s* shadow_data;

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
                            hgobj->layers[i].gobjs[j] = ReadNuIFFGeom(fh, hgobj->mtls);
                        }
                    }
                }
                if (NuFileReadChar(fh) != 0) {
                    hgobj->layers[i].skin_gobj = ReadNuIFFGeom(fh, hgobj->mtls);
                }
                if (NuFileReadChar(fh) != 0) {
                    hgobj->layers[i].blend_gobjs = NuMemAlloc(hgobj->num_joints * 4);
                    memset(hgobj->layers[i].blend_gobjs, 0, hgobj->num_joints * 4);
                    for (j = 0; j < hgobj->num_joints; j++) {
                        if (NuFileReadChar(fh) != 0) {
                            hgobj->layers[i].blend_gobjs[j] = ReadNuIFFGeom(fh, hgobj->mtls);
                        }
                    }
                }
                if (NuFileReadChar(fh) != 0) {
                    hgobj->layers[i].blend_skin_gobj = ReadNuIFFGeom(fh, hgobj->mtls);
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
                shadow_data = &hgobj->shadow_data[i];

                shadow_data->ellipsoids = NULL;
                shadow_data->shadow_meshes = NULL;
                shadow_data->cylinders = NULL;
                shadow_data->nellipsoids = NuFileReadChar(fh);
                if (shadow_data->nellipsoids != 0) {
                    bytes = shadow_data->nellipsoids * sizeof(struct NUELLIPSOID_s);
                    shadow_data->ellipsoids = NuMemAlloc(bytes);
                    if (shadow_data->ellipsoids == NULL) {
                        NuErrorProlog("C:/source/crashwoc/code/nu3dx/nuhgobj.c",0x289)("failed to alloc shadow ellipsoids");
                    }
                    NuFileRead(fh, shadow_data->ellipsoids, bytes);
                }
                shadow_data->ncylinders = NuFileReadChar(fh);
                if (shadow_data->ncylinders != 0) {
                    bytes = shadow_data->ncylinders * sizeof(struct NUCYLINDER_s);
                    shadow_data->cylinders = NuMemAlloc(bytes);
                    if (shadow_data->cylinders == NULL) {
                        NuErrorProlog("C:/source/crashwoc/code/nu3dx/nuhgobj.c", 0x294)("failed to alloc shadow cylinders");
                    }
                    NuFileRead(fh, shadow_data->cylinders, bytes);
                }
                shadow_data->nshadow_meshes = NuFileReadChar(fh);
                if (shadow_data->nshadow_meshes != 0) {
                    bytes = shadow_data->nshadow_meshes * sizeof(struct NUSHADOWMESH_s);
                    shadow_data->shadow_meshes = NuMemAlloc(bytes);
                    if (shadow_data->shadow_meshes == NULL) {
                        NuErrorProlog("C:/source/crashwoc/code/nu3dx/nuhgobj.c", 0x29F)("failed to alloc shadow_meshes");
                    }
                    memset(shadow_data->shadow_meshes, 0, bytes);
                    for (j = 0; j < shadow_data->nshadow_meshes; j++) {
                        bytes = NuFileReadInt(fh) * sizeof(struct nuvec4_s);
                        shadow_data->shadow_meshes[j].normals = NuMemAlloc(bytes);
                        NuFileRead(fh, shadow_data->shadow_meshes[j].normals, bytes);

                        bytes = NuFileReadInt(fh) * sizeof(struct nuvec4_s);
                        shadow_data->shadow_meshes[j].verts = NuMemAlloc(bytes);
                        NuFileRead(fh, shadow_data->shadow_meshes[j].verts, bytes);
                    }
                }
                shadow_data->joint = NuFileReadChar(fh);
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
    if ((hgobj->min.x == 0.0f) && (hgobj->min.y == 0.0f) && (hgobj->min.z == 0.0f)) {
        hgobj->min.z = -1.0f;
        hgobj->min.y = -1.0f;
        hgobj->min.x = -1.0f;
    }
    if ((hgobj->max.x == 0.0f) && (hgobj->max.y == 0.0f) && (hgobj->max.z == 0.0f)) {
        hgobj->max.z = 1.0f;
        hgobj->max.y = 1.0f;
        hgobj->max.x = 1.0f;
    }
}
