#ifndef __IOCP_INTERNAL_CONDITION_HPP
#define __IOCP_INTERNAL_CONDITION_HPP



namespace async
{
	namespace iocp
	{

		namespace internal
		{
			// Ĭ�ϵ��δ����ֽڴ�С
			enum { DEFAULT_MAX_TRANSFER = 64 * 1024 };


			// ----------------------------------------------------
			// struct TransferAll

			struct TransferAllT
			{
				typedef size_t	result_type;

				result_type operator()() const
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
				result_type operator()() const
				{
					return min_;
				}
			};
		}


		// ��������

		inline internal::TransferAllT TransferAll()
		{
			return internal::TransferAllT();
		}

		inline internal::TransferAtLeatT TransferAtLeast(size_t min)	
		{
			return internal::TransferAtLeatT(min);
		}
	}
}








#endif