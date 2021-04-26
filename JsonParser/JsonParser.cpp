#include "JsonParser.h"

JObject JsonParser::parse(Stream& stream) {
	JObject result;
	char character;
	while (true) {																							// Find the first object using loop.
		if (stream.readChar(&character)) {																	// Keep looking as long as the stream still has available data.
			if (character == '{') { break; }
			continue;
		}
		return result;																						// If file ends before the first JObject could be opened, then return an empty JObject.
																											// No need to std::move here. RVO takes care of it for use since we have move constructor.
	}
	if (result.parse(stream)) {																				// Tell the opened JObject to start parsing it's contents.
		return result;																						// Return the entirety of the parsed JSON contents as long as no errors were encountered.
	}
	return result;																							// If errors were encountered, return an empty JObject.
																											// No need to std::move here either for same reason.
}