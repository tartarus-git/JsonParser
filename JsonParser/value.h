#pragma once

#include "Stream.h"
#include "TypedVoidPtr.h"
#include "whitespace.h"
#include "JObject.h"
#include "JArray.h"
#include "LinkedList.h"
#include "TypedVoidPtr.h"
#include "EndingType.h"

#define ASCII_NUMBERS_BEGIN 0x30
#define ASCII_NUMBERS_END ASCII_NUMBERS_BEGIN + 9

EndingType parseValue(Stream& stream, TypedVoidPtr& value) {
	char character;
	while (stream.readChar(character)) {					// TODO: What if this fails in the middle of a value? Will the stop just propagate up the call stack or something? Make sure all the cases are accounted for.
		if (filterWhitespace(character)) { continue; }

		LinkedList<char> buffer;												// We have to allocate it here so that the switch statement doesn't complain because of inconstant stack.
		switch (character) {
		case '{':
			value.type = ValueType::object;
			value.pointer = new JObject();
			((JObject*)value.pointer)->parse(stream);
			break;
		case '[':
			value.type = ValueType::array;
			value.pointer = new JArray();
			((JArray*)value.pointer)->parse(stream);
			break;
		case '\"':
			value.type = ValueType::string;
			while (stream.readChar(character)) {
				if (character == '\"') { break; }
				buffer.add(character);
			}
			buffer.add('\0');						// TODO: Is there any way to have this happen at array level at the end so it's more efficient without compromising the variability of the LinkedList class?
			value.pointer = buffer.toArray();			// TODO: Handle the memory and everything.
			break;
		case 't':
			value.type = ValueType::boolean;
			value.pointer = new bool(true);				// TODO: Find out which default constructor this uses and why. Should I use std::move here. Technically I should right? Defo check that out. I should use it everywhere no?
			stream.skip(3);
			break;
		case 'f':
			value.type = ValueType::boolean;
			value.pointer = new bool(false);
			stream.skip(4);
			break;
		case 'n':
			value.type = ValueType::null;
			value.pointer = nullptr;					// TODO: Should it be a pointer to a nullptr or just a nullptr? I guess you'll find out soon enough.
			stream.skip(3);
			break;
		// Check for end of value:
		case ',':
			return EndingType::comma;
		case '}':
			return EndingType::object;
		case ']':
			return EndingType::array;
		// In case of number:
		default:
			if (character >= ASCII_NUMBERS_BEGIN && character <= ASCII_NUMBERS_END) {
				value.type = ValueType::integer;
				value.pointer = new int(character - ASCII_NUMBERS_BEGIN);
				// This is commented out but still do the TODO's. TODO.
				//*(int*)value *= 10;						// TODO: Does this type of cast cost any processing? It's technically not even doing anything, so it shouldn't cost anything right?
				//return parseNumber(stream);							// TODO: So I think that decimal numbers are mostly transmitted through string in JSON, but what about normal numbers, am I ok with int. I should specify to int32_t right? Check with the spec.
				while (stream.readChar(character)) {							// TODO: As always, make sure the edge case of the whole file ending here actually gets handled well by the rest of the program.
					switch (character) {
					case ',':
						return EndingType::comma;
					case '}':
						return EndingType::object;
					case ']':
						return EndingType::array;
					// In case of actual number, which is why we're here:
					default:
						*(int*)value.pointer *= 10;
						*(int*)value.pointer += character - ASCII_NUMBERS_BEGIN;
					}
				}
			}
		}
	}
}