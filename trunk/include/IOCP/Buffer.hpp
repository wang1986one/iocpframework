#ifndef __IOCP_BUFFER_HPP
#define __IOCP_BUFFER_HPP



#include "Object.hpp"


#pragma warning(disable: 4996) // ȥ��

namespace async
{


	namespace IOCP
	{

		//-------------------------------------------------------------------
		// class BufferT
		// �ṩ����������֧��STL��׼�ӿ�

		template<typename T, size_t __DefaultSize, typename AllocT>
		class BufferT
			: public Object
		{
		public:
			typedef T			value_type;
			typedef T*			pointer;
			typedef const T*	const_pointer;
			typedef T&			reference;
			typedef const T&	const_reference;

			// �ڴ��������
			typedef typename AllocT::alloc_type alloc_type;

		private:
			// �����ڴ��
			static alloc_type BufferPool;
			AllocT alloc_;		// ������

			size_t allocSize_;	// �ѷ��仺������С
			size_t bufferSize_;	// ��ʹ�û�������С

			value_type *buffer_;// ������ָ��
			bool internal_;		// �Ƿ��ⲿ�ṩ������

		public:
			BufferT()
				: alloc_(BufferPool)
				, allocSize_(__DefaultSize)
				, bufferSize_(0)
				, buffer_(_Allocate(allocSize_))
				, internal_(true)
			{}
			explicit BufferT(size_t nSize)
				: alloc_(BufferPool)
				, allocSize_(nSize)
				, bufferSize_(0)
				, internal_(true)
			{
				if( nSize < 4 )
					allocSize_ = 4;

				buffer_ = _Allocate(allocSize_);
			}
			BufferT(pointer pStr, size_t nSize)
				: alloc_(BufferPool)
				, allocSize_(nSize)
				, bufferSize_(nSize)
				, buffer_(pStr)
				, internal_(false)
			{
			}
			BufferT(pointer beg, const_pointer end)
				: alloc_(BufferPool)
				, allocSize_(std::distance(beg, end))
				, bufferSize_(allocSize_)
				, buffer_(beg)
				, internal_(false)
			{
			}

			~BufferT()
			{
				// ���Ϊ�ڲ����仺����������Ҫ�Զ��ͷ�
				if( internal_ )
					alloc_.deallocate(buffer_, allocSize_);
			}

		public:
			pointer begin()
			{
				return buffer_;
			}
			const_pointer begin() const
			{
				return buffer_;
			}

			pointer end()
			{
				return buffer_ + bufferSize_;
			}
			const_pointer end() const
			{
				return buffer_ + bufferSize_;
			}

			size_t size() const
			{
				return bufferSize_;
			}
			size_t allocSize() const
			{
				return allocSize_;
			}

			void resize(size_t nNewSize)
			{
				if( !internal_ )
					return;

				if( nNewSize <= allocSize_ )
					bufferSize_ = nNewSize;
				else
				{
					// �����»�����
					allocSize_ = nNewSize;
					pointer pNewBuf = _Allocate(allocSize_);

					// ���ƻ�����
					std::copy(buffer_, buffer_ + bufferSize_, pNewBuf);

					// �ͷžɻ�����
					alloc_.deallocate(buffer_, allocSize_);

					buffer_ = pNewBuf;
				}
			}

			pointer data(size_t offset = 0)
			{
				assert(offset < allocSize_);
				return buffer_ + offset;
			}
			const_pointer data(size_t offset = 0) const
			{
				assert(offset < allocSize_);
				return buffer_ + offset;
			}

			pointer operator[](size_t offset)
			{
				return buffer_ + offset;
			}
			const_pointer operator[](size_t offset) const
			{
				return buffer_ + offset;
			}

		private:
			pointer _Allocate(size_t nSize)
			{
				pointer pBuf = alloc_.allocate(nSize);
				std::uninitialized_fill_n(pBuf, nSize, 0);

				return pBuf;
			}
		};

		template<typename T, size_t __DefaultSize, typename AllocT>
		typename BufferT<T, __DefaultSize, AllocT>::alloc_type BufferT<T, __DefaultSize, AllocT>::BufferPool;


	} // end of IOCP


} // end of async





#endif