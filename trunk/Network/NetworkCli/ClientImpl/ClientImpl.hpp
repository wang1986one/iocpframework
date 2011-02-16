#ifndef __CLIENT_IMPL_HPP
#define __CLIENT_IMPL_HPP


#include "../../include/Network/TCP.hpp"


using namespace async::network;



class Client
{
private:
	IODispatcher &io_;
	Tcp::Socket socket_;
	std::tr1::array<char, 4096>	buf_;

public:
	Client(IODispatcher &io, const std::string &ip, u_short port)
		: io_(io)
		, socket_(io, Tcp::V4())
	{
		try
		{
			socket_.AsyncConnect(IPAddress::Parse(ip), port, 
				std::tr1::bind(&Client::_OnConnect, this, _Size, _Error));
		}
		catch(std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}


private:
	void _OnConnect(u_long bytes, u_long error)
	{
		if( error != 0 )
			return;

		static char msg[] = "I am a new client";

		try
		{
			AsyncWrite(socket_, Buffer(msg), TransferAll(), 
				std::tr1::bind(&Client::_HandleWrite, this, _Size, _Error));
		}
		catch(std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	void _HandleRead(u_long bytes, u_long error)
	{
		try
		{
			if( bytes == 0 )
			{
				socket_.AsyncDisconnect(std::tr1::bind(&Client::_DisConnect, this));
				return;
			}

			std::cout.write(buf_.data(), bytes) << std::endl;

			::Sleep(2000);

			AsyncWrite(socket_, Buffer(buf_.data(), bytes), TransferAll(), 
				std::tr1::bind(&Client::_HandleWrite, this, _Size, _Error));
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
			if( bytes == 0 )
			{
				socket_.AsyncDisconnect(std::tr1::bind(&Client::_DisConnect, this));
				return;
			}

			AsyncRead(socket_, Buffer(buf_), TransferAtLeast(1),
				std::tr1::bind(&Client::_HandleRead, this, _Size, _Error));

		}
		catch(std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	void _DisConnect()
	{
		socket_.Close();
	}
};



#endif