#include "nuerror.h"

char txt[0x400];
char captxt[0x100];
char* nufile;
u32 nuline;

void NuErrorFunction(char message[0x60], ...)
{
	sprintf(captxt, "NuError - %s Line %d : ", nufile, nuline);
	puts(captxt);
	va_list aptr;
	va_start(aptr, message);
	vsprintf(txt, message, aptr);
	puts(txt);
	va_end(aptr);
	exit(1);
}

error_func NuErrorProlog(char* file, u32 line)
{
	nufile = file;
	nuline = line;
	return NuErrorFunction;
}