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

		public:
			explicit BasicStreamSocket(AsyncIODispatcherType &io)
				: impl_(MakeSocket(io))
			{}
			explicit BasicStreamSocket(const ImplementType &impl)
				: impl_(impl)
			{}

			BasicStreamSocket(AsyncIODispatcherType &io, const ProtocolType &protocol)
				: impl_(MakeSocket(io, protocol.Family(), protocol.Type(), protocol.Protocol()))
			{}
			~BasicStreamSocket()
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
			template<typename HandlerT>
			AsyncResultPtr AsyncConnect(const IPAddress &addr, u_short port, const HandlerT &handler)
			{
				return impl_->AsyncConnect(addr, port, handler);
			}
			const AsyncResultPtr &AsyncConnect(const AsyncResultPtr &result, const IPAddress &addr, u_short uPort)
			{
				return impl_->AsyncConnect(result, addr, uPort);
			}

			// 异步断开链接
			template<typename HandlerT>
			AsyncResultPtr AsyncDisconnect(const HandlerT &handler, bool reuse = true)
			{
				return impl_->AsyncDisconnect(handler, reuse);
			}
			const AsyncResultPtr &AsyncDisconnect(const AsyncResultPtr &result, bool reuse = true)
			{
				return impl_->AsyncDisconnect(result, reuse);
			}

			// 阻塞式发送数据直到数据发送成功或出错
			template<typename ConstBufferT>
			size_t Write(const ConstBufferT &buffer)
			{
				return impl_->Write(buffer.data(), buffer.size(), 0);
			}
			template<typename ConstBufferT>
			size_t Write(const ConstBufferT &buffer, DWORD flag)
			{
				return impl_->Write(buffer.data(), buffer.size(), flag);
			}

			// 异步发送数据
			template<typename ConstBufferT, typename HandlerT>
			AsyncResultPtr AsyncWrite(const ConstBufferT &buffer, const HandlerT &callback)
			{
				return impl_->AsyncWrite(buffer.data(), buffer.size(), callback);
			}

			template<typename ConstBufferT>
			const AsyncResultPtr &AsyncWrite(const AsyncResultPtr &result, const ConstBufferT &buffer)
			{
				return impl_->AsyncWrite(result, buffer.data(), buffer.size());
			}
			
			size_t EndWrite(const AsyncResultPtr &result)
			{
				return impl_->EndWrite(result);
			}


			// 阻塞式接收数据直到成功或出错
			template<typename MutableBufferT>
			size_t Read(MutableBufferT &buffer)
			{
				return impl_->Read(buffer.data(), buffer.size(), 0);
			}
			template<typename MutableBufferT>
			size_t Read(MutableBufferT &buffer, u_long flag)
			{
				return impl_->Read(buffer.data(), buffer.size(), flag);
			}
	

			// 异步接收数据
			template<typename MutableBufferT, typename HandlerT>
			AsyncResultPtr AsyncRead(MutableBufferT &buffer, const HandlerT &callback)
			{
				return impl_->AsyncRead(buffer.data(), buffer.size(), callback);
			}

			template<typename MutableBufferT>
			const AsyncResultPtr &AsyncRead(const AsyncResultPtr &result, MutableBufferT &buffer)
			{
				return impl_->AsyncRead(result, buffer.data(), buffer.size());
			}

			size_t EndRead(const AsyncResultPtr &result)
			{
				return impl_->EndRead(result);
			}
			
		};
	}
}





#endif