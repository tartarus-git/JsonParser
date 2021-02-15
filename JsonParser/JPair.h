#pragma once

#include "JSubParser.h"
#include "Stream.h"

// Return type indicating whether the value was ended by an object ending, array ending or comma.
enum EndingType {
	comma = 0,
	object = 1,
	array = 2
};

class JPair// : public JSubParser				// TODO: Is this thing even necessary? Where are we going to generalize and going to need this?
{
public:
	char* key;
	void* value;

	EndingType parse(char character, Stream& stream);
};

