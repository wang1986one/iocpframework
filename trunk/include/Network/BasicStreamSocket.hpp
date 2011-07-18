#ifndef __NETWORK_BASIC_STREAM_SOCKET_HPP
#define __NETWORK_BASIC_STREAM_SOCKET_HPP


namespace async
{
	namespace network
	{
		// -------------------------------------------------
		// class BasicStreamSocket

		template<typename ProtocolT>
		class BasicStreamSocket
		{
		public:
			typedef ProtocolT						ProtocolType;
			typedef SocketPtr						ImplementType;
			typedef Socket::AsyncIODispatcherType	AsyncIODispatcherType;	

		private:
			ImplementType impl_;
			iocp::CallbackType callback_;

		public:
			BasicStreamSocket(AsyncIODispatcherType &io, const iocp::CallbackType &callback = 0)
				: impl_(MakeSocket(io))
				, callback_(callback)
			{}
			BasicStreamSocket(const ImplementType &impl, const iocp::CallbackType &callback = 0)
				: impl_(impl)
				, callback_(callback)
			{}

			BasicStreamSocket(AsyncIODispatcherType &io, const ProtocolType &protocol, const iocp::CallbackType &callback = 0)
				: impl_(MakeSocket(io, protocol.Family(), protocol.Type(), protocol.Protocol()))
				, callback_(callback)
			{}

		public:
			// 显示获取
			ImplementType &Get() 
			{
				return impl_;
			}
			const ImplementType &Get() const
			{
				return impl_;
			}
		
			// 支持隐式转换
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

			void Shutdown(int shut)
			{
				impl_->Shutdown(shut);
			}
			void Close()
			{
				impl_->Shutdown(SD_BOTH);
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


			// 连接远程服务
			void Connect(int family, const IPAddress &addr, u_short port)
			{
				impl_->Connect(family, addr, port);
			}

			void DisConnect(int shut = SD_BOTH)
			{
				impl_->DisConnect(shut, true);
			}
			 
			// 异步链接
			void AsyncConnect(const IPAddress &addr, u_short port)
			{
				assert(callback_ != 0);
				return impl_->AsyncConnect(addr, port, callback_);
			}

			template < typename HandlerT >
			void AsyncConnect(const IPAddress &addr, u_short port, const HandlerT &handler)
			{
				callback_ = handler;
				return AsyncConnect(addr, port);
			}
			

			// 异步断开链接
			void AsyncDisconnect(bool reuse = true)
			{
				assert(callback_ != 0);
				return impl_->AsyncDisconnect(callback_, reuse);
			}

			template < typename HandlerT >
			void AsyncDisconnect(const HandlerT &callback, bool reuse = true)
			{
				//callback_ = callback;
				return impl_->AsyncDisconnect(callback, reuse);
			}
			

			// 阻塞式发送数据直到数据发送成功或出错
			template<typename ConstBufferT>
			size_t Write(const ConstBufferT &buffer)
			{
				return Write(buffer, 0);
			}
			template<typename ConstBufferT>
			size_t Write(const ConstBufferT &buffer, DWORD flag)
			{
				return impl_->Write(buffer.data(), buffer.size(), flag);
			}

			// 异步发送数据
			template<typename ConstBufferT>
			void AsyncWrite(const ConstBufferT &buffer)
			{
				//assert(callback_ != 0);
				return impl_->AsyncWrite(buffer.data(), buffer.size(), callback_);
			}
			template<typename ConstBufferT, typename HandlerT >
			void AsyncWrite(const ConstBufferT &buffer, const HandlerT &callback)
			{
				//callback_ = callback;
				return impl_->AsyncWrite(buffer.data(), buffer.size(), callback);
			}

			

			// 阻塞式接收数据直到成功或出错
			template<typename MutableBufferT>
			size_t Read(MutableBufferT &buffer)
			{
				return Read(buffer, 0);
			}
			template<typename MutableBufferT>
			size_t Read(MutableBufferT &buffer, u_long flag)
			{
				return impl_->Read(buffer.data(), buffer.size(), flag);
			}
	

			// 异步接收数据
			template<typename MutableBufferT>
			void AsyncRead(MutableBufferT &buffer)
			{
				//assert(callback_ != 0);
				return impl_->AsyncRead(buffer.data(), buffer.size(), callback_);
			}

			template<typename MutableBufferT, typename HandlerT >
			void AsyncRead(MutableBufferT &buffer, const HandlerT &callback)
			{
				//callback_ = callback;
				return impl_->AsyncRead(buffer.data(), buffer.size(), callback);
			}

		};
	}
}





#endif