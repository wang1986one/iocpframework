#ifndef __SERVICE_HPP
#define __SERVICE_HPP

#include "../../../include/iocp/Dispatcher.hpp"
#include "../../../include/Network/TCP.hpp"


using namespace async::iocp;
using namespace async::network;


class Session
	: public std::tr1::enable_shared_from_this<Session>
{
	IODispatcher &io_;
	Tcp::Socket socket_;
	size_t blockSize_;
	size_t readDataLen_;

	char *readData_;
	char *writeData_;

	size_t unSentCount_;
	size_t opCount_;

public:
	Session(IODispatcher &io, size_t blockSize)
		: io_(io)
		, blockSize_(blockSize)
		, socket_(io)
		, readDataLen_(0)
		, readData_(new char[blockSize_])
		, writeData_(new char[blockSize_])
		, unSentCount_(0)
		, opCount_(0)
	{}

	~Session()
	{
		delete []readData_;
		delete []writeData_;
	}

	Tcp::Socket &Socket()
	{
		return socket_;
	}

	void Start()
	{
		bool suc = socket_.SetOption(NoDelay(true));

		try
		{
			if( suc )
			{
				++opCount_;
				socket_.AsyncRead(Buffer(readData_, blockSize_), 
					std::tr1::bind(&Session::_OnRead, shared_from_this(), _Error, _Size));
			}
			else
			{
				//io_.Post(std::tr1::)
			}
		}
		catch(const std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
		
	}




private:
	void _OnRead(u_long err, u_long size)
	{
		--opCount_;
		if( size == 0 )
			return;

		try
		{
			if( !err )
			{
				readDataLen_ = size;
				++unSentCount_;

				if( unSentCount_ == 1 )
				{
					opCount_ += 2;
					std::swap(readData_, writeData_);
					AsyncWrite(socket_, Buffer(writeData_, readDataLen_), 
						std::tr1::bind(&Session::_OnWrite, shared_from_this(), _Error));
					socket_.AsyncRead(Buffer(readData_, blockSize_), 
						std::tr1::bind(&Session::_OnRead, shared_from_this(), _Error, _Size));
				}
			}
		}
		catch(const std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
		
	}	

	void _OnWrite(u_long err)
	{
		--opCount_;


		if( !err )
		{

			try
			{
				--unSentCount_;
				if( unSentCount_ == 1 )
				{
					opCount_ += 2;
					std::swap(readData_, writeData_);

					AsyncWrite(socket_, Buffer(writeData_, readDataLen_), 
						std::tr1::bind(&Session::_OnWrite, shared_from_this(), _Error));
					socket_.AsyncRead(Buffer(readData_, blockSize_), 
						std::tr1::bind(&Session::_OnRead, shared_from_this(), _Error, _Size));
				}
			}
			catch(std::exception &e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
	}
};

typedef std::tr1::shared_ptr<Session> SessionPtr;


class Server
{
	IODispatcher &io_;
	Tcp::Accpetor acceptor_;
	size_t blockSize_;

public:
	Server(IODispatcher &io, u_short port, size_t blockSize)
		: io_(io)
		, blockSize_(blockSize)
		, acceptor_(io)
	{
		acceptor_.Open();
		acceptor_.SetOption(ReuseAddr(true));
		acceptor_.Bind(AF_INET, port, INADDR_ANY);
		acceptor_.Listen();

		SocketPtr acceptSock(MakeSocket(io_, Tcp::V4().Family(), Tcp::V4().Type(), Tcp::V4().Protocol()));
		acceptor_.AsyncAccept(acceptSock, 0, 
			std::tr1::bind(&Server::_OnAccept, this, std::tr1::placeholders::_1, std::tr1::placeholders::_2));
	}


private:
	void _OnAccept(u_long err, const SocketPtr &sock)
	{
		if( err != 0 )
			return;

		try
		{
			SessionPtr session(new Session(io_, blockSize_));
			session->Socket().Get() = sock;

			session->Start();

			SocketPtr acceptSock(MakeSocket(io_, Tcp::V4().Family(), Tcp::V4().Type(), Tcp::V4().Protocol()));
			acceptor_.AsyncAccept(acceptSock, 0, 
				std::tr1::bind(&Server::_OnAccept, this, std::tr1::placeholders::_1, std::tr1::placeholders::_2));
		}
		catch(const std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
};


#endif