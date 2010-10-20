#ifndef __NETWORK_READ_HELPER_HPP
#define __NETWORK_READ_HELPER_HPP


#include "../iocp/internal/Condition.hpp"
#include "../iocp/internal/HandlerAlloc.hpp"


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
			size_t bufSize = buffer->allocSize();

			while( transfers <= totalTransfer )
			{
				if( transfers >= bufSize )
					break;

				size_t ret = s.Read(buffer, transfers, 0);	
				transfers += ret;
			}

			return transfers;
		}



		namespace internal
		{
			template<typename AsyncWriteStreamT, typename MutableBufferT, typename CompletionConditionT, typename WriteHandlerT>
			class ReadHandler
			{
				typedef ReadHandler<AsyncWriteStreamT, MutableBufferT, CompletionConditionT, WriteHandlerT>	ThisType;
				//typedef typename AsyncWriteStreamT::ImplementType											ImplementType;

			private:
				AsyncWriteStreamT &stream_;
				MutableBufferT &buffer_;
				CompletionConditionT condition_;
				size_t transfers_;
				size_t total_;
				WriteHandlerT handler_;

			public:
				ReadHandler(AsyncWriteStreamT &stream, MutableBufferT &buffer, const CompletionConditionT &condition, const WriteHandlerT &handler)
					: stream_(stream)
					, buffer_(buffer)
					, condition_(condition)
					, transfers_(0)
					, total_(buffer->allocSize())
					, handler_(handler)
				{}

				void operator()(const AsyncResultPtr &result, u_long size, u_long error)
				{
					transfers_ += size; 

					if( transfers_ < total_ && size != 0 && error == 0 )
					{
						if( transfers_ <= condition_(transfers_) )
						{
							using namespace std::tr1::placeholders;

							static AsyncCallbackFunc callback = std::tr1::bind(&ThisType::operator(), this, _1, _2, _3);
							stream_->AsyncRead(buffer_, transfers_, total_ - transfers_, callback);

							return;
						}
					}
					
					// 回调
					iocp::HandlerInvoke::Invoke(handler_, result, transfers_, error);
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

			HookReadHandler *hook = iocp::internal::HandlerAlloc<HookReadHandler>(s, buffer, condition, handler);
			
			s->AsyncRead(buffer, 0, buffer->allocSize(), 
				std::tr1::bind(&HookReadHandler::operator(), hook, 
				std::tr1::placeholders::_1, std::tr1::placeholders::_2, std::tr1::placeholders::_3));
		}


	}
}







#endif