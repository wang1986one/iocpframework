#ifndef __IOCP_OBJECT_FACTORY_HPP
#define __IOCP_OBJECT_FACTORY_HPP

#include "MemoryMgr.hpp"
#include <memory>


namespace async
{
	namespace iocp
	{
		// 每个类型一个Memory Pool
		template<typename T>
		struct ObjectPool
		{

			typedef std::tr1::shared_ptr<T> ObjectPtr;

			typedef async::memory::SGIMTMemoryPool SmallObjectMemoryPool;
			typedef async::memory::MemAllocator<T, SmallObjectMemoryPool>	Allocator;


			static Allocator &GetAllocator()
			{
				static SmallObjectMemoryPool pool;
				static Allocator allocator(pool);

				return allocator;
			}
		};


		// 每个类型的内存的声请释放
		template<typename T, typename PoolTraits = ObjectPool<T>>
		struct ObjectFactory
		{
			static void ReleaseBuffer(typename ObjectPool<T>::Allocator::pointer p)
			{
				return PoolTraits::GetAllocator().deallocate(p);
			}

			static T *CreateBuffer()
			{
				return PoolTraits::GetAllocator().allocate();
			}
		};


		template<typename T>
		inline void ObjectDeallocate(T *p)
		{
			return ObjectFactory<T>::ReleaseBuffer(p);
		}

		template<typename T>
		inline T *ObjectAlloc()
		{
			return ::new (ObjectFactory<T>::CreateBuffer()) T;
		}

		template<typename T, typename Arg1>
		inline T *ObjectAlloc(const Arg1 &a1)
		{
			return ::new (ObjectFactory<T>::CreateBuffer()) T(a1);
		}

		template<typename T, typename Arg1, typename Arg2>
		inline T *ObjectAlloc(const Arg1 &a1, const Arg2& a2)
		{
			return ::new (ObjectFactory<T>::CreateBuffer()) T(a1, a2);
		}

		template<typename T, typename Arg1, typename Arg2, typename Arg3>
		inline T *ObjectAlloc(const Arg1& a1, const Arg2& a2, const Arg3& a3)
		{
			return ::new (ObjectFactory<T>::CreateBuffer()) T(a1, a2, a3);
		}

		template<typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
		inline T *ObjectAlloc(const Arg1& a1, const Arg2& a2, const Arg3& a3, const Arg4 &a4)
		{
			return ::new (ObjectFactory<T>::CreateBuffer()) T(a1, a2, a3, a4);
		}

		template<typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
		inline T *ObjectAlloc(const Arg1& a1, const Arg2& a2, const Arg3& a3, const Arg4 &a4, const Arg5 &a5)
		{
			return ::new (ObjectFactory<T>::CreateBuffer()) T(a1, a2, a3, a4, a5);
		}

		template<typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
		inline T *ObjectAlloc(const Arg1& a1, const Arg2& a2, const Arg3& a3, const Arg4 &a4, const Arg5 &a5, const Arg6 &a6)
		{
			return ::new (ObjectFactory<T>::CreateBuffer()) T(a1, a2, a3, a4, a5, a6);
		}

		template<typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
		inline T *ObjectAlloc(const Arg1& a1, const Arg2& a2, const Arg3& a3, const Arg4 &a4, const Arg5 &a5, const Arg6 &a6, const Arg7 &a7)
		{
			return ::new (ObjectFactory<T>::CreateBuffer()) T(a1, a2, a3, a4, a5, a6, a7);
		}
	}
}








#endif