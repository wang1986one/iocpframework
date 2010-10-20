#ifndef __HTTP_REPLY_HPP
#define __HTTP_REPLY_HPP




#include <string>
#include <vector>

#include "Header.h"
#include "../../include/Network/TCP.hpp"


namespace http
{
	
	// 返回客服端的回复
	struct Reply
	{
		// 返回客户端的状态码
		enum StatusType
		{
			ok					= 200,
			created				= 201,
			accepted			= 202,
			no_content			= 204,
			multiple_choices	= 300,
			moved_permanently	= 301,
			moved_temporarily	= 302,
			not_modified		= 304,
			bad_request			= 400,
			unauthorized		= 401,
			forbidden			= 403,
			not_found			= 404,
			internal_server_error = 500,
			not_implemented		= 501,
			bad_gateway			= 502,
			service_unavailable = 503
		} status;

		// 回复内容的头信息.
		std::vector<Header> headers;

		// 回复内容的征文
		std::string content;

		/// Convert the reply into a vector of buffers. The buffers do not own the
		/// underlying memory blocks, therefore the reply object must remain valid and
		/// not be changed until the write operation has completed.
		std::vector<async::network::SocketBufferPtr> ToBuffers();

		// 返回一个Reply
		static Reply StockReply(StatusType status);
	};
}






#endif