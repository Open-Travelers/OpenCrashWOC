#include "edobj.h"
#include "../nu.h"

//NGC MATCH
s32 edobjRenderCutoffTest(struct nuvec_s *pos) {
  float x;
  float y;
  float z;
  
  x = pos->x - global_camera.mtx._30;
  y = pos->y - global_camera.mtx._31;
  z = pos->z - global_camera.mtx._32;
  if (NuFsqrt(x * x + y * y + z * z) > 80.0f) {
      return 0;
  }
  return 1;
}

//NGC MATCH
void edobjRenderObjects(struct nugscn_s *scn) {
    struct nuinstance_s *instance;
    s32 i;
    struct nuinstanim_s *instanim;

    instance = ObjectInstance;
    
    for (i = 0; i < 0x40; i++) {
        instance = &ObjectInstance[i];
        if (instance->objid != -1) {
            instanim = instance->anim;
            if (((edobj_particle_mode != 0) || (edobj_sound_mode != 0)) && (edobj_nearest == i)) {
                instance->flags.onscreen = NuRndrGScnObj(scn->gobjs[instance->objid],&instance->mtx);
            }
            else if (instanim != NULL) {
                if (edobjRenderCutoffTest((struct nuvec_s *)&(instanim->mtx)._30) != 0) {
                    instance->flags.onscreen = NuRndrGScnObj(scn->gobjs[instance->objid],&instanim->mtx);
                }
            }
            else {
                if (edobjRenderCutoffTest((struct nuvec_s *)&ObjectInstance[i].mtx._30) != 0) {
                    instance->flags.onscreen = NuRndrGScnObj(scn->gobjs[instance->objid], &instance->mtx);
                }
            }
        }
    }
}

//NGC MATCH
u32 reverse_endian_32(u32 arg0) {
    return (arg0 >> 0x18U) | ((arg0 >> 8U) & 0xFF00) | ((arg0 << 8) & 0xFF0000) | (arg0 << 0x18);
}