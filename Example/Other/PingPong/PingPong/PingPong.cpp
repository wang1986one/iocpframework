// PingPong.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "HandlerAllocator.hpp"
#include "Service.hpp"



void TestHanlderAllocator()
{
	HandlerAllocator<> alloc;
	void *p = alloc.Allocate(1023);
	memset(p, 10, 1023);
	alloc.Deallocate(p);
}

int _tmain(int argc, _TCHAR* argv[])
{
	// TestHanlderAllocator();

	try
	{
		if( argc != 4 )
		{
			std::cerr << "Usage: server <port> <threads> <blocksize> \n";
				
			system("pause");
			return 1;
		}

		u_short port = _tstoi(argv[1]);
		size_t threads = _tstoi(argv[2]);
		size_t blockSize = _tstoi(argv[3]);

		IODispatcher io(threads);

		Server svr(io, port, blockSize);
		system("pause");
	}
	catch(std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}

	
	return 0;
}

