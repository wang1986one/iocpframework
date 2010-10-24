#ifndef __SERVICE_HPP
#define __SERVICE_HPP



#include "../../include/network/Socket.hpp"
#include "../../include/Network/tcp.hpp"


using namespace async::network;

class Session
	: public std::tr1::enable_shared_from_this<Session>
{
private:
	Tcp::StreamSocket socket_;
	std::tr1::array<char, 4096> buf_;

public:
	explicit Session(OverlappedDispatcher &io, const SocketPtr &sock)
		: socket_(io, sock)
	{}
	~Session()
	{
		//Stop();
	}


public:
	Tcp::StreamSocket& GetSocket()
	{
		return socket_;
	}

	void Start()
	{
		try
		{		
			
			AsyncRead(socket_, Buffer(buf_), TransferAtLeast(15),
				std::tr1::bind(&Session::_HandleRead, shared_from_this(), 
				std::tr1::placeholders::_1, std::tr1::placeholders::_2));

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
	void _HandleRead(u_long bytes, u_long error)
	{
		try
		{
			if( bytes == 0 )
			{
				socket_.AsyncDisconnect(std::tr1::bind(&Session::_DisConnect, shared_from_this()));
				//Stop();
				return;
			}

			std::cout.write(buf_.data(), bytes) << std::endl;


			AsyncWrite(socket_, Buffer(buf_.data(), bytes), TransferAll(), 
				std::tr1::bind(&Session::_HandleWrite, shared_from_this()));
		}
		catch(const std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	void _HandleWrite()
	{
		Start();
	}

	void _DisConnect()
	{
		Stop();
	}
};

typedef std::tr1::shared_ptr<Session> SessionPtr;


inline SocketPtr CreateSocket(OverlappedDispatcher &io)
{
	return SocketPtr(new Socket(io, Tcp::V4().Family(), Tcp::V4().Type(), Tcp::V4().Protocol()));
}

inline SessionPtr CreateSession(OverlappedDispatcher &io, const SocketPtr &socket)
{
	return SessionPtr(new Session(io, socket));
}

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
			SocketPtr acceptSock(CreateSocket(io_));
			acceptor_.AsyncAccept(acceptSock, 0, 
				std::tr1::bind(&Server::_OnAccept, this, std::tr1::placeholders::_1, std::tr1::placeholders::_2));
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
	void _OnAccept(u_long error, const SocketPtr &acceptSocket)
	{
		if( error != 0 )
			return;

		try
		{
			SessionPtr session(CreateSession(io_, acceptSocket));
			session->Start();

			_StartAccept();
		}
		catch(const std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
};



#endif