#include "stdafx.h"
#include "FileImpl.h"





// 圆整为扇区大小倍数
template<typename T, typename U>
inline U Round(const T &n, const U &m)
{
	return (n + m - 1) & ~(m - 1);
}




// 将源文件的句柄作为目标文件hFileTemplate的参数，使其具有同源文件相同的属性

FileImpl::FileImpl(IODispatcher &io, LPCTSTR lpszReadFile, DWORD dwReadFlag, DWORD creation)
: file_(new File(io
			 , lpszReadFile
			 , GENERIC_READ | GENERIC_WRITE
			 , FILE_SHARE_READ | FILE_SHARE_WRITE
			 , NULL
			 , creation
			 , dwReadFlag))
{}

void FileImpl::RegisterCallback(const ReadCallback &readCallback /* = NULL */, const WriteCallback &writeCallback /* = NULL */, const ErrorCallback &errorCallbaCK /* = NULL */)
{
	readCallback_	= readCallback;
	writeCallback_	= writeCallback;
	errorCallback_	= errorCallbaCK;
}

void FileImpl::Read(const FileBufferPtr &buffer, size_t nOffsetBuf, size_t nBufSize, const LARGE_INTEGER &fileIndex, const ObjectPtr &obj)
{
	try
	{
		file_->BeginRead(buffer, nOffsetBuf, nBufSize, &fileIndex, std::tr1::bind(&FileImpl::_OnRead, this, std::tr1::placeholders::_1));
	}
	catch(std::exception &e)
	{
		if( errorCallback_ != NULL )
			errorCallback_(std::tr1::cref(e));
	}	
}

void FileImpl::Write(const FileBufferPtr &buffer, size_t bufOffset, size_t bufSize, const LARGE_INTEGER &fileIndex, const ObjectPtr &obj)
{
	try
	{
		file_->BeginWrite(buffer, bufOffset, bufSize, &fileIndex, std::tr1::bind(&FileImpl::_OnWrite, this, std::tr1::placeholders::_1));
	}
	catch(std::exception &e)
	{
		if( errorCallback_ != NULL )
			errorCallback_(std::tr1::cref(e));
	}
}


void FileImpl::_OnRead(const AsyncResultPtr &asyncResult)
{
	try 
	{
		size_t nSize = file_->EndRead(asyncResult);
		const FileBufferPtr &buffer = asyncResult->m_buffer;
		const ObjectPtr &obj = asyncResult->m_asynState;

		buffer->resize(nSize);

		if( readCallback_ != NULL )
			readCallback_(std::tr1::cref(buffer), std::tr1::cref(obj));
	}
	catch(const std::exception &e) 
	{
		if( errorCallback_ != NULL )
			errorCallback_(std::tr1::cref(e));
	}
}

void FileImpl::_OnWrite(const AsyncResultPtr &result)
{
	try 
	{
		size_t nSize = file_->EndWrite(result);
		const FileBufferPtr &buffer = result->m_buffer;
		const ObjectPtr &obj = result->m_asynState;

		buffer->resize(nSize);

		if( writeCallback_ != NULL )
			writeCallback_(std::tr1::cref(buffer), std::tr1::cref(obj));
	}
	catch(const std::exception &e) 
	{
		if( errorCallback_ != NULL )
			errorCallback_(std::tr1::cref(e));
	}
}
