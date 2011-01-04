#ifndef __THREAD_OVERLAPPEDIO_THREAD_POOL_HPP
#define __THREAD_OVERLAPPEDIO_THREAD_POOL_HPP

#include <Comm/SmartPointer/SmartHandle.hpp>
#include <Comm/AsyncIO/IOCP/IOCP.hpp>
#include <Comm/Thread/Lock.hpp>
#include <map>


namespace DataStructure
{

	template<typename ImplT>
	class IWorker
	{
	public:
		template<typename RequestT, typename ParamT>
		void Execute(RequestT *p, const ParamT &param)
		{
			return _This()->Execute(p, param);
		}

		template<typename ParamT>
		bool Initialize(const ParamT &param)
		{
			return _This()->Initialize(param);
		}

		template<typename ParamT>
		bool Terminate(const ParamT &param)
		{
			return _This()->Terminate(param);
		}	
	

	private:
		ImplT *_This()
		{
			return static_cast<ImplT *>(this);
		}
	};

	#define POOL_SHUTDOWN ((OVERLAPPED*) ((__int64) -1))
	

	//-----------------------------------------------------------------
	// class OverlappedThreadPool

	template<
		typename WorkerT, 
		typename RequestT,							// 请求者类型
		typename ParamT			= LPVOID,			// 参数类型
		size_t NumThreads		= 0,				// 线程池中线程数量
		size_t StackSize		= 0,				// 每个线程默认堆栈大小
		typename ThreadTraitsT	= DefaultThreadTraits, 
		typename WaitTraitsT	= DefaultWaitTraits
		>
	class OverlappedThreadPool
	{
	public:
		typedef WorkerT							WorkerType;
		typedef RequestT*						RequestType;
		typedef ParamT							ParameterType;
		typedef ThreadTraitsT					ThreadTraitsType;
		typedef WaitTraitsT						WaitThreadTraitsType;
		
		// DWORD: ThreadID
		typedef std::map<DWORD, HANDLE>			Threads;
		typedef CAutoLock<AutoCriticalSection>	LockType;

		
		
	private:
		Threads	m_threads;
		AutoCriticalSection m_cs;
		AutoEvent m_hThreadEvent;
		async::IOCP::CIOCP m_hRequestQueue;

		DWORD m_dwStackSize;
		DWORD m_dwMaxWait;
		DWORD m_dwThreadEventID;

		LONG m_bShutdown;				// InterLockedExchange返回值为LONG
		ParameterType m_pWorkParam;
		
		
	
		// 线程池关闭等待时间
		enum { THREADPOOL_SHUTDOWN_TIMEOUT = 60 * 60 * 1000 };

		// 默认每个CPU支持线程个数
		enum { DEFAULT_THREADSPERPROC = 2 };

	public:
		OverlappedThreadPool()
			: m_dwStackSize(StackSize)
			, m_dwMaxWait(THREADPOOL_SHUTDOWN_TIMEOUT)
			, m_dwThreadEventID(0)
			, m_bShutdown(0)
			, m_pWorkParam(NULL)
		{}
		~OverlappedThreadPool()
		{
			Shutdown();
		}

	private:
		OverlappedThreadPool(const OverlappedThreadPool &);
		OverlappedThreadPool &operator=(const OverlappedThreadPool &);



	public:
		/*
			if NumThreads > 0, 则指定线程数量
			if NumThreads < 0, 则需要取相反数
			if NumThreads == 0, 则为每个CPU指定两个线程
		*/
		bool Initialize(ParameterType pWorkerParam = NULL)
		{
			assert(m_hRequestQueue == NULL);
			if( m_hRequestQueue != NULL )
				return false;
			
			m_hThreadEvent.Create();
			if( !m_hThreadEvent.IsOpen() )
				return false;

			// 创建完成端口到请求队列
			m_hRequestQueue.Create(NumThreads);
			if( !m_hRequestQueue.IsOpen() )
				return false;

			m_pWorkParam = pWorkerParam;

			return SetSize(NumThreads);
		}


		// 关闭线程池中所有请求
		void Shutdown(DWORD dwMaxWait = 0)
		{
			if( !m_hRequestQueue.IsOpen() )
				return;

			// Lock
			{
				LockType lock(m_cs);


				if( dwMaxWait == 0 )
					dwMaxWait = m_dwMaxWait;

				bool bSuc = _ResizePool(0, dwMaxWait);
				assert(bSuc);

				for(Threads::iterator iter = m_threads.begin(); 
					iter != m_threads.end(); ++iter)
				{
					HANDLE hThread = iter->second;
					DWORD dwExitCode = 0;

					::GetExitCodeThread(hThread, &dwExitCode);
					if( dwExitCode == STILL_ACTIVE )
					{
						// 从设计上故意使用此方式终止线程
						::TerminateThread(hThread, 0);
					}

					::CloseHandle(hThread);
				}
			}
		}

		bool QueueRequest(RequestType request)
		{
			if( !m_hRequestQueue.IsOpen() )
				return false;

			return m_hRequestQueue.PostStatus(reinterpret_cast<ULONG_PTR>(request));
		}


	public:
		bool SetSize(DWORD dwNumThreads)
		{
			if( dwNumThreads == 0 )
				dwNumThreads = DEFAULT_THREADSPERPROC;

			SYSTEM_INFO info = {0};
			::GetSystemInfo(&info);
			dwNumThreads = dwNumThreads * info.dwNumberOfProcessors;
			

			return _ResizePool(dwNumThreads, m_dwMaxWait);
		}
		DWORD GetSize() const
		{
			return GetNumThreads();
		}
		
		void SetTimeout(DWORD dwMaxWait)
		{
			m_dwMaxWait = dwMaxWait;
		}
		DWORD GetTimeout() const
		{
			return m_dwMaxWait;
		}

		HANDLE GetQueueHandle() const
		{
			return m_hRequestQueue;
		}
	
		DWORD GetNumThreads() const
		{
			m_threads.size();
		}
	
		

	private:
		DWORD _ThreadProc()
		{
			
			// 确保worker获取的数据在线程关闭之前
			{
				// 在线程栈上创建一个对象，保证其在线程生命期内有效
				IWorker<WorkerType> worker;
				if( worker.Initialize(m_pWorkParam) == false )
					return 1;

				m_hThreadEvent.SetEvent();
				

				DWORD dwBytesTransfer		= 0;
				ULONG_PTR uCompletionKey	= 0;
				OVERLAPPED *pOverlapped		= NULL;

				// 从IO Completion Port上获取请求
				while(m_hRequestQueue.GetStatus(&uCompletionKey, &dwBytesTransfer, &pOverlapped))
				{
					if( pOverlapped == POOL_SHUTDOWN )
					{
						// Shutdown
						LONG bRes = ::InterlockedExchange(&m_bShutdown, FALSE);
						if( bRes )
							break;
						// else, 取消了Shutdown，继续
					}
					else
					{
						RequestType request = reinterpret_cast<RequestType>(uCompletionKey);

						// 处理该请求，需要注意：
						// 1. 只有请求处理完了才能释放内存
						// 2. 如果请求还需要处理，则需要再次分派
						worker.Execute(request, m_pWorkParam);
					}
				}

				worker.Terminate(m_pWorkParam);
			}

			m_dwThreadEventID = ::GetCurrentThreadId();
			m_hThreadEvent.SetEvent();

			return 0;
		}

		bool _ResizePool(DWORD dwNumThreads, DWORD dwMaxWait)
		{
			if( !m_hRequestQueue.IsOpen() )
				return false;

			// Lock
			LockType lock(m_cs);
			
			size_t nCurThreads = m_threads.size();
			if( dwNumThreads == nCurThreads )
				return true;
			else if( dwNumThreads < nCurThreads )
			{
				DWORD dwShutdownThreads = nCurThreads - dwNumThreads;
				for(DWORD i = 0; i != dwShutdownThreads; ++i)
				{
					m_hThreadEvent.ResetEvent();

					::InterlockedExchange(&m_bShutdown, TRUE);
					m_hRequestQueue.PostStatus(0, 0, POOL_SHUTDOWN);
					DWORD dwRet = WaitThreadTraitsType::WaitForSingleObject(m_hThreadEvent, dwMaxWait);

					if( dwRet == WAIT_TIMEOUT )
					{
						LONG bRes = ::InterlockedExchange(&m_bShutdown, FALSE);
						if( bRes )
						{
							// 不需要shutdown
							return false;
						}
					}
					else if( dwRet != WAIT_OBJECT_0 )
					{
						return false;
					}

					
					Threads::iterator iter = m_threads.find(m_dwThreadEventID); 
					if( iter != m_threads.end() )
					{
						HANDLE hThread = iter->second;

						// Wait for Shutdown
						if( WaitThreadTraitsType::WaitForSingleObject(hThread, 60000) == WAIT_OBJECT_0 )
						{
							::CloseHandle(hThread);
							m_threads.erase(iter);
						}
						else
						{
							// 关闭Thread失败
							return false;
						}
					}

				}
			}
			else	// dwNumThreads > nCurThreads
			{
				DWORD dwNumNewThreads = dwNumThreads - nCurThreads;

				// 创建新线程
				for(DWORD i = 0; i != dwNumNewThreads; ++i)
				{
					DWORD dwThreadID = 0;
					m_hThreadEvent.ResetEvent();

					CAutoHandle hThread(ThreadTraitsType::CreateThread(NULL, m_dwStackSize, _WorkerThreadProc, this, 0, &dwThreadID));

					if( !hThread.IsValid() )
						return false;

					DWORD dwRet = WaitThreadTraitsType::WaitForSingleObject(m_hThreadEvent, dwMaxWait);
					if( dwRet != WAIT_OBJECT_0 )
					{
						if( dwRet == WAIT_TIMEOUT )
							return false;
						else
							return false;
					}

					m_threads.insert(std::make_pair(dwThreadID, hThread.Detach()));
				}
			}

			return true;
		}

	private:
		static DWORD WINAPI _WorkerThreadProc(LPVOID pParam)
		{
			OverlappedThreadPool *pThis = reinterpret_cast<OverlappedThreadPool *>(pParam);

			return pThis->_ThreadProc();
		}

	};

}


#endif