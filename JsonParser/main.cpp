#include <iostream>

#include "FileStream.h"
#include "JObject.h"
#include "JsonParser.h"
#include "JArray.h"

int main(int argc, char* argv[]) {
	FileStream f(argv[1]);																		// Construct stream which feeds from the file input.
	JObject result = JsonParser::parse(f);														// Connect up the free end of the stream to the JSON parser.

	// Very simple display code for debugging the parsed data.
	for (int i = 0; i < result.length; i++) {
		std::cout << result.content[i].key << " : " << (char*)result.content[i].value.pointer << std::endl;
	}

	/*for (int i = 0; i < ((JArray*)((JObject*)((JObject*)result.content[3].value.pointer)->content[0].value.pointer)->content[0].value.pointer)->length; i++) {
		std::cout << (char*)((JArray*)((JObject*)((JObject*)result.content[3].value.pointer)->content[0].value.pointer)->content[0].value.pointer)->content[i].pointer << std::endl;
	}*/

	// Wait for keyboard input so the window doesn't close automatically.
	std::cin.get();

	// Dispose JObject because the data structure could be taking up all sorts of space on the heap.
	result.release();
}