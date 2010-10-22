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
		// ��IO�첽�ص����

		struct AsyncCallbackBase;
		typedef pointer<AsyncCallbackBase> AsyncCallbackBasePtr;

		struct AsyncCallbackBase
			: public Object
		{
			// �Ƿ�����OVERLAPPED
			enum { IS_OVERLAPPED = FALSE };		

			// ���ú���ָ�룬����virtual function
			typedef void (*CallbackFuncPtr)(AsyncCallbackBase *, u_long, u_long);
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
				AsyncCallbackBasePtr p = reinterpret_cast<AsyncCallbackBase *>(key);
				p->Release();

				p->Invoke(size, error);
			}

			static void Call(AsyncCallbackBasePtr &result)
			{
				result->Invoke(0, ::GetLastError());
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
		inline AsyncCallbackBasePtr MakeAsyncCallback(const HandlerT &handler)
		{
			return new AsyncCallback<HandlerT>(handler);
		}



		struct AsyncResult;
		typedef pointer<AsyncResult> AsyncResultPtr;

		//typedef std::tr1::function<void(const AsyncResultPtr &asyncResult, u_long bytes, u_long error)> AsyncCallbackFunc;


		//---------------------------------------------------------------------------
		// class AsyncResult
		// ��Ӳ㣬���𻺳������շ��ߺͻص�����

		struct AsyncResult
			: public OVERLAPPED
			, public Object
		{
			// �Ƿ�����OVERLAPPED
			enum { IS_OVERLAPPED = TRUE };

			// �ص�����
			AsyncCallbackBasePtr callback_;
	

			template<typename HandlerT>
			explicit AsyncResult(const HandlerT &callback)
				: callback_(MakeAsyncCallback(callback))
			{
				RtlZeroMemory((OVERLAPPED *)this, sizeof(OVERLAPPED));
			}
			
			template<typename HandlerT>
			void Reset(const HandlerT &callback)
			{
				callback_ = MakeAsyncCallback(callback);
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
			static void Call(KeyT *key, OverlappedT *overlapped, u_long size, u_long error)
			{
				AsyncResultPtr asynResult = static_cast<AsyncResult *>(overlapped);
				asynResult->Release();

				_CallImpl(asynResult, size, error);
			}

			static void Call(const AsyncResultPtr &result)
			{
				_CallImpl(result, 0, ::GetLastError());
			}


		private:
			static void _CallImpl(const AsyncResultPtr &result, u_long size, u_long error)
			{
				result->callback_->Invoke(size, error);
			}

		private:
			AsyncResult();
		};

	}
}



#endif