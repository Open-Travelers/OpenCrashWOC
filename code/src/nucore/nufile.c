
#define FIRST 0

#include "nufile.h"
#include "nuerror.h"

unsigned char* filebuffer = NULL;
s32 blkcnt = 1;
struct fileinfo_s file_info[16];
static struct BlockInfo blkinfo[0x400];
unsigned char* bpointer;
s32 badGameDisk = 0;
fileHandle currentpointer = -1;
s32 bytesleft = 0;
s32 thisbytesread = 0;
FILE* fpointers[MAX_FILES] = {
	NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL
};
const char* fmode[] = {"rb", "wb"};
struct numemfile_s memfiles[MAX_MEM_FILES];
struct nudatfile_s datfiles[MAX_MEM_FILES];
s32 loadscreen = -1;
s32 loadscreenfadedir = 0;
s32 datacounter = 0;
s32 datafull;
s32 totalbytesread = 0;

s32 NuFileGetBadGameDisc()
{
	return badGameDisk;
}

//NGC MATCH
void NuFileInitEx(s32 deviceid, s32 rebootiop) {

	if (filebuffer == NULL) {
		filebuffer = malloc_x(0x10000);
	}
	memset(memfiles, 0 , MAX_MEM_FILES * sizeof(struct numemfile_s));
	memset(datfiles, 0, MAX_MEM_FILES  * sizeof(struct nudatfile_s));

	printf("memfiles after memset(): %d\n", memfiles);
	printf("datfiles after memset(): %d\n", datfiles);
}

//NGC MATCH
s32 NuFileExists(char* filename) {
    FILE* filep; //__sFILE*
    char name[128] = "";
    char tmp;

    //strcpy(name, "/");
    seekoffset = 0; //fopen NGC
	strcat(name, filename);
	filep = fopen(name, "r");
	printf("check F.E. \n");
	if (filep != NULL) {
            //skip SDK check
        fileoffset = 0;
        filelength = 1;
            //
		fclose(filep);
        printf("file exist! %s \n", filename);
        return 1;
	}
	printf("file not exist %s \n", filename);
	return 0;
}

s32 checkmemfile(char* name)
{
	return -1;
}

s32 checkdiscfile(char* name)
{
	return -1;
}

//NGC 94%
s32 NuFileOpen(char* file, enum nufilemode_e mode) {
    int s; // r31
    int t; // r3
    int f;
    FILE* fp; // r9	//__sFILE*
    FILE** p; //__sFILE**
    char name[128] = ""; // 0x80229388
    void* m;

    printf("F.O. before strcat name %s \n", name);
    if (NuFileGetBadGameDisc() == 0) {
        thisbytesread = 0;
        checkmemfile(file);
        if (checkdiscfile(file) == -1) {
            //strcpy(name, "/");
            printf("checkdiscfile = -1 \n");
        } else {
            strcpy(name, "z:\\");
        }
        strcat(name, file);
        printf("F.O. strcat name %s \n", name);
        if (mode > NUFILE_APPEND) {
            NuErrorProlog("OpenCrashWOC/code/nucore/nufile.c", 0x39e,"assert");
        }
        for (p = fpointers, s = 0; s < 10; s++) {
            m = *p;
            if (*p == NULL) {
                    printf("F.O. fopen \n");
                fp = fopen(name, fmode[mode]);
                if (fp == NULL) {
                    printf("file null \n");
                    return NULL;
                }
                    //skip SDK check
                fileoffset = 0;
                filelength = 1;
                    //
                *p = fp;
                bytesleft = m;
                //printf("bytesleft %d \n",bytesleft);
                s++;
                return s;
            }
            p++;
        }
    }
    return NULL;
}

//NGC MATCH
void NuFileClose(s32 handle) {
	if (handle > 0x3ff) {
		NuMemFileClose(handle);
	}
	else {
		if (handle == currentpointer)
		{
			currentpointer = -1;
			bytesleft = 0;
		}
        handle--;
		fclose(fpointers[handle]);
		fpointers[handle] = NULL;
	}
}

//NGC MATCH
s32 NuMemFileOpen(void* start, s32 buffsize, enum nufilemode_e mode) {
    s32 i;

	if (buffsize > 0 && mode == NUFILE_READ) {
		for (i = 0; i < 20; i++) {
			if (!memfiles[i].used) {
				memfiles[i].start = (char*)start;
				memfiles[i].end = (char*)(((s32)start + buffsize) - 1);
				memfiles[i].curr = (char*)start;
				memfiles[i].mode = mode;
				memfiles[i].used = 1;
				return i + 0x400;
			}
		}
	}
	return 0;
}

s32 NuDatFilePos(s32 handle) {
	return 0;
}

s32 NuDatFileRead(s32 fh, void* data, s32 size)
{
	return 0;
}

s32 NuDatFileSeek(s32 fh, s32 offset, s32 origin)
{
	return 0;
}

void NuDatFileClose(s32 handle) {
	return 0;
}

//NGC MATCH
void NuMemFileClose(s32 handle) {
	if (handle > 0x7ff) {
		NuDatFileClose(handle);
	}
	else {
        handle -= 0x400;
		memfiles[handle].used = 0;
	}
}

//NGC MATCH
s32 NuMemFilePos(s32 handle)
{
	if (handle > 0x7ff) {
		return NuDatFilePos(handle);
	}
    handle = handle - 0x400;
	return ((s32)memfiles[handle].curr - (s32)memfiles[handle].start);
}

//NGC MATCH
s32 NuMemFileRead(s32 handle, void* data, s32 size) {
    s32 left;
    
	if (handle > 0x7ff) {
		return NuDatFileRead(handle, data, size);
	}
	else {
		handle -= 0x400;
		left = (s32)memfiles[handle].end - ((s32)memfiles[handle].curr) + 1;
		if (size > left) {
			size = left;
		}
		if (size != 0)
		{
			memcpy(data, memfiles[handle].curr, size);
			memfiles[handle].curr = memfiles[handle].curr + size;
		}
		return size;
	}
}

//NGC MATCH
s32 NuMemFileSeek(s32 handle, s32 offset, s32 origin) {
	if (handle > 0x7ff) {
		return NuDatFileSeek(handle, offset, origin);
	}
	else {
        handle = handle - 0x400;
            switch (origin) {
                    case NUFILE_SEEK_START:
                    default:
            			memfiles[handle].curr = memfiles[handle].start + offset;
                    break;
                    case NUFILE_SEEK_CURRENT:
            			memfiles[handle].curr = memfiles[handle].curr + offset;
            		break;
            		case NUFILE_SEEK_END:
                         memfiles[handle].curr = (memfiles[handle].end - offset) + 1;
            		break;
            }
        return (int)memfiles[handle].curr - (int)memfiles[handle].start;
	}
}

//NGC MATCH
s32 NuFilePos(s32 handle) {
    s32 ret;
    struct fileinfo_s* info;

	if (handle > 0x400) {
		return NuMemFilePos(handle);
	}
	else {
        handle--;
        info = &file_info[handle];
		if (info->use_buff == NULL) {
		    ret = thisbytesread;
		}
        else{
			ret = ftell(fpointers[handle]); //ret = info->read_pos;
            printf("NuFilePos buff exist %d \n", ret);
        }
		return ret;
	}
}

//NGC MATCH
s32 NuFileSeek(s32 handle, s32 offset, s32 origin)
{
    static s32 forig[] = {0, 1, 2};

	if (handle > 0x3ff)
	{
		return NuMemFileSeek(handle, offset, origin);
	}
	else
	{
        handle--;
		if (origin == NUFILE_SEEK_CURRENT)
		{
			offset -= bytesleft;
		}
		bytesleft = 0;
		return fseek(fpointers[handle], (long)offset, forig[origin]);
	}
}


//NGC MATCH
s32 NuFileSize(char* fileName) {
	s32 rv = -1;
	s32 handle;

    //printf("checking file size \n");
	if (fileName != NULL && *fileName != 0) {
	    if (NuFileExists(fileName) != 0) {
	    printf("file size exist... \n");
            handle = NuFileOpen(fileName, NUFILE_READ);
            printf("handle check... FileSize %d\n", handle);
            if (handle != NULL)
			{
				rv = GCFileSize(handle);
				NuFileClose(handle);
			}
        }
	}
	printf("return FileSize %d\n", rv);
	return rv;
}

//NGC MATCH
void* NuFileLoad(char* fileName) {
    s32 handle;
    s32 size;
	void* mem = NULL;

	size = NuFileSize(fileName);
	if (size > 0) {
		handle = NuFileOpen(fileName, NUFILE_READ);
		if (handle != NULL) {
			mem = NuMemAlloc(size);
			if (mem != NULL)
			{
				NuFileRead(handle, mem, size);
			}
			NuFileClose(handle);
		}
	}
	return mem;
}

//NGC MATCH
s32 NuFileLoadBuffer(char* fileName, void* mem, s32 buffsize) {
	s32 size = NuFileSize(fileName);
	if (size == 0) {
		NuErrorProlog("OpenCrashWOC/code/nucore/nufile.c", 0x58d) ("File %s does not exist!", fileName);
	}
	if (size > buffsize) {
		NuErrorProlog("OpenCrashWOC/code/nucore/nufile.c", 0x590) ("Super Buffer out of space!");
        return 0;
	}
	if (size != 0) {
		s32 handle = NuFileOpen(fileName, NUFILE_READ);
		if (handle != NULL) {
			NuFileRead(handle, mem, size);
			NuFileClose(handle);
			return size;
		} else {
                   return 0; 
                }
	}
    return 0;
}

//NGC MATCH
s32 NuFileRead(s32 handle, void* data, s32 size) {
    s32 bytesread;
    u8* pt;
    s32 tbytesread;
    
	if (handle > 0x3ff) {
		return NuMemFileRead(handle, data, size);
	}
	else {
		if (currentpointer != handle || handle == -1)
		{
			printf("handle = %d ?...\n", handle);
			bytesleft = 0;
			totalbytesread = 0;
			currentpointer = handle;
		}
		handle--;
// There's some kind of gamecube reset call here then an error check, I don't think leaving it out will matter on PC.
		Reseter(1);
        	GC_DiskErrorPoll();
		if (bytesleft == 0) {
			bytesread = fread(filebuffer, 1, 0x10000, fpointers[handle]);
			printf("bytesleft check 1: %d...\n", bytesleft);
			totalbytesread += bytesread;
            		bytesleft = bytesread;
			bpointer = filebuffer;
		}
		datacounter += size;
		if (size <= bytesleft) {
			memcpy(data, bpointer, size);
			bpointer = (u8*)((u32)bpointer + size);
			bytesleft -= size;
			thisbytesread += size;
            return size;
		}
		else {
			memcpy(data, bpointer, bytesleft);
			pt = (u8*)((s32)data + bytesleft);
			thisbytesread += size;
            bytesread = bytesleft;
			size -= bytesleft;
            
			while (size > 0x10000) {
				size -= 0x10000;
				bytesread += fread(filebuffer, 1, 0x10000, fpointers[handle]);
				//tmpsize = freadcheck_NGC(fpointers[handle], 1, 0x10000);
                		printf("memcpy filebuff 1...\n");
				memcpy(pt, filebuffer, 0x10000); 
				pt = (u8*)((u32)pt + 0x10000);
			}
			bytesleft = 0;
			if (size > 0) {
				tbytesread = fread(filebuffer, 1, 0x10000, fpointers[handle]);
                  		//tmpsize = freadcheck_NGC(fpointers[handle], 1, 0x10000);
				printf("memcpy filebuff 2...\n");
				memcpy(pt, filebuffer, size);
				bytesread += tbytesread;
                tbytesread -= size;
				bytesleft = tbytesread;
				bpointer = (u8*)((u32)filebuffer + size);
			}
		}
	}
	printf("NuFileRead bytesread: %d...\n", bytesread);
	return bytesread;
}

//NGC MATCH
void NuStartLoadScreen(s32 screen) {

	loadscreen = screen;
	datacounter = 0;
	if (screen > 0) {
		datafull = screen;
        return;
	}

	datafull = 0;
	return;
}

//NGC MATCH
void NuStopLoadScreen() {
	loadscreen = -1;
	loadscreenfadedir = 0;

	return;
}

//NGC MATCH
float NuFileReadFloat(s32 handle) {
	float data = 0.0f;
	NuFileRead(handle, &data, 4);
	return data;
}

//NGC MATCH
s32 NuFileReadInt(s32 handle) {
	s32 data;
	NuFileRead(handle, &data, 4);
	return data;
}

//NGC MATCH
s16 NuFileReadShort(s32 handle) {
	s16 data;
	NuFileRead(handle, &data, 2);
	return data;
}

//NGC MATCH
char NuFileReadChar(s32 handle) {
	char data;
	NuFileRead(handle, &data, 1);
	return data;
}

//NGC MATCH
s32 NuFileBeginBlkRead(s32 handle, s32 blkType) {
    s32 pos;
    s32 bh;
    
	bh = blkcnt++;
	pos = NuFilePos(handle);
	if (bh > 1 && pos >= blkinfo[bh - 1].pos + blkinfo[bh - 1].hdr.size) {
		blkcnt--;
		return 0;
	}
	else {
        pos = NuFilePos(handle);
		blkinfo[bh].pos = pos;
		NuFileRead(handle, &blkinfo[bh], 8);
		if (blkinfo[bh].hdr.size < 0) {
			blkinfo[bh].hdr.size = -blkinfo[bh].hdr.size;
		}
		if (blkType != 0 && blkType != blkinfo[bh].hdr.blk) {
			NuErrorProlog("OpenCrashWOC/code/nucore/nufile.c", 0x727,"NuFileBeginBlkRead : Block header mismatch!");
		}
		return blkinfo[bh].hdr.blk;
	}
}

//NGC MATCH
s32 NuFileEndBlkRead(s32 fh) {
    s32 bh;
    s32 pos;

    bh = --blkcnt;
    
    pos = NuFilePos(fh);
    
    if (pos == blkinfo[bh].pos + blkinfo[bh].hdr.size) {
        return pos;
    }
    
    return NuFileSeek(fh, blkinfo[bh].pos + blkinfo[bh].hdr.size, NUFILE_SEEK_START);
}

//NGC MATCH
void NuDatClose(struct nudathdr_s* ndh) {
	NuFileClose(ndh->fh);
	if (ndh->dfhandle) {
		datfiles[ndh->dfhandle - 0x800].used = 0;
	}
	if (ndh->intalloc) //managedmem
	{
		if (ndh->filetree != NULL)
		{
			NuMemFree(ndh->filetree);
		}
		NuMemFree(ndh);
	}
	return;
}
