#ifndef __IOCP_ASYNC_RESULT_HPP
#define __IOCP_ASYNC_RESULT_HPP


#include "Object.hpp"


namespace async
{
	namespace iocp
	{


		namespace HandlerInvoke
		{
			template<typename HandlerT, typename T>
			inline void Invoke(const HandlerT &callback, const T &val, u_long size)
			{
				callback(std::tr1::cref(val), size);
			}
		}



		struct AsyncResult;
		typedef pointer<AsyncResult> AsyncResultPtr;

		typedef std::tr1::function<void(const AsyncResultPtr &asyncResult, u_long)> AsyncCallbackFunc;


		//---------------------------------------------------------------------------
		// class AsyncResult
		// 间接层，负责缓冲区、收发者和回调函数

		struct AsyncResult
			: public OVERLAPPED
			, public Object
		{
			// 是否利用OVERLAPPED
			enum { IS_OVERLAPPED = TRUE };

			// 缓冲区
			ObjectPtr	m_buffer;
			// 异步状态
			ObjectPtr	m_accept;
			// 回调函数
			AsyncCallbackFunc m_callback;


			AsyncResult::AsyncResult(const ObjectPtr &buffer, const ObjectPtr &accept, const AsyncCallbackFunc &callback)
				: m_buffer(buffer), m_accept(accept), m_callback(callback)
			{
				RtlZeroMemory((OVERLAPPED *)this, sizeof(OVERLAPPED));
			}

			void reset(const ObjectPtr &buffer,
				const ObjectPtr &accept, const AsyncCallbackFunc &callback)
			{
				m_buffer		= buffer;
				m_accept		= accept;
				m_callback		= callback;
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
			template<typename KeyT, typename OverlappedT>
			static void Call(KeyT *key, OverlappedT *overlapped, u_long size)
			{
				AsyncResultPtr asynResult = static_cast<AsyncResult *>(overlapped);
				asynResult->Release();

				_CallImpl(asynResult, size);
			}

			static void Call(const AsyncResultPtr &result)
			{
				_CallImpl(result, 0);
			}


		private:
			static void _CallImpl(const AsyncResultPtr &result, u_long size)
			{
				if( result->m_callback != NULL )
					HandlerInvoke::Invoke(result->m_callback, result, size);
					//result->m_callback(std::tr1::cref(result), size);
			}

		private:
			AsyncResult();
		};



		struct AsyncCallbackBase;
		typedef pointer<AsyncCallbackBase> AsyncCallbackBasePtr;

		struct AsyncCallbackBase
			: public Object
		{
			// 是否利用OVERLAPPED
			enum { IS_OVERLAPPED = FALSE };		

			// 利用函数指针，避免virtual function
			typedef void (*CallbackFuncPtr)(AsyncCallbackBase *);
			CallbackFuncPtr callback_;

			AsyncCallbackBase(CallbackFuncPtr callback)
				: callback_(callback)
			{}

			void Invoke()
			{
				callback_(this);
			}

			template<typename KeyT, typename OverlappedT>
			static void Call(KeyT *key, OverlappedT *overlapped, u_long/* size*/)
			{
				AsyncCallbackBasePtr p = reinterpret_cast<AsyncCallbackBase *>(key);
				p->Release();

				p->Invoke();
			}

			static void Call(AsyncCallbackBasePtr &result)
			{
				result->Invoke();
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
			static void Call(AsyncCallbackBase *param)
			{
				AsyncCallback<HandlerT> *pThis = static_cast<AsyncCallback<HandlerT> *>(param);
				pThis->handler_();
			}
		};

		template<typename HandlerT>
		inline AsyncCallbackBasePtr MakeAsyncCallback(const HandlerT &handler)
		{
			return new AsyncCallback<HandlerT>(handler);
		}


	}
}



#endif