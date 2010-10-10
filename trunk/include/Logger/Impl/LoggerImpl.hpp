#ifndef __LOGGER_LOGGER_IMPL_HPP
#define __LOGGER_LOGGER_IMPL_HPP


#include "LoggerService.hpp"
#include "BasicLogger.hpp"


namespace async
{
	namespace logger
	{
		typedef impl::BasicLogger<impl::LoggerService>	Logger;
		typedef iocp::pointer<Logger>					LoggerPtr;
	}
}






#endif