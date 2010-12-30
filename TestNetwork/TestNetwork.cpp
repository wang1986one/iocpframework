// TestNetwork.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include "impl/ServerImpl.h"



int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


	OverlappedDispatcher ioService(GetFitThreadNum());

	shared_ptr
	try
	{
		Server server(ioService, 5050);
		server.Start();

		system("pause");
	}
	catch(std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}

	system("pause");
	return 0;
}
