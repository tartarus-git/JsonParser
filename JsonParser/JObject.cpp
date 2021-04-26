#include "JObject.h"

#include <vector>
#include <utility>

#include "Stream.h"
#include "whitespace.h"
#include "value.h"

bool JObject::parse(Stream& stream) {
	char character;
	while (true) {
		if (stream.readChar(&character)) {																		// Only continue if stream hasn't ended.
			if (filterWhitespace(character)) { continue; }														// Filter out all the whitespace.
			if (character == '}') { return true; }																// Angel bracket = object end, so end object.

			JPair newPair;
			EndingType ending = newPair.parse(character, stream);												// Parse a JPair (key-value pair).
			content.push_back(std::move(newPair));																// std::move JPair into content so that JPair finalizer doesn't mess everything up.
			if (ending == EndingType::object) { return true; }													// If the JPair ends with angel brackets, close this object.
			continue;
		}
		content.clear();																						// If stream ends before JObject could be closed, clear content vector and return false.
		return false;
	}
}

void JObject::release() { content.clear(); }																	// Clear the content vector when this JObject is released.
																												// Note: This doesn't mean that the vector is completely deallocated.
																												// The container stuff in vector may still be alive until the destructor is called.
																												// Same is true for the actual allocated memory for the content:
																												// JPair destructors are called but the memory may still be there.
																												// Bottom line: Don't assume that the content vector is just an empty hull.

// No need for a finalizer because the content vector automatically destructs itself. There would be nothing to do in a finalizer here.