#ifndef __LOGGER_LOGGER_SERVICE_HPP
#define __LOGGER_LOGGER_SERVICE_HPP

#include <sstream>
#include <fstream>
#include <string>

#include "../iocp/Dispatcher.hpp"


namespace async
{
	namespace logger
	{
		namespace impl
		{

			// -------------------------------------------
			// class LoggerService

			class LoggerService
			{
			private:
				iocp::IODispatcher &io_;
				std::ofstream out_;
				
			public:
				// Logger 实现
				struct LoggerImpl
				{
					std::string id_;
					explicit LoggerImpl(const std::string &id)
						: id_(id)
					{}
				};
				typedef LoggerImpl * LoggerImplType;


			public:
				LoggerService(iocp::IODispatcher &io)
					: io_(io)
				{}
				~LoggerService()
				{}

			private:
				LoggerService(const LoggerService &);
				LoggerService &operator=(const LoggerService &);

			public:
				static LoggerService &GetInstance(iocp::IODispatcher &io)
				{
					static LoggerService service(io);
					return service;
				}

			public:
				void Create(LoggerImplType &impl, const std::string &id)
				{
					impl = new LoggerImpl(id);
				}
				void Destroy(LoggerImplType &impl)
				{
					delete impl;
					impl = Null();
				}
				LoggerImplType Null() const
				{
					return 0;
				}

				// 设置日志文件
				void UseFile(LoggerImplType &/*impl*/, const std::string &file)
				{
					iocp::AsyncResultPtr result(new iocp::AsyncResult(iocp::nothing, iocp::nothing, iocp::nothing, iocp::nothing,
						std::tr1::bind(&LoggerService::_UseFileImpl, this, std::tr1::placeholders::_1, file)));

					io_.Dispatch(result);
				}

				// 记录信息
				void Log(LoggerImplType &impl, const std::string &msg)
				{
					// 格式化
					std::ostringstream os;
					os << impl->id_ << ": " << msg;

					iocp::AsyncResultPtr result(new iocp::AsyncResult(iocp::nothing, iocp::nothing, iocp::nothing, iocp::nothing,
						std::tr1::bind(&LoggerService::_LogImpl, this, std::tr1::placeholders::_1, os.str())));

					io_.Dispatch(result);
				}

			private:
				void _UseFileImpl(const iocp::AsyncResultPtr &result, const std::string &file)
				{
					out_.close();
					out_.clear();
					out_.open(file.c_str());
				}

				void _LogImpl(const iocp::AsyncResultPtr &result, const std::string &text)
				{
					if( out_ )
						out_ << text << std::endl;
				}
			};
		}
	}
}




#endif