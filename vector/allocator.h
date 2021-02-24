#pragma once

#include <new>
#include "exception.h"

template <class T>
class allocator {
public:
	T* allocate(int count) {
		T* Ptr = nullptr;
		if (count == 0);
		else if (size_t(-1) / sizeof(T) < count || (Ptr = new (std::nothrow) T[sizeof(T) * count])== nullptr)
			throw exception("can't allocate memory");
		return Ptr;
	}

	void deallocate(T* ptr) {
		 delete[] ptr;
	}
};
