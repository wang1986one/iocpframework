#include "stdafx.h"
#include "Queue.h"






void HandlerPrority::Add(const WrappedHandler &handler)
{
	handlers_.push(QueueHandler(io_, handler.priority_, handler.result_));
}

void HandlerPrority::ExcuteAll()
{
	while( !handlers_.empty() )
	{
		QueueHandler &handler = handlers_.top();
		handler.Excute();
		handlers_.pop();
	}
}