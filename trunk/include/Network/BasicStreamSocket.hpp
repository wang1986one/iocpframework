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
				: impl_(new Socket(io))
			{}
			BasicStreamSocket(AsyncIODispatcherType &io, const ImplementType &impl)
				: impl_(impl)
			{}

			BasicStreamSocket(AsyncIODispatcherType &io, const ProtocolType &protocol)
				: impl_(new Socket(io))
			{
				impl_->Open(protocol.Family(), protocol.Type(), protocol.Protocol());
			}
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
				impl_->Open(protocol.family(), protocol.Type(), protocol.Protocol());
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

			void Bind(u_short port, const IPAddress &addr)
			{
				impl_->Bind(ProtocolType::V4().Family(), port, addr);
			}


			// 连接远程服务
			void Connect(const IPAddress &addr, u_short port)
			{
				impl_->Connect(ProtocolType::V4().Family(), addr, port);
			}

			void DisConnect(int shut = SD_BOTH)
			{
				impl_->DisConnect(shut, true);
			}

			template<typename HandlerT>
			AsyncResultPtr AsyncConnect(const IPAddress &addr, u_short port, const HandlerT &handler)
			{
				return impl_->AsyncConnect(addr, port, handler);
			}

			template<typename HandlerT>
			AsyncResultPtr AsyncDisconnect(const HandlerT &handler, bool reuse = true)
			{
				return impl_->AsyncDisconnect(handler, reuse);
			}

			// 阻塞式发送数据直到数据发送成功或出错
			template<typename ConstBufferT>
			size_t Write(const ConstBufferT &buffer)
			{
				return impl_->Write(buffer, 0, 0);
			}
			template<typename ConstBufferT>
			size_t Write(const ConstBufferT &buffer, DWORD flag)
			{
				return impl_->Write(buffer, 0, flag);
			}
			template<typename ConstBufferT>
			size_t Write(const ConstBufferT &buffer, size_t offset, DWORD flag)
			{
				return impl_->Write(buffer, offset, flag);
			}

			// 异步发送数据
			template<typename ConstBufferT, typename HandlerT>
			const AsyncResultPtr AsyncWrite(const ConstBufferT &buffer, const HandlerT &callback)
			{
				return impl_->AsyncWrite(buffer.data(), buffer.size(), callback);
			}
			/*template<typename MutableBufferT>
			const AsyncResultPtr AsyncWrite(MutableBufferT &buffer, size_t offset, size_t size, const AsyncCallbackFunc &callback)
			{
				return impl_->AsyncWrite(buffer, offset, size, callback);
			}*/
			
			size_t EndWrite(const AsyncResultPtr &result)
			{
				return impl_->EndWrite(result);
			}

			// 阻塞式接收数据直到成功或出错
			template<typename MutableBufferT>
			size_t Read(MutableBufferT &buffer)
			{
				return impl_->Read(buffer, 0, 0);
			}
			template<typename MutableBufferT>
			size_t Read(MutableBufferT &buffer, u_long flag)
			{
				return impl_->Read(buffer, 0, flag);
			}
			template<typename MutableBufferT>
			size_t Read(MutableBufferT &buffer, size_t offset, u_long flag)
			{
				return impl_->Read(buffer, offset, flag);
			}

			// 异步接收数据
			template<typename MutableBufferT, typename HandlerT>
			AsyncResultPtr AsyncRead(MutableBufferT &buffer, const HandlerT &callback)
			{
				return impl_->AsyncRead(buffer.data(), buffer.size(), callback);
			}
			/*template<typename MutableBufferT>
			AsyncResultPtr AsyncRead(const MutableBufferT &buffer, size_t offset, size_t size, const AsyncCallbackFunc &callback)
			{
				return impl_->AsyncRead(buffer, offset, size, callback);
			}*/

			size_t EndRead(const AsyncResultPtr &result)
			{
				return impl_->EndRead(result);
			}
			
		};
	}
}





#endif