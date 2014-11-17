#include "SafeCRT.h"
#include <cstring>

errno_t strcat_safe(char* dst, size_t count, char const* src)
{
    return strcat_s(dst, count, src);
}

errno_t strcpy_safe(char* dst, size_t size, char const* src)
{
    return strcpy_s(dst, size, src);
}

char*   strtok_safe(char* str, char const* delims, char** context)
{
    return strtok_s(str, delims, context);
}

errno_t fopen_safe(FILE** pFile, char const* filename, char const* mode)
{
    return fopen_s(pFile, filename, mode);
}

int sprintf_safe(char* buffer, size_t bufferSize, char const* format, ...)
{
	va_list args;
	va_start(args, format);
	int res = vsprintf_safe(buffer, bufferSize, format, args);
	va_end(args);
	return res;
}

int vsprintf_safe(char* buffer, size_t bufferSize, char const* format, va_list args)
{
	return vsprintf_s(buffer, bufferSize, format, args);
}

