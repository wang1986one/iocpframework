#ifndef __NETWORK_WRITE_HELPER_HPP
#define __NETWORK_WRITE_HELPER_HPP


#include "../iocp/internal/Condition.hpp"


namespace async
{
	namespace network
	{

		template<typename SyncWriteStreamT, typename ConstBufferT>
		size_t Write(SyncWriteStreamT &s, const ConstBufferT &buffer)
		{
			return Write(s, buffer, iocp::TransferAll());
		}

		// 
		template<typename SyncWriteStreamT, typename ConstBufferT, typename CompleteConditionT>
		size_t Write(SyncWriteStreamT &s, const ConstBufferT &buffer, const CompleteConditionT &condition)
		{
			size_t transfers = 0;
			size_t totalTransfer = condition(transfers);
			size_t bufSize = buffer.size();

			while( transfers <= totalTransfer )
			{
				if( transfers >= bufSize )
					break;

				size_t ret = s.Write(buffer + transfers);	
				transfers += ret;
			}

			return transfers;
		}



		namespace detail
		{
			template<typename AsyncWriteStreamT, typename ConstBufferT, typename CompletionConditionT, typename WriteHandlerT>
			class WriteHandler
			{
				typedef WriteHandler<AsyncWriteStreamT, ConstBufferT, CompletionConditionT, WriteHandlerT>	ThisType;

			private:
				AsyncWriteStreamT &stream_;
				ConstBufferT buffer_;
				CompletionConditionT condition_;
				size_t transfers_;
				size_t total_;
				WriteHandlerT handler_;

			public:
				WriteHandler(AsyncWriteStreamT &stream, const ConstBufferT &buffer, const CompletionConditionT &condition, size_t transfer, const WriteHandlerT &handler)
					: stream_(stream)
					, buffer_(buffer_)
					, condition_(condition)
					, transfers_(transfer)
					, total_(buffer.size())
					, handler_(handler)
				{}

				void operator()(u_long size, u_long error)
				{
					transfers_ += size;

					if( transfers_ < total_ && size != 0 && error == 0 )
					{
						if( transfers_ <= condition_() )
						{
							stream_.AsyncWrite(buffer_ + size, 
								ThisType(stream_, buffer_ + size, condition_, transfers_, handler_));

							return;
						}
					}
					
					// 回调
					iocp::HandlerInvoke::Invoke(handler_, transfers_, error);
				}
			};
		}

		// 异步读取指定的数据

		template<typename SyncWriteStreamT, typename ConstBufferT, typename HandlerT>
		void AsyncWrite(SyncWriteStreamT &s, const ConstBufferT &buffer, const HandlerT &handler)
		{
			AsyncWrite(s, buffer, iocp::TransferAll(), handler);
		}

		template<typename SyncWriteStreamT, typename ConstBufferT, typename ComplateConditionT, typename HandlerT>
		void AsyncWrite(SyncWriteStreamT &s, const ConstBufferT &buffer, const ComplateConditionT &condition, const HandlerT &handler)
		{
			typedef detail::WriteHandler<SyncWriteStreamT, ConstBufferT, ComplateConditionT, HandlerT> HookWriteHandler;

			s.AsyncWrite(buffer, HookWriteHandler(s, buffer, condition, 0, handler));
		}


	}
}







#endif