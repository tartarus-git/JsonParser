#include "JObject.h"

#include "Stream.h"
#include "LinkedList.h"
#include "whitespace.h"

void JObject::parse(Stream& stream) {
	LinkedList<JPair> buffer;
	char character;
	if (stream.readChar(character)) {
		while (true) {																									// Loop through the object and parse all the contents.
			if (filterWhitespace(character)) { continue; }																// Filter out all the whitespace.
			if (character == '}') { break; }																			// Angel bracket = object end, so end object.

			JPair newPair;
			if (newPair.parse(character, stream) == EndingType::object) { break; }										// If char isn't whitespace or angel bracket, parse new key-value pair. End object if the pair ends with angel bracket.
			buffer.add(newPair);																						// Add the newly parsed key-value pair to the content of this object.

			if (stream.readChar(character)) { continue; }				// TODO: This is stupid. You need to decide what you want to do when the input is wrong. Like if there isn't anything after this. Quit?
		}
		content = buffer.toArray();
		length = buffer.length;
		return;
	}
}