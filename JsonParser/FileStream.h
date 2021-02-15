#pragma once

#include <iostream>
#include <fstream>

#include "Stream.h"

class FileStream : public Stream						// Public means that all the public members will be accessible as public members through the derived class, which is useful in this case. TODO: Find out if actually useful because public.
{
	std::ifstream f;

public:
	FileStream(const char* path);

	int read(char* buffer, int amount);

	~FileStream();
};

