#include "JArray.h"

#include "LinkedList.h"
#include "TypedVoidPtr.h"
#include "value.h"

#include <iostream>

void JArray::parse(Stream& stream) {
	LinkedList<TypedVoidPtr> buffer;
	char character;
	while (stream.readChar(character)) {
		TypedVoidPtr value;
		//parseValue(stream, value);							// Is this good practice? Cuz you can't easily see that it's going to be modified. Should I put an & in for cosmetic reasons? That would ruin the point of having no pointer right? TODO.
		// TODO: Just copied it cuz lazy, TODO still applies.
		if (parseValue(stream, value) == EndingType::array) { break; }
	}
	content = buffer.toArray();
}

JArray::~JArray() {
	delete content;
}