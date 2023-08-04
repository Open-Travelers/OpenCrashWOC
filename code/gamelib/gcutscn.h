#ifndef NUGCUT_H
#define NUGCUT_H

#include "../types.h"
#include "../nu.h"

typedef struct NUCUTSCENE_s NUCUTSCENE_s, *PNUCUTSCENE_s;

typedef struct NUCUTSCENECAMERA_s NUCUTSCENECAMERA_s, *PNUCUTSCENECAMERA_s;

typedef struct NUHINSTANCE_s NUHINSTANCE_s, *PNUHINSTANCE_s;

typedef struct NUCUTSCENEAUDIO_s NUCUTSCENEAUDIO_s, *PNUCUTSCENEAUDIO_s;

struct NUCUTSCENE_s { /* 0x1f13f */
    int * tids;
    int numtid;
    struct numtl_s * mtls;
    int nummtl;
    float length;
    char * string_table;
    struct NUCUTSCENECAMERA_s * cameras[10];
    struct NuAnimData_s * camera_anim;
    float * camera_change_times;
    uchar * camera_changes;
    int numgobj;
    struct nugobj_s * gobjs;
    int numinstance;
    struct nuinstance_s * instances;
    struct NuAnimData_s * instance_animdata;
    int numhgobj;
    struct NUHGOBJ_s * hgobj;
    int numhinstance;
    struct NUHINSTANCE_s * hinstances;
    struct NUCUTSCENEAUDIO_s * audio;
    float current_time;
    int flags;
    char audio_path[64];
    uchar num_camera_changes;
    uchar camera_change_ix;
    uchar camera_ix;
    uchar camera_anim_ix;
    uchar numaudio;
    uchar next_audioix;
    char pad[2];
};

struct NUHINSTANCE_s {
    struct numtx_s mtx;
    struct NUHGOBJ_s * hgobj;
    struct NuAnimData_s * animdata;
    struct NuAnimData_s * blendweightanimdata;
    char pad[4];
};

struct NUCUTSCENEAUDIO_s {
    float start_time;
    char * wav_time;
};

struct NUCUTSCENECAMERA_s {
    struct nucamera_s * camera;
};

typedef void(*NuCutSceneCharacterCreateData)(struct NUGCUTCHAR_s*, struct instNUGCUTCHAR_s*, union variptr_u*);


#endif // !NUGCUT_H