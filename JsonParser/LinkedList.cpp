#include "LinkedList.h"

template <typename T>
LinkedList<T>::LinkedList<T>(T element) {
	this->element = element;
}

template <typename T>
void LinkedList<T>::add(T element) {
	if (length == 1) {
		child = new LinkedList<T>();				// TODO: Worry about the release of this.
		child->element = element;
		length++;
		return;
	}
	child->add(element);
}

template <typename T>
void LinkedList<T>::reset() {
	length = 1;
}

template <typename T>
T* LinkedList<T>::toArray() {
	T* result = new T[length];
	result[0] = element;												// Set the first element to our element.

	LinkedList<T> current = child;
	for (int i = 1; i < length; i++) {									// Loop through the whole family tree and put the elements into their respective slots.
		result[i] = current.element;
		current = current.child;
	}

	return result;
}