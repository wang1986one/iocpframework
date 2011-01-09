// NetworkCli.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "ClientImpl/ClientImpl.hpp"


int _tmain(int argc, _TCHAR* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	IODispatcher ioService(GetFitThreadNum());

	try
	{
		Client client(ioService, "127.0.0.1", 5050);
		system("pause");
	}
	catch(std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	
	return 0;
}

