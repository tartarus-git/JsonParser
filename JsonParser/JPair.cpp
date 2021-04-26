#include "JPair.h"

#include "JsonParser.h"

JPair::JPair(JPair&& other) noexcept : key(std::move(other.key)), value(other.value) {
	other.value.type = ValueType::null;
}

JPair& JPair::operator=(JPair&& other) noexcept {
	key = std::move(other.key);
	value = other.value;
	other.value.type = ValueType::null;
	return *this;
}

EndingType JPair::parse(char character, Stream& stream) {
	bool quotations = character == '\"';													// Make sure to account for keys with quotations around them.

	// Parse key.
	key.push_back(character);																// Add the first (already-known, from caller) character to the key vector.
	while (true) {
		if (stream.readChar(&character)) {													// If the stream has not ended, parse.
			if (filterWhitespace(character)) { continue; }									// Filter out the whitespace.

			if (quotations && character == '\"') {											// If quotation mode is on, quotation marks end of key, so skip past upcoming colon and start parsing value.
				while (true) {
					if (stream.readChar(&character)) {										// If stream hasn't ended, keep preparing for value parsing.
						if (filterWhitespace(character)) { continue; }						// Filter whitespace.
						if (character == ':') { goto keyEnd; }								// When we hit the colon, we can hand over control to the value parser.
						continue;
					}
					key.clear();															// If stream ends before we're ready, clear key vector and return error.
					return EndingType::error;												// Vector will automatically destruct whatever's left so it doesn't matter what happens to JPair from here.
				}
			}
			if (character == ':') { break; }												// Colon marks end of key, so break.
			key.push_back(character);														// Read the next character and add it to the key.
			continue;
		}
		key.clear();																		// If the stream ends before proper closure, clear key vector and return EndingType::error.
		return EndingType::error;
	}

keyEnd:																						// Finalize key and hand over control to the value parser.
	key.push_back('\0');
	return parseValue(stream, value);														// Return ending type of parsed value.
}

void JPair::release() {
	key.clear();																		// Clear the key vector.
	value.release();																	// Release the pointer, whatever type it may be.
	value.type = ValueType::null;														// Prevent double release by setting value type to null.
}

JPair::~JPair() {
	value.release();																		// We're writing this again so that we can avoid an unnecessary key.clear() since vectors destruct.
	value.type = ValueType::null;
}