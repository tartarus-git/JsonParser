#include "JPair.h"

#include "Stream.h"
#include "LinkedList.h"
#include "whitespace.h"
#include "value.h"

EndingType JPair::parse(char character, Stream& stream) {
	bool quotations = character == '\"';												// Make sure to account for keys with quotations around them.

	// Parse key.
	LinkedList<char> buffer(character);
	while (stream.readChar(character)) {
		if (filterWhitespace(character)) { continue; }									// Filter out the whitespace.

		if (quotations && character == '\"') {											// If quotation mode is on, quotation marks end of key, so skip past upcoming colon and break.
			while (stream.readChar(character)) {				// TODO: Make sure that a break here doesn't break the whole thing.
				if (filterWhitespace(character)) { continue; }
				if (character == ':') { break; }
			}
			break;
		}
		if (character == ':') { break; }												// Colon marks end of key, so break.
		buffer.add(character);															// Read the next character and add it to the key.
	}
	key = buffer.toArray();																// Store the linked list as an array permanently for efficient random access.
	// TODO: Make sure to release the array in the finalizer of JPair and to release manually somewhere too maybe.

	// Parse value.
	return parseValue(stream, value);
}

JPair::~JPair() {
	delete[] key;
	delete value.pointer;					// TODO: This is a major no-no and causes all sorts of memory leaks. Find a creative solution for the releasing of all this stuff.
}