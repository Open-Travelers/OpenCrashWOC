#include "fs.h"

/*
//NGC MATCH
void GC_DiskErrorPoll(void) {
    s32 var1;
    u32 var2;
    u32 motor [4];

    var1 = 0;
    memset(motor,0,0x10);
    var2 = 0;
    do {
        switch(DVDGetDriveStatus()) {
            case -1:
                var2 = 5;
                var1 = 5;
            break;
            case 4:
                var2 = 2;
                var1 = 2;
            break;
            case 5:
                var2 = 1;
                var1 = 1;
            break;
            case 6:
                var2 = 3;
                var1 = 3;
            break;
            case 0xb:
                var2 = 4;
                var1 = 4;
            break;
            case 0:
            case 2:
                if (var2 != 0) {
                    break;
                }
            case 1:
            default:
                Lockupbuffer1[0] = Lockupbuffer2[0] = Lockupbuffer3[0] = 0;
            if (var1 != 0) {
                GS_RenderClearBlack();
                GS_BeginScene();
                GS_EndScene();
                GS_FlipScreen();
                return;
            }
            return;
        }
        switch((s32)var2) {
            case 1:
                sprintf(Lockupbuffer1,GetDiskErrString(0,0));
                sprintf(Lockupbuffer2,GetDiskErrString(0,1));
                sprintf(Lockupbuffer3,GetDiskErrString(0,2));
                Reseter(0);
            break;
            case 2:
            case 3:
                sprintf(Lockupbuffer1,GetDiskErrString(1,0));
                sprintf(Lockupbuffer2,GetDiskErrString(1,1));
                Reseter(0);
            break;
            case 4:
                sprintf(Lockupbuffer1,GetDiskErrString(2,0));
                sprintf(Lockupbuffer2,GetDiskErrString(2,1));
                sprintf(Lockupbuffer3,GetDiskErrString(2,2));
            break;
            case 5:
                sprintf(Lockupbuffer1,GetDiskErrString(3,0));
                sprintf(Lockupbuffer2,GetDiskErrString(3,1));
                sprintf(Lockupbuffer3,GetDiskErrString(3,2));
            default:
            break;
        }
                PADControlAllMotors(motor);
                PrintError = 1;
                GS_RenderClearBlack();
                GS_BeginScene();
                GS_EndScene();
                GS_FlipScreen();
                if (var2 == 5) {
                    GS_RenderClearBlack();
                    GS_BeginScene();
                    GS_EndScene();
                    GS_FlipScreen();
                    do {
                            /* WARNING: Do nothing block with infinite loop */
                    } while( 1 );
                }
        PrintError = 0;
    } while( 1 );
}
*/


/*
//97%
s32 GCFileOpen(char *filename) {
    if (FSStart == 0) {
        memset(&MAHFile,0,0x3c);
        FSStart = 1;
        DVDInit();
    }
    StreamClear();
    GC_DiskErrorPoll();
    if (DVDOpen(filename,&MAHFile) != 0) {
        filelength = MAHFile.length;
        fileoffset = 0;
        return 1;
    }
    
    return 0;
}*/

//NGC MATCH
s32 GCFileSize(s32 fd) {
    //GC_DiskErrorPoll();
    //return MAHFile.length;
    return filelength;
}

//NGC MATCH
s32 GCFileRead(s32 fd, void* buf, s32 nbyte) {
    s32 bVar1;
    u32 uVar2;
    s32 aligned;

    GC_DiskErrorPoll();
    bVar1 = 0;
    if (nbyte < 0x20) {
        DisplayErrorAndLockup(
            "C:/source/crashwoc/code/system/gc/fs.c", 0x7e, "GCFileRead Attempt to read less than 32 bytes from file"
        );
    }
    if (nbyte > filelength) {
        nbyte = filelength + (0x20 - (filelength & 0x1fU) & 0x1f);
    }
    uVar2 = filelength - fileoffset;
    if (nbyte > uVar2) {
        nbyte = uVar2 + (0x20 - (uVar2 & 0x1f) & 0x1f);
    }
    if (((s32)buf & 0x1f) != 0) {
        DisplayErrorAndLockup("C:/source/crashwoc/code/system/gc/fs.c", 0x8c, "GCFileRead Buf not aligned");
    }
    if ((seekoffset & 3) != 0) {
        DisplayErrorAndLockup(
            "C:/source/crashwoc/code/system/gc/fs.c", 0x90, "GCFileRead Seek offset must be multiple of 4"
        );
    }
    if (seekoffset > filelength) {
        DisplayErrorAndLockup("C:/source/crashwoc/code/system/gc/fs.c", 0x94, "GCFileRead Seek offset outside of file");
    }
    
    DVDReadAsyncPrio(&MAHFile, buf, (nbyte + (0x20 - 1)) & ~(0x20 - 1), seekoffset, NULL, 2);
    while (bVar1 == 0) {
        Reseter(1);
        switch (DVDGetDriveStatus()) {
            case -1:
                GC_DiskErrorPoll();
                break;
            case 4:
                GC_DiskErrorPoll();
                break;
            case 5:
                GC_DiskErrorPoll();
                break;
            case 6:
                GC_DiskErrorPoll();
                break;
            case 0xb:
                GC_DiskErrorPoll();
                break;
            case 0:
                bVar1 = 1;
                break;
            case 1:
            case 2:
                break;
        }
    }
    return (nbyte + (0x20 - 1)) & ~(0x20 - 1);
}

/*
//MATCH GCN
struct __sFILE * fopen_NGC(const char* _name, const char* _type) {
    char *pcVar1;

    seekoffset = 0;
    strcpy(namesave,_name);
    while( 1 ) {
        pcVar1 = strchr(_name,0x5c);
        if (pcVar1 == NULL) break;
        *pcVar1 = '/';
    }
    amread = (u32)pcVar1;
    currentfd = GCFileOpen(_name);
    return (struct __sFILE *)currentfd;
}
*/

//MATCH GCN
u32 fread_NGC(void *buffer,u32 size,u32 count,struct __sFILE *stream) {
    s32 ret;

    if (stream != NULL) {
        sprintf(temp,"fd %d size %d count %d offset %d length %d\n",stream,size,count,fileoffset,
                filelength);
        if (fileoffset > filelength) {
            DisplayErrorAndLockup("C:/source/crashwoc/code/system/gc/fs.c",0xcf,"fread");
        }
        ret = size * count;
        GCFileRead((int)stream,buffer,ret);
        seekoffset = seekoffset + ret;
        fileoffset = fileoffset + ret;
        if (fileoffset > filelength) {
            fileoffset = filelength;
        }
        if (ret != -1) {
            amread = amread + ret;
            return ret;
        }
    }
    return 0;
}

//extern int thisbytesread; //nufile.c

//MATCH GCN
s32 fseek_NGC(struct __sFILE *__stream, long __off, int __whence) {
    switch (__whence) {
    case 0:
        seekoffset = __off;
        fileoffset = __off;
        thisbytesread = __off;
        break;
    case 1:
        DisplayErrorAndLockup("C:/source/crashwoc/code/system/gc/fs.c",0xfc,"fseek");
        break;
    case 2:
        seekoffset = MAHFile.length;
        fileoffset = MAHFile.length;
        break;
    }
    return 1;
}

//MATCH GCN
s32 GCFileClose(s32 fd) {
    //GC_DiskErrorPoll();
    //DVDClose(&MAHFile);
    return 0;
}

//MATCH GCN
s32 fclose_NGC(struct __sFILE *__stream) {

    if (__stream == NULL) {
        return 0;
    }
    else {
        seekoffset = 0;
        return GCFileClose((s32)__stream);
    }
}

u32 freadcheck_NGC (FILE* handle, u32 size, u32 count) {
    s32 ret;
    u32 tmp2;
        //fread NGC
        if (handle != 0){
                ret = size * count;
                        //GCFileRead
                                        if (ret > filelength) {
                                            ret = filelength + (0x20 - (filelength & 0x1fU) & 0x1f);
                                        }
                                        tmp2 = filelength - fileoffset;
                                        if (ret > tmp2) {
                                            ret = tmp2 + (0x20 - (tmp2 & 0x1f) & 0x1f);
                                        }
                        //
                seekoffset += ret;
                if (filelength < fileoffset) {
                    fileoffset = filelength;
                } else{
                    fileoffset += ret;
                }
                if (ret != -1) {
                    amread = amread + ret;
                    printf("ret val: %d\n", ret);
                    return ret;
                }

            }
        printf("ret NULL: %d\n", ret);
        return 0;
                    //
}
