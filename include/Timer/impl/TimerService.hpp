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

			template<typename TimerT>
			class TimerService
			{
			public:
				typedef TimerT							TimerType;
				typedef std::tr1::shared_ptr<TimerType>	TimerPointer;

			private:
				typedef std::tr1::unordered_map<TimerPointer, async::iocp::AsyncResultPtr>	TimerMaps;
				typedef typename TimerMaps::iterator	TimerMapsIter;

				typedef std::vector<TimerPointer>		TimersArray;

			private:
				TimerMaps timerMaps_;			// Hash Table of Timer 
				TimersArray timerArray_;		// Array of Timer

				async::thread::ThreadImplEx thread_;	// WaitForMutipleObjectEx

				volatile long change_;			// Timer change
				async::iocp::IODispatcher &io_;	// IO service

			private:
				TimerService(async::iocp::IODispatcher &io)
					: io_(io)
				{
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
				static TimerService<TimerType> &GetInstance(async::iocp::IODispatcher &io)
				{
					static TimerService<TimerType> service(io);
					return service;
				}

			public:
				// 增加一个Timer
				TimerPointer AddTimer(long period, const async::iocp::AsyncCallbackFunc &handler)
				{
					using namespace async::iocp;
					TimerPointer timer(new TimerType(period));
					AsyncResultPtr result(new AsyncResult(nothing, nothing, nothing, nothing, handler));
			
					timerMaps_.insert(TimerMaps::value_type(timer, result));
					timerArray_.push_back(timer);

					::InterlockedExchange(&change_, 1);
					return timer;
				}

				// 设置Timer回调
				void ChangeCallback(const TimerPointer &timer, const async::iocp::AsyncCallbackFunc &handler)
				{
					TimerMaps::iterator iter = timerMaps_.find(timer);
					if( iter != timerMaps_.end() )
						iter->second->m_callback = handler;
					else
						throw std::logic_error("has no timer");
				}

			private:
				DWORD _ThreadCallback()
				{
					std::vector<HANDLE> handles;

					while(!thread_.IsAborted())
					{
						// 如果有变化，则重置
						if( ::InterlockedCompareExchange(&change_, 0, 1) == 1 )
						{
							handles.resize(timerArray_.size());
							_Copy(handles, timerArray_);
						}
						
						// 防止刚启动时没有timer生成
						if( handles.size() == 0 )
						{
							::Sleep(500);
							continue;
						}

						// 等待Timer到点
						DWORD res = ::WaitForMultipleObjectsEx(handles.size(), &handles[0], FALSE, INFINITE, TRUE);
						if( res == WAIT_FAILED )
							throw async::iocp::Win32Exception("WaitForSingleObjectEx");
						else if( res == WAIT_IO_COMPLETION )
							break;

						const TimerPointer &timer = timerArray_[WAIT_OBJECT_0 + res];
						TimerMapsIter iter = timerMaps_.find(timer);
						if( iter != timerMaps_.end() )
							io_.Post(iter->second);
					}

					::OutputDebugString(_T("Exit Timer Thread\n"));
					return 0;
				}

				void _Copy(std::vector<HANDLE> &handles, TimersArray &timers)
				{
					const size_t count = timers.size();
					for(size_t i = 0; i != count; ++i)
						handles[i] = *(timers[i]);
				}
			};

			
		}
	}
}


#endif