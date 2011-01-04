#ifndef __QUEUE_HPP
#define __QUEUE_HPP




namespace DataStructure
{

	template<typename T, typename Sequence = List<T> >
	class Queue
	{
	public:
		typedef typename Sequence::value_type		value_type;
		typedef typename Sequence::size_type		size_type;
		typedef typename Sequence::reference		reference;
		typedef typename Sequence::const_reference	const_reference;

	private:
		Sequence m_container;

	public:
		bool empty() const
		{
			return m_container.empty();
		}

		size_type size() const
		{
			return m_container.size();
		}

		reference front() 
		{
			return m_container.front();
		}
		const_reference front() const
		{
			return m_container.front();
		}

		reference back()
		{
			return m_container.back();
		}
		const_reference back() const
		{
			return m_container.back();
		}

		void Push(const value_type &x)
		{
			m_container.push_back(x);
		}
		void Pop()
		{
			m_container.PopFront();
		}

		friend bool operator==(const Queue&, const Queue &);
		friend bool operator<(const Queue&, const Queue &);
	};


	template<typename T, typename Sequence>
	bool operator==(const Queue<T, Sequence> &lhs, const Queue<T, Sequence> &rhs)
	{
		return lhs.m_container == rhs.m_container;
	}
	template<typename T, typename Sequence>
	bool operator<(const Queue<T, Sequence> &lhs, const Queue<T, Sequence> &rhs)
	{
		return lhs.m_container < rhs.m_container;
	}



	//----------------------------------------------------------------------------------------
	// class PriorityQueue
	// ��һ��ӵ��Ȩֵ�����Queue�����ڵ�Ԫ���Զ�����Ȩֵ���У�Ȩֵ���������ǰ

	template<typename T, typename Sequence = Vector<T>, typename Compare = Less<typename Sequence::value_type>>
	class PriorityQueue
	{
	public:
		typedef typename Sequence::value_type		value_type;
		typedef typename Sequence::size_type			size_type;
		typedef typename Sequence::reference		reference;
		typedef typename Sequence::const_reference	const_reference;

	private:
		Sequence m_container;	// �ײ�����
		Compare  m_comp;		// Ԫ�رȽϱ�׼

	public:
		PriorityQueue()
			: m_container()
		{}
		explicit PriorityQueue(const Compare &comp)
			: m_container()
			, m_comp(comp)
		{}
		template<typename InputIterator>
		PriorityQueue(InputIterator first, InputIterator last, const Compare &comp)
			: m_container(first, last)
			, m_comp(comp)
		{
			MakeHeap(m_container.begin(), m_container.end(), m_comp);
		}
		template<typename InputIterator>
		PriorityQueue(InputIterator first, InputIterator last)
			: m_container(first, last)
		{
			MakeHeap(m_container.begin(), m_container.end(), m_comp);
		}

	public:
		bool empty() const
		{
			return m_container.empty();
		}

		size_type size() const
		{
			return m_container.size();
		}

		const_reference Top() const
		{
			return m_container.front();
		}

		void Push(const value_type &x)
		{
			try
			{
				m_container.push_back(x);
				PushHeap(m_container.begin(), m_container.end(), m_comp);
			}
			catch(...)
			{
				m_container.clear();
			}
			
		}
		void Pop()
		{
			try
			{
				PopHeap(m_container.begin(), m_container.end(), m_comp);
				m_container.pop_back();
			}
			catch(...)
			{
				m_container.clear();
			}
		}
	};
}


#endif