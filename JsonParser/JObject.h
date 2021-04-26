#pragma once

#include <vector>

#include "JPair.h"
#include "Stream.h"

class JObject
{
public:
	std::vector<JPair> content;

	// No move semantics are defined becaue the lack of a destructor negates the need for explicitly defined move semantics. Implicity move semantics will do just fine here. Absolutely fine.

	bool parse(Stream& stream);

	void release();
};