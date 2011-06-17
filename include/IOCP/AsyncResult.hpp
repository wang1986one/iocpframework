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
			enum { IS_OVERLAPPED = FALSE };

			// 利用函数指针，避免virtual function
			typedef void (*CallbackFuncPtr)(AsyncCallbackBase*, u_long, u_long);
			CallbackFuncPtr callback_;

			// 是否是IO回调
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
				void *tmp = (char *)overlapped ? (char *)overlapped - sizeof(AsyncCallbackBase) : (char *)key;
				AsyncCallbackBasePtr p(static_cast<AsyncCallbackBase*>(tmp));
				
				p->Invoke(size, error);
				p->Release();
			}

			static void Call(const AsyncCallbackBasePtr &result)
			{
				result->Invoke(0, ::GetLastError());
				result->Release();
			}
		};

		// 回调接口
		typedef std::tr1::function<void(u_long size, u_long error)> CallbackType;

		static std::tr1::_Ph<1> _Size;
		static std::tr1::_Ph<2>	_Error;

		//---------------------------------------------------------------------------
		// class AsyncCallback

		// 非IO回调
		template < typename HandlerT >
		struct AsyncCallback
			: public AsyncCallbackBase
		{
			enum { IS_OVERLAPPED = FALSE };
			HandlerT handler_;

			explicit AsyncCallback(const HandlerT &handler)
				: AsyncCallbackBase(&AsyncCallback<HandlerT>::Call)
				, handler_(handler)
			{}

		private:
			static void Call(AsyncCallbackBase *param, u_long, u_long)
			{
				AsyncCallback *pThis = static_cast<AsyncCallback *>(param);
				pThis->handler_();
			}
		};

		template < typename HandlerT >
		inline AsyncCallbackBase *MakeAsyncCallback(const HandlerT &handler)
		{
			return new AsyncCallback<HandlerT>(handler);
		}


		//---------------------------------------------------------------------------
		// class AsyncIOCallback

		// IO回调

		struct AsyncIOCallback
			: public OVERLAPPED
			, public AsyncCallbackBase
		{
			enum { IS_OVERLAPPED = TRUE };
			CallbackType handler_;

			explicit AsyncIOCallback(const CallbackType &callback)
				: AsyncCallbackBase(&AsyncIOCallback::Call)
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

			OVERLAPPED *Get()
			{
				return static_cast<OVERLAPPED *>(this);
			}

		public:
			static void Call(AsyncCallbackBase *param, u_long size, u_long error)
			{
				AsyncIOCallback *pThis = static_cast<AsyncIOCallback *>(param);
				pThis->handler_(size, error);
			}

		private:
			AsyncIOCallback();
		};

		inline AsyncIOCallback *MakeAsyncIOCallback(const CallbackType &handler)
		{
			return new AsyncIOCallback(handler);
		}

	}
}



#endif