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
	explicit Session(OverlappedDispatcher &io, const SocketPtr &sock)
		: socket_(io, sock)
		, data_(MakeBuffer(buf_))
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
			//socket_.AsyncRead(data_, 0, data_->allocSize(), 
			//	std::tr1::bind(&Session::_HandleRead, shared_from_this(), std::tr1::placeholders::_1, std::tr1::placeholders::_2));

			AsyncRead(socket_.Get(), data_, TransferAtLeast(15),
				std::tr1::bind(&Session::_HandleRead, shared_from_this(), std::tr1::placeholders::_1, std::tr1::placeholders::_2));

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
	void _HandleRead(const AsyncResultPtr &asyncResult, u_long bytes)
	{
		try
		{
			//size_t bytes = socket_.EndRead(asyncResult);
			if( bytes == 0 )
			{
				socket_.AsyncDisconnect(std::tr1::bind(&Session::_DisConnect, shared_from_this()));
				return;
			}

			data_->resize(bytes);
			std::cout.write(data_->data(), bytes) << std::endl;

			//socket_.AsyncWrite(data_, 0, bytes, 
			//	std::tr1::bind(&Session::_HandleWrite, shared_from_this(), std::tr1::placeholders::_1));
			AsyncWrite(socket_.Get(), data_, TransferAll(), 
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
				std::tr1::bind(&Server::_OnAccept, this, std::tr1::placeholders::_1));
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