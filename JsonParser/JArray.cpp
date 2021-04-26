#include <vector>
#include <utility>

#include "JsonParser.h"

bool JArray::parse(Stream& stream) {
	while (true) {																			// No reading of anything here, just continual parsing of values.
		TypedVoidPtr value;
		EndingType ending = parseValue(stream, value);										// Parse a value in the array.

		switch (ending) {
		case EndingType::error:																// If the ending type of the parsed value is that of an error, clear content vector and return failure.
			content.clear();
			return false;
		case EndingType::array:																// If ending is that of an array, finalize and close this array.
			return true;																	// Return sucess.
		}
		content.push_back(value);															// If everything is fine but we haven't reached the end of the array yet, add the new value to the buffer.
	}
	// We don't need to put a return failure here because we don't actually do any parsing in this function, the values themselves take care of everything for us.
}

void JArray::release() {
	content.clear();																		// When we release, clear the content vector so that all the destructors of the values get called.
																							// Also because the memory consumption might become lower because std::vector decides to resize.
}

// No destructor because our only member (std::vector<char> content) destructs automatically.