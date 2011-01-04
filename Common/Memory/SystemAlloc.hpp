#ifndef __MEMORY_SYSTEM_ALLOC_HPP
#define __MEMORY_SYSTEM_ALLOC_HPP


#include "MemoryBase.hpp"


namespace DataStructure
{



	// =========================================================================
	// class StdlibAlloc

	class StdlibAlloc
	{
	public:
		// type definitions
		typedef char		value_type;
		typedef char *		pointer;
		typedef const char *const_pointer;
		typedef char &		reference;
		typedef const char &const_reference;
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;

		typedef StdlibAlloc ClassType;	// class type	
		typedef StdlibAlloc AllocType;

	public:
		enum { Padding = 16 /* must >= sizeof(AllocateHeader) */ };

	public:
		static void* Allocate(size_type cb) 
		{
			return ::malloc(cb);
		}

		static void* ReAllocate(void* p, size_type/* oldSize*/, size_type newSize) 
		{
			return ::realloc(p, newSize);
		}

		static void Deallocate(void* p)			
		{ ::free(p); }
		static void Deallocate(void* p, size_type)	
		{ ::free(p); }

		static void swap(ClassType&/* o*/)	
		{}

		static size_type AllocSize(void* p)
		{
			// _msize--returns the size of a memory block allocated in the heap
			return _msize(p);
		}
	};
}


#endif