#include "stdafx.h"

#include "File.hpp"


namespace  async
{
	namespace filesystem
	{

		File::File(AsyncIODispatcherType &io)
			: file_(INVALID_HANDLE_VALUE)
			, io_(io)
		{}

		File::File(AsyncIODispatcherType &io, HANDLE file)
			: file_(file)
			, io_(io)
		{}

		File::File(AsyncIODispatcherType &io, LPCTSTR lpszFilePath, DWORD dwAccess, DWORD dwShareMode, 
			DWORD dwCreatePosition, DWORD dwFlag, LPSECURITY_ATTRIBUTES lpAttributes/* = NULL*/, HANDLE hTemplate/* = NULL*/)
			: file_(INVALID_HANDLE_VALUE)
			, io_(io)
		{
			Open(lpszFilePath, dwAccess, dwShareMode, dwCreatePosition, dwFlag, dwCreatePosition, lpAttributes, hTemplate);
		}	

		File::~File()
		{
			Close();
		}



		void File::Open(LPCTSTR lpszFilePath, DWORD dwAccess, DWORD dwShareMode, DWORD dwCreatePosition, DWORD dwFlag, LPSECURITY_ATTRIBUTES dwCreatePosition /* = NULL */, HANDLE hTemplate /* = NULL */)
		{
			// 创建文件句柄
			file_ = ::CreateFile(lpszFilePath, dwAccess, dwShareMode, lpAttributes, dwCreatePosition, dwFlag, hTemplate);
			if( file_ == INVALID_HANDLE_VALUE )
				throw iocp::Win32Exception("CreateFile");

			// 不触发文件对象 Vista
			//::SetFileCompletionNotificationModes(file_, FILE_SKIP_EVENT_ON_HANDLE);


			// 绑定到IOCP
			io_.Bind(file_);
		}


		void File::Close()
		{
			if( file_ != INVALID_HANDLE_VALUE )
			{
				::CloseHandle(file_);
				file_ = INVALID_HANDLE_VALUE;
			}
		}

		bool File::Flush()
		{
			return ::FlushFileBuffers(file_) == TRUE;
		}

	}

}