#ifndef __TIMER_WAITEABLE_TIMER_HPP
#define __TIMER_WAITEABLE_TIMER_HPP

#include "../../Basic.hpp"
#include "../../iocp/WinException.hpp"
#include "../../iocp/Callback.hpp"


namespace async
{
	namespace timer
	{
		namespace impl
		{
			// --------------------------------------------------
			// class WaitableTimer

			class WaitableTimer
			{
			private:
				HANDLE timer_;
				long period_;		// 间隔时间
				long due_;			// 第一次后延长时间
				
			public:
				WaitableTimer(long period, long due, bool manualReset = false, const wchar_t *timerName = NULL)
					: timer_(NULL)
					, period_(period)
					, due_(due)
				{
					timer_ = ::CreateWaitableTimerW(NULL, manualReset ? TRUE : FALSE, timerName);

				}

				~WaitableTimer()
				{
					if( timer_ != NULL )
					{
						::CloseHandle(timer_);
						timer_ = NULL;
					}
				}

			public:
				operator HANDLE()
				{
					return timer_;
				}
				operator HANDLE() const
				{
					return timer_;
				}

				operator size_t()
				{
					return (size_t)timer_;
				}

				bool operator=(const WaitableTimer &timer)
				{
					return timer.timer_ == timer_;
				}

			public:
				void SetTimer(long period, long delay)
				{
					assert(timer_ != NULL);

					LARGE_INTEGER dueTime = {0};
					dueTime.QuadPart = -(delay * 10000000);


					if( !::SetWaitableTimer(timer_, &dueTime, period, NULL, NULL, TRUE) )
						throw async::iocp::Win32Exception("SetWaitableTimer");
				}

				void Cancel()
				{
					assert(timer_ != NULL);
					if( !::CancelWaitableTimer(timer_) )
						throw async::iocp::Win32Exception("CancelWaitableTimer");
				}

				void SyncWait() const
				{
					assert(timer_ != NULL);

					DWORD res = ::WaitForSingleObject(timer_, period_);
					if( res == WAIT_FAILED )
						throw async::iocp::Win32Exception("WaitForSingleObject");
				}

				void AsyncWait()
				{
					assert(timer_ != NULL);

					SetTimer(period_, due_);
				}
			};

		}
	}
}

namespace std
{


namespace tr1 
{	
	// always include std::tr1::hash for unordered_map/set
	// TEMPLATE CLASS hash
	template<>
	class hash<std::tr1::shared_ptr<async::timer::impl::WaitableTimer>>
		: public unary_function<std::tr1::shared_ptr<async::timer::impl::WaitableTimer>, size_t>
	{	
		typedef std::tr1::shared_ptr<async::timer::impl::WaitableTimer> _Kty;
		
		// hash functor
	public:
		size_t operator()(const _Kty& _Keyval) const
		{	// hash _Keyval to size_t value by pseudorandomizing transform
			ldiv_t _Qrem = ldiv((long)(size_t)(*_Keyval), 127773);

			_Qrem.rem = 16807 * _Qrem.rem - 2836 * _Qrem.quot;
			if (_Qrem.rem < 0)
				_Qrem.rem += 2147483647;
			return ((size_t)_Qrem.rem);
		}
	};
}

}

#endif