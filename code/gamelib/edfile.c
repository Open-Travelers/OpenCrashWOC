#include "edfile.h"
#include "../nu.h"

fileHandle edfile_handle = -1;

u32 EdFileOpen(char* filename)
{
	if (edfile_handle == -1)
	{
		edfile_handle = NuFileOpen(filebuffer, ReadBinary);
		if (edfile_handle != -1)
		{
			return 1;
		}
	}
	return 0;
}

u32 EdFileClose()
{
	u32 fileOpened = edfile_handle != -1;
	if (fileOpened) {
		NuFileClose(edfile_handle);
		edfile_handle = -1;
	}
	return fileOpened;
}

void EdFileRead(void* dest, size_t size)
{
	NuFileRead(edfile_handle, dest, size);
}

f32 EdFileReadFloat()
{
	return NuFileReadFloat(edfile_handle); // Makes more sense than not calling and doing the hard way.
}

s32 EdFileReadInt()
{
	return NuFileReadInt(edfile_handle); // Makes more sense than not calling and doing the hard way.
}

s16 EdFileReadShort()
{
	return NuFileReadShort(edfile_handle); // Makes more sense than not calling and doing the hard way.
}

char EdFileReadChar()
{
	char tmp = 0;
	NuFileRead(edfile_handle, &tmp, 1);
	return tmp;
}