#pragma once

#include <iostream>
#include <fstream>

#include "Stream.h"

class FileStream : public Stream
{
	std::ifstream f;

public:
	FileStream(const char* path);

	int read(char* buffer, int amount);

	int skip(int amount);

	~FileStream();
};

