#ifndef __IOCP_ASYNC_HANDLER_ALLOC_HPP
#define __IOCP_ASYNC_HANDLER_ALLOC_HPP


#include "../MemoryMgr.hpp"


namespace async
{
	namespace iocp
	{

		namespace internal
		{


			// Allocate Helper Function
			inline void* Allocate(std::size_t s)
			{
				return MemoryMgr::GetMemory().allocate(s);
			}

			inline void Deallocate(void* p, std::size_t s)
			{
				return MemoryMgr::GetMemory().deallocate(static_cast<char *>(p), s);
			}


			// Handler Allocate
			template<typename T>
			T *HandlerAlloc()
			{
				return ::new (Allocate(sizeof(T))) T;
			}

			template<typename T, typename Arg1>
			T *HandlerAlloc(Arg1& a1)
			{
				return ::new (Allocate(sizeof(T))) T(a1);
			}

			template<typename T, typename Arg1, typename Arg2>
			T *HandlerAlloc(Arg1& a1, Arg2& a2)
			{
				return ::new (Allocate(sizeof(T))) T(a1, a2);
			}

			template<typename T, typename Arg1, typename Arg2, typename Arg3>
			T *HandlerAlloc(Arg1& a1, Arg2& a2, Arg3& a3)
			{
				return ::new (Allocate(sizeof(T))) T(a1, a2, a3);
			}

			template<typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
			T *HandlerAlloc(Arg1& a1, Arg2& a2, Arg3& a3, Arg4 &a4)
			{
				return ::new (Allocate(sizeof(T))) T(a1, a2, a3, a4);
			}

			template<typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
			T *HandlerAlloc(Arg1& a1, Arg2& a2, Arg3& a3, Arg4 &a4, Arg5 &a5)
			{
				return ::new (Allocate(sizeof(T))) T(a1, a2, a3, a4, a5);
			}
				
			template<typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
			T *HandlerAlloc(Arg1& a1, Arg2& a2, Arg3& a3, Arg4 &a4, Arg5 &a5, Arg6 &a6)
			{
				return ::new (Allocate(sizeof(T))) T(a1, a2, a3, a4, a5, a6);
			}
	
			template<typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
			T *HandlerAlloc(Arg1& a1, Arg2& a2, Arg3& a3, Arg4 &a4, Arg5 &a5, Arg6 &a6, Arg7 &a7)
			{
				return ::new (Allocate(sizeof(T))) T(a1, a2, a3, a4, a5, a6, a7);
			}


			template<typename T>
			void HandlerDealloc(T *p)
			{
				p->~T();
				Deallocate(p, sizeof(T));
			}
		}

	}
}







#endif