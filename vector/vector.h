#pragma once

#include <initializer_list> 
#include "allocator.h"

template <class T, class Allocator=allocator<T>>
class vector
{
public:
	class iterator {
	private:
		T* ptr{ nullptr };
		int _position{ 0 };
		const vector<T>* ptr_1{ nullptr };
	public:
		iterator() {}
		iterator(const iterator& A) : ptr(A.ptr), ptr_1(A.ptr_1), _position(A._position) {}
		T& operator*() const{
			if (_position > -1 && _position < ptr_1->_size && ptr_1)
				return *ptr;
			else
				throw(exception("going beyond the borders of the vector"));
		}
		int position() {
			return _position;
		}
		iterator& operator-=(int A);
		iterator& operator+=(int A);
		iterator operator-(int A);												
		iterator operator+(int A);
		iterator& operator++(); 
		const iterator& operator++(int);
		iterator& operator--();
		const iterator& operator--(int);
		bool operator==(const iterator& B) const{
			return this->ptr == B.ptr;
		}
		bool operator!=(const iterator& B) const{
			return this->ptr != B.ptr;
		}
	friend class vector;
	};
private:
	T* ptr{ nullptr };
	int _size{ 0 };
	int _capacity{ 0 };
	Allocator all;
public:
	vector() {}

	vector(const std::initializer_list<T>& list);

	vector(const vector<T>& A);

	vector(vector<T>&& A) noexcept;

	~vector();

	int size() const noexcept {
		return _size;
	}

	int capacity() const noexcept{
		return _capacity;
	}

	T& operator[](int n) const;

	void push_back(T&& A);

	void push_back(const T& A);

	vector<T>& operator=(vector<T> && A) noexcept;

	vector<T>& operator=(const vector<T>& A);

	vector<T>& operator=(const std::initializer_list<T>& list);

	void reserve(int n);

	void shrink_to_fit();

	T& front() const{
		if (_size > 0)
			return ptr[0];
		else
			throw(exception("front empty vector"));
	}

	T& back() const{
		if (_size > 0)
			return ptr[_size - 1];
		else
			throw(exception("back empty vector"));
	}

	void clear() noexcept{
		_size = 0;
	}

	void pop_back() {
		if (_size > 0) 
			_size--;
		else
			throw(exception("pop empty vector"));
	}

	void swap(vector<T>& A);

	auto begin() const->iterator;

	iterator end() const;

	void insert(const iterator& A, T&& B);

	void insert(const iterator& A, const T& B);

	void erase(const iterator& A);
};

template<class T, class Allocator>
vector<T, Allocator>::vector(const std::initializer_list<T>& list)  {
	ptr = all.allocate(list.size());
	_size = list.size();
	_capacity = list.size();
	int i = 0;
	for (auto& r : list) {
		ptr[i] = r;
		i++;
	}
}

template<class T, class Allocator>
vector<T, Allocator>::vector(const vector<T>& A)  {
	ptr = all.allocate(A.size());
	_size = A._size;
	_capacity = A._capacity;
	for (int i = 0;i<A.size();i++) 
		ptr[i] = A.ptr[i];
}

template<class T, class Allocator>
vector<T, Allocator>::vector(vector<T>&& A) noexcept {
	ptr = A.ptr;
	_size = A._size ;
	_capacity = A._capacity;
	A.ptr = nullptr;
	A._size = 0;
	A._capacity = 0;
}

template<class T, class Allocator>
vector<T, Allocator>::~vector() {
	all.deallocate(ptr);
	_size = 0;
}

template<class T, class Allocator>
T& vector<T, Allocator>::operator[](int n) const{
	if (n < _size && n >= 0)
		return ptr[n];
	else
		throw exception("going beyond the borders of the vector");
}

template<class T, class Allocator>
void vector<T, Allocator>::push_back(T&& A) {
	if (_size < _capacity) {
		ptr[_size] = std::move(A);
		_size++;
	}
	else {
		if (_capacity == 0) {
			all.deallocate(ptr);
			ptr = all.allocate(1);
			ptr[_size] = std::move(A);
			_size++;
			_capacity++;
		}
		else if (_capacity == 1) {
			T t = ptr[0];
			all.deallocate(ptr);
			ptr = all.allocate(2);
			ptr[0] = t;
			ptr[_size] = std::move(A);
			_size++;
			_capacity++;
		}
		else {
			T* ptr_1;
			ptr_1 = all.allocate(1.5 * _capacity);
			for (int i = 0; i < _size; i++)
				ptr_1[i] = ptr[i];
			ptr_1[_size] = std::move(A);
			all.deallocate(ptr);
			ptr = ptr_1;
			_size++;
			_capacity *= 1.5;
		}
	}
}

template<class T, class Allocator>
void vector<T, Allocator>::push_back(const T& A) {
	if (_size < _capacity) {
		ptr[_size] = A;
		_size++;
	}
	else {
		if (_capacity == 0) {
			all.deallocate(ptr);
			ptr = all.allocate(1);
			ptr[_size] = A;
			_size++;
			_capacity++;
		}
		else if (_capacity == 1) {
			T t = ptr[0];
			all.deallocate(ptr);
			ptr = all.allocate(2);
			ptr[0] = t;
			ptr[_size] = A;
			_size++;
			_capacity++;
		}
		else {
			T* ptr_1;
			ptr_1 = all.allocate(1.5 * _capacity);
			for (int i = 0; i < _size; i++)
				ptr_1[i] = ptr[i];
			ptr_1[_size] = A;
			all.deallocate(ptr);
			ptr = ptr_1;
			_size++;
			_capacity *= 1.5;
		}
	}
}

template<class T, class Allocator>
vector<T>& vector<T, Allocator>::operator=(vector<T>&& A) noexcept {
	if (*this == A)
		return *this;
	else {
		all.deallocate(ptr);
		ptr = A.ptr;
		A.ptr = nullptr;
		_size = A._size;
		_capacity = A._capacity;
		A._size = 0;
		A._capacity = 0;
		return *this;
	}
}
	
template<class T, class Allocator>
vector<T>& vector<T, Allocator>::operator=(const std::initializer_list<T>& list) {
	if (list.size() > _capacity) {
		T* ptr_1 = all.allocate(list.size());
		int i = 0;
		for (auto r : list) {
			ptr_1[i] = r;
			i++;
		}
		all.deallocate(ptr);
		ptr = ptr_1;
		_capacity = list.size();
		_size = list.size();
	}
	else {
		int i = 0;
		for (auto r : list) {
			ptr[i] = r;
			i++;
		}
		_size = list.size();
	}
	return *this;
}

template<class T, class Allocator>
vector<T>& vector<T, Allocator>::operator=(const vector<T>& A) {
	if (_capacity < A._size) {
		all.deallocate(ptr);
		ptr=all.allocate(A._size);
		_size = A._size;
		_capacity = A._size;
		for (int i = 0; i < _size; i++)
			ptr[i] = A.ptr[i];
	}
	else {
		for (int i = 0; i < A._size; i++)
			ptr[i] = A.ptr[i];
		_size = A._size;
	}
	return *this;
}

template<class T, class Allocator>
void vector<T, Allocator>::reserve(int n) {
	if (n > _capacity) {
		T* ptr_1 = all.allocate(n);
		for (int i = 0; i < _size; i++)
			ptr_1[i] = ptr[i];
		all.deallocate(ptr);
		ptr = ptr_1;
		_capacity = n;
	}
}

template<class T, class Allocator>
void vector<T, Allocator>::shrink_to_fit() {
	T* ptr_1 = all.allocate(_size);
	for (int i = 0; i < _size; i++)
		ptr_1[i] = ptr[i];
	all.deallocate(ptr);
	ptr = ptr_1;
	_capacity = _size;
}

template<class T, class Allocator>
void vector<T, Allocator>::swap(vector<T>& A){
	T* ptr_1 = ptr;
	ptr = A.ptr;
	A.ptr = ptr_1;
	int k = _size;
	_size = A._size;
	A._size = k;
	k = _capacity;
	_capacity = A._capacity;
	A._capacity = k;
}

template<class T, class Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::end() const{
	iterator v;
	v._position = _size;
	v.ptr_1 = this;
	v.ptr = ptr + _size;
	return v;
}

template<class T, class Allocator>
auto vector<T, Allocator>::begin() const->vector<T, Allocator>::iterator {
	iterator v;
	v._position = 0;
	v.ptr_1 = this;
	v.ptr = ptr;
	return v;
}

template<class T, class Allocator>
auto vector<T, Allocator>::iterator::operator+(int A)->vector<T, Allocator>::iterator {
	if (_position+A > ptr_1->_size)
		throw exception("going beyond the borders of the vector");
	else {
		vector<T, Allocator>::iterator B = A;
		B._position += A;
		B.ptr += A;
		return B;
	}
}

template<class T, class Allocator>
auto vector<T, Allocator>::iterator::operator++(int)->const vector<T, Allocator>::iterator& {
	if (_position+1 > ptr_1->_size)
		throw exception("going beyond the borders of the vector");
	else {
		vector<T, Allocator>::iterator t = *this;
		_position++;
		ptr++;
		return t;
	}
}

template<class T, class Allocator>
auto vector<T, Allocator>::iterator::operator++()->vector<T, Allocator>::iterator& {
	if (_position+1 > ptr_1->_size)
		throw exception("going beyond the borders of the vector");
	else {
		_position++;
		ptr++;
		return *this;
	}
}

template<class T, class Allocator>
auto vector<T, Allocator>::iterator::operator-(int A)->vector<T, Allocator>::iterator {
	if (_position - A < 0)
		throw exception("going beyond the borders of the vector");
	else {
		vector<T, Allocator>::iterator B = A;
		B._position -= A;
		B.ptr -= A;
		return B;
	}
}

template<class T, class Allocator>
auto vector<T, Allocator>::iterator::operator--(int)-> const vector<T, Allocator>::iterator& {
	if (_position-1 < 0)
		throw exception("going beyond the borders of the vector");
	else {
		vector<T, Allocator>::iterator t = *this;
		_position--;
		ptr--;
		return t;
	}
}

template<class T, class Allocator>
auto vector<T, Allocator>::iterator::operator--()->vector<T, Allocator>::iterator& {
	if (_position-1 < 0)
		throw exception("going beyond the borders of the vector");
	else {
		_position--;
		ptr--;
		return *this;
	}
}

template<class T, class Allocator>
auto vector<T, Allocator>::iterator::operator-=(int A)->vector<T, Allocator>::iterator& {
	if (_position - A < 0)
		throw exception("going beyond the borders of the vector");
	else {
		_position -= A;
		ptr -= A;
		return *this;
	}
}

template<class T, class Allocator>
auto vector<T, Allocator>::iterator::operator+=(int A)->vector<T, Allocator>::iterator& {
	if (_position + A > ptr_1->_size)
		throw exception("going beyond the borders of the vector");
	else {
	_position += A;
	ptr += A;
	return *this;
	}
}


template<class T, class Allocator>
auto vector<T, Allocator>::insert(const vector<T, Allocator>::iterator& A, T&& B)->void {
	if (A.ptr_1 != this)
		throw exception("going beyond the borders of the vector");
	else {
		vector<T, Allocator>::push_back(std::move(B));
		for (int i = _size - 1; i > A._position; i--) {
			T a = ptr[i];
			ptr[i] = ptr[i - 1];
			ptr[i - 1] = a;
		}
	}
}

template<class T, class Allocator>
auto vector<T, Allocator>::insert(const vector<T, Allocator>::iterator& A, const T& B)->void {
	if (A.ptr_1 != this)
		throw exception("going beyond the borders of the vector");
	else {
		vector<T, Allocator>::push_back(B);
		for (int i = _size - 1; i > A._position; i--) {
			T a = ptr[i];
			ptr[i] = ptr[i - 1];
			ptr[i - 1] = a;
		}
	}
}

template<class T, class Allocator>
void vector<T, Allocator>::erase(const vector<T, Allocator>::iterator& A) {
	if (A._position >= _size || A._position < 0 || A.ptr_1 != this)
		throw exception("going beyond the borders of the vector");
	else if (_size == 0)
		throw exception("vector is empty");
	else if (_size == 1)
		_size = 0;
	else {
		for (int i = A._position; i < _size-1; i++) {
			T a = ptr[i];
			ptr[i] = ptr[i + 1];
			ptr[i + 1] = a;
		}
		_size--;
	}
}