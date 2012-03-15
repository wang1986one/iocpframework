#include "stdafx.h"
#include "AsyncResult.hpp"


namespace async
{
	namespace iocp
	{

		AsyncCallback::~AsyncCallback()
		{

		}

		void AsyncCallback::Invoke(AsyncCallbackBase *p, u_long size, u_long error)
		{
			if( handler_ != 0 )
				handler_(size, error);

 			AsyncCallbackBasePtr ptr(p);
		}
	}
}