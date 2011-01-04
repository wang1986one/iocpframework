#ifndef __THREAD_AUTO_LOCK_HPP
#define __THREAD_AUTO_LOCK_HPP

#include <cassert>

namespace async
{

	namespace thread
	{


		//-------------------------------------------------------
		// AutoNull

		class AutoNull
		{
		public:
			void Lock(){}
			void Unlock(){}
		};



		//-------------------------------------------------------
		// AutoSpinLock

		class AutoSpinLock
		{
		private:
			CRITICAL_SECTION mutex_;

		public:
			AutoSpinLock(unsigned long dwCount = 4000) 
			{
				::InitializeCriticalSectionAndSpinCount(&mutex_, dwCount);	// MS 
			}

			~AutoSpinLock() 
			{
				::DeleteCriticalSection(&mutex_);
			}

		public:
			void Lock() 
			{
				::EnterCriticalSection(&mutex_);
			}

			void Unlock() 
			{
				::LeaveCriticalSection(&mutex_);
			}

			CRITICAL_SECTION &operator()()
			{
				return mutex_;
			}
		};

		//-------------------------------------------------------
		// AutoCriticalSection

		class AutoCriticalSection
		{
		private:
			CRITICAL_SECTION mutex_;

		public:
			AutoCriticalSection() 
			{
				::InitializeCriticalSection(&mutex_);
			}

			~AutoCriticalSection() 
			{
				::DeleteCriticalSection(&mutex_);
			}

		public:
			void Lock() 
			{
				::EnterCriticalSection(&mutex_);
			}

			void Unlock() 
			{
				::LeaveCriticalSection(&mutex_);
			}

			CRITICAL_SECTION &operator()()
			{
				return mutex_;
			}
		};


		//-------------------------------------------------------
		// class RWLock

		class RWLock
		{
		private:
			SRWLOCK rwLock_;

		public:
			RWLock()
			{
				::InitializeSRWLock(&rwLock_);
			}
			~RWLock()
			{
				// nothing
			}

		public:
			// 共享模式
			void RLock()
			{
				::AcquireSRWLockShared(&rwLock_);
			}
			// 独占模式
			void WLock()
			{
				::AcquireSRWLockExclusive(&rwLock_);
			}

			void RUnlock()
			{
				::ReleaseSRWLockShared(&rwLock_);
			}
			void WUnlock()
			{
				::ReleaseSRWLockShared(&rwLock_);
			}

		public:
			struct AutoReadLock
			{
				AutoReadLock(RWLock &lock)
					: rwLock_(lock)
				{
					rwLock_.RLock();
				}
				~AutoReadLock()
				{
					rwLock_.RUnlock();
				}

				RWLock &rwLock_;
			};

			struct AutoWriteLock
			{
				AutoWriteLock(RWLock &lock)
					: rwLock_(lock)
				{
					rwLock_.WLock();
				}

				~AutoWriteLock()
				{
					rwLock_.WUnlock();
				}

				RWLock &rwLock_;
			};
		};


		//-------------------------------------------------------
		// AutoMutex

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
		// AutoEvent

		class AutoEvent
		{
		public:
			HANDLE m_hEvent;

			AutoEvent(HANDLE hEvent = INVALID_HANDLE_VALUE) 
				: m_hEvent(hEvent)
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

			operator HANDLE()
			{ 
				return m_hEvent; 
			}

			operator const HANDLE() const 
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