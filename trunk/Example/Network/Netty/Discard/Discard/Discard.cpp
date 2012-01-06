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

class DiscadSvr
{
	IODispatcher &io_;
	Tcp::Accpetor acceptor_;
	Timer timer_;


	ulonglong transffered_;
	ulonglong recevied_;

	ulonglong oldCounter_;
	CTime startTime_;

	std::vector<char> buffer_;

public:
	DiscadSvr(IODispatcher &io, u_short port, size_t size)
		: io_(io)
		, acceptor_(io, Tcp::V4(), port)
		, timer_(io)
		, transffered_(0)
		, recevied_(0)
		, oldCounter_(0)
	{
		buffer_.resize(size);
	}

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
		printf("%4.3f MiB/s %4.3f Ki Msgs/s %6.2f bytes per msg\n",
			static_cast<double>(bytes)/span.GetTimeSpan()/1024/1024,
			static_cast<double>(msgs)/span.GetTimeSpan()/1024,
			static_cast<double>(bytes)/static_cast<double>(msgs));

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
			std::tr1::shared_ptr<Tcp::Socket> sock(new Tcp::Socket(acceptSocket));
			//sock->SetOption(RecvBufSize(512));
			//sock->SetOption(SendBufSize(512));
			sock->AsyncRead(Buffer(buffer_), std::tr1::bind(&DiscadSvr::_OnRead, this, _Size, _Error, sock));

			acceptor_.AsyncAccept(0, std::tr1::bind(&DiscadSvr::_OnAccept, this, _1, _2));
		}
		catch(const std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	void _OnRead(u_long size, u_long error, const std::tr1::shared_ptr<Tcp::Socket> &sock)
	{
		if( error != 0 )
			return;

		recevied_ += size;
		transffered_ += size;

		try
		{
			AsyncWrite(*sock, Buffer(buffer_, size), TransferAll(),
				std::tr1::bind(&DiscadSvr::_OnWrite, this, _Error, sock));
		}
		catch(std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}

	}

	void _OnWrite(u_long err, const std::tr1::shared_ptr<Tcp::Socket> &sock)
	{
		if( err != 0 )
			return;

		try
		{
			AsyncRead(*sock, Buffer(buffer_), TransferAtLeast(1),
				std::tr1::bind(&DiscadSvr::_OnRead, this, _Size, _Error, sock));
		}
		catch(std::exception &e)
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

