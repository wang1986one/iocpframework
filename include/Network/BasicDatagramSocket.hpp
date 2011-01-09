#ifndef __NETWORK_DATAGRAM_HPP
#define __NETWORK_DATAGRAM_HPP




namespace async
{
	namespace network
	{

		// ---------------------------------------------------
		// class BasicDatagramSocket

		template<typename ProtocolT>
		class BasicDatagramSocket
		{
		public:
			typedef ProtocolT						ProtocolType;
			typedef SocketPtr						ImplementType;
			typedef Socket::AsyncIODispatcherType	AsyncIODispatcherType;	

		private:
			ImplementType impl_;

		public:
			explicit BasicDatagramSocket(AsyncIODispatcherType &io)
				: impl_(new Socket(io))
			{}
			explicit BasicDatagramSocket(const ImplementType &impl)
				: impl_(impl)
			{}

			BasicDatagramSocket(AsyncIODispatcherType &io, const ProtocolType &protocol)
				: impl_(new Socket(io, protocol.Family(), protocol.Type(), protocol.Protocol()))
			{}
			BasicDatagramSocket(AsyncIODispatcherType &io, const ProtocolType &protocol, u_short port)
				: impl_(new Socket(io, protocol.Family(), protocol.Type(), protocol.Protocol()))
			{
				impl_->Bind(protocol.Family(), port, INADDR_ANY);
			}
			~BasicDatagramSocket()
			{}

		public:
			// ��ʾ��ȡ
			ImplementType &Get() 
			{
				return impl_;
			}
			const ImplementType &Get() const
			{
				return impl_;
			}

			// ֧����ʽת��
			operator ImplementType()
			{
				return impl_;
			}
			operator const ImplementType() const
			{
				return impl_;
			}


		public:
			void Open(const ProtocolType &protocol = ProtocolType::V4())
			{
				impl_->Open(protocol.Family(), protocol.Type(), protocol.Protocol());
			}

			bool IsOpen() const
			{
				return impl_->IsOpen();
			}

			void Close()
			{
				impl_->Close();
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

			void Bind(int family, u_short port, const IPAddress &addr)
			{
				impl_->Bind(family, port, addr);
			}


			// ����Զ�̷���
			void Connect(int family, const IPAddress &addr, u_short port)
			{
				impl_->Connect(family, addr, port);
			}

			void DisConnect(int shut = SD_BOTH)
			{
				impl_->DisConnect(shut, true);
			}

			// �첽����
			template<typename HandlerT>
			AsyncIOCallback<HandlerT> *AsyncConnect(const IPAddress &addr, u_short port, const HandlerT &handler)
			{
				return impl_->AsyncConnect(addr, port, handler);
			}
			template<typename AsyncT>
			const AsyncT &AsyncConnect(const AsyncT &result, const IPAddress &addr, u_short uPort)
			{
				return impl_->AsyncConnect(result, addr, uPort);
			}

			
			// ����ʽ��������ֱ�����ݷ��ͳɹ������
			template<typename ConstBufferT>
			size_t SendTo(const ConstBufferT &buffer, const SOCKADDR_IN &addr)
			{
				return impl_->SendTo(buffer.data(), buffer.size(), addr, 0);
			}
			template<typename ConstBufferT>
			size_t SendTo(const ConstBufferT &buffer, const SOCKADDR_IN &addr, u_long flag)
			{
				return impl_->SendTo(buffer.data(), buffer.size(), addr, flag);
			}

			// ����ʽ��������ֱ���ɹ������
			template<typename MutableBufferT>
			size_t RecvFrom(MutableBufferT &buffer, SOCKADDR_IN &addr)
			{
				return impl_->RecvFrom(buffer.data(), buffer.size(), addr, 0);
			}
			template<typename MutableBufferT>
			size_t RecvFrom(MutableBufferT &buffer, SOCKADDR_IN &addr, u_long flag)
			{
				return impl_->RecvFrom(buffer.data(), buffer.size(), addr, flag);
			}



			// �첽��������
			template<typename ConstBufferT, typename HandlerT>
			AsyncIOCallback<HandlerT> *AsyncSendTo(const ConstBufferT &buffer, const SOCKADDR_IN &addr, const HandlerT &callback)
			{
				return impl_->AsyncSendTo(buffer.data(), buffer.size(), addr, callback);
			}

			template<typename MutableBufferT, typename HandlerT>
			AsyncIOCallback<HandlerT> *AsyncRecvFrom(MutableBufferT &buffer, SOCKADDR_IN &addr, const HandlerT &callback)
			{
				return impl_->AsyncRecvFrom(buffer.data(), buffer.size(), addr, callback);
			}

		};
	}
}




#endif