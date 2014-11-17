//----------------------------------------------------------------------------------------
/**
* \file       BinaryFile.h
* \author     Jakub Bokšanský
* \date       2013/30/12
* \brief      Binary file reader/writer
*
*  File stream wrapper for reading and writing binary files
*
*/
//----------------------------------------------------------------------------------------
#pragma once

#include <stdio.h>
#include <iostream>
#include "assert.h"

class BinaryFile {
public:

	BinaryFile() : fileStream(NULL) { }

	virtual ~BinaryFile() {
		if (fileStream != NULL) {
			fclose(fileStream);
			fileStream = NULL;
		}
	}

	errno_t openFileReadBinary(const char* fileName) {
		assert(fileStream == NULL);

		mode = READ;
		return fopen_s(&fileStream, fileName, "rb");
	}

	errno_t openFileWriteBinary(const char* fileName) {
		assert(fileStream == NULL);

		mode = WRITE;
		return fopen_s(&fileStream, fileName, "wb");
	}

	size_t getFileSize() {
		assert(fileStream != NULL && mode == READ);

		size_t count = 0;

		fseek(fileStream, 0, SEEK_END);
		count = ftell(fileStream);
		rewind(fileStream);

		return count;
	}

	template <class Type>
	errno_t read(Type* buffer) {
		assert(fileStream != NULL && mode == READ);

		if (1 != fread(buffer, sizeof(Type), 1, fileStream)) {
			return ferror(fileStream);
		}

		return 0;
	}

	template <class Type>
	errno_t readArray(Type* buffer, size_t count) {
		assert(fileStream != NULL && mode == READ);

		if (count != fread(buffer, sizeof(Type), count, fileStream)) {
			return ferror(fileStream);
		}

		return 0;
	}

	inline bool eof() {
		return feof(fileStream) > 0;
	}

	void close() {
		if (fileStream) {
			fclose(fileStream);
			fileStream = NULL;
		}
	}

	static char const* getFileError(errno_t fileErr) {
		switch (fileErr) {
		case ENOENT:
			return "File doesn't exist.";
			break;
		case EPERM:
			return "Operation not permitted.";
			break;
		case EIO:
			return "I/O error.";
			break;
		case EACCES:
			return "Permission denied.";
			break;
		case EISDIR:
			return "Is a directory.";
			break;
		case ENFILE:
		case EMFILE:
			return "Too many open files.";
			break;
		case EFBIG:
			return "File too big.";
			break;
		case EROFS:
			return "Read-only file system.";
			break;
		case ENAMETOOLONG:
			return "File name too long.";
			break;
		}
	}

protected:

	FILE* fileStream;

	enum {
		READ,
		WRITE
	} mode;
};
