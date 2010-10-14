#ifndef __SERVICE_HPP
#define __SERVICE_HPP



#include "../../include/network/Socket.hpp"
#include "../../include/Network/tcp.hpp"

#include "../../include/Network/BufferHelper.hpp"



using namespace async::network;

class Session
	: public std::tr1::enable_shared_from_this<Session>
{
private:
	Tcp::StreamSocket socket_;
	SocketBufferPtr data_;
	std::tr1::array<char, 4096> buf_;

public:
	Session(const SocketPtr &sock)
		: socket_(sock)
		, data_(MakeBuffer(buf_))
	{
	}
	~Session()
	{
		//Stop();
	}
	Tcp::StreamSocket& GetSocket()
	{
		return socket_;
	}

	void Start()
	{
		try
		{		
			socket_.AsyncRecv(data_, 0, data_->allocSize(), 
				std::tr1::bind(&Session::_HandleRead, shared_from_this(), std::tr1::placeholders::_1));
		}
		catch(std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	void Stop()
	{
		socket_.Close();
	}

private:
	void _HandleRead(const AsyncResultPtr &asyncResult)
	{
		try
		{
			size_t bytes = socket_.EndRecv(asyncResult);
			if( bytes == 0 )
			{
				socket_.AsyncDisconnect(std::tr1::bind(&Session::_DisConnect, shared_from_this(), std::tr1::placeholders::_1));
				return;
			}

			data_->resize(bytes);
			socket_.AsyncSend(data_, 0, bytes, std::tr1::bind(&Session::_HandleWrite, shared_from_this(), std::tr1::placeholders::_1));
		}
		catch(const std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	void _HandleWrite(const AsyncResultPtr &asyncResult)
	{
		Start();
	}

	void _DisConnect(const AsyncResultPtr &asyncResult)
	{
		Stop();
	}
};

typedef std::tr1::shared_ptr<Session> SessionPtr;


class Server
{
private:
	OverlappedDispatcher &io_;
	Tcp::Accpetor acceptor_;

public:
	Server(OverlappedDispatcher &io, short port)
		: io_(io)
		, acceptor_(io_, Tcp::V4(), port, INADDR_ANY)
	{}

	~Server()
	{
		_StopServer();
	}

public:
	void Start()
	{
		_StartAccept();
	}

	void Stop()
	{
		_StopServer();
	}

private:
	void _StartAccept()
	{		
		try
		{
			acceptor_.AsyncAccept(std::tr1::bind(&Server::_OnAccept, this, std::tr1::placeholders::_1));
		} 
		catch(const std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	void _StopServer()
	{
		acceptor_.Close();
	}

private:
	void _OnAccept(const AsyncResultPtr &asyncResult)
	{
		try
		{
			const SocketPtr &acceptSocket = acceptor_.EndAccept(asyncResult);

			SessionPtr session_(new Session(acceptSocket));
			session_->Start();

			_StartAccept();
		}
		catch(const std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
};



#endif