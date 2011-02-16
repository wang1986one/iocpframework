#ifndef __TIMER_TIMER_SERVICE_HPP
#define __TIMER_TIMER_SERVICE_HPP

#include <unordered_map>

#include "../../iocp/Dispatcher.hpp"

#include "../../MultiThread/Thread.hpp"




namespace async
{
	namespace timer
	{
		namespace impl
		{
			
			// alterable IO
			void WINAPI APCFunc(ULONG_PTR pParam)
			{
				// do nothing
			}

			// ------------------------------------------------
			// class TimerService

			template<typename TimerT, typename ServiceT>
			class TimerService
			{
			public:
				typedef TimerT							TimerType;
				typedef std::tr1::shared_ptr<TimerType>	TimerPointer;
				typedef ServiceT						ServiceType;

			private:
				typedef std::vector<std::pair<TimerPointer, iocp::AsyncCallbackBasePtr>>		TimersArray;
				typedef typename TimersArray::iterator	TimerArrayIter;
				

			private:
				TimersArray timerArray_;						// Array of Timer

				async::thread::ThreadImplEx thread_;			// WaitForMutipleObjectEx
				async::thread::AutoEvent   update_;	

				ServiceType &io_;								// Asynchronous Callback service

			private:
				TimerService(ServiceType &io)
					: io_(io)
				{
					update_.Create();

					// 启动等待Timer线程
					thread_.RegisterFunc(std::tr1::bind(&TimerService::_ThreadCallback, this));
					thread_.Start();
				}

				~TimerService()
				{
					// 可提醒IO，退出监听线程
					::QueueUserAPC(APCFunc, thread_, NULL);

					thread_.Stop();
				}

				TimerService(const TimerService &);
				TimerService &operator=(const TimerService &);
				

			public:
				// 单件
				static TimerService<TimerType, ServiceType> &GetInstance(ServiceType &io)
				{
					static TimerService<TimerType, ServiceType> service(io);
					return service;
				}

			public:
				// 增加一个Timer
				template<typename HandlerT>
				TimerPointer AddTimer(long period, long due, const HandlerT &handler)
				{
					TimerPointer timer(new TimerType(period, due));
					iocp::AsyncCallbackBasePtr callback(new async::iocp::AsyncCallback(handler));
					timerArray_.push_back(std::make_pair(timer, callback));
 
					// 设置更新事件信号
					update_.SetEvent();

					return timer;
				}

				void EraseTimer(const TimerPointer &timer)
				{
					struct FindTimer
					{
						const TimerPointer &timer_;

						FindTimer(const TimerPointer &timer)
							: timer_(timer)
						{}

						bool operator()(const std::pair<TimerPointer, iocp::AsyncCallbackBasePtr> &val) const
						{
							return val.first == timer_;
						}
					};
					TimerArrayIter iter = std::find_if(timerArray_.begin(), timerArray_.end(), FindTimer(timer));

					if( iter != timerArray_.end() )
						timerArray_.erase(iter);

					// 设置更新事件信号
					update_.SetEvent();
				}

			private:
				DWORD _ThreadCallback()
				{
					std::vector<HANDLE> handles;

					while(!thread_.IsAborted())
					{
						// 如果有变化，则重置
						if( WAIT_OBJECT_0 == ::WaitForSingleObject(update_, 0) )
						{
							_Copy(handles, timerArray_);
						}
						
						// 防止刚启动时没有timer生成
						if( handles.size() == 0 )
						{
							if( WAIT_IO_COMPLETION == ::WaitForSingleObjectEx(update_, INFINITE, TRUE) )
								break;
							else
								_Copy(handles, timerArray_);
						}

						// 等待Timer到点
						DWORD res = ::WaitForMultipleObjectsEx(handles.size(), &handles[0], FALSE, INFINITE, TRUE);
						if( res == WAIT_IO_COMPLETION )
							break;

						if( WAIT_OBJECT_0 == ::WaitForSingleObject(update_, 0) )
						{
							update_.SetEvent();
							continue;
						}
						
						if( res == WAIT_FAILED )
						{
							update_.SetEvent();
							continue;
						}
						else if( res + WAIT_OBJECT_0 > timerArray_.size() )
							throw std::logic_error("handle out of range");

						const TimersArray::value_type &val = timerArray_[WAIT_OBJECT_0 + res];
						io_.Post(val.second.Get());
					}

					::OutputDebugString(_T("Exit Timer Service Thread\n"));
					return 0;
				}

				void _Copy(std::vector<HANDLE> &handles, const TimersArray &timers)
				{
					const size_t count = timers.size();
					if( count > handles.size() )
						handles.resize(count);

					for(size_t i = 0; i != count; ++i)
					{
						handles[i] = *(timers[i].first);
					}
				}
			};

			
		}
	}
}


#endif