#include "JPair.h"

#include "Stream.h"
#include "LinkedList.h"
#include "whitespace.h"

EndingType JPair::parse(char character, Stream& stream) {
	bool quotations = character == '\"';

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
	buffer.reset();																		// Prepare buffer for use by the value parser.

	EndingType endingType;
	while (stream.readChar(character)) {					// TODO: What if this fails in the middle of a value? Will the stop just propagate up the call stack or something? Make sure all the cases are accounted for.
		if (filterWhitespace(character)) { continue; }

		switch (character) {
		case ',':
			endingType = EndingType::comma;
			goto quit;
		case '}':
			endingType = EndingType::object;
			goto quit;
		case ']':
			endingType = EndingType::array;
			goto quit;
		}
		buffer.add(character);
	}
quit:
	value = buffer.toArray();			// TODO: Dispose this one as well in the finalizer.
	return endingType;
}