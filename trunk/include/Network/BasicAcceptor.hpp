#ifndef __NETWORK_BASIC_ACCEPTOR_HPP
#define __NETWORK_BASIC_ACCEPTOR_HPP


namespace async
{
	namespace network
	{

		// -------------------------------------------------
		// class BasicAcceptor

		template<typename ProtocolT, typename AcceptorServiceT>
		class BasicAcceptor
		{
		public:
			typedef ProtocolT			ProtocolType;
			typedef AcceptorServiceT	AcceptorServiceType;

		private:
			AcceptorServiceType impl_;

		public:
			explicit BasicAcceptor(IODispatcher &io)
				: impl_(new Socket(io))
			{

			}
			BasicAcceptor(IODispatcher &io, const ProtocolType &protocol)
				: impl_(new Socket(io))
			{
				impl_.Open(protocol.Type(), protocol.Protocol());
			}
			BasicAcceptor(IODispatcher &io, const ProtocolType &protocol, u_short port, const IPAddress &addr, bool reuseAddr = true)
				: impl_(new Socket(io))
			{
				impl_.Open(protocol.Type(), protocol.Protocol());

				if( reuseAddr )
					impl_.SetOption(ReuseAddr(true));

				Bind(port, addr);
				Listen();
			}

		public:
			bool Open(const ProtocolType &protocol)
			{
				if( protocol.Type() == SOCK_STREAM )
					impl_.Open(protocol.Type(), protocol.Protocol());
				else
					return false;

				return true;
			}

			bool IsOpen() const
			{
				return impl_.IsOpen();
			}

			void Close()
			{
				return impl_.Close();
			}

			bool Cancel()
			{
				return impl_.Cancel();
			}

			template<typename SetSocketOptionT>
			bool SetOption(const SetSocketOptionT &option)
			{
				return impl_.SetOption(option);
			}
			template<typename GetSocketOptionT>
			bool GetOption(GetSocketOptionT &option)
			{
				return impl_.GetOption(option)
			}
			template<typename IOControlCommandT>
			bool IOControl(IOControlCommandT &control)
			{
				return impl_.IOControl(control);
			}

			void Bind(u_short port, const IPAddress &addr)
			{
				impl_.Bind(ProtocolType::V4(), port, addr);
			}

			void Listen(int backlog = SOMAXCONN)
			{
				impl_.Listen(backlog);
			}

			void Accept()
			{
				impl_.Accept();
			}

			AsyncResultPtr AsyncAccept(size_t szOutSize = 0, const AsyncCallbackFunc &callback = NULL, const ObjectPtr &asyncState = nothing)
			{
				return impl_.AsyncAccept(szOutSize, callback, asyncState);
			}
			SocketPtr EndAccept(const AsyncResultPtr &asynResult)
			{
				return impl_.EndAccept(asynResult);
			}

		};
	}
}







#endif