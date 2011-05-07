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
			// 是否是IO回调
			enum { IS_OVERLAPPED = FALSE };

			
			AsyncCallbackBase()
			{}
			virtual ~AsyncCallbackBase()
			{}

			virtual void Invoke(u_long size, u_long error) = 0;

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
		struct AsyncCallback
			: public AsyncCallbackBase
		{
			enum { IS_OVERLAPPED = FALSE };
			CallbackType handler_;

			explicit AsyncCallback(const CallbackType &handler)
				: handler_(handler)
			{}

		private:
			virtual void Invoke(u_long size, u_long error)
			{
				handler_(size, error);
			}
		};

		inline AsyncCallbackBasePtr MakeAsyncCallback(const CallbackType &handler)
		{
			return AsyncCallbackBasePtr(new AsyncCallback(handler));
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
				: handler_(callback)
			{
				memset((OVERLAPPED *)this, 0, sizeof(OVERLAPPED));
				/*OVERLAPPED::Internal	= 0;
				OVERLAPPED::InternalHigh= 0;
				OVERLAPPED::Offset		= 0;
				OVERLAPPED::OffsetHigh	= 0;
				OVERLAPPED::hEvent		= 0;*/
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
			virtual void Invoke(u_long size, u_long error)
			{
				handler_(size, error);
			}
		};

		inline AsyncIOCallback *MakeAsyncIOCallback(const CallbackType &handler)
		{
			return new AsyncIOCallback(handler);
		}

	}
}



#endif