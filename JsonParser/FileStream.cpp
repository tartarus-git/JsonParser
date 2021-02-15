#include "FileStream.h"

std::ifstream f;

FileStream::FileStream(const char* path) {
	f = std::ifstream(path, std::ios::in);
}

int FileStream::read(char* buffer, int length) {
	f.read(buffer, length);
	return (int)f.gcount();					// TODO: Check that this actually does what you need it to.
}

int FileStream::skip(int amount) {
	f.seekg(amount, std::ios::cur);
}

FileStream::~FileStream() {
	f.close();
}