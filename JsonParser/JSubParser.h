#pragma once

#include "Stream.h"

class JSubParser
{
public:
	virtual void parse(Stream stream) = 0;					// Setting this to 0 is like making it abstract. This will implicitly make the whole class abstract. The fact that that is implicit is a little dumb in my opinion.
};