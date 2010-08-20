#ifndef __NETWORK_PROVIDER_HPP
#define __NETWORK_PROVIDER_HPP


#include "../Basic.hpp"
#include "../IOCP/Dispatcher.hpp"

#include "SockInit.hpp"



namespace async
{


	namespace network
	{
		using namespace IOCP;

		class Socket;
		typedef pointer<Socket> SocketPtr;


		class SocketProvider
		{
		private:
			Internal::SockInit<> sockInit_;	

		public:
			explicit SocketProvider(IODispatcher &);
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
			// �ṩΨһʵ��
			static SocketProvider &GetSingleton(IODispatcher &io)
			{
				static SocketProvider provider(io);
				return provider;
			}

			// ��ȡ��չAPI
			static void GetExtensionFuncPtr(const SocketPtr &sock, GUID *guid, LPVOID pFunc);
		};
	}

}



#endif