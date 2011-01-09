#include "stdafx.h"

#include "Socket.hpp"
#include "SocketOption.hpp"

#include "../IOCP/WinException.hpp"


namespace async
{


	namespace network
	{

		Socket::Socket(AsyncIODispatcherType &io)
			: socket_(INVALID_SOCKET)
			, io_(io)
		{
			SocketProvider &provider = SocketProvider::GetSingleton(io_);
		}
		Socket::Socket(AsyncIODispatcherType &io, SOCKET sock)
			: socket_(sock)
			, io_(io)
		{
			SocketProvider &provider = SocketProvider::GetSingleton(io_);
		}
		Socket::Socket(AsyncIODispatcherType &io, int family, int type, int protocol)
			: socket_(INVALID_SOCKET)
			, io_(io)
		{
			SocketProvider &provider = SocketProvider::GetSingleton(io_);

			Open(family, type, protocol);
		}

		Socket::~Socket()
		{
			Close();
		}


		void Socket::Open(int family, int nType /* SOCK_STREAM */, int nProtocol /* IPPROTO_TCP */)
		{
			if( IsOpen() )
				throw std::logic_error("Socket already opened!");

			socket_ = ::WSASocket(family, nType, nProtocol, NULL, 0, WSA_FLAG_OVERLAPPED);
			if( socket_ == INVALID_SOCKET )
				throw Win32Exception("WSASocket");

			// °ó¶¨µ½IOCP
			io_.Bind(reinterpret_cast<HANDLE>(socket_));
		}
		
		void Socket::Shutdown(int shut)
		{
			if( !IsOpen() )
				return;

			::shutdown(socket_, shut);
		}
		void Socket::Close()
		{
			if( !IsOpen() )
				return;

			::closesocket(socket_);
			socket_ = INVALID_SOCKET;
		}
		
		void Socket::Cancel()
		{
			if( !IsOpen() )
				throw std::logic_error("Socket not Open");
			else
				SocketProvider::CancelIO(socket_);
		}

		void Socket::Bind(int family/* AF_INET*/, u_short uPort /* 0 */, const IPAddress &addr /* INADDR_ANY */)
		{
			if( !IsOpen() )
				throw std::logic_error("Socket not open");

			sockaddr_in addrIn = {0};
			addrIn.sin_family		= family;
			addrIn.sin_port			= ::htons(uPort);
			addrIn.sin_addr.s_addr	= ::htonl(addr.Address());

			if( SOCKET_ERROR == ::bind(socket_, (const SOCKADDR *)&addrIn, sizeof(SOCKADDR)) )
				throw Win32Exception("bind");
		}

		void Socket::Listen(int nMax)
		{
			if( !IsOpen() )
				throw std::logic_error("Socket not open");

			if( SOCKET_ERROR == ::listen(socket_, nMax) )
				throw Win32Exception("listen");
		}

		SocketPtr Socket::Accept()
		{
			if( !IsOpen() )
				throw std::logic_error("Socket not open");

			SOCKET sock = ::accept(socket_, 0, 0);
			if( sock == INVALID_SOCKET )
				throw Win32Exception("accept");

			SocketPtr remote(MakeSocket(io_, sock));
			return remote;
		}

		void Socket::Connect(int family, const IPAddress &addr, u_short uPort)
		{
			if( !IsOpen() )
				throw std::logic_error("Socket not open");

			SOCKADDR_IN serverAddr = {0};
			serverAddr.sin_family		= family;
			serverAddr.sin_addr.s_addr	= ::htonl(addr.Address());
			serverAddr.sin_port			= ::htons(uPort);

			if( SOCKET_ERROR == ::connect(socket_, reinterpret_cast<const sockaddr *>(&serverAddr), sizeof(SOCKADDR_IN)) )
				throw Win32Exception("connect");
		}

		void Socket::DisConnect(int shut, bool bReuseSocket/* = true*/)
		{
			if( !IsOpen() )
				throw std::logic_error("Socket not open");

			::shutdown(socket_, shut);		
			
			if( bReuseSocket )
			{
				ReuseAddr reuse;
				SetOption(reuse);
			}
			else
			{
				Close();
			}
		}
		
		size_t Socket::Read(char *buf, size_t size, DWORD flag)
		{
			if( !IsOpen() )
				throw std::logic_error("Socket not open");

			WSABUF wsabuf = {0};
			wsabuf.buf = buf;
			wsabuf.len = size;

			if( wsabuf.len == 0 )
				throw std::logic_error("Buffer allocate size is zero");

			DWORD dwSize = 0;

			if( 0 != ::WSARecv(socket_, &wsabuf, 1, &dwSize, &flag, 0, 0) )
				throw Win32Exception("WSARecv");

			return dwSize;
		}

		size_t Socket::Write(const char *buffer, size_t sz, DWORD flag)
		{
			if( !IsOpen() )
				throw std::logic_error("Socket not open");

			WSABUF wsabuf = {0};
			wsabuf.buf = const_cast<char *>(buffer);
			wsabuf.len = sz;

			if( wsabuf.len == 0 )
				throw std::logic_error("Buffer size is zero");

			DWORD dwSize = 0;
			if( 0 != ::WSASend(socket_, &wsabuf, 1, &dwSize, flag, 0, 0) )
				throw Win32Exception("WSASend");

			return dwSize;
		}


		size_t Socket::SendTo(const char *buffer, size_t sz,  const SOCKADDR_IN &addr, DWORD flag)
		{
			if( !IsOpen() )
				throw std::logic_error("Socket not open");

			WSABUF wsabuf = {0};
			wsabuf.buf = const_cast<char *>(buffer);
			wsabuf.len = sz;

			if( wsabuf.len == 0 )
				throw std::logic_error("Buffer size is zero");

			DWORD dwSize = 0;
			if( 0 != ::WSASendTo(socket_, &wsabuf, 1, &dwSize, flag, reinterpret_cast<const sockaddr *>(&addr), sizeof(addr), 0, 0) )
				throw Win32Exception("WSASendTo");

			return dwSize;
		}

		size_t Socket::RecvFrom(char *buffer, size_t size, SOCKADDR_IN &addr, DWORD flag)
		{
			if( !IsOpen() )
				throw std::logic_error("Socket not open");

			WSABUF wsabuf = {0};
			wsabuf.buf = buffer;
			wsabuf.len = size;

			if( wsabuf.len == 0 )
				throw std::logic_error("Buffer allocate size is zero");

			DWORD dwSize = 0;
			int addrLen = sizeof(addr);

			if( 0 != ::WSARecvFrom(socket_, &wsabuf, 1, &dwSize, &flag, reinterpret_cast<sockaddr *>(&addr), &addrLen, 0, 0) )
				throw Win32Exception("WSARecvFrom");

			return dwSize;
		}

	}

}