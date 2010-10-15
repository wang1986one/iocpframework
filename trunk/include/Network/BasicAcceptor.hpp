#ifndef __NETWORK_BASIC_ACCEPTOR_HPP
#define __NETWORK_BASIC_ACCEPTOR_HPP



namespace async
{
	namespace network
	{

		// -------------------------------------------------
		// class BasicAcceptor

		template<typename ProtocolT>
		class BasicAcceptor
		{
		public:
			typedef ProtocolT						ProtocolType;
			typedef SocketPtr						ImplementType;
			typedef Socket::AsyncIODispatcherType	AsyncIODispatcherType;	

		private:
			ImplementType impl_;

		public:
			explicit BasicAcceptor(AsyncIODispatcherType &io)
				: impl_(new Socket(io))
			{}
			BasicAcceptor(AsyncIODispatcherType &io, const ProtocolType &protocol)
				: impl_(new Socket(io))
			{
				impl_->Open(protocol.Family(), protocol.Type(), protocol.Protocol());
			}
			BasicAcceptor(AsyncIODispatcherType &io, const ProtocolType &protocol, u_short port, const IPAddress &addr, bool reuseAddr = true)
				: impl_(new Socket(io))
			{
				impl_->Open(protocol.Family(), protocol.Type(), protocol.Protocol());

				if( reuseAddr )
					impl_->SetOption(ReuseAddr(true));

				Bind(port, addr);
				Listen();
			}

		public:
			ImplementType &Get() 
			{
				return impl_;
			}
			const ImplementType &Get() const
			{
				return impl_;
			}

			bool Open(const ProtocolType &protocol = ProtocolType::V4())
			{
				if( protocol.Type() == SOCK_STREAM )
					impl_->Open(protocol.Family(), protocol.Type(), protocol.Protocol());
				else
					return false;

				return true;
			}

			bool IsOpen() const
			{
				return impl_->IsOpen();
			}

			void Close()
			{
				return impl_->Close();
			}

			void Cancel()
			{
				return impl_->Cancel();
			}

			template<typename SetSocketOptionT>
			bool SetOption(const SetSocketOptionT &option)
			{
				return impl_->SetOption(option);
			}
			template<typename GetSocketOptionT>
			bool GetOption(GetSocketOptionT &option)
			{
				return impl_->GetOption(option)
			}
			template<typename IOControlCommandT>
			bool IOControl(IOControlCommandT &control)
			{
				return impl_->IOControl(control);
			}

			void Bind(u_short port, const IPAddress &addr)
			{
				// warning: only support AF_INET
				impl_->Bind(ProtocolType::V4().Family(), port, addr);
			}

			void Listen(int backlog = SOMAXCONN)
			{
				impl_->Listen(backlog);
			}

			void Accept()
			{
				impl_->Accept();
			}

			AsyncResultPtr AsyncAccept(const AsyncCallbackFunc &callback, size_t szOutSize = 0, const ObjectPtr &asyncState = nothing)
			{
				return impl_->BeginAccept(szOutSize, callback, asyncState);
			}
			SocketPtr EndAccept(const AsyncResultPtr &asynResult)
			{
				return impl_->EndAccept(asynResult);
			}

		};
	}
}







#endif