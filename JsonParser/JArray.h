#pragma once

#include <vector>

#include "Stream.h"
#include "TypedVoidPtr.h"

class JArray
{
public:
	std::vector<TypedVoidPtr> content;

	bool parse(Stream& stream);

	void release();
};