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
			explicit BasicStreamSocket(const ImplementType &impl)
				: impl_(impl)
			{}

			BasicStreamSocket(AsyncIODispatcherType &io, const ProtocolType &protocol)
				: impl_(new Socket(io))
			{
				impl_->Open(protocol.Type(), protocol.Protocol());
			}
			~BasicStreamSocket()
			{}

		public:
			ImplementType &Get() 
			{
				return impl_;
			}
			const ImplementType &Get() const
			{
				return impl_;
			}

			void Open(const ProtocolType &protocol = ProtocolType::V4())
			{
				impl_->Open(protocol.Type(), protocol.Protocol());
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
				AsyncConnect(addr, port, NULL);
			}

			AsyncResultPtr AsyncConnect(const IPAddress &addr, u_short port, const AsyncCallbackFunc &handler)
			{
				return impl_->BeginConnect(addr, port, handler);
			}

			AsyncResultPtr AsyncDisconnect(const AsyncCallbackFunc &handler, bool reuse = true)
			{
				return impl_->BeginDisconnect(reuse, handler);
			}

			// 阻塞式发送数据直到数据发送成功或出错
			template<typename ConstBufferT>
			size_t Send(const ConstBufferT &buffer)
			{
				return impl_->Send(buffer, 0);
			}
			template<typename ConstBufferT>
			size_t Send(const ConstBufferT &buffer, DWORD flag)
			{
				return impl_->Send(buffer, flag);
			}

			// 异步发送数据
			template<typename MutableBufferT>
			const AsyncResultPtr AsyncSend(MutableBufferT &buffer, size_t offset, size_t size, const AsyncCallbackFunc &callback)
			{
				return impl_->BeginSend(buffer, offset, size, callback, nothing, nothing);
			}
			
			size_t EndSend(const AsyncResultPtr &result)
			{
				return impl_->EndSend(result);
			}

			// 阻塞式接收数据直到成功或出错
			template<typename MutableBufferT>
			size_t Recv(const MutableBufferT &buffer)
			{
				return impl_->Recv(buffer, 0)
			}
			template<typename MutableBufferT>
			size_t Recv(const MutableBufferT &buffer, DWORD flag)
			{
				return impl_->Recv(buffer, 0)
			}

			// 异步接收数据
			template<typename MutableBufferT>
			AsyncResultPtr AsyncRecv(const MutableBufferT &buffer, size_t offset, size_t size, const AsyncCallbackFunc &callback)
			{
				return impl_->BeginRecv(buffer, offset, size, callback, nothing, nothing);
			}

			size_t EndRecv(const AsyncResultPtr &result)
			{
				return impl_->EndRecv(result);
			}
			

			//// 阻塞式写数据直到有数据返回或出错
			//template<typename ConstBufferT>
			//size_t WriteSome(const ConstBufferT &);

			//// 异步写部分数据
			//template<typename ConstBufferT, typename WriteHandlerT>
			//void AsyncWriteSome(const ConstBufferT &, WriteHandlerT);


			//// 阻塞式读取部分数据
			//template<typename MutableBufferT>
			//size_t ReadSome(const MutableBufferT &);

			//// 异步读取部分数据
			//template<typename MutableBufferT, typename ReadHandlerT>
			//void AsyncReadSome(const MutableBufferT &, ReadHandlerT);
		};
	}
}





#endif