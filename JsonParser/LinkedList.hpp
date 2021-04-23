#pragma once

template <typename T>
void LinkedList<T>::add(T element) {
	length++;
	if (length == 1) {
		this->element = element;
		child = new LinkedList<T>();				// TODO: Worry about releasing this allocation. Especially when resetting a list. Does everything get release properly?
		return;
	}
	child->add(element);
}

template <typename T>
LinkedList<T>::LinkedList(T element) {
	this->element = element;
	child = new LinkedList<T>();
	length++;
}

template <typename T>
void LinkedList<T>::reset() {
	length = 0;
}

template <typename T>
T* LinkedList<T>::toArray() {
	T* result = new T[length];
	result[0] = element;												// Set the first element to our element.

	LinkedList<T>* current = child;
	for (int i = 1; i < length; i++) {									// Loop through the whole family tree and put the elements into their respective slots.
		result[i] = current->element;
		current = current->child;
	}

	return result;
}

template <typename T>
LinkedList<T>::~LinkedList() {
	delete child;
}