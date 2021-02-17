#pragma once

#include "Stream.h"
#include "value.h"

class JSubParser
{
public:
	virtual EndingType parse(char character, Stream& stream) = 0;					// Setting this to 0 is like making it abstract. This will implicitly make the whole class abstract. The fact that that is implicit is a little dumb in my opinion.
};