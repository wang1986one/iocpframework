#ifndef __CONSTRUCT_HPP
#define __CONSTRUCT_HPP


#include "TypeTraits.hpp"


namespace DataStructure
{

	template<typename T>
	inline void __Construct(T *p)
	{
		new (p) T;
	}
	template<typename T1, typename T2>
	inline void __Construct(T1* p, const T2& value) 
	{
		new (p) T1(value); 						
	}
	template<typename T1, typename T2, typename T3>
	inline void __Construct(T1* p, const T2& value1, const T3 &value2) 
	{
		new (p) T1(value1, value2); 							
	}
	template<typename T1, typename T2, typename T3, typename T4>
	inline void __Construct(T1* p, const T2& value1, const T3& value2, const T4& value3) 
	{
		new (p) T1(value1, value2, value3); 					
	}
	template<typename T1, typename T2, typename T3, typename T4, typename T5>
	inline void __Construct(T1* p, const T2& value1, const T3& value2, const T4& value3, const T5& value4) 
	{
		new (p) T1(value1, value2, value3, value4); 			
	}
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	inline void __Construct(T1* p, const T2& value1, const T3& value2, const T4& value3, const T5& value4, const T6& value5) 
	{
		new (p) T1(value1, value2, value3, value4, value5); 	
	}
	

	template<typename T>
	inline void __Destroy(T *p)
	{
		p->~T();	// dtor ~T()
	}

	// �ж�Ԫ�ص���ֵ�̈́e��value type���Ƿ��� trivial destructor
	template<typename ForwardIterator, typename T>
	inline void __Destroy(ForwardIterator first, ForwardIterator last, T*) 
	{
		typedef typename __TypeTraits<T>::has_trivial_destructor trivial_destructor;
		__DestroyAux(first, last, trivial_destructor());
	}

	// ������ destroy() �ڶ��汾�����܃ɂ����������˺�ʽ���O���ҳ�Ԫ�صĔ�ֵ�̈́e��
	// �M������ __TypeTraits<> ��ȡ���m����ʩ��
	template<typename ForwardIterator>
	inline void __Destroy(ForwardIterator first, ForwardIterator last) 
	{
		typedef typename IteratorTraits<ForwardIterator>::value_type value_type;
		value_type *pType = NULL;
		__Destroy(first, last, pType);
	}

	// ������destroy() �ڶ��汾��Ե�����Ϊ char* �� wchar_t* ���ػ���
	inline void __Destroy(char*, char*) 
	{}
	inline void __Destroy(wchar_t*, wchar_t*) 
	{}
	

	// ���Ԫ�ص���ֵ�̈́e��value type���� non-trivial destructor��
	template<typename ForwardIterator>
	inline void __DestroyAux(ForwardIterator first, ForwardIterator last, __FalseType) 
	{
		for( ; first < last; ++first)
			__Destroy(&*first);
	}

	// ���Ԫ�ص���ֵ�̈́e��value type���� trivial destructor��
	template<typename ForwardIterator> 
	inline void __DestroyAux(ForwardIterator, ForwardIterator, __TrueType) 
	{}

	


}



#endif