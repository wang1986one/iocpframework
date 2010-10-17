#ifndef __NETWORK_WRITE_HELPER_HPP
#define __NETWORK_WRITE_HELPER_HPP


#include "../iocp/internal/Condition.hpp"
#include "../iocp/internal/HandlerAlloc.hpp"


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
		size_t Write(SyncWriteStreamT &s, const ConstBufferT &buffer, CompleteConditionT &condition)
		{
			size_t transfers = 0;
			size_t totalTransfer = condition(transfers);
			size_t bufSize = buffer->size();

			while( transfers <= totalTransfer )
			{
				if( transfers >= bufSize )
					break;

				size_t ret = s.Write(buffer, transfers, 0);	
				transfers += ret;
			}

			return transfers;
		}



		namespace internal
		{
			template<typename AsyncWriteStreamT, typename ConstBufferT, typename CompletionConditionT, typename WriteHandlerT>
			class WriteHandler
			{
				typedef WriteHandler<AsyncWriteStreamT, ConstBufferT, CompletionConditionT, WriteHandlerT>	ThisType;
				//typedef typename AsyncWriteStreamT::ImplementType											ImplementType;

			private:
				AsyncWriteStreamT &stream_;
				ConstBufferT &buffer_;
				CompletionConditionT condition_;
				size_t transfers_;
				size_t total_;
				WriteHandlerT handler_;

			public:
				WriteHandler(AsyncWriteStreamT &stream, ConstBufferT &buffer, const CompletionConditionT &condition, const WriteHandlerT &handler)
					: stream_(stream)
					, buffer_(buffer_)
					, condition_(condition)
					, transfers_(0)
					, total_(buffer->size())
					, handler_(handler)
				{}

				void operator()(const AsyncResultPtr &result, u_long size)
				{
					transfers_ += size; //result->EndAsync(stream_->GetHandle());

					if( transfers_ < total_ && size != 0)
					{
						if( transfers_ <= condition_(transfers_) )
						{
							using std::tr1::placeholders::_1;
							using std::tr1::placeholders::_2;


							static AsyncCallbackFunc callback = std::tr1::bind(&ThisType::operator(), this, _1, _2);
							stream_->AsyncWrite(buffer_, transfers_, total_ - transfers_, callback);

							return;
						}
					}
					
					// call user callback
					if( size == 0 )
						transfers_ = 0;

					iocp::HandlerInvoke::Invoke(handler_, result, transfers_);
					iocp::internal::HandlerDealloc(this);
				}
			};
		}

		// �첽��ȡָ��������

		template<typename SyncWriteStreamT, typename ConstBufferT, typename HandlerT>
		void AsyncWrite(SyncWriteStreamT &s, ConstBufferT &buffer, const HandlerT &handler)
		{
			AsyncWrite(s, buffer, iocp::TransferAll(), handler);
		}

		template<typename SyncWriteStreamT, typename ConstBufferT, typename ComplateConditionT, typename HandlerT>
		void AsyncWrite(SyncWriteStreamT &s, ConstBufferT &buffer, const ComplateConditionT &condition, const HandlerT &handler)
		{
			typedef internal::ReadHandler<SyncWriteStreamT, ConstBufferT, ComplateConditionT, HandlerT> HookReadHandler;
			HookReadHandler *hook = iocp::internal::HandlerAlloc<HookReadHandler>(s, buffer, condition, handler);

			s->AsyncWrite(buffer, 0, buffer->size(),
				std::tr1::bind(&HookReadHandler::operator(), hook, 
				std::tr1::placeholders::_1, std::tr1::placeholders::_2));
		}


	}
}







#endif