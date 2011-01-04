#ifndef __STRING_HPP
#define __STRING_HPP


#include "../Basic.hpp"

namespace DataStructure
{
	template<typename CharT, typename Traits = CharTraits<CharT>, typename Alloc = Allocator<CharT>>
	class BasicString
	{
	private:
		struct Rep
		{
			size_t m_nLen;			// 内部的有效数据的长度
			size_t m_nRes;			// 分配的内存长度
			size_t m_nRef;			// reference count
			bool   m_bSelFish;		// 用于标记是否为私有

			CharT *data()
			{
				// 这里(this +1)之后的地址，也就是字符串真正开始的地方
				return reinterpret_cast<CharT *>(this + 1);
			}

			CharT &operator[](size_t n)
			{
				return data()[n];
			}

			// 如果m_bSelFish(完全私有)，那么就拷贝一份返回 否则 直接将自己内部的字符串的首地址返回， 修改引用计数值
			CharT *Grab()
			{
				if( m_bSelFish )
					return Clone();

				++m_nRef;
				return data();
			}

			// 这个和普通引用计数里面的用法一致，release的过程中，修改引用计数，如果没有引用了，那么释放内存
			void Release()
			{
				if( --m_nRef == 0 )
					delete this;
			}

			CharT *Clone();
			void copy(size_t , const CharT *, size_t);
			void Move(size_t , const CharT *, size_t );
			void Set(size_t , const CharT, size_t );

			static void *operator new(size_t nSize, size_t );
			static void operator delete(void *);
			static Rep *Create(size_t);
			static bool ExcessSlop(size_t , size_t );
			static size_t FrobSize(size_t );

		private:
			Rep &operator=(const Rep &);
		};


	public:
		// types:
		typedef	Traits						TraitsType;
		typedef typename Traits::char_type	value_type;
		typedef	Alloc						AllocatorType;

		typedef size_t						size_type;
		typedef ptrdiff_t					difference_type;
		typedef CharT&						reference;
		typedef const CharT&				const_reference;
		typedef CharT*						pointer;
		typedef const CharT*				const_pointer;
		typedef pointer						iterator;
		typedef const_pointer				const_iterator;
		typedef ReverseIterator<iterator>	ReverseIteratorType;
		typedef ReverseIterator<const_iterator> ConstReverseIteratorType;


	public:
		static const size_type NPOS = static_cast<size_type>(-1);

	private:
		// 初始化为{0, 0, 1, false}
		static Rep	m_Rep;
		// 存放字符串的地方
		CharT		*m_dat;

	public:
		explicit BasicString()
			: m_dat(m_Rep.Grab()) 
		{ }
		BasicString(const BasicString& str)
			: m_dat(str._Rep()->Grab()) 
		{ }
		BasicString(const BasicString& str, size_type pos, size_type n = NPOS)
			: m_dat(m_Rep.Grab()) 
		{ Assign(str, pos, n); }
		BasicString(const CharT* s, size_type n)
			: m_dat(m_Rep.Grab()) 
		{ Assign(s, n); }
		BasicString(const CharT* s)
			: m_dat(m_Rep.Grab())
		{ Assign(s); }
		BasicString(size_type n, CharT c)
			: m_dat(m_Rep.Grab()) 
		{ Assign(n, c); }
		template<typename InputIterator>
		BasicString(InputIterator begin, InputIterator end)
			: m_dat(m_Rep.Grab()) 
		{ Assign(begin, end); }

		~BasicString()
		{ _Rep()->Release(); }

		BasicString &operator=(const BasicString &str)
		{
			if( &str != this ) 
			{ 
				_Rep()->Release(); 
				m_dat = str._Rep()->Grab(); 
			}

			return *this;
		}

	public:
		const CharT* data() const
		{ return _Rep()->data(); }

		size_type Length() const
		{ return _Rep()->m_nLen; }

		size_type size() const
		{ return _Rep()->m_nLen; }

		size_type capacity() const
		{ return _Rep()->m_nRes; }

		size_type MaxSize() const
		{ return(NPOS - 1) / sizeof(CharT); }		// XXX

		bool empty() const
		{ return size() == 0; }

		void swap(BasicString &s)
		{ 
			CharT *d	= m_dat; 
			m_dat		= s.m_dat;
			s.m_dat		= d; 
		}

		// Append
		BasicString& Append(const BasicString& str, size_type pos = 0, size_type n = NPOS)
		{ return Replace(Length(), 0, str, pos, n); }
		BasicString& Append(const CharT* s, size_type n)
		{ return Replace(Length(), 0, s, n); }
		BasicString& Append(const CharT* s)
		{ return Append(s, Traits::Length(s)); }
		BasicString& Append(size_type n, CharT c)
		{ return Replace(Length(), 0, n, c); }
		template<typename InputIterator>
		BasicString& Append(InputIterator first, InputIterator last)
		{ return Replace(_End(), _End(), first, last); }

		
		// Assign
		BasicString& Assign(const BasicString& str, size_type pos = 0, size_type n = NPOS)
		{ return Replace(0, NPOS, str, pos, n); }
		BasicString& Assign(const CharT* s, size_type n)
		{ return Replace(0, NPOS, s, n); }
		BasicString& Assign(const CharT* s)
		{ return Assign(s, Traits::Length(s)); }
		BasicString& Assign(size_type n, CharT c)
		{ return Replace(0, NPOS, n, c); }
		template<typename InputIterator>
		BasicString& Assign(InputIterator first, InputIterator last)
		{ return Replace(_Begin(), _End(), first, last); }

		// operator=
		BasicString& operator=(const CharT* s)
		{ return Assign(s); }
		BasicString& operator=(CharT c)
		{ return Assign(1, c); }

		// operator+=
		BasicString& operator+=(const BasicString& rhs)
		{ return Append(rhs); }
		BasicString& operator+=(const CharT* s)
		{ return Append(s); }
		BasicString& operator+=(CharT c)
		{ return Append(1, c); }

		// insert
		BasicString& insert(size_type pos1, const BasicString& str, size_type pos2 = 0, size_type n = NPOS)
		{ return Replace(pos1, 0, str, pos2, n); }
		BasicString& insert(size_type pos, const CharT* s, size_type n)
		{ return Replace(pos, 0, s, n); }
		BasicString& insert(size_type pos, const CharT* s)
		{ return insert(pos, s, Traits::Length(s)); }
		BasicString& insert(size_type pos, size_type n, CharT c)
		{ return Replace(pos, 0, n, c); }
		iterator insert(iterator p, CharT c)
		{ 
			size_type __o = p - _Begin();
			insert(p - _Begin(), 1, c); _SelfFish();

			return _Begin() + __o;
		}
		iterator insert(iterator p, size_type n, CharT c)
		{ 
			size_type __o = p - _Begin();
			insert(p - _Begin(), n, c); 
			_SelfFish();
		
			return _Begin() + __o; 
		}
		template<typename InputIterator>
		void insert(iterator p, InputIterator first, InputIterator last)
		{ Replace(p, p, first, last); }

		// erase
		BasicString& erase(size_type pos = 0, size_type n = NPOS)
		{ return Replace(pos, n, static_cast<size_type>(0), static_cast<CharT>(0)); }
		iterator erase(iterator p)
		{ 
			size_type __o = p - begin();
			Replace(__o, 1, 0, 0); 
			_SelfFish();
		
			return _Begin() + __o; 
		}
		iterator erase(iterator f, iterator l)
		{
			size_type __o = f - _Begin();
			Replace(__o, l-f, 0, 0);
			_SelfFish();
		
			return _Begin() + __o; 
		}

		// Replace
		BasicString& Replace(size_type pos1, size_type n1, const BasicString& str, size_type pos2 = 0, size_type n2 = NPOS);
		BasicString& Replace(size_type pos, size_type n1, const CharT* s, size_type n2);
		BasicString& Replace(size_type pos, size_type n1, const CharT* s)
		{ return Replace(pos, n1, s, Traits::Length(s)); }
		BasicString& Replace(size_type pos, size_type n1, size_type n2, CharT c);
		BasicString& Replace(size_type pos, size_type n, CharT c)
		{ return Replace(pos, n, 1, c); }
		BasicString& Replace(iterator i1, iterator i2, const BasicString& str)
		{ return Replace(i1 - _Begin(), i2 - i1, str); }
		BasicString& Replace(iterator i1, iterator i2, const CharT* s, size_type n)
		{ return Replace(i1 - _Begin(), i2 - i1, s, n); }
		BasicString& Replace(iterator i1, iterator i2, const CharT* s)
		{ return Replace(i1 - _Begin(), i2 - i1, s); }
		BasicString& Replace(iterator i1, iterator i2, size_type n, CharT c)
		{ return Replace(i1 - _Begin(), i2 - i1, n, c); }
		template<typename InputIterator>
		BasicString& Replace(iterator i1, iterator i2, InputIterator j1, InputIterator j2)
		{
			const size_type m_nLen	= Length();
			size_type pos		= i1 - _Begin();
			size_type n1			= i2 - i1;
			size_type n2			= j2 - j1;

			assert(!(pos > m_nLen));
			if( n1 > m_nLen - pos )
				n1 = m_nLen - pos;

			assert(!(m_nLen - n1 > MaxSize() - n2));

			size_t newlen = m_nLen - n1 + n2;

			if( _CheckRealloc(newlen) )
			{
				Rep *p = Rep::Create(newlen);
				p->copy(0, data(), pos);
				p->copy(pos + n2, data() + pos + n1, m_nLen -(pos + n1));
				for(; j1 != j2; ++j1, ++pos)
					Traits::Assign((*p)[pos], *j1);

				_RepUp(p);
			}
			else
			{
				_Rep()->Move(pos + n2, data() + pos + n1, m_nLen -(pos + n1));

				for(; j1 != j2; ++j1, ++pos)
					Traits::Assign((*_Rep())[pos], *j1);
			}
			_Rep()->m_nLen = newlen;

			return *this;
		}


		CharT operator[](size_type pos) const
		{
			if( pos == Length() )
				return Eos();

			// 不需要判断越界，因为是const成员，不会修改内部数据
			return data()[pos];
		}

		reference operator[](size_type pos)
		{ 
			// 可能会修改数据,所以不能再和别人共有数据
			_SelfFish(); 
			return(*_Rep())[pos]; 
		}

		reference at(size_type pos)
		{
			// 应该判断是否越界，抛出异常
			assert(!(pos >= Length()));

			return(*this)[pos];
		}
		const_reference at(size_type pos) const
		{
			assert(!(pos >= Length()));

			return data()[pos];
		}

		// CStr
		const CharT* CStr() const
		{ 
			if( Length() == 0 ) 
				return NULL;

			_Terminate(); 
			
			return data(); 
		}

		// resize
		void resize(size_type n, CharT c);
		void resize(size_type n)
		{ resize(n, Eos()); }

		// Reserver
		void reserve(size_type) 
		{ }

		// copy
		size_type copy(CharT* s, size_type n, size_type pos = 0) const;

		// Find
		size_type Find(const BasicString& str, size_type pos = 0) const
		{ return Find(str.data(), pos, str.Length()); }
		size_type Find(const CharT* s, size_type pos, size_type n) const;
		size_type Find(const CharT* s, size_type pos = 0) const
		{ return Find(s, pos, Traits::Length(s)); }
		size_type Find(CharT c, size_type pos = 0) const;

		// RFind
		size_type RFind(const BasicString& str, size_type pos = NPOS) const
		{ return RFind(str.data(), pos, str.Length()); }
		size_type RFind(const CharT* s, size_type pos, size_type n) const;
		size_type RFind(const CharT* s, size_type pos = NPOS) const
		{ return RFind(s, pos, Traits::Length(s)); }
		size_type RFind(CharT c, size_type pos = NPOS) const;

		// FindFirstOf
		size_type FindFirstOf(const BasicString& str, size_type pos = 0) const
		{ return FindFirstOf(str.data(), pos, str.Length()); }
		size_type FindFirstOf(const CharT* s, size_type pos, size_type n) const;
		size_type FindFirstOf(const CharT* s, size_type pos = 0) const
		{ return FindFirstOf(s, pos, Traits::Length(s)); }
		size_type FindFirstOf(CharT c, size_type pos = 0) const
		{ return Find(c, pos); }

		// FindLastOf
		size_type FindLastOf(const BasicString& str, size_type pos = NPOS) const
		{ return FindLastOf(str.data(), pos, str.Length()); }
		size_type FindLastOf(const CharT* s, size_type pos, size_type n) const;
		size_type FindLastOf(const CharT* s, size_type pos = NPOS) const
		{ return FindLastOf(s, pos, Traits::Length(s)); }
		size_type FindLastOf(CharT c, size_type pos = NPOS) const
		{ return RFind(c, pos); }

		// FindFirstNotOf
		size_type FindFirstNotOf(const BasicString& str, size_type pos = 0) const
		{ return FindFirstNotOf(str.data(), pos, str.Length()); }
		size_type FindFirstNotOf(const CharT* s, size_type pos, size_type n) const;
		size_type FindFirstNotOf(const CharT* s, size_type pos = 0) const
		{ return FindFirstNotOf(s, pos, Traits::Length(s)); }
		size_type FindFirstNotOf(CharT c, size_type pos = 0) const;

		// FindLastNotOf
		size_type FindLastNotOf(const BasicString& str, size_type pos = NPOS) const
		{ return FindLastNotOf(str.data(), pos, str.Length()); }
		size_type FindLastNotOf(const CharT* s, size_type pos, size_type n) const;
		size_type FindLastNotOf(const CharT* s, size_type pos = NPOS) const
		{ return FindLastNotOf(s, pos, Traits::Length(s)); }
		size_type FindLastNotOf(CharT c, size_type pos = NPOS) const;

		// SubStr
		BasicString SubStr(size_type pos = 0, size_type n = NPOS) const
		{ return BasicString(*this, pos, n); }

		// Compare
		int Compare(const BasicString& str, size_type pos = 0, size_type n = NPOS) const;
		// There is no 'strncmp' equivalent for CharT pointers.
		int Compare(const CharT* s, size_type pos, size_type n) const;
		int Compare(const CharT* s, size_type pos = 0) const
		{ return Compare(s, pos, Traits::Length(s)); }


		iterator begin() 
		{ 
			_SelfFish(); 
			return &(*this)[0];
		}
		iterator end() 
		{ 
			_SelfFish(); 
			return &(*this)[Length()]; 
		}

		const_iterator begin() const 
		{ 
			return _Begin(); 
		}
		const_iterator end() const 
		{ 
			return _End(); 
		}

		ReverseIteratorType RBegin()
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
		}


	private:
		// 得到指向内部Rep对象
		Rep *_Rep() const
		{ 
			return reinterpret_cast<Rep *>(m_dat) - 1; 
		}
		// 重新给BasicString内存存放数据m_dat赋值
		void _RepUp(Rep *p) 
		{ 
			_Rep()->Release();
			m_dat = p->data(); 
		}

		void _Unique()
		{ 
			if( _Rep()->m_nRef > 1 ) 
				_Alloc(Length(), true); 
		}
		void _SelfFish()
		{ 
			_Unique(); 
			_Rep()->m_bSelFish = true; 
		}

		static CharT Eos() 
		{ 
			return Traits::Eos(); 
		}

		void _Terminate() const
		{ 
			Traits::Assign((*_Rep())[Length()], Eos()); 
		}

		iterator _Begin() const 
		{ return &(*_Rep())[0]; }
		iterator _End() const 
		{ return &(*_Rep())[Length()]; }

		void _Alloc(size_type size, bool save);
		bool _CheckRealloc(size_type s) const;


		static size_type _Find(const CharT* ptr, CharT c, size_type xpos, size_type m_nLen);
		
	};



	
	template<typename CharT, typename Traits, typename Allocator>
	inline void * BasicString<CharT, Traits, Allocator>::Rep::operator new(size_t nSize, size_t extra)
	{
		return Allocator::Allocate(nSize + extra * sizeof(CharT));
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline void BasicString<CharT, Traits, Allocator>::Rep::operator delete(void * ptr)
	{
		Allocator::Deallocate(reinterpret_cast<CharT *>(ptr), sizeof(Rep) + reinterpret_cast<Rep *>(ptr)->m_nRes * sizeof(CharT)); 
	}

	// 分配内存大小的策略，16的倍数
	template<typename CharT, typename Traits, typename Allocator>
	inline size_t BasicString<CharT, Traits, Allocator>::Rep::FrobSize(size_t s)
	{
		size_t i = 16;
		while(i < s) 
			i *= 2;

		return i;
	}

	// 调用Allocator 分配内存， 这里的两个参数 
	// BasicString内部包含一个Rep指针对象，用于存储相关的一系列数据，另外还有用于存放char数组的对象
	// s -- sizeof(Rep) , extra --BasicString 里面char数据的长度！
	// 所以整个sizeof(BasicString)应该是 Rep的大小
	// 将BasicString::Rep::operator new()和 BasicString::Rep::Create 结合起来考察
	template<typename CharT, typename Traits, typename Allocator>
	inline typename BasicString<CharT, Traits, Allocator>::Rep *BasicString<CharT, Traits, Allocator>::Rep::Create(size_t extra)
	{
		// 得到一个适中的长度
		extra		= FrobSize(extra + 1);
		// 并不是placement new，是调用自己的new(sizeof(Rep), extra)
		// 给字符串分配内存时，BasicString添加了一个“头”，用于记录该字符串的信息
		Rep *p		= new(extra) Rep;

		// 初始化这个头的内部数据
		p->m_nRes	= extra;
		p->m_nRef	= 1;
		p->m_bSelFish = false;

		return p;
	}

	template<typename CharT, typename Traits, typename Allocator>
	CharT * BasicString<CharT, Traits, Allocator>::Rep::Clone()
	{
		// 从新分配内存
		Rep *p = Rep::Create(m_nLen);
		// 拷贝字符串
		p->copy(0, data(), m_nLen);
		p->m_nLen = m_nLen;

		return p->data();
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline bool BasicString<CharT, Traits, Allocator>::Rep::ExcessSlop(size_t s, size_t r)
	{
		// 类似FrobSize，判断s是否满足要求
		return 2 *(s <= 16 ? 16 : s) < r;
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline bool BasicString<CharT, Traits, Allocator>::_CheckRealloc(typename BasicString::size_type s) const
	{
		// 判断是否满足重新分配的条件
		s += sizeof(CharT);
		_Rep()->m_bSelFish = false;

		return(_Rep()->m_nRef > 1 || s > capacity() || Rep::ExcessSlop(s, capacity()));
	}

	template<typename CharT, typename Traits, typename Allocator>
	void BasicString<CharT, Traits, Allocator>::_Alloc(typename BasicString::size_type size, bool save)
	{
		if( !_CheckRealloc(size) )
			return;

		Rep *p = Rep::Create(size);

		if( save )
		{
			p->copy(0, data(), Length());
			p->m_nLen = Length();
		}
		else
			p->m_nLen = 0;

		_RepUp(p);
	}

	template<typename CharT, typename Traits, typename Allocator>
	BasicString<CharT, Traits, Allocator>& BasicString<CharT, Traits, Allocator>::Replace(size_type pos1, size_type n1, const BasicString& str, size_type pos2, size_type n2)
	{
		const size_t len2 = str.Length();

		if( pos1 == 0 && n1 >= Length() && pos2 == 0 && n2 >= len2 )
			return operator=(str);

		assert(!(pos2 > len2));

		if( n2 > len2 - pos2 )
			n2 = len2 - pos2;

		return Replace(pos1, n1, str.data() + pos2, n2);
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline void BasicString<CharT, Traits, Allocator>::Rep::copy(size_t pos, const CharT *s, size_t n)
	{
		if( n )
			Traits::copy(data() + pos, s, n);
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline void BasicString<CharT, Traits, Allocator>::Rep::Move(size_t pos, const CharT *s, size_t n)
	{
		if( n )
			Traits::Move(data() + pos, s, n);
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline BasicString<CharT, Traits, Allocator>&BasicString<CharT, Traits, Allocator>::Replace(size_type pos, size_type n1, const CharT* s, size_type n2)
	{
		const size_type m_nLen = Length();

		assert(!(pos > m_nLen));
		if(n1 > m_nLen - pos)
			n1 = m_nLen - pos;

		assert(!(m_nLen - n1 > MaxSize() - n2));
		size_t newlen = m_nLen - n1 + n2;

		if (_CheckRealloc(newlen) )
		{
			Rep *p = Rep::Create(newlen);

			p->copy(0, data(), pos);
			p->copy(pos + n2, data() + pos + n1, m_nLen -(pos + n1));
			p->copy(pos, s, n2);

			_RepUp(p);
		}
		else
		{
			_Rep()->Move(pos + n2, data() + pos + n1, m_nLen -(pos + n1));
			_Rep()->copy(pos, s, n2);
		}
		_Rep()->m_nLen = newlen;

		return *this;
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline void BasicString<CharT, Traits, Allocator>::Rep::Set(size_t pos, const CharT c, size_t n)
	{
		Traits::Set(data() + pos, c, n);
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline BasicString<CharT, Traits, Allocator>& BasicString<CharT, Traits, Allocator>::Replace(size_type pos, size_type n1, size_type n2, CharT c)
	{
		const size_t m_nLen = Length();
		assert(!(pos > m_nLen));

		if( n1 > m_nLen - pos )
			n1 = m_nLen - pos;

		assert(!(m_nLen - n1 > MaxSize() - n2));
		size_t newlen = m_nLen - n1 + n2;

		if( _CheckRealloc(newlen) )
		{
			Rep *p = Rep::Create(newlen);

			p->copy(0, data(), pos);
			p->copy(pos + n2, data() + pos + n1, m_nLen -(pos + n1));
			p->Set (pos, c, n2);

			_RepUp(p);
		}
		else
		{
			_Rep()->Move(pos + n2, data() + pos + n1, m_nLen -(pos + n1));
			_Rep()->Set (pos, c, n2);
		}

		_Rep()->m_nLen = newlen;

		return *this;
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline void BasicString<CharT, Traits, Allocator>::resize(size_type n, CharT c)
	{
		assert(!(n > MaxSize()));

		if( n > Length() )
			Append(n - Length(), c);
		else
			erase(n);
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline typename BasicString<CharT, Traits, Allocator>::size_type BasicString<CharT, Traits, Allocator>::copy(CharT* s, size_type n, size_type pos) const
	{
		assert(!(pos > Length()));

		if(n > Length() - pos)
			n = Length() - pos;

		Traits::copy(s, data() + pos, n);

		return n;
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline typename BasicString<CharT, Traits, Allocator>::size_type BasicString<CharT, Traits, Allocator>::Find(const CharT* s, size_type pos, size_type n) const
	{
		for(size_t xpos = pos; xpos + n <= Length(); ++xpos)
			if( Traits::Equal(data()[xpos], *s) && Traits::Compare(data() + xpos, s, n) == 0 )
				return xpos;

		return NPOS;
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline typename BasicString<CharT, Traits, Allocator>::size_type BasicString<CharT, Traits, Allocator>::_Find(const CharT* ptr, CharT c, size_type xpos, size_type m_nLen)
	{
		for(; xpos < m_nLen; ++xpos)
			if( Traits::Equal(ptr [xpos], c) )
				return xpos;

		return NPOS;
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline typename BasicString<CharT, Traits, Allocator>::size_type BasicString<CharT, Traits, Allocator>::Find(CharT c, size_type pos) const
	{
		return _Find(data(), c, pos, Length());
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline typename BasicString<CharT, Traits, Allocator>::size_type BasicString<CharT, Traits, Allocator>::
		RFind(const CharT* s, size_type pos, size_type n) const
	{
		if(n > Length())
			return NPOS;

		size_t xpos = Length() - n;
		if(xpos > pos)
			xpos = pos;

		for(++xpos; xpos-- > 0; )
			if(Traits::Equal(data()[xpos], *s)
				&& Traits::Compare(data() + xpos, s, n) == 0)
				return xpos;
		return NPOS;
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline typename BasicString<CharT, Traits, Allocator>::size_type BasicString<CharT, Traits, Allocator>::RFind(CharT c, size_type pos) const
	{
		if( 1 > Length() )
			return NPOS;

		size_t xpos = Length() - 1;
		if(xpos > pos)
			xpos = pos;

		for(++xpos; xpos-- > 0; )
			if(Traits::Equal(data()[xpos], c))
				return xpos;

		return NPOS;
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline typename BasicString<CharT, Traits, Allocator>::size_type BasicString<CharT, Traits, Allocator>::FindFirstOf(const CharT* s, size_type pos, size_type n) const
	{
		for(size_t xpos = pos; xpos < Length(); ++xpos)
			if(_Find(s, data()[xpos], 0, n) != NPOS)
				return xpos;

		return NPOS;
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline typename BasicString<CharT, Traits, Allocator>::size_type BasicString<CharT, Traits, Allocator>::FindLastOf(const CharT* s, size_type pos, size_type n) const
	{
		if( Length() == 0 )
			return NPOS;

		size_t xpos = Length() - 1;
		if( xpos > pos )
			xpos = pos;
		for(++xpos; xpos-- > 0;)
			if( _Find(s, data()[xpos], 0, n) != NPOS )
				return xpos;

		return NPOS;
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline typename BasicString<CharT, Traits, Allocator>::size_type BasicString<CharT, Traits, Allocator>::FindFirstNotOf(const CharT* s, size_type pos, size_type n) const
	{
		for(size_t xpos = pos; xpos < Length(); ++xpos)
			if(_Find(s, data()[xpos], 0, n) == NPOS)
				return xpos;

		return NPOS;
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline typename BasicString<CharT, Traits, Allocator>::size_type BasicString<CharT, Traits, Allocator>::FindFirstNotOf(CharT c, size_type pos) const
	{
		for(size_t xpos = pos; xpos < Length(); ++xpos)
			if( Traits::NotEqual(data()[xpos], c) )
				return xpos;

		return NPOS;
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline typename BasicString<CharT, Traits, Allocator>::size_type BasicString<CharT, Traits, Allocator>::FindLastNotOf(const CharT* s, size_type pos, size_type n) const
	{
		if( Length() == 0 )
			return NPOS;

		size_t xpos = Length() - 1;
		if( xpos > pos )
			xpos = pos;

		for(++xpos; xpos-- > 0;)
			if( _Find(s, data()[xpos], 0, n) == NPOS )
				return xpos;

		return NPOS;
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline typename BasicString<CharT, Traits, Allocator>::size_type BasicString<CharT, Traits, Allocator>::FindLastNotOf(CharT c, size_type pos) const
	{
		if( Length() == 0 )
			return NPOS;

		size_t xpos = Length() - 1;
		if( xpos > pos )
			xpos = pos;

		for(++xpos; xpos-- > 0;)
			if( Traits::NotEqual(data()[xpos], c) )
				return xpos;

		return NPOS;
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline int BasicString<CharT, Traits, Allocator>::Compare(const BasicString& str, size_type pos, size_type n) const
	{
		assert(!(pos > Length()));

		size_t rlen = Length() - pos;
		if(rlen > n)
			rlen = n;

		if(rlen > str.Length())
			rlen = str.Length();

		int r = Traits::Compare(data() + pos, str.data(), rlen);
		if(r != 0)
			return r;

		if(rlen == n)
			return 0;

		return(Length() - pos) - str.Length();
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline int BasicString<CharT, Traits, Allocator>::Compare(const CharT* s, size_type pos, size_type n) const
	{
		assert(!(pos > Length()));

		size_t rlen = Length() - pos;
		if(rlen > n)
			rlen = n;

		int r = Traits::Compare(data() + pos, s, rlen);
		if(r != 0)
			return r;

		return(Length() - pos) - n;
	}


	#include<iostream>

	template<typename CharT, typename Traits, typename Allocator>
	inline std::istream &operator>>(std::istream &is, BasicString<CharT, Traits, Allocator> &s)
	{
		int w = is.width(0);
		if(is.ipfx())
		{
			register streambuf *sb = is.rdbuf();
			s.resize(0);
			while(1)
			{
				int ch = sb->sbumpc();
				if(ch == EOF)
				{
					is.setstate(ios::eofbit);
					break;
				}
				else if(Traits::is_del(ch))
				{
					sb->sungetc();
					break;
				}
				s += ch;
				if(--w == 1)
					break;
			}
		}

		is.isfx();
		if(s.Length() == 0)
			is.setstate(ios::failbit);

		return is;
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline std::ostream &operator<<(std::ostream &o, const BasicString<CharT, Traits, Allocator>& s)
	{
		return o.write(s.data(), s.Length());
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline std::istream& GetLine(std::istream &is, BasicString<CharT, Traits, Allocator>& s, CharT delim)
	{
		if(is.ipfx())
		{
			size_t count = 0;
			streambuf *sb = is.rdbuf();
			s.resize(0);

			while(1)
			{
				int ch = sb->sbumpc();
				if(ch == EOF)
				{
					is.setstate(count == 0
						?(ios::failbit|ios::eofbit)
						: ios::eofbit);
					break;
				}

				++count;

				if(ch == delim)
					break;

				s += ch;

				if(s.Length() == s.NPOS - 1)
				{
					is.setstate(ios::failbit);
					break;
				}
			}
		}

		// We need to be friends with std::istream to do this.
		// is._gcount = count;
		is.isfx();

		return is;
	}

	// static data member of class BasicString<>
	template<typename CharT, typename Traits, typename Allocator>
	typename BasicString<CharT, Traits, Allocator>::Rep BasicString<CharT, Traits, Allocator>::m_Rep = { 0, 0, 1, false };

	template<typename CharT, typename Traits, typename Allocator>
	const typename BasicString<CharT, Traits, Allocator>::size_type BasicString<CharT, Traits, Allocator>::NPOS;



	// Operator
	template<typename CharT, typename Traits, typename Allocator>
	inline BasicString<CharT, Traits, Allocator> operator+(const BasicString<CharT, Traits, Allocator>& lhs, const BasicString<CharT, Traits, Allocator>& rhs)
	{
		BasicString<CharT, Traits, Allocator> str(lhs);
		str.Append(rhs);

		return str;
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline BasicString<CharT, Traits, Allocator> operator+(const CharT* lhs, const BasicString<CharT, Traits, Allocator>& rhs)
	{
		BasicString<CharT, Traits, Allocator> str(lhs);
		str.Append(rhs);

		return str;
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline BasicString<CharT, Traits, Allocator> operator+(CharT lhs, const BasicString<CharT, Traits, Allocator>& rhs)
	{
		BasicString<CharT, Traits, Allocator> str(1, lhs);
		str.Append(rhs);

		return str;
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline BasicString<CharT, Traits, Allocator> operator+(const BasicString<CharT, Traits, Allocator>& lhs, const CharT* rhs)
	{
		BasicString<CharT, Traits, Allocator> str(lhs);
		str.Append(rhs);

		return str;
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline BasicString<CharT, Traits, Allocator> operator+(const BasicString<CharT, Traits, Allocator>& lhs, CharT rhs)
	{
		BasicString<CharT, Traits, Allocator> str(lhs);
		str.Append(1, rhs);

		return str;
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline bool operator==(const BasicString<CharT, Traits, Allocator>& lhs, const BasicString<CharT, Traits, Allocator>& rhs)
	{
		return(lhs.Compare(rhs) == 0);
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline bool operator==(const CharT* lhs, const BasicString<CharT, Traits, Allocator>& rhs)
	{
		return(rhs.Compare(lhs) == 0);
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline bool operator==(const BasicString<CharT, Traits, Allocator>& lhs, const CharT* rhs)
	{
		return(lhs.Compare(rhs) == 0);
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline bool operator!=(const CharT* lhs, const BasicString<CharT, Traits, Allocator>& rhs)
	{
		return(rhs.Compare(lhs) != 0);
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline bool operator!=(const BasicString<CharT, Traits, Allocator>& lhs, const CharT* rhs)
	{
		return(lhs.Compare(rhs) != 0);
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline bool operator<(const BasicString<CharT, Traits, Allocator>& lhs,
		const BasicString<CharT, Traits, Allocator>& rhs)
	{
		return(lhs.Compare(rhs) < 0);
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline bool operator<(const CharT* lhs, const BasicString<CharT, Traits, Allocator>& rhs)
	{
		return(rhs.Compare(lhs) > 0);
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline bool operator<(const BasicString<CharT, Traits, Allocator>& lhs, const CharT* rhs)
	{
		return(lhs.Compare(rhs) < 0);
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline bool operator>(const CharT* lhs, const BasicString<CharT, Traits, Allocator>& rhs)
	{
		return(rhs.Compare(lhs) < 0);
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline bool operator>(const BasicString<CharT, Traits, Allocator>& lhs, const CharT* rhs)
	{
		return(lhs.Compare(rhs) > 0);
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline bool operator<=(const CharT* lhs, const BasicString<CharT, Traits, Allocator>& rhs)
	{
		return(rhs.Compare(lhs) >= 0);
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline bool operator<=(const BasicString<CharT, Traits, Allocator>& lhs, const CharT* rhs)
	{
		return(lhs.Compare(rhs) <= 0);
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline bool operator>=(const CharT* lhs, const BasicString<CharT, Traits, Allocator>& rhs)
	{
		return(rhs.Compare(lhs) <= 0);
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline bool operator>=(const BasicString<CharT, Traits, Allocator>& lhs, const CharT* rhs)
	{
		return(lhs.Compare(rhs) >= 0);
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline bool operator!=(const BasicString<CharT, Traits, Allocator>& lhs, const BasicString<CharT, Traits, Allocator>& rhs)
	{
		return(lhs.Compare(rhs) != 0);
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline bool operator>(const BasicString<CharT, Traits, Allocator>& lhs, const BasicString<CharT, Traits, Allocator>& rhs)
	{
		return(lhs.Compare(rhs) > 0);
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline bool operator<=(const BasicString<CharT, Traits, Allocator>& lhs, const BasicString<CharT, Traits, Allocator>& rhs)
	{
		return(lhs.Compare(rhs) <= 0);
	}

	template<typename CharT, typename Traits, typename Allocator>
	inline bool operator>=(const BasicString<CharT, Traits, Allocator>& lhs, const BasicString<CharT, Traits, Allocator>& rhs)
	{
		return(lhs.Compare(rhs) >= 0);
	}



	typedef BasicString<char>		String;
	typedef BasicString<wchar_t>	WString;

}


#endif