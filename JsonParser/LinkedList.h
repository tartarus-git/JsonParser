#pragma once

template <typename T>
class LinkedList
{
public:
	T* element;
	LinkedList* child;

	void add(T* element);

};

