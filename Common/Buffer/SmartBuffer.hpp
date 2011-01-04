#ifndef __BUFFER_SMART_BUFFER_HPP
#define __BUFFER_SMART_BUFFER_HPP


#include "../Basic.hpp"
#include <type_traits>


#pragma warning(push)
#pragma warning(disable: 4996)	// for std::copy

namespace DataStructure
{

	//-------------------------------------------------------
	// class SmartBuffer
	// ģ��POD���͵ı�׼����������﷨�����壬����޶ȵ�����ջ�ڴ�
	// �����ܺͶ��ڴ������ԡ�
	// �����ڴ���������ַ�ʽ:
	//		1). �����ܵĴ��Լ��ڲ����������з��䣨ջ�ڴ棩
	//		2). ���ջ�ڴ治����ͨ�������������ж��ڴ����

	template<typename T, typename AllocT = Allocator<T>, size_t Space = 256>
	class SmartBuffer
		: protected AllocT
	{
	public:
		typedef T										value_type;
		typedef AllocT									AllocatorType;
		typedef SmartBuffer<T, AllocT, Space>			ClassType;

		typedef value_type *								iterator;
		typedef const value_type*						const_iterator;
		typedef typename AllocatorType::reference		reference;
		typedef typename AllocatorType::const_reference	const_reference;
		typedef typename AllocatorType::pointer			pointer;
		typedef typename AllocatorType::const_pointer	const_pointer;

		typedef size_t									size_type;
		typedef ptrdiff_t								difference_type;


	private:
		pointer m_pBuf;
		size_type m_nItems;
		bool m_bExternal;
		value_type m_internal[Space];	// Internal Storage



	public:
		explicit SmartBuffer(size_type nItems)
			: m_pBuf((Space < nItems) ? _Allocate(nItems) : const_cast<pointer>(m_internal))
			, m_nItems((NULL != m_pBuf) ? nItems : 0)
			, m_bExternal(Space < nItems)
		{
			STATIC_ASSERT(0 != Space, Space);
			STATIC_ASSERT(std::tr1::is_pod<value_type>::value == true, value_type);

			assert(_IsValid());
		}
		~SmartBuffer()
		{
			assert(_IsValid());
			if( _IsInExternal() )
			{
				assert(NULL != m_pBuf);
				assert(m_bExternal);
				assert(&m_internal[0] != m_pBuf);

				_Deallocate(m_pBuf, m_nItems);
			}
		}

	private:
		SmartBuffer(const ClassType &);
		SmartBuffer &operator=(const ClassType &);


	public:
		operator pointer()
		{
			assert(_IsValid());
			return m_pBuf;
		}
		reference operator[](size_type index)
		{
			assert(_IsValid());

			return m_pBuf[index];
		}
		const_reference operator[](size_type index) const
		{
			assert(_IsValid());

			return m_pBuf[index];
		}
		operator const_pointer() const
		{
			assert(_IsValid());
			return m_pBuf;
		}

		pointer data()
		{
			assert(_IsValid());
			return m_pBuf;
		}
		const_pointer data() const
		{
			assert(_IsValid());
			return m_pBuf;
		}

		reference front() 
		{
			assert(_IsValid());
			return m_pBuf[0];
		}
		const_reference front() const
		{
			assert(_IsValid());
			return m_pBuf[0];
		}
		reference back()
		{
			assert(_IsValid());
			return m_pBuf[size() - 1];
		}
		const_reference back() const
		{
			assert(_IsValid());
			return m_pBuf[size() - 1];
		}

		iterator begin()
		{
			assert(_IsValid());
			return m_pBuf;
		}
		const_iterator begin() const
		{
			assert(_IsValid());
			return m_pBuf;
		}
		iterator end()
		{
			assert(_IsValid());
			return m_pBuf + m_nItems;
		}
		const_iterator end() const
		{
			assert(_IsValid());
			return m_pBuf + m_nItems;
		}

		


	public:
		size_type size() const
		{
			assert(_IsValid());
			return m_nItems;
		}
		size_type InternalSize() const
		{
			return Space;
		}
	
		bool empty() const
		{
			assert(_IsValid());
			return 0 == m_nItems;
		}

		bool resize(size_type nItems)
		{
			/*
				������ѡ��:
					1). �����ⲿ�ѻ�����
					2). ��С�ⲿ�ѻ�����
					3). �����ڲ�ջ������ָ��
					4). ��С�ڲ�ջ������ָ��
					
			*/
		
			assert(_IsValid());

			if( m_nItems < nItems )
			{
				if( _IsInExternal() )
				{
					pointer pNew = _Reallocate(m_pBuf, m_nItems, nItems);

					// ����ʧ��
					if( NULL == pNew )
						return false;

					m_pBuf = pNew;
				}
				else
				{
					if( Space < nItems )
					{
						pointer pNew = _Allocate(nItems);

						if( NULL == pNew )
							return false;

						_BlockCopy(pNew, m_pBuf, m_nItems);
						m_pBuf = pNew;

						m_bExternal = true;
					}
					else
					{
						assert(!(Space < nItems));
					}
				}
			}
			else
			{
				if( _IsInExternal() )
				{
					if( Space < nItems )
					{
						assert(Space < nItems);
					}
					else
					{
						_BlockCopy(const_cast<pointer>(&m_internal[0]), m_pBuf, nItems);
						_Deallocate(m_pBuf, m_nItems);

						m_pBuf = const_cast<pointer>(&m_internal[0]);

						m_bExternal = false;
					}
				}
				else
				{
					assert(!(Space < nItems));
				}
			}

			m_nItems = nItems;
			assert(_IsValid());
		
			return true;
		}

	private:
		static AllocatorType &_GetAllocator()
		{
			static AllocatorType allocator;

			return allocator;
		}

		static void _BlockCopy(pointer pDest, const_pointer pSrc, size_type nItems)
		{
			std::copy(pSrc, pSrc + nItems, pDest);
		}

		static void _BlockSet(pointer pDest, size_type nItems, const_reference val)
		{
			std::fill_n(pDest, nItems, val);
		}
	
	private:
		pointer _Allocate(size_type nItems, void* /*hint*/)
		{
			return reinterpret_cast<pointer>(_GetAllocator().Allocate(nItems));
		}

		pointer _Allocate(size_type nItems)
		{
			return _Allocate(nItems, NULL);
		}

		void _Deallocate(pointer p, size_type nItems)
		{
			assert(NULL != p);

			_GetAllocator().Deallocate(p, nItems);
		}

		pointer _Reallocate(pointer p, size_type nItems, size_type nNewItems)
		{
			pointer pNew = _Allocate(nNewItems, p);

			if( NULL != pNew )
			{
				_BlockCopy(pNew, p, nItems);

				_Deallocate(p, nItems);
			}

			return pNew;
		}		

		
		bool _IsInExternal() const
		{
			assert((Space < m_nItems) == (m_pBuf != &m_internal[0]));
			assert((m_pBuf != &m_internal[0]) == m_bExternal);
			assert(m_bExternal || !(Space < m_nItems));

			return m_bExternal;
		}


		bool _IsValid() const
		{
			bool bRet = true;
			
			if( Space < m_nItems && !m_bExternal )
				bRet = false;

			if( !(Space < m_nItems) && m_bExternal )
				bRet = false;

			if( m_bExternal )
			{
				if( m_pBuf == &m_internal[0] )
					bRet = false;
			}
			else
			{
				if( m_pBuf != &m_internal[0] )
					bRet = false;
			}

			return bRet;
		}

	
	};
}


#pragma warning(pop)


#endif