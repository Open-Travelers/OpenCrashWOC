#ifndef NUANIM_H
#define NUANIM_H

#include "../types.h"
#include "nu3dxtypes.h"

/*
  800a970c 00005c 800a970c  4 buildBitCountTable 	Global
  800a9768 0002c0 800a9768  4 NuAnimDataRead 	Global
  800a9a28 000098 800a9a28  4 NuAnimDataChunkCreate 	Global
  800a9ac0 00006c 800a9ac0  4 NuAnimDataCreate 	Global
  800a9b2c 00006c 800a9b2c  4 NuAnimDataDestroy 	Global
  800a9b98 00008c 800a9b98  4 NuAnimDataChunkDestroy 	Global
  800a9c24 00013c 800a9c24  4 NuAnimDataCalcTime 	Global
  800a9d60 00013c 800a9d60  4 NuAnimData2CalcTime 	Global
  800a9e9c 000098 800a9e9c  4 NuAnimCurveCreate 	Global
  800a9f34 000040 800a9f34  4 NuAnimCurveDestroy 	Global
  800a9f74 000160 800a9f74  4 NuAnimCurveCalcVal2 	Global
  800aa0d4 000090 800aa0d4  4 NuAnimCurveSetCreate 	Global
  800aa164 000084 800aa164  4 NuAnimCurveSetDestroy 	Global
  800aa1e8 000534 800aa1e8  4 NuAnimCurve2SetApplyToJoint 	Global
  800aa71c 000308 800aa71c  4 NuAnimCurve2SetApplyToJointBasic 	Global
  800aaa24 0005f8 800aaa24  4 NuAnimCurveSetApplyToJoint2 	Global
  800ab01c 00034c 800ab01c  4 NuAnimCurveSetApplyToJointBasic 	Global
  800ab368 0007fc 800ab368  4 NuAnimCurveSetApplyBlendToJoint2 	Global
  800abb64 00029c 800abb64  4 NuAnimCurveSetApplyToMatrix 	Global
  800abe00 0001f8 800abe00  4 NuAnimDataFixPtrs 	Global
  800abff8 0000a4 800abff8  4 NuAnimDataLoadBuff 	Global
  800ac09c 000004 800ac09c  4 okdokey 	Global
  800ac0a0 000338 800ac0a0  4 NuAnimCurve2CalcVal 	Global
  800ac3d8 000144 800ac3d8  4 NuAnimData2FixPtrs 	Global
  800ac51c 0001ec 800ac51c  4 NuAnimCurve2SetApplyToMatrix 	Global
*/


struct nuanimdata_s* NuAnimDataRead(s32 fh);


// Size: 0x10
struct nuanimcurveset_s
{
    s32 flags;
    f32* constants;
    struct nuanimcurve_s** set;
    char ncurves;
    char pad[3];
};


// Size: 0x10
struct nuanimcurve_s
{
    u32 mask;
    struct nuanimkey_s* animkeys;
    u32 numkeys;
    u32 flags;
};


// Size: 0x4
struct nuanimcurve2_s
{
    // Size: 0x4
    union
    {
        float constant; // Offset: 0x0, DWARF: 0x1D80C
        struct nuanimcurvedata_s* curvedata; // Offset: 0x0, DWARF: 0x1D837
    } data;
};

// Size: 0x10
struct nuanimkey_s
{
    f32 time;
    f32 dtime;
    f32 c;
    f32 d;
};


// DWARF: 0x20F6A
enum NUANIMKEYTYPES_e
{
    NUANIMKEYTYPE_BOOLEAN = 4,
    NUANIMKEYTYPE_INTEGER = 3,
    NUANIMKEYTYPE_SMALL = 2,
    NUANIMKEYTYPE_BIG = 1,
    NUANIMKEYTYPE_NONE = 0
};


// Size: 0x4
struct NUANIMKEYSMALL_s
{
    s16 val;
    char grad;
    char time;
};


// Size: 0x8
struct NUANIMKEYINTEGER_s
{
    f32 val;
    f32 time;
};


// Size: 0x10
struct NUANIMKEYBIG_s
{
    f32 time;
    f32 dtime;
    f32 val;
    f32 grad;
};

// Size: 0xC
struct NUANIMDATAHDR_s
{
    s32 version;
    s32 address_offset;
    struct nuanimdata_s* animdata;
};

// Size: 0x14, DWARF: 0x1D4D6
struct nuanim_s
{
    struct nuanimdata_s* animdata;
    struct NUNODE_s* node;
    s32* prev_keys;
    s32 prev_key_ix;
    struct nuanimkey_s** prev_anim_key;
};


enum nuanimcomponents_e
{
    NUANIM_NUMMTXOPERATIONS = 9,
    NUANIM_Z_SCALE = 8,
    NUANIM_Y_SCALE = 7,
    NUANIM_X_SCALE = 6,
    NUANIM_Z_ROTATION = 5,
    NUANIM_Y_ROTATION = 4,
    NUANIM_X_ROTATION = 3,
    NUANIM_Z_TRANSLATION = 2,
    NUANIM_Y_TRANSLATION = 1,
    NUANIM_X_TRANSLATION = 0
};

enum nuanimmode_e
{
    NUANIM_NUMMODES = 3,
    NUANIM_PLAYBACK_OSCILLATE = 2,
    NUANIM_PLAYBACK_LOOP = 1,
    NUANIM_PLAYBACK_ONCE = 0
};

// Size: 0xC
struct nuanimcurvedata_s
{
    u32* mask;
    u16* key_ixs;
    void* key_array;
};


// Size: 0x18, DWARF: 0x1D88D
struct nuanimdata2_s
{
    f32 endframe;
    s16 nnodes;
    s16 ncurves;
    s16 nchunks;
    s16 pad;
    struct nuanimcurve2_s* curves;
    char* curveflags;
    char* curvesetflags;
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

// Size: 0x14
struct nuanimtime_s
{
    f32 time;
    f32 time_offset;
    s32 chunk;
    u32 time_mask;
    u32 time_byte;
};

#endif // !NUANIM_H
