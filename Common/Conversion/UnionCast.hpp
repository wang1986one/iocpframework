#ifndef __CONVERSION_UNION_CAST_HPP
#define __CONVERSION_UNION_CAST_HPP

#include "../Utility/LokiTypeTraits.hpp"
#include "../Constraints.hpp"

namespace DataStructure
{

	// =========================================================================
	// class union_cast
	// ʹ��ԭ��
	//			1.��Щת����Ҫ��ֹһ��ǿ�Ʋ�����ɣ����ܻᵼ�������Ķ���ά��
	//			2.��Щʱ������ʽת����C���ת���о���
	

	/*	1. ��С����һ��
		2. ����POD����
		3. ������const\voliolate����
		4. �����벻Ϊָ�룬���߱���Ϊָ��POD���͵�ָ��
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