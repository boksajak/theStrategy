//----------------------------------------------------------------------------------------
/**
* \file       TextFile.h
* \author     Jakub Bokšanský
* \date       2013/30/12
* \brief      Text file reader/writer
*
*  File stream wrapper for reading and writing text files
*
*/
//----------------------------------------------------------------------------------------
#pragma once

#include "Utils\BinaryFile.h"

class TextFile {
public:

	TextFile(const char* fileName) : contents(NULL), opened(false) {
		if (!file.openFileReadBinary(fileName)) {
			opened = true;
		}
	}

	~TextFile() {
		if (contents != NULL) {
			delete[] contents;
		}
	}

	errno_t ReadFile(char* &target) {

		if (!opened) return -1;

		if (contents != NULL) {
			target = contents;
			return 0;
		}

		size_t fileSize = file.getFileSize();
		contents = new char[fileSize + 1]; //< Add 1 for terminal zero byte
		errno_t err = file.readArray(contents, fileSize);

		if (err) return err;

		// Add zero byte so we have C-style zero-terminated string
		contents[fileSize] = '\0';

		target = contents;

		return 0;
	}


private:
	BinaryFile file;
	char* contents;
	bool opened;
};
