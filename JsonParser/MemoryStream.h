#pragma once
class MemoryStream
{
	int pos = 0;
	int length;
	const char* data;

public:
	MemoryStream(const char* string);

	int read(char* buffer, int length);

	int skip(int amount);
};

