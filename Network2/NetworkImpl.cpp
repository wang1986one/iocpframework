#include "stdafx.h"
#include "NetworkImpl.h"

#include <MSTcpIP.h>

//#include "Protocol.h"
#include "../include/network/SocketProvider.hpp"
#include "../include/Network/SocketOption.hpp"



	// alterable IO
	void WINAPI APCFunc(ULONG_PTR pParam)
	{
		// do nothing
	}





	NetworkImpl::NetworkImpl(OverlappedDispatcher &io)
		: io_(io)
		, acceptor_(new Socket(io))
	{
	}

	NetworkImpl::~NetworkImpl()
	{
		Stop();
	}


	void NetworkImpl::Register(const AcceptCallback &acceptCallback/* = NULL*/, const RecvCallback &recvCallback /*= NULL*/
		, const SendCallback &sendCallback/* = NULL*/, const DisconnectCallback &disconnectCallback /*= NULL*/
		, const ErrorCallback &errorCallback/* = NULL*/)
	{
		acceptCallback_ = acceptCallback;
		recvCallback_	= recvCallback;
		sendCallback_	= sendCallback;
		disconnectCallback_ = disconnectCallback;
		errorCallback_	= errorCallback;
	}

	void NetworkImpl::Start(u_short uPort)
	{
		try
		{
			// 将建监听socket
			if( *acceptor_ == INVALID_SOCKET )
				acceptor_->Open(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		
			acceptor_->Bind(AF_INET, uPort, INADDR_ANY);
			acceptor_->Listen(SOMAXCONN);
		} 
		catch(const std::exception &e)
		{
			if( errorCallback_ != NULL )
				errorCallback_(std::tr1::ref(acceptor_), std::tr1::ref(e));
		}

		// 创建Accept 线程
		acceptThread_.RegisterFunc(std::tr1::bind(&NetworkImpl::_ThreadAccept, this));
		acceptThread_.Start();
	}


	void NetworkImpl::Stop()
	{
		acceptor_->Close();

		// 可提醒IO，退出监听线程
		::QueueUserAPC(APCFunc, acceptThread_, NULL);
		acceptThread_.Stop();
	}


	void NetworkImpl::Send(const SocketPtr &socket, const SocketBufferPtr &buffer)
	{
		try 
		{
			static AsyncCallbackFunc callback = std::tr1::bind(&NetworkImpl::_OnSend, this, std::tr1::placeholders::_1, socket);
			socket->AsyncWrite(buffer, 0, buffer->size(), callback);
		}
		catch(const std::exception &e)
		{
			if( errorCallback_ != NULL )
				errorCallback_(std::tr1::ref(socket), std::tr1::ref(e));
		}
	}
	AsyncResultPtr NetworkImpl::Send(const SocketPtr &socket, const AsyncResultPtr &asyncResult, size_t bufLen)
	{
		try 
		{
			static AsyncCallbackFunc callback = std::tr1::bind(&NetworkImpl::_OnSend, this, std::tr1::placeholders::_1, socket);
			socket->AsyncWrite(asyncResult, 0, bufLen);
		}
		catch(const std::exception &e)
		{
			if( errorCallback_ != NULL )
				errorCallback_(std::tr1::ref(socket), std::tr1::ref(e));
		}

		return asyncResult;
	}


	void NetworkImpl::_OnAccept(const AsyncResultPtr &asyncResult)
	{
		try 
		{
			// 获取远程连接
			const SocketPtr &acceptSocket = acceptor_->EndAccept(asyncResult);

			// 获取提供的Buffer
			const SocketBufferPtr &addrBuffer = asyncResult->m_buffer;

			// 复制Listen socket属性
			async::network::UpdateAcceptContext context(*acceptor_);
			acceptSocket->SetOption(context);

			// 设置超时
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

			inKeepAlive.onoff = 1;
			inKeepAlive.keepalivetime = 30000;
			inKeepAlive.keepaliveinterval = 2000;	

			acceptSocket->IOControl(SIO_KEEPALIVE_VALS, &inKeepAlive, dwInLen, &outKeepAlive, dwOutLen);

			// 获取远程地址
			SOCKADDR_IN *pLocalAddr = NULL, *pRemoteAddr = NULL;
			int szLocalLen = 0, szRemoteLen = 0;

			SocketProvider::GetSingleton(io_).GetAcceptExSockaddrs(addrBuffer->data(), 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, 
				reinterpret_cast<SOCKADDR **>(&pLocalAddr), &szLocalLen, reinterpret_cast<SOCKADDR **>(&pRemoteAddr), &szRemoteLen);

			// 投递新的接受请求
			SocketBufferPtr buffer(new SocketBuffer);

			static AsyncCallbackFunc callback = std::tr1::bind(&NetworkImpl::_OnRecv, this, std::tr1::placeholders::_1, acceptSocket);
			asyncResult->reset(buffer, nothing, callback);

			// 回调
			if( acceptCallback_ != NULL )
				acceptCallback_(std::tr1::cref(acceptSocket), std::tr1::cref(addrBuffer), std::tr1::cref(*pRemoteAddr));

			acceptSocket->AsyncRead(asyncResult, 0, buffer->allocSize());
		} 
		catch(const std::exception &e) 
		{
			std::cerr << e.what() << std::endl;
		}
	}


	void NetworkImpl::_OnRecv(const AsyncResultPtr &asyncResult, const SocketPtr &socket)
	{
		try 
		{
			size_t size = socket->EndRead(asyncResult);
			const SocketBufferPtr &buffer = asyncResult->m_buffer;
			
			if( size != 0 )
			{
				buffer->resize(size);
				
				// 回调
				if( recvCallback_ != NULL )
					recvCallback_(std::tr1::cref(socket), std::tr1::cref(buffer));

				static AsyncCallbackFunc callback = std::tr1::bind(&NetworkImpl::_OnRecv, this, std::tr1::placeholders::_1, socket);
				asyncResult->m_callback = callback;
				socket->AsyncRead(asyncResult, 0, buffer->allocSize());
			}
			else
			{
				if( disconnectCallback_ != NULL )
					disconnectCallback_(std::tr1::ref(socket));

				asyncResult->m_callback = std::tr1::bind(&NetworkImpl::_OnDisConnect, this, std::tr1::placeholders::_1, socket);
				socket->AsyncDisconnect(asyncResult, true);
			}
		}
		catch(const std::exception &e) 
		{
			if( errorCallback_ != NULL )
				errorCallback_(std::tr1::ref(socket), std::tr1::ref(e));
		}
	}

	void NetworkImpl::_OnSend(const AsyncResultPtr &asyncResult, const SocketPtr &socket)
	{
		try 
		{
			const SocketBufferPtr &buffer = asyncResult->m_buffer;
			size_t size = socket->EndWrite(asyncResult);
			if( size != 0 )
			{
				// 回调
				if( sendCallback_ != NULL )
					sendCallback_(std::tr1::cref(socket), std::tr1::cref(buffer), size);
			}
			else 
			{
				if( disconnectCallback_ != NULL )
					disconnectCallback_(std::tr1::ref(socket));

				asyncResult->m_callback = std::tr1::bind(&NetworkImpl::_OnDisConnect, this, std::tr1::placeholders::_1, socket);
				socket->AsyncDisconnect(asyncResult);
			}
		}
		catch(const std::exception &e) 
		{
			if( errorCallback_ != NULL )
				errorCallback_(std::tr1::ref(socket), std::tr1::ref(e));
		}
	}

	void NetworkImpl::_OnDisConnect(const AsyncResultPtr &asyncResult, const SocketPtr &socket)
	{
		socket->Shutdown(SD_BOTH);
		socket->Close();
	}


	DWORD NetworkImpl::_ThreadAccept()
	{
		// 通过使用WSAEventSelect来判断是否有足够的AcceptEx，或者检测出一个非正常的客户请求
		HANDLE hAcceptEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
		::WSAEventSelect(acceptor_->GetHandle(), hAcceptEvent, FD_ACCEPT);

		while(!acceptThread_.IsAborted())
		{	
			DWORD dwError = ::WaitForSingleObjectEx(hAcceptEvent, INFINITE, TRUE);
			if( dwError == WAIT_FAILED || dwError == WAIT_IO_COMPLETION )
				break;
			else if( dwError == WAIT_OBJECT_0 )
			{
				// 投递接收连接
				for(int i = 0; i != MAX_ACCEPT; ++i)
				{
					static AsyncCallbackFunc callback = std::tr1::bind(&NetworkImpl::_OnAccept, this, std::tr1::placeholders::_1);
					
					SocketPtr sock(new Socket(io_, AF_INET, SOCK_STREAM, IPPROTO_TCP));
					acceptor_->AsyncAccept(sock, 0, callback);
				}
			}
		}

		::CloseHandle(hAcceptEvent);

		return 0;
	}