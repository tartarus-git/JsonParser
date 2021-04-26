#pragma once

class Stream							// TODO: You can probably use some sort of standard thing like iostream instead of using this Stream class for this entire project. Would make more sense.
{
public:
	virtual int read(char* buffer, int amount) = 0;					// Virtual means that the derived function will be called even when calling through base class reference, which is imperative in this case.
	bool readChar(char* character);

	virtual int skip(int amount) = 0;								// Advances the stream by the specified number of characters without reading.
};