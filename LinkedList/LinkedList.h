#pragma once
#include "LinkedListIterator.h"
#include "LinkedListNode.h"

template<class Type>
class LinkedList
{
public:
	using ValueType = Type;                                                 // Type for stored values
	using Node = typename LinkedListNode<LinkedList<ValueType>>;            // Node type
	using Iterator = typename LinkedListIterator<LinkedList<ValueType>>;    // Iterator type

private:
	size_t _size = 0;                                                       // Number of Nodes held by this
	Node* _head = new Node();                                               // Head of this list
	Node* _tail = new Node();                                               // Tail of this list
	mutable Node* _workspaceNode = nullptr;                                 // Auxiliary Node for work

public:
	// Constructors

	LinkedList() {                                                              // Default Constructor
		_head->Next = _tail;
		_tail->Previous = _head;
	}                                                 

	template<class... Args>
	LinkedList(const size_t& newSize, Args&&... args) : LinkedList() {          // Emplace type Constructor
		while (_size < newSize)
			emplace_back(std::forward<Args>(args)...);
	}

	LinkedList(const size_t& newSize, const ValueType& value) : LinkedList() {  // Reference type Constructor
		while (_size < newSize)
			push_back(value);
	}

	LinkedList(const size_t& newSize, ValueType&& value) : LinkedList() {       // Temporary type Constructor
		while (_size < newSize)
			push_back(std::move(value));
	}

	LinkedList(const LinkedList& other) : LinkedList() {                        // Copy Constructor
		_workspaceNode = other._head;
		while (_size < other._size) {
			push_back(_workspaceNode->Value);
			_workspaceNode = _workspaceNode->Next;
		}
	}

	LinkedList(LinkedList<ValueType>&& other) noexcept : LinkedList() {         // Move Constructor
		_workspaceNode = other._head;
		while (_size < other._size) {
			push_back(std::move(_workspaceNode->Value));
			_workspaceNode = _workspaceNode->Next;
		}
	}

	~LinkedList() {                                                             // Destructor
		clear();
	}

public:
	// Main functions

	template<class... Args>
	void resize(const size_t& newSize, Args&&... args) {                    // Resize the list by removing or adding elements to the tail
		while (_size > newSize)
			pop_back();
		while (_size < newSize)
			emplace_back(std::forward<Args>(args)...);                      // Emplace type addition
	}

	void resize(const size_t& newSize, const ValueType& copyValue) {
		while (_size > newSize)
			pop_back();
		while (_size < newSize)
			push_back(copyValue);                                           // Reference type addition
	}

	void resize(const size_t& newSize, ValueType&& moveValue) {
		while (_size > newSize)
			pop_back();
		while (_size < newSize)
			push_back(std::move(moveValue));                                // Temporary type addition
	}

	template<class... Args>
	void emplace_back(Args&&... args) {                                     // Construct object using arguments (Args) and add it to the tail
		Node* newNode = new Node(std::forward<Args>(args)...);

		newNode->Previous = _tail->Previous;
		newNode->Next = _tail;

		_tail->Previous->Next = newNode;
		_tail->Previous = newNode;
		
		_size++;
	}

	void push_back(const ValueType& copyValue) {                            // Construct object using reference and add it to the tail
		emplace_back(copyValue);
	}

	void push_back(ValueType&& moveValue) {                                 // Construct object using temporary and add it to the tail
		emplace_back(std::move(moveValue));
	}

	void pop_back() {                                                       // Remove last component
		if (_tail->Previous != _head) {
			_workspaceNode = _tail->Previous;
			_tail->Previous = _tail->Previous->Previous;
			_tail->Previous->Next = _tail;

			delete _workspaceNode;
			_size--;
		}
	}

	template<class... Args>
	void emplace_front(Args&&... args) {                                    // Construct object using arguments (Args) and add it to the head
		Node* newNode = new Node(std::forward<Args>(args)...);
		
		newNode->Next = _head->Next;
		newNode->Previous = _head;

		_head->Next->Previous = newNode;
		_head->Next = newNode;

		_size++;
	}

	void push_front(const ValueType& copyValue) {                           // Construct object using reference and add it to the head
		emplace_front(copyValue);
	}

	void push_front(ValueType&& moveValue) {                                // Construct object using temporary and add it to the head
		emplace_front(std::move(moveValue));
	}

	void pop_front() {                                                      // Remove first component
		if (_head->Next != _tail) {
			_workspaceNode = _head->Next;
			_head->Next = _head->Next->Next;
			_head->Next->Previous = _head;

			delete _workspaceNode;
			_size--;
		}
	}

	template<class... Args>
	Iterator emplace(const Iterator& iterator, Args&&... args) {            // Construct object using arguments (Args) and add it to the index position
		_workspaceNode = iterator.NodePtr;
		Node* newNode = new Node(std::forward<Args>(args)...);

		_workspaceNode->Previous->Next = newNode;
		newNode->Previous = _workspaceNode->Previous;

		newNode->Next = _workspaceNode;
		_workspaceNode->Previous = newNode;

		_size++;

		return Iterator(newNode);
	}

	Iterator push(const Iterator& iterator, const ValueType& copyValue) {    // Construct object using reference and add it to the index position
		return emplace(iterator, copyValue);
	}

	Iterator push(const Iterator& iterator, ValueType&& moveValue) {         // Construct object using temporary and add it to the index position
		return emplace(iterator, std::move(moveValue));
	}

	Iterator pop(const Iterator& iterator) {                                 // Remove component at index position
		_workspaceNode = iterator.NodePtr;

		_workspaceNode->Previous->Next = _workspaceNode->Next;
		_workspaceNode->Next->Previous = _workspaceNode->Previous;

		Iterator nextIterator = Iterator(_workspaceNode->Next);
		delete _workspaceNode;
		_size--;

		return nextIterator;
	}

	ValueType& front() {                                                     // Get the value of the first component
		return _head->Next->Value;
	}

	ValueType& back() {                                                      // Get the value of the last component
		return _tail->Previous->Value;
	}

	const size_t size() const {                                              // Get size
		return _size;
	}

	bool empty() const {                                                     // Check if list is empty
		return _size == 0;
	}

	void clear() {                                                           // Remove ALL components
		while (_size)
			pop_back();
	}

public:
	// Operators

	LinkedList& operator=(const LinkedList<ValueType>& other) {              // Assign operator using reference
		clear();
		
		_workspaceNode = other._head;
		while (_size < other._size) {
			push_back(_workspaceNode->Value);
			_workspaceNode = _workspaceNode->Next;
		}
		return *this;
	}

	LinkedList& operator=(LinkedList<ValueType>&& other) noexcept {          // Assign operator using temporary
		clear();

		_workspaceNode = other._head;
		while (_size < other._size) {
			push_back(std::move(_workspaceNode->Value));
			_workspaceNode = _workspaceNode->Next;
		}
		return *this;
	}

public:
	// Iterator specific functions

	Iterator begin() {
		return Iterator(_head->Next);
	}

	Iterator end() {

		return Iterator(_tail);
	}

	Iterator at(const size_t& index) {
		if (index < 0 || index >= _size)
			throw std::out_of_range("Invalid Index...");

		return Iterator(scroll_node(index));
	}

private:
	// Others

	Node* scroll_node(const size_t& index) const {                           // Get object in the list at index position by going through all components
		_workspaceNode = _head->Next;
		if (_workspaceNode != _tail)
			for (size_t i = 0; i < index; i++)
				_workspaceNode = _workspaceNode->Next;

		return _workspaceNode;
	}
};