// Udp.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../../../../include/Network/UDP.hpp"


using namespace async::iocp;
using namespace async::network;

using namespace std::tr1::placeholders;

class Server
{
private:
	async::iocp::IODispatcher& io_service_;
	async::network::Udp::Socket socket_;
	SOCKADDR_IN addr_;
	u_short port_;

	enum { max_length = 1024 };
	char data_[max_length];

public:
	Server(async::iocp::IODispatcher& io, async::network::IPAddress &ip, u_short port)
		: io_service_(io)
		, socket_(io, Udp::V4())
	{
		std::uninitialized_fill_n(reinterpret_cast<char *>(&addr_), sizeof(addr_), 0);
		
		addr_.sin_family = Udp::V4().Family();
		addr_.sin_addr.S_un.S_addr = ::htonl(ip);
		addr_.sin_port = ::htons(port);

		try
		{
			//socket_.SetOption(async::network::RecvTimeOut(3000));
			socket_.AsyncRecvFrom(
				async::network::Buffer(data_, max_length), &addr_,
				std::tr1::bind(&Server::handle_receive_from, this, _1, _2));
		}
		catch(std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}

	}
	~Server()
	{
		Close();
	}

	void handle_receive_from(size_t bytes_recvd, u_long error)
	{
		if( error != 0 )
			return;

		try
		{
			if (!error && bytes_recvd > 0)
			{
				socket_.AsyncSendTo(
					Buffer(data_, bytes_recvd), &addr_,
					std::tr1::bind(&Server::handle_send_to, this, _1, _2));
			}
			else
			{
				socket_.AsyncRecvFrom(
					async::network::Buffer(data_, max_length), &addr_,
					std::tr1::bind(&Server::handle_receive_from, this, _1, _2));
			}
		}
		catch(std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	void handle_send_to(size_t /*bytes_sent*/, u_long& /*error*/)
	{
		try
		{
			socket_.AsyncRecvFrom(
				async::network::Buffer(data_, max_length), &addr_,
				std::tr1::bind(&Server::handle_receive_from, this, _1, _2));
		}
		catch(std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	void Close()
	{
		socket_.Cancel();
		socket_.Close();
	}

};

int main(int argc, char* argv[])
{
	try
	{
		async::iocp::IODispatcher io;

		using namespace std; // For atoi.
		Server s(io, async::network::IPAddress::Parse("127.0.0.1"), 5050);

		system("pause");
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	system("pause");
	return 0;
}

