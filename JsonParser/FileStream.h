#pragma once

#include <iostream>
#include <fstream>

#include "Stream.h"

class FileStream : public Stream
{
	std::ifstream f;

public:
	FileStream(const char* path);
	FileStream(FileStream&& other) noexcept;

	FileStream& operator=(FileStream&& other) noexcept;										// TODO: Is it good practice to put noexcept on move assignment operators?

	int read(char* buffer, int amount);

	int skip(int amount);

	void release();
	~FileStream();
};