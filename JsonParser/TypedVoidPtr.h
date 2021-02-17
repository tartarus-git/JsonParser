#pragma once

// Descriptor that goes along with void* pointers so that the user can tell what type of object he has without doing some weird RTI (Runtime Type Information) thing.
enum class ValueType {
	object,
	array,
	string,
	boolean,
	null,
	integer						// TODO: Is this the best description? Should I use number or integer?
};

struct TypedVoidPtr {
	void* pointer;
	ValueType type;
};