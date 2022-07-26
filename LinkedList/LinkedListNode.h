#pragma once

template<class LinkedList>
struct LinkedListNode                                                             // Struct that holds data and references to next and previous struct
{
public:
	using ValueType = typename LinkedList::ValueType;

	ValueType Value;                                                              // Data
	LinkedListNode* Previous = nullptr;                                           // Reference to previous 
	LinkedListNode* Next = nullptr;                                               // Reference to next

	template<class... Args>
	LinkedListNode(Args&&... args) {                                              // Add data using emplace ValueTypepe Constructor
		new(&Value) ValueType(std::forward<Args>(args)...);
	}

	LinkedListNode(const ValueType& value) {                                      // Add data using reference ValueTypepe Constructor
		new(&Value) ValueType(value);
	}

	LinkedListNode(ValueType&& value) {                                           // Add data using temporary ValueTypepe Constructor
		new(&Value) ValueType(std::move(value));
	}
};