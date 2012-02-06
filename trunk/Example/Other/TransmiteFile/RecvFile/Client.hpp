#ifndef __CLIENT_IMPL_HPP
#define __CLIENT_IMPL_HPP


#include "../../../../include/Network/TCP.hpp"

#include <fstream>

using namespace async::network;

struct header
{
	char name[MAX_PATH];
	size_t size;
};

class Client
{
private:
	IODispatcher &io_;
	Tcp::Socket socket_;
	std::tr1::array<char, 4096>	buf_;

	std::ofstream os;
	size_t total_size_;
	size_t read_size_;

public:
	Client(IODispatcher &io, const std::string &ip, u_short port)
		: io_(io)
		, socket_(io, Tcp::V4())
		, total_size_(0)
		, read_size_(0)
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

		try
		{
			
			AsyncRead(socket_, Buffer(buf_, sizeof(header)), TransferAtLeast(sizeof(header)),
				std::tr1::bind(&Client::_HandleReadHeader, this, _Size, _Error));
		}
		catch(std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	void _HandleReadHeader(u_long bytes, u_long error)
	{
		header *head = reinterpret_cast<header *>(buf_.data());
		os.open(head->name, std::ios::binary);
		total_size_ = head->size;

		AsyncRead(socket_, Buffer(buf_), TransferAtLeast(1),
			std::tr1::bind(&Client::_HandleRead, this, _Size, _Error));
	}

	void _HandleRead(u_long bytes, u_long error)
	{
		try
		{
			if( bytes == 0 )
			{
				_DisConnect();
				return;
			}

			os.write(buf_.data(), bytes);

			read_size_ += bytes;
			if( read_size_ != total_size_ )
			{
				AsyncRead(socket_, Buffer(buf_), TransferAtLeast(1),
					std::tr1::bind(&Client::_HandleRead, this, _Size, _Error));
			}
		}
		catch(const std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	void _DisConnect()
	{
		os.close();
		socket_.Close();
	}
};



#endif