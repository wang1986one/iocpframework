#ifndef __CONVERSION_LITERAL_CAST_HPP
#define __CONVERSION_LITERAL_CAST_HPP


#include "../Constraints.hpp"
#include <limits>

namespace DataStructure
{
	typedef long long			literal_cast_t;
	typedef unsigned long long	invalid_cast_t;



	// =========================================================================
	// class literal_cast
	// 能够侦测出类型转换产生的阶段并强制产生一个编译期错误

	
	template<typename T, literal_cast_t v>
	class literal_cast
	{
	public:
		operator T() const
		{
			const bool tooLarge = (v <= LLONG_MAX);
			const bool tooSamll = (v >= LLONG_MIN);

			STATIC_CHECK(tooLarge, tooLarge);
			STATIC_CHECK(tooSamll, tooSamll);

			return T(v);
		}
	};


	template<literal_cast_t v>
	class literal_cast<invalid_cast_t, v>
	{
	public:
		literal_cast()
		{
			const bool Invalid_cast_type = false;

			STATIC_CHECK(Invalid_cast_type, Invalid_cast_type);
		}
	};
}





#endif