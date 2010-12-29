// Udp.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../../include/Network/UDP.hpp"


using namespace async::iocp;
using namespace async::network;

using namespace std::tr1::placeholders;

class server
{
private:
	async::iocp::OverlappedDispatcher& io_service_;
	async::network::Udp::Socket socket_;
	SOCKADDR_IN addr_;
	u_short port_;

	enum { max_length = 1024 };
	char data_[max_length];

public:
	server(async::iocp::OverlappedDispatcher& io, short port)
		: io_service_(io)
		, socket_(io, Udp::V4(), port)
	{
		try
		{
			socket_.AsyncRecvFrom(
				async::network::Buffer(data_, max_length), addr_,
				std::tr1::bind(&server::handle_receive_from, this,
				_1, _2));
		}
		catch(std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}

	}

	void handle_receive_from(size_t bytes_recvd, u_long error)
	{
		try
		{
			if (!error && bytes_recvd > 0)
			{
				socket_.AsyncSendTo(
					Buffer(data_, bytes_recvd), addr_,
					std::tr1::bind(&server::handle_send_to, this,
					_1, _2));
			}
			else
			{
				socket_.AsyncRecvFrom(
					async::network::Buffer(data_, max_length), addr_,
					std::tr1::bind(&server::handle_receive_from, this,
					_1, _2));
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
				async::network::Buffer(data_, max_length), addr_,
				std::tr1::bind(&server::handle_receive_from, this,
				_1, _2));
		}
		catch(std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}


};

int main(int argc, char* argv[])
{
	try
	{
		async::iocp::OverlappedDispatcher io;

		using namespace std; // For atoi.
		server s(io, 5050);

		system("pause");
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	system("pause");
	return 0;
}

