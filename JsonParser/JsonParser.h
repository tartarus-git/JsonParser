#pragma once

#include "Stream.h"

class JsonParser
{
public:
	JsonParser() = delete;

	static void parse(Stream& stream);						// The & is necessary because copying would require instantiating, which we can't do with an abstract class. This just uses the already existing instance using the reference thing.
};

