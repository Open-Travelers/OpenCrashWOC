
#define FIRST 0

#include "nufile.h"
#include "nuerror.h"

unsigned char* filebuffer = NULL;
s32 blkcnt = 1;
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

void NuFileInitEx(int deviceid, int rebootiop)
{
	if (filebuffer == NULL)
	{
		filebuffer = malloc_x(0x10000);
	}
	memset(memfiles, 0 , MAX_MEM_FILES * sizeof(struct numemfile_s));
	memset(datfiles, 0, MAX_MEM_FILES  * sizeof(struct nudatfile_s));

	printf("memfiles after memset(): %d\n", memfiles);
	printf("datfiles after memset(): %d\n", datfiles);
}

s32 NuFileExists(char* filename)
{
	char buf[128];
	FILE* filep;

	memset(buf, 0, sizeof(buf));
	buf[0] = '.';
	buf[1] = '/';
	strcat(buf, filename);
	filep = fopen(buf, "r");
	if (filep != NULL)
	{
		fclose(filep);
	}
	//return filep != NULL;
	return 1;
}

s32 checkmemfile(char* name)
{
	return -1;
}

s32 checkdiskfile(char* name)
{
	return -1;
}

fileHandle NuFileOpen(char* filename, enum nufilemode_e mode)
{
	if (NuFileGetBadGameDisc() == 0)
	{
		char namebuf[128];
		memset(namebuf, 0, sizeof(namebuf));
		thisbytesread = 0;
		checkmemfile(filename);
		if (checkdiskfile(filename) == -1)
		{
			namebuf[0] = '.';
			namebuf[1] = '/';
		}
		else
		{
			namebuf[0] = 'z';
			namebuf[1] = ':';
			namebuf[2] = '\\';
		}
		strcat(namebuf, filename);
		if (mode > NUFILE_APPEND)
		{
			error_func e = NuErrorProlog("OpenCrashWOC/code/nucore/nufile.c", 75);
			e("assert");
		}

		for (s32 i = 0; i < MAX_FILES; i++)
		{
			if (fpointers[i] == NULL)
			{
				struct FILE* f = fopen(namebuf, mode == NUFILE_READ ? "rb" : "wb");
				//printf("Test");
				if (f == NULL)
				{
				    printf("file null\n");
					return NULL;
				}
				//printf("/nreaded");
				bytesleft = (s32)fpointers[i];
				fpointers[i] = f;

				return i + 1;

			}
		}
	}
	return NULL;
}

void NuFileClose(fileHandle handle)
{
	if (handle < 0x400)
	{
		if (handle == currentpointer)
		{
			currentpointer = -1;
			bytesleft = 0;
		}
		fclose(fpointers[handle - 1]);
		fpointers[handle - 1] = NULL;
	}
	else
	{
		NuMemFileClose(handle);
	}
}

fileHandle NuMemFileOpen(void* start, u32 size, u32 mode)
{
	if (size > 0 && mode == NUFILE_READ)
	{
		for (s32 i = 0; i < MAX_MEM_FILES; i++) {
			struct numemfile_s* m = &memfiles[i];
			if (!m->used)
			{
				m->used = 1;
				m->end = (char*)((s32)start + (size - 1));
				m->currposition = (char*)start;
				m->mode = NUFILE_READ;
				m->start = (char*)start;
				return i + 0x400;
			}
		}
	}
	return 0;
}

s32 NuDatFilePos(fileHandle handle)
{
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

void NuDatFileClose(fileHandle handle)
{
}

void NuMemFileClose(fileHandle handle)
{
	if (handle < 0x800)
	{
		memfiles[handle - 0x400].used = 0;
	}
	else
	{
		NuDatFileClose(handle);
	}
}

s32 NuMemFilePos(fileHandle handle)
{
	if (handle < 0x800)
	{
		return ((s32)memfiles[handle - 0x400].currposition - (s32)memfiles[handle - 0x400].start);
	}
	else
	{
		return NuDatFilePos(handle);
	}
}

s32 NuMemFileRead(fileHandle handle, void* data, s32 size)
{
	if (handle < 0x800)
	{
		s32 f = handle - 0x400;
		s32 left = (s32)memfiles[f].end + (1 - (s32)memfiles[f].currposition);
		if (size > left)
		{
			size = left;
		}
		if (size != 0)
		{
			memcpy(data, memfiles[f].currposition, size);
			memfiles[f].currposition = memfiles[f].currposition + size;
		}
		return size;
	}
	else
	{
		return NuDatFileRead(handle, data, size);
	}
}

s32 NuMemFileSeek(fileHandle handle, s32 offset, s32 origin)
{
	if (handle < 0x800)
	{
		fileHandle f = handle - 0x400;
		switch (origin)
		{
		case NUFILE_SEEK_START:
			memfiles[f].currposition = (void*)((s32)memfiles[f].start + offset);
			break;
		case NUFILE_SEEK_CURRENT:
			memfiles[f].currposition = (void*)((s32)memfiles[f].currposition + offset);
			break;
		case NUFILE_SEEK_END:
			memfiles[f].currposition = (void*)((s32)memfiles[f].end + (1 - offset));
			break;
		}
		return (s32)memfiles[f].currposition - (s32)memfiles[f].start;
	}
	else
	{
		return NuDatFileSeek(handle, offset, origin);
	}
}

// I completely redid a part of this function, I hope it's right.
s32 NuFilePos(fileHandle handle)
{
	if (handle < 0x400)
	{
		s32 ret = thisbytesread;
		if (fpointers[handle - 1] != NULL)
		{
			ret = ftell(fpointers[handle - 1]);
		}
		return ret;
	}
	else
	{
		return NuMemFilePos(handle);
	}
}

s32 NuFileSeek(fileHandle handle, s32 offset, s32 origin)
{
	if (handle < 0x400)
	{
		if (origin == NUFILE_SEEK_CURRENT)
		{
		    printf("seek curr...\n");
			offset -= bytesleft;
		}
		bytesleft = 0;
		return fseek(fpointers[handle - 1], offset, origin);
	}
	else
	{
		return NuMemFileSeek(handle, offset, origin);
	}
}

s32 NuFileSize(char* fileName)
{
	s32 ret = -1;
	if (fileName != NULL && *fileName != 0)
	{
	    s32 check = NuFileExists(fileName);
	    if (check != 0)
        {
            printf("file exist...\n");
            fileHandle handle = NuFileOpen(fileName, NUFILE_READ);
            if (handle != NULL)
			{
				NuFileSeek(handle, 0, NUFILE_SEEK_END);
				ret = NuFilePos(handle);
				NuFileClose(handle);
			}
        }
		else
        {
            printf("file non-existent...\n");
            return 0;
        }
	}
	return ret;
}

void* NuFileLoad(char* fileName)
{
	void* dest = NULL;
	s32 size = NuFileSize(fileName);
	if (size > 0)
	{
		fileHandle handle = NuFileOpen(fileName, NUFILE_READ);
		if (handle != NULL)
		{
			dest = NuMemAlloc(size);
			if (dest != NULL)
			{
				NuFileRead(handle, &dest, size);
			}
			NuFileClose(handle);
		}
	}
	return dest;
}

s32 NuFileLoadBuffer(char* fileName, void* dest, s32 maxSize)
{
	s32 size = NuFileSize(fileName);
	if (size == 0)
	{
		printf("file size: %d\n", size);
		error_func e = NuErrorProlog("OpenCrashWOC/code/nucore/nufile.c", 321);
		e("File %s does not exist!", fileName);
	}
	if (size > maxSize)
	{
		printf("file size: %d\n", size);
		error_func e = NuErrorProlog("OpenCrashWOC/code/nucore/nufile.c", 327);
		e("Super Buffer out of space!");

	}
	else if (size != 0)
	{
		fileHandle handle = NuFileOpen(fileName, NUFILE_READ);
		if (handle != NULL)
		{
			printf("filebuffer reading...\n");
			NuFileRead(handle, &dest, size);
			NuFileClose(handle);
			return size;

		}
	}
	return 0;
}

s32 NuFileRead(fileHandle handle, void* data, s32 size)
{
	if (handle < 0x400)
	{
		if (currentpointer != handle || handle == -1)
		{
			totalbytesread = 0;
			bytesleft = 0;
			currentpointer = handle;
		}
		fileHandle f = handle - 1;
		// There's some kind of gamecube reset call here then an error check, I don't think leaving it out will matter on PC.
		if (bytesleft == 0)
		{
			bytesleft = fread(filebuffer, 1, 0x10000, fpointers[f]);
			totalbytesread += bytesleft;
			bpointer = filebuffer;
		}
		datacounter += size;
		if (size > bytesleft)
		{
			//memcpy(data, bpointer, bytesleft);      //causes crash
			u8* tmp = (u8*)((s32)data + bytesleft);
			thisbytesread += size;
			u32 read = size - bytesleft;
			size = bytesleft;
			while (read > 0x10000)
			{
				read -= 0x10000;
				u32 dat = fread(filebuffer, 1, 0x10000, fpointers[f]);
				size += dat;
				//memcpy(tmp, filebuffer, 0x10000);   //causes crash
				tmp = (u8*)((u32)tmp + 0x10000);
			}
			bytesleft = 0;
			if (read > 0)
			{
				u32 dat = fread(filebuffer, 1, 0x10000, fpointers[f]);
				//memcpy(tmp, filebuffer, read);  //causes crash
				size += dat;
				bytesleft = dat - read;
				bpointer = ((u32)filebuffer + read);
			}
		}
		else
		{
			//memcpy(data, bpointer, size);   //causes crash
			bpointer = ((u32)bpointer + size);
			bytesleft -= size;
			thisbytesread += size;
		}
	}
	else
	{
		size = NuMemFileRead(handle, data, size);
	}
	return size;
}

void NuStartLoadScreen(s32 screen)
{
	if (screen > 0)
	{
		loadscreen = screen;
		datacounter = 0;
		datafull = screen;
		return;
	}
	else
	{
		loadscreen = screen;
		datacounter = 0;
		datafull = 0;
	}

	return;
}

void NuStopLoadScreen()
{
	loadscreen = -1;
	loadscreenfadedir = 0;

	return;
}

f32 NuFileReadFloat(fileHandle handle)
{
	f32 tmp = 0.0;
	NuFileRead(handle, &tmp, 4);
	return tmp;
}

s32 NuFileReadInt(fileHandle handle)
{
	s32 tmp;
	NuFileRead(handle, &tmp, 4);
	return tmp;
}

s16 NuFileReadShort(fileHandle handle)
{
	s16 tmp;
	NuFileRead(handle, &tmp, 2);
	return tmp;
}

char NuFileReadChar(fileHandle handle)
{
	char tmp;
	NuFileRead(handle, &tmp, 1);
	return tmp;
}

s32 NuFileBeginBlkRead(fileHandle handle, s32 blkType)
{
	s32 currBlock = blkcnt;
	blkcnt++;
	s32 pos = NuFilePos(handle);
	if (currBlock < 2 || pos < blkinfo[currBlock - 1].pos + blkinfo[currBlock - 1].hdr.size)
	{
		blkinfo[currBlock].pos = pos; //old: blkinfo[currBlock - 1].pos = pos;
		NuFileRead(handle, blkinfo + currBlock, 8);
		if (blkinfo[currBlock].hdr.size < 0)
		{
			blkinfo[currBlock].hdr.size = -blkinfo[currBlock].hdr.size;
		}
		if (blkType != 0 && blkType != blkinfo[currBlock].hdr.blk)
		{
			error_func e = NuErrorProlog("OpenCrashWOC/code/nucore/nufile.c", 471);
			e("NuFileBeginBlkRead : Block header mismatch!");
		}
		return blkinfo[currBlock].hdr.blk;
	}
	else
	{
		currBlock = 0;
		blkcnt--;
		return 0;
	}
}

s32 NuFileEndBlkRead(fileHandle handle)
{
	blkcnt--;
	s32 pos = NuFilePos(handle);
	s32 blkEnd = blkinfo[blkcnt].pos + blkinfo[blkcnt].hdr.size;
	if (pos != blkEnd)
	{
		pos = NuFileSeek(handle, blkEnd, NUFILE_SEEK_START);
	}

	return pos;
}

void NuDatClose(struct nudathdr_s* ndh)
{
	NuFileClose(ndh->fh);
	if (ndh->finfo->foffset) //fileid
	{
		datfiles[ndh->finfo->foffset - 0x800].used = 0;
	}
	if (ndh->intalloc) //managedmem
	{
		if (ndh->memdatptr != NULL)
		{
			NuMemFree(ndh->memdatptr);
		}
		NuMemFree(ndh);
	}
	return;
}
