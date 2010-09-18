#include "stdafx.h"

#include "File.hpp"
#include "../IOCP/WinException.hpp"


namespace  async
{



	namespace filesystem
	{
		File::File(IODispatcher &io, LPCTSTR lpszFilePath, DWORD dwAccess, DWORD dwShareMode, 
			LPSECURITY_ATTRIBUTES lpAttributes, DWORD dwCreatePosition, DWORD dwFlag, HANDLE hTemplate/* = NULL*/)
			: file_(INVALID_HANDLE_VALUE)
			, io_(io)
		{
			// 创建文件句柄
			file_ = ::CreateFile(lpszFilePath, dwAccess, dwShareMode, lpAttributes, dwCreatePosition, dwFlag, hTemplate);
			if( file_ == INVALID_HANDLE_VALUE )
				throw Win32Exception("CreateFile");

			// 不触发文件对象 Vista
			//::SetFileCompletionNotificationModes(file_, FILE_SKIP_EVENT_ON_HANDLE);

			io_.Bind(file_);
		}	

		File::~File()
		{
			Close();
		}



		// Read
		AsyncResultPtr File::BeginRead(const FileBufferPtr &buf, size_t nOffset, size_t nBufSize, const LARGE_INTEGER *index
			, AsyncCallbackFunc callback/* = 0*/, const ObjectPtr &asyncState/* = nothing*/ , const ObjectPtr &internalState/* = nothing*/)
		{
			AsyncResultPtr asynResult(new AsyncResult(this, buf, asyncState, internalState, callback));
			asynResult->AddRef();

			// 设置偏移
			if( index != NULL )
			{
				(static_cast<OVERLAPPED *>(asynResult.Get()))->Offset		= index->LowPart;
				(static_cast<OVERLAPPED *>(asynResult.Get()))->OffsetHigh	= index->HighPart;
			}	

			DWORD bytesRead = 0;
			BOOL bSuc = ::ReadFile(file_, buf->data(nOffset), nBufSize, &bytesRead, asynResult.Get());
			DWORD dwError = ::WSAGetLastError();

			// 立即完成，由IOCP转发
			if( bSuc && bytesRead != 0 )
			{
				io_.Post(asynResult);
			}
			else if( !bSuc && dwError != WSA_IO_PENDING )
			{
				asynResult->Release();
				throw Win32Exception("ReadFile");
			}

			return asynResult;
		}

		size_t File::EndRead(const AsyncResultPtr &asynResult)
		{
			return _EndAsyncOperation(asynResult);
		}


		// Write
		AsyncResultPtr File::BeginWrite(const FileBufferPtr &buf, size_t bufOffset, size_t bufSize, const LARGE_INTEGER *index
			, AsyncCallbackFunc callback/* = 0*/, const ObjectPtr &asyncState/* = nothing*/, const ObjectPtr &internalState/* = nothing*/)
		{
			AsyncResultPtr asynResult(new AsyncResult(this, buf, asyncState, internalState, callback));
			asynResult->AddRef();

			// 设置偏移
			if( index != NULL )
			{
				(static_cast<OVERLAPPED *>(asynResult.Get()))->Offset = index->LowPart;
				(static_cast<OVERLAPPED *>(asynResult.Get()))->OffsetHigh = index->HighPart;
			}		

			DWORD bytesWrite = 0;
			BOOL bSuc = ::WriteFile(file_, buf->data(bufOffset), bufSize, &bytesWrite, asynResult.Get()); 
			DWORD dwError = ::WSAGetLastError();

			// 立即完成，由IOCP转发
			if( bSuc && bytesWrite != 0 )
			{
				io_.Post(asynResult);
			}
			else if( !bSuc && dwError != WSA_IO_PENDING )
			{
				asynResult->Release();
				throw Win32Exception("WriteFile");
			}

			return asynResult;
		}

		size_t File::EndWrite(const AsyncResultPtr &asynResult)
		{
			return _EndAsyncOperation(asynResult);
		}


		void File::Close()
		{
			if( file_ != INVALID_HANDLE_VALUE )
			{
				::CloseHandle(file_);
				file_ = INVALID_HANDLE_VALUE;
			}
		}


		size_t File::_EndAsyncOperation(const AsyncResultPtr &asyncResult)
		{
			DWORD size = 0;

			if( 0 == ::GetOverlappedResult(file_, asyncResult.Get(), &size, TRUE) )
				throw Win32Exception("GetOverlappedResult");

			return size;
		}
	}

}