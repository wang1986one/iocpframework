// Http.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include <string>
#include <iostream>

#include "Server.h"


std::tr1::function<void()> console_ctrl_function;



BOOL WINAPI console_ctrl_handler(DWORD ctrl_type)
{
	switch (ctrl_type)
	{
	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		console_ctrl_function();
		return TRUE;
	default:
		return FALSE;
	}
}

// Debug Memory Info
void DebugAllocSize(LONG allocCount, size_t size)
{
	std::cout << "Alloc " << allocCount << ": " << size << std::endl;
}

void DebugDeallocSize(LONG deallocCount, size_t size)
{
	std::cout << "Dealloc " << deallocCount << ": " << size << std::endl;
}



int _tmain(int argc, _TCHAR* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// �����ڴ������ͷ�
	async::iocp::DefaultDebug::RegisterCallback(
		std::tr1::bind(&DebugAllocSize, std::tr1::placeholders::_1, std::tr1::placeholders::_2),
		std::tr1::bind(&DebugDeallocSize, std::tr1::placeholders::_1, std::tr1::placeholders::_2));


	try
	{
		http::Server server("127.0.0.1", "5050",
			"F:/Program/tut/doc/htdocs");

		console_ctrl_function = std::tr1::bind(&http::Server::Stop, &server);
		::SetConsoleCtrlHandler(&console_ctrl_handler, TRUE);

		server.Start();
	
		system("pause");
	}
	catch(std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		console_ctrl_function();
	}



	
	return 0;
}
