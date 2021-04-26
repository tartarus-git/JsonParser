#include "TypedVoidPtr.h"

#include <vector>
#include <cstdint>

#include "JsonParser.h"

void TypedVoidPtr::release() {
	switch (type) {																	// Delete the value in just the right way based on what type of value it is.
	case ValueType::object:															// This is necessary because delete and delete[] need to know what finalizer to call.
		delete (JObject*)pointer;
		return;
	case ValueType::array:
		delete (JArray*)pointer;
		return;
	case ValueType::string:
		delete (std::vector<char>*)pointer;
		return;
	case ValueType::boolean: return;
	case ValueType::null: return;
	case ValueType::integer:
		delete (int32_t*)pointer;
		return;
	}
}