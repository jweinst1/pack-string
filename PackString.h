#ifndef PACK_STRING_LIB_H
#define PACK_STRING_LIB_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>

/**
 * @brief Macro allows realloc to be used in CPP with cast requirements
 */
#define CPP_REALLOC(ptr, type, new_size) reinterpret_cast<type*>(std::realloc(reinterpret_cast<void*>(ptr), new_size))


struct MallocArrayException : public std::exception
{
	enum ErrType
	{
		NULL_ON_MALLOC,
		NULL_ON_REALLOC,
		OUT_OF_BOUNDS
	};

	MallocArrayException(MallocArrayException::ErrType code);

	const char * what () const throw ()
    {
    	return message;
    }

    const char* message;
    ErrType code;
};

/**
 * @class MallocArray
 * @brief A template class array that uses memory from malloc and realloc
 * @detail This is a class meant to enhance performance by using a template array
 *         that derives memory from the C standard library functions.
 *         In particular, C stdlib has realloc, while C++ does not. If there is
 *         a chance to extend existing memory without a copy, it's a huge perf
 *         boost.
 */
template<class T> 
class MallocArray
{
public:
	MallocArray(size_t size = 20): _len(0),
	                               _cap(size * sizeof(T)), 
	                               _items(reinterpret_cast<T*>(std::malloc(_cap)))
	{
		if(_items == nullptr) throw MallocArrayException(MallocArrayException::NULL_ON_MALLOC);
	}
	~MallocArray()
	{
		std::free(_items);
	}

	size_t getLen() const { return _len; }
	size_t getCap() const { return _cap; }
	T* getPtr() const { return _items; }

	T& operator[] (size_t index)
	{
		if(index > _len) throw MallocArrayException(MallocArrayException::OUT_OF_BOUNDS);
		else return _items[index];
	}
private:
	void expand()
	{
#ifdef PSM_ARRAY_EXP_GROW
		_cap *= _cap;
#else
		_cap *= 2;
#endif // PSM_ARRAY_EXP_GROW
		_items = CPP_REALLOC(_items, T, _cap);
		if(_items == nullptr) throw MallocArrayException(MallocArrayException::NULL_ON_REALLOC);
	}
private:
	size_t _len;
	size_t _cap;
	T* _items;
};


class PackString
{
private:
	// todo
	int _a;
};

#endif // PACK_STRING_LIB_H
