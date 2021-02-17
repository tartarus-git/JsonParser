#pragma once

#include "LinkedList.h"
#include "Stream.h"
#include "TypedVoidPtr.h"

class JArray
{
public:
	TypedVoidPtr* content;

	void parse(Stream& stream);

	~JArray();
};