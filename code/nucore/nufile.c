#include "nufile.h"
#include "nuerror.h"

void* filebuffer = NULL;
u32 blkcnt = 1;
u32 badGameDisk = 0;
fileHandle currentpointer = -1;
u32 bytesleft = 0;
u32 thisbytesread = 0;
FILE* fpointers[MAX_FILES] = {
	NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL
};
s32 loadscreen = -1;
u32 loadscreenfadedir = 0;
u32 datacounter = 0;
u32 totalbytesread = 0;

u32 NuFileGetBadGameDisc()
{
	return badGameDisk;
}

void NuFileInitEx()
{
	if (filebuffer == NULL)
	{
		filebuffer = malloc_x(0x10000);
	}
	memset(memfiles, 0, MAX_MEM_FILES * sizeof(MemFile));
	memset(datfiles, 0, MAX_DAT_FILES * sizeof(MemFile));
}

u32 NuFileExists(char* filename)
{
	char buf[128];
	memset(buf, 0, sizeof(buf));
	FILE* file;
	buf[0] = '.';
	buf[1] = '/';
	strcat(buf, filename);
	file = fopen(buf, "r");
	if (file != NULL)
	{
		fclose(file);
	}
	return file != NULL;
}

s32 checkmemfile()
{
	return -1;
}

s32 checkdiskfile()
{
	return -1;
}

fileHandle NuFileOpen(char* filename, u32 mode)
{
	if (NuFileGetBadGameDisc() == 0)
	{
		char namebuf[128];
		memset(namebuf, 0, sizeof(namebuf));
		thisbytesread = 0;
		checkmemfile();
		if (checkdiskfile() == -1)
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
		if (mode > 2)
		{
			error_func e = NuErrorProlog("OpenCrashWOC/code/nucore/nufile.c", 75);
			e("assert");
		}
		for (s32 i = 0; i < MAX_FILES; i++)
		{
			if (fpointers[i] == NULL)
			{
				FILE* f = fopen(namebuf, mode == ReadBinary ? "rb" : "wb");
				if (f == NULL)
				{
					return NULL;
				}
				bytesleft = (u32)fpointers[i];
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

fileHandle NuMemFileOpen(void* buffer, u32 size, u32 mode)
{
	if (size > 0 && mode == ReadBinary)
	{
		for (s32 i = 0; i < MAX_MEM_FILES; i++) {
			MemFile* m = &memfiles[i];
			if (!m->open)
			{
				m->open = 1;
				m->end = (void*)((u32)buffer + (size - 1));
				m->position = buffer;
				m->unused = 0;
				m->start = buffer;
				return i + 0x400;
			}
		}
	}
	return 0;
}

s64 NuDatFilePos(fileHandle handle)
{
	return 0;
}

size_t NuDatFileRead(fileHandle handle, void* dest, size_t size)
{
	return 0;
}

s32 NuDatFileSeek(fileHandle handle, s64 off, s32 whence)
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
		memfiles[handle - 0x400].open = 0;
	}
	else
	{
		NuDatFileClose(handle);
	}
}

s64 NuMemFilePos(fileHandle handle)
{
	if (handle < 0x800)
	{
		return ((s64)memfiles[handle - 0x400].position - (s64)memfiles[handle - 0x400].start);
	}
	else
	{
		return NuDatFilePos(handle);
	}
}

size_t NuMemFileRead(fileHandle handle, void* dest, size_t size)
{
	if (handle < 0x800)
	{
		s32 f = handle - 0x400;
		s32 left = (s32)memfiles[f].end + (1 - (s32)memfiles[f].position);
		if (size > left)
		{
			size = left;
		}
		if (size != 0)
		{
			memcpy(dest, memfiles[f].position, size);
			memfiles[f].position = (void*)((u32)memfiles[f].position + size);
		}
		return size;
	}
	else
	{
		return NuDatFileRead(handle, dest, size);
	}
}

s32 NuMemFileSeek(fileHandle handle, s64 off, s32 whence)
{
	if (handle < 0x800)
	{
		fileHandle f = handle - 0x400;
		switch (whence)
		{
		case SEEK_SET:
			memfiles[f].position = (void*)((s32)memfiles[f].start + off);
			break;
		case SEEK_CUR:
			memfiles[f].position = (void*)((s32)memfiles[f].position + off);
			break;
		case SEEK_END:
			memfiles[f].position = (void*)((s32)memfiles[f].end + (1 - off));
			break;
		}
		return (s32)memfiles[f].position - (s32)memfiles[f].start;
	}
	else
	{
		return NuDatFileSeek(handle, off, whence);
	}
}

// I completely redid a part of this function, I hope it's right.
s64 NuFilePos(fileHandle handle)
{
	if (handle < 0x400)
	{
		u32 ret = thisbytesread;
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

s32 NuFileSeek(fileHandle handle, s64 off, s32 whence)
{
	if (handle < 0x400)
	{
		if (whence == SEEK_CUR)
		{
			off -= bytesleft;
		}
		bytesleft = 0;
		return fseek(fpointers[handle - 1], off, whence);
	}
	else
	{
		return NuMemFileSeek(handle, off, whence);
	}
}

s32 NuFileSize(char* fileName)
{
	s32 ret = -1;
	if (fileName != NULL && *fileName != 0)
	{
		if (NuFileExists(fileName))
		{
			fileHandle handle = NuFileOpen(fileName, ReadBinary);
			if (handle != NULL)
			{
				NuFileSeek(handle, 0, SEEK_END);
				ret = NuFilePos(handle);
				NuFileClose(handle);
			}
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
		fileHandle handle = NuFileOpen(fileName, ReadBinary);
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

size_t NuFileLoadBuffer(char* fileName, void* dest, s32 maxSize)
{
	size_t size = NuFileSize(fileName);
	if (size == 0)
	{
		error_func e = NuErrorProlog("OpenCrashWOC/code/nucore/nufile.c", 306);
		e("File %s does not exist!", fileName);
	}
	if (size > maxSize)
	{
		error_func e = NuErrorProlog("OpenCrashWOC/code/nucore/nufile.c", 309);
		e("Super Buffer out of space!");
	}
	else if (size != 0)
	{
		fileHandle handle = NuFileOpen(fileName, ReadBinary);
		if (handle != NULL)
		{
			NuFileRead(handle, &dest, size);
			NuFileClose(handle);
			return size;
		}
	}
	return 0;
}

size_t NuFileRead(fileHandle handle, void* dest, size_t size)
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
			memcpy(dest, bpointer, bytesleft);
			void* tmp = (void*)((u32)dest + bytesleft);
			thisbytesread += size;
			u32 read = size - bytesleft;
			size = bytesleft;
			while (read > 0x10000)
			{
				read -= 0x10000;
				size_t dat = fread(filebuffer, 1, 0x10000, fpointers[f]);
				size += dat;
				memcpy(tmp, filebuffer, 0x10000);
				tmp = (void*)((u32)tmp + 0x10000);
			}
			bytesleft = 0;
			if (read > 0)
			{
				size_t dat = fread(filebuffer, 1, 0x10000, fpointers[f]);
				memcpy(tmp, filebuffer, read);
				size += dat;
				bytesleft = dat - read;
				bpointer = (void*)((u32)filebuffer + read);
			}
		}
		else
		{
			memcpy(dest, bpointer, size);
			bpointer = (void*)((u32)bpointer + size);
			bytesleft -= size;
			thisbytesread += size;
		}
	}
	else
	{
		size = NuMemFileRead(handle, dest, size);
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
	}
	else
	{
		loadscreen = screen;
		datacounter = 0;
		datafull = 0;
	}
}

void NuStopLoadScreen()
{
	loadscreen = -1;
	loadscreenfadedir = 0;
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

u32 NuFileBeginBlkRead(fileHandle handle, u32 magic)
{
	u32 currBlock = blkcnt;
	blkcnt++;
	s64 pos = NuFilePos(handle);
	if (currBlock < 2 || pos < blkinfo[currBlock - 1].pos + blkinfo[currBlock - 1].size)
	{
		blkinfo[currBlock - 1].pos = pos;
		NuFileRead(handle, blkinfo + currBlock, 8);
		if (blkinfo[currBlock].size < 0)
		{
			blkinfo[currBlock].size = -blkinfo[currBlock].size;
		}
		if (magic != 0 && magic != blkinfo[currBlock].magic)
		{
			error_func e = NuErrorProlog("OpenCrashWOC/code/nucore/nufile.c", 455);
			e("NuFileBeginBlkRead : Block header mismatch!");
		}
		return blkinfo[currBlock].magic;
	}
	else
	{
		currBlock = 0;
		blkcnt--;
		return 0;
	}
}

void NuFileEndBlkRead(fileHandle handle)
{
	blkcnt--;
	s64 pos = NuFilePos(handle);
	u32 blkEnd = blkinfo[blkcnt].pos + blkinfo[blkcnt].size;
	if (pos != blkEnd)
	{
		NuFileSeek(handle, blkEnd, SEEK_SET);
	}
}

void NuDatClose(DatFile* datFile)
{
	NuFileClose(datFile->handle);
	if (datFile->fileId)
	{
		datfiles[datFile->fileId - 0x800].open = 0;
	}
	if (datFile->managedMem)
	{
		if (datFile->data != NULL)
		{
			NuMemFree(datFile->data);
		}
		NuMemFree(datFile);
	}
}