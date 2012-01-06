// Discard.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "../../../../../include/Network/TCP.hpp"
#include "../../../../../include/IOCP/Dispatcher.hpp"

#include <atlbase.h>
#include <atlconv.h>
#include <iostream>

using namespace async;
using namespace iocp;
using namespace network;


typedef unsigned long long ulonglong;

class DiscadCli
{
	IODispatcher &io_;
	Tcp::Socket sock_;
	u_short port_;

	std::vector<char> buffer_;

public:
	DiscadCli(IODispatcher &io, u_short port, size_t size)
		: io_(io)
		, sock_(io, Tcp::V4())
		, port_(port)
	{
		buffer_.resize(size);
	}

	void Start(const std::string &ip)
	{
		using namespace std::tr1::placeholders;

		try
		{
			//sock_.SetOption(RecvBufSize(512));
			//sock_.SetOption(SendBufSize(512));
			sock_.AsyncConnect(IPAddress::Parse(ip), port_,
				std::tr1::bind(&DiscadCli::_OnConnect, this, _Error));
		}
		catch(std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

private:
	void _OnConnect(u_long error)
	{
		if( error != 0 )
			return;

		try
		{
			AsyncWrite(sock_, Buffer(buffer_), TransferAll(), 
				std::tr1::bind(&DiscadCli::_OnWrite, this, _Error));
		}
		catch(const std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	void _OnWrite(u_long err)
	{
		if( err != 0 )
			return;

		try
		{
			AsyncRead(sock_, Buffer(buffer_), TransferAtLeast(1), 
				std::tr1::bind(&DiscadCli::_OnRead, this, _Size, _Error));
		}
		catch(const std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	void _OnRead(u_long size, u_long err)
	{
		if( err != 0 )
			return;

		try
		{
			AsyncWrite(sock_, Buffer(buffer_), TransferAll(), 
				std::tr1::bind(&DiscadCli::_OnWrite, this, _Error));
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
	if( argc > 3 )
		msgSize = _tstoi(argv[3]);

	std::vector<DiscadCli *> clis;
	if( argc > 2 )
	{
		size_t cnt = _tstoi(argv[2]);

		clis.reserve(cnt);
		for(size_t i = 0; i != cnt; ++i)
			clis.push_back(new DiscadCli(io, 5050, msgSize));
	}
	else
		clis.push_back(new DiscadCli(io, 5050, msgSize));

	std::string ip;
	if( argc > 1 )
		ip = CW2A(argv[1]);
	else
		ip = "127.0.0.1";

	for(size_t i = 0; i != clis.size(); ++i)
		clis[i]->Start(ip.c_str());

	system("pause");
	return 0;
}

