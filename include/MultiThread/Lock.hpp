#ifndef __THREAD_AUTO_LOCK_HPP
#define __THREAD_AUTO_LOCK_HPP

#include <cassert>

namespace async
{

	namespace thread
	{


		//-------------------------------------------------------
		// CAutoNull

		class AutoNull
		{
		public:
			void Lock(){}
			void Unlock(){}
		};



		//-------------------------------------------------------
		// CAutoSpinLock

		class AutoSpinLock
		{
		private:
			CRITICAL_SECTION m_sec;

		public:
			AutoSpinLock(u_long dwCount = 4000) 
			{
				::InitializeCriticalSectionAndSpinCount(&m_sec, dwCount);	// MS 
			}

			~AutoSpinLock() 
			{
				::DeleteCriticalSection(&m_sec);
			}

		public:
			void Lock() 
			{
				::EnterCriticalSection(&m_sec);
			}

			void Unlock() 
			{
				::LeaveCriticalSection(&m_sec);
			}

			CRITICAL_SECTION &operator()()
			{
				return m_sec;
			}
		};

		//-------------------------------------------------------
		// CAutoCriticalSection

		class AutoCriticalSection
		{
		private:
			CRITICAL_SECTION m_sec;

		public:
			AutoCriticalSection() 
			{
				::InitializeCriticalSection(&m_sec);
			}

			~AutoCriticalSection() 
			{
				::DeleteCriticalSection(&m_sec);
			}

		public:
			void Lock() 
			{
				::EnterCriticalSection(&m_sec);
			}

			void Unlock() 
			{
				::LeaveCriticalSection(&m_sec);
			}

			CRITICAL_SECTION &operator()()
			{
				return m_sec;
			}
		};


		//-------------------------------------------------------
		// CAutoMutex

		class AutoMutex
		{
		private:
			HANDLE m_hMutex;

		public:
			AutoMutex()
				: m_hMutex(NULL)
			{
				m_hMutex = ::CreateMutex(NULL, FALSE, NULL);
			}
			~AutoMutex()
			{
				if( m_hMutex != NULL )
					::CloseHandle(m_hMutex);
			}

		public:
			void Lock()
			{
				::WaitForSingleObject(m_hMutex, INFINITE);
			}

			void UnLock()
			{
				::ReleaseMutex(m_hMutex);
			}

			HANDLE &operator()()
			{
				return m_hMutex;
			}
		};

		//-------------------------------------------------------
		// CAutoEvent

		class AutoEvent
		{
		public:
			HANDLE m_hEvent;

			AutoEvent(HANDLE hEvent = INVALID_HANDLE_VALUE) : m_hEvent(hEvent)
			{ 
			}

			~AutoEvent()
			{
				Close();
			}

			BOOL Create(LPCTSTR pstrName = NULL, BOOL bManualReset = FALSE, BOOL bInitialState = FALSE, LPSECURITY_ATTRIBUTES pEventAttributes = NULL)
			{
				assert(pstrName==NULL || !::IsBadStringPtr(pstrName,(UINT)-1));
				assert(m_hEvent==INVALID_HANDLE_VALUE);

				m_hEvent = ::CreateEvent(pEventAttributes, bManualReset, bInitialState, pstrName);
				assert(m_hEvent!=INVALID_HANDLE_VALUE);

				return m_hEvent != INVALID_HANDLE_VALUE;
			}

			BOOL Open(LPCTSTR pstrName, DWORD dwDesiredAccess = EVENT_ALL_ACCESS, BOOL bInheritHandle = TRUE)
			{
				assert(!::IsBadStringPtr(pstrName,(UINT)-1));
				assert(m_hEvent==INVALID_HANDLE_VALUE);

				m_hEvent = ::OpenEvent(dwDesiredAccess, bInheritHandle, pstrName);
				assert(m_hEvent!=INVALID_HANDLE_VALUE);

				return m_hEvent != INVALID_HANDLE_VALUE;
			}

			BOOL IsOpen() const
			{
				return m_hEvent != INVALID_HANDLE_VALUE;
			}

			void Close()
			{
				if( m_hEvent == INVALID_HANDLE_VALUE ) 
					return;

				::CloseHandle(m_hEvent);
				m_hEvent = INVALID_HANDLE_VALUE;
			}

			void Attach(HANDLE hEvent)
			{
				assert(m_hEvent==INVALID_HANDLE_VALUE);
				m_hEvent= hEvent;
			}  

			HANDLE Detach()
			{
				HANDLE hEvent = m_hEvent;
				m_hEvent = INVALID_HANDLE_VALUE;
				return hEvent;
			}

			BOOL ResetEvent()
			{
				assert(m_hEvent!=INVALID_HANDLE_VALUE);
				return ::ResetEvent(m_hEvent);
			}

			BOOL SetEvent()
			{
				assert(m_hEvent!=INVALID_HANDLE_VALUE);
				return ::SetEvent(m_hEvent);
			}

			BOOL PulseEvent()
			{
				assert(m_hEvent!=INVALID_HANDLE_VALUE);
				return ::PulseEvent(m_hEvent);
			}

			BOOL IsSignalled() const
			{
				assert(m_hEvent!=INVALID_HANDLE_VALUE);
				return ::WaitForSingleObject(m_hEvent, 0) == WAIT_OBJECT_0;
			}

			BOOL WaitForEvent(DWORD dwTimeout = INFINITE)
			{
				assert(m_hEvent!=INVALID_HANDLE_VALUE);
				return ::WaitForSingleObject(m_hEvent, dwTimeout) == WAIT_OBJECT_0;
			}

			operator HANDLE() const 
			{ 
				return m_hEvent; 
			}
		};



		//-------------------------------------------------------
		// CAutoLock

		template<typename T>
		class AutoLock
		{
		private:
			T &m_lock;

		public:
			AutoLock(T &lock)
				: m_lock(lock)
			{
				m_lock.Lock();
			}
			~AutoLock()
			{
				m_lock.Unlock();
			}

		private:
			AutoLock(const AutoLock &);
			AutoLock &operator=(const AutoLock &);
		};

	}

}

#endif