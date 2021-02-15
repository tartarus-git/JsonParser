#pragma once

template <typename T>
class LinkedList
{
public:
	T element;
	LinkedList<T>* child;
	int length = 0;

	void add(T element);

	void reset();

	T* toArray();

	~LinkedList();
};

