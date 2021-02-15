#include "JArray.h"

void JArray::parse(Stream& stream) {
	// Allocate the content array somewhere in here.
}

JArray::~JArray() {
	delete[] content;
}