#include "stdafx.h"
#include "NetworkImpl.h"

#include <MSTcpIP.h>

//#include "Protocol.h"
#include"../include/network/SocketProvider.hpp"




// alterable IO
void WINAPI APCFunc(ULONG_PTR pParam)
{
	// do nothing
}


NetworkImpl::NetworkImpl(IODispatcher &io)
: io_(io)
, acceptor_(new Socket(io))
{
}

NetworkImpl::~NetworkImpl()
{
	Stop();
}

void NetworkImpl::Start(u_short uPort)
{
	try
	{
		// ��������socket
		if( *acceptor_ == INVALID_SOCKET )
			acceptor_->Create();
	
		acceptor_->Bind(uPort);
		acceptor_->Listen();

		// ����Accept �߳�
		acceptThread_.RegisterFunc(std::tr1::bind(&NetworkImpl::_ThreadAccept, this));
		acceptThread_.Start();
	} 
	catch(const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}


void NetworkImpl::Stop()
{
	acceptor_->Close();

	// ������IO���˳������߳�
	::QueueUserAPC(APCFunc, acceptThread_, NULL);
	acceptThread_.Stop();
}



void NetworkImpl::_OnAccept(const AsyncResultPtr &asyncResult)
{
	try 
	{
		// ��ȡԶ������
		const SocketPtr &listenSocket = asyncResult->m_asynState;
		const SocketPtr &acceptSocket = listenSocket->EndAccept(asyncResult);
		
		// ��ȡ�ṩ��Buffer
		const SocketBufferPtr &addrBuffer = asyncResult->m_buffer;

		// ���ó�ʱ
		struct TCPKeepAlive
		{
			u_long onoff;
			u_long keepalivetime;
			u_long keepaliveinterval;				
		};

		TCPKeepAlive inKeepAlive = {0};
		DWORD dwInLen = sizeof(TCPKeepAlive);
		TCPKeepAlive outKeepAlive = {0};
		DWORD dwOutLen = sizeof(TCPKeepAlive);
		DWORD dwBytesReturn = 0;
		inKeepAlive.onoff = 1;
		inKeepAlive.keepalivetime = 30000;
		inKeepAlive.keepaliveinterval = 2000;	
		::WSAIoctl(acceptSocket->GetHandle(), SIO_KEEPALIVE_VALS, &inKeepAlive, dwInLen, 
			&outKeepAlive, dwOutLen, &dwBytesReturn, NULL, NULL);


		// ��ȡԶ�̵�ַ
		SOCKADDR_IN *pLocalAddr = NULL, *pRemoteAddr = NULL;
		int szLocalLen = 0, szRemoteLen = 0;

		SocketProvider::GetSingleton(io_).GetAcceptExSockaddrs(addrBuffer->data(), 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, 
			reinterpret_cast<SOCKADDR **>(&pLocalAddr), &szLocalLen, reinterpret_cast<SOCKADDR **>(&pRemoteAddr), &szRemoteLen);


		// Ͷ���µĽ�������
		SocketBufferPtr buffer(new SocketBuffer);
		static AsyncCallbackFunc callback = std::tr1::bind(&NetworkImpl::_OnRecv, this, std::tr1::placeholders::_1);
		asyncResult->reset(acceptSocket, buffer, acceptSocket, nothing, callback);

		acceptSocket->BeginRecv(asyncResult, 0, buffer->allocSize());
	} 
	catch(const std::exception &e) 
	{
		std::cerr << e.what() << std::endl;
	}

}


void NetworkImpl::_OnRecv(const AsyncResultPtr &asyncResult)
{
	const SocketPtr &sock = asyncResult->m_asynState;

	try 
	{
		size_t size = sock->EndRecv(asyncResult);
		const SocketBufferPtr &buffer = asyncResult->m_buffer;

		buffer->resize(size);

		if( size == 0 )
		{
			return ;
		}

		sock->BeginRecv(asyncResult, 0, buffer->allocSize());

		// �����ѽ��յ�����
		//static AsyncCallbackFunc callback = std::tr1::bind(&NetworkImpl::_OnSend, this, std::tr1::placeholders::_1);
		//asyncResult->m_callback = callback;
		//sock->BeginSend(asyncResult, 0, size);

	}
	catch(const std::exception &e) 
	{
		std::cerr << e.what() << std::endl;
	}
}

void NetworkImpl::_OnSend(const AsyncResultPtr &asyncResult)
{
	const SocketPtr &socket = asyncResult->m_asynState;

	try 
	{
		const SocketBufferPtr &buffer = asyncResult->m_buffer;
		size_t size = socket->EndSend(asyncResult);
		if( size != 0 )
		{
			static AsyncCallbackFunc callback = std::tr1::bind(&NetworkImpl::_OnRecv, this, std::tr1::placeholders::_1);
			//asyncResult->m_callback = callback;
			//socket->BeginRecv(asyncResult, 0, buffer->allocSize());
		}
		else 
		{
			return;
		}
	}
	catch(const std::exception &e) 
	{
		std::cerr << e.what() << std::endl;
	}
}


DWORD NetworkImpl::_ThreadAccept()
{
	// ͨ��ʹ��WSAEventSelect���ж��Ƿ����㹻��AcceptEx�����߼���һ���������Ŀͻ�����
	HANDLE hAcceptEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	::WSAEventSelect(acceptor_->GetHandle(), hAcceptEvent, FD_ACCEPT);

	while(!acceptThread_.IsAborted())
	{	
		DWORD dwError = ::WaitForSingleObjectEx(hAcceptEvent, INFINITE, TRUE);
		if( dwError == WAIT_FAILED || dwError == WAIT_IO_COMPLETION )
			break;
		else if( dwError == WAIT_OBJECT_0 )
		{
			// Ͷ�ݽ�������
			for(int i = 0; i != MAX_ACCEPT; ++i)
			{
				acceptor_->BeginAccept(0, std::tr1::bind(&NetworkImpl::_OnAccept, this, std::tr1::placeholders::_1), acceptor_);
			}

			static u_long connect = 0;

			connect += MAX_ACCEPT;
			std::cout << connect << std::endl;
		}
	}

	::CloseHandle(hAcceptEvent);

	return 0;
}