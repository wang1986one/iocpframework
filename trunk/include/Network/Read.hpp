#ifndef __NETWORK_READ_HELPER_HPP
#define __NETWORK_READ_HELPER_HPP


#include "../iocp/internal/Condition.hpp"


namespace async
{
	namespace network
	{

		template<typename SyncWriteStreamT, typename MutableBufferT>
		size_t Read(SyncWriteStreamT &s, MutableBufferT &buffer)
		{
			return Read(s, buffer, iocp::TransferAll());
		}

		// 
		template<typename SyncWriteStreamT, typename MutableBufferT, typename CompleteConditionT>
		size_t Read(SyncWriteStreamT &s, MutableBufferT &buffer, CompleteConditionT &condition)
		{
			size_t transfers = 0;
			size_t totalTransfer = condition(transfers);
			size_t bufSize = buffer.size();

			while( transfers <= totalTransfer )
			{
				if( transfers >= bufSize )
					break;

				size_t ret = s.Read(buffer + transfers);	
				transfers += ret;
			}

			return transfers;
		}



		namespace detail
		{
			template<typename AsyncWriteStreamT, typename MutableBufferT, typename CompletionConditionT, typename ReadHandlerT>
			class ReadHandler
			{
				typedef ReadHandler<AsyncWriteStreamT, MutableBufferT, CompletionConditionT, ReadHandlerT>	ThisType;
				
			private:
				AsyncWriteStreamT &stream_;
				MutableBufferT buffer_;
				CompletionConditionT condition_;
				size_t transfers_;
				size_t total_;
				ReadHandlerT handler_;

			public:
				ReadHandler(AsyncWriteStreamT &stream, MutableBufferT &buffer, const CompletionConditionT &condition, size_t transfer, const ReadHandlerT &handler)
					: stream_(stream)
					, buffer_(buffer)
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
							stream_.AsyncRead(buffer_ + size, 
								ThisType(stream_, buffer_ + size, condition_, transfers_, handler_));

							return;
						}
					}
					
					// �ص�
					iocp::HandlerInvoke::Invoke(handler_, transfers_, error);
				}
			};
		}

		// �첽��ȡָ��������

		template<typename SyncWriteStreamT, typename MutableBufferT, typename HandlerT>
		void AsyncRead(SyncWriteStreamT &s, MutableBufferT &buffer, const HandlerT &handler)
		{
			AsyncRead(s, buffer, iocp::TransferAll(), handler);
		}

		template<typename SyncWriteStreamT, typename MutableBufferT, typename ComplateConditionT, typename HandlerT>
		void AsyncRead(SyncWriteStreamT &s, MutableBufferT &buffer, const ComplateConditionT &condition, const HandlerT &handler)
		{
			typedef detail::ReadHandler<SyncWriteStreamT, MutableBufferT, ComplateConditionT, HandlerT> HookReadHandler;

			s.AsyncRead(buffer, HookReadHandler(s, buffer, condition, 0, handler));
		}


	}
}







#endif