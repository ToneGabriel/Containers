#pragma once
#include "DynamicArrayIterator.h"

template<class Type>
class DynamicArray
{
public:
	using ValueType = Type;                                                          // Type for stored values
	using Iterator = typename DynamicArrayIterator<DynamicArray<ValueType>>;         // Iterator type

private:
	size_t _size = 0;                                                                // Number of components held by this
	size_t _capacity = 0;                                                            // Allocated momory of type ValueType
	ValueType* _array = nullptr;                                                     // Actual container array

public:
	// Constructors

	DynamicArray() = default;                                                        // Default Constructor
	
	template<class... Args>
	DynamicArray(const size_t& newCapacity, Args&&... args) {                        // Emplace type Constructor
		realloc(newCapacity, std::forward<Args>(args)...);
	}

	DynamicArray(const size_t& newCapacity, const ValueType& copyValue) {            // Reference type Constructor
		realloc(newCapacity, copyValue);
	}

	DynamicArray(const size_t& newCapacity, ValueType&& moveValue) {                 // Temporary type Constructor
		realloc(newCapacity, std::move(moveValue));
	}

	DynamicArray(const DynamicArray<ValueType>& other) {                             // Copy Constructor
		_array = alloc(other._capacity);
		for (size_t i = 0; i < other._size; i++)
			new(&_array[i]) ValueType(other._array[i]);

		_size = other._size;
		_capacity = other._capacity;
	}

	DynamicArray(DynamicArray<ValueType>&& other) noexcept {                         // Move Constructor
		_array = alloc(other._capacity);
		for (size_t i = 0; i < other._size; i++)
			new(&_array[i]) ValueType(std::move(other._array[i]));

		_size = other._size;
		_capacity = other._capacity;
	}

	~DynamicArray() {                                                                // Destructor
		clear();
		dealloc();
	}

public:
	// Main functions

	void reserve(const size_t& newCapacity) {                                     // Allocate memory and move values if needed
		if (newCapacity < _size)
			_size = newCapacity;

		ValueType* newArray = alloc(newCapacity);
		for (size_t i = 0; i < _size; i++)
			new(&newArray[i]) ValueType(std::move(_array[i]));

		destruct_all();
		dealloc();

		_array = newArray;
		_capacity = newCapacity;
	}

	void shrink_to_fit() {                                                       // Allocate memory with capacity equal to size and move values there
		reserve(_size);
	}

	template<class... Args>
	void realloc(const size_t& newCapacity, Args&&... args) {                    // Allocate memory and populate it with objects constructed with given arguments (delete old)
		realloc_emplace(newCapacity, std::forward<Args>(args)...);
	}

	void realloc(const size_t& newCapacity, const ValueType& copyValue) {        // Allocate memory and populate it with given reference (delete old)
		realloc_emplace(newCapacity, copyValue);
	}

	void realloc(const size_t& newCapacity, ValueType&& moveValue) {             // Allocate memory and populate it with given temporary (delete old)
		realloc_emplace(newCapacity, std::move(moveValue));
	}

	template<class... Args>
	void resize(const size_t& newSize, Args&&... args) {                         // Change size and Construct/Destruct objects with given arguments if needed
		resize_emplace(newSize, std::forward<Args>(args)...);
	}

	void resize(const size_t& newSize, const ValueType& copyValue) {             // Change size and Construct/Destruct objects with given reference if needed
		resize_emplace(newSize, copyValue);
	}

	void resize(const size_t& newSize, ValueType&& moveValue) {                  // Change size and Construct/Destruct objects with given temporary if needed
		resize_emplace(newSize, std::move(moveValue));
	}
	
	template<class... Args>
	void emplace_back(Args&&... args) {                                          // Construct object using arguments (Args) and add it to the tail
		extend_if_full();
		new(&_array[_size++]) ValueType(std::forward<Args>(args)...);
	}

	void push_back(const ValueType& copyValue) {                                 // Construct object using reference and add it to the tail
		emplace_back(copyValue);
	}

	void push_back(ValueType&& moveValue) {                                      // Construct object using temporary and add it to the tail
		emplace_back(std::move(moveValue));
	}

	void pop_back() {                                                            // Remove last component
		if (_size > 0)
			_array[--_size].~ValueType();
	}
	
	template<class... Args>
	Iterator emplace(const Iterator& iterator, Args&&... args) {                 // Emplace object at iterator position with given arguments
		size_t index = get_iterator_index(iterator);
		if (index < 0 || index > _size)
			throw std::out_of_range("Array emplace iterator outside range...");

		emplace_back();
		for (size_t i = _size - 1; i > index; i--)
			_array[i] = _array[i - 1];

		_array[index].~ValueType();
		new(&_array[index]) ValueType(std::forward<Args>(args)...);

		return Iterator(_array + index);
	}

	Iterator push(const Iterator& iterator, const ValueType& copyValue) {        // Push copy object at iterator position
		return emplace(iterator, copyValue);
	}

	Iterator push(const Iterator& iterator, ValueType&& moveValue) {             // Push temporary object at iterator position
		return emplace(iterator, std::move(moveValue));
	}

	Iterator pop(const Iterator& iterator) {                                     // Remove component at iterator position
		if (iterator == end())
			throw std::out_of_range("Array pop iterator outside range...");

		size_t index = get_iterator_index(iterator);
		for (size_t i = index; i < _size - 1; i++)
			_array[i] = _array[i + 1];
		pop_back();

		if (iterator == end())
			return Iterator(_array + _size - 1);

		return iterator;
	}

	const size_t capacity() const {                                       // Get capacity
		return _capacity;
	}

	const size_t size() const {                                           // Get size
		return _size;
	}

	void clear() {                                                        // Remove ALL components but keep memory
		destruct_all();
		_size = 0;
	}

	bool empty() const {                                                  // Check if array is empty
		return _size == 0;
	}

	const ValueType& at(const size_t& index) const {                      // Acces object at index with check (read only)
		if (index < 0 || index >= _size)
			throw std::out_of_range("Invalid Index...");

		return _array[index];
	}

	ValueType& at(const size_t& index) {                                  // Acces object at index with check
		if (index < 0 || index >= _size)
			throw std::out_of_range("Invalid Index...");

		return _array[index];
	}

public:
	// Operators

	const ValueType& operator[](const size_t& index) const {              // Acces object at index (read only)
		return _array[index];
	}

	ValueType& operator[](const size_t& index) {                          // Acces object at index
		return _array[index];
	}

	DynamicArray<ValueType>& operator=(const DynamicArray<ValueType>& other) {         // Assign operator using reference
		if (_array == other._array)
			return *this;

		destruct_all();
		dealloc();

		_array = alloc(other._capacity);
		for (size_t i = 0; i < other._size; i++)
			new(&_array[i]) ValueType(other._array[i]);

		_size = other._size;
		_capacity = other._capacity;
		return *this;
	}

	DynamicArray<ValueType>& operator=(DynamicArray<ValueType>&& other) noexcept {     // Assign operator using temporary
		if (_array == other._array)
			return *this;

		destruct_all();
		dealloc();

		_array = alloc(other._capacity);
		for (size_t i = 0; i < other._size; i++)
			new(&_array[i]) ValueType(std::move(other._array[i]));

		_size = other._size;
		_capacity = other._capacity;
		return *this;
	}

public:
	// Iterator specific functions

	Iterator begin() const {
		return Iterator(_array);
	}

	Iterator end() const {
		return Iterator(_array + _size);
	}

private:
	// Others

	template<class... Args>
	void realloc_emplace(const size_t& newCapacity, Args&&... args) {     // Allocate memory and populate it with objects constructed with given arguments (delete old)
		destruct_all();
		dealloc();

		_capacity = newCapacity;
		_size = _capacity;

		_array = alloc(_capacity);
		for (size_t i = 0; i < _capacity; i++)
			new(&_array[i]) ValueType(std::forward<Args>(args)...);
	}

	template<class... Args>
	void resize_emplace(const size_t& newSize, Args&&... args) {         // Change size and Construct/Destruct objects with given arguments if needed
		if (newSize < _size)
			for (size_t i = newSize; i < _size; i++)
				_array[i].~ValueType();
		else {
			if (newSize > _capacity)
				reserve(newSize);
			for (size_t i = _size; i < newSize; i++)
				new(&_array[i]) ValueType(std::forward<Args>(args)...);
		}

		_size = newSize;
	}

	const size_t get_iterator_index(const Iterator& iterator) const {    // Get the position for the element in array from iterator
		return iterator.Ptr - begin().Ptr;
	}

	void extend_if_full() {                                              // Reserve 50% more capacity when full
		if (_size >= _capacity)
			reserve(_capacity + _capacity / 2 + 1);
	}

	void destruct_all() {                                                // Call ~Destructor for ALL components held by this
		for (size_t i = 0; i < _size; i++)
			_array[i].~ValueType();
	}

	ValueType* alloc(const size_t& newCapacity) const {                  // Allocate memory without using Constructor
		return (ValueType*) ::operator new(newCapacity * sizeof(ValueType));
	}

	void dealloc() {                                                     // Deallocate memory without using ~Destructor
		::operator delete(_array, _capacity * sizeof(ValueType));
	}
};