#ifndef __HTTP_MIME_TYPES_HPP
#define __HTTP_MIME_TYPES_HPP



#include <string>


namespace http
{
	namespace mime_types
	{

		// ���ļ�����չ��ת����MIME��ʽ
		std::string ExtensionToType(const std::string &exten);
	}
}






#endif