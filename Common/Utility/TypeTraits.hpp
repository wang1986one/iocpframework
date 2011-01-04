#ifndef __TYPE_TRAITS_HPP
#define __TYPE_TRAITS_HPP



/*
本檔提供了一個框架（framework），允許針對型別屬性（type attributes），在編譯時期完成函式派送（dispatch）。
這對於撰寫 template 很有幫助。
例如，當我們準備對一個「元素型別未知」的陣列執行拷貝（copy）動作時，如果我們能事先知道其元素型別是否有一個trivial copy constructor，
便能夠幫助我們決定是否可使用快速的 memcpy()。

class template __TypeTraits 提供許多typedefs，每一個若非 __TrueType 就是 __FalseType。__TypeTraits 的引數可以是任何型別。這些typedefs將經由以下管道獲得正確值：
1. 一般具現體（general instantiation），內含對所有型別而言都必定有效的保守值。
2. 經過宣告的特化版本（例如本檔對所有 C++ 內建型別所提供的特化宣告）。
3. 某些編譯器（如Silicon Graphics N32 和 N64 編譯器）會自動為所有型別提供適當的特化版本。

*/
struct __TrueType
{
};

struct __FalseType
{
};

template<typename type>
struct __TypeTraits
{ 
	typedef __TrueType     this_dummy_member_must_be_first;
	/* 不要移除這個成員。它通知「有能力自動將 __TypeTraits特化」的編譯器說，我們現在所看到的這個 __TypeTraits template 是特殊的。
	這是為了確保萬一編譯器也使用一個名為 __TypeTraits 而其實與此處定義並無任何關聯的template 時，所有事情都仍將順利運作。*/

	/* 以下條件應被遵守，因為編譯器有可能自動為各型別產生專屬的 __TypeTraits特化版本：
	- 你可以重新排列以下的成員次序
	- 你可以移除以下任何成員
	- 絕對不可以將以下成員重新命名而卻沒有改變編譯器中的對應名稱
	- 新加入的成員會被視為一般成員，除非你在編譯器中加上適當支援。*/

	typedef __FalseType    has_trivial_default_constructor;
	typedef __FalseType    has_trivial_copy_constructor;
	typedef __FalseType    has_trivial_assignment_operator;
	typedef __FalseType    has_trivial_destructor;
	typedef __FalseType    is_POD_type;
	// 所謂 POD 意指 Plain Old data structure. 
};

// 以下提供某些特化版本。這對於內建有 __types_traits 支援能力的編譯器並無
// 傷害，而對於無該等支援能力的編譯器而言則屬必要。

/* 以下針對 C++ 基本型別 char, signed char, unsigned char, short, unsigned short, int, unsigned int, long, unsigned long, float, double, long double 提供特化版本。
   注意，每一個成員的值都是 __TrueType，表示這些型別都可採用最快速方式（例如 memcpy）來進行拷貝動作或賦值動作。*/


template<>
struct __TypeTraits<char> 
{
	typedef __TrueType    has_trivial_default_constructor;
	typedef __TrueType    has_trivial_copy_constructor;
	typedef __TrueType    has_trivial_assignment_operator;
	typedef __TrueType    has_trivial_destructor;
	typedef __TrueType    is_POD_type;
};

template<>
struct __TypeTraits<signed char> 
{
	typedef __TrueType    has_trivial_default_constructor;
	typedef __TrueType    has_trivial_copy_constructor;
	typedef __TrueType    has_trivial_assignment_operator;
	typedef __TrueType    has_trivial_destructor;
	typedef __TrueType    is_POD_type;
};

template<>
struct __TypeTraits<unsigned char>
{
	typedef __TrueType    has_trivial_default_constructor;
	typedef __TrueType    has_trivial_copy_constructor;
	typedef __TrueType    has_trivial_assignment_operator;
	typedef __TrueType    has_trivial_destructor;
	typedef __TrueType    is_POD_type;
};

template<>
struct __TypeTraits<short> 
{
	typedef __TrueType    has_trivial_default_constructor;
	typedef __TrueType    has_trivial_copy_constructor;
	typedef __TrueType    has_trivial_assignment_operator;
	typedef __TrueType    has_trivial_destructor;
	typedef __TrueType    is_POD_type;
};

template<>
struct __TypeTraits<unsigned short> 
{
	typedef __TrueType    has_trivial_default_constructor;
	typedef __TrueType    has_trivial_copy_constructor;
	typedef __TrueType    has_trivial_assignment_operator;
	typedef __TrueType    has_trivial_destructor;
	typedef __TrueType    is_POD_type;
};

template<>
struct __TypeTraits<int> 
{
	typedef __TrueType    has_trivial_default_constructor;
	typedef __TrueType    has_trivial_copy_constructor;
	typedef __TrueType    has_trivial_assignment_operator;
	typedef __TrueType    has_trivial_destructor;
	typedef __TrueType    is_POD_type;
};

template<>
struct __TypeTraits<unsigned int>
{
	typedef __TrueType    has_trivial_default_constructor;
	typedef __TrueType    has_trivial_copy_constructor;
	typedef __TrueType    has_trivial_assignment_operator;
	typedef __TrueType    has_trivial_destructor;
	typedef __TrueType    is_POD_type;
};

template<> struct __TypeTraits<long> 
{
	typedef __TrueType    has_trivial_default_constructor;
	typedef __TrueType    has_trivial_copy_constructor;
	typedef __TrueType    has_trivial_assignment_operator;
	typedef __TrueType    has_trivial_destructor;
	typedef __TrueType    is_POD_type;
};

template<>
struct __TypeTraits<unsigned long>
{
	typedef __TrueType    has_trivial_default_constructor;
	typedef __TrueType    has_trivial_copy_constructor;
	typedef __TrueType    has_trivial_assignment_operator;
	typedef __TrueType    has_trivial_destructor;
	typedef __TrueType    is_POD_type;
};

template<>
struct __TypeTraits<float> 
{
	typedef __TrueType    has_trivial_default_constructor;
	typedef __TrueType    has_trivial_copy_constructor;
	typedef __TrueType    has_trivial_assignment_operator;
	typedef __TrueType    has_trivial_destructor;
	typedef __TrueType    is_POD_type;
};

template<>
struct __TypeTraits<double>
{
	typedef __TrueType    has_trivial_default_constructor;
	typedef __TrueType    has_trivial_copy_constructor;
	typedef __TrueType    has_trivial_assignment_operator;
	typedef __TrueType    has_trivial_destructor;
	typedef __TrueType    is_POD_type;
};

template<>
struct __TypeTraits<long double> 
{
	typedef __TrueType    has_trivial_default_constructor;
	typedef __TrueType    has_trivial_copy_constructor;
	typedef __TrueType    has_trivial_assignment_operator;
	typedef __TrueType    has_trivial_destructor;
	typedef __TrueType    is_POD_type;
};

template<typename T>
struct __TypeTraits<T*> 
{
	typedef __TrueType    has_trivial_default_constructor;
	typedef __TrueType    has_trivial_copy_constructor;
	typedef __TrueType    has_trivial_assignment_operator;
	typedef __TrueType    has_trivial_destructor;
	typedef __TrueType    is_POD_type;
};

template<>
struct __TypeTraits<char*> 
{
	typedef __TrueType    has_trivial_default_constructor;
	typedef __TrueType    has_trivial_copy_constructor;
	typedef __TrueType    has_trivial_assignment_operator;
	typedef __TrueType    has_trivial_destructor;
	typedef __TrueType    is_POD_type;
};

template<>
struct __TypeTraits<signed char*> 
{
	typedef __TrueType    has_trivial_default_constructor;
	typedef __TrueType    has_trivial_copy_constructor;
	typedef __TrueType    has_trivial_assignment_operator;
	typedef __TrueType    has_trivial_destructor;
	typedef __TrueType    is_POD_type;
};

template<>
struct __TypeTraits<unsigned char*> 
{
	typedef __TrueType    has_trivial_default_constructor;
	typedef __TrueType    has_trivial_copy_constructor;
	typedef __TrueType    has_trivial_assignment_operator;
	typedef __TrueType    has_trivial_destructor;
	typedef __TrueType    is_POD_type;
};




#endif