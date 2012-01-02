#ifndef __HTTP_REQUEST_HANDLER_HPP
#define __HTTP_REQUEST_HANDLER_HPP


#include <string>


namespace http
{

	struct Reply;
	struct Request;

	// ���н����Request Handler
	class RequestHandler
	{
	private:
		// �ļ�Ŀ¼
		std::string docRoot_;

	public:
		// �ļ�Ŀ¼����
		explicit RequestHandler(const std::string& docRoot);

		// ��������
		void HandleRequest(const Request& req, Reply& rep);

	
	private:
		static bool _UrlDecode(const std::string& in, std::string& out);
	};
}





#endif