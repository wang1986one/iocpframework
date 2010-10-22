#ifndef __HTTP_REQUEST_PARSER_HPP
#define __HTTP_REQUEST_PARSER_HPP

#include <tuple>




namespace http
{

	// ��������󷵻�ֵ
	enum ParseRet
	{
		TRUE_VALUE,
		FALSE_VALUE,
		INDETERMINATE
	};


	struct Request;

	// ��������
	class RequestParser
	{
		// The current state of the parser.
		enum state
		{
			method_start,
			method,
			uri_start,
			uri,
			http_version_h,
			http_version_t_1,
			http_version_t_2,
			http_version_p,
			http_version_slash,
			http_version_major_start,
			http_version_major,
			http_version_minor_start,
			http_version_minor,
			expecting_newline_1,
			header_line_start,
			header_lws,
			header_name,
			space_before_header_value,
			header_value,
			expecting_newline_2,
			expecting_newline_3
		} state_;

	public:
		RequestParser();

	public:
		//����״̬
		void Reset();

		// ����ֵ TRUE_VALUE, FALSE_VALUE, INDETERMINATE
		template<typename InputIteratorT>
		std::tr1::tuple<ParseRet, ParseRet> Parse(Request& req, InputIteratorT begin, InputIteratorT end)
		{
			while(begin != end)
			{
				ParseRet result = Consume(req, *begin++);
				if( result != INDETERMINATE )
					return std::tr1::make_tuple(result, result);
			}
			
			return std::tr1::make_tuple(INDETERMINATE, INDETERMINATE);
		}

	private:
		// ������һ�������ַ�
		ParseRet Consume(Request& req, char input);

		// ����Ƿ�ΪHTTP�ַ�
		static bool isChar(int c);

		//����Ƿ�ΪHTTP������
		static bool isCtl(int c);

		//����Ƿ�ΪHTTP������
		static bool isTSpecial(int c);

		// ����Ƿ�Ϊ����
		static bool isDigit(int c);
	};
		
}






#endif