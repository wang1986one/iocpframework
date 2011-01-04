#ifndef __TYPE_TRAITS_HPP
#define __TYPE_TRAITS_HPP



/*
���n�ṩ��һ����ܣ�framework�������Sᘌ��̈́e���ԣ�type attributes�����ھ��g�r����ɺ�ʽ���ͣ�dispatch����
�@���׫�� template ���Ў�����
���磬���҂��ʂ䌦һ����Ԫ���̈́eδ֪������Ј��п�ؐ��copy�������r������҂�������֪����Ԫ���̈́e�Ƿ���һ��trivial copy constructor��
���܉�����҂��Q���Ƿ��ʹ�ÿ��ٵ� memcpy()��

class template __TypeTraits �ṩ�S��typedefs��ÿһ������ __TrueType ���� __FalseType��__TypeTraits �������������κ��̈́e���@Щtypedefs���������¹ܵ��@�����_ֵ��
1. һ��߬F�w��general instantiation�����Ⱥ��������̈́e���Զ��ض���Ч�ı���ֵ��
2. ���^������ػ��汾�����籾�n������ C++ �Ƚ��̈́e���ṩ���ػ����棩��
3. ĳЩ���g������Silicon Graphics N32 �� N64 ���g�������ԄӞ������̈́e�ṩ�m�����ػ��汾��

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
	/* ��Ҫ�Ƴ��@���ɆT����֪ͨ���������Ԅӌ� __TypeTraits�ػ����ľ��g���f���҂��F�����������@�� __TypeTraits template ������ġ�
	�@�Ǟ��˴_���fһ���g��Ҳʹ��һ������ __TypeTraits ���䌍�c��̎���x�K�o�κ��P��template �r���������鶼�Ԍ�����\����*/

	/* ���l���������أ���龎�g���п����ԄӞ���̈́e�a�����ٵ� __TypeTraits�ػ��汾��
	- ����������������µĳɆT����
	- ������Ƴ������κγɆT
	- �^�������Ԍ����³ɆT�����������s�]�и�׃���g���еČ������Q
	- �¼���ĳɆT����ҕ��һ��ɆT���������ھ��g���м����m��֧Ԯ��*/

	typedef __FalseType    has_trivial_default_constructor;
	typedef __FalseType    has_trivial_copy_constructor;
	typedef __FalseType    has_trivial_assignment_operator;
	typedef __FalseType    has_trivial_destructor;
	typedef __FalseType    is_POD_type;
	// ���^ POD ��ָ Plain Old data structure. 
};

// �����ṩĳЩ�ػ��汾���@��춃Ƚ��� __types_traits ֧Ԯ�����ľ��g���K�o
// ����������춟oԓ��֧Ԯ�����ľ��g�����Ԅt�ٱ�Ҫ��

/* ����ᘌ� C++ �����̈́e char, signed char, unsigned char, short, unsigned short, int, unsigned int, long, unsigned long, float, double, long double �ṩ�ػ��汾��
   ע�⣬ÿһ���ɆT��ֵ���� __TrueType����ʾ�@Щ�̈́e���ɒ�������ٷ�ʽ������ memcpy�����M�п�ؐ�������xֵ������*/


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