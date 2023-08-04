#ifndef NU3DXTYPES_H
#define NU3DXTYPES_H

#include "types.h"
#include<string.h>
#include"system/gxtype.h"


s32 initialised;
s32 force_decal;
s32 force_glass_screencopy_enable;


enum nucamfxmode_s
{
    NUCAMFX_REFLECT = 1,
    NUCAMFX_NONE = 0
};


enum gobjtype_s
{
	NUGOBJ_FACEON = 1,
	NUGOBJ_MESH = 0
};

// Texture format
enum nutextype_e
{
	NUTEX_NUMTEXTURETYPES = 26,
	NUTEX_RZT16 = 25,
	NUTEX_RZT24 = 24,
	NUTEX_RZT32 = 23,
	NUTEX_RTT16 = 22,
	NUTEX_RTT24 = 21,
	NUTEX_RTT32 = 20,
	NUTEX_RT16 = 19,
	NUTEX_RT24 = 18,
	NUTEX_RT32 = 17,
	NUTEX_RT8H = 16,
	NUTEX_RTT8 = 15,
	NUTEX_RT8 = 14,
	NUTEX_BB = 13,
	NUTEX_DXT5 = 12,
	NUTEX_DXT4 = 11,
	NUTEX_DXT3 = 10,
	NUTEX_DXT2 = 9,
	NUTEX_DXT1 = 8,
	NUTEX_PAL8_S = 7,
	NUTEX_PAL4_S = 6,
	NUTEX_PAL8 = 5,
	NUTEX_PAL4 = 4,
	NUTEX_RGBA32 = 3,
	NUTEX_RGB24 = 2,
	NUTEX_RGBA16 = 1,
	NUTEX_RGB16 = 0
};

enum gs_texturetype_e
{
	GS_TEX_NUMTEXTURETYPES = 26,
	GS_TEX_RZT16 = 25,
	GS_TEX_RZT24 = 24,
	GS_TEX_RZT32 = 23,
	GS_TEX_RTT16 = 22,
	GS_TEX_RTT24 = 21,
	GS_TEX_RTT32 = 20,
	GS_TEX_RT16 = 19,
	GS_TEX_RT24 = 18,
	GS_TEX_RT32 = 17,
	GS_TEX_RT8H = 16,
	GS_TEX_RTT8 = 15,
	GS_TEX_RT8 = 14,
	GS_TEX_BB = 13,
	GS_TEX_DXT5 = 12,
	GS_TEX_DXT4 = 11,
	GS_TEX_DXT3 = 10,
	GS_TEX_DXT2 = 9,
	GS_TEX_DXT1 = 8,
	GS_TEX_PAL8_S = 7,
	GS_TEX_PAL4_S = 6,
	GS_TEX_PAL8 = 5,
	GS_TEX_PAL4 = 4,
	GS_TEX_RGBA32 = 3,
	GS_TEX_RGB24 = 2,
	GS_TEX_RGBA16 = 1,
	GS_TEX_RGB16 = 0
};

//Size: 0x4
enum nurndritemtype_s
{
	NURNDRITEM_GEOMFACE = 3,
	NURNDRITEM_SKIN3D2 = 2,
	NURNDRITEM_GEOM2D = 1,
	NURNDRITEM_GEOM3D = 0
};


// Size: 0x10
// Render data.
struct nurndritem_s
{
	struct nurndritem_s* next;
	enum nurndritemtype_s type;
	s32 flags;
	s16 lights_index;
};

// Size: 0xC
struct nucolour3_s
{
	f32 r;
	f32 g;
	f32 b;
};

// Camera.
// Size: 0x60
struct nucamera_s
{
	struct numtx_s mtx;
	f32 fov;
	f32 aspect;
	f32 nearclip;
	f32 farclip;
	f32 portalnearclip;
	struct nuvec_s scale;
};


struct nureflect_s {
    float distortion;
    struct numtx_s mtx;
    struct numtx_s uvmtx;
};

typedef struct CamMtx CamMtx, *PCamMtx;

struct CamMtx {
    struct numtx_s m;
    struct numtx_s m0;
    struct nuvec_s vX;
    struct nuvec_s vY;
    struct nuvec_s vZ;
    struct nuvec_s pos;
    struct nuvec_s oldpos;
    struct nuvec_s newpos;
    struct nuvec_s pos2;
    struct nuvec_s seek;
    float distance;
    float ahead;
    float judder;
    float blend_time;
    float blend_duration;
    int xrot;
    int yrot;
    int zrot;
    u16 old_xrot;
    u16 new_xrot;
    u16 old_yrot;
    u16 new_yrot;
    u16 old_zrot;
    u16 new_zrot;
    u16 hdg_to_player;
    short route;
    short i0;
    short i1;
    char mode;
    char iRAIL;
    short iALONG;
    u16 angle;
    char vertical;
    char pad1;
};

// Size: 0x18
struct D3DSurface
{
	unsigned int Common;
	unsigned int Data;
	unsigned int Lock;
	unsigned int Format;
	unsigned int Size;
	struct D3DBaseTexture* Parent;
};

// Size: 0x14
struct D3DBaseTexture
{
	unsigned int Common;
	unsigned int Data;
	unsigned int Lock;
	unsigned int Format;
	unsigned int Size;
};

// Size: 0x1C
//Texture.
struct nutex_s
{
	enum nutextype_e type;
	int width;
	int height;
	int mmcnt;
	void* bits;
	int* pal;
	short decal;
	short linear;
};

// Size: 0x28
struct nusystex_s
{
	struct nutex_s tex;
	struct D3DTexture* dds;
	int ref;
	int rtid;
};


// Size: 0x4
union numtlattrib_s
{
    struct {
	unsigned int alpha : 2; // Offset: 0x0, Bit Offset: 0, Bit Size: 2
	unsigned int filter : 2; // Offset: 0x0, Bit Offset: 2, Bit Size: 2
	unsigned int fx : 2; // Offset: 0x0, Bit Offset: 4, Bit Size: 2
	unsigned int utc : 2; // Offset: 0x0, Bit Offset: 6, Bit Size: 2
	unsigned int vtc : 2; // Offset: 0x0, Bit Offset: 8, Bit Size: 2
	unsigned int cull : 2; // Offset: 0x0, Bit Offset: 10, Bit Size: 2
	unsigned int zmode : 2; // Offset: 0x0, Bit Offset: 12, Bit Size: 2
	unsigned int lighting : 2; // Offset: 0x0, Bit Offset: 14, Bit Size: 2
	unsigned int colour : 1; // Offset: 0x0, Bit Offset: 16, Bit Size: 1
	unsigned int fill : 1; // Offset: 0x0, Bit Offset: 17, Bit Size: 1
	unsigned int atst : 3; // Offset: 0x0, Bit Offset: 18, Bit Size: 3
	unsigned int aref : 8; // Offset: 0x0, Bit Offset: 21, Bit Size: 8
	unsigned int afail : 2; // Offset: 0x0, Bit Offset: 29, Bit Size: 2
	unsigned int uvmode : 1; // Offset: 0x0, Bit Offset: 31, Bit Size: 1
    } _bitfld;
    u32 _word;
};

// Size: 0x4
union nufx_u
{
	void* voidptr;
	unsigned char u8;
	char s8;
	unsigned short u16;
	short s16;
	unsigned int u32;
	int s32;
	float f32;
};



enum nuvtxtype_e //s32
{
    NUVT_PS = 0x11,
    NUVT_LC1 = 0x51,
    NUVT_TLTC1 = 0x53,
    NUVT_SK3TC1 = 0x5d,
    NUVT_TC1 = 0x59
};

enum nufaceontype_s
{
    NUFACEON_FACEY = 0x1,
    NUFACEON_FACEON = 0x0
};


enum nuprimtype_e
{
    NUPT_QUADLIST = 0xa,
    NUPT_FACEON = 0x9,
    NUPT_BEZTRI = 0x8,
    NUPT_BEZPATCH = 0x7,
    NUPT_NDXTRISTRIP = 0x6,
    NUPT_NDXTRI = 0x5,
    NUPT_NDXLINE = 0x4,
    NUPT_TRISTRIP = 0x3,
    NUPT_TRI = 0x2,
    NUPT_LINE = 0x1,
    NUPT_POINT = 0x0
};


// Size: 0x1C, defined as 0x30.. why?
typedef struct nufaceongeom_s
{
    struct nufaceongeom_s* next;
    struct numtl_s* mtl;
    int mtl_id;
    enum nufaceontype_s faceon_type;
    struct nufaceon_s* faceons;
    int nfaceons;
    float faceon_radius;
};

// Size: 0x24
struct nugeomitem_s
{
    struct nurndritem_s hdr;
    struct numtx_s* mtx;
    struct nugeom_s* geom;
    f32** blendvals;
    u16 instancelights_index[3];
    u16 hShader;
};

// Size: 0x18
struct nufaceon_s
{
    struct nuvec_s point;
    float width;
    float height;
    int colour;
};

// Size: 0x40
struct NUBLENDGEOM_s
{
    int nblends;
    struct nuvec_s** blend_offsets;
    int* ix;
    struct nuvec_s* offsets;
    struct nuvec_s* ooffsets;
    int hVB; //VertexBuffer, GS_Buffer * ?
    int blendindex[10];
};

// Size: 0x18
struct nuskin_s
{
    struct nuskin_s* next;
    int vtxoffset;
    int vtxcnt;
    int mtxcnt;
    int* mtxid;
    float* weights;
};


// Size: 0x10
struct NUVTXSKININFO_s
{
    float wts[3];
    unsigned char joint_ixs[3];
    unsigned char pad;
};



// Size: 0x30
struct nugeom_s
{
    struct nugeom_s* next;
    struct numtl_s* mtl;
    s32 mtl_id;
    enum nuvtxtype_e vtxtype; //Size: 0x4
    s32 vtxcnt;
    s32 vtxmax;
    s32 hVB;  //GS_Buffer*
    void* basisvbptr;
    struct nuprim_s* prim;
    struct nuskin_s* skin;
    struct NUVTXSKININFO_s* vtxskininfo;
    struct NUBLENDGEOM_s* blendgeom;
};

// Size: 0x64
struct nugobj_s
{
    struct nugobj_s* sysnext;
    struct nugobj_s* syslast;
    enum gobjtype_s type; //Size: 0x4
    struct nugeom_s* geom;
    struct nufaceongeom_s* faceon_geom;
    float bounding_radius_from_origin;
    float bounding_rsq_from_origin;
    struct nuvec_s bounding_box_min;
    struct nuvec_s bounding_box_max;
    struct nuvec_s bounding_box_center;
    float bounding_radius_from_center;
    float bounding_rsq_from_center;
    int ngobjs;
    struct nugobj_s* next_gobj;
    struct nuvec_s origin;
    int ignore;
    int culltype;
};


// Size: 0x3C
struct nuprim_s
{
    struct nuprim_s* next;
    enum nuprimtype_e type; //Size: 0x4
    unsigned short vertexCount;
    unsigned short max;
    unsigned short* vid;
    struct nuplane_s* pln;
    int idxbuff;  //GS_Buffer
    int cached;
    short skinmtxlookup[16];
};

// Size: 0x10
struct nuplane_s
{
    float a;
    float b;
    float c;
    float d;
};

// Size: 0x54
struct numtl_s
{
	struct numtl_s* next;
	union numtlattrib_s attrib;
	struct nucolour3_s ambient;
	struct nucolour3_s diffuse;
	union nufx_u fx1;
	union nufx_u fx2;
	union nufx_u fx3;
	union nufx_u fx4;
	f32 power;
	f32 alpha;
	s32 tid;
	s16 alpha_sort;
	u8 fxid;
	u8 special_id;
	s16 K;
	u8 L;
	u8 uanmmode : 4;
	u8 vanmmode : 4;
	f32 du;
	f32 dv;
	f32 su;
	f32 sv;
};

// Size: 0x6C
struct nusysmtl_s
{
	struct numtl_s mtl;
	struct nurndritem_s* rndrlist;
	struct nugeomitem_s* geom2d;
	struct nugeomitem_s* geom3d;
	struct nusysmtl_s* next;
	struct nusysmtl_s* last;
	s32 hShader;
};

struct nufaceonitem_s {
    struct nufaceonitem_s * next;
    struct nurndritem_s * hdr;
    struct nusysmtl_s * mtl;
};

// Size: 0x3C
struct nuscene_s
{
	int nnodes;
	char** names;
	struct NUNODE_s** nodes;
	char* strings;
	int numtids;
	short* tids;
	int nummtls;
	struct numtl_s** mtls;
	int numgobjs;
	struct nugobj_s** gobjs;
	int numsplines;
	struct nuspline_s* splines;
	struct nuvec_s* spline_cvs;
	struct NUNODE_s* root_node;
	struct nugscn_s* gscene;
};

// Size: 0x74
struct nugscn_s
{
	short* tids;
	int numtid;
	struct numtl_s** mtls;
	int nummtl;
	int numgobj;
	struct nugobj_s** gobjs;
	int numinstance;
	struct nuinstance_s* instances;
	int numspecial;
	struct nuspecial_s* specials;
	void* splinedata;
	int numsplines;
	struct nugspline_s* splines;
	char* nametable;
	int numexspecials;
	struct nuspecial_s* exspecials;
	int exallocix;
	struct nuinstanim_s* instanimblock;
	struct nuanimdata_s** instanimdata;
	int numinstanims;
	int numinstanceixs;
	short* instanceixs;
	short crossfade;
	short crossfaderate;
	int crossfadegobj;
	int crossfadefirst;
	int numtexanims;
	struct nutexanim_s* texanims;
	short* texanim_tids;
	short* instancelightix;
};

// Size: 0xC
struct nugspline_s
{
	short len;
	short ptsize;
	char* name;
	char* pts;
};

// Size: 0x40
struct nuspline_s
{
	char* name;
	int ncvs;
	struct nuvec_s* cvs;
	float r;
	float r2;
	struct nuvec_s min;
	struct nuvec_s max;
	struct nuvec_s cntr;
	float cntr_r;
	float cntr_r2;
};


// Size: 0x20
struct NUNODE_s
{
	char* name;
	unsigned int id;
	unsigned int type;
	int nchildren;
	struct NUNODE_s* parent;
	struct NUNODE_s* child;
	struct NUNODE_s* next;
	void* data;
};

// Size: 0x4
struct nuinstflags_s
{
	int visible : 1; // Offset: 0x0, Bit Offset: 0, Bit Size: 1
	int onscreen : 1; // Offset: 0x0, Bit Offset: 1, Bit Size: 1
	int visitest : 1; // Offset: 0x0, Bit Offset: 2, Bit Size: 1
	int isanimated : 1; // Offset: 0x0, Bit Offset: 3, Bit Size: 1
};

// Size: 0x50
struct nuinstance_s
{
	struct numtx_s mtx;
	int objid;
	struct nuinstflags_s flags;
	struct nuinstanim_s* anim;
	short room_group;
	char special_flag;
	char pad[1];
};

// Size: 0x60
struct nuinstanim_s
{
	struct numtx_s mtx;
	float tfactor;
	float tfirst;
	float tinterval;
	float ltime;
	unsigned int playing : 1; // Offset: 0x50, Bit Offset: 0, Bit Size: 1
	unsigned int backwards : 1; // Offset: 0x50, Bit Offset: 1, Bit Size: 1
	unsigned int waiting : 1; // Offset: 0x50, Bit Offset: 2, Bit Size: 1
	unsigned int repeating : 1; // Offset: 0x50, Bit Offset: 3, Bit Size: 1
	unsigned int oscillate : 1; // Offset: 0x50, Bit Offset: 4, Bit Size: 1
	int ipad[2]; // Offset: 0x54
	unsigned char anim_ix;
	char pad[3];
};

// Size: 0x4
struct nuspecialflags
{
	int ext : 1; // Offset: 0x0, Bit Offset: 0, Bit Size: 1
	int ext_vis : 1; // Offset: 0x0, Bit Offset: 1, Bit Size: 1
	int ext_onscreen : 1; // Offset: 0x0, Bit Offset: 2, Bit Size: 1
};


// Size: 0x50
struct nuspecial_s
{
	struct numtx_s mtx;
	struct nuinstance_s* instance;
	char* name;
	struct nuspecialflags flags;
	int pad;
};


struct nufspecial_s {
    struct numtx_s mtx;
    int instanceix;
    int nameix;
    struct nuspecialflags flags;
    int pad;
};


struct NULAYERDATA_s {
    char * name;
    struct nugobj_s * * gobjs;
    struct nugobj_s * skin_gobj;
    struct nugobj_s * * blend_gobjs;
    struct nugobj_s * blend_skin_gobj;
};

struct NUPOINTOFINTEREST_s {
    struct numtx_s offset;
    char * name;
    u8 parent_joint_ix;
    u8 pad[11];
};

struct NUJOINTDATA_s {
    struct numtx_s orient;
    struct nuvec_s locator_offset;
    char * name;
    int parent_ix;
    char flags;
    char pad[11];
};


struct NUELLIPSOID_s {
    struct nuvec_s centre;
    struct nuvec_s y_axis;
    struct nuvec_s x_axis;
    struct nuvec_s z_axis;
};

struct NUCYLINDERS_s {
    struct nuvec_s centre;
    struct nuvec_s y_axis;
    struct Vec4 x_axis;
    struct Vec4 z_axis;
    char pad[8];
};

struct NUSHADOWMESH_s {
    struct Vec4 * normals;
    struct Vec4 * verts;
};

struct NUSHADOWDATA_s {
    struct NUELLIPSOID_s * ellipsoids;
    struct NUCYLINDERS_s * cylinders;
    struct NUSHADOWMESH_s * shadow_meshes;
    u8 nellipsoids;
    u8 ncylinders;
    u8 nshadow_meshes;
    u8 joint;
};



// Size: 0x10
struct nuanimdata_s
{
	float time;
	char* node_name;
	int nchunks;
	struct nuanimdatachunk_s** chunks;
};

// Size: 0x14
struct nuanimdatachunk_s
{
	int numnodes;
	int num_valid_animcurvesets;
	struct nuanimcurveset_s** animcurvesets;
	struct nuanimkey_s* keys;
	struct nuanimcurve_s* curves;
};

// Size: 0x10
struct nuanimcurveset_s
{
	int flags;
	float* constants;
	struct nuanimcurve_s** set;
	char ncurves;
	char pad[3];
};

// Size: 0x10
struct nuanimcurve_s
{
	unsigned int mask;
	struct nuanimkey_s* animkeys;
	unsigned int numkeys;
	unsigned int flags;
};

// Size: 0x10
struct nuanimkey_s
{
	float time;
	float dtime;
	float c;
	float d;
};

typedef struct Rail Rail, *PRail;

struct Rail {
    struct nugspline_s * pINPLAT;
    struct nugspline_s * pINCAM;
    struct nugspline_s * pLEFT;
    struct nugspline_s * pCAM;
    struct nugspline_s * pRIGHT;
    struct nugspline_s * pOUTCAM;
    struct nugspline_s * pOUTPLAT;
    float in_distance;
    float out_distance;
    short edges;
    char type;
    char circuit;
};

// Size: 0x1C
typedef struct {
    int next;
    int last;
    u32 width;
    u32 height;
    struct D3DSurface * pddsRenderTarget;
    struct D3DSurface * pddsZBuffer;
    struct D3DSurface * pddsStencilBuffer;
} rendertargetlist_s;

static rendertargetlist_s g_pRTArray[16];

typedef struct scene_inst_s scene_inst_s, *Pscene_inst_s;

struct scene_inst_s {
    char * name;
    struct nugscn_s * scene;
};

typedef struct sceneinst_s sceneinst_s, *Psceneinst_s;

struct sceneinst_s {
    struct nuscene_s * nus;
    char name[256];
    int inst_cnt;
};

typedef struct SplTab SplTab, *PSplTab;

struct SplTab {
    struct nugspline_s * spl;
    char * name;
    short min;
    short max;
    char unk[4];
    u64 levbits; /* unk_type */
};

typedef struct visidata_s visidata_s, *Pvisidata_s;

struct visidata_s {
    struct nugscn_s * sc;
    void * * * vspline; /* unk_type */
    int binfosize;
    int curspline;
    int curknot;
};


 // Size: 0x4C
struct _GS_TEXTURE
{
    s32 Flags;
    enum gs_texturetype_e Format;
    u32 Width;
    u32 Height;
    u32 TexBits;
    u32 PalBits;
    u32 NUID;
    u32 Pad;
    struct _GXTexObj Tex;
	//struct _GXTlutObj Tlut;   //Gamecube SDK
};

// Size: 0x24
struct _GS_VERTEX
{
    float x; // Offset: 0x0
    float y; // Offset: 0x4
    float z; // Offset: 0x8
    float nx; // Offset: 0xC
    float ny; // Offset: 0x10
    float nz; // Offset: 0x14
    u32 diffuse; // Offset: 0x18
    float u; // Offset: 0x1C
    float v; // Offset: 0x20
};

#endif // !NU3DXTYPES_H
