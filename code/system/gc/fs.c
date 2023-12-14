#include "fs.h"

/*
void GC_DiskErrorPoll(void)

{
    long Dstatus;
    char *Errstr;
    char *str;
    int var1;
    uint var2;
    uint motor [4];
    
    var1 = 0;
    memset(motor,0,0x10);
    var2 = 0;
    do {
        Dstatus = DVDGetDriveStatus();
        if (Dstatus == 2) {
LAB_800c6614:
            if (var2 == 0) {
LAB_800c661c:
                Lockupbuffer1[0] = '\0';
                Lockupbuffer3[0] = '\0';
                Lockupbuffer2[0] = '\0';
                if (var1 != 0) {
                    GS_RenderClearBlack();
                    GS_BeginScene();
                    GS_EndScene();
                    GS_FlipScreen();
                }
                return;
            }
        }
        else if (Dstatus < 3) {
            if (Dstatus == 0) goto LAB_800c6614;
            if ((0 < Dstatus) || (Dstatus != -1)) goto LAB_800c661c;
            var2 = 5;
            var1 = 5;
        }
        else if (Dstatus == 5) {
            var2 = 1;
            var1 = 1;
        }
        else if (Dstatus < 6) {
            if (Dstatus != 4) goto LAB_800c661c;
            var2 = 2;
            var1 = 2;
        }
        else if (Dstatus == 6) {
            var2 = 3;
            var1 = 3;
        }
        else {
            if (Dstatus != 0xb) goto LAB_800c661c;
            var2 = 4;
            var1 = 4;
        }
        if (var2 < 4) {
            if (var2 < 2) {
                if (var2 != 1) goto LAB_800c67f4;
                Errstr = GetDiskErrString(0,0);
                sprintf(Lockupbuffer1,Errstr);
                Errstr = GetDiskErrString(0,1);
                sprintf(Lockupbuffer2,Errstr);
                Errstr = GetDiskErrString(0,2);
                str = Lockupbuffer3;
            }
            else {
                Errstr = GetDiskErrString(1,0);
                sprintf(Lockupbuffer1,Errstr);
                Errstr = GetDiskErrString(1,1);
                str = Lockupbuffer2;
            }
            sprintf(str,Errstr);
            Reseter();
        }
        else if (var2 == 4) {
            Errstr = GetDiskErrString(2,0);
            sprintf(Lockupbuffer1,Errstr);
            Errstr = GetDiskErrString(2,1);
            sprintf(Lockupbuffer2,Errstr);
            Errstr = GetDiskErrString(2,2);
            sprintf(Lockupbuffer3,Errstr);
        }
        else if (var2 == 5) {
            Errstr = GetDiskErrString(3,0);
            sprintf(Lockupbuffer1,Errstr);
            Errstr = GetDiskErrString(3,1);
            sprintf(Lockupbuffer2,Errstr);
            Errstr = GetDiskErrString(3,2);
            sprintf(Lockupbuffer3,Errstr);
        }
LAB_800c67f4:
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
            } while( true );
        }
        PrintError = 0;
    } while( true );
}

//96%
s32 GCFileOpen(char *fname) {
    int i;
    
    if (FSStart == 0) {
        memset(&MAHFile,0,0x3c);
        FSStart = 1;
        DVDInit();
    }
    StreamClear();
    GC_DiskErrorPoll();
    if (DVDOpen(fname,&MAHFile) == 0) {
        i = 0;
    }
    else{
        fileoffset = 0;
        filelength = MAHFile.length;
        i = 1;
    }
    return i;
}

//NGC MATCH
s32 GCFileSize(s32 fd) {
    GC_DiskErrorPoll();
    return MAHFile.length;
}

//86%
s32 GCFileRead(s32 fd,void *buf,s32 nbyte) {
    s32 bVar1;
    u32 uVar2;
    s32 temp;
    
    GC_DiskErrorPoll();
    bVar1 = 0;
    if (nbyte < 0x20) {
        DisplayErrorAndLockup
                  ("C:/source/crashwoc/code/system/gc/fs.c",0x7e,
                   "GCFileRead Attempt to read less than 32 bytes from file");
    }
    if (nbyte > filelength) {
        nbyte = filelength + (0x20 - (filelength & 0x1fU) & 0x1f);
    }
    uVar2 = filelength - fileoffset;
    if (nbyte > uVar2) {
        nbyte = uVar2 + (0x20 - (uVar2 & 0x1f) & 0x1f);
    }
    if (((s32)buf & 0x1f) != 0) {
        DisplayErrorAndLockup
                  ("C:/source/crashwoc/code/system/gc/fs.c",0x8c,"GCFileRead Buf not aligned");
    }
    if ((seekoffset & 3) != 0) {
        DisplayErrorAndLockup
                  ("C:/source/crashwoc/code/system/gc/fs.c",0x90,
                   "GCFileRead Seek offset must be multiple of 4");
    }
    if (seekoffset > filelength) {
        DisplayErrorAndLockup
                  ("C:/source/crashwoc/code/system/gc/fs.c",0x94,
                   "GCFileRead Seek offset outside of file");
    }
    temp = nbyte + 0x1fU;
    DVDReadAsyncPrio(&MAHFile,buf,temp & 0xffffffe0,seekoffset,NULL,2);
    do {
        Reseter(1);
        switch (DVDGetDriveStatus()) {
        case -1:
        case 1:
        case 2:
        case 5:
        case 4:
        case 6:
            GC_DiskErrorPoll();
        break;
        case 0:    
        case 0xb:
            bVar1 = 1;
            break;
        }
    } while (bVar1 == 0);
    return temp & 0xFFFFFFE0;
}

//MATCH GCN
struct __sFILE * fopen(const char* _name, const char* _type) {
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

//MATCH GCN
u32 fread(void *buffer,u32 size,u32 count,struct __sFILE *stream) {
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
s32 fseek(struct __sFILE *__stream, long __off, int __whence) {
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
    GC_DiskErrorPoll();
    DVDClose(&MAHFile);
    return 0;
}

//MATCH GCN
s32 fclose(struct __sFILE *__stream) {
    
    if (__stream == NULL) {
        return 0;
    }
    else {
        seekoffset = 0;
        return GCFileClose((s32)__stream);
    }
}


*/
