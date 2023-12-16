#include "ssmix.h"

s32 __MIXPanTableL[128] = {
              0,            0,           -1,           -1,
             -1,           -2,           -2,           -2,
             -3,           -3,           -4,           -4,
             -4,           -5,           -5,           -5,
             -6,           -6,           -7,           -7,
             -7,           -8,           -8,           -9,
             -9,          -10,          -10,          -10,
            -11,          -11,          -12,          -12,
            -13,          -13,          -14,          -14,
            -14,          -15,          -15,          -16,
            -16,          -17,          -17,          -18,
            -18,          -19,          -20,          -20,
            -21,          -21,          -22,          -22,
            -23,          -23,          -24,          -25,
            -25,          -26,          -26,          -27,
            -28,          -28,          -29,          -30,
            -30,          -31,          -32,          -33,
            -33,          -34,          -35,          -36,
            -36,          -37,          -38,          -39,
            -40,          -40,          -41,          -42,
            -43,          -44,          -45,          -46,
            -47,          -48,          -49,          -50,
            -51,          -52,          -54,          -55,
            -56,          -57,          -59,          -60,
            -61,          -63,          -64,          -66,
            -67,          -69,          -71,          -72,
            -74,          -76,          -78,          -80,
            -83,          -85,          -87,          -90,
            -93,          -96,          -99,         -102,
           -106,         -110,         -115,         -120,
           -126,         -133,         -140,         -150,
           -163,         -180,         -210,         -904
};

s32 __MIXPanTableR[128] = {
           -904,         -210,         -180,         -163,
           -150,         -140,         -133,         -126,
           -120,         -115,         -110,         -106,
           -102,          -99,          -96,          -93,
            -90,          -87,          -85,          -83,
            -80,          -78,          -76,          -74,
            -72,          -71,          -69,          -67,
            -66,          -64,          -63,          -61,
            -60,          -59,          -57,          -56,
            -55,          -54,          -52,          -51,
            -50,          -49,          -48,          -47,
            -46,          -45,          -44,          -43,
            -42,          -41,          -40,          -40,
            -39,          -38,          -37,          -36,
            -36,          -35,          -34,          -33,
            -33,          -32,          -31,          -30,
            -30,          -29,          -28,          -28,
            -27,          -26,          -26,          -25,
            -25,          -24,          -23,          -23,
            -22,          -22,          -21,          -21,
            -20,          -20,          -19,          -18,
            -18,          -17,          -17,          -16,
            -16,          -15,          -15,          -14,
            -14,          -14,          -13,          -13,
            -12,          -12,          -11,          -11,
            -10,          -10,          -10,           -9,
             -9,           -8,           -8,           -7,
             -7,           -7,           -6,           -6,
             -5,           -5,           -5,           -4,
             -4,           -4,           -3,           -3,
             -2,           -2,           -2,           -1,
             -1,           -1,            0,            0
};

s32 __MIXGetPanL(s8 pan)
{
    return __MIXPanTableL[pan];
}

s32 __MIXGetPanR(s8 pan)
{
    return __MIXPanTableR[pan];
}

void MIXInit()
{
    for (s32 i = 0; i < NUM_MIX_CHANNELS; i++)
    {
        //__MIXResetChannel(&__MIXChannel[i]);
    }
    __MIXDvdStreamAttenCurrent = 0;
    __MIXDvdStreamAttenUser = 0;
    __MIXSoundMode = 1;
}