#include "stdafx.h"

#include <process.h>

#include "Dispatcher.hpp"
#include "WinException.hpp"
#include "../MultiThread/Tls.hpp"


namespace async
{
	namespace HandlerInvoke
	{
		inline void Invoke(const iocp::AsyncResultPtr &result)
		{
			result->m_callback(std::tr1::cref(result));
		}
	}

	
	namespace iocp
	{
		// 获取适合系统的线程数
		size_t GetFitThreadNum(size_t perCPU/* = 2*/)
		{
			SYSTEM_INFO systemInfo = {0};
			::GetSystemInfo(&systemInfo);

			return perCPU * systemInfo.dwNumberOfProcessors + 2;
		}


		IODispatcher::IODispatcher(size_t numThreads/* = 0*/)
		{
			if( !m_hIOCP.Create(numThreads) )
				throw Win32Exception("m_hIOCP.Create()");

			try
			{
				// 创建指定的线程数
				m_threads.reserve(numThreads);

				for(int i = 0; i != numThreads; ++i)
				{
					HANDLE hThread = (HANDLE)::_beginthreadex(NULL, 0, &IODispatcher::_ThreadIOImpl, this, 0, 0);

					if( hThread == NULL )
						throw Win32Exception("_beginthreadex");

					m_threads.push_back(hThread);
				}
			}
			catch(const std::exception &e)
			{
				Stop();
				throw e;
			}
		}

		IODispatcher::~IODispatcher()
		{
			try
			{		
				Stop();
				m_hIOCP.Close();
			}
			catch(...)
			{
				std::cerr << "Unknown error!" << std::endl;
			}
		}

		void IODispatcher::Bind(HANDLE hHandle)
		{
			if( !m_hIOCP.AssociateDevice(hHandle, 0) )
				throw Win32Exception("m_hIOCP.AssociateDevice");
		}

		void IODispatcher::Post(const AsyncResultPtr &result)
		{
			result->AddRef();

			if( 0 == m_hIOCP.PostStatus(0, 0, &*result) )
				throw Win32Exception("m_hIOCP.PostStatus");
		}

		void IODispatcher::Dispatch(const AsyncResultPtr &result)
		{	
			result->AddRef();

			if( thread::CallStack<IODispatcher>::Contains(this) )
				HandlerInvoke::Invoke(result);
			else
				Post(result);
		}

		void IODispatcher::Stop()
		{
			// 取消所有线程上的待决的IO操作
			std::for_each(m_threads.begin(), m_threads.end(), std::ptr_fun(::CancelIo));

			// 先停止所有的线程
			for(ThreadsIter iter = m_threads.begin();
				iter != m_threads.end();
				++iter)
			{
				m_hIOCP.PostStatus(static_cast<ULONG_PTR>(~0), 0, NULL);
			}

			// 等待线程退出并关闭句柄
			if( !m_threads.empty() )
				::WaitForMultipleObjects(m_threads.size(), &*m_threads.begin(), TRUE, INFINITE);

			std::for_each(m_threads.begin(), m_threads.end(), std::ptr_fun(::CloseHandle));

			m_threads.clear();
		}


		void IODispatcher::_ThreadIO()
		{
			thread::CallStack<IODispatcher>::Context ctx(this);

			DWORD dwSize = 0;
			ULONG_PTR uKey = 0;
			OVERLAPPED *pOverlapped = NULL;

			while(true)
			{
				BOOL bSuc = ::GetQueuedCompletionStatus(m_hIOCP, &dwSize, &uKey, &pOverlapped, INFINITE);
				//if( !bSuc )
				//{
				//	//throw Win32Exception("GetQueuedCompletionStatus");
				//	continue;
				//}

				if( uKey != 0 || pOverlapped == NULL )
					break;

				// 回调
				AsyncResultPtr asynResult = static_cast<AsyncResult *>(pOverlapped);
				asynResult->Release();

				try
				{
					HandlerInvoke::Invoke(asynResult);
				}
				catch(const std::exception &e)
				{
					std::cerr << e.what() << std::endl;
					assert(0);
					// Opps!!
				}
				catch(...)
				{
					assert(0);
					// Opps!!
				}
				
			}
		}


		size_t IODispatcher::_ThreadIOImpl(LPVOID pParam)
		{
			IODispatcher *pThis = reinterpret_cast<IODispatcher *>(pParam);

			pThis->_ThreadIO();

			::OutputDebugStringW(L"IO Thread Exit\n");
			return 0;
		}


	}

}