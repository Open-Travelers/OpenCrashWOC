#ifndef NUCORETYPES_H
#define NUCORETYPES_H

#include "types.h"

enum nufilemode_e
{
	NUFILE_MODE_CNT = 3,
	NUFILE_APPEND = 2,
	NUFILE_WRITE = 1,
	NUFILE_READ = 0
};

enum nufileseek_e
{
	NUFILE_SEEK_CNT = 3,
	NUFILE_SEEK_END = 2,
	NUFILE_SEEK_CURRENT = 1,
	NUFILE_SEEK_START = 0
};

// Size: 0x8
struct memexternal_s
{
	union variptr_u* ptr;
	union variptr_u* end;
};

// Size: 0x10
struct nudatinfo_s
{
	s32 foffset;
	s32 flen;
	s32 uplen;
	s32 ppack : 1; // Offset: 0xC, Bit Offset: 0, Bit Size: 1
};

// Size: 0x8
struct nudfnode_s
{
	short childix;
	short sibix;
	char* txt;
};

// Data file, but this doesn't even need to exist.
// Size: 0x30
struct nudathdr_s
{
	s32 ver;
	s32 nfiles;
	struct nudatinfo_s* finfo;
	s32 treecnt;
	struct nudfnode_s* filetree;
	s32 leafnamesize;
	char* leafnames;
	s32 dfhandle;
	s32 fh;
	short intalloc;
	short openmode;
	s32 start_lsn;
	void* memdatptr;
};

// Size: 0x14
struct nudatfile_s
{
	struct nudathdr_s* ndh;
	int start;
	s32 len;
	s32 fix;
	s32 used;
};

// Memory file. // Size: 0x14
struct numemfile_s
{
	char* start;
	char* end;
	char* currposition;
	enum nufilemode_e mode;
	s32 used;
};

struct fileinfo_s
{
	s32 fh;
	s32 read_pos;
	s32 file_pos;
	s32 file_length;
	s32 buff_start;
	s32 buff_length;
	s32 use_buff;
	struct filebuff_s* buff;
};

struct filebuff_s
{
	void* unk;
};

// Size: 0x8
struct nuiffhdr_s
{
	s32 blk;
	s32 size;
};

// Size: 0xC
struct BlockInfo
{
	struct nuiffhdr_s hdr;
	s32 pos;
};

/*OLD
typedef struct
{
	fileHandle handle;
	u8 buffer[0x1000];
	char textBuffer[0x100];
	char wordBuffer[0x104];
	s32 line_num;
	u32 linePos;
	u32 pos;
	s32 bufferPos;
	s32 bufferEndPos;
	u32 f7;
	s32 commandStack[7]; // The last entry is the index of the last entry, where -1 is null.
	struct FParCommand* fpCmd;
	u32 fileLength;
} FPar;

typedef struct
{
	char* str;
	void (*cb)(FPar*);
} FParCommand;*/

typedef struct nufpar_s nufpar_s, *Pnufpar_s;

typedef struct nufpcomjmp_s nufpcomjmp_s, *Pnufpcomjmp_s;

struct nufpcomjmp_s {
    char * fname;
    void (*func)(nufpar_s*);
};

struct nufpar_s {
    fileHandle handle; /* fH */
    u8 buffer[4096]; /* fbuff */
    char textBuffer[256]; /* lbuff */
    char wordBuffer[260];
    s32 line_num;
    s32 linepos;
    s32 cpos;
    s32 buffstart;
    s32 buffend;
    struct nufpcomjmp_s * comstack[8];
    s32 compos;
    s32 size;
};

typedef struct nufparpos_s nufparpos_s, *Pnufparpos_s;

struct nufparpos_s {
    s32 line_num;
    s32 line_pos;
    s32 cpos;
    s32 buffstart;
    s32 buffend;
};



#endif // !NUCORETYPES_H
