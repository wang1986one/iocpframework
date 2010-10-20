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
		/// Perform URL-decoding on a string. Returns false if the encoding was
		/// invalid.
		static bool UrlDecode(const std::string& in, std::string& out);
	};
}





#endif