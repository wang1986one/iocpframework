#ifndef __NETWORK_SOCKET_HPP
#define __NETWORK_SOCKET_HPP



#include "../IOCP/Dispatcher.hpp"
#include "../IOCP/Buffer.hpp"

#include "IPAddress.hpp"



namespace async
{


	namespace network
	{
		using namespace IOCP;

		// forward declare
		class SocketProvider;

		class Socket;
		typedef pointer<Socket> SocketPtr;



		// class SocketBuffer
		typedef BufferT<char, MemoryMgr::DEFAULT_SOCKET_SIZE, async::memory::MemAllocator<char, MemoryMgr::SocketMemoryPool> > SocketBuffer;
		//typedef BufferT<char, MemoryMgr::DEFAULT_SOCKET_SIZE, std::allocator<char>> SocketBuffer;
		typedef pointer<SocketBuffer> SocketBufferPtr;


		//----------------------------------------------------------------------
		// class Socket

		class Socket
			: public Object
		{
		private:
			SOCKET socket_;

			// IO服务
			IODispatcher &io_;

		public:
			explicit Socket(IODispatcher &, int nType = SOCK_STREAM, int nProtocol = IPPROTO_TCP);
			virtual ~Socket();

			// non-copyable
		private:
			Socket(const Socket &);
			Socket &operator=(const Socket &);

			
		public:
			// explicit转换
			operator SOCKET() 
			{
				return socket_;
			}
			operator SOCKET() const
			{
				return socket_;
			}

			// 显示获取
			SOCKET GetHandle() 
			{
				return socket_;
			}
			const SOCKET GetHandle() const
			{
				return socket_;
			}


			// 不需设置回调接口
		public:
			void IOControl(DWORD dwIOControl, const SocketBufferPtr &inBuf, const SocketBufferPtr &outBuf);
			
			template<typename SocketOptionT>
			bool SetOption(const SocketOptionT &option);

			template<typename SocketOptionT>
			bool GetOption(SocketOptionT &option) const;

			void Create(int nType = SOCK_STREAM, int nProtocol = IPPROTO_TCP);
			void Close();

			bool IsOpen();

			void Bind(u_short uPort = 0, const IPAddress &addr = INADDR_ANY);
			void Listen(int nMax = SOMAXCONN);

			
			SocketPtr Accept();
			void Connect(const IPAddress &addr, u_short uPort);
			void DisConnect(bool bReuseSocket = false);

			size_t Recv(const SocketBufferPtr &buf);
			size_t Send(const SocketBufferPtr &buf);

			// 异步调用接口
		public:
			AsyncResultPtr BeginAccept(AsyncCallbackFunc callback = NULL, const ObjectPtr &asyncState = nothing);
			SocketPtr EndAccept(const AsyncResultPtr &asynResult);

			AsyncResultPtr BeginConnect(const IPAddress &addr, u_short uPort, AsyncCallbackFunc callback = NULL, const ObjectPtr &asyncState = nothing);
			void EndConnect(const AsyncResultPtr &asyncResult);

			AsyncResultPtr BeginDisconnect(bool bReuseSocket = true, AsyncCallbackFunc callback = NULL, const ObjectPtr &asyncState = nothing);
			void EndDisconnect(const AsyncResultPtr &asyncResult);

			AsyncResultPtr BeginRecv(const SocketBufferPtr &buf, size_t nOffset, size_t nSize, AsyncCallbackFunc callback = NULL, const ObjectPtr &asyncState = nothing, const ObjectPtr &internalState = nothing);
			size_t EndRecv(const AsyncResultPtr &asyncResult);

			AsyncResultPtr BeginSend(const SocketBufferPtr &buf,size_t nOffset, size_t nSize, AsyncCallbackFunc callback = NULL, const ObjectPtr &asyncState = nothing, const ObjectPtr &internalState = nothing);
			size_t EndSend(const AsyncResultPtr &asyncResult);

		private:
			size_t _EndAsyncOperation(const AsyncResultPtr &asyncResult);
		};


		// ---------------------------

		inline bool Socket::IsOpen()
		{
			return socket_ != INVALID_SOCKET;
		}

		template<typename SocketOptionT>
		bool Socket::SetOption(const SocketOptionT &option)
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
		bool Socket::GetOption(SocketOptionT &option) const
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