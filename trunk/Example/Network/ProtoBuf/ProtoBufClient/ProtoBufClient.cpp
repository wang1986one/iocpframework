// ProtoBufClient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "ClientImpl.hpp"



#pragma comment(lib, "libprotobuf")


int _tmain(int argc, _TCHAR* argv[])
{
	async::iocp:: IODispatcher io;
	Client client(io, "127.0.0.1", 5050);
	
	muduo::Query query;
	query.set_id(1);
	query.set_questioner("TestTest");
	query.add_question("Running?");
	query.add_question("Running123123");
	query.add_question("Running1231");
	
	client.Send(query);
	system("pause");
	
	muduo::Answer ans;
	ans.set_id(1000);
	ans.set_questioner("chenyu");
	ans.set_answerer("hahaha");
	ans.add_solution("test1");
	ans.add_solution("test2");
	ans.add_solution("test3");
	ans.add_solution("test4");
	ans.add_solution("test5");
	client.Send(ans);

	system("pause");
	return 0;
}

