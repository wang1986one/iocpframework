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
			inline void Invoke(const HandlerT &callback, const T &val)
			{
				callback(std::tr1::cref(val));
			}
		}



		struct AsyncResult;
		typedef pointer<AsyncResult> AsyncResultPtr;

		typedef std::tr1::function<void(const AsyncResultPtr &asyncResult)> AsyncCallbackFunc;


		//---------------------------------------------------------------------------
		// class AsyncResult
		// 间接层，负责缓冲区、收发者和回调函数

		struct AsyncResult
			: public OVERLAPPED
			, public Object
		{
			// 是否利用OVERLAPPED
			enum { IS_OVERLAPPED = TRUE };

			// 发送者
			ObjectPtr	m_sender;
			// 缓冲区
			ObjectPtr	m_buffer;
			// 异步状态
			ObjectPtr	m_asynState;
			// 内部状态
			ObjectPtr	m_internalState;
			// 回调函数
			AsyncCallbackFunc m_callback;


			AsyncResult::AsyncResult(const ObjectPtr &sender, const ObjectPtr &buffer,
				const ObjectPtr &asyncState, const ObjectPtr &internalState, const AsyncCallbackFunc &callback)
				: m_sender(sender), m_buffer(buffer), m_asynState(asyncState), m_internalState(internalState), m_callback(callback)
			{
				RtlZeroMemory((OVERLAPPED *)this, sizeof(OVERLAPPED));
			}

			void reset(const ObjectPtr &sender, const ObjectPtr &buffer,
				const ObjectPtr &asyncState, const ObjectPtr &internalState, const AsyncCallbackFunc &callback)
			{
				m_sender		= sender;
				m_buffer		= buffer;
				m_asynState		= asyncState;
				m_internalState = internalState;
				m_callback		= callback;
			}

			template<typename HandleT>
			size_t EndAsync(HandleT &handle)
			{
				DWORD size = 0;

				if( 0 == ::GetOverlappedResult(reinterpret_cast<HANDLE>(handle), this, &size, TRUE) )
					throw Win32Exception("GetOverlappedResult");

				return size;
			}


		public:
			template<typename KeyT, typename OverlappedT>
			static void Call(KeyT *key, OverlappedT *overlapped)
			{
				AsyncResultPtr asynResult = static_cast<AsyncResult *>(overlapped);
				asynResult->Release();

				Call(asynResult);
			}

			static void Call(const AsyncResultPtr &result)
			{
				if( result->m_callback != NULL )
					HandlerInvoke::Invoke(result->m_callback, result);
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
			static void Call(KeyT *key, OverlappedT *overlapped)
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