#include "JsonParser.h"

#include <cstring>

MemoryStream::MemoryStream(const char* string) {
	data = string;
	length = strlen(data);
}

int MemoryStream::read(char* buffer, int amount) {
	if (pos + amount > length) { amount = length - pos; }
	memcpy(buffer, data + pos, amount);
	pos += amount;
	return amount;
}

int MemoryStream::skip(int amount) {									// TODO: This skip code makes absolutely no sense whatsoever. I don't know why this is like this. Fix this eventually.
	int newPos = pos + amount;
	if (newPos > length) { return length - pos; }
	if (newPos < 0) { return -pos; }
	return amount;
}