#pragma once

#include "Stream.h"
#include "JObject.h"

class JsonParser
{
public:
	JsonParser() = delete;

	static JObject parse(Stream& stream);						// The & is necessary because copying would require instantiating, which we can't do with an abstract class. This just uses the already existing instance using the reference thing.
};

