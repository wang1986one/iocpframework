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

	// 判断元素的数值型別（value type）是否有 trivial destructor
	template<typename ForwardIterator, typename T>
	inline void __Destroy(ForwardIterator first, ForwardIterator last, T*) 
	{
		typedef typename __TypeTraits<T>::has_trivial_destructor trivial_destructor;
		__DestroyAux(first, last, trivial_destructor());
	}

	// 以下是 destroy() 第二版本，接受兩個迭代器。此函式是設法找出元素的數值型別，
	// 進而利用 __TypeTraits<> 求取最適當措施。
	template<typename ForwardIterator>
	inline void __Destroy(ForwardIterator first, ForwardIterator last) 
	{
		typedef typename IteratorTraits<ForwardIterator>::value_type value_type;
		value_type *pType = NULL;
		__Destroy(first, last, pType);
	}

	// 以下是destroy() 第二版本针对迭代器为 char* 和 wchar_t* 的特化版
	inline void __Destroy(char*, char*) 
	{}
	inline void __Destroy(wchar_t*, wchar_t*) 
	{}
	

	// 如果元素的数值型別（value type）有 non-trivial destructor…
	template<typename ForwardIterator>
	inline void __DestroyAux(ForwardIterator first, ForwardIterator last, __FalseType) 
	{
		for( ; first < last; ++first)
			__Destroy(&*first);
	}

	// 如果元素的数值型別（value type）有 trivial destructor…
	template<typename ForwardIterator> 
	inline void __DestroyAux(ForwardIterator, ForwardIterator, __TrueType) 
	{}

	


}



#endif