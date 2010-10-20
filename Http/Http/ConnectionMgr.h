#ifndef __HTTP_CONNECTION_MANAGER_HPP
#define __HTTP_CONNECTION_MANAGER_HPP

#include <set>
#include "Connection.h"



namespace http
{

	class ConnectionMgr
	{
	private:
		std::set<ConnectionPtr> connections_;

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