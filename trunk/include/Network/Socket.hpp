#ifndef __NETWORK_SOCKET_HPP
#define __NETWORK_SOCKET_HPP



#include "../IOCP/Dispatcher.hpp"
#include "../IOCP/Buffer.hpp"

#include "IPAddress.hpp"



namespace async
{


	namespace network
	{
		using namespace iocp;

		// forward declare

		class Socket;
		typedef pointer<Socket> SocketPtr;



		// class SocketBuffer
		typedef BufferT<char, MemoryMgr::DEFAULT_SOCKET_SIZE, async::memory::MemAllocator<char, MemoryMgr::SocketMemoryPool> > SocketBuffer;
		typedef pointer<SocketBuffer> SocketBufferPtr;


		//----------------------------------------------------------------------
		// class Socket

		class Socket
			: public Object
		{
		public:
			typedef OverlappedDispatcher	AsyncIODispatcherType;

		private:
			// socket handle
			SOCKET socket_;

			// IO服务
			AsyncIODispatcherType &io_;

		public:
			explicit Socket(AsyncIODispatcherType &);
			Socket(AsyncIODispatcherType &, int nType, int nProtocol);
			~Socket();

			// non-copyable
		private:
			Socket(const Socket &);
			Socket &operator=(const Socket &);

		public:
			// explicit转换
			operator SOCKET()				{ return socket_; }
			operator const SOCKET() const	{ return socket_; }

			// 显示获取
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
			void Open(int nType, int nProtocol);
			// shutdown
			void Shutdown(int shut);
			// closesocket
			void Close();
		
			bool IsOpen() const;
			// CancelIO/CancelIOEx
			void Cancel();

			// bind
			void Bind(u_short family, u_short uPort, const IPAddress &addr);
			// listen
			void Listen(int nMax);

			
			// 不需设置回调接口
		public:
			SocketPtr Accept();
			void Connect(const IPAddress &addr, u_short uPort);
			void DisConnect(bool bReuseSocket = true);

			size_t Recv(const SocketBufferPtr &buf, DWORD flag);
			size_t Send(const SocketBufferPtr &buf, DWORD flag);

			// 异步调用接口
		public:
			// szOutSize指定额外的缓冲区大小，以用来Accept远程连接后且收到第一块数据包才返回
			AsyncResultPtr BeginAccept(size_t szOutSize = 0, const AsyncCallbackFunc &callback = NULL, const ObjectPtr &asyncState = nothing);
			SocketPtr EndAccept(const AsyncResultPtr &asynResult);

			// 异步连接需要先绑定端口
			AsyncResultPtr BeginConnect(const IPAddress &addr, u_short uPort, const AsyncCallbackFunc &callback = NULL, const ObjectPtr &asyncState = nothing);
			const AsyncResultPtr &BeginConnect(const AsyncResultPtr &result, const IPAddress &addr, u_short uPort);
			void EndConnect(const AsyncResultPtr &asyncResult);

			AsyncResultPtr BeginDisconnect(bool bReuseSocket = true, const AsyncCallbackFunc &callback = NULL, const ObjectPtr &asyncState = nothing);
			const AsyncResultPtr &BeginDisconnect(const AsyncResultPtr &result, bool bReuseSocket = true);
			void EndDisconnect(const AsyncResultPtr &asyncResult);

			AsyncResultPtr BeginRecv(const SocketBufferPtr &buf, size_t offset, size_t size, const AsyncCallbackFunc &callback = NULL, const ObjectPtr &asyncState = nothing, const ObjectPtr &internalState = nothing);
			const AsyncResultPtr &BeginRecv(const AsyncResultPtr &result, size_t offset, size_t size);
			size_t EndRecv(const AsyncResultPtr &asyncResult);

			AsyncResultPtr BeginSend(const SocketBufferPtr &buf,size_t offset, size_t size, const AsyncCallbackFunc &callback = NULL, const ObjectPtr &asyncState = nothing, const ObjectPtr &internalState = nothing);
			const AsyncResultPtr &BeginSend(const AsyncResultPtr &result, size_t offset, size_t size);
			size_t EndSend(const AsyncResultPtr &asyncResult);

			
		private:
			void _BeginConnectImpl(const AsyncResultPtr &result, const IPAddress &addr, u_short uPort);
			void _BeginDisconnectImpl(const AsyncResultPtr &result, bool bReuseSocket);
			void _BeginRecvImpl(const AsyncResultPtr &result, size_t offset, size_t size);
			void _BeginSendImpl(const AsyncResultPtr &result, size_t offset, size_t size);
			
			
		};


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

	}


}



#endif