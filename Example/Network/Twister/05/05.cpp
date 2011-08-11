// 05.cpp : 定义控制台应用程序的入口点。
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
		if( err != 0 )
			return;

		char msg[] = "No User";
		network::ConstBuffer buf(msg, _countof(msg));
		newSock->AsyncWrite(buf.data(), buf.size(), std::tr1::bind(
			&Server::_OnWrite, this, iocp::_Error));
	}

	void _OnWrite(u_long err)
	{
		assert(err == 0);
	}
};


int _tmain(int argc, _TCHAR* argv[])
{
	iocp::IODispatcher io(1);
	Server svr(io, 1079);


	svr.Start();
	system("pause");
	return 0;
}

