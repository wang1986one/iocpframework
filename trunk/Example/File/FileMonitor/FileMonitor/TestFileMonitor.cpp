// FileMonitor.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream>
#include "../../../../include/FileSystem/FileMonitor.hpp"


void Monitor(LPCTSTR fileName, size_t len)
{
	std::wcout.write(fileName, len) << std::endl;
}


int _tmain(int argc, _TCHAR* argv[])
{
	async::iocp::IODispatcher io;
	async::filesystem::FileSystemMonitor monitor(io, L"C:/");

	using namespace std::tr1::placeholders;
	monitor.Monitor(std::tr1::bind(&Monitor, _3, _4));

	system("pause");

	return 0;
}

