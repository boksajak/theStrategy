//----------------------------------------------------------------------------------------
/**
* \file       SafeCRT.h
* \author     Martin Krošlák
* \date       2014/24/03
* \brief      Safe C functions.
*
*  Provides functions that wrap MSVC-specific secure functions and thus help
*  avoid problems with CRT warnings.
*
*/
//----------------------------------------------------------------------------------------
#pragma once

#include <cerrno>
#include <cstdio>
#include <cstdarg>

errno_t strcat_safe(char* dst, size_t size, char const* src);
errno_t strcpy_safe(char* dst, size_t size, char const* src);
char*   strtok_safe(char* str, char const* delims, char** context);
errno_t fopen_safe(FILE** pFile, char const* filename, char const* mode);

int sprintf_safe(char* buffer, size_t bufferSize, char const* format, ...);
int vsprintf_safe(char* buffer, size_t bufferSize, char const* format, va_list args);

template <size_t size>
errno_t strcat_safe(char (&dst)[size], char const* src)
{
	return strcat_safe(dst, size, src);
}

template <size_t size>
errno_t strcpy_safe(char (&dst)[size], char const* src)
{
	return strcpy_safe(dst, size, src);
}

template <size_t size>
int sprintf_safe(char (&buffer)[size], char const* format, ...)
{
	va_list args;
	va_start(args, format);
	int res = vsprintf_safe(buffer, size, format, args);
	va_end(args);
	return res;
}

template <size_t size>
int vsprintf_safe(char (&buffer)[size], char const* format, va_list args)
{
	return vsprintf_safe(buffer, size, format, args);
}
