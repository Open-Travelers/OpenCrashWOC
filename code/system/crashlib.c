#include "system/crashlib.h"



//MATCH GCN
unsigned long timeGetTime(void) {
	return 0;
    //return OSGetTick(); //SDK GCN
}

//MATCH GCN
void DBTimerStart(int index) {
    unsigned long time;

    time = timeGetTime();
    DBTimers[index].start = time;
    return;
}

//MATCH GCN
void DBTimerEnd(int index) {
    unsigned long avg;
    unsigned long time;
    unsigned long elapsed;

    time = timeGetTime();
    avg = DBTimers[index].average;
    elapsed = DBTimers[index].elapsed + (time - DBTimers[index].start);
    DBTimers[index].stop = time;
    DBTimers[index].elapsed = elapsed;
    DBTimers[index].average = avg + elapsed >> 1;
    DBTimers[0].elapsed = 0xa4cb8;
    return;
}

//MATCH GCN
void DBTimerReset(void) {
    s32 i;

    for (i = 0; i < 0x28; i++) {
        DBTimers[i].elapsed = 0;
        DBTimers[i].stop = 0;
        DBTimers[i].start = 0;
    }
    return;
}

//84% WIP
struct D3DTexture* NudxTx_Create(struct nutex_s* texture, s32 rendertargetflag) {
    long data;
    long data2;
    unsigned char* inbits24;
    unsigned short* inbits16;
    long* inbits32;
    long* outbits;
    long* outline;
    unsigned short* outline16;
    unsigned char* inbits8;
    int s;
    int t;
    unsigned char* b1;
    int imagesize;
    int mapix;
    int ix;
    int format;
    int solid;
    int blended;
    int transparent;
    enum nutextype_e type;
    int width;
    int height;
    u8* bits; // void
    int* pal;
    float alphatest;

    int iss3;

    u8 *pbVar1;
    u8 *pbVar2;
    char cVar3;
    char bVar4;
    float fVar5; //alphatest ?
    u32 uVar6;
    //short sVar7;
    s32 unkcnt;
    // long* outbits;
    long* px_buffer;
    u32 uVar8;
    char *puVar9;
    long *puVar10; //register 10?
    long *puVar11;
    long *piVar9;
    u32 uVar10;
    u32 uVar11;
    s32 unkcnt2;

    //tempcmp =  iss3cmp;
    type = texture->type;
    width = texture->width;
    height = texture->height;
    bits = texture->bits;
    pal = texture->pal;
    iss3 = iss3cmp;

    if (iss3 != 0) {
        GS_TexCreateNU(type, width, height, bits, texture->mmcnt, rendertargetflag, GetTPID());
        return NULL;
    }

    if (type == 0x82) {
        GS_TexCreateNU(0x82, width, height, bits, texture->mmcnt, rendertargetflag, GetTPID());
        return NULL;
    }

    if (type == 0x81) {
        GS_TexCreateNU(0x81, width, height, bits, texture->mmcnt, rendertargetflag, GetTPID());
        return NULL;
    }

    if (type == 0x80) {
        GS_TexCreateNU(0x80, width, height, bits, texture->mmcnt, rendertargetflag, GetTPID());
        return NULL;
    }

    unkcnt = width * height;
    inbits16 = (u16*)bits;
    inbits24 = (s8*)bits;
    inbits32 = (long*)bits;
    inbits8 = (s8*)bits;
    //uVar11 = 0;
    //uVar10 = 0;
    unkcnt2 = 0;
    bits = (void* )malloc_x(unkcnt * 4);
    texture->decal = iss3;
    texture->linear = iss3;
    px_buffer = bits;

    switch (type) {
        case NUTEX_RGB16:
        case NUTEX_RGBA16:
            sprintf(DebugText, "RGB16/RGBA16");

            for(s = 0; s < height; s++)
            {
                u8* puVar10 = px_buffer;
                for(t = 0; t < width; t++) {
                     u8 tmp1;
                    u8 tmp3;
                    data = *inbits16;

                    //bitwise transformation on uVar8, rearranging and multiplying specific bits from different positions to create a new value
                    //uVar8 & 0x1f, the mask gets you the lowest 5 bits of uVar8
                    //with uVar8 & 0x3e0 you get the next 5 bits above that; uVar8 & 0x7c00 you get the next 5 after that
                    uVar8 = ((data & 0x1f) << 3) | ((data & 0x3e0) << 6) | ((data & 0x7c00) << 9);
                    if ((data & 1) != 0) {
                        // set the highest 8 bits (bits 24 to 31) of uVar8 to 1 while keeping the other bits unchanged
                        uVar8 |= 0xff000000;
                    }

                    *(u32*)puVar10 = uVar8;
                    tmp1 = puVar10[1];
                    tmp3 = puVar10[3];
                    puVar10[3] = tmp1;
                    puVar10[1] = tmp3;
                    puVar10 += 4;
                    inbits16++;
                }
                px_buffer = px_buffer + width;
            }
            break;
        case NUTEX_RGB24:
            sprintf(DebugText, "RGB24"); // print texture type we're decoding

            for(unkcnt = 0; unkcnt < height; unkcnt++)
            {
                //puVar9 = (char *)inbits24; // puVar9 is output buffer for this row of pixels
                u8* puVar10 = px_buffer; // Pushing px_buffer into local puVar10 (What makes puVar10?)
                for(t = 0; t < width; t++) {
                    u8 tmp1;
                    u8 tmp3;

                    // NUTEXes seems to be little-endian based on this code
                    uVar8  = (*inbits24++ << 0x00); // the blue component
                    uVar8 += (*inbits24++ << 0x08); // the green component
                    uVar8 += (*inbits24++ << 0x10); // the red component
                    uVar8 += 0xFF000000; // set the upper two bytes to FF which means solid alpha

                    *(u32*)puVar10 = uVar8;

                    // swaps blue and red components
                    tmp1 = puVar10[1];
                    tmp3 = puVar10[3];

                    // finishes swap, resulting in a big-endian ARBG color
                    puVar10[3] = tmp1;
                    puVar10[1] = tmp3;
                    puVar10 += 4;
                }
                px_buffer = px_buffer + width;
            }
            break;
        case NUTEX_RGBA32:
            sprintf(DebugText,"RGB32");

            px_buffer = bits;

            for(unkcnt = 0; unkcnt < height; unkcnt++)
            {
                s8* puVar10 = px_buffer;
                for(t = 0; t < width; t++) {
                    *(u32*)puVar10 = *inbits32;
                    fVar5 = puVar10[3] / 255.0f;

                    if (fVar5 < 0.9f) {
                        uVar10 = uVar10 + 1;
                    }
                    else if (0.063f < fVar5) {
                        uVar11 = uVar11 + 1;
                    }
                    else {
                        unkcnt2 = unkcnt2 + 1;
                    }

                    puVar10 += 4;
                    inbits32++;
                }
                px_buffer = px_buffer + width;
            }
            break;
        case NUTEX_PAL4:
        case NUTEX_PAL8:
            if (type == NUTEX_PAL4) {
                sprintf(DebugText, "PAL4");
            } else if (type == NUTEX_PAL8) {
                sprintf(DebugText, "PAL8");
            }

            // piVar9 = bits;
            {

                s8* puVar10 = px_buffer;
                for(s = 0; s < unkcnt;s++) {
                    // uVar6 = (u32)bits[s];
                    if (type == NUTEX_PAL4) {
                        if ((s & 1) != 0) {
                            uVar6 = bits[s / 2] >> 4;
                        }
                        else {
                            uVar6 = bits[s / 2] & 0xf;
                        }
                    } else if (type == NUTEX_PAL8) {
                        uVar6 = bits[s];
                    }

                    *(u32*)puVar10 = pal[uVar6];

                    fVar5 = puVar10[0] / 255.0f;

                    if (fVar5 < 0.9f) {
                        uVar10 = uVar10 + 1;
                    }
                    else if (0.063f < fVar5) {
                        uVar11 = uVar11 + 1;
                    }
                    else {
                        unkcnt2 = unkcnt2 + 1;
                    }

                    puVar10 += 4;
                }
            }
            break;
        default:
            NuErrorProlog("C:/source/crashwoc/code/system/crashlib.c", 0x187,"NudxTx_Create:\tUnknown texture type!");
            break;
    }

    if (((uVar11 == 0) || (uVar10 ==  0)) || (unkcnt2 != 0)) {
        texture->decal = 0;
    }
    else if (0.6f > (float)uVar10/(float)uVar11)
    {
        texture->decal = 1;
    }

    GS_TexCreateNU(type, texture->width, texture->height, &outbits, texture->mmcnt, rendertargetflag, GetTPID());
    free_x(&outbits);
    DebugText[0] = '\0';
    return NULL;
}

