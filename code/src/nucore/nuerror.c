#include "nuerror.h"

char txt[0x400];
char captxt[0x100];
char* nufile;
u32 nuline;
u32 err_msg_quiet = 0;
u32 initialised = 0;
u32 dbg_msg_cnt = 0;
char* errfile;
char* errfilepath;
u32 errline;
u32 allow_printf = 1;
u32 errmsg_to_file = 0;

static void NuErrorFunction(char* msg, ...)
{
	printf(captxt, "NuError - %s Line %d : ", nufile, nuline);
	puts(captxt);
	va_list aptr;
	va_start(aptr, msg);
	vsprintf(txt, msg, aptr);
	puts(txt);
	va_end(aptr);
	exit(1);
}

// PS2 MATCH
static void NuDebugMsgFunction(char* msg, ...)
{
	int fd;
	size_t len;
	char buf[0x400];
	char buf2[0x400];
	va_list aptr;

	if (err_msg_quiet == 0) {
		va_start(aptr, msg);
		NuDisableVBlankE();
		if (!initialised) {
			initialised = 1;
			fd = NuFileOpen("nu2.err", 1);
			if (fd != 0)
				NuFileClose(fd);
		}

		dbg_msg_cnt++;
		sprintf(buf2, "%05d NuDebugMsg - %s(%d) : ", dbg_msg_cnt, errfile, errline);
		vsprintf(buf, msg, aptr);
		strcat(buf2, buf);
		strcat(buf2, "\r\n");
		va_end(aptr);
		if (errmsg_to_file == 0) {
			if (allow_printf == 1)
				printf(buf2);
		} else {
			err_msg_quiet = 1;
			fd = NuFileOpen("nu2.err", 2);
			if (fd == 0)
				fd = NuFileOpen("nu2.err", 1);
			else
				NuFileSeek(fd, 0, 2);

			if (fd == 0)
				err_msg_quiet = 1;
			else {
				len = strlen(buf2);
				NuFileWrite(fd, buf2, len);
				NuFileClose(fd);
				err_msg_quiet = 0;
			}
		}
		NuEnableVBlankE();
	}
}

error_func* NuErrorProlog(char* file, s32 line, ...)
{
	nufile = file;
	nuline = line;
	return NuErrorFunction;
}

// PS2 MATCH 
error_func* NuDebugMsgProlog(char* file, s32 line, ...)
{
	char c;

	errfilepath = file;
	errline = line;
	c = strrchr(file, '\\');
	errfile = file;
	if (c != '\0')
		errfile = c + 1;
	return NuDebugMsgFunction;
}
