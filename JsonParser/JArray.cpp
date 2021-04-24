#include "JArray.h"

#include <utility>

#include "LinkedList.h"
#include "TypedVoidPtr.h"
#include "value.h"

bool JArray::parse(Stream& stream) {
	LinkedList<TypedVoidPtr> buffer;
	while (true) {																			// No reading of anything here, just continual parsing of values.
		TypedVoidPtr value;
		EndingType ending = parseValue(stream, value);										// Parse a value in the array.

		switch (ending) {
		case EndingType::error:																// If the ending type of the parsed value is that of an error, release buffer and return failure.
			buffer.reset();
			return false;
		case EndingType::array:																// If ending is that of an array, finalize and close this array.
			content = buffer.toArray();
			length = buffer.length;
			return true;																	// Return sucess.
		}
		buffer.add(std::move(value));																	// If everything is fine but we haven't reached the end of the array yet, just add the new value to the buffer.
	}
	// We don't need to put a return failure here because we don't actually do any parsing in this function, the values themselves take care of everything for us.
}

void JArray::release() {
	if (length) {
		delete[] content;
		length = 0;
	}
}

JArray::~JArray() {
	delete content;
}