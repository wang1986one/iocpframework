#ifndef __TIMER_BASIC_TIMER_HPP
#define __TIMER_BASIC_TIMER_HPP

#include "../../iocp/Dispatcher.hpp"

#include "TimerService.hpp"


namespace async
{
	namespace timer
	{
		namespace impl
		{
		
			// ---------------------------------------
			// class BasicTimer

			template<typename ImplT>
			class BasicTimer
				: public async::iocp::Object
			{
				typedef TimerService<ImplT>						TimerServiceType;
				typedef typename TimerServiceType::TimerPointer	TimerPointer;

			private:
				// Timer指针
				TimerPointer timer_;
				async::iocp::IODispatcher &io_;

			public:
				// 不接受回调函数，且注册一个Timer
				// period为时间间隔
				BasicTimer(async::iocp::IODispatcher &io, long period)
					: io_(io)
					, timer_(TimerServiceType::GetInstance(io).AddTimer(period, NULL))
				{}
				// 接受回调函数，且注册一个Timer
				template<typename HandlerT>
				BasicTimer(async::iocp::IODispatcher &io, long period, const HandlerT &handler)
					: io_(io)
					, timer_(TimerServiceType::GetInstance(io).AddTimer(period, handler))
				{}
				~BasicTimer()
				{}

			private:
				BasicTimer(const BasicTimer &);
				BasicTimer &operator=(const BasicTimer &);

			public:
				// 设置时间间隔
				// period 时间间隔
				// delay 延迟时间
				void SetTimer(long period, long delay = 0)
				{
					timer_->SetTimer(period);
				}

				// 取消Timer
				void Cancel()
				{
					timer_->Cancel();
				}

				// 同步等待
				void Wait()
				{
					timer_->SyncWait();
				}

				// 异步等待
				void BeginWait()
				{
					timer_->AsyncWait();
				}

				template<typename HandlerT>
				void BeginWait(const HandlerT &handler)
				{
					TimerServiceType::GetInstance(io_).ChangeCallback(timer_, handler);
					timer_->AsyncWait();
				}
				
				void ExpireseAt()
				{
					
				}

			private:
				// 投递给IOCP进行分派
				void _Callback(const async::iocp::AsyncResultPtr &result)
				{
					io_.Post(result);
				}
			};
		}
	}
}


#endif