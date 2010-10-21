#ifndef __NETWORK_TCP_HPP
#define __NETWORK_TCP_HPP



#include "Socket.hpp"

#include "BasicAcceptor.hpp"
#include "BasicStreamSocket.hpp"


#include "SocketOption.hpp"
#include "SocketProvider.hpp"
#include "BufferHelper.hpp"
#include "Write.hpp"
#include "Read.hpp"

#include "../iocp/ReadWriteBuffer.hpp"


namespace async
{
	namespace network
	{

		// --------------------------------------------------
		// class Tcp

		class Tcp
		{
		public:
			typedef BasicAcceptor<Tcp>			Accpetor;
			typedef BasicStreamSocket<Tcp>		StreamSocket;
			


		private:
			int family_;

		private:
			explicit Tcp(int family)
				: family_(family)
			{}


		public:
			int Type() const
			{
				return SOCK_STREAM;
			}

			int Protocol() const
			{
				return IPPROTO_TCP;
			}

			int Family() const
			{
				return family_;
			}

		public:
			static Tcp V4()
			{
				return Tcp(AF_INET);
			}

		public:
			friend bool operator==(const Tcp &lhs, const Tcp &rhs)
			{
				return lhs.family_ == rhs.family_;
			}
			friend bool operator!=(const Tcp &lhs, const Tcp &rhs)
			{
				return !(lhs == rhs);
			}
		};
	}
}







#endif