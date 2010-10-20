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

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		http::Server server("127.0.0.1", "5050",
			"D:/Files/Extent Library/boost_1_39_0/doc/boost-doc-zh");

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

