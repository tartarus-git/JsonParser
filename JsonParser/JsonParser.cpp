#include "JsonParser.h"

#include "Stream.h"
#include "JObject.h"

JObject JsonParser::parse(Stream& stream) {
	JObject result;
	char character;
	while (true) {																							// Find the first object using loop.
		if (stream.readChar(&character)) {																	// Keep looking as long as the stream still has available data.
			if (character == '{') { break; }
			continue;
		}
		result.length = 0;																					// If file ends before the first JObject could be opened, then return an empty JObject.
		return result;
	}
	if (result.parse(stream)) {																				// Tell the opened JObject to start parsing it's contents.
		return result;																						// Return the entirety of the parsed JSON contents as long as no errors were encountered.
	}
	result.length = 0;																						// If errors were encountered, return an empty JObject.
	return result;
}