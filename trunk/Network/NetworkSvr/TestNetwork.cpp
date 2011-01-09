// TestNetwork.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include "impl/ServerImpl.h"
#include "../../include/Timer/Timer.hpp"


void AsyncPrint()
{
	std::cout << " remote number: " << g_ClientNum << std::endl;
}

int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


	IODispatcher ioService(GetFitThreadNum());

	try
	{
		Server server(ioService, 5050);
		server.Start();

		async::timer::Timer time(ioService, 2000, 0);
		time.BeginWait(std::tr1::bind(&AsyncPrint));

		system("pause");
	}
	catch(std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}

	system("pause");
	return 0;
}
