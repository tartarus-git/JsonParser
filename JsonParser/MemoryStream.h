#pragma once

#include "Stream.h"

class MemoryStream : public Stream
{
	int pos = 0;
	int length;
	const char* data;

public:
	MemoryStream(const char* string);

	int read(char* buffer, int length);

	int skip(int amount);
};

