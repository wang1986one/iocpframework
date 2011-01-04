#ifndef __CONVERSION_NUM_TO_STRING_HPP
#define __CONVERSION_NUM_TO_STRING_HPP



namespace DataStructure
{
	/*
		类型				大小（包含结束符）
		8位有符号		5
		16位有符号		7
		32位有符号		12
		64位有符号		21
	*/

	// 可以容纳有符号整型最大值
	enum { MAX_INT_STRING_SIZE = 21 };

	template<typename T>
	inline const T *GetDigitCharachter()
	{
		static const T characters[] = 
		{
			'9',   '8',   '7',   '6',   '5',   '4',   '3',   '2',   '1',   '0',   
			'1',   '2',   '3',   '4',   '5',   '6',   '7',   '8',   '9'
		};

		return characters + 9;
	}

	// 无符号数字转换
	template<typename T, typename I>
	inline const T *UnsignedInteger2StringHelper(T *buf, size_t size, I val)
	{
		// 设置结束符
		T *psz = buf + size - 1;
		*psz = 0;

		do 
		{
			// 获取末位的数字
			I index = static_cast<I>(val % 10);
			// 取整
			val = static_cast<I>(val / 10);

			--psz;

			*psz = GetDigitCharachter<T>()[index];
		} while (val != 0);

		return psz;
	}
	template<typename T, typename I>
	inline const T *UnsignedInteger2StringHelper(T *buf, size_t size, I val, size_t &retSize)
	{
		const T *psz = UnsignedInteger2StringHelper<T, I>(buf, size, val);

		retSize = size - (psz - (buf - 1));

		return psz;
	}

	// 有符号数字转换
	template<typename T, typename I>
	inline const T *SignedInteger2StringHelper(T *buf, size_t size, I i)
	{
		const T *psz = UnsignedInteger2StringHelper(buf, size, i);
		
		if( i < 0 )
		{
			*const_cast<T *>(--psz) = T('-');
		}

		return psz;
	}
	template<typename T, typename I>
	inline const T *SignedInteger2StringHelper(T *buf, size_t size, I i, size_t &retSize)
	{
		const T *psz = SignedInteger2StringHelper<T, I>(buf, size, i);

		retSize = size - (psz - (buf - 1));

		return psz;
	}



	// Num2Str--需要外部提供缓冲区

	// char / unsigned char
	template<typename T>
	inline const T *Num2Str(T *buf, size_t size, char val)
	{
		return SignedInteger2StringHelper(buf, size, val);
	}
	template<typename T>
	inline const T *Num2Str(T *buf, size_t size, unsigned char val)
	{
		return UnsignedInteger2StringHelper(buf, size, val);
	}

	// short / unsigned short
	template<typename T>
	inline const T *Num2Str(T *buf, size_t size, short val)
	{
		return SignedInteger2StringHelper(buf, size, val);
	}
	template<typename T>
	inline const T *Num2Str(T *buf, size_t size, unsigned short val)
	{
		return UnsignedInteger2StringHelper(buf, size, val);
	}

	// int / unsigned int
	template<typename T>
	inline const T *Num2Str(T *buf, size_t size, int val)
	{
		return SignedInteger2StringHelper(buf, size, val);
	}
	template<typename T>
	inline const T *Num2Str(T *buf, size_t size, unsigned int val)
	{
		return UnsignedInteger2StringHelper(buf, size, val);
	}

	// long / unsigned long
	template<typename T>
	inline const T *Num2Str(T *buf, size_t size, long val)
	{
		return SignedInteger2StringHelper(buf, size, val);
	}
	template<typename T>
	inline const T *Num2Str(T *buf, size_t size, unsigned long val)
	{
		return UnsignedInteger2StringHelper(buf, size, val);
	}

	// char / unsigned char
	template<typename T, size_t size>
	inline const T *Num2Str(T (&buf)[size], char val)
	{
		return SignedInteger2StringHelper(buf, size, val);
	}
	template<typename T, size_t size>
	inline const T *Num2Str(T (&buf)[size], unsigned char val)
	{
		return UnsignedInteger2StringHelper(buf, size, val);
	}

	// short / unsigned short
	template<typename T, size_t size>
	inline const T *Num2Str(T (&buf)[size], short val)
	{
		return SignedInteger2StringHelper(buf, size, val);
	}
	template<typename T, size_t size>
	inline const T *Num2Str(T (&buf)[size], unsigned short val)
	{
		return UnsignedInteger2StringHelper(buf, size, val);
	}

	// int / unsigned int
	template<typename T, size_t size>
	inline const T *Num2Str(T (&buf)[size], int val)
	{
		return SignedInteger2StringHelper(buf, size, val);
	}
	template<typename T, size_t size>
	inline const T *Num2Str(T (&buf)[size], unsigned int val)
	{
		return UnsignedInteger2StringHelper(buf, size, val);
	}

	// long / unsigned long
	template<typename T, size_t size>
	inline const T *Num2Str(T (&buf)[size], long val)
	{
		return SignedInteger2StringHelper(buf, size, val);
	}
	template<typename T, size_t size>
	inline const T *Num2Str(T (&buf)[size], unsigned long val)
	{
		return UnsignedInteger2StringHelper(buf, size, val);
	}

	// __int64 / unsigned __int64
	template<typename T, size_t size>
	inline const T *Num2Str(T (&buf)[size], __int64 val)
	{
		return SignedInteger2StringHelper(buf, size, val);
	}
	template<typename T, size_t size>
	inline const T *Num2Str(T (&buf)[size], unsigned __int64 val)
	{
		return UnsignedInteger2StringHelper(buf, size, val);
	}



	// 利用TLS，不需要外部提供缓冲区，且线程安全

	template<typename T, size_t size>
	inline T *TLSBuf()
	{
		__declspec(thread) static T buf[size];
		return buf;
	}

	template<typename T, typename I>
	inline const T *Num2Str(I val)
	{
		const size_t size = MAX_INT_STRING_SIZE;

		T *buf = TLSBuf<T, size>();

		return Num2Str(buf, size, val);
	}



	//--------------------------------------------------------
	// class Num2StringProxy
	// 

	template<typename T, typename I>
	class _Num2StringProxy
	{
	public:
		typedef T		char_type;
		typedef I		NumberType;

	private:
		const char_type *const m_pResult;
		char_type m_buf[MAX_INT_STRING_SIZE];

	public:
		_Num2StringProxy(NumberType val)
			: m_pResult(Num2Str(m_buf, val))
		{}
		_Num2StringProxy(const _Num2StringProxy &rhs)
			: m_pResult(m_buf)
		{
			char_type *pDest = m_buf;
			const char_type *pSrc = rhs.m_pResult;

			for(; 0 != (*pDest++ = *pSrc++); )
				;
		}
	
	private:
		_Num2StringProxy &operator=(const _Num2StringProxy &);

	public:
		operator const char_type *() const
		{
			return m_pResult;
		}
	};


	// 使用代理类，返回代理类的实例
	template<typename T, typename I>
	inline _Num2StringProxy<T, I> Num2StrProxy(I val)
	{
		return _Num2StringProxy<T, I>(val);
	}
}



#endif