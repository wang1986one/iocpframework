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
			// 利用函数指针，避免virtual function
			typedef void (*CallbackFuncPtr)(AsyncCallbackBase*, u_long, u_long);
			CallbackFuncPtr callback_;

			// 是否是IO回调
			bool isIO_;
			AsyncCallbackBase(CallbackFuncPtr callback, bool ioFlag)
				: callback_(callback)
				, isIO_(ioFlag)
			{}

			void Invoke(u_long size, u_long error)
			{
				callback_(this, size, error);
			}

			bool IsIOCallback() const
			{
				return isIO_;
			}

			template<typename KeyT, typename OverlappedT>
			static void Call(KeyT *key, OverlappedT *overlapped, u_long size, u_long error)
			{
				void *tmp = isIO_ ? overlapped : key;
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



		//---------------------------------------------------------------------------
		// class AsyncCallback

		// 非IO回调
		template<typename HandlerT>
		struct AsyncCallback
			: public AsyncCallbackBase
		{
			HandlerT handler_;

			explicit AsyncCallback(const HandlerT &handler)
				: AsyncCallbackBase(&AsyncCallback<HandlerT>::Call, false)
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

		// IO回调

		template<typename HandlerT>
		struct AsyncIOCallback
			: public AsyncCallbackBase
			, public OVERLAPPED
		{
			HandlerT handler_;

			template<typename HandlerT>
			explicit AsyncIOCallback(const HandlerT &callback)
				: AsyncCallbackBase(&AsyncIOCallback<HandlerT>::Call, true)
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