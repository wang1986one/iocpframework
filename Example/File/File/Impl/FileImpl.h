#ifndef __FILE_IMPL_HPP
#define __FILE_IMPL_HPP



#include "../../include/FileSystem/BasicFile.hpp"


#include <algorithm>


using namespace async;
using namespace async::iocp;
using namespace async::filesystem;

using namespace std::tr1::placeholders;


// Բ��Ϊ������С����
template<typename U>
inline U Round(const U &m)
{
	return (m + 512 - 1) & ~(512 - 1);
}



class FileImpl
{
private:
	//std::tr1::array<char, 8192> buf
	AutoBufferPtr buf_;
	u_int64 offset_;

	BasicFile readFile_;
	BasicFile writeFile_;

	DWORD fileSize_;

public:
	FileImpl(IODispatcher &io, LPCTSTR lpszReadFile, LPCTSTR lpszWriteFile)
		: readFile_(io, lpszReadFile, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS | FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED)
		, writeFile_(io, lpszWriteFile, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED)
		, offset_(0)
		, buf_(MakeBuffer(8192))
	{
		//std::uninitialized_fill(buf_.begin(), buf_.end(), 0);
		HANDLE file = ::CreateFile(lpszReadFile, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		fileSize_ = ::GetFileSize(file, &fileSize_);
		::CloseHandle(file);
	}

public:
	void Start()
	{
		try
		{
			DWORD left = min(fileSize_ - (DWORD)offset_, 8192);
			if( left == 0 )
			{
				Stop();
			}

			AsyncRead(readFile_, Buffer(buf_->data(), Round(left)), offset_, TransferAtLeast(left), 
				std::tr1::bind(&FileImpl::_OnRead, this, _1, _2));
		}
		catch(std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	void Stop()
	{
		readFile_.Close();

		writeFile_.Close();
	}

private:
	void _OnRead(u_long size, u_long error)
	{
		if( error != 0 )
		{
			assert(0);
			return;
		}

		try
		{
			AsyncWrite(writeFile_, Buffer(buf_->data(), size), offset_, TransferAtLeast(size), 
				std::tr1::bind(&FileImpl::_OnWrite, this, _1, _2));
		}
		catch(std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	void _OnWrite(u_long size, u_long error)
	{
		if( error != 0 )
		{
			assert(0);
			return;
		}

		offset_ += size;
		Start();
	}
};







#endif