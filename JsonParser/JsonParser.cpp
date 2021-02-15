#include "JsonParser.h"

#include "Stream.h"
#include "JObject.h"

JObject JsonParser::parse(Stream& stream) {
	JObject result;
	char character;
	while (true) {																							// Find the first object using loop.
		if (stream.readChar(character)) {																	// If there is another character to read, keep reading from the stream and parsing.
			if (character == '{') { break; }
			continue;
		}
		break;											// TODO: If this break happens before we have a solid object, what do we return? null? how?
	}
	result.parse(stream);																					// Tell the result object to start parsing its contents.
	return result;
}