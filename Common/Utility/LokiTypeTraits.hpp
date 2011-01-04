#ifndef __TYPE_INFORMATION_TRAITS_HPP
#define __TYPE_INFORMATION_TRAITS_HPP



#include "../Constraints.hpp"


namespace DataStructure
{

	namespace Internal
	{
		template<typename U> struct AddPointer
		{
			typedef U* Result;
		};

		template<typename U> struct AddPointer<U&>
		{
			typedef U* Result;
		};

		template<typename U> struct AddReference
		{
			typedef U & Result;
		};

		template<typename U> struct AddReference<U &>
		{
			typedef U & Result;
		};

		template<> struct AddReference<void>
		{
			typedef NullType Result;
		};

		template<typename U> struct AddParameterType
		{
			typedef const U & Result;
		};

		template<typename U> struct AddParameterType<U &>
		{
			typedef U & Result;
		};

		template<> struct AddParameterType<void>
		{
			typedef NullType Result;
		};


		/***********************************************************
		 * class IsFunctionPointerRaw 
		 *
		 ***********************************************************/
		template<typename T>
		struct IsFunctionPointerRaw
		{enum{result = 0};};

		template<typename T>
		struct IsFunctionPointerRaw<T(*)()> 
		{enum {result = 1};};

		template<typename T, typename P01>
		struct IsFunctionPointerRaw<T(*)(P01)> 
		{enum {result = 1};};

		template<typename T, typename P01, typename P02>
		struct IsFunctionPointerRaw<T(*)(P01, P02)> 
		{enum {result = 1};};

		template<typename T, typename P01, typename P02, typename P03>
		struct IsFunctionPointerRaw<T(*)(P01, P02, P03)> 
		{enum {result = 1};};



		/***********************************************************
		* class IsMemberFunctionPointerRaw 
		*
		***********************************************************/
		template<typename T>
		struct IsMemberFunctionPointerRaw
		{enum {result = 0};};

		template<typename T, typename S>
		struct IsMemberFunctionPointerRaw<T (S::*)()> 
		{enum {result = 1};};

		template<typename T, typename S, 
			typename P01>
		struct IsMemberFunctionPointerRaw<T (S::*)(P01)> 
		{enum {result = 1};};

		template<typename T, typename S, 
			typename P01, typename P02>
		struct IsMemberFunctionPointerRaw<T (S::*)(
			P01, P02)> 
		{enum {result = 1};};

		template<typename T, typename S, typename P01, typename P02, typename P03>
		struct IsMemberFunctionPointerRaw<T (S::*)(P01, P02, P03)> 
		{enum {result = 1};};
	};

	

	



	/***********************************************************
	 * class TypeTraits 
	 *
	 ***********************************************************/
	template<typename T>
	class TypeTraits
	{
	private:
		template<typename U> struct ReferenceTraits
		{
			enum { result = false };
			typedef U ReferenceType;
		};

		template<typename U> struct ReferenceTraits<U&>
		{
			enum { result = true };
			typedef U ReferenceType;
		};

		template<typename U> struct PointerTraits
		{
			enum { result = false };
			typedef NullType PointerType;
		};

		template<typename U> struct PointerTraits<U*>
		{
			enum { result = true };
			typedef U PointerType;
		};

		template<typename U> struct PointerTraits<U*&>
		{
			enum { result = true };
			typedef U PointerType;
		};

		template<typename U> struct PToMTraits
		{
			enum { result = false };
		};

		template<typename U, class V> struct PToMTraits<U V::*>
		{
			enum { result = true };
		};

		template<typename U, class V> struct PToMTraits<U V::*&>
		{
			enum { result = true };
		};

		template<typename U> struct FunctionPointerTraits
		{
			enum{ result = Internal::IsFunctionPointerRaw<U>::result };
		};

		template<typename U> struct PToMFunctionTraits
		{
			enum{ result = Internal::IsMemberFunctionPointerRaw<U>::result };
		};

		template<typename U> struct UnConst
		{
			typedef U Result;
			enum { isConst = 0 };
		};

		template<typename U> struct UnConst<const U>
		{
			typedef U Result;
			enum { isConst = 1 };
		};

		template<typename U> struct UnConst<const U&>
		{
			typedef U& Result;
			enum { isConst = 1 };
		};

		template<typename U> struct UnVolatile
		{
			typedef U Result;
			enum { isVolatile = 0 };
		};

		template<typename U> struct UnVolatile<volatile U>
		{
			typedef U Result;
			enum { isVolatile = 1 };
		};

		template<typename U> struct UnVolatile<volatile U&>
		{
			typedef U& Result;
			enum { isVolatile = 1 };
		};

	public:
		typedef T															value_type;

		typedef typename UnConst<T>::Result									NonConstType;
		typedef typename UnVolatile<T>::Result								NonVolatileType;
		typedef typename UnVolatile<typename UnConst<T>::Result>::Result	UnqualifiedType;


		typedef typename PointerTraits<UnqualifiedType>::PointerType		PointerType;
		typedef const PointerType											ConstPointerType;
		typedef typename ReferenceTraits<T>::ReferenceType					ReferenceType;
		typedef const ReferenceType											ConstReferenceType;

		

		enum { isConst          = UnConst<T>::isConst };
		enum { isVolatile       = UnVolatile<T>::isVolatile };
		enum { isReference      = ReferenceTraits<UnqualifiedType>::result };
		enum { isFunction       = FunctionPointerTraits<typename Internal::AddPointer<T>::Result >::result };
		enum { isFunctionPointer= FunctionPointerTraits<typename ReferenceTraits<UnqualifiedType>::ReferenceType >::result };
		enum { isMemberFunctionPointer= PToMFunctionTraits<typename ReferenceTraits<UnqualifiedType>::ReferenceType >::result };
		enum { isMemberPointer  = PToMTraits<typename ReferenceTraits<UnqualifiedType>::ReferenceType >::result || isMemberFunctionPointer };
		enum { isPointer        = PointerTraits<typename ReferenceTraits<UnqualifiedType>::ReferenceType >::result || isFunctionPointer };

		/*enum { isStdUnsignedInt = TL::IndexOf<Internal::StdUnsignedInts, UnqualifiedType>::value >= 0 ||
			TL::IndexOf<Internal::StdUnsignedInts, 
			typename ReferenceTraits<UnqualifiedType>::ReferenceType>::value >= 0};
		enum { isStdSignedInt   = TL::IndexOf<Internal::StdSignedInts, UnqualifiedType>::value >= 0 ||
			TL::IndexOf<Internal::StdSignedInts, 
			typename ReferenceTraits<UnqualifiedType>::ReferenceType>::value >= 0};
		enum { isStdIntegral    = isStdUnsignedInt || isStdSignedInt ||
			TL::IndexOf<Internal::StdOtherInts, UnqualifiedType>::value >= 0 ||
			TL::IndexOf<Internal::StdOtherInts, 
			typename ReferenceTraits<UnqualifiedType>::ReferenceType>::value >= 0};
		enum { isStdFloat       = TL::IndexOf<Internal::StdFloats, UnqualifiedType>::value >= 0 ||
			TL::IndexOf<Internal::StdFloats, 
			typename ReferenceTraits<UnqualifiedType>::ReferenceType>::value >= 0};*/


		/*enum { isStdArith       = isStdIntegral || isStdFloat };
		enum { isStdFundamental = isStdArith || isStdFloat || Conversion<T, void>::sametype };

		enum { isUnsignedInt    = isStdUnsignedInt || IsCustomUnsignedInt<UnqualifiedType>::value };
		enum { isSignedInt      = isStdSignedInt || IsCustomSignedInt<UnqualifiedType>::value };
		enum { isIntegral       = isStdIntegral || isUnsignedInt || isSignedInt };
		enum { isFloat          = isStdFloat || IsCustomFloat<UnqualifiedType>::value };


		enum { isArith          = isIntegral || isFloat };
		enum { isFundamental    = isStdFundamental || isArith };

		typedef typename Select<isStdArith || isPointer || isMemberPointer, T, 
			typename Internal::AddParameterType<T>::Result>::Result 
			ParameterType;*/
	};
}














#endif