#ifndef __IOCP_ASYNC_RESULT_HPP
#define __IOCP_ASYNC_RESULT_HPP


#include "Object.hpp"


namespace async
{
	namespace iocp
	{


		namespace HandlerInvoke
		{
			template<typename HandlerT>
			inline void Invoke(const HandlerT &callback, u_long size, u_long error)
			{
				callback(size, error);
			}
		}



		

		//---------------------------------------------------------------------------
		// class AsyncCallbackBase
		// 非IO异步回调组件

		struct AsyncCallbackBase;
		typedef pointer<AsyncCallbackBase>	AsyncCallbackBasePtr;


		struct AsyncCallbackBase
			: public Object
		{
			// 是否利用OVERLAPPED
			enum { IS_OVERLAPPED = FALSE };	

			// 利用函数指针，避免virtual function
			typedef void (*CallbackFuncPtr)(AsyncCallbackBase*, u_long, u_long);
			CallbackFuncPtr callback_;

			AsyncCallbackBase(CallbackFuncPtr callback)
				: callback_(callback)
			{}

			void Invoke(u_long size, u_long error)
			{
				callback_(this, size, error);
			}

			template<typename KeyT, typename OverlappedT>
			static void Call(KeyT *key, OverlappedT *overlapped, u_long size, u_long error)
			{
				char *tmp = ((char *)overlapped == 0) ? (char *)key : (char *)overlapped - 0x0C;
				AsyncCallbackBasePtr p(static_cast<AsyncCallbackBase*>((void *)tmp));
				
				p->Invoke(size, error);
				p->Release();
			}

			static void Call(const AsyncCallbackBasePtr &result)
			{
				result->Invoke(0, ::GetLastError());
				result->Release();
			}
		};


		template<typename HandlerT>
		struct AsyncCallback
			: public AsyncCallbackBase
		{
			HandlerT handler_;

			explicit AsyncCallback(const HandlerT &handler)
				: AsyncCallbackBase(&AsyncCallback<HandlerT>::Call)
				, handler_(handler)
			{}

		private:
			static void Call(AsyncCallbackBase *param, u_long size, u_long error)
			{
				AsyncCallback<HandlerT> *pThis = static_cast<AsyncCallback<HandlerT> *>(param);
				pThis->handler_(size, error);
			}
		};

		template<typename HandlerT>
		inline AsyncCallbackBase *MakeAsyncCallback(const HandlerT &handler)
		{
			return new AsyncCallback<HandlerT>(handler);
		}


		//---------------------------------------------------------------------------
		// class AsyncIOCallback
		// 间接层，负责缓冲区、收发者和回调函数

		template<typename HandlerT>
		struct AsyncIOCallback
			: public AsyncCallbackBase
			, public OVERLAPPED
		{
			enum { IS_OVERLAPPED = TRUE };

			HandlerT handler_;

			template<typename HandlerT>
			explicit AsyncIOCallback(const HandlerT &callback)
				: AsyncCallbackBase(&AsyncIOCallback<HandlerT>::Call)
				, handler_(callback)
			{
				//RtlZeroMemory((OVERLAPPED *)this, sizeof(OVERLAPPED));
				OVERLAPPED::Internal	= 0;
				OVERLAPPED::InternalHigh= 0;
				OVERLAPPED::Offset		= 0;
				OVERLAPPED::OffsetHigh	= 0;
				OVERLAPPED::hEvent		= 0;
			}

			template<typename HandleT>
			size_t EndAsync(HandleT handle)
			{
				DWORD size = 0;

				if( 0 == ::GetOverlappedResult(reinterpret_cast<HANDLE>(handle), this, &size, TRUE) )
					throw Win32Exception("GetOverlappedResult");

				return size;
			}

		public:
			static void Call(AsyncCallbackBase *param, u_long size, u_long error)
			{
				AsyncIOCallback<HandlerT> *pThis = static_cast<AsyncIOCallback<HandlerT> *>(param);
				pThis->handler_(size, error);
			}

		private:
			AsyncIOCallback();
		};

		template<typename HandlerT>
		inline AsyncIOCallback<HandlerT> *MakeAsyncIOCallback(const HandlerT &handler)
		{
			return new AsyncIOCallback<HandlerT>(handler);
		}

	}
}



#endif