// Discard.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "../../../../../include/Network/TCP.hpp"
#include "../../../../../include/IOCP/Dispatcher.hpp"
#include "../../../../../include/Timer/Timer.hpp"

#include <atlbase.h>
#include <atltime.h>
#include <iostream>

using namespace async;
using namespace iocp;
using namespace network;
using namespace timer;

typedef unsigned long long ulonglong;



ulonglong transffered_;
ulonglong recevied_;

ulonglong oldCounter_;
CTime startTime_;
volatile long connectedCnt_;


class Session
	: public std::tr1::enable_shared_from_this<Session>
{
private:
	Tcp::Socket socket_;
	std::vector<char> buf_;

	async::iocp::CallbackType readCallback_;
	async::iocp::CallbackType writeCallback_;

public:
	explicit Session(const SocketPtr &sock, size_t bufLen)
		: socket_(sock)
	{
		buf_.resize(bufLen);
		::InterlockedIncrement(&connectedCnt_);
	}
	~Session()
	{
		::InterlockedDecrement(&connectedCnt_);
	}


public:
	Tcp::Socket& GetSocket()
	{
		return socket_;
	}

	void Start()
	{
		try
		{		
			readCallback_	= std::tr1::bind(&Session::_HandleRead, shared_from_this(), _Size, _Error);
			writeCallback_	= std::tr1::bind(&Session::_HandleWrite, shared_from_this(), _Size, _Error);

			AsyncRead(socket_, Buffer(buf_), TransferAtLeast(1), readCallback_);

		}
		catch(std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	void Stop()
	{
		socket_.Close();

		readCallback_	= 0;
		writeCallback_	= 0;
	}

private:
	void _HandleRead(u_long bytes, u_long error)
	{
		try
		{
			if( bytes == 0 )
			{
				socket_.AsyncDisconnect(std::tr1::bind(&Session::_DisConnect, shared_from_this()));
				return;
			}

			recevied_ += bytes;
			transffered_ += bytes;

			AsyncWrite(socket_, Buffer(&buf_[0], bytes), TransferAll(), writeCallback_);
		}
		catch(const std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	void _HandleWrite(u_long bytes, u_long error)
	{
		try
		{		
			AsyncRead(socket_, Buffer(buf_), TransferAtLeast(1), readCallback_);
		}
		catch(std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	void _DisConnect()
	{
		Stop();
	}
};

typedef std::tr1::shared_ptr<Session> SessionPtr;




// 定制自己的工厂
namespace async
{
	namespace iocp
	{
		template<>
		struct ObjectFactory< Session >
		{
			typedef async::memory::FixedMemoryPool<true, sizeof(Session)>	PoolType;
			typedef ObjectPool< PoolType >									ObjectPoolType;
		};
	}
}


inline SessionPtr CreateSession(const SocketPtr &socket, size_t bufLen)
{
	return SessionPtr(ObjectAllocate<Session>(socket, bufLen), &ObjectDeallocate<Session>);
}


class DiscadSvr
{
	IODispatcher &io_;
	Tcp::Accpetor acceptor_;
	Timer timer_;
	size_t bufLen_;
	
public:
	DiscadSvr(IODispatcher &io, u_short port, size_t size)
		: io_(io)
		, acceptor_(io, Tcp::V4(), port)
		, timer_(io)
		, bufLen_(size)
	{}

	void Start()
	{
		using namespace std::tr1::placeholders;

		try
		{
			acceptor_.AsyncAccept(0, std::tr1::bind(&DiscadSvr::_OnAccept, this, _1, _2));
			timer_.AsyncWait(std::tr1::bind(&DiscadSvr::_OnTimer, this), 3000, 0);
		}
		catch(std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

private:
	void _OnTimer()
	{
		CTime endTime = CTime::GetCurrentTime();
		ulonglong bytes = transffered_ - oldCounter_;
		ulonglong msgs = recevied_;
		recevied_ = 0;

		CTimeSpan span = endTime - startTime_;
		printf("%4.3f MiB/s %4.3f Ki Msgs/s %6.2f bytes per msg\t connected count: %ld\n",
			static_cast<double>(bytes)/span.GetTimeSpan()/1024/1024,
			static_cast<double>(msgs)/span.GetTimeSpan()/1024,
			static_cast<double>(bytes)/static_cast<double>(msgs),
			connectedCnt_);

		oldCounter_ = transffered_;
		startTime_ = endTime;
	}

	void _OnAccept(u_long error, const SocketPtr &acceptSocket)
	{
		if( error != 0 )
			return;

		using namespace std::tr1::placeholders;
		try
		{
			SessionPtr session(CreateSession(acceptSocket, bufLen_));
			session->Start();

			acceptor_.AsyncAccept(0, std::tr1::bind(&DiscadSvr::_OnAccept, this, _1, _2));
		}
		catch(const std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	IODispatcher io;
	size_t msgSize = 512;
	if( argc > 1 )
		msgSize = _tstoi(argv[1]);
	DiscadSvr svr(io, 5050, msgSize);

	svr.Start();

	system("pause");
	return 0;
}

