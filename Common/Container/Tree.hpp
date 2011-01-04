#ifndef __TREE_HPP
#define __TREE_HPP





namespace DataStructure
{
	template<typename T>
	struct __BinaryTreeNode
	{
		T m_data;
		__BinaryTreeNode *m_pParent;
		__BinaryTreeNode *m_pLeft;
		__BinaryTreeNode *m_pRight;
		int	m_nHeight;

		__BinaryTreeNode(const T &data, __BinaryTreeNode *pParent, __BinaryTreeNode *pLeft, __BinaryTreeNode *pRight, int nHeight = 0)
			: m_data(data)
			, m_pParent(pParent)
			, m_pLeft(pLeft)
			, m_pRight(pRight)
			, m_nHeight(nHeight)
		{}
	};

	enum {Preorder = 1, Inorder, Postorder} TraverseMode;

	template<typename T, typename Ref, typename Ptr>
	struct __BinaryTreeIterator
	{
		typedef __BinaryTreeIterator<T, Ref, Ptr>			Self;
		typedef __BinaryTreeIterator<T, T&, T*>				iterator;
		typedef __BinaryTreeIterator<T, const T&, const T*> const_iterator;

		// δ�̳�iterator�����Ա����Լ�д�����Ҫ�ĵ���������
		typedef BidirectionalIteratorTag	iterator_category;
		typedef T							value_type;
		typedef Ptr							pointer;
		typedef Ref							reference;
		typedef ptrdiff_t				difference_type;
		typedef size_t						size_type;
		typedef __BinaryTreeNode<T>*		tree_node;				

	public:
		tree_node m_pNode;
		static int m_mode;

	public:
		__BinaryTreeIterator()
			: m_pNode(NULL)
		{}
		__BinaryTreeIterator(tree_node p)
			: m_pNode(p)
		{}
		__BinaryTreeIterator(const iterator &rhs)
			: m_pNode(rhs.m_pNode)
		{}
		Self operator=(const Self &rhs)
		{
			if( this != &rhs )
			{
				m_pNode = rhs.m_pNode;
			}

			return *this;
		}

	public:
		static void SetTraverseMode(int mode)
		{
			m_mode = mode;
		}

	public:
		// ��������Ҫ�Ĳ���
		bool operator==(const Self &rhs)
		{
			if( m_pNode == NULL || rhs.m_pNode == NULL )
				return false;
			if( m_pNode == NULL && rhs.m_pNode == NULL )
				return true;

			return m_pNode->m_data == rhs.m_pNode->m_data;
		}
		bool operator!=(const Self &rhs)
		{
			return !operator==(rhs);
		}

		// ������ȡֵ(dereference)��ȡ���ǽڵ��ֵ
		reference operator*() const
		{
			return (*m_pNode).m_data;
		}
		tree_node operator->() const
		{
			return m_pNode;
		}

		// �������ۼ�
		Self &operator++()
		{
			
			/*switch(m_mode)
			{
			case Preorder:
				{
					if( m_pNode->m_pLeft != NULL )
						m_pNode = m_pNode->m_pLeft;
					else if( m_pNode->m_pRight != NULL )
						m_pNode = m_pNode->m_pRight;
					else
						m_pNode = m_pNode->m_pParent;
				}
				break;
			case Inorder:
				{
					if( m_pNode->m_pLeft != NULL )
						m_pNode = m_pNode->m_pLeft;
					else
						m_pNode = m_pNode->m_pRight;
				}
				break;
			case Postorder:
				{
					if( m_pNode->m_pLeft != NULL )
						m_pNode = m_pNode->m_pLeft;
					else
						m_pNode = m_pNode->m_pRight;
				}
				break;
			}*/
			//m_pNode = reinterpret_cast<tree_node>((*m_pNode).m_pRight);
			return *this;
		}
		Self operator++(int)
		{
			Self tmp = *this;
			++*this;
			return tmp;
		}

		// �������ۼ�
		Self &operator--()
		{
			//m_pNode = reinterpret_cast<tree_node>((*m_pNode).m_pLeft);
			return *this;
		}
		Self operator--(int)
		{
			Self tmp = *this;
			--*this;

			return tmp;
		}
	};

	template<typename T, typename Ref, typename Ptr> 
	int __BinaryTreeIterator<T, Ref, Ptr>::m_mode = Preorder;

	template<typename T, typename Compare = Less<T>, typename Alloc = Allocator<T> >
	class BinaryTree
	{
	private:
		typedef void *void_pointer;
		typedef __BinaryTreeNode<T> tree_node;

	public:
		typedef T					value_type;
		typedef value_type*			pointer;
		typedef const value_type*	const_pointer;
		typedef value_type&			reference;
		typedef const value_type&	const_reference;
		typedef tree_node*			tree_type;
		typedef size_t				size_type;
		typedef ptrdiff_t		difference_type;

		typedef __BinaryTreeIterator<T, T&, T*> iterator;
		typedef __BinaryTreeIterator<T, const T&, const T*> const_iterator;

		typedef ReverseIterator<iterator>		ReverseIteratorType;
		typedef ReverseIterator<const_iterator>	ConstReverseIteratorType;

	private:
		tree_type m_pRoot;
		size_type m_count;
		Compare m_comp;

	public:
		BinaryTree()
			: m_pRoot(NULL)
			, m_count(0)
		{}
		template<typename InputIterator>
		BinaryTree(InputIterator first, InputIterator last)
			: m_pRoot(NULL)
			, m_count(0)
		{
			while(first != last)
				insert(*first++);
		}
		BinaryTree(const BinaryTree &rhs)
			: m_pRoot(NULL)
			, m_count(0)
		{
			m_pRoot = _Clone(rhs.m_pRoot);
			m_count = rhs.m_count;
		}
		BinaryTree &operator=(const BinaryTree &rhs)
		{
			if( &rhs != this )
			{
				_Clear();
				m_pRoot = _Clone(rhs.m_pRoot);
				m_count = rhs.m_count;
			}

			return *this;
		}
		~BinaryTree()
		{
			_Clear(m_pRoot);
		}


	public:
		iterator begin()
		{
			return reinterpret_cast<tree_type>(m_pRoot);
		}
		const_iterator begin() const
		{
			return reinterpret_cast<tree_type>(m_pRoot);
		}

		iterator end()
		{
			return NULL;
		}
		const_iterator end() const
		{
			return NULL;
		}

		/*ReverseIteratorType RBegin()
		{
			return ReverseIteratorType(end());
		}
		ConstReverseIteratorType RBegin() const
		{
			return ConstReverseIteratorType(end());
		}

		ReverseIteratorType REnd()
		{
			return ReverseIteratorType(begin());
		}
		ConstReverseIteratorType REnd() const
		{
			return ConstReverseIteratorType(begin());
		}*/

		void SetTraverseMode(int nMode)
		{
			iterator::SetTraverseMode(nMode);
		}

		size_type size() const
		{
			return m_count;
		}

		size_t Height(iterator pNode) const
		{
			return pNode == NULL ? -1 : pNode->m_nHeight;
		}

		const T &Left(iterator pNode) const
		{
			return pNode->m_pLeft->m_data;
		}

		const T &Right(iterator pNode) const
		{
			return pNode->m_pRight->m_data;
		}

		const T &Parent(iterator pNode) const
		{
			return pNode->m_pParent->m_data;
		}

		const T &Max() const
		{
			tree_type pNode = _Max(m_pRoot);

			if( pNode != NULL )
				return pNode->m_data;
			else
				return m_pRoot->m_data;
		}
		const T &Min() const
		{
			tree_type pNode = _Min(m_pRoot);

			if( pNode != NULL )
				return pNode->m_data;
			else
				return m_pRoot->m_data;
		}

		// �������е����Ƿ����
		bool Contains(const T &val) const
		{
			return _Contains(val, m_pRoot);
		}

		iterator Find(const T &val) const
		{
			return _Find(val, m_pRoot);
		}

		bool empty() const
		{
			return (m_pRoot == NULL) ? true : false;
		}


		void clear()
		{
			return _Clear(m_pRoot);
		}

		void insert(const T &val)
		{
			return _Insert(val, m_pRoot, m_pRoot);
		}

		void Remove(const T &val)
		{
			return _Remove(val, m_pRoot);
		}


	private:
		tree_type _GetNode()
		{
			++m_count;

			return reinterpret_cast<tree_type>(Alloc::Allocate(sizeof(tree_node)));
		}
		tree_type _CreateBinaryNode(const T &val, tree_type pParent, tree_type pLeft, tree_type pRight)
		{
			tree_type pNode = _GetNode();

			try
			{
				__Construct(pNode, val, pParent, pLeft, pRight);
			}
			catch(...)
			{
				__Destroy(pNode);
			}

			return pNode;
		}
		void _DestroyBinartNode(tree_type &pNode)
		{
			--m_count;

			__Destroy(pNode);
			pNode = NULL;
		}
			
		// ��ֵ��������,����������������
		void _Insert(const T &val, tree_type &pParent, tree_type &pNode)
		{
			if( pNode == NULL )
			{
				pNode = _CreateBinaryNode(val, pParent, NULL, NULL);
			}
			else if( m_comp(val, pNode->m_data) )
				_Insert(val, pNode, pNode->m_pLeft);
			else if( m_comp(pNode->m_data, val) )
				_Insert(val, pNode, pNode->m_pRight);
			else
				// �ظ�Ԫ�Ĳ������ͨ���ڽڵ��¼�б���һ�������ֶ���ָʾ�����ݵĳ���Ƶ��
				;	// Duplicate, do nothing

		}

		void _Remove(const T &val, tree_type &pNode)
		{
			if( pNode == NULL )
				return;		// not found

			if( m_comp(val, pNode->m_data) )
				_Remove(val, pNode->m_pLeft);
			else if( m_comp(pNode->m_data, val) )
				_Remove(val, pNode->m_pRight);
			else if( pNode->m_pLeft != NULL && pNode->m_pRight != NULL )	// Two children
			{
				// �����������ӵĽڵ�ʱ��ɾ��������ʹ��������������С���ݴ���ýڵ�����ݲ��ݹ��ɾ���Ǹ��ڵ�
				pNode->m_data = _Min(pNode->m_pRight)->m_data;
				_Remove(pNode->m_data, pNode->m_pRight);
			}
			else
			{
				// ֻ��һ�����ӣ���ֻ��Ҫ�Ѷ���������������
				tree_type pOldNode = pNode;
				pNode = (pNode->m_pLeft != NULL ) ? pNode->m_pLeft : pNode->m_pRight;
				pNode->m_pParent = pOldNode->m_pParent;

				_DestroyBinartNode(pOldNode);
			}
		}

		tree_type _Max(tree_type pNode) const
		{
			while( pNode != NULL && pNode->m_pRight != NULL )
			{
				pNode = pNode->m_pRight;
			}

			return pNode;


			/*if( pNode == NULL )
				return NULL;

			if( pNode->m_pRight == NULL )
				return pNode;

			return _Max(pNode->m_pRight);*/
		}


		tree_type _Min(tree_type pNode) const
		{
			while( pNode != NULL && pNode->m_pLeft != NULL )
			{
				pNode = pNode->m_pLeft;
			}

			return pNode;


			/*if( pNode == NULL )
				return NULL;

			if( pNode->m_pLeft == NULL )
				return pNode;

			return _Min(pNode->m_pLeft);*/
		}

		bool _Contains(const T &val, tree_type pNode) const
		{
			while(pNode != NULL )
			{
				if( m_comp(val, pNode->m_data) )			// ������
					pNode = pNode->m_pLeft;
				else if( m_comp(pNode->m_data, val) )		// ������
					pNode = pNode->m_pRight;
				else
					return true;
			}

			return false;

			// ʹ�õݹ飬ջ�ռ�ΪO(logN)

			//if( pNode == NULL )
			//	return false;
			//else if( m_comp(val, pNode->m_data) )			// ����������
			//	return _Contains(val, pNode->m_pLeft);		
			//else if( m_comp(val, pNode->m_pRight) )			// ����������
			//	return _Contains(val, pNode->m_pRight);
			//else
			//	return true;								// ����
		}

		iterator _Find(const T &val, tree_type pNode) const
		{
			while(pNode != NULL )
			{
				if( m_comp(val, pNode->m_data) )			// ������
					pNode = pNode->m_pLeft;
				else if( m_comp(pNode->m_data, val) )		// ������
					pNode = pNode->m_pRight;
				else
					break;
			}

			return pNode;
		}

		void _Clear(tree_type &pNode)
		{
			if( pNode != NULL )
			{
				_Clear(pNode->m_pLeft);
				_Clear(pNode->m_pRight);

				_DestroyBinartNode(pNode);
			}
		}

		tree_type _Clone(tree_type pNode)
		{
			if( pNode == NULL )
				return NULL;

			return _CreateBinaryNode(pNode->m_data, pNode->m_pParent, _Clone(pNode->m_pLeft), _Clone(pNode->m_pRight));
		}

	};



	//----AVL Tree----------------

	template<typename T, typename Compare = Less<T>, typename Alloc = Allocator<T> >
	class AVLTree
	{
	private:
		typedef void *void_pointer;
		typedef __BinaryTreeNode<T> tree_node;

	public:
		typedef T					value_type;
		typedef value_type*			pointer;
		typedef const value_type*	const_pointer;
		typedef value_type&			reference;
		typedef const value_type&	const_reference;
		typedef tree_node*			tree_type;
		typedef size_t				size_type;
		typedef ptrdiff_t		difference_type;

		typedef __BinaryTreeIterator<T, T&, T*> iterator;
		typedef __BinaryTreeIterator<T, const T&, const T*>		const_iterator;

		typedef ReverseIterator<iterator>						ReverseIteratorType;
		typedef ReverseIterator<const_iterator>					ConstReverseIteratorType;

	private:
		tree_type m_pRoot;
		size_type m_count;
		Compare m_comp;

	public:
		AVLTree()
			: m_pRoot(NULL)
			, m_count(0)
		{}
		template<typename InputIterator>
		AVLTree(InputIterator first, InputIterator last)
			: m_pRoot(NULL)
			, m_count(0)
		{
			while( first != last )
				insert(*first++);
		}
		AVLTree(const AVLTree &rhs)
			: m_pRoot(NULL)
			, m_count(0)
		{
			m_pRoot = _Clone(rhs.m_pRoot);
			m_count = rhs.m_count;
		}
		AVLTree &operator=(const AVLTree &rhs)
		{
			if( &rhs != this )
			{
				_Clear();
				m_pRoot = _Clone(rhs.m_pRoot);
				m_count = rhs.m_count;
			}

			return *this;
		}
		~AVLTree()
		{
			_Clear(m_pRoot);
		}


	public:
		iterator begin()
		{
			return reinterpret_cast<tree_type>(m_pRoot);
		}
		const_iterator begin() const
		{
			return reinterpret_cast<tree_type>(m_pRoot);
		}

		iterator end()
		{
			return NULL;
		}
		const_iterator end() const
		{
			return NULL;
		}

		/*ReverseIteratorType RBegin()
		{
		return ReverseIteratorType(end());
		}
		ConstReverseIteratorType RBegin() const
		{
		return ConstReverseIteratorType(end());
		}

		ReverseIteratorType REnd()
		{
		return ReverseIteratorType(begin());
		}
		ConstReverseIteratorType REnd() const
		{
		return ConstReverseIteratorType(begin());
		}*/

		void SetTraverseMode(int nMode)
		{
			iterator::SetTraverseMode(nMode);
		}

		size_type size() const
		{
			return m_count;
		}

		const T &Left(iterator pNode) const
		{
			return pNode->m_pLeft->m_data;
		}

		const T &Right(iterator pNode) const
		{
			return pNode->m_pRight->m_data;
		}

		const T &Parent(iterator pNode) const
		{
			return pNode->m_pParent->m_data;
		}

		const T &Max() const
		{
			tree_type pNode = _Max(m_pRoot);

			if( pNode != NULL )
				return pNode->m_data;
			else
				throw underflow_error("AVL Tree Max empty");
		}
		const T &Min() const
		{
			tree_type pNode = _Min(m_pRoot);

			if( pNode != NULL )
				return pNode->m_data;
			else
				throw underflow_error("AVL Tree Min empty");
		}

		// �������е����Ƿ����
		bool Contains(const T &val) const
		{
			return _Contains(val, m_pRoot);
		}

		iterator Find(const T &val) const
		{
			return _Find(val, m_pRoot);
		}

		bool empty() const
		{
			return (m_pRoot == NULL) ? true : false;
		}


		void clear()
		{
			return _Clear(m_pRoot);
		}

		void insert(const T &val)
		{
			return _Insert(val, m_pRoot, m_pRoot);
		}

		void Remove(const T &val)
		{
			return _Remove(val, m_pRoot);
		}


	private:
		tree_type _GetNode()
		{
			++m_count;

			return reinterpret_cast<tree_type>(Alloc::Allocate(sizeof(tree_node)));
		}
		template<typename U>
		tree_type _CreateBinaryNode(const T &val, tree_type pParent, tree_type pLeft, tree_type pRight, U nHeight)
		{
			tree_type pNode = _GetNode();

			try
			{
				__Construct(pNode, val, pParent, pLeft, pRight, nHeight);
			}
			catch(...)
			{
				__Destroy(pNode);
			}

			return pNode;
		}
		void _DestroyBinartNode(tree_type &pNode)
		{
			--m_count;

			__Destroy(pNode);
			pNode = NULL;
		}

		// ��ֵ��������,����������������
		void _Insert(const T &val, tree_type &pParent, tree_type &pNode)
		{
			if( pNode == NULL )
			{
				pNode = _CreateBinaryNode(val, pParent, NULL, NULL, 0);
			}
			else if( m_comp(val, pNode->m_data) )
			{
				_Insert(val, pNode, pNode->m_pLeft);
			
				// �������������߶����2
				if( _Height(pNode->m_pLeft) - _Height(pNode->m_pRight) == 2 )
				{
					// ���ӽڵ��������
					if( m_comp(val, pNode->m_pLeft->m_data) )
						_RotateLeftChild(pNode);
					// ���ӽڵ��������
					else
						_DoubleLeftChild(pNode);
				}
			}
			else if( m_comp(pNode->m_data, val) )
			{
				_Insert(val, pNode, pNode->m_pRight);
			
				// �������������߶����2
				if( _Height(pNode->m_pRight) - _Height(pNode->m_pLeft) == 2 )
				{
					// ���ӽڵ��������
					if( m_comp(pNode->m_pRight->m_data, val) )
						_RotateRightChild(pNode);
					// ���ӽڵ��������
					else
						_DoubleRightChild(pNode);
				}
				
			}
			else
				// �ظ�Ԫ�Ĳ������ͨ���ڽڵ��¼�б���һ�������ֶ���ָʾ�����ݵĳ���Ƶ��
				;	// Duplicate, do nothing

			pNode->m_nHeight = DataStructure::Max(_Height(pNode->m_pLeft), _Height(pNode->m_pRight)) + 1;	
		}

		void _Remove(const T &val, tree_type &pNode)
		{
			if( pNode == NULL )
				return;		// not found

			if( m_comp(val, pNode->m_data) )
				_Remove(val, pNode->m_pLeft);
			else if( m_comp(pNode->m_data, val) )
				_Remove(val, pNode->m_pRight);
			else if( pNode->m_pLeft != NULL && pNode->m_pRight != NULL )	// Two children
			{
				// �����������ӵĽڵ�ʱ��ɾ��������ʹ��������������С���ݴ���ýڵ�����ݲ��ݹ��ɾ���Ǹ��ڵ�
				pNode->m_data = _Min(pNode->m_pRight)->m_data;
				_Remove(pNode->m_data, pNode->m_pRight);
			}
			else
			{
				// ֻ��һ�����ӣ���ֻ��Ҫ�Ѷ���������������
				tree_type pOldNode = pNode;
				pNode = (pNode->m_pLeft != NULL ) ? pNode->m_pLeft : pNode->m_pRight;
				pNode->m_pParent = pOldNode->m_pParent;

				_DestroyBinartNode(pOldNode);
			}
		}

		tree_type _Max(tree_type pNode) const
		{
			while( pNode != NULL && pNode->m_pRight != NULL )
			{
				pNode = pNode->m_pRight;
			}

			return pNode;


			/*if( pNode == NULL )
			return NULL;

			if( pNode->m_pRight == NULL )
			return pNode;

			return _Max(pNode->m_pRight);*/
		}


		tree_type _Min(tree_type pNode) const
		{
			while( pNode != NULL && pNode->m_pLeft != NULL )
			{
				pNode = pNode->m_pLeft;
			}

			return pNode;


			/*if( pNode == NULL )
			return NULL;

			if( pNode->m_pLeft == NULL )
			return pNode;

			return _Min(pNode->m_pLeft);*/
		}

		int _Height(tree_type pNode) const
		{
			return pNode == NULL ? -1 : pNode->m_nHeight;
		}

		void _RotateLeftChild(tree_type &k2)
		{
			tree_type k1	= k2->m_pLeft;

			k2->m_pLeft		= k1->m_pRight;
			k1->m_pRight	= k2;
			swap(k1->m_pParent, k2->m_pParent);
			k2->m_nHeight	= DataStructure::Max(_Height(k2->m_pLeft), _Height(k2->m_pRight));
			k1->m_nHeight	= DataStructure::Max(_Height(k1->m_pLeft), k2->m_nHeight) + 1;
			k2				= k1;
		}

		void _RotateRightChild(tree_type &k1)
		{
			tree_type k2	= k1->m_pRight;

			k1->m_pRight	= k2->m_pLeft;
			k2->m_pLeft		= k1;
			swap(k1->m_pParent, k2->m_pParent);
			k1->m_nHeight	= DataStructure::Max(_Height(k1->m_pLeft), _Height(k1->m_pRight));
			k2->m_nHeight	= DataStructure::Max(_Height(k2->m_pRight), k1->m_nHeight) + 1;
			k1				= k2;
		}

		void _DoubleLeftChild(tree_type &k3)
		{
			_RotateRightChild(k3->m_pLeft);
			_RotateLeftChild(k3);
		}

		void _DoubleRightChild(tree_type &k1)
		{
			_RotateLeftChild(k1->m_pRight);
			_RotateRightChild(k1);
		}

		bool _Contains(const T &val, tree_type pNode) const
		{
			while(pNode != NULL )
			{
				if( m_comp(val, pNode->m_data) )			// ������
					pNode = pNode->m_pLeft;
				else if( m_comp(pNode->m_data, val) )		// ������
					pNode = pNode->m_pRight;
				else
					return true;
			}

			return false;

			// ʹ�õݹ飬ջ�ռ�ΪO(logN)

			//if( pNode == NULL )
			//	return false;
			//else if( m_comp(val, pNode->m_data) )			// ����������
			//	return _Contains(val, pNode->m_pLeft);		
			//else if( m_comp(val, pNode->m_pRight) )			// ����������
			//	return _Contains(val, pNode->m_pRight);
			//else
			//	return true;								// ����
		}

		iterator _Find(const T &val, tree_type pNode) const
		{
			while(pNode != NULL )
			{
				if( m_comp(val, pNode->m_data) )			// ������
					pNode = pNode->m_pLeft;
				else if( m_comp(pNode->m_data, val) )		// ������
					pNode = pNode->m_pRight;
				else
					break;
			}

			return pNode;
		}

		void _Clear(tree_type &pNode)
		{
			if( pNode != NULL )
			{
				_Clear(pNode->m_pLeft);
				_Clear(pNode->m_pRight);

				_DestroyBinartNode(pNode);
			}
		}

		tree_type _Clone(tree_type pNode)
		{
			if( pNode == NULL )
				return NULL;

			return _CreateBinaryNode(pNode->m_data, pNode->m_pParent, _Clone(pNode->m_pLeft), _Clone(pNode->m_pRight), pNode->m_nHeight);
		}

	};
}



#endif