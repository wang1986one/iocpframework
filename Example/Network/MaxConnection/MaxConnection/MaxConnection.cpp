// MaxConnection.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "../../../../include/Network/TCP.hpp"

#include <atlbase.h>
#include <atlconv.h>
#include <ctime>

class Server
{
	enum { MAX_CONNECTION = 1024 };

	async::iocp::IODispatcher &io_;
	async::network::Tcp::Accpetor acceptor_;

	size_t connectCnt_;
	char buf_[1024];

public:
	Server(async::iocp::IODispatcher &io, u_short port)
		: io_(io)
		, acceptor_(io, async::network::Tcp::V4(), port)
		, connectCnt_(0)
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
			if( connectCnt_ > MAX_CONNECTION )
				newSock->Shutdown(SD_BOTH);
			else
			{
				++connectCnt_;
				newSock->AsyncRead(buf_, 1024, std::tr1::bind(&Server::OnRead, this, async::iocp::_Error, async::iocp::_Size));
				_Start();
			}
		}
		catch(std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	void OnRead(u_long err, u_long size)
	{
		if( err != 0 || size == 0 )
			--connectCnt_;
	}
};


int _tmain(int argc, _TCHAR* argv[])
{
	async::iocp::IODispatcher io;
	Server svr(io, 5050);

	system("pause");
	return 0;
}

