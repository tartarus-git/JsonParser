#pragma once

#include "JPair.h"
#include "Stream.h"

class JObject
{
public:
	JPair* content;
	int length;

	bool parse(Stream& stream);

	void release();
	~JObject();
};