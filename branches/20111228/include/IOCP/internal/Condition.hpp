#ifndef __IOCP_INTERNAL_CONDITION_HPP
#define __IOCP_INTERNAL_CONDITION_HPP



namespace async
{
	namespace iocp
	{

		namespace detail
		{
			// 默认单次传输字节大小
			enum { DEFAULT_MAX_TRANSFER = 64 * 1024 };


			// ----------------------------------------------------
			// struct TransferAll

			struct TransferAllT
			{
				typedef size_t	result_type;

				result_type operator()(size_t min) const
				{
					return DEFAULT_MAX_TRANSFER;
				}
			};


			// ----------------------------------------------------
			// class TransferAtLeat

			class TransferAtLeatT
			{
			public:
				typedef size_t	result_type;

			private:
				size_t min_;

			public:
				explicit TransferAtLeatT(size_t min)
					: min_(min)
				{}

			public:
				result_type operator()(size_t min) const
				{
					return min_;
				}
			};
		}


		// 传输条件

		inline detail::TransferAllT TransferAll()
		{
			return detail::TransferAllT();
		}

		inline detail::TransferAtLeatT TransferAtLeast(size_t min)	
		{
			return detail::TransferAtLeatT(min);
		}
	}
}








#endif