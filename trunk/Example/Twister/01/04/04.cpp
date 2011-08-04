// 04.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


#include "../../../../include/Network/TCP.hpp"

using namespace async;

class Server
{
	iocp::IODispatcher &io_;
	network::Tcp::Accpetor acceptor_;

public:
	explicit Server(iocp::IODispatcher &io, u_short port)
		: io_(io)
		, acceptor_(io, network::Tcp::V4(), port)
	{}


	void Start()
	{
		using namespace std::tr1::placeholders;

		acceptor_.AsyncAccept(0, std::tr1::bind(
			&Server::_OnAccept, this, _1, _2));
	}

	void _OnAccept(u_long err, const network::SocketPtr &newSock)
	{
		newSock->Shutdown(SD_BOTH);
	}
};


int _tmain(int argc, _TCHAR* argv[])
{
	iocp::IODispatcher io(1);
	Server svr(io, 1079);

	return 0;
}

