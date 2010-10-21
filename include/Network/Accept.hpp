#ifndef __NETWORK_ACCEPT_HPP
#define __NETWORK_ACCEPT_HPP




namespace internal
{

	// Hook User Accept Callback
	template<typename HandlerT>
	struct AcceptorHandle
	{	
		network::Socket &acceptor_;
		network::SocketPtr remoteSocket_;
		network::SocketBufferPtr buf_;
		HandlerT handler_;

		AcceptorHandle(network::Socket &acceptor, const network::SocketPtr &remoteSocket, const SocketBufferPtr &buf, const HandlerT &handler)
			: acceptor_(acceptor)
			, remoteSocket_(remoteSocket)
			, buf_(buf)
			, handler_(handler)
		{}

		void operator()(const iocp::AsyncResultPtr &reuslt, u_long size, u_long error)
		{
			// ¸´ÖÆListen socketÊôÐÔ
			async::network::UpdateAcceptContext context(acceptor_);
			remoteSocket_->SetOption(context);

			handler_(std::tr1::cref(reuslt), size, error, std::tr1::cref(remoteSocket_));
		}
	};
}



#endif