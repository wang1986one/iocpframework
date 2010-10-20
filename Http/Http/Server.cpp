#include "stdafx.h"
#include "Server.h"



namespace http
{

	Server::Server(const std::string &addr, const std::string &port, const std::string &dir)
		: io_()
		, acceptor_(io_, async::network::Tcp::V4(), ::atoi(port.c_str()))
		, connectionMgr_()
		, newConnection_(new Connection(io_, connectionMgr_, requestHandler_))
		, requestHandler_(dir)
	{
		
	}

	void Server::Start()
	{
		acceptor_.AsyncAccept(newConnection_->Socket().Get(), 0, 
			std::tr1::bind(&Server::_HandleAccept, this, std::tr1::placeholders::_1));
	}

	void Server::Stop()
	{
		async::iocp::AsyncResultPtr result(new async::iocp::AsyncResult(
			async::iocp::nothing, async::iocp::nothing, std::tr1::bind(&Server::_HandleStop, this)));
		io_.Post(result);
	}

	void Server::_HandleAccept(const async::iocp::AsyncResultPtr &result)
	{
		try
		{
			const async::network::SocketPtr &remoteSocket = acceptor_.EndAccept(result);
			connectionMgr_.Start(newConnection_);

			newConnection_.reset(new Connection(io_, connectionMgr_, requestHandler_));
			
			acceptor_.AsyncAccept(newConnection_->Socket().Get(), 0, std::tr1::bind(&Server::_HandleAccept,
				this, std::tr1::placeholders::_1));
		}
		catch(std::exception &e)
		{
			std::cerr << e.what() << std::endl;

		}
	}

	void Server::_HandleStop()
	{
		acceptor_.Close();
		connectionMgr_.StopAll();
	}
}