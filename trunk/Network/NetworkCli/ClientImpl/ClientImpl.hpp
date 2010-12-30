#ifndef __CLIENT_IMPL_HPP
#define __CLIENT_IMPL_HPP


#include "../../include/Network/TCP.hpp"


using namespace async::network;



class Client
{
private:
	OverlappedDispatcher &io_;
	Tcp::Socket socket_;

public:
	Client(OverlappedDispatcher &io, const std::string &ip, u_short port)
		: io_(io)
		, socket_(io, Tcp())
	{
		socket_.AsyncConnect(IPAddress::Parse(ip), port, 
			std);
	}


private:
	void _OnConnect()
	{

	}
};



#endif