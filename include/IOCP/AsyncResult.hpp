#ifndef __IOCP_ASYNC_RESULT_HPP
#define __IOCP_ASYNC_RESULT_HPP

#include "../Basic.hpp"
#include "Pointer.hpp"
#include "ObjectFactory.hpp"
#include "../../MultiThread/Tls.hpp"
#include <functional>
#include <type_traits>


namespace async
{
	namespace iocp
	{

		// �ص��ӿ�
		typedef std::tr1::function<void(u_long size, u_long error)> CallbackType;
		static std::tr1::_Ph<1> _Size;
		static std::tr1::_Ph<2>	_Error;


		// Allocate IO Callback
	
		template < typename T >
		struct IOAsyncCallback;

		struct AsyncCallbackBase;
		struct AsyncCallback;
		typedef Pointer< AsyncCallbackBase, IOAsyncCallback<AsyncCallback> > AsyncCallbackBasePtr;


		// Pointer Realase
		template < >
		struct IOAsyncCallback<AsyncCallback>
		{
			template < typename HandlerT >
			AsyncCallbackBase *operator()(const HandlerT &handler)
			{
				return reinterpret_cast<AsyncCallbackBase *>(ObjectAllocate<AsyncCallback>(handler));
			}

			void operator()(AsyncCallbackBase *p)
			{
				ObjectDeallocate<AsyncCallback>(reinterpret_cast<AsyncCallback *>(p));
			}
		};


		template < typename T, typename HandlerT >
		inline AsyncCallbackBase *MakeAsyncCallback(const HandlerT &handler)
		{
			return IOAsyncCallback<T>()(handler);
		}

		//---------------------------------------------------------------------------
		// class AsyncCallbackBase

		struct AsyncCallbackBase
			: public OVERLAPPED
		{
			AsyncCallbackBase()
			{
				RtlZeroMemory((OVERLAPPED *)this, sizeof(OVERLAPPED));
			}

			virtual ~AsyncCallbackBase()
			{

			}

			virtual void Invoke(AsyncCallbackBase *p, u_long size, u_long error) = 0;

			template<typename KeyT, typename OverlappedT>
			static void Call(KeyT *key, OverlappedT *overlapped, u_long size, u_long error)
			{
				AsyncCallbackBase *p(static_cast<AsyncCallback *>(overlapped));
				
				p->Invoke(p, size, error);
			}
		};


		//---------------------------------------------------------------------------
		// class AsyncCallback

		struct AsyncCallback
			: public AsyncCallbackBase
		{
			CallbackType handler_;

			explicit AsyncCallback(const CallbackType &callback)
				: handler_(callback)
			{}
			virtual ~AsyncCallback()
			{

			}

		public:
			virtual void Invoke(AsyncCallbackBase *p, u_long size, u_long error)
			{
				if( handler_ != 0 )
					handler_(size, error);

				AsyncCallbackBasePtr ptr(p);
			}

		private:
			AsyncCallback();
		};

		

		// Memory Pool
		template< >
		struct ObjectFactory<AsyncCallback>
		{
			typedef memory::FixedMemoryPool<true, sizeof(AsyncCallback)>	PoolType;
			typedef ObjectPool<PoolType>									ObjectPoolType;
		};



		struct TlsMemoryPool
		{
			typedef std::tr1::aligned_storage<
				sizeof(AsyncCallback),
				std::tr1::alignment_of<AsyncCallback>::value 
			>::type StorageBuffer;

			StorageBuffer buf_;

			static void *Allocate(size_t n)
			{
	
				return 0;//reinterpret_cast<AsyncCallback *>(&Pool.buf_);//::operator new(n);
			}

			static void Deallocate(void *p, size_t n)
			{
				//::operator delete(p);
			}
		};


		
	}
}



#endif