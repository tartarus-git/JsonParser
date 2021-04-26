#pragma once

#include <iostream>
#include <fstream>

#include <vector>
#include <utility>
#include <cstdint>

// An abstract Stream class so that the parsers can pull in data from whatever source the user wants.
class Stream							// TODO: You can probably use some sort of standard thing like iostream instead of using this Stream class for this entire project. Would make more sense.
{
public:
	virtual int read(char* buffer, int amount) = 0;					// Virtual means that the derived function will be called even when calling through base class reference, which is imperative in this case.
	bool readChar(char* character);

	virtual int skip(int amount) = 0;								// Advances the stream by the specified number of characters without reading.
};

// Derived class based on Stream class which uses a file as the basis for the stream. This allows for streaming JSON from files while parsing it.
class FileStream : public Stream
{
	std::ifstream f;

public:
	FileStream(const char* path);
	FileStream(FileStream&& other) noexcept;

	FileStream& operator=(FileStream&& other) noexcept;										// TODO: Is it good practice to put noexcept on move assignment operators?

	int read(char* buffer, int amount);

	int skip(int amount);

	void release();
	~FileStream();
};

// Derived class of Stream for parsing JSON data from memory locations.
// NOTE: This class doesn't release/destruct/finalize anything so we don't need any move semantics for it.
// The implicit copy assignment operator is totally fine for this class since it's up to the user to dispose his data in this case.
class MemoryStream : public Stream
{
	int pos = 0;
	int length;
	const char* data;

public:
	MemoryStream(const char* string);

	int read(char* buffer, int length);

	int skip(int amount);
};

// Descriptor that goes along with void* pointers so that the user can tell what type of object he has without doing some weird RTI (Runtime Type Information) thing.
enum class ValueType {
	object,
	array,
	string,
	boolean,
	null,
	integer
};

// A void* pointer with an associated type so that pointers to different types of objects can all be stored in the same way. Also releases the object in the correct way based on its type.
struct TypedVoidPtr {
	void* pointer;
	ValueType type;

	void release();
};

// enum which is used often as a return type. This tells a parent parser if its child parser has found the end of the parent parsers object. Also used to return errors (EOF).
enum class EndingType {
	error = -1,
	comma,
	object,
	array
};

// Function for filtering out whitespace while parsing JSON content.
inline bool filterWhitespace(char character) { return character == ' ' || character == '\t' || character == '\n' || character == '\r'; }

// Representation of a key-value pair in C++ code. Parses key-value pairs.
class JPair
{
public:
	std::vector<char> key;
	TypedVoidPtr value;

	JPair() = default;

	JPair(JPair&& other) noexcept;
	JPair& operator=(JPair&& other) noexcept;

	EndingType parse(char character, Stream& stream);

	void release();
	~JPair();
};

// Representation of a JSON array in C++ code. Also functions as a parser for arrays.
class JArray
{
public:
	std::vector<TypedVoidPtr> content;

	bool parse(Stream& stream);

	void release();
};

// Representation of a JSON object in C++ code. Also functions as a parser for objects.
class JObject
{
public:
	std::vector<JPair> content;

	// No move semantics are defined becaue the lack of a destructor negates the need for explicitly defined move semantics. Implicity move semantics will do just fine here. Absolutely fine.

	bool parse(Stream& stream);

	void release();
};

// Functions for parsing a single value in the JSON format.

#define ASCII_NUMBERS_BEGIN 0x30
#define ASCII_NUMBERS_END (ASCII_NUMBERS_BEGIN + 9)												// This needs to be in parenthesese because C++ just drops defines into code, which could cause errors.

inline EndingType parseEnding(Stream& stream) {
	char character;
	while (stream.readChar(&character)) {														// Keep parsing ending as long as the stream hasn't ended.
		if (filterWhitespace(character)) { continue; }											// Filter whitespace.

		switch (character) {																	// Check for all the different ending types. Return the right one.
		case ',':
			return EndingType::comma;
		case '}':
			return EndingType::object;
		case ']':
			return EndingType::array;
		}
	}
	return EndingType::error;																	// Return error if stream ends before we're finished.
}

inline EndingType parseValue(Stream& stream, TypedVoidPtr& value) {								// TODO: Make sure that inline is the best way to do this. You can't leave it out, maybe static instead?
	char character;
	while (stream.readChar(&character)) {														// If stream hasn't ended keep parsing.
		if (filterWhitespace(character)) { continue; }											// Filter out whitespace.

		std::vector<char> buffer;																// You have to do this here because or else the switch statement will complain because stack issues.
		bool numberIsNegative = false;															// Flag for keeping track of the sign of an ongoing number.
		switch (character) {
		case '{':
			value.type = ValueType::object;														// If angel brackets are opened, start parsing JObject.
			value.pointer = new JObject();
			if (((JObject*)value.pointer)->parse(stream)) { return parseEnding(stream); }		// If JObject sucessfully parses, parse the value ending and return the ending type.
			return EndingType::error;															// Return error if that isn't the case.
		case '[':
			value.type = ValueType::array;
			value.pointer = new JArray();
			if (((JArray*)value.pointer)->parse(stream)) { return parseEnding(stream); }		// If JArray sucessfully parses, parse the value ending and return the ending type.
			return EndingType::error;															// Return error if JArray doesn't sucessfully parse.
		case '\"':
			value.type = ValueType::string;
			while (stream.readChar(&character)) {
				if (character == '\"') {
					buffer.push_back('\0');
					value.pointer = new std::vector<char>(std::move(buffer));					// Move the contents of the buffer into a newly created vector pointed to by the value pointer.
					return parseEnding(stream);													// If everything works out, parse ending and return ending type.
				}
				buffer.push_back(std::move(character));
			}
			buffer.clear();																		// Clear buffer vector.
			return EndingType::error;															// If stream runs out before the string could be parsed, return error.
		case 't':
			value.type = ValueType::boolean;
			value.pointer = (void*)true;
			stream.skip(3);
			return parseEnding(stream);															// If everything works out, parse ending and return ending type.
		case 'f':
			value.type = ValueType::boolean;
			value.pointer = (void*)false;
			stream.skip(4);
			return parseEnding(stream);															// If everything works out, parse ending and return ending type.
		case 'n':
			value.type = ValueType::null;
			value.pointer = nullptr;
			stream.skip(3);
			return parseEnding(stream);															// If everything works out. parse ending and return ending type.
		// In case of number:
		case '-':																				// If a minus is read, go forward expecting a negative number.
			value.type = ValueType::integer;
			value.pointer = new int32_t(character - ASCII_NUMBERS_BEGIN);						// Get the first digit by subtracting the index of the number starting point in the ASCII table.
			numberIsNegative = true;															// Set this flag so that the number parser knows that the number is negative.
			goto readNumber;																	// Jump straight to the number parsing code so that you don't waste any cycles or time.
		default:
			if (character >= ASCII_NUMBERS_BEGIN && character <= ASCII_NUMBERS_END) {			// Check if the character is in the number range in the ASCII table.
				value.type = ValueType::integer;
				value.pointer = new int32_t(character - ASCII_NUMBERS_BEGIN);					// Get first digit of number by subtracting.

			readNumber:
				while (stream.readChar(&character)) {											// Read actual number and return ending type if ending when ending is encountered.
					switch (character) {
					case ',':
						return EndingType::comma;
					case '}':
						return EndingType::object;
					case ']':
						return EndingType::array;
						// In case of actual number, which is why we're here:
					default:
						*(int32_t*)value.pointer *= 10;
						if (numberIsNegative) { *(int32_t*)value.pointer -= character - ASCII_NUMBERS_BEGIN; break; }
						*(int32_t*)value.pointer += character - ASCII_NUMBERS_BEGIN;
					}
				}
				return EndingType::error;														// If stream ends before we're good, then return error.
			}
		}
	}
}

// Main parser which the user uses to get started on parsing JSON data.
class JsonParser
{
public:
	JsonParser() = delete;

	static JObject parse(Stream& stream);						// The & is necessary because copying would require instantiating, which we can't do with an abstract class. This just uses the already existing instance using the reference thing.
};