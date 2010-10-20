#include "stdafx.h"
#include "connection.h"


#include <vector>
#include "ConnectionMgr.h"
#include "RequestHandler.h"



namespace http
{
	
	Connection::Connection(async::iocp::OverlappedDispatcher &io, ConnectionMgr &mgr, RequestHandler &handler)
		: socket_(io, async::network::Tcp::V4())
		, connectionMgr_(mgr)
		, requestHandler_(handler)
		, socketBuffer_(async::network::MakeBuffer(buffer_))
	{}


	async::network::Tcp::StreamSocket &Connection::Socket()
	{
		return socket_;
	}

	void Connection::Start()
	{
		using namespace std::tr1::placeholders;

		socket_.AsyncRead(socketBuffer_, std::tr1::bind(&Connection::_HandleRead,
			shared_from_this(), _1, _2, _3));
	}

	void Connection::Stop()
	{
		socket_.Close();
	}
	

	void Connection::_HandleRead(const async::iocp::AsyncResultPtr &result, size_t bytes, u_long error)
	{
		if( error == ERROR_OPERATION_ABORTED || bytes == 0 )
		{
			connectionMgr_.Stop(shared_from_this());
			return;
		}
		else if( !error )
		{
			try
			{
				ParseRet result, ignore;
				std::tr1::tie(result, ignore) = requestParser_.Parse(request_, socketBuffer_->data(), socketBuffer_->data(bytes));

				if( result == TRUE_VALUE )
				{
					requestHandler_.HandleRequest(request_, reply_);
					AsyncWrite(socket_.Get(), async::network::MakeBuffer(reply_.ToBuffers()),
						std::tr1::bind(&Connection::_HandleWrite, shared_from_this(), 
						std::tr1::placeholders::_2, std::tr1::placeholders::_3));
				}	
				else if( result == FALSE_VALUE )
				{
					reply_ = Reply::StockReply(Reply::bad_request);
					AsyncWrite(socket_.Get(), async::network::MakeBuffer(reply_.ToBuffers()),
						std::tr1::bind(&Connection::_HandleWrite, shared_from_this(), 
						std::tr1::placeholders::_2, std::tr1::placeholders::_3));
				}
				else	// ParseRet::INDETERMINATE
				{
					socket_.AsyncRead(socketBuffer_, 
						std::tr1::bind(&Connection::_HandleRead, shared_from_this(),
						std::tr1::placeholders::_1, std::tr1::placeholders::_2, std::tr1::placeholders::_3));
				}
			}
			catch(std::exception &e)
			{
				std::cerr << e.what() << std::endl;

				connectionMgr_.Stop(shared_from_this());
			}
		}
	}


	void Connection::_HandleWrite(size_t bytes, u_long error)
	{
		// нч╢М
		if( error == 0 )
		{
			socket_.Shutdown(SD_BOTH);
		}
		
		if( error != ERROR_OPERATION_ABORTED )
		{
			connectionMgr_.Stop(shared_from_this());
		}
	}
}