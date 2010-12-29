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
		, socketBuffer_()
	{}


	async::network::Tcp::Socket &Connection::Socket()
	{
		return socket_;
	}

	void Connection::Start()
	{
		using namespace std::tr1::placeholders;

		socket_.AsyncRead(async::iocp::Buffer(buffer_), 
			std::tr1::bind(&Connection::_HandleRead, shared_from_this(), _1, _2));
	}

	void Connection::Stop()
	{
		socket_.Close();
	}
	

	void Connection::_CopyBuffer(const std::vector<async::iocp::ConstBuffer> &buf)
	{
		size_t total = 0;
		for(size_t i = 0; i != buf.size(); ++i)
		{
			total += buf[i].size();
		}
		socketBuffer_.resize(total);

		size_t index = 0;
		for(size_t i =0; i != buf.size(); ++i)
		{
			std::copy(buf[i].begin(), buf[i].end(), socketBuffer_.data(index));
			index += buf[i].size();
		}
	}

	void Connection::_HandleRead(size_t bytes, u_long error)
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
				using namespace std::tr1::placeholders;

				ParseRet result, ignore;
				std::tr1::tie(result, ignore) = requestParser_.Parse(request_, buffer_.data(), buffer_.data() + bytes);

				if( result == TRUE_VALUE )
				{
					requestHandler_.HandleRequest(request_, reply_);

					_CopyBuffer(reply_.ToBuffers());
					AsyncWrite(socket_, async::iocp::Buffer(socketBuffer_.data(), socketBuffer_.size()),
						std::tr1::bind(&Connection::_HandleWrite, shared_from_this(), _1, _2));
				}	
				else if( result == FALSE_VALUE )
				{
					reply_ = Reply::StockReply(Reply::bad_request);

					_CopyBuffer(reply_.ToBuffers());
					AsyncWrite(socket_, async::iocp::Buffer(socketBuffer_.data(), socketBuffer_.size()),
						std::tr1::bind(&Connection::_HandleWrite, shared_from_this(), _1, _2));
				}
				else	// ParseRet::INDETERMINATE
				{
					socket_.AsyncRead(async::iocp::Buffer(buffer_), 
						std::tr1::bind(&Connection::_HandleRead, shared_from_this(), _1, _2));
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
			//socket_.AsyncDisconnect(NULL);
		}
		
		//if( error != ERROR_OPERATION_ABORTED )
		{
			connectionMgr_.Stop(shared_from_this());
		}
	}
}