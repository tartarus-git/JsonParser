#pragma once

#include "LinkedList.h"
#include "Stream.h"

class JArray
{
public:
	void* content;

	void parse(Stream& stream);

	~JArray();
};

