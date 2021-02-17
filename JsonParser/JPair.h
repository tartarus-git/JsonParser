#pragma once

#include "JSubParser.h"
#include "Stream.h"
#include "value.h"
#include "TypedVoidPtr.h"

class JPair// : public JSubParser				// TODO: Is this thing even necessary? Where are we going to generalize and going to need this?
{
public:
	char* key;
	TypedVoidPtr value;

	EndingType parse(char character, Stream& stream);

	~JPair();
};

