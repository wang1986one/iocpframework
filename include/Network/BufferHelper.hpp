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
		inline SocketBufferPtr MakeBuffer(const char *buf, size_t sz)
		{
			return MakeBuffer(const_cast<char *>(buf), sz);
		}

		// --------------------------

		template<size_t _N>
		inline SocketBufferPtr MakeBuffer(char (&arr)[_N])
		{
			return SocketBufferPtr(new SocketBuffer(arr, _N));
		}
		template<size_t _N>
		inline SocketBufferPtr MakeBuffer(const char (&arr)[_N])
		{
			return MakeBuffer(const_cast<char (&)[_N]>(arr));
		}

		// --------------------------

		template<size_t _N>
		inline SocketBufferPtr MakeBuffer(std::tr1::array<char, _N> &arr)
		{
			return SocketBufferPtr(new SocketBuffer(arr.data(), _N));
		}
		template<size_t _N>
		inline SocketBufferPtr MakeBuffer(const std::tr1::array<char, _N> &arr)
		{
			return MakeBuffer(const_cast<std::tr1::array<char, _N> &>(arr));
		}

		// --------------------------

		inline SocketBufferPtr MakeBuffer(std::vector<char> &arr)
		{
			return SocketBufferPtr(new SocketBuffer(&arr[0], arr.size()));
		}
		inline SocketBufferPtr MakeBuffer(const std::vector<char> &arr)
		{
			return MakeBuffer(const_cast<std::vector<char> &>(arr));
		}

		// --------------------------

		inline SocketBufferPtr MakeBuffer(std::string &arr)
		{
			return SocketBufferPtr(new SocketBuffer(&*arr.begin(), arr.length()));
		}
		inline SocketBufferPtr MakeBuffer(const std::string &arr)
		{
			return MakeBuffer(const_cast<std::string &>(arr));
		}

		inline SocketBufferPtr MakeBuffer(std::vector<std::string> &arr)
		{
			size_t total = 0;
			for(size_t i = 0; i != arr.size(); ++i)
				total += arr[i].length();

			SocketBufferPtr buf(new SocketBuffer(total));

			size_t len = 0;
			for(size_t i = 0; i != arr.size(); ++i)
			{
				std::copy(arr[i].begin(), arr[i].end(), buf->data(len));
				len += arr[i].length();
			}

			buf->resize(len);
			return buf;
		}
		inline SocketBufferPtr MakeBuffer(const std::vector<std::string> &arr)
		{
			return MakeBuffer(const_cast<std::vector<std::string> &>(arr));
		}



		inline SocketBufferPtr MakeBuffer(std::vector<SocketBufferPtr> &arr)
		{
			size_t total = 0;
			for(size_t i = 0; i != arr.size(); ++i)
				total += arr[i]->size();
		

			SocketBufferPtr buf(new SocketBuffer(total));

			size_t len = 0;
			for(size_t i = 0; i != arr.size(); ++i)
			{
				std::copy(arr[i]->begin(), arr[i]->end(), buf->data(len));
				len += arr[i]->size();
			}

			buf->resize(len);
			return buf;
		}


		inline SocketBufferPtr MakeBuffer(const std::vector<SocketBufferPtr> &arr)
		{
			return MakeBuffer(const_cast<std::vector<SocketBufferPtr> &>(arr));
		}
	}
}


#endif // endif