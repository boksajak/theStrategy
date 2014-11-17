//----------------------------------------------------------------------------------------
/**
* \file       BufferedBinaryFile.h
* \author     Jakub Bokšanský
* \date       2014/07/03
* \brief      Buffered Binary file reader/writer
*
*  File stream wrapper for reading and writing binary files.
*  Employs additional buffering to avoid multiple fread calls (which can be slow 
*  even when fread itself is buffered)
*
*/
//----------------------------------------------------------------------------------------
#pragma once

#include <stdio.h>
#include <iostream>
#include "assert.h"

class BufferedBinaryFile : BinaryFile {
public:

	BufferedBinaryFile(size_t bufferSize) : buffer(NULL), bufferSize(bufferSize), currentIdx(0) { }

	~BufferedBinaryFile() {
		BinaryFile::close();

		if (buffer != NULL) {
			delete[] buffer;
			buffer = NULL;
		}
	}

	errno_t openFileReadBinary(const char* fileName) {
		errno_t err = BinaryFile::openFileReadBinary(fileName);

		if (err) return err;

		if (!buffer) {
			buffer = new char[bufferSize];
		}

		if (!buffer) return -1;

		fread(buffer, sizeof(char), bufferSize, fileStream);

		return 0;
	}

	template <class Type>
	errno_t read(Type* targetBuffer) {
		assert(fileStream != NULL && mode == READ);
		assert(sizeof(Type) <= bufferSize/*, "Type cannot be larger than buffer"*/);
		errno_t result = 0;

		if (currentIdx + sizeof(Type) <= bufferSize) {
			memcpy(targetBuffer, buffer + currentIdx, sizeof(Type));
			currentIdx += sizeof(Type);
		} else {

			size_t overflow = bufferSize - currentIdx;

			if (overflow) {
				memcpy(buffer, buffer + currentIdx, overflow);
			}

			fread(buffer + overflow, sizeof(char), bufferSize - overflow, fileStream);
			if (result = ferror(fileStream)) return result;

			memcpy(targetBuffer, buffer, sizeof(Type));

			currentIdx = sizeof(Type);

		} 

		return result;
	}

	template <class Type>
	errno_t readArray(Type* targetBuffer, size_t count) {
		assert(fileStream != NULL && mode == READ);
		assert(sizeof(Type) <= bufferSize/*, "Type cannot be larger than buffer"*/);
		errno_t result = 0;
		size_t copied = 0;

		while (count > 0) {
			size_t toCopy = bufferSize - currentIdx;
			if (count < toCopy) toCopy = count;

			memcpy(targetBuffer + copied, buffer + currentIdx, toCopy * sizeof(Type));
			copied += toCopy;
			currentIdx += toCopy;
			count -= toCopy;

			if (currentIdx == bufferSize) {
				fread(buffer, sizeof(char), bufferSize, fileStream);
				if (result = ferror(fileStream)) return result;

				currentIdx = 0;
			}
		}

		return result;
	}

	void close() {
		BinaryFile::close();
		currentIdx = 0;
	}

private:

	char* buffer;
	size_t currentIdx;
	size_t bufferSize;

};