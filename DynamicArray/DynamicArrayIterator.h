#pragma once

template<class DynamicArray>
class DynamicArrayIterator
{
public:
	using ValueType = typename DynamicArray::ValueType;

	ValueType* Ptr = nullptr;

public:
	DynamicArrayIterator() = default;

	DynamicArrayIterator(ValueType* ptr)
		:Ptr(ptr) { }

	DynamicArrayIterator& operator++() {
		Ptr++;
		return *this;
	}

	DynamicArrayIterator& operator++(int) {
		DynamicArrayIterator temp = *this;
		++(*this);
		return temp;
	}

	DynamicArrayIterator& operator+=(const size_t diff) {
		Ptr += diff;
		return *this;
	}

	DynamicArrayIterator operator+(const size_t diff) const {
		DynamicArrayIterator temp = *this;
		temp += diff;
		return temp;
	}

	DynamicArrayIterator& operator--() {
		Ptr--;
		return *this;
	}

	DynamicArrayIterator& operator--(int) {
		DynamicArrayIterator temp = *this;
		--(*this);
		return temp;
	}

	DynamicArrayIterator& operator-=(const size_t diff) {
		Ptr -= diff;
		return *this;
	}

	DynamicArrayIterator operator-(const size_t diff) const {
		DynamicArrayIterator temp = *this;
		temp -= diff;
		return temp;
	}

	ValueType& operator[](const int& index) const {
		return *(Ptr + index);
	}

	ValueType* operator->() {
		return Ptr;
	}

	ValueType& operator*() {
		return *Ptr;
	}

	bool operator==(const DynamicArrayIterator& other) const {
		return Ptr == other.Ptr;
	}

	bool operator!=(const DynamicArrayIterator& other) const {
		return !(*this == other);
	}
};