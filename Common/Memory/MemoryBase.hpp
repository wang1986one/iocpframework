#ifndef __MEMORY_BASE_HPP
#define __MEMORY_BASE_HPP



#include <crtdbg.h> // _CrtSetDbgFlag, _ASSERTE
#include <malloc.h> // _malloca, _freea
#include <cassert>	// assert
#include <cstdlib>  // malloc, free, _msize
#include <new>		// for placement new

#include "../Constraints.hpp"
#include "TypeTraits.hpp"


namespace DataStructure
{

	inline void EnableMemoryLeakCheck()
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);	// _CRTDBG_REPORT_FLAG
	}


	template<typename Type>
	inline void SwapObject(Type *a, Type *b)
	{
		__try
		{
			void* t = ::_malloca(sizeof(Type));

			::memcpy(t, a, sizeof(Type));
			::memcpy(a, b, sizeof(Type));
			::memcpy(b, t, sizeof(Type));

			::_freea(t);
		}
		// catch any exceptions that may occur
		__except( ::GetExceptionCode() == STATUS_STACK_OVERFLOW )
		{
			// if the stack overflows , use this function to restore
			int nErr = _resetstkoflw();
			if( nErr )
			{
				_exit(1);
			}
		}
	}


	namespace Internal
	{
		template<typename T>
		inline T Round(T x, T y)
		{
			return (x + y - 1) & ~(y - 1);
		}

		template<typename T>
		inline T Max(T x, T y)
		{
			return x > y ? x : y;
		}

		template<typename T>
		inline T Min(T x, T y)
		{
			return x > y ? y : x;
		}

#ifdef _DEBUG
		template<typename T, typename U>
		inline void DebugFill(T *p, U cb)
		{
			::memset(p, 0xcd, cb);
		}
#else
		template<typename T, typename U>
		inline void DebugFill(T *p, U cb)
		{
		}
#endif


		inline size_t log2(size_t val)
		{
			assert(val != 0);

			__asm
			{
				bsr eax, val
			};
		}
	}




	/*
		Allocator解决的是大量小块内存的管理。
		如果我们需要大块内存，则应该考虑直接使用malloc/free，或者其他替换方案。



	*/

	// 不带自动调用构造函数、析构函数的分配内存函数
	template<typename T, typename AllocT>
	inline T *Alloc(AllocT &alloc)
	{
		return static_cast<T *>(alloc.Allocate(sizeof(T)));
	}
	template<typename T, typename AllocT>
	inline T *AllocArray(AllocT &alloc, size_t nCount)
	{
		return static_cast<T *>(alloc.Allocate(sizeof(T) * nCount));
	}

	// 带自动调用构造函数、析构函数的内存分配函数
	template<typename T, typename AllocT>
	inline T *New(AllocT &alloc)
	{
		return ::new(alloc.Allocate(sizeof(T), &DestructorTraits<T>::Destruct)) T;
	}
	template<typename T, typename AllocT, typename TARG1>
	inline T *New(AllocT &alloc, const TARG1 &arg1)
	{
		return ::new(alloc.Allocate(sizeof(T), &DestructorTraits<T>::Destruct)) T(arg1);
	}
	template<typename T, typename AllocT, typename TARG1, typename TARG2>
	inline T *New(AllocT &alloc, const TARG1 &arg1, const TARG2 &arg2)
	{
		return ::new(alloc.Allocate(sizeof(T), &DestructorTraits<T>::Destruct)) T(arg1, arg2);
	}

	// 分配内存数组函数
	template<typename T, typename AllocT>
	inline T *NewArray(AllocT &alloc, size_t nCount)
	{
		return ArrayFactory<T>::Create(alloc, nCount);
	}
}

#endif