#pragma once

#include <vector>

#include "TypedVoidPtr.h"
#include "EndingType.h"
#include "Stream.h"

class JPair
{
public:
	std::vector<char> key;
	TypedVoidPtr value;

	JPair() = default;

	JPair(JPair&& other) noexcept;
	JPair& operator=(JPair&& other) noexcept;

	EndingType parse(char character, Stream& stream);

	void release();
	~JPair();
};

