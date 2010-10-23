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
		size_t Write(SyncWriteStreamT &s, const ConstBufferT &buffer, CompleteConditionT &condition)
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



		namespace internal
		{
			template<typename AsyncWriteStreamT, typename ConstBufferT, typename CompletionConditionT, typename WriteHandlerT>
			class WriteHandler
			{
				typedef WriteHandler<AsyncWriteStreamT, ConstBufferT, CompletionConditionT, WriteHandlerT>	ThisType;
				//typedef typename AsyncWriteStreamT::ImplementType											ImplementType;

			private:
				AsyncWriteStreamT &stream_;
				const ConstBufferT &buffer_;
				CompletionConditionT condition_;
				size_t transfers_;
				size_t total_;
				WriteHandlerT handler_;

			public:
				WriteHandler(AsyncWriteStreamT &stream, const ConstBufferT &buffer, const CompletionConditionT &condition, const WriteHandlerT &handler)
					: stream_(stream)
					, buffer_(buffer_)
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

							//static WriteHandlerT callback = std::tr1::bind(&ThisType::operator(), this, _1, _2, _3);
							stream_.AsyncWrite(Buffer(buffer_ + transfers_), 
								std::tr1::bind(&ThisType::operator(), this, _1, _2));

							return;
						}
					}
					
					// 回调
					iocp::HandlerInvoke::Invoke(handler_, transfers_, error);
					//iocp::internal::HandlerDealloc(this);
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
			typedef internal::WriteHandler<SyncWriteStreamT, ConstBufferT, ComplateConditionT, HandlerT> HookWriteHandler;
			//HookWriteHandler *hook = iocp::internal::HandlerAlloc<HookWriteHandler>(s, buffer, condition, handler);

			using namespace std::tr1::placeholders;
			s.AsyncWrite(buffer, HookWriteHandler(s, buffer, condition, handler));
				//std::tr1::bind(&HookWriteHandler::operator(), hook, _1, _2));
		}


	}
}







#endif