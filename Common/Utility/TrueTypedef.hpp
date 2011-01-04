#ifndef __UTILITY_TRUE_TYPEDEF_HPP
#define __UTILITY_TRUE_TYPEDEF_HPP




namespace DataStructure
{
	// 生成一个不透明的类型
	#define GEN_OPAQUE(type) typedef struct __htype##type{ int i;} const* type; 


	// =========================================================================
	// class TrueTypedef
	//  

	template<typename T, typename U>
	class TrueTypedef
	{
	public:
		typedef T					value_type;
		typedef U					UniqueType;
		typedef T&					reference;
		typedef const T&			const_reference;

		typedef TrueTypedef<T, U>	ClassType;

	private:
		value_type m_value;

	public:
		TrueTypedef()
			: m_value(value_type())
		{}

		explicit TrueTypedef(const value_type &value)
			: m_value(value)
		{}

		TrueTypedef(const ClassType &rhs)
			: m_value(rhs.m_value)
		{}

		const ClassType& operator=(const ClassType &rhs)
		{
			if( this != &rhs )
				m_value = rhs.m_value;

			return *this;
		}

	private:
		const ClassType &operator=(const value_type &);


	public:
		reference BaseTypeValue()
		{
			return m_value;
		}
		const_reference BaseTypeValue() const
		{
			return m_value;
		}
	};


	// operator++
	template<typename T, typename U>
	inline TrueTypedef<T, U> &operator++(TrueTypedef<T, U> &v)
	{
		++v.BaseTypeValue();

		return v;
	}
	template<typename T, typename U>
	inline TrueTypedef<T, U> &operator++(TrueTypedef<T, U> &v, int)
	{
		TrueTypedef<T, U> tmp(v);

		++v.BaseTypeValue();

		return tmp;
	}

	// operator--
	template<typename T, typename U>
	inline TrueTypedef<T, U> &operator--(TrueTypedef<T, U> &v)
	{
		--v.BaseTypeValue();

		return v;
	}
	template<typename T, typename U>
	inline TrueTypedef<T, U> &operator--(TrueTypedef<T, U> &v, int)
	{
		TrueTypedef<T, U> tmp(v);

		--v.BaseTypeValue();

		return tmp;
	}


	// operator==
	template<typename T, typename U>
	inline bool operator==(const TrueTypedef<T, U> &lhs, const TrueTypedef<T, U> &rhs)
	{
		return lhs.BaseTypeValue() == rhs.BaseTypeValue();
	}
	template<typename T, typename U>
	inline bool operator==(const TrueTypedef<T, U> &lhs, const typename TrueTypedef<T, U>::value_type &rhs)
	{
		return lhs.BaseTypeValue() == rhs;
	}
	template<typename T, typename U>
	inline bool operator==(const typename TrueTypedef<T, U>::value_type &lhs, const TrueTypedef<T, U> &rhs)
	{
		return lhs == rhs.BaseTypeValue();
	}

	// operator!=
	template<typename T, typename U>
	inline bool operator!=(const TrueTypedef<T, U> &lhs, const TrueTypedef<T, U> &rhs)
	{
		return !operator==(lhs, rhs);
	}
	template<typename T, typename U>
	inline bool operator!=(const TrueTypedef<T, U> &lhs, const typename TrueTypedef<T, U>::value_type &rhs)
	{
		return !operator==(lhs, rhs);
	}
	template<typename T, typename U>
	inline bool operator!=(const typename TrueTypedef<T, U>::value_type &lhs, const TrueTypedef<T, U> &rhs)
	{
		return !operator==(lhs, rhs);
	}

	
	// operator<
	template<typename T, typename U>
	inline bool operator<(const TrueTypedef<T, U> &lhs, const TrueTypedef<T, U> &rhs)
	{
		return lhs.BaseTypeValue() < rhs.BaseTypeValue();
	}
	template<typename T, typename U>
	inline bool operator<(const TrueTypedef<T, U> &lhs, const typename TrueTypedef<T, U>::value_type &rhs)
	{
		return lhs.BaseTypeValue() < rhs;
	}
	template<typename T, typename U>
	inline bool operator<(const typename TrueTypedef<T, U>::value_type &lhs, const TrueTypedef<T, U> &rhs)
	{
		return lhs < rhs.BaseTypeValue();
	}


	// operator<=
	template<typename T, typename U>
	inline bool operator<=(const TrueTypedef<T, U> &lhs, const TrueTypedef<T, U> &rhs)
	{
		return lhs.BaseTypeValue() <= rhs.BaseTypeValue();
	}
	template<typename T, typename U>
	inline bool operator<=(const TrueTypedef<T, U> &lhs, const typename TrueTypedef<T, U>::value_type &rhs)
	{
		return lhs.BaseTypeValue() <= rhs;
	}
	template<typename T, typename U>
	inline bool operator<=(const typename TrueTypedef<T, U>::value_type &lhs, const TrueTypedef<T, U> &rhs)
	{
		return lhs <= rhs.BaseTypeValue();
	}


	// operator>
	template<typename T, typename U>
	inline bool operator>(const TrueTypedef<T, U> &lhs, const TrueTypedef<T, U> &rhs)
	{
		return lhs.BaseTypeValue() > rhs.BaseTypeValue();
	}
	template<typename T, typename U>
	inline bool operator>(const TrueTypedef<T, U> &lhs, const typename TrueTypedef<T, U>::value_type &rhs)
	{
		return lhs.BaseTypeValue() > rhs;
	}
	template<typename T, typename U>
	inline bool operator>(const typename TrueTypedef<T, U>::value_type &lhs, const TrueTypedef<T, U> &rhs)
	{
		return lhs > rhs.BaseTypeValue();
	}

	// operator>=
	template<typename T, typename U>
	inline bool operator>=(const TrueTypedef<T, U> &lhs, const TrueTypedef<T, U> &rhs)
	{
		return lhs.BaseTypeValue() >= rhs.BaseTypeValue();
	}
	template<typename T, typename U>
	inline bool operator>=(const TrueTypedef<T, U> &lhs, const typename TrueTypedef<T, U>::value_type &rhs)
	{
		return lhs.BaseTypeValue() >= rhs;
	}
	template<typename T, typename U>
	inline bool operator>=(const typename TrueTypedef<T, U>::value_type &lhs, const TrueTypedef<T, U> &rhs)
	{
		return lhs >= rhs.BaseTypeValue();
	}


	// operator+
	template<typename T, typename U>
	inline TrueTypedef<T, U> operator+(const TrueTypedef<T, U> &lhs, const TrueTypedef<T, U> &rhs)
	{
		return TrueTypedef<T, U>(lhs.BaseTypeValue() + rhs.BaseTypeValue());
	}
	template<typename T, typename U>
	inline TrueTypedef<T, U> operator+(const TrueTypedef<T, U> &lhs, const typename TrueTypedef<T, U>::value_type &rhs)
	{
		return TrueTypedef<T, U>(lhs.BaseTypeValue() + rhs);
	}
	template<typename T, typename U>
	inline TrueTypedef<T, U> operator+(const typename TrueTypedef<T, U>::value_type &lhs, const TrueTypedef<T, U> &rhs)
	{
		return TrueTypedef<T, U>(lhs + rhs.BaseTypeValue());
	}


	// operator-
	template<typename T, typename U>
	inline TrueTypedef<T, U> operator-(const TrueTypedef<T, U> &lhs, const TrueTypedef<T, U> &rhs)
	{
		return TrueTypedef<T, U>(lhs.BaseTypeValue() - rhs.BaseTypeValue());
	}
	template<typename T, typename U>
	inline TrueTypedef<T, U> operator-(const TrueTypedef<T, U> &lhs, const typename TrueTypedef<T, U>::value_type &rhs)
	{
		return TrueTypedef<T, U>(lhs.BaseTypeValue() - rhs);
	}
	template<typename T, typename U>
	inline TrueTypedef<T, U> operator-(const typename TrueTypedef<T, U>::value_type &lhs, const TrueTypedef<T, U> &rhs)
	{
		return TrueTypedef<T, U>(lhs - rhs.BaseTypeValue());
	}


	// operator*
	template<typename T, typename U>
	inline TrueTypedef<T, U> operator*(const TrueTypedef<T, U> &lhs, const TrueTypedef<T, U> &rhs)
	{
		return TrueTypedef<T, U>(lhs.BaseTypeValue() * rhs.BaseTypeValue());
	}
	template<typename T, typename U>
	inline TrueTypedef<T, U> operator*(const TrueTypedef<T, U> &lhs, const typename TrueTypedef<T, U>::value_type &rhs)
	{
		return TrueTypedef<T, U>(lhs.BaseTypeValue() * rhs);
	}
	template<typename T, typename U>
	inline TrueTypedef<T, U> operator*(const typename TrueTypedef<T, U>::value_type &lhs, const TrueTypedef<T, U> &rhs)
	{
		return TrueTypedef<T, U>(lhs * rhs.BaseTypeValue());
	}


	// operator/
	template<typename T, typename U>
	inline TrueTypedef<T, U> operator/(const TrueTypedef<T, U> &lhs, const TrueTypedef<T, U> &rhs)
	{
		return TrueTypedef<T, U>(lhs.BaseTypeValue() / rhs.BaseTypeValue());
	}
	template<typename T, typename U>
	inline TrueTypedef<T, U> operator/(const TrueTypedef<T, U> &lhs, const typename TrueTypedef<T, U>::value_type &rhs)
	{
		return TrueTypedef<T, U>(lhs.BaseTypeValue() / rhs);
	}
	template<typename T, typename U>
	inline TrueTypedef<T, U> operator/(const typename TrueTypedef<T, U>::value_type &lhs, const TrueTypedef<T, U> &rhs)
	{
		return TrueTypedef<T, U>(lhs / rhs.BaseTypeValue());
	}


	// operator%
	template<typename T, typename U>
	inline TrueTypedef<T, U> operator%(const TrueTypedef<T, U> &lhs, const TrueTypedef<T, U> &rhs)
	{
		return TrueTypedef<T, U>(lhs.BaseTypeValue() % rhs.BaseTypeValue());
	}
	template<typename T, typename U>
	inline TrueTypedef<T, U> operator%(const TrueTypedef<T, U> &lhs, const typename TrueTypedef<T, U>::value_type &rhs)
	{
		return TrueTypedef<T, U>(lhs.BaseTypeValue() % rhs);
	}
	template<typename T, typename U>
	inline TrueTypedef<T, U> operator%(const typename TrueTypedef<T, U>::value_type &lhs, const TrueTypedef<T, U> &rhs)
	{
		return TrueTypedef<T, U>(lhs % rhs.BaseTypeValue());
	}


	// operator^
	template<typename T, typename U>
	inline TrueTypedef<T, U> operator^(const TrueTypedef<T, U> &lhs, const TrueTypedef<T, U> &rhs)
	{
		return TrueTypedef<T, U>(lhs.BaseTypeValue() ^ rhs.BaseTypeValue());
	}
	template<typename T, typename U>
	inline TrueTypedef<T, U> operator^(const TrueTypedef<T, U> &lhs, const typename TrueTypedef<T, U>::value_type &rhs)
	{
		return TrueTypedef<T, U>(lhs.BaseTypeValue() ^ rhs);
	}
	template<typename T, typename U>
	inline TrueTypedef<T, U> operator^(const typename TrueTypedef<T, U>::value_type &lhs, const TrueTypedef<T, U> &rhs)
	{
		return TrueTypedef<T, U>(lhs ^ rhs.BaseTypeValue());
	}


	// operator~
	template<typename T, typename U>
	inline TrueTypedef<T, U> operator~(const TrueTypedef<T, U> &lhs)
	{
		return ~lhs.BaseTypeValue();
	}


	// operator<<
	template<typename T, typename U>
	inline TrueTypedef<T, U> operator<<(const TrueTypedef<T, U> &lhs, const TrueTypedef<T, U> &rhs)
	{
		return TrueTypedef<T, U>(lhs.BaseTypeValue() << rhs.BaseTypeValue());
	}
	template<typename T, typename U>
	inline TrueTypedef<T, U> operator<<(const TrueTypedef<T, U> &lhs, const typename TrueTypedef<T, U>::value_type &rhs)
	{
		return TrueTypedef<T, U>(lhs.BaseTypeValue() << rhs);
	}
	template<typename T, typename U>
	inline TrueTypedef<T, U> operator<<(const typename TrueTypedef<T, U>::value_type &lhs, const TrueTypedef<T, U> &rhs)
	{
		return TrueTypedef<T, U>(lhs << rhs.BaseTypeValue());
	}


	// operator>>
	template<typename T, typename U>
	inline TrueTypedef<T, U> operator>>(const TrueTypedef<T, U> &lhs, const TrueTypedef<T, U> &rhs)
	{
		return TrueTypedef<T, U>(lhs.BaseTypeValue() >> rhs.BaseTypeValue());
	}
	template<typename T, typename U>
	inline TrueTypedef<T, U> operator>>(const TrueTypedef<T, U> &lhs, const typename TrueTypedef<T, U>::value_type &rhs)
	{
		return TrueTypedef<T, U>(lhs.BaseTypeValue() >> rhs);
	}
	template<typename T, typename U>
	inline TrueTypedef<T, U> operator>>(const typename TrueTypedef<T, U>::value_type &lhs, const TrueTypedef<T, U> &rhs)
	{
		return TrueTypedef<T, U>(lhs >> rhs.BaseTypeValue());
	}


	// operator&
	template<typename T, typename U>
	inline TrueTypedef<T, U> operator&(const TrueTypedef<T, U> &lhs, const TrueTypedef<T, U> &rhs)
	{
		return TrueTypedef<T, U>(lhs.BaseTypeValue() & rhs.BaseTypeValue());
	}
	template<typename T, typename U>
	inline TrueTypedef<T, U> operator&(const TrueTypedef<T, U> &lhs, const typename TrueTypedef<T, U>::value_type &rhs)
	{
		return TrueTypedef<T, U>(lhs.BaseTypeValue() & rhs);
	}
	template<typename T, typename U>
	inline TrueTypedef<T, U> operator&(const typename TrueTypedef<T, U>::value_type &lhs, const TrueTypedef<T, U> &rhs)
	{
		return TrueTypedef<T, U>(lhs & rhs.BaseTypeValue());
	}


	// operator|
	template<typename T, typename U>
	inline TrueTypedef<T, U> operator|(const TrueTypedef<T, U> &lhs, const TrueTypedef<T, U> &rhs)
	{
		return TrueTypedef<T, U>(lhs.BaseTypeValue() | rhs.BaseTypeValue());
	}
	template<typename T, typename U>
	inline TrueTypedef<T, U> operator|(const TrueTypedef<T, U> &lhs, const typename TrueTypedef<T, U>::value_type &rhs)
	{
		return TrueTypedef<T, U>(lhs.BaseTypeValue() | rhs);
	}
	template<typename T, typename U>
	inline TrueTypedef<T, U> operator|(const typename TrueTypedef<T, U>::value_type &lhs, const TrueTypedef<T, U> &rhs)
	{
		return TrueTypedef<T, U>(lhs | rhs.BaseTypeValue());
	}
}







#endif