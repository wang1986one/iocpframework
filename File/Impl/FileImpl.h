#ifndef __FILE_IMPL_HPP
#define __FILE_IMPL_HPP



#include "../../include/FileSystem/File.hpp"





using namespace async;
using namespace async::filesystem;


class FileImpl
{
public:
	typedef std::tr1::function<void(const FileBufferPtr &, const ObjectPtr &)>	ReadCallback;
	typedef std::tr1::function<void(const FileBufferPtr &, const ObjectPtr &)>	WriteCallback;
	typedef std::tr1::function<void(const std::exception &)>					ErrorCallback;

private:
	FilePtr			file_;

	ReadCallback	readCallback_;
	WriteCallback	writeCallback_;
	ErrorCallback	errorCallback_;

public:
	FileImpl(IODispatcher &, LPCTSTR, DWORD, DWORD);

private:
	FileImpl(const FileImpl &);
	FileImpl &operator=(const FileImpl &);


public:
	void RegisterCallback(const ReadCallback &readCallback = NULL, const WriteCallback &writeCallback = NULL, const ErrorCallback &errorCallbaCK = NULL);
	
	void Read(const FileBufferPtr &, size_t, size_t, const LARGE_INTEGER &, const ObjectPtr &);
	void Write(const FileBufferPtr &, size_t, size_t, const LARGE_INTEGER &, const ObjectPtr &);

	void Flush()
	{
		::FlushFileBuffers(file_->GetHandle());
	}


private:
	void _OnRead(const AsyncResultPtr &);
	void _OnWrite(const AsyncResultPtr &);
};







#endif