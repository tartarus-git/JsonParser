#pragma once

#include "JPair.h"
#include "Stream.h"

class JObject// : public JSubParser
{
public:
	JPair* content;
	int length;

	void parse(Stream& stream);
};