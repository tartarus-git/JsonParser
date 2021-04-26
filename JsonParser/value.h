#pragma once

#include <vector>
#include <utility>
#include <cstdint>

#include "Stream.h"
#include "TypedVoidPtr.h"
#include "whitespace.h"
#include "JObject.h"
#include "JArray.h"
#include "TypedVoidPtr.h"
#include "EndingType.h"

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