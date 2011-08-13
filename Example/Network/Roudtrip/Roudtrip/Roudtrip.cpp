// Roudtrip.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


#include "../../../../include/Network/TCP.hpp"

#include <atlbase.h>
#include <atlconv.h>
#include <ctime>

class Server
{
	async::iocp::IODispatcher &io_;
	async::network::Tcp::Accpetor acceptor_;

	char buf_[1024];

public:
	Server(async::iocp::IODispatcher &io, u_short port)
		: io_(io)
		, acceptor_(io, async::network::Tcp::V4(), port)
	{
		_Start();
	}

	void _Start()
	{
		using namespace std::tr1::placeholders;

		acceptor_.AsyncAccept(0, std::tr1::bind(
			&Server::OnAccept, this, _1, _2));
	}

	void OnAccept(u_long err, const async::network::SocketPtr &newSock)
	{
		if( err != 0 )
			return;
	
		try
		{
			newSock->SetOption(async::network::NoDelay(true));
			newSock->AsyncRead(buf_, 1024, std::tr1::bind(&Server::OnRead, this, newSock, async::iocp::_Error,  async::iocp::_Size));
		}
		catch(std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	void OnRead(async::network::SocketPtr newSock, u_long err, u_long size)
	{
		if( err != 0 )
			return;

		time_t curTime = 0;
		::time(&curTime);
		
		memcpy(buf_ + size, &curTime, sizeof(curTime));

		try
		{
			newSock->AsyncWrite(buf_, size + sizeof(curTime), 0);
			newSock->AsyncRead(buf_, 1024, std::tr1::bind(&Server::OnRead, this, newSock, async::iocp::_Error,  async::iocp::_Size));
		}
		catch(std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
};



class Client
{
	async::iocp::IODispatcher &io_;
	async::network::Tcp::Socket sock_;

	char buf_[1024];

public:
	Client(async::iocp::IODispatcher &io, const std::string &ip, u_short port)
		: io_(io)
		, sock_(io, async::network::Tcp::V4())
	{
		_Start(ip, port);
	}

	void _Start(const std::string &ip, u_short port)
	{
		using namespace std::tr1::placeholders;

		sock_.SetOption(async::network::NoDelay());
		sock_.AsyncConnect(async::network::IPAddress::Parse(ip), port,
			std::tr1::bind(&Client::OnConnect, this, async::iocp::_Error));
	}

	void OnConnect(u_long err)
	{
		if( err != 0 )
			return;

		_Send();
	}

	void OnRead(u_long err, u_long size)
	{
		if( err != 0 )
			return;

		time_t start = 00, stop = 0;
		memcpy(&start, buf_, sizeof(start));
		memcpy(&stop, buf_ + sizeof(start), sizeof(stop));

		std::cout << "RTT: " << stop - start << std::endl;

		_Send();
	}

	void _Send()
	{
		time_t curTime = 0;
		::time(&curTime);

		memcpy(buf_, &curTime, sizeof(curTime));

		try
		{
			sock_.AsyncWrite(async::network::Buffer(buf_, sizeof(curTime)), 0);
			sock_.AsyncRead(async::network::Buffer(buf_), std::tr1::bind(&Client::OnRead, this, async::iocp::_Error, async::iocp::_Size));
		}
		catch(std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	async::iocp::IODispatcher io(2);

	if( argc > 2 )
	{
		u_short port = static_cast<u_short>(_tstoi(argv[2]));
		if( _tcscmp(argv[1], _T("s")) == 0 )
		{
			Server svr(io, port);
			system("pause");
		}
		else
		{
			Client cli(io, (LPCSTR)CT2A(argv[1]), port);
			system("pause");
		}
	}
	else
	{
		printf("Usage:\n%s -s port\n%s ip port\n", argv[0], argv[0]);
		system("pause");
	}


	return 0;
}

