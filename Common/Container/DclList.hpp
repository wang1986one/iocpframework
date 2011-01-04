#ifndef __DCL_LIST_HPP
#define __DCL_LIST_HPP


#include "../Algorithm.hpp"

namespace DataStructure
{
	// -------------------------------------------------------------------------
	// class DclListNodeBase

	class DclListNodeBase
	{
	protected:
		DclListNodeBase *m_pPrev;
		DclListNodeBase *m_pNext;

	public:
		enum InsertFront { InsertAtFront = 0 };
		enum InsertBack	 { InsertAtBack  = 1 };

	public:
		DclListNodeBase()
		{
			m_pNext = m_pNext = this;
		}
		DclListNodeBase(DclListNodeBase &head, InsertFront)
			: m_pPrev(&head)
			, m_pNext(head.m_pNext)
		{
			head.m_pNext->m_pPrev	= this;
			head.m_pNext			= this;
		}
		DclListNodeBase(DclListNodeBase &head, InsertBack)
			: m_pPrev(head.m_pPrev)
			, m_pNext(&head)
		{
			head.m_pPrev->m_pNext	= this;
			head.m_pPrev			= this;
		}

	private:
		DclListNodeBase(const DclListNodeBase &);
		DclListNodeBase &operator=(const DclListNodeBase &);

	public:
		void swap(DclListNodeBase &rhs)
		{
			m_pPrev->m_pNext = &rhs;
			m_pNext->m_pPrev = &rhs;

			rhs.m_pPrev->m_pNext = this;
			rhs.m_pNext->m_pPrev = this;

			//DataStructure::swap(m_pPrev, rhs.m_pPrev);
			//DataStructure::swap(m_pNext, rhs.m_pNext);
			std::swap(m_pPrev, rhs.m_pPrev);
			std::swap(m_pNext, rhs.m_pNext);
		}

		void InsertMeFront(DclListNodeBase &head)
		{
			m_pPrev = &head;
			m_pNext = head.m_pNext;

			head.m_pNext->m_pPrev = this;
			head.m_pNext		  = this;
		}

		void InsertMeBack(DclListNodeBase &head)
		{
			m_pNext = &head;
			m_pPrev = head.m_pPrev;

			head.m_pPrev->m_pNext	= this;
			head.m_pPrev			= this;
		}

		bool IsValid() const
		{
			return m_pPrev->m_pNext == this &&
				m_pNext->m_pPrev == this;
		}

		void erase()
		{
			assert(IsValid());

			m_pNext->m_pPrev = m_pPrev;
			m_pPrev->m_pNext = m_pNext;

			m_pPrev = m_pNext = this;
		}

		DclListNodeBase *Next() 
		{
			return m_pNext;
		}
		const DclListNodeBase *Next() const
		{
			return m_pNext;
		}

		DclListNodeBase *Prev() 
		{
			return m_pPrev;
		}
		const DclListNodeBase *Prev() const
		{
			return m_pPrev;
		}
	};	


	
	// -------------------------------------------------------------------------
	// class DclListNode

	template<typename NodeT>
	class DclListNode
		: public DclListNodeBase
	{
	public:
		typedef NodeT	value_type;

	public:
		DclListNode()
		{}
		DclListNode(DclListNodeBase &head, InsertFront)
			: DclListNodeBase(head, InsertAtFront)
		{}
		DclListNode(DclListNodeBase &head, InsertBack)
			: DclListNodeBase(head, InsertAtBack)
		{}

	public:
		value_type *Next() 
		{
			return reinterpret_cast<value_type *>(m_pNext);
		}
		const value_type *Next() const
		{
			return reinterpret_cast<const value_type *>(m_pNext);
		}

		value_type *Prev() 
		{
			return reinterpret_cast<value_type *>(m_pPrev);
		}
		const value_type *Prev() const
		{
			return reinterpret_cast<const value_type *>(m_pPrev);
		}

		value_type &data()
		{
			return *reinterpret_cast<value_type *>(this);
		}
		const value_type &data() const
		{
			return *reinterpret_cast<const value_type *>(this);
		}
	};


	// -------------------------------------------------------------------------
	// class DclListIterator

	template<typename NodeT, typename ReferenceT, typename NodePtrT>
	class DclListIterator
		: public Iterator<BidirectionalIteratorTag, NodeT>
	{
	private:
		NodePtrT m_node;

	public:
		DclListIterator(NodePtrT node)
			: m_node(node)
		{}

		template<typename RefT2, typename NPtrT2>
		DclListIterator(const DclListIterator<NodeT, RefT2, NPtrT2> &iter)
			: m_node(iter.InnerData())
		{}

	public:
		NodePtrT InnerData() const
		{
			return m_node;
		}

		template<typename RefT2, typename NPtrT2>
		bool operator==(const DclListIterator<NodeT, RefT2, NPtrT2> &iter) const
		{
			return m_node == iter.InnerData();
		}

		template<typename RefT2, typename NPtrT2>
		bool operator!=(const DclListIterator<NodeT, RefT2, NPtrT2> &iter) const
		{
			return m_node != iter.InnerData();
		}

		ReferenceT operator*() const
		{
			return *static_cast<NodeT *>(m_node);
		}

		DclListIterator &operator++()
		{
			m_node = m_node->m_pNext;
			return *this;
		}

		DclListIterator &operator--()
		{
			m_node = m_node->m_pPrev;
			return *this;
		}

		DclListIterator operator++(int)
		{
			DclListIterator iter = *this;
			operator++();

			return iter;
		}

		DclListIterator operator--(int)
		{
			DclListIterator iter = *this;
			operator--();

			return iter;
		}
	};


	// -------------------------------------------------------------------------
	// class DclList:  Doubly-circularly-linked list

	template<typename NodeT>
	class DclList
		: public DclListNodeBase
	{
	public:
		typedef NodeT			value_type;
		typedef NodeT			NodeType;
		typedef NodeT&			reference;
		typedef const NodeT&	ConstRefernce;

		typedef DclListIterator<NodeT, NodeT&, NodeT*> iterator;
		typedef DclListIterator<NodeT, const NodeT&, const NodeT*> const_iterator;
		
	public:
		bool empty() const
		{
			return m_pNext == this;
		}

		void clear()
		{
			DclListNodeBase::erase();
		}

		void PopFront()
		{
			assert(!empty());
			reinterpret_cast<DclListNodeBase *>(m_pNext)->erase();
		}

		void pop_back()
		{
			assert(!empty());
			reinterpret_cast<DclListNodeBase *>(m_pPrev)->erase();
		}

		void PushFront(NodeType *node)
		{
			node->InsertMeFront(*this);
		}

		void push_back(NodeType *node)
		{
			node->InsertMeBack(*this);
		}


		NodeType &front()
		{
			assert(!empty());
			return *reinterpret_cast<NodeType *>(m_pNext);
		}
		const NodeType &front() const
		{
			assert(!empty());
			return *reinterpret_cast<const NodeType *>(m_pNext);
		}

		NodeType &back()
		{
			assert(!empty());
			return *reinterpret_cast<NodeType *>(m_pPrev);
		}
		const NodeType &back() const
		{
			assert(!empty());
			return *reinterpret_cast<const NodeType *>(m_pPrev);
		}


		NodeType *First()
		{
			return reinterpret_cast<NodeType *>(m_pNext);
		}
		const NodeType *First() const
		{
			return reinterpret_cast<const NodeType *>(m_pNext);
		}

		bool Done(const NodeType * /*node*/) const
		{
			return reinterpret_cast<const DclListNodeBase *>(m_pNext) == this;
		}
		

		iterator begin() 
		{
			return iterator(First());
		}
		const_iterator begin() const
		{
			return const_iterator(First());
		}

		iterator end()
		{
			return iterator(reinterpret_cast<NodeType *>(this));
		}
		const_iterator end() const
		{
			return const_iterator(reinterpret_cast<const NodeType *>(this));
		}
	};
}





#endif