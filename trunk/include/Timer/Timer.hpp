#ifndef __TIMER_TIMER_HPP
#define __TIMER_TIMER_HPP

#include "impl/BasicTimer.hpp"
#include "impl/TimerImpl.hpp"


namespace async
{
	namespace timer
	{
		typedef impl::BasicTimer<impl::WaitableTimer>	Timer;

		typedef async::iocp::pointer<Timer>				TimerPtr;
	}
}



#endif