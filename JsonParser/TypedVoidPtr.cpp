#include "TypedVoidPtr.h"

#include <cstdint>

#include "JObject.h"
#include "JArray.h"

void TypedVoidPtr::release() {
	if (pointer) {
		switch (type) {																	// Delete the value in just the right way based on what type of value it is.
		case ValueType::object:
			delete (JObject*)pointer;
			break;
		case ValueType::array:
			delete (JArray*)pointer;
			break;
		case ValueType::string:
			delete[](char*)pointer;
			break;
		case ValueType::boolean: break;
		case ValueType::null: return;
		case ValueType::integer:
			delete (int32_t*)pointer;
			break;
		}
		pointer = nullptr;
	}
}