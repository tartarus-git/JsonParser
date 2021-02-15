#pragma once

template <typename T>
class LinkedList
{
public:
	T element;
	LinkedList* child;
	int length = 1;

	LinkedList<T>(T element);

	void add(T element);

	void reset();

	T* toArray();
};

