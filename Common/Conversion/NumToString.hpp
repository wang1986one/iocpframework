#ifndef __CONVERSION_NUM_TO_STRING_HPP
#define __CONVERSION_NUM_TO_STRING_HPP



namespace DataStructure
{
	/*
		����				��С��������������
		8λ�з���		5
		16λ�з���		7
		32λ�з���		12
		64λ�з���		21
	*/

	// ���������з����������ֵ
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

	// �޷�������ת��
	template<typename T, typename I>
	inline const T *UnsignedInteger2StringHelper(T *buf, size_t size, I val)
	{
		// ���ý�����
		T *psz = buf + size - 1;
		*psz = 0;

		do 
		{
			// ��ȡĩλ������
			I index = static_cast<I>(val % 10);
			// ȡ��
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

	// �з�������ת��
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



	// Num2Str--��Ҫ�ⲿ�ṩ������

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



	// ����TLS������Ҫ�ⲿ�ṩ�����������̰߳�ȫ

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


	// ʹ�ô����࣬���ش������ʵ��
	template<typename T, typename I>
	inline _Num2StringProxy<T, I> Num2StrProxy(I val)
	{
		return _Num2StringProxy<T, I>(val);
	}
}



#endif