#pragma once

#include "LinkedList.h"
#include "Stream.h"
#include "TypedVoidPtr.h"

class JArray
{
public:
	TypedVoidPtr* content;
	int length;

	bool parse(Stream& stream);

	~JArray();
};