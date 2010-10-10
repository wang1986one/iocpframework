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
				// Timerָ��
				TimerPointer timer_;
				async::iocp::IODispatcher &io_;			// IO service

			public:
				// �����ܻص���������ע��һ��Timer
				// periodΪʱ����
				BasicTimer(async::iocp::IODispatcher &io, long period, long due)
					: io_(io)
					, timer_(TimerServiceType::GetInstance(io).AddTimer(period, due, NULL))
				{}
				// ���ܻص���������ע��һ��Timer
				template<typename HandlerT>
				BasicTimer(async::iocp::IODispatcher &io, long period, long due, const HandlerT &handler)
					: io_(io)
					, timer_(TimerServiceType::GetInstance(io).AddTimer(period, due, handler))
				{}
				~BasicTimer()
				{}

			private:
				BasicTimer(const BasicTimer &);
				BasicTimer &operator=(const BasicTimer &);

			public:
				// ����ʱ����
				// period ʱ����
				// delay �ӳ�ʱ��
				void SetTimer(long period, long delay = 0)
				{
					timer_->SetTimer(period, delay);
				}

				// ȡ��Timer
				void Cancel()
				{
					timer_->Cancel();
				}

				// ͬ���ȴ�
				/*void Wait()
				{
					timer_->SyncWait();
				}*/

				// �첽�ȴ�
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
			};
		}
	}
}


#endif