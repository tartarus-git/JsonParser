#include "MemoryStream.h"

#include <cstring>

MemoryStream::MemoryStream(const char* string) {
	data = string;
	length = strlen(data);
}

int MemoryStream::read(char* buffer, int amount) {
	if (pos + amount > length) { amount = length - pos; }
	memcpy(buffer, data + pos, amount);									// TODO: Find out what void * means in c++. Also TODO: Is it ok to pass in a 0 here on the edge cases or should I put an if in for safety or something.
	pos += amount;
	return amount;
}

int MemoryStream::skip(int amount) { pos += amount; }