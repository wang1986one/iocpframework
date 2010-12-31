#include "stdafx.h"

#include "Socket.hpp"
#include "SocketOption.hpp"

#include "../IOCP/WinException.hpp"


namespace async
{


	namespace network
	{

		Socket::Socket(OverlappedDispatcher &io)
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

			// 绑定到IOCP
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


		// 异步调用
		
		const AsyncResultPtr &Socket::AsyncConnect(const AsyncResultPtr &result, const IPAddress &addr, u_short uPort)
		{
			result->AddRef();

			_BeginConnectImpl(result, addr, uPort);
			return result;
		}

		void Socket::EndConnect(const AsyncResultPtr &asyncResult)
		{
			asyncResult->EndAsync(socket_);
		}


		// AsyncDisconnect
		const AsyncResultPtr &Socket::AsyncDisconnect(const AsyncResultPtr &result, bool bReuseSocket)
		{
			result->AddRef();

			_BeginDisconnectImpl(result, bReuseSocket);

			return result;
		}

		void Socket::EndDisconnect(const AsyncResultPtr &asyncResult)
		{
			asyncResult->EndAsync(socket_);
		}


		// AsyncRead
		const AsyncResultPtr &Socket::AsyncRead(const AsyncResultPtr &result, char *buf, size_t size)
		{
			result->AddRef();

			_BeginReadImpl(result, buf, size);

			return result;
		}

		size_t Socket::EndRead(const AsyncResultPtr &asyncResult)
		{
			return asyncResult->EndAsync(socket_);;
		}


		// AsyncWrite
		const AsyncResultPtr &Socket::AsyncWrite(const AsyncResultPtr &result, const char *buf, size_t size)
		{
			result->AddRef();

			_BeginWriteImpl(result, buf, size);

			return result;
		}

		size_t Socket::EndWrite(const AsyncResultPtr &asyncResult)
		{
			return asyncResult->EndAsync(socket_);
		}


		// --------------------------------

		void Socket::_BeginConnectImpl(const AsyncResultPtr &result, const IPAddress &addr, u_short uPort)
		{
			sockaddr_in remoteAddr		= {0};
			remoteAddr.sin_family		= AF_INET;
			remoteAddr.sin_port			= ::htons(uPort);
			remoteAddr.sin_addr.s_addr	= ::htonl(addr.Address());

			sockaddr_in localAddr		= {0};
			localAddr.sin_family		= AF_INET;
			localAddr.sin_port			= 0;
			localAddr.sin_addr.s_addr	= 0;

			// 很变态，需要先bind
			::bind(socket_, reinterpret_cast<const sockaddr *>(&localAddr), sizeof(localAddr));

			if( !SocketProvider::GetSingleton(io_).ConnectEx(socket_, reinterpret_cast<SOCKADDR *>(&remoteAddr), sizeof(SOCKADDR), 0, 0, 0, result.Get()) 
				&& ::WSAGetLastError() != WSA_IO_PENDING )
			{
				result->Release();
				throw Win32Exception("ConnectionEx");
			}
		}

		void Socket::_BeginDisconnectImpl(const AsyncResultPtr &result, bool bReuseSocket/* = true*/)
		{
			DWORD dwFlags = bReuseSocket ? TF_REUSE_SOCKET : 0;

			if( !SocketProvider::GetSingleton(io_).DisconnectEx(socket_, result.Get(), dwFlags, 0) 
				&& ::WSAGetLastError() != WSA_IO_PENDING )
			{
				result->Release();
				throw Win32Exception("DisConnectionEx");
			}
		}

		void Socket::_BeginReadImpl(const AsyncResultPtr &result, char *buf, size_t size)
		{
			WSABUF wsabuf = {0};
			wsabuf.buf = buf;
			wsabuf.len = size;

			DWORD dwFlag = 0;
			DWORD dwSize = 0;
			
			if( 0 != ::WSARecv(socket_, &wsabuf, 1, &dwSize, &dwFlag, result.Get(), NULL)
				&& ::WSAGetLastError() != WSA_IO_PENDING )
			{
				result->Release();
				throw Win32Exception("WSARecv");
			}
		}

		void Socket::_BeginWriteImpl(const AsyncResultPtr &result, const char *buf, size_t size)
		{
			WSABUF wsabuf = {0};
			wsabuf.buf = const_cast<char *>(buf);
			wsabuf.len = size;

			DWORD dwFlag = 0;
			DWORD dwSize = 0;
			
			if( 0 != ::WSASend(socket_, &wsabuf, 1, &dwSize, dwFlag, result.Get(), NULL)
				&& ::WSAGetLastError() != WSA_IO_PENDING )
			{
				result->Release();
				throw Win32Exception("WSASend");
			}
		}

	}

}