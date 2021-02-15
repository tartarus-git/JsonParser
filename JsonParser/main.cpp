#include "FileStream.h"						// TODO: Obviously make these includes more structured for the final product.
#include "JsonParser.h"						// TODO: Consider making the Json into a JSON.
#include "JObject.h"

int main(int argc, char* argv[]) {
	// Construct stream which feeds from the file input.
	FileStream f(argv[0]);
	// Connect up the free end of the stream to the JSON parser.
	JObject result = JsonParser::parse(f);
}