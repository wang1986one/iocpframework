// File.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "Impl/FileImpl.h"



class FileRead
{
	LARGE_INTEGER readIndex;
	LARGE_INTEGER writeIndex;

	std::auto_ptr<FileImpl> readFile;
	std::auto_ptr<FileImpl> writeFile;

public:
	FileRead(IODispatcher &io)
		: readFile(new FileImpl(io, _T("C:/jtts4_sapi5.log"), FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS | FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING, OPEN_EXISTING))
		, writeFile(new FileImpl(io, _T("C:/jtts4_sapi5_tmp.log"), FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS | FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING, CREATE_ALWAYS))
	{
		RtlZeroMemory(&readIndex, sizeof(readIndex));
		RtlZeroMemory(&writeIndex, sizeof(writeIndex));

		readFile->RegisterCallback(std::tr1::bind(&FileRead::OnRead, this, std::tr1::placeholders::_1, std::tr1::placeholders::_2),
			NULL,
			std::tr1::bind(&FileRead::OnError, this, std::tr1::placeholders::_1));

		writeFile->RegisterCallback(NULL,
			std::tr1::bind(&FileRead::OnWrite, this, std::tr1::placeholders::_1, std::tr1::placeholders::_2),
			std::tr1::bind(&FileRead::OnError, this, std::tr1::placeholders::_1));
	}

public:
	void Read(const FileBufferPtr &buf)
	{
		readFile->Read(buf, 0, buf->allocSize(), readIndex, nothing);
	}

private:
	void OnRead(const FileBufferPtr &buffer, const ObjectPtr &obj)
	{
		std::cout << buffer->data() << std::endl;

		readIndex.QuadPart += buffer->size();

		writeFile->Write(buffer, 0, buffer->size(), writeIndex, obj);
	}

	void OnWrite(const FileBufferPtr &buffer, const ObjectPtr &obj)
	{
		writeIndex.QuadPart += buffer->size();

		readFile->Read(buffer, 0, buffer->allocSize(), readIndex, obj);
	}

	void OnError(const std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
};




int _tmain(int argc, _TCHAR* argv[])
{
	IODispatcher io;

	try
	{
		FileRead file(io);

		FileBufferPtr buf(new FileBuffer);
		
		file.Read(buf);

		system("pause");
	}
	catch(std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	

	system("pause");
	return 0;
}

