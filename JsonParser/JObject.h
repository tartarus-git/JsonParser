#pragma once

#include "JSubParser.h"
#include "LinkedList.h"
#include "JPair.h"
#include "Stream.h"

class JObject : public JSubParser
{
	LinkedList<JPair> content;

public:
	void parse(Stream& stream);
};

