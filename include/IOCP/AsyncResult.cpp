#include "stdafx.h"
#include "AsyncResult.hpp"


namespace async
{
	namespace iocp
	{


		AsyncCallbackBase::AsyncCallbackBase(CallbackFuncPtr callback)
			: callback_(callback)
		{
			RtlZeroMemory((OVERLAPPED *)this, sizeof(OVERLAPPED));
		}

		void AsyncCallbackBase::Invoke(u_long size, u_long error)
		{
			callback_(size, error);
		}




		AsyncCallback::AsyncCallback(const CallbackType &handler)
			: AsyncCallbackBase(&AsyncCallback::Call)
			, handler_(handler)
		{}

		void AsyncCallback::Call(AsyncCallbackBase *param, u_long, u_long)
		{
			AsyncCallback *pThis = static_cast<AsyncCallback *>(param);
			pThis->handler_();
		}





		AsyncCallback::AsyncCallback(const CallbackType &callback)
			: AsyncCallbackBase(&AsyncCallback::Call)
			, handler_(callback)
		{}


		void AsyncCallback::Call(AsyncCallbackBase *param, u_long size, u_long error)
		{
			AsyncCallback *pThis = static_cast<AsyncCallback *>(param);
			pThis->handler_(size, error);
		}
	}
}