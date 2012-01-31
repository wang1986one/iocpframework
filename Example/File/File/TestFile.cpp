// File.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "Impl/FileImpl.h"





int _tmain(int argc, _TCHAR* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	IODispatcher io;

	try
	{
		FileImpl file(io, _T("D:\\VM7\\VMware_7.0_CN.rar"), _T("C:\\VMware_7.0_CN.rar"));
		file.Start();

		system("pause");
		file.Stop();
	}
	catch(std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	

	system("pause");
	return 0;
}

