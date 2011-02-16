#ifndef __NETWORK_SOCKET_HPP
#define __NETWORK_SOCKET_HPP



#include "../IOCP/Dispatcher.hpp"
#include "../IOCP/Buffer.hpp"

#include "IPAddress.hpp"
#include "SocketProvider.hpp"


namespace async
{


	namespace network
	{
		// forward declare

		class Socket;
		typedef std::tr1::shared_ptr<Socket> SocketPtr;



		//----------------------------------------------------------------------
		// class Socket

		class Socket
		{
		public:
			typedef iocp::IODispatcher	AsyncIODispatcherType;

		private:
			// socket handle
			SOCKET socket_;

			// IO����
			AsyncIODispatcherType &io_;

		public:
			explicit Socket(AsyncIODispatcherType &);
			Socket(AsyncIODispatcherType &, SOCKET sock);
			Socket(AsyncIODispatcherType &, int family, int type, int protocol);
			~Socket();

			// non-copyable
		private:
			Socket(const Socket &);
			Socket &operator=(const Socket &);

		public:
			// explicitת��
			operator SOCKET()				{ return socket_; }
			operator const SOCKET() const	{ return socket_; }

			// ��ʾ��ȡ
			SOCKET GetHandle()				{ return socket_; }
			const SOCKET GetHandle() const	{ return socket_; }

		public:
			// WSAIoctl
			template<typename InT, typename OutT>
			DWORD IOControl(DWORD dwIOControl, InT *inData, DWORD inSize, OutT *outData, DWORD outSize);

			// setsocketopt
			template<typename SocketOptionT>
			bool SetOption(const SocketOptionT &option);

			// getsockopt
			template<typename SocketOptionT>
			bool GetOption(SocketOptionT &option) const;

			// WSASocket
			void Open(int family, int nType, int nProtocol);
			// shutdown
			void Shutdown(int shut);
			// closesocket
			void Close();

			bool IsOpen() const;
			// CancelIO/CancelIOEx
			void Cancel();

			// bind
			void Bind(int family, u_short uPort, const IPAddress &addr);
			// listen
			void Listen(int nMax);


			// �������ûص��ӿ�,ͬ������
		public:
			SocketPtr Accept();
			void Connect(int family, const IPAddress &addr, u_short uPort);
			void DisConnect(int shut, bool bReuseSocket = true);

			size_t Read(char *buf, size_t size, DWORD flag);
			size_t Write(const char *buffer, size_t size, DWORD flag);

			size_t SendTo(const char *buffer, size_t sz,  const SOCKADDR_IN &addr, DWORD flag);
			size_t RecvFrom(char *buffer, size_t sz, SOCKADDR_IN &addr, DWORD flag);

			// �첽���ýӿ�
		public:
			// szOutSizeָ������Ļ�������С��������AcceptԶ�����Ӻ����յ���һ�����ݰ��ŷ���
			template<typename HandlerT>
			void /*AsyncIOCallback<HandlerT> **/AsyncAccept(const SocketPtr &remoteSocket, size_t szOutSize, const HandlerT &callback);

			// �첽������Ҫ�Ȱ󶨶˿�
			template<typename HandlerT>
			AsyncIOCallback *AsyncConnect(const IPAddress &addr, u_short uPort, const HandlerT &callback);
			AsyncIOCallback &AsyncConnect(AsyncIOCallback &result, const IPAddress &addr, u_short uPort);

			// �첽�Ͽ�����
			template<typename HandlerT>
			AsyncIOCallback *AsyncDisconnect(const HandlerT &callback, bool bReuseSocket);
			AsyncIOCallback &AsyncDisconnect(AsyncIOCallback &result, bool bReuseSocket);

			// �첽TCP��ȡ
			template<typename HandlerT>
			AsyncIOCallback *AsyncRead(char *buf, size_t size, const HandlerT &callback);
			AsyncIOCallback &AsyncRead(AsyncIOCallback &result, char *buf, size_t size);

			// �첽TCPд��
			template<typename HandlerT>
			AsyncIOCallback *AsyncWrite(const char *buf, size_t size, const HandlerT &callback);
			AsyncIOCallback &AsyncWrite(AsyncIOCallback &result, const char *buf, size_t size);

			// �첽UDP��ȡ
			template<typename HandlerT>
			AsyncIOCallback *AsyncSendTo(const char *buf, size_t size, const SOCKADDR_IN &addr, const HandlerT &callback);

			// �첽UDP����
			template<typename HandlerT>
			AsyncIOCallback *AsyncRecvFrom(char *buf, size_t size, SOCKADDR_IN &addr, const HandlerT &callback);

		private:
			template<typename AsyncT>
			void _BeginConnectImpl(const AsyncT &result, const IPAddress &addr, u_short uPort);
			template<typename AsyncT>
			void _BeginDisconnectImpl(const AsyncT &result, bool bReuseSocket = true);
			template<typename AsyncT>
			void _BeginReadImpl(const AsyncT &result, char *buf, size_t size);
			template<typename AsyncT>		
			void _BeginWriteImpl(const AsyncT &result, const char *buf, size_t size);	
		};
	}
}

namespace async
{
	namespace iocp
	{
		typedef async::network::Socket Socket;

		// Socket ����
		template<>
		struct ObjectFactory< Socket >
		{
			typedef async::memory::FixedMemoryPool<true, sizeof(Socket)>	PoolType;
			typedef ObjectPool< PoolType >									ObjectPoolType;
		};
	}
}


#include "Accept.hpp"


namespace async
{
	namespace network
	{

		inline SocketPtr MakeSocket(Socket::AsyncIODispatcherType &io)
		{
			return SocketPtr(ObjectAlloc<Socket>(io), &ObjectDeallocate<Socket>);
		}

		inline SocketPtr MakeSocket(Socket::AsyncIODispatcherType &io, SOCKET sock)
		{
			return SocketPtr(ObjectAlloc<Socket>(io, sock), &ObjectDeallocate<Socket>);
		}

		inline SocketPtr MakeSocket(Socket::AsyncIODispatcherType &io, int family, int type, int protocol)
		{
			return SocketPtr(ObjectAlloc<Socket>(io, family, type, protocol), &ObjectDeallocate<Socket>);
		}


		// ---------------------------

		inline bool Socket::IsOpen() const
		{
			return socket_ != INVALID_SOCKET;
		}

		template<typename InT, typename OutT>
		inline DWORD Socket::IOControl(DWORD dwIOControl, InT *inData, DWORD inSize, OutT *outData, DWORD outSize)
		{
			DWORD dwRet = 0;

			if( 0 != ::WSAIoctl(socket_, dwIOControl, inData, inSize, outData, outSize, &dwRet, 0, 0) )
				throw Win32Exception("WSAIoCtl");

			return dwRet;
		}

		template<typename SocketOptionT>
		inline bool Socket::SetOption(const SocketOptionT &option)
		{
			if( !IsOpen() )
				return false;

			/*if( option.level() == SOL_SOCKET && option.name() == SO_LINGER )
			{
			const ::linger *lingerData = reinterpret_cast<const ::linger *>(option.data());
			if( lingerData->l_onoff != 0 && lingerData->l_linger != 0 )

			}*/

			return SOCKET_ERROR != ::setsockopt(socket_, option.level(), option.name(), option.data(), option.size());
		}

		template<typename SocketOptionT>
		inline bool Socket::GetOption(SocketOptionT &option) const
		{
			if( !IsOpen() )
				return false;

			size_t sz = option.size();
			if( SOCKET_ERROR != ::getsockopt(socket_, option.level(), option.name(), option.data(), sz) )
			{
				option.resize(sz);
				return true;
			}
			else
				return false;
		}

		// �첽����Զ������
		template<typename HandlerT>
		void Socket::AsyncAccept(const SocketPtr &remoteSocket, size_t szOutSize, const HandlerT &callback)
		{
			if( !IsOpen() ) 
				throw std::logic_error("Socket not open");

			if( !remoteSocket->IsOpen() )
				throw std::logic_error("Remote socket not open");

			typedef detail::AcceptorHandle<HandlerT> HookAcceptor;
			iocp::AutoBufferPtr acceptBuffer(MakeBuffer((sizeof(sockaddr_in) + 16) * 2 + szOutSize));
			AsyncIOCallback *asyncResult = MakeAsyncIOCallback(HookAcceptor(*this, remoteSocket, acceptBuffer, callback));
			asyncResult->AddRef();

			// ����szOutSide��С�жϣ��Ƿ���Ҫ����Զ�̿ͻ�����һ�����ݲŷ��ء�
			// ���Ϊ0�����������ء�������0����������ݺ��ٷ���
			DWORD dwRecvBytes = 0;
			if( !SocketProvider::GetSingleton(io_).AcceptEx(socket_, remoteSocket->socket_, acceptBuffer->data(), szOutSize,
				sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, &dwRecvBytes, asyncResult) 
				&& ::WSAGetLastError() != ERROR_IO_PENDING )
			{
				asyncResult->Release();

				throw Win32Exception("AcceptEx");
			}

			//return asyncResult;
		}

		// �첽���ӷ���
		template<typename HandlerT>
		AsyncIOCallback *Socket::AsyncConnect(const IPAddress &addr, u_short uPort, const HandlerT &callback)
		{
			if( !IsOpen() )
				throw std::logic_error("Socket not open");

			AsyncIOCallback *asynResult(MakeAsyncIOCallback(callback));
			asynResult->AddRef();

			_BeginConnectImpl(asynResult, addr, uPort);

			return asynResult;
		}

		// �첽�ر�����
		template<typename HandlerT>
		AsyncIOCallback *Socket::AsyncDisconnect(const HandlerT &callback, bool bReuseSocket/* = true*/)
		{
			AsyncIOCallback *asynResult(MakeAsyncIOCallback(callback));
			asynResult->AddRef();

			_BeginDisconnectImpl(asynResult, bReuseSocket);

			return asynResult;
		}

		// �첽�ӽ�������
		template<typename HandlerT>
		AsyncIOCallback *Socket::AsyncRead(char *buf, size_t size, const HandlerT &callback)
		{
			AsyncIOCallback *asynResult(MakeAsyncIOCallback(callback));
			asynResult->AddRef();

			_BeginReadImpl(asynResult, buf, size);

			return asynResult;
		}

		// �첽��������
		template<typename HandlerT>
		AsyncIOCallback *Socket::AsyncWrite(const char *buf, size_t size, const HandlerT &callback)
		{
			AsyncIOCallback *asynResult(MakeAsyncIOCallback(callback));
			asynResult->AddRef();

			_BeginWriteImpl(asynResult, buf, size);

			return asynResult;
		}

		// �첽UDPд��
		template<typename HandlerT>
		AsyncIOCallback *Socket::AsyncSendTo(const char *buf, size_t size, const SOCKADDR_IN &addr, const HandlerT &callback)
		{
			AsyncIOCallback *asynResult(MakeAsyncIOCallback(callback));
			asynResult->AddRef();

			WSABUF wsabuf = {0};
			wsabuf.buf = const_cast<char *>(buf);
			wsabuf.len = size;

			DWORD dwFlag = 0;
			DWORD dwSize = 0;

			if( 0 != ::WSASendTo(socket_, &wsabuf, 1, &dwSize, dwFlag, reinterpret_cast<const sockaddr *>(&addr), sizeof(addr), asynResult, NULL)
				&& ::WSAGetLastError() != WSA_IO_PENDING )
			{
				asynResult->Release();
				throw Win32Exception("WSASendTo");
			}

			return asynResult;
		}	

		// �첽UDP����
		template<typename HandlerT>
		AsyncIOCallback *Socket::AsyncRecvFrom(char *buf, size_t size, SOCKADDR_IN &addr, const HandlerT &callback)
		{
			AsyncIOCallback *asynResult(MakeAsyncIOCallback(callback));
			asynResult->AddRef();

			WSABUF wsabuf = {0};
			wsabuf.buf = buf;
			wsabuf.len = size;

			DWORD dwFlag = 0;
			DWORD dwSize = 0;
			int addrLen = sizeof(addr);

			if( 0 != ::WSARecvFrom(socket_, &wsabuf, 1, &dwSize, &dwFlag, reinterpret_cast<sockaddr *>(&addr), &addrLen, asynResult, NULL)
				&& ::WSAGetLastError() != WSA_IO_PENDING )
			{
				asynResult->Release();
				throw Win32Exception("WSARecvFrom");
			}

			return asynResult;
		}



		// --------------------------------
		template<typename AsyncT>
		void Socket::_BeginConnectImpl(const AsyncT &result, const IPAddress &addr, u_short uPort)
		{
			sockaddr_in remoteAddr		= {0};
			remoteAddr.sin_family		= AF_INET;
			remoteAddr.sin_port			= ::htons(uPort);
			remoteAddr.sin_addr.s_addr	= ::htonl(addr.Address());

			sockaddr_in localAddr		= {0};
			localAddr.sin_family		= AF_INET;

			// �ܱ�̬����Ҫ��bind
			::bind(socket_, reinterpret_cast<const sockaddr *>(&localAddr), sizeof(localAddr));

			if( !SocketProvider::GetSingleton(io_).ConnectEx(socket_, reinterpret_cast<SOCKADDR *>(&remoteAddr), sizeof(SOCKADDR), 0, 0, 0, result) 
				&& ::WSAGetLastError() != WSA_IO_PENDING )
			{
				result->Release();
				throw Win32Exception("ConnectionEx");
			}
		}

		template<typename AsyncT>
		void Socket::_BeginDisconnectImpl(const AsyncT &result, bool bReuseSocket/* = true*/)
		{
			DWORD dwFlags = bReuseSocket ? TF_REUSE_SOCKET : 0;

			if( !SocketProvider::GetSingleton(io_).DisconnectEx(socket_, result, dwFlags, 0) 
				&& ::WSAGetLastError() != WSA_IO_PENDING )
			{
				result->Release();
				throw Win32Exception("DisConnectionEx");
			}
		}

		template<typename AsyncT>
		void Socket::_BeginReadImpl(const AsyncT &result, char *buf, size_t size)
		{
			WSABUF wsabuf = {0};
			wsabuf.buf = buf;
			wsabuf.len = size;

			DWORD dwFlag = 0;
			DWORD dwSize = 0;

			if( 0 != ::WSARecv(socket_, &wsabuf, 1, &dwSize, &dwFlag, result, NULL)
				&& ::WSAGetLastError() != WSA_IO_PENDING )
			{
				result->Release();
				throw Win32Exception("WSARecv");
			}
		}

		template<typename AsyncT>
		void Socket::_BeginWriteImpl(const AsyncT &result, const char *buf, size_t size)
		{
			WSABUF wsabuf = {0};
			wsabuf.buf = const_cast<char *>(buf);
			wsabuf.len = size;

			DWORD dwFlag = 0;
			DWORD dwSize = 0;

			if( 0 != ::WSASend(socket_, &wsabuf, 1, &dwSize, dwFlag, result, NULL)
				&& ::WSAGetLastError() != WSA_IO_PENDING )
			{
				result->Release();
				throw Win32Exception("WSASend");
			}
		}
	}
}



#endif