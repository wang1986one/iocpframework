#include "stdafx.h"
#include "Object.hpp"



namespace async
{

	namespace iocp
	{

		DebugCallback::AllocCallback DebugCallback::allocCallback_;
		DebugCallback::DeallocCallback DebugCallback::deallocCallback_;


		void DebugCallback::Alloc(u_long size)
		{
			static volatile LONG uAllocCount = 0;
			static volatile LONG uAllocSize = 0;

			if( uAllocCount >= LONG_MAX )
				uAllocCount = 0;
			if( uAllocSize >= LONG_MAX )
				uAllocSize = 0;

			::InterlockedExchangeAdd(&uAllocCount, 1);
			::InterlockedExchangeAdd(&uAllocSize, size);

			if( allocCallback_ != NULL )
				allocCallback_(uAllocCount, uAllocSize);
		}

		void DebugCallback::Dealloc(u_long size)
		{
			static volatile LONG uDeallocCount = 0;
			static volatile LONG uDeallocSize = 0;

			if( uDeallocCount >= LONG_MAX )
				uDeallocCount = 0;
			if( uDeallocSize >= LONG_MAX )
				uDeallocSize = 0;

			::InterlockedExchangeAdd(&uDeallocCount, 1);
			::InterlockedExchangeAdd(&uDeallocSize, size);

			if( deallocCallback_ != NULL )
				deallocCallback_(uDeallocCount, uDeallocSize);
		}


		void DebugCallback::RegisterCallback(const AllocCallback &allocCallback, const DeallocCallback &deallocCallback)
		{
			allocCallback_ = allocCallback;
			deallocCallback_ = deallocCallback;
		}
	}

}