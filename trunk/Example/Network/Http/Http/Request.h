#ifndef __HTTP_REQUEST_HPP
#define __HTTP_REQUEST_HPP



#include <string>
#include <vector>
#include "Header.h"



namespace http
{

	// �ӿͷ��˽��յ�������

	struct Request
	{
		std::string method;
		std::string uri;
		int httpVersionMajor;
		int httpVersionMinor;

		std::vector<Header> headers;
	};
}




#endif