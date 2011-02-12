// File.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include "Impl/FileImpl.h"





int _tmain(int argc, _TCHAR* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	IODispatcher io;

	try
	{
		FileImpl file(io, _T("C:/GTcpClt_Log.ini"), _T("C:/pltmp.xml"));
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

