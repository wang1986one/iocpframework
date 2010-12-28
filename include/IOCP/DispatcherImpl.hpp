#include <process.h>

#include "Dispatcher.hpp"
#include "WinException.hpp"
#include "../MultiThread/Tls.hpp"


namespace async
{
	
	namespace iocp
	{

		template<typename AsyncT>
		IODispatcher<AsyncT>::IODispatcher(size_t numThreads/* = 0*/)
		{
			if( !iocp_.Create(numThreads) )
				throw Win32Exception("iocp_.Create()");

			try
			{
				// 创建指定的线程数
				threads_.reserve(numThreads);

				for(int i = 0; i != numThreads; ++i)
				{
					HANDLE hThread = (HANDLE)::_beginthreadex(NULL, 0, &IODispatcher::_ThreadIOImpl, this, 0, 0);

					if( hThread == NULL )
						throw Win32Exception("_beginthreadex");

					threads_.push_back(hThread);
				}
			}
			catch(const std::exception &e)
			{
				Stop();
				throw e;
			}
		}

		template<typename AsyncT>
		IODispatcher<AsyncT>::~IODispatcher()
		{
			try
			{		
				Stop();
				iocp_.Close();
			}
			catch(...)
			{
				std::cerr << "Unknown error!" << std::endl;
			}
		}

		template<typename AsyncT>
		void IODispatcher<AsyncT>::Bind(HANDLE hHandle)
		{
			if( !iocp_.AssociateDevice(hHandle, 0) )
				throw Win32Exception("iocp_.AssociateDevice");
		}

		template<typename AsyncT>
		template<typename AsyncPtrT>
		void IODispatcher<AsyncT>::Post(AsyncPtrT &result)
		{
			_PostImpl(result, detail::Int2Type<AsyncType::IS_OVERLAPPED>());
		}

		template<typename AsyncT>
		template<typename AsyncPtrT>
		void IODispatcher<AsyncT>::_PostImpl(AsyncPtrT &result, detail::Int2Type<FALSE>)
		{
			result->AddRef();

			if( 0 == iocp_.PostStatus(reinterpret_cast<ULONG_PTR>(&*result), 0, 0) )
				throw Win32Exception("iocp_.PostStatus");
		}
		template<typename AsyncT>
		template<typename AsyncPtrT>
		void IODispatcher<AsyncT>::_PostImpl(AsyncPtrT &result, detail::Int2Type<TRUE>)
		{
			result->AddRef();

			if( 0 == iocp_.PostStatus(0, 0, &*result) )
				throw Win32Exception("iocp_.PostStatus");
		}

		template<typename AsyncT>
		template<typename AsyncPtrT>
		void IODispatcher<AsyncT>::Dispatch(AsyncPtrT &result)
		{	
			if( thread::CallStack<IODispatcher<AsyncType>>::Contains(this) )
				AsyncType::Call(result);
			else
				Post(result);
		}

		template<typename AsyncT>
		void IODispatcher<AsyncT>::Stop()
		{
			// 取消所有线程上的待决的IO操作
			std::for_each(threads_.begin(), threads_.end(), std::ptr_fun(::CancelIo));

			// 先停止所有的线程
			for(ThreadsIter iter = threads_.begin();
				iter != threads_.end();
				++iter)
			{
				iocp_.PostStatus(0, 0, NULL);
			}

			// 等待线程退出并关闭句柄
			if( !threads_.empty() )
				::WaitForMultipleObjects(threads_.size(), &*threads_.begin(), TRUE, INFINITE);

			std::for_each(threads_.begin(), threads_.end(), std::ptr_fun(::CloseHandle));

			threads_.clear();
		}


		template<typename AsyncT>
		void IODispatcher<AsyncT>::_ThreadIO()
		{
			thread::CallStack<IODispatcher<AsyncT>>::Context ctx(this);

			DWORD dwSize			= 0;
			ULONG_PTR uKey			= 0;
			OVERLAPPED *pOverlapped = 0;

			ULONG_PTR *key			= &uKey;

			while(true)
			{
				::SetLastError(0);
				bool bSuc = iocp_.GetStatus(reinterpret_cast<ULONG_PTR *>(&key), &dwSize, &pOverlapped);
				u_long err = ::GetLastError();

				// 请求退出
				if( key == 0 && pOverlapped == 0 )
					break;

				try
				{
					// 回调
					AsyncType::Call(key, pOverlapped, dwSize, err);
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


		template<typename AsyncT>
		size_t IODispatcher<AsyncT>::_ThreadIOImpl(LPVOID pParam)
		{
			IODispatcher<AsyncT> *pThis = reinterpret_cast<IODispatcher<AsyncT> *>(pParam);

			pThis->_ThreadIO();

			::OutputDebugStringW(L"OVERLAPPED Thread Exit\n");
			return 0;
		}


	}

}