#ifndef __CONVERSION_UNION_CAST_HPP
#define __CONVERSION_UNION_CAST_HPP

#include "../Utility/LokiTypeTraits.hpp"
#include "../Constraints.hpp"

namespace DataStructure
{

	// =========================================================================
	// class union_cast
	// 使用原因：
	//			1.有些转换需要不止一次强制才能完成，可能会导致难以阅读和维护
	//			2.有些时候会对隐式转换和C风格转换有警告
	

	/*	1. 大小必须一致
		2. 都是POD类型
		3. 可以用const\voliolate修饰
		4. 都必须不为指针，或者必须为指向POD类型的指针
	*/
	template<typename ToT, typename FromT, bool CheckAlign = true>
	union union_caster
	{
	public:
		typedef ToT									ToType;
		typedef FromT								FromType;
		typedef union_caster<ToT, FromT, CheckAlign>	ClassType;

	private:
		FromType	m_from;
		ToType		m_to;

	public:
		explicit union_caster(const FromType from, bool bCheckAlign = CheckAlign)
			: m_from(from)
		{
			
			STATIC_CHECK(sizeof(FromType) == sizeof(ToType), union_caster);

			typedef typename TypeTraits<FromType>::value_type	FromBaseType;
			typedef typename TypeTraits<ToType>::value_type		ToBaseType;

			MustBePODOrVoid<FromBaseType>();
			MustBePODOrVoid<ToBaseType>();

		}

	private:
		ClassType &operator=(const ClassType &);

	public:
		operator ToType() const
		{
			return m_to;
		}
	};


	template<typename To, typename From>
	inline union_caster<To, From, true> union_cast(const From from, bool bCheckAlign = true)
	{
		return union_caster<To, From, true>(from, bCheckAlign);
	}

}







#endif