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

//89% WIP
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
    u8* bits;
    int* pal;
    float alphatest;

    int iss3;
    u32 uVar10;
    u32 uVar11;
    
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
    
    imagesize = width * height;
    inbits8 = (s8*)bits;
    inbits16 = (u16*)bits;
    inbits24 = (s8*)bits;
    inbits32 = (long*)bits;
    bits = (void* )malloc_x(imagesize * 4);
    texture->decal = texture->linear = iss3;
    outline = bits;
    ix = 0;
    uVar10 = 0;
    uVar11 = 0;
    
    switch (type) {
        case NUTEX_RGB16:
        case NUTEX_RGBA16:
            sprintf(DebugText, "RGB16/RGBA16");
            
            for(s = 0; s < height; s++)
            {
                u8* b1 = outline;
                for(t = 0; t < width; t++) {
                     u8 tmp1;
                    u8 tmp3;
                    data = *inbits16;
                    
                    //bitwise transformation on uVar8, rearranging and multiplying specific bits from different positions to create a new value
                    //solid & 0x1f, the mask gets you the lowest 5 bits of solid
                    //with solid & 0x3e0 you get the next 5 bits above that; solid & 0x7c00 you get the next 5 after that
                    solid = ((data & 0x1f) << 3) | ((data & 0x3e0) << 6) | ((data & 0x7c00) << 9);
                    if ((data & 1) != 0) {
                        // set the highest 8 bits (bits 24 to 31) of uVar8 to 1 while keeping the other bits unchanged
                        solid |= 0xff000000;
                    }
                    
                    *(u32*)b1 = solid;
                    tmp1 = b1[1];
                    tmp3 = b1[3];
                    b1[3] = tmp1;
                    b1[1] = tmp3;
                    b1 += 4;
                    inbits16++;
                }
                outline = outline + width;
            }
            break;
        case NUTEX_RGB24:
            sprintf(DebugText, "RGB24"); // print texture type we're decoding
    
            for(s = 0; s < height; s++)
            {
                //puVar9 = (char *)inbits24; // puVar9 is output buffer for this row of pixels
                u8* b1 = outline; // Pushing outline into local b1 (What makes b1?)
                for(t = 0; t < width; t++) {
                    u8 tmp1;
                    u8 tmp3;

                    // NUTEXes seems to be little-endian based on this code
                    solid  = (*inbits24++ << 0x00); // the blue component
                    solid += (*inbits24++ << 0x08); // the green component
                    solid += (*inbits24++ << 0x10); // the red component
                    solid += 0xFF000000; // set the upper two bytes to FF which means solid alpha
                    
                    *(u32*)b1 = solid;

                    // swaps blue and red components
                    tmp1 = b1[1];
                    tmp3 = b1[3];
                    
                    // finishes swap, resulting in a big-endian ARBG color
                    b1[3] = tmp1;
                    b1[1] = tmp3;
                    b1 += 4;
                }
                outline = outline + width;
            }
            break;
        case NUTEX_RGBA32:
            sprintf(DebugText,"RGB32");
            
            outline = bits;
            
            for(s = 0; s < height; s++)
            {
                s8* b1 = outline;
                for(t = 0; t < width; t++) {
                    *(u32*)b1 = *inbits32;
                    alphatest = b1[3] / 255.0f;
                    
                    if (alphatest < 0.9f) {
                        uVar10 = uVar10 + 1;
                    }
                    else if (0.063f < alphatest) {
                        uVar11 = uVar11 + 1;
                    }
                    else {
                        ix++;
                    }
                    
                    b1 += 4;
                    inbits32++;
                }
                outline = outline + width;
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
        
                s8* b1 = outline;
                for(s = 0; s < imagesize;s++) {
                    // uVar6 = (u32)bits[s];
                    if (type == NUTEX_PAL4) {
                        if ((s & 1) != 0) {
                            mapix = bits[s / 2] >> 4;
                        }
                        else {
                            mapix = bits[s / 2] & 0xf;
                        }
                    } else if (type == NUTEX_PAL8) {
                        mapix = bits[s];
                    }
                    
                    *(u32*)b1 = pal[mapix];
                    
                    alphatest = b1[0] / 255.0f;
                    
                    if (alphatest < 0.9f) {
                        uVar10 = uVar10 + 1;
                    }
                    else if (0.063f < alphatest) {
                        uVar11 = uVar11 + 1;
                    }
                    else {
                        ix++;
                    }
                    
                    b1 += 4;
                }
            }
            break;
        default:
            NuErrorProlog("C:/source/crashwoc/code/system/crashlib.c", 0x187)("NudxTx_Create:\tUnknown texture type!");
            break;
    }
    
    if (((uVar11 != 0) && (uVar10 !=  0)) && (ix != 0)) {
            //texture->mmcnt = 0;
     if (0.6f > (float)((s32)uVar10)/(float)((s32)uVar11)) {
            texture->decal = 1;
    } 
    } else {
            texture->decal = 0;
    } 
    
    GS_TexCreateNU(type, texture->width, texture->height, &outbits, texture->mmcnt, rendertargetflag, GetTPID());
    free_x(&outbits);
    DebugText[0] = '\0';
    return NULL;
}
