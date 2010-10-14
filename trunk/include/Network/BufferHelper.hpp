#ifndef __NETWORK_BUFFER_HELPER_HPP
#define __NETWORK_BUFFER_HELPER_HPP



#include "Socket.hpp"
#include <array>
#include <vector>
#include <string>

namespace async
{
	namespace network
	{
		
		// ------------------------

		inline SocketBufferPtr MakeBuffer(char *buf, size_t sz)
		{
			return SocketBufferPtr(new SocketBuffer(buf, sz));
		}


		// --------------------------

		template<size_t _N>
		inline SocketBufferPtr MakeBuffer(char (&arr)[_N])
		{
			return SocketBufferPtr(new SocketBuffer(arr, _N));
		}

		// --------------------------

		template<size_t _N>
		inline SocketBufferPtr MakeBuffer(std::tr1::array<char, _N> &arr)
		{
			return SocketBufferPtr(new SocketBuffer(arr.data(), _N));
		}


		// --------------------------

		inline SocketBufferPtr MakeBuffer(std::vector<char> &arr)
		{
			return SocketBufferPtr(new SocketBuffer(&arr[0], arr.size()));
		}


		// --------------------------

		inline SocketBufferPtr MakeBuffer(std::string &arr)
		{
			return SocketBufferPtr(new SocketBuffer(&*arr.begin(), arr.size()));
		}

	}
}


#endif // endif