#pragma once

#include "Stream.h"

// This class doesn't release/destruct/finalize anything so we don't need any move semantics for it.
// The implicit copy assignment operator is totally fine for this class since it's up to the user to dispose his data in this case.
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