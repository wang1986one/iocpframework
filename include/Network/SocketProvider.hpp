#ifndef __NETWORK_PROVIDER_HPP
#define __NETWORK_PROVIDER_HPP


#include "../Basic.hpp"
#include "../IOCP/Dispatcher.hpp"

#include "SockInit.hpp"



namespace async
{


	namespace network
	{
		using namespace iocp;

		class Socket;
		typedef pointer<Socket> SocketPtr;


		class SocketProvider
		{
		private:
			Internal::SockInit<> sockInit_;	

		public:
			explicit SocketProvider(OverlappedDispatcher &);
			~SocketProvider();

			// non-copyable
		private:
			SocketProvider(const SocketProvider &);
			SocketProvider &operator=(const SocketProvider &);

		public:
			LPFN_TRANSMITFILE			TransmitFile;
			LPFN_ACCEPTEX				AcceptEx;
			LPFN_GETACCEPTEXSOCKADDRS	GetAcceptExSockaddrs;
			LPFN_TRANSMITPACKETS		TransmitPackets;
			LPFN_CONNECTEX				ConnectEx;
			LPFN_DISCONNECTEX			DisconnectEx;
			LPFN_WSARECVMSG				WSARecvMsg;
			

		public:
			// 提供唯一实例
			template<typename DispatcherT>
			static SocketProvider &GetSingleton(DispatcherT &io)
			{
				static SocketProvider provider(io);
				return provider;
			}

			// 获取扩展API
			static void GetExtensionFuncPtr(const SocketPtr &sock, GUID *guid, LPVOID pFunc);

			static void CancelIO(SOCKET sock);
		};
	}

}



#endif