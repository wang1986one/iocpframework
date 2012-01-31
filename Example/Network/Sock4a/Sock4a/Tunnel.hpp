#ifndef __TUNNEL_HPP
#define __TUNNEL_HPP


#include "../../../../include/IOCP/Dispatcher.hpp"
#include "../../../../include/Network/TCP.hpp"


using namespace async;

class Tunnel
	: public std::tr1::enable_shared_from_this<Tunnel>
{
	network::Tcp::Socket sock_;
	network::Tcp::Socket connect_;

	char buffer_[1024];

public:
	Tunnel(iocp::IODispatcher &io, const network::SocketPtr &sock)
		: sock_(sock)
		, connect_(io, network::Tcp::V4())
	{
	}
	~Tunnel()
	{

	}

public:
	void Start(const std::string &svrIP, u_short svrPort)
	{
		sock_.SetOption(network::NoDelay(true));
			
		try
		{
			connect_.AsyncConnect(network::IPAddress::Parse(svrIP), svrPort, 
				std::tr1::bind(&Tunnel::_OnSvrConnect, shared_from_this(), iocp::_Error, iocp::_Size));

			sock_.AsyncRead(iocp::Buffer(buffer_), 
				std::tr1::bind(&Tunnel::_OnCliRead, shared_from_this(), iocp::_Error, iocp::_Size));
		}
		catch(std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	void Stop()
	{
		_DisConnect();
	}


private:
	void _OnSvrConnect(u_long error, u_long bytes)
	{
		if( error == 0 )
			return;
		else
			_DisConnect();
	}

	void _OnCliRead(u_long error, u_long bytes)
	{
		try
		{
			if( bytes == 0 || error != 0 )
			{
				_DisConnect();
				return;
			}


			iocp::AsyncWrite(connect_, iocp::Buffer(buffer_, bytes), iocp::TransferAll(), 
				std::tr1::bind(&Tunnel::_OnSvrWrite, shared_from_this(), iocp::_Error, iocp::_Size));
		}
		catch(const std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}

	}

	void _OnSvrRead(u_long error, u_long bytes)
	{
		try
		{
			if( bytes == 0 || error != 0 )
			{
				_DisConnect();
				return;
			}


			iocp::AsyncWrite(sock_, iocp::Buffer(buffer_, bytes), iocp::TransferAll(), 
				std::tr1::bind(&Tunnel::_OnCliWrite, shared_from_this(), iocp::_Error, iocp::_Size));
		}
		catch(const std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}

	}


	void _OnSvrWrite(u_long error, u_long bytes)
	{
		try
		{		
			if( bytes == 0 || error != 0 )
			{
				_DisConnect();
				return;
			}

			connect_.AsyncRead(iocp::Buffer(buffer_), 
				std::tr1::bind(&Tunnel::_OnSvrRead, shared_from_this(), iocp::_Error, iocp::_Size));

		}
		catch(std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	void _OnCliWrite(u_long error, u_long bytes)
	{
		try
		{		
			if( bytes == 0 || error != 0 )
			{
				_DisConnect();
				return;
			}

			sock_.AsyncRead(iocp::Buffer(buffer_), 
				std::tr1::bind(&Tunnel::_OnCliRead, shared_from_this(), iocp::_Error, iocp::_Size));

		}
		catch(std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	void _DisConnect()
	{
		sock_.Close();
	}

};

typedef std::tr1::shared_ptr<Tunnel> TunnelPtr;


class Server
{
	iocp::IODispatcher &io_;
	network::Tcp::Accpetor acceptor_;

	const std::string connectIP_;
	u_short connectPort_;

public:
	Server(iocp::IODispatcher &io, u_short listenPort, const std::string &connectIP, u_short connectPort)
		: io_(io)
		, acceptor_(io, network::Tcp::V4(), listenPort)
		, connectIP_(connectIP)
		, connectPort_(connectPort)
	{
		
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
			acceptor_.AsyncAccept(0, 
				std::tr1::bind(&Server::_OnAccept, this, std::tr1::placeholders::_1, std::tr1::placeholders::_2/*std::tr1::cref(acceptSock)*/));
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
	void _OnAccept(u_long error, const network::SocketPtr &acceptSocket)
	{
		if( error != 0 )
			return;

		try
		{
			TunnelPtr tunnel(new Tunnel(io_, acceptSocket));
			tunnel->Start(connectIP_, connectPort_);

			_StartAccept();
		}
		catch(const std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
};





#endif