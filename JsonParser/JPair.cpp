#include "JPair.h"

#include "Stream.h"
#include "LinkedList.h"
#include "whitespace.h"
#include "JObject.h"
#include "JArray.h"

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

	EndingType endingType;
	while (stream.readChar(character)) {					// TODO: What if this fails in the middle of a value? Will the stop just propagate up the call stack or something? Make sure all the cases are accounted for.
		if (filterWhitespace(character)) { continue; }

		switch (character) {
		case '{':
			value = new JObject();
			((JObject*)value)->parse(stream);
			break;
		case '[':
			value = new JArray();
			((JArray*)value)->parse(stream);
			break;
		case '\"':
			buffer.reset();																// Reset the buffer before using it again.
			while (stream.readChar(character)) {
				if (character == '\"') { break; }
				buffer.add(character);
			}
			buffer.add('\0');
			value = buffer.toArray();			// TODO: Handle the memory and everything.
			break;
		case 't':
			value = new bool(true);				// TODO: Refresh on what the hell those different default constructors do, because I think that I'm using one of those right here.
			stream.skip(3);
			break;
		case 'f':
			value = new bool(false);
			stream.skip(4);
			break;
		case 'n':
			value = nullptr;					// TODO: Should it be a pointer to a nullptr or just a nullptr? I guess you'll find out soon enough.
			stream.skip(3);
		// TODO: Handle numbers here.
		}

		switch (character) {
		case ',':
			return EndingType::comma;
		case '}':
			return EndingType::object;
		case ']':
			return EndingType::array;
		}
	}
}

JPair::~JPair() {
	delete[] key;
	delete[] value;
}