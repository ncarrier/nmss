#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "error.h"

void error(int status, int errnum, const char *format, ...)
{
	va_list args;
	FILE *output = stdout;

	va_start(args, format);
	vfprintf(output, format, args);
	va_end(args);
	if (errnum != 0)
		fprintf(output, ": %s\n", strerror(errnum));
	else
		fputs("\n", output);

	exit(status);
}

