#include "FileStream.h"

#include <iostream>
#include <fstream>
#include <utility>

std::ifstream f;

FileStream::FileStream(const char* path) {
	f = std::ifstream(path, std::ios::in);
}

FileStream::FileStream(FileStream&& other) noexcept : f(std::move(other.f)) { }

FileStream& FileStream::operator=(FileStream&& other) noexcept {
	f = std::move(other.f);
	return *this;
}

int FileStream::read(char* buffer, int length) {
	f.read(buffer, length);
	return (int)f.gcount();
}

int FileStream::skip(int amount) {
	f.seekg(amount, std::ios::cur);
	return (int)f.gcount();
}

void FileStream::release() { f.close(); }

FileStream::~FileStream() { release(); }