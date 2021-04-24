#include "JObject.h"

#include <utility>


#include "LinkedList.h"
#include "Stream.h"
#include "whitespace.h"

bool JObject::parse(Stream& stream) {
	LinkedList<JPair> buffer;										// LinkedList's are pretty bad most of the time I think. We should definitely use std::vector for this. TODO.
	char character;
	while (true) {
		if (stream.readChar(&character)) {																		// Only continue if stream hasn't ended.
			if (filterWhitespace(character)) { continue; }														// Filter out all the whitespace.
			if (character == '}') {																				// Angel bracket = object end, so end object.
				length = 0;
				return true;
			}

			JPair newPair;
			EndingType ending = newPair.parse(character, stream);												// Parse a JPair (key-value pair).
			buffer.add(std::move(newPair));																				// Add newly parsed pair to the buffer for this object.
			if (ending == EndingType::object) {																	// If the JPair ends with angel brackets, close this object.
				content = buffer.toArray();
				length = buffer.length;
				return true;
			}
			continue;
		}
		buffer.reset();																							// If stream ends before JObject could be closed, release buffer and return false.
		return false;
	}
}

void JObject::release() {
	if (length) {
		delete[] content;
		length = 0;
	}
}

JObject::~JObject() { release(); }