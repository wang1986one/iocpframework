#ifndef __CONVERSION_PTR_CAST_HPP
#define __CONVERSION_PTR_CAST_HPP



namespace DataStructure
{
	// =========================================================================
	// class ptr_cast
	// 既能够被用在引用上，又可以用在指针上，在两种情况下转换失败都会抛出异常bad_cast


	template<typename T>
	class ptr_cast
	{
	public:
		typedef T												TargetType;
		typedef ptr_cast<T>										ClassType;
		
		typedef typename TypeTraits<TargetType>::value_type		TargetValueType;

		typedef typename TypeTraits<TargetType>::ConstReferenceType								ReferenceType;
		typedef typename TypeTraits<TargetType>::ConstPointerType								PointerType;

	private:
		PointerType m_p;

	public:
		template<typename SrcT>
		ptr_cast(SrcT &s)
			: m_p(&dynamic_cast<TargetType>(s))
		{}
		template<typename SrcT>
		ptr_cast(SrcT *&s)
			: m_p(&dynamic_cast<TargetType>(s))
		{
			if( NULL == m_p )
				throw std::bad_cast("ptr_cast error!");
		}
		ptr_cast(PointerType pt)
			: m_p(pt)
		{}
		ptr_cast(ReferenceType t)
			: m_p(&t)
		{}

	private:
		ptr_cast(const ClassType &);
		
	public:
		operator ReferenceType() const
		{
			return const_cast<ReferenceType>(*m_p);
		}
		operator PointerType() const
		{
			return const_cast<PointerType>(m_p);
		}
	};
}




#endif