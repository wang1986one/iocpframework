#ifndef __STATCK_HPP_H
#define __STATCK_HPP_H





namespace DataStructure
{


	template<typename T, typename Sequence = List<T> >
	class Stack
	{
	public:
		typedef typename Sequence::value_type		value_type;
		typedef typename Sequence::size_type		size_type;
		typedef typename Sequence::reference		reference;
		typedef typename Sequence::const_reference	const_reference;

	private:
		Sequence m_container;	// µ×²ãÈÝÆ÷

	public:
		bool empty() const
		{
			return m_container.empty();
		}

		size_type size() const
		{
			return m_container.size();
		}

		reference Top()
		{
			return m_container.back();
		}
		const_reference Top() const
		{
			return m_container.back();
		}

		void Push(const value_type &x)
		{
			m_container.push_back(x);
		}

		void Pop()
		{
			m_container.pop_back();
		}

		friend bool operator==(const Stack&, const Stack &);
		friend bool operator<(const Stack&, const Stack &);
	};


	template<typename T, typename Sequence>
	bool operator==(const Stack<T, Sequence> &lhs, const Stack<T, Sequence> &rhs)
	{
		return lhs.m_container == rhs.m_container;
	}
	template<typename T, typename Sequence>
	bool operator<(const Stack<T, Sequence> &lhs, const Stack<T, Sequence> &rhs)
	{
		return lhs.m_container < rhs.m_container;
	}
}







#endif