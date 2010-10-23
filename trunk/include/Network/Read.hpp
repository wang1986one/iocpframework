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



		namespace internal
		{
			template<typename AsyncWriteStreamT, typename MutableBufferT, typename CompletionConditionT, typename ReadHandlerT>
			class ReadHandler
			{
				typedef ReadHandler<AsyncWriteStreamT, MutableBufferT, CompletionConditionT, ReadHandlerT>	ThisType;
				
			private:
				AsyncWriteStreamT &stream_;
				MutableBufferT &buffer_;
				CompletionConditionT condition_;
				size_t transfers_;
				size_t total_;
				ReadHandlerT handler_;

			public:
				ReadHandler(AsyncWriteStreamT &stream, MutableBufferT &buffer, const CompletionConditionT &condition, const ReadHandlerT &handler)
					: stream_(stream)
					, buffer_(buffer)
					, condition_(condition)
					, transfers_(0)
					, total_(buffer.size())
					, handler_(handler)
				{}

				void operator()(u_long size, u_long error)
				{
					transfers_ += size; 

					if( transfers_ < total_ && size != 0 && error == 0 )
					{
						if( transfers_ <= condition_(transfers_) )
						{
							using namespace std::tr1::placeholders;

							stream_.AsyncRead(Buffer(buffer_ + transfers_), 
								std::tr1::bind(&ThisType::operator(), this, _1, _2));

							return;
						}
					}
					
					// 回调
					iocp::HandlerInvoke::Invoke(handler_, transfers_, error);
					iocp::internal::HandlerDealloc(this);
				}
			};
		}

		// 异步读取指定的数据

		template<typename SyncWriteStreamT, typename MutableBufferT, typename HandlerT>
		void AsyncRead(SyncWriteStreamT &s, MutableBufferT &buffer, const HandlerT &handler)
		{
			AsyncRead(s, buffer, iocp::TransferAll(), handler);
		}

		template<typename SyncWriteStreamT, typename MutableBufferT, typename ComplateConditionT, typename HandlerT>
		void AsyncRead(SyncWriteStreamT &s, MutableBufferT &buffer, const ComplateConditionT &condition, const HandlerT &handler)
		{
			typedef internal::ReadHandler<SyncWriteStreamT, MutableBufferT, ComplateConditionT, HandlerT> HookReadHandler;

			//HookReadHandler *hook = iocp::internal::HandlerAlloc<HookReadHandler>(s, buffer, condition, handler);
			
			using namespace std::tr1::placeholders;
			s.AsyncRead(buffer, HookReadHandler(s, buffer, condition, handler));
				//std::tr1::bind(&HookReadHandler::operator(), hook, _1, _2));
		}


	}
}







#endif