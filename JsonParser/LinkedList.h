#pragma once

template <typename T>
class LinkedList
{
public:
	T element;
	LinkedList<T>* child;
	int length = 0;

	LinkedList() = default;

	void add(T&& element);

	LinkedList(T element);

	void reset();

	T* toArray();

	~LinkedList();
};

#include "LinkedList.hpp"