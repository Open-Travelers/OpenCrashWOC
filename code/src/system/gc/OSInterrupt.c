#include "OSAlloc.h"

s32 OSEnableInterrupts(void)
{
    return;
}

s32 OSDisableInterrupts(void)
{
    return;
}

s32 OSRestoreInterrupts(s32 enable)
{
   if (enable) {
      return OSEnableInterrupts();
   } else {
      return OSDisableInterrupts();
   }
}
