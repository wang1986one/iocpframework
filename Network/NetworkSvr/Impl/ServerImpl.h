#ifndef __SERVICE_HPP
#define __SERVICE_HPP



#include "../../include/Network/TCP.hpp"


using namespace async::network;


volatile long g_ClientNum = 0;


class Session
	: public std::tr1::enable_shared_from_this<Session>
{
private:
	Tcp::Socket socket_;
	std::tr1::array<char, 4096> buf_;

public:
	explicit Session(const SocketPtr &sock)
		: socket_(sock)
	{
		::InterlockedIncrement(&g_ClientNum);
	}
	~Session()
	{
		//Stop();
		::InterlockedDecrement(&g_ClientNum);
	}


public:
	Tcp::Socket& GetSocket()
	{
		return socket_;
	}

	void Start()
	{
		try
		{		
			AsyncRead(socket_, Buffer(buf_), TransferAtLeast(1),
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




// �����Լ��Ĺ���
namespace async
{
	namespace iocp
	{
		template<>
		struct ObjectFactory< Session >
		{
			typedef async::memory::FixedMemoryPool<true, sizeof(Session)>	PoolType;
			typedef ObjectPool< Session, PoolType >							ObjectPoolType;
		};
	}
}


template<typename T>
struct NoneDeletor
{
	void operator()(T *)
	{}
};


inline SessionPtr CreateSession(const SocketPtr &socket)
{
	return SessionPtr(ObjectAlloc<Session>(socket), &ObjectDeallocate<Session>);
	//return SessionPtr(new Session(io, socket));
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
			SocketPtr acceptSock(MakeSocket(io_, Tcp::V4().Family(), Tcp::V4().Type(), Tcp::V4().Protocol()));
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
			SessionPtr session(CreateSession(acceptSocket));
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