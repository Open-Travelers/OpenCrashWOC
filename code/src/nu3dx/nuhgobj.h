#ifndef NUHGOBJ_H
#define NUHGOBJ_H

#include "../types.h"
#include "nu3dxtypes.h"
#include "nu3dx/nuscene.h"

/*
  800b06f8 000050 800b06f8  4 NuHGobjCreate 	Global
  800b0748 0001e0 800b0748  4 NuHGobjDestroyDynamic 	Global
  800b0928 000028 800b0928  4 NuHGobjDestroy 	Global
  800b0950 000080 800b0950  4 NuHGobjLoad 	Global
  800b09d0 00006c 800b09d0  4 NuHGobjRead 	Global
  800b0a3c 0001d0 800b0a3c  4 ReadNuIFFHGobj 	Global
  800b0c0c 000834 800b0c0c  4 ReadNuIFFHGobjSet 	Global
  800b1440 000068 800b1440  4 NuHGobjPOIMtx 	Global
  800b14a8 000034 800b14a8  4 NuHGobjGetPOI 	Global
*/

// Size: 0x80
struct NUHGOBJ_s
{
    short* tids;
    int numtid;
    struct numtl_s** mtls;
    int nummtl;
    struct NUJOINTDATA_s* joints;
    struct numtx_s* T;
    struct numtx_s* INV_WT;
    unsigned char* joint_ixs;
    struct NULAYERDATA_s* layers;
    struct NUPOINTOFINTEREST_s* points_of_interest;
    unsigned char* poi_ixs;
    char* string_table;
    int string_table_size;
    float tbrdist;
    struct NUSHADOWDATA_s* shadow_data;
    float sphere_radius;
    float sphere_yoff;
    struct nuvec_s min;
    struct nuvec_s max;
    float cylinder_yoff;
    float cylinder_height;
    float cylinder_radius;
    struct NUCOLLISIONDATA_s* collision_data;
    int numtexanims;
    struct nutexanim_s* texanims;
    short* texanim_tids;
    unsigned char num_joints;
    unsigned char num_joint_ixs;
    unsigned char num_layers;
    unsigned char num_points_of_interest;
    unsigned char num_poi_ixs;
    unsigned char shadowoff;
    char pad[2];
};



static struct NUHGOBJ_s* NuHGobjCreate(void);
struct NUHGOBJ_s* NuHGobjLoad(char* file);
static void ReadNuIFFHGobj(s32 handle, struct NUHGOBJ_s *hgobj);
static void ReadNuIFFHGobjSet(s32 fh, struct NUHGOBJ_s* hgobj);


#endif // !NUHGOBJ_H
