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

			template<typename ImplT, typename ServiceT>
			class BasicTimer
			{
				typedef TimerService<ImplT, ServiceT>				TimerServiceType;
				typedef typename TimerServiceType::ServiceType		ServiceType;
				typedef typename TimerServiceType::TimerPointer		TimerPointer;

			private:
				TimerServiceType &service_;		// service
				TimerPointer timer_;			// Timerָ��
				long period_;
				long due_;

			public:
				// �����ܻص���������ע��һ��Timer
				// periodΪʱ����
				BasicTimer(ServiceType &io, long period, long due)
					: service_(TimerServiceType::GetInstance(io))
					, period_(period)
					, due_(due)
				{}
				// ���ܻص���������ע��һ��Timer
				template<typename HandlerT>
				BasicTimer(ServiceType &io, long period, long due, const HandlerT &handler)
					: service_(TimerServiceType::GetInstance(io))
					, timer_(service_.AddTimer(period, due, handler))
				{}
				~BasicTimer()
				{
					assert(timer_);
					service_.EraseTimer(timer_);
				}

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
					timer_ = service_.AddTimer(period_, due_, handler);
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