#pragma once

#include "Utils/SafeCRT.h"

class StringBuilder {
public:

	StringBuilder(size_t maxLength) : maxLength(maxLength), currentLength(0) {
		string = new char[maxLength + 1]; //< +1 for extra terminal zero byte
		string[0] = 0;
	}

	~StringBuilder() {
		delete string;
	}

	void Append(const char* newString) {
		size_t newStringLength = strlen(newString);

		assert (newStringLength + currentLength <= maxLength);

		currentLength += newStringLength;

		strcat_safe(string, maxLength, newString);
	}

	void Append(const int number) {
		char buffer[33];
		sprintf_safe(buffer, "%d" , number);
		Append(buffer);
	}

	void Append(const char* string1, const int number,const char* string2) {
		Append(string1);
		Append(number);
		Append(string2);
	}

	void Append(const char* string1, const int number1,const char* string2,  const int number2) {
		Append(string1);
		Append(number1);
		Append(string2);
		Append(number2);
	}

	void Append(const char* string1, const char* string2) {
		Append(string1);
		Append(string2);
	}

	char* GetString() const {
		return string;
	}

	void Clear() {
		currentLength = 0;
		string[0] = 0;
	}

private:
	size_t maxLength;
	size_t currentLength;

	char* string;
};