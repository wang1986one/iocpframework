#ifndef __HTTP_CONNECTION_MANAGER_HPP
#define __HTTP_CONNECTION_MANAGER_HPP

#include <set>
#include "Connection.h"

#include "../../include/MultiThread/Lock.hpp"

namespace http
{

	class ConnectionMgr
	{
		typedef async::thread::CAutoCriticalSection CSLock;
		typedef async::thread::CAutoLock<CSLock>	AutoLock;

	private:
		std::set<ConnectionPtr> connections_;
		CSLock lock_;

	public:
		ConnectionMgr(){}

	private:
		ConnectionMgr(const ConnectionMgr &);
		ConnectionMgr &operator=(const ConnectionMgr &);

	public:
		void Start(const ConnectionPtr &cc);
		void Stop(const ConnectionPtr &c);

		void StopAll();
	};
}




#endif