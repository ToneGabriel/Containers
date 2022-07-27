#pragma once

template<class LinkedList>
class LinkedListIterator
{
public:
	using ValueType = typename LinkedList::ValueType;
	using Node = typename LinkedList::Node;                        // Node type accessed via friendship

	Node* NodePtr = nullptr;

public:
	LinkedListIterator() = default;

	LinkedListIterator(Node* nodePtr)
		:NodePtr(nodePtr) { }

	LinkedListIterator& operator++() {
		NodePtr = NodePtr->Next;
		return *this;
	}

	LinkedListIterator& operator++(int) {
		LinkedListIterator iterator = NodePtr;
		NodePtr = NodePtr->Next;
		return iterator;
	}

	LinkedListIterator& operator+=(const size_t diff) {
		size_t aux = diff;
		while (aux) {
			NodePtr = NodePtr->Next;
			aux--;
		}
		return *this;
	}

	LinkedListIterator& operator+(const size_t diff) {
		LinkedListIterator temp = *this;
		temp += diff;
		return temp;
	}

	LinkedListIterator& operator--() {
		NodePtr = NodePtr->Previous;
		return *this;
	}

	LinkedListIterator& operator--(int) {
		LinkedListIterator iterator = NodePtr;
		NodePtr = NodePtr->Previous;
		return iterator;
	}

	LinkedListIterator& operator-=(const size_t diff) {
		size_t aux = diff;
		while (aux) {
			NodePtr = NodePtr->Previous;
			aux--;
		}
		return *this;
	}

	LinkedListIterator& operator-(const size_t diff) {
		LinkedListIterator temp = *this;
		temp -= diff;
		return temp;
	}

	Node* operator->() {
		return NodePtr;
	}

	ValueType& operator*() {
		return NodePtr->Value;
	}

	bool operator==(const LinkedListIterator& other) const {
		return NodePtr == other.NodePtr;
	}

	bool operator!=(const LinkedListIterator& other) const {
		return !(*this == other);
	}
};