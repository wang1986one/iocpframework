#ifndef __META_RANGE_HPP
#define __META_RANGE_HPP

#include <Comm/Constraints.hpp>
#include <stdexcept>



namespace DataStructure
{
	//------------------------------------------------------
	// class Range

	template<long L, long U>
	class Range
	{
	private:
		long val_;

	public:
		enum 
		{
			LBOUND = L,
			UBOUND = U
		};

	public:
		Range(long v)
			: val_(v)
		{}
		template<long L2, long U2>
		Range(const Range<L2, U2> &rhs)
			: val_(rhs.get())
		{
			STATIC_ASSERT(L2 >= L && U2 <= U, rhs);
		}

		Range &operator=(long v)
		{
			val_ = _Ensure(v);
			return *this;
		}

		template<long L2, long U2>
		Range &operator=(const Range<L2, U2> &rhs)
		{
			STATIC_ASSERT(L2 >= L && U2 <= U, rhs);

			val_ = rhs.get();
			return *this;
		}

	public:
		Range &operator++()
		{
			_Ensure(++val_);

			return *this;
		}
		Range operator++(int)
		{
			Range tmp(*this);
			_Ensure(++val_);

			return tmp;
		}

		Range &operator--()
		{
			_Ensure(--val_);

			return *this;
		}
		Range operator--(int)
		{
			Range tmp(*this);
			_Ensure(--val_);

			return tmp;
		}

		long get() const 
		{
			return val_;
		}

		void set(long v)
		{
			val_ = v;
		}

	public:
		static bool Check(long v)
		{
			return v >= L && v < U;
		}

	private:
		static long _Ensure(long v)
		{
			if( !Check(v) )
				throw std::range_error("range exception");

			return v;
		}
	};

	// RangeCast
	template<long L1, long U1, long L2, long U2>
	Range<L1, U1> RangeCast(const Range<L2, U2> &rhs)
	{
		STATIC_ASSERT(L2 <= U1 && U2 >= L1, rhs);
		return Range<L1, U1>(rhs.get());
	}

	// operator++
	template<long L1, long U1, long L2, long U2>
	Range<L1 + L2, U1 + U2> operator+(const Range<L1, U1> &lhs, const Range<L2, U2> &rhs)
	{
		return Range<L1 + L2, U1 + U2>(lhs.get() + rhs.get());
	}

	// operator--
	template<long L1, long U1, long L2, long U2>
	Range<L1 - L2, U1 - U2> operator-(const Range<L1, U1> &lhs, const Range<L2, U2> &rhs)
	{
		return Range<L1 - L2, U1 - U2>(lhs.get() - rhs.get());
	}




	//--------------------------------------------------
	// class StaticMinMax
	
	template<long L1, long L2>
	struct StaticMinMax2
	{
		enum { MIN_VAL = L1 < L2 ? L1 : L2 };
		enum { MAX_VAL = L1 > L2 ? L1 : L2 };
	};

	template<long L1, long L2, long L3, long L4>
	struct StaticMinMax
	{
	private:
		enum 
		{ 
			t1 = StaticMinMax2<L1, L2>::MIN_VAL,
			t2 = StaticMinMax2<L3, L4>::MIN_VAL,
			t3 = StaticMinMax2<L1, L2>::MAX_VAL,
			t4 = StaticMinMax2<L3, L4>::MAX_VAL
		};

	public:
		enum { MIN_VAL = t1 < t2 ? t1 : t2 };
		enum { MAX_VAL = t3 > t4 ? t3 : t4 };
	};


	// operator *
	template<long L1, long U1, long L2, long U2>
	Range<
		StaticMinMax<L1 * L2, L1 * U2, U1 * L2, U1 * U2>::MIN_VAL,
		StaticMinMax<L1 * L2, L1 * U2, U1 * L2, U1 * U2>::MAX_VAL
	> operator*(const Range<L1, U1> &lhs, const Range<L2, U2> &rhs)
	{
		return Range<
			StaticMinMax<L1 * L2, L1 * U2, U1 * L2, U1 * U2>::MIN_VAL,
			StaticMinMax<L1 * L2, L1 * U2, U1 * L2, U1 * U2>::MAX_VAL
		>(lhs.get() * rhs.get());
	}


	// operator /
	template<long L1, long U1, long L2, long U2>
	Range<
		StaticMinMax<L1 / L2, L1 / U2, U1 / L2, U1 / U2>::MIN_VAL,
		StaticMinMax<L1 / L2, L1 / U2, U1 / L2, U1 / U2>::MAX_VAL
	> operator/(const Range<L1, U1> &lhs, const Range<L2, U2> &rhs)
	{
		return Range<
			StaticMinMax<L1 / L2, L1 / U2, U1 / L2, U1 / U2>::MIN_VAL,
			StaticMinMax<L1 / L2, L1 / U2, U1 / L2, U1 / U2>::MAX_VAL
		>(lhs.get() / rhs.get());
	}
}



#endif