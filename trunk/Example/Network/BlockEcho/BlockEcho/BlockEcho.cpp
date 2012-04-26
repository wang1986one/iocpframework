// BlockEcho.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


#include "../../../../include/Network/TCP.hpp"

#include "../../../../MultiThread/Thread.hpp"



const size_t MaxLenth = 1024;

using namespace async;
using namespace async::iocp;
using namespace async::network;
using namespace async::thread;



DWORD Session(SocketPtr socket)
{
	try
	{
		//socket->Open(Tcp::Family(), Tcp::Type(), Tcp::Protocol());

		while(1)
		{
			static char data[MaxLenth];
			static AutoBufferPtr buffer(MakeBuffer(data));
			buffer->resize(MaxLenth);

			size_t len = socket->Read(buffer->data(), buffer->size(), 0);
			buffer->resize(len);
			if( len == 0 )
				break;

			//size_t sendLen = socket->Write(buffer);
			socket->Write(buffer->data(), buffer->size(), 0/*, TransferAtLeast(2)*/);
		}
	}
	catch(std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}

DWORD Server(IODispatcher &io, u_short port)
{
	try
	{
		Tcp::Accpetor accpetor(io, Tcp::V4(), port);

		while(1)
		{
			Tcp::Accpetor::ImplementType sock = accpetor.Accept();
			SocketPtr session(sock);

			ThreadImplEx *threadSession(new ThreadImplEx);	// 不管了 没释放
			threadSession->RegisterFunc(std::tr1::bind(&Session, std::tr1::ref(session)));
			threadSession->Start();
		}
	}
	catch(std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}


int _tmain(int argc, _TCHAR* argv[])
{	
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	try
	{
		IODispatcher io(0);
		Server(io, 5050);
	}
	catch(std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}

	system("pause");
	return 0;
}

