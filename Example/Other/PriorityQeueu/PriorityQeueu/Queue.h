#ifndef __QUEUE_HPP
#define __QUEUE_HPP





class HandlerPrority
{
	// forward declare
public:
	struct WrappedHandler;

	template<typename HandlerT>
	WrappedHandler Wrap(size_t priority, const HandlerT &callback)
	{
		return HandlerPrority::WrappedHandler(*this, priority, callback);
	}

private:
	class QueueHandler;


private:
	async::iocp::IODispatcher &io_;
	std::priority_queue<QueueHandler> handlers_;

public:
	explicit HandlerPrority(async::iocp::IODispatcher &io)
		: io_(io)
	{}

public:
	void Add(const WrappedHandler &handler);
	void ExcuteAll();

};


template<typename FuncT>
void HanlderInvoke(const FuncT &func, HandlerPrority::WrappedHandler *h)
{
	h->queue_.Add(h->priority_, func);
}




struct HandlerPrority::WrappedHandler
{
	HandlerPrority &queue_;
	size_t priority_;
	async::iocp::AsyncCallbackBasePtr result_;

public:
	template<typename HandlerT>
	WrappedHandler(HandlerPrority &queue, size_t priority, const HandlerT &callback)
		: queue_(queue)
		, priority_(priority)
		, result_(async::iocp::MakeAsyncCallback(callback))
	{}


public:
	const async::iocp::AsyncCallbackBasePtr &operator()() const
	{
		return result_;
	}
	/*template<typename Arg1>
	const AsyncResultPtr & operator()(const Arg1 &arg1)
	{
		return handler_(arg1);
	}
	template<typename Arg1, typename Arg2>
	const AsyncResultPtr & operator()(const Arg1 &arg1, const Arg2 &arg2)
	{
		return handler_(arg1, arg2);
	}*/

};


class HandlerPrority::QueueHandler
{
private:
	size_t priority_;
	async::iocp::AsyncCallbackBasePtr result_;
	async::iocp::IODispatcher &io_;

public:
	QueueHandler(async::iocp::IODispatcher &io, size_t priority, const async::iocp::AsyncCallbackBasePtr &result)
		: priority_(priority)
		, result_(result)
		, io_(io)
	{}

	QueueHandler &operator=(const QueueHandler &rhs)
	{
		if( &rhs != this )
		{
			priority_	= rhs.priority_;
			result_		= rhs.result_;
			io_			= rhs.io_;
		}

		return *this;
	}

public:
	void Excute()
	{
		io_.Post(result_.Get());
	}

	friend bool operator<(const QueueHandler &lhs, const QueueHandler &rhs)
	{
		return lhs.priority_ < rhs.priority_;
	}
};



#endif