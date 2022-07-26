#pragma once
#include "QueueNode.h"

template<class Type>
class Queue
{
public:
	using ValueType = Type;                                                 // Type for stored values
	using Node = typename QueueNode<Queue<ValueType>>;                      // Node type

private:
	size_t _size = 0;                                                       // Number of Nodes held by this
	Node* _head = nullptr;                                                  // Head of this list
	Node* _tail = nullptr;                                                  // Tail of this list
	mutable Node* _workspaceNode = nullptr;                                 // Auxiliary Node for work
	mutable ValueType _workspaceValue;                                      // Auxiliary Value for work

public:
	// Constructors

	Queue() = default;

	template<class... Args>
	Queue(const size_t& newSize, Args&&... args) {                          // Emplace ValueType Constructor
		while (_size < newSize)
			enqueue(std::forward<Args>(args)...);
	}

	Queue(const size_t& newSize, const ValueType& value) {                  // Reference type Constructor
		while (_size < newSize)
			enqueue(value);
	}

	Queue(const size_t& newSize, ValueType&& value) {                       // Temporary type Constructor
		while (_size < newSize)
			enqueue(std::move(value));
	}

	Queue(const Queue& other) {                                            // Copy Constructor
		_workspaceNode = other._head;
		while (_size < other._size) {
			enqueue(_workspaceNode->Value);
			_workspaceNode = _workspaceNode->Next;
		}
	}

	Queue(Queue<ValueType>&& other) noexcept {                              // Move Constructor
		_workspaceNode = other._head;
		while (_size < other._size) {
			enqueue(std::move(_workspaceNode->Value));
			_workspaceNode = _workspaceNode->Next;
		}
	}

	~Queue() {                                                             // Destructor
		clear();
	}

public:
	// Main functions

	template<class... Args>
	void enqueue(Args&&... args) {                                         // Construct object using arguments (Args) and add it to the tail
		emplace_back(std::forward<Args>(args)...);
	}

	void enqueue(const ValueType& copyValue) {                             // Construct object using reference and add it to the tail
		emplace_back(copyValue);
	}

	void enqueue(ValueType&& moveValue) {                                  // Construct object using temporary and add it to the tail
		emplace_back(std::move(moveValue));
	}

	ValueType dequeue() {                                                  // Return first component and remove it from queue
		if (_head) {
			_workspaceValue = _head->Value;
			_workspaceNode = _head;
			_head = _head->Next;

			if (_head == nullptr)
				_tail = nullptr;

			delete _workspaceNode;
			_size--;

			return _workspaceValue;
		}
	}

	const size_t size() const {                                          // Get size
		return _size;
	}

	bool empty() const {                                                 // Check if list is empty
		return _size == 0;
	}

	void clear() {                                                       // Remove ALL components
		while (_size) {
			_workspaceNode = _head;
			_head = _head->Next;

			delete _workspaceNode;
			_size--;
		}
	}

public:
	// Operators

	Queue& operator=(const Queue<ValueType>& other) {                   // Assign operator using reference
		clear();

		_workspaceNode = other._head;
		while (_size < other._size) {
			enqueue_push(_workspaceNode->Value);
			_workspaceNode = _workspaceNode->Next;
		}
		return *this;
	}

	Queue& operator=(Queue<ValueType>&& other) noexcept {               // Assign operator using temporary
		clear();

		_workspaceNode = other._head;
		while (_size < other._size) {
			enqueue_push(std::move(_workspaceNode->Value));
			_workspaceNode = _workspaceNode->Next;
		}
		return *this;
	}

private:
	// Others

	template<class... Args>
	void emplace_back(Args&&... args) {                                 // Construct object using arguments (Args) and add it to the tail
		Node* newNode = new Node(std::forward<Args>(args)...);

		if (_head == nullptr)
			_head = _tail = newNode;
		else {
			_tail->Next = newNode;
			_tail = newNode;
		}
		_size++;
	}
};