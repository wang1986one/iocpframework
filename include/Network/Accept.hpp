#ifndef __NETWORK_ACCEPT_HPP
#define __NETWORK_ACCEPT_HPP




namespace detail
{

	// Hook User Accept Callback
	template<typename HandlerT>
	struct AcceptorHandle
	{	
		network::Socket &acceptor_;
		network::SocketPtr remoteSocket_;
		iocp::AutoBufferPtr buf_;
		HandlerT handler_;

		AcceptorHandle(network::Socket &acceptor, const network::SocketPtr &remoteSocket, const iocp::AutoBufferPtr &buf, const HandlerT &handler)
			: acceptor_(acceptor)
			, remoteSocket_(remoteSocket)
			, buf_(buf)
			, handler_(handler)
		{}

		void operator()(u_long size, u_long error)
		{
			// ¸´ÖÆListen socketÊôÐÔ
			async::network::UpdateAcceptContext context(acceptor_);
			remoteSocket_->SetOption(context);

			handler_(error, std::tr1::cref(remoteSocket_), size);
		}
	};
}



#endif