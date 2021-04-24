#include "JPair.h"

#include <cstdint>

#include "Stream.h"
#include "LinkedList.h"
#include "whitespace.h"
#include "value.h"

EndingType JPair::parse(char character, Stream& stream) {
	bool quotations = character == '\"';													// Make sure to account for keys with quotations around them.

	// Parse key.
	LinkedList<char> buffer(character);
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
					buffer.reset();															// If stream ends before we're ready, release buffer and return error.
					return EndingType::error;
				}
			}
			if (character == ':') { break; }												// Colon marks end of key, so break.
			buffer.add(character);															// Read the next character and add it to the key.
			continue;
		}
		buffer.reset();																		// If the stream ends before proper closure, release buffer and return EndingType::error.
		return EndingType::error;
	}

keyEnd:																						// Finalize key and hand over control to the value parser.
	buffer.add('\0');
	key = buffer.toArray();
	// TODO: Make sure to release the array in the finalizer of JPair and to release manually somewhere too maybe.

	return parseValue(stream, value);														// Return ending type of parsed value.
}

void JPair::release() {
	if (key) {																					// Make sure this is the first time releasing.
		delete[] key;																			// Delete the char array that's responsible for storing the key.
		value.release();																		// Release the pointer, whatever type it may be.
		key = nullptr;																			// Set the key to null so that multiple calls to release won't break anything.
	}
}

JPair::~JPair() { release(); }