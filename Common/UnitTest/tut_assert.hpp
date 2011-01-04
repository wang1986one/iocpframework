#ifndef TUT_ASSERT_H_GUARD
#define TUT_ASSERT_H_GUARD


#include "tut_exception.hpp"

namespace tut
{

	namespace detail
	{
		// 增加消息前缀
		template< typename M >
		std::ostringstream &MsgPrefix(std::ostringstream &str, const M &msg)
		{
			std::ostringstream ss;
			ss << msg;

			if( !ss.str().empty() )
				str << msg << ": ";

			return str;
		}
	}


	namespace
	{

		/**
		* Tests provided condition.
		* Throws if false.
		*/
		void Ensure(bool cond)
		{
			if( !cond)
			{
				// TODO: default ctor?
				throw Failure("");
			}
		}

		/**
		* Tests provided condition.
		* Throws if true.
		*/
		void EnsureNot(bool cond)
		{
			Ensure(!cond);
		}

		/**
		* Tests provided condition.
		* Throws if false.
		*/
		template< typename M >
		void Ensure(const M& msg, bool cond)
		{
			if( !cond)
			{
				throw Failure(msg);
			}
		}

		/**
		* Tests provided condition.
		* Throws if true.
		*/
		template< typename M >
		void EnsureNot(const M& msg, bool cond)
		{
			Ensure(msg, !cond);
		}

		/**
		* Tests two objects for being equal.
		* Throws if false.
		*
		* NB: both LHS and RHS must have operator << defined somewhere, or
		* client code will not compile at all!
		*/
		template< typename M, typename LHS, typename RHS >
		void EnsureEqual(const M& msg, const LHS& actual, const RHS& expected)
		{
			if( expected != actual )
			{
				std::ostringstream ss;
				detail::MsgPrefix(ss,msg)
					<< "expected `"
					<< expected
					<< "` actual `"
					<< actual
					<< "`";

				throw Failure(ss.str());
			}
		}

		/**
		* Tests two pointers for being equal.
		* Throws if false.
		*
		* NB: both T and Q must have operator << defined somewhere, or
		* client code will not compile at all!
		*/
		template< typename M, typename LHS, typename RHS >
		void EnsureEqual(const M& msg, const LHS * const actual, const RHS * const expected)
		{
			if( expected != actual )
			{
				std::ostringstream ss;
				detail::MsgPrefix(ss,msg)
					<< "expected `"
					<< (void*)expected
					<< "` actual `"
					<< (void*)actual
					<< "`";
				throw Failure(ss.str());
			}
		}

		template< typename M >
		void EnsureEqual(const M& msg, const double& actual, const double& expected, const double& epsilon)
		{
			const double diff = actual - expected;

			if( (actual != expected) && !((diff <= epsilon) && (diff >= -epsilon )) )
			{
				std::ostringstream ss;
				detail::MsgPrefix(ss,msg)
					<< std::scientific
					<< std::showpoint
					<< std::setprecision(16)
					<< "expected `" << expected
					<< "` actual `" << actual
					<< "` with precision `" << epsilon << "`";
				throw Failure(ss.str());
			}
		}

		template< typename M >
		void EnsureEqual(const M& msg, const double& actual, const double& expected)
		{
			EnsureEqual(msg, actual, expected, std::numeric_limits<double>::epsilon());
		}

		template< typename LHS, typename RHS>
		void EnsureEqual(const LHS& actual, const RHS& expected)
		{
			EnsureEqual("Values are not equal", actual, expected);
		}


		template< typename LhsIterator, typename RhsIterator >
		void EnsureEqual(const std::string &msg, const LhsIterator &lhs_begin, const LhsIterator &lhs_end, const RhsIterator &rhs_begin, const RhsIterator &rhs_end)
		{
			typename std::iterator_traits<LhsIterator>::difference_type lhs_size = std::distance(lhs_begin, lhs_end);
			typename std::iterator_traits<RhsIterator>::difference_type rhs_size = std::distance(rhs_begin, rhs_end);

			if( lhs_size < rhs_size )
			{
				EnsureEqual(msg + ": range is too short", lhs_size, rhs_size);
			}

			if( lhs_size > rhs_size )
			{
				EnsureEqual(msg + ": range is too long", lhs_size, rhs_size);
			}

			assert(lhs_size == rhs_size);

			LhsIterator lhs_i = lhs_begin;
			RhsIterator rhs_i = rhs_begin;
			while( (lhs_i != lhs_end) && (rhs_i != rhs_end) )
			{
				if(*lhs_i != *rhs_i)
				{
					std::ostringstream ss;
					detail::MsgPrefix(ss,msg)
						<< "expected `" << *rhs_i
						<< "` actual `" << *lhs_i
						<< "` at offset " << std::distance(lhs_begin, lhs_i);
					throw Failure(ss.str());
				}

				lhs_i++;
				rhs_i++;
			}

			assert(lhs_i == lhs_end);
			assert(rhs_i == rhs_end);
		}

		template<typename LhsIterator, typename RhsIterator >
		void EnsureEqual(const LhsIterator &lhs_begin, const LhsIterator &lhs_end, const RhsIterator &rhs_begin, const RhsIterator &rhs_end)
		{
			EnsureEqual("Ranges are not equal", lhs_begin, lhs_end, rhs_begin, rhs_end);
		}

		template<typename LhsType, typename RhsType >
		void EnsureEqual(const LhsType *lhs_begin, const LhsType *lhs_end, const RhsType *rhs_begin, const RhsType *rhs_end)
		{
			EnsureEqual("Ranges are not equal", lhs_begin, lhs_end, rhs_begin, rhs_end);
		}

		/**
		* Tests two objects for being at most in given distance one from another.
		* Borders are excluded.
		* Throws if false.
		*
		* NB: T must have operator << defined somewhere, or
		* client code will not compile at all! Also, T shall have
		* operators + and -, and be comparable.
		*
		* TODO: domains are wrong, T - T might not yield T, but Q
		*/
		template< typename M, class T>
		void EnsureDistance(const M& msg, const T& actual, const T& expected, const T& distance)
		{
			if( expected-distance >= actual || expected+distance <= actual )
			{
				std::ostringstream ss;
				detail::MsgPrefix(ss,msg)
					<< " expected `"
					<< expected-distance
					<< "` - `"
					<< expected+distance
					<< "` actual `"
					<< actual
					<< "`";
				throw Failure(ss.str());
			}
		}

		template< typename T >
		void EnsureDistance(const T& actual, const T& expected, const T& distance)
		{
			EnsureDistance<>("Distance is wrong", actual, expected, distance);
		}

		template< typename M >
		void EnsureErrno(const M& msg, bool cond)
		{
			if( !cond )
			{
				throw Failure(msg);
			}
		}

		/**
		* Unconditionally fails with message.
		*/
		void Fail(const char* msg = "")
		{
			throw Failure(msg);
		}

		template< typename M >
		void Fail(const M& msg)
		{
			throw Failure(msg);
		}

		/**
		* Mark test case as known Failure and Skip execution.
		*/
		void Skip(const char* msg = "")
		{
			throw Skipped(msg);
		}

		template<typename M>
		void Skip(const M& msg)
		{
			throw Skipped(msg);
		}

	} // end of namespace

}

#endif

