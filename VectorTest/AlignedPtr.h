#pragma once
#include <memory>

template <class T>
class AlignedPtr
{
public:

	AlignedPtr()
		:
		a_ptr(nullptr)
	{
	}
	AlignedPtr(unsigned int Count, unsigned int Alignment = 16)		
	{
		unsigned int size = sizeof(T) * Count;
		a_ptr = (T*)_aligned_malloc(size, Alignment);
	}
	AlignedPtr(AlignedPtr &&APtr)
	{
		Reset(std::forward<AlignedPtr &&>(APtr));
	}
	AlignedPtr &operator=(AlignedPtr &&Ptr)
	{
		Reset(std::forward<AlignedPtr &&>(Ptr));
		return (*this);
	}
	T& operator[](unsigned int Index)
	{
		return a_ptr[Index];
	}
	bool operator==(const AlignedPtr &Ptr)
	{
		return a_ptr == Ptr.a_ptr;
	}
	operator bool()
	{
		return a_ptr != nullptr ? true : false;
	}

	T* Get()
	{
		return a_ptr;
	}
	void Reset(T *Ptr)
	{
		Release();
		a_ptr = Ptr;
	}
	void Reset(AlignedPtr &&Ptr)
	{
		Release();
		a_ptr = Ptr.a_ptr;
		Ptr.Release();
	}
	void Release()
	{
		if (a_ptr)
		{
			_aligned_free(a_ptr);
			a_ptr = nullptr;
		}
	}

	~AlignedPtr()
	{
		Release();
	}

	// Use std::move instead
	AlignedPtr &operator=(const AlignedPtr &Ptr) = delete;

	// Objects can only be moved
	AlignedPtr(const AlignedPtr &Ptr) = delete;

private:
	T *a_ptr;
};

