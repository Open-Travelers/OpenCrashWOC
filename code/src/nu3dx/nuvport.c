#include "nuvport.h"
#include "../system.h"

static struct nuviewport_s vpDevice;
static struct nuviewport_s vpCurrent;
static s32 vport_inval;
static struct numtx_s smtx;

//PS2
void NuVpInit(void)
{
    //GS_GetViewport(&vpDevice);  //NuPs2GetViewport(&vpDevice);
    vpDevice.centre_x = 0.5f;
    vpDevice.centre_y = 0.5f;
    vpDevice.clipmin_x = 0.0f;
    vpDevice.clipmin_y = 0.0f;
    vpDevice.clipmax_x = 1.0f;
    vpDevice.clipmax_y = 1.0f;

    memcpy(&vpDevice, &vpCurrent, sizeof(struct nuviewport_s));
    vport_inval = 1;

    NuVpUpdate();
    return;
}


//PS2
static void NuVpSetScalingMtx(void)
{
  smtx._00 = (float)vpCurrent.width * 0.5f;
  smtx._01 = 0.0f;
  smtx._02 = 0.0f;
  smtx._03 = 0.0f;
  smtx._10 = 0.0f;
  smtx._11 = -(float)vpCurrent.height * 0.5f;
  smtx._12 = 0.0f;
  smtx._13 = 0.0f;
  smtx._20 = 0.0f;
  smtx._21 = 0.0f;
  smtx._22 = vpCurrent.zmax - vpCurrent.zmin;
  smtx._23 = 0.0f;
  smtx._30 = (float)vpCurrent.x + (float)vpCurrent.width * vpCurrent.centre_x;
  smtx._31 = (float)vpCurrent.y + (float)vpCurrent.height * vpCurrent.centre_y;
  smtx._32 = vpCurrent.zmin;
  smtx._33 = 1.0f;
  return;
}

//PS2
void NuVpUpdate(void)
{
  if (vport_inval != 0) {
    vport_inval = 0;
    NuVpSetScalingMtx();
    if ((vpCurrent.clipmin_x == 0.0f) && (vpCurrent.clipmax_x == 1.0f)) {
      vpCurrent.clip_w = 1.0f;
      vpCurrent.scis_w = (float)vpCurrent.width / 57600.0f;
    }
    else {
      vpCurrent.scis_w = 1.0f / (vpCurrent.clipmax_x - vpCurrent.clipmin_x);
      vpCurrent.clip_w = vpCurrent.scis_w;
    }
    if ((vpCurrent.clipmin_y == 0.0f) && (vpCurrent.clipmax_y == 1.0f)) {
      vpCurrent.clip_h = 1.0f;
      vpCurrent.scis_h = (float)vpCurrent.height / 57600.0f;
    }
    else {
      vpCurrent.scis_h = 1.0f / (vpCurrent.clipmax_y - vpCurrent.clipmin_y);
      vpCurrent.clip_h = vpCurrent.scis_h;
    }
  }
  return;
}

void NuVpSetClippingMtx(void)
{
  NuMtxSetIdentity(&cmtx);
  return;
}

//PS2
void NuVpSetSize(float w,float h)
{
  vport_inval = 1;
  vpCurrent.width = (int)w;
  vpCurrent.height = (int)h;
  return;
}

//PS2
float NuVpPixelWidth(float param_1)
{
  return param_1 * (float)vpCurrent.width + (float)vpCurrent.x;
}

//PS2
float NuVpPixelHeight(float param_1)
{
  return param_1 * (float)vpCurrent.height + (float)vpCurrent.y;
}

//PS2
struct nuviewport_s * NuVpGetCurrentViewport(void)
{
  return &vpCurrent;
}

//PS2
void NuVpGetScalingMtx(struct numtx_s *mtx)
{
    if (mtx != NULL) {
      memcpy(mtx, &smtx, sizeof(struct numtx_s));
    }
    return;
}

//NGC MATCH
void NuVpGetClippingMtx(struct numtx_s *mtx) {
  if (mtx != NULL) {
    *mtx = cmtx;
  }
  return;
}
