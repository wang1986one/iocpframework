#ifndef __HTTP_CONNECTION_HPP
#define __HTTP_CONNECTION_HPP

#include <array>
#include <memory>


#include "Reply.h"
#include "Request.h"
#include "RequestHandler.h"
#include "RequestParser.h"


namespace http
{

	class ConnectionMgr;

	class Connection
		: public std::tr1::enable_shared_from_this<Connection>
	{
	private:
		async::network::Tcp::StreamSocket socket_;
		ConnectionMgr &connectionMgr_;
		RequestHandler &requestHandler_;

		std::tr1::array<char, 8192> buffer_;
		async::iocp::AutoBuffer socketBuffer_;
		Request request_;
		RequestParser requestParser_;
		Reply reply_;

	public:
		Connection(async::iocp::OverlappedDispatcher &io, ConnectionMgr &mgr, RequestHandler &handler);

	private:
		Connection(const Connection &);
		Connection &operator=(const Connection &);

	public:
		async::network::Tcp::StreamSocket &Socket();

		void Start();
		void Stop();

	private:
		void _CopyBuffer(const std::vector<async::iocp::ConstBuffer> &buf);
		void _HandleRead(size_t bytes, u_long error);
		void _HandleWrite(size_t bytes, u_long error);
	};


	typedef std::tr1::shared_ptr<Connection>	ConnectionPtr;
}

#endif