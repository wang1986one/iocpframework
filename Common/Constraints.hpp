#ifndef __CONSTRAINTS_HPP
#define __CONSTRAINTS_HPP


namespace DataStructure
{
	// 只声明，不定义
	class NullType;

	// 可被继承的空类型
	struct EmptyType
	{};


	/************************************************************************
	*  
	*	MustHaveBase	: 把D类型的指针赋值给B类型的指针，Constraints为静态成员函数，确保没有任何运行期负担
	*	调用方式			: MustHaveBase<X *, XX *>() 或 MustHaveBase_(&b, &a)
	*	
	*
	***********************************************************************/
	template<typename D, typename B>
	struct MustHaveBase
	{
		~MustHaveBase()
		{
			void (*p)(D *, B *) = Constraints;
		}

	private:
		static void Constraints(D *pD, B *pB)
		{ pB = pD; }
	};

	template<typename D>
	struct MustHaveBase<D, D>
	{
		~MustHaveBase()
		{
			// static_assert
		}
	};

	template<typename D, typename B>
	inline void MustHaveBase_(D *, B *)
	{
		MustHaveBase<D, B>();
	}




	/************************************************************************
	*  
	*	MustBeSubcriptable	: 确保该类型能以[]方式访问
	*	调用方式				: MustBeSubcriptable<X>() 或 MustBeSubcriptable_(x)
	*	
	*
	***********************************************************************/
	template<typename T>
	struct MustBeSubcriptable
	{
		~MustBeSubcriptable()
		{
			void (*p)(const T &) = Constraints;
		}

	private:
		static void Constraints(const T &IsNotSubcriptable)
		{
			sizeof(IsNotSubcriptable[0]);
		}
	};

	template<typename T>
	inline void MustBeSubcriptable_(T)
	{
		MustBeSubcriptable<T>();
	}



	/************************************************************************
	*  
	*	MustBePOD	: 如果一个类具有特殊的构造函数、拷贝构造函数、析构函数、赋值操作符函数，则不能作为联合的成员
	*	调用方式		: MustBePOD<int>() 或 MustBePOD_(x)
	*	
	*
	***********************************************************************/
	template<typename T>
	struct MustBePOD
	{
		~MustBePOD()
		{
			int (*p)() = _Constraints;
			p;
		}

	private:
		static int _Constraints()
		{
			union
			{
				T   T_is_not_a_POD_type;
				int i; // Not used
			}u;
			
			return (&u)->i;
		}
	};

	template<typename T>
	inline void MustBePOD_(T)
	{
		MustBePOD<T>();
	}



	/************************************************************************
	*  
	*	MustBePODOrVoid	: 如果一个类具有特殊的构造函数、拷贝构造函数、析构函数、赋值操作符函数，则不能作为联合的成员
	*	调用方式			: MustBePODOrVoid<int>()
	*	
	*
	***********************************************************************/
	
	template<typename T>
	union MustBePODOrVoid
	{
	private:
		typedef MustBePODOrVoid<T>		ClassType;

	public:
		T t;
		int i;
		typedef int (*pFuncType)();

	public:
		static pFuncType Constraint()
		{
			return _Constraint;
		}

	private:
		static int _Constraint()
		{
			ClassType u;

			u.i = 1;

			return sizeof(u);
		}
	};

	template<>
	union MustBePODOrVoid<void>
	{
		typedef int (*pFuncType)();

		static pFuncType Constraint()
		{
			// 这里不能用NULL
			return static_cast<pFuncType>(0);
		}
	};
	


	// 调试、断点
	#define BREAK_ASSERT(x)    do { if(!x) __asm { int 3 }; } while(0)




	/************************************************************************
	*  
	*	IsVoid		: 类型侦测
	*	调用方式		: IsVoid<void>::value
	*	
	*
	***********************************************************************/
	template<typename T>
	struct IsVoid
	{
		enum { value = 0 };
	};

	template<>
	struct IsVoid<void>
	{
		enum { value = 1 };
	};


	/************************************************************************
	*  
	*	SizeOf		: 代替sizeof不能作用于void类型
	*	调用方式		: SizeOf<void>::value  Sizeof<int>::value
	*	
	*
	***********************************************************************/
	template<typename T>
	struct SizeOf
	{
		enum { value = sizeof(T) };
	};

	template<>
	struct SizeOf<void>
	{
		enum { value = 0 };
	};




	/************************************************************************
	*  
	*	STATIC_CHECK		: 编译期确保表达式为真
	*	调用方式				: STATIC_ASSERT(true, m)
	*	
	*
	***********************************************************************/
	namespace Internal
	{
		template<bool>
		struct ComplieTimeError;
		template<>
		struct ComplieTimeError<true>
		{};
	}
	#define STATIC_ASSERT(expr, msg)	\
	{								\
	Internal::ComplieTimeError<(expr) != 0> Error_##msg;	\
	(void)Error_##msg;			\
	}




	/************************************************************************
	*  
	*	Int2Type		: 常整数映射为类型，由编译期计算出来的结果选择不同的函数，达到静态分派
	*	调用方式			: Int2Type<isPolymorphics>
	*	适用情形			: 有需要根据某个编译期常数调用一个或数个不同的函数
	*					  有必要在编译期实施静态分派
	*
	*	Type2Type		: 利用函数重载机制，模拟template偏特化，利用轻量型型别来传递型别信息
	*
	***********************************************************************/
	template<int v>
	struct Int2Type
	{
		enum { value = v };
	};

	template<typename T>
	struct Type2Type
	{
		typedef T value_type;
	};



	/************************************************************************
	*  
	*	Type2Int		: 类型转换成整型值，需要在运行期第一次确定该值
	*	调用方式			: Type2Int<int>		Type2Int< Type2Int<int> >
	*
	***********************************************************************/
	typedef void(*pfn)();

	union Convert
	{
		pfn pFn;
		int value;
	};

	template<typename T>
	struct Type2Int
	{
		static void Dummy() {}
		static int Value()
		{
			Convert c;
			c.pFn = Dummy;

			return c.value;
		}
	};



	/************************************************************************
	*  
	*	Select--根据bool值选择类型
	*	调用方式: Select<flag, T, U>::value_type
	*	T 和 U都是类型，当flag为true时，返回T，反之则为U
	*
	***********************************************************************/
	template<bool flag, typename T, typename U>
	struct Select
	{
		typedef T value_type;
	};
	template<typename T, typename U>
	struct Select<false, T, U>
	{
		typedef U value_type;
	};



	/************************************************************************
	*  
	*	IsSameType--判断是否为同一类型
	*	调用方式: IsSameType<A, B>::value
	*	T 和 U都是类型，当flag为true时，返回T，反之则为U
	*
	***********************************************************************/
	template<typename T, typename U>
	struct IsSameType
	{
		enum { value = false };
	};
	template<typename T>
	struct IsSameType<T, T>
	{
		enum { value = true };
	};




	/************************************************************************
	*  
	*	Conversion--指明两个类型间的关系
	*	调用方式: Conversion<T, U>::exist		是否存在从T到U的隐式类型转换
	*			 Conversion<T, U>::exist2way	是否存在T到U且U到T的类型转换
	*			 Conversion<T, U>::sametype		是否为相同的类型
	*
	***********************************************************************/
	namespace Internal
	{
		template<typename T, typename U>
		struct ConversionHelper
		{
			typedef char Small;
			struct Big { char dummy[2]; };
			static Big Test(...);			// 只有在自动类型转换情况都不成立时，才会选中此优先级最低的转换方式
			static Small Test(U);			// 接受一个U类型参数，返回Small
			static T MakeT();				// 稻草人函数，解决当T的默认构造函数为private时，sizeof(T())不能通过
		};
	}

	template<typename T, typename U>
	struct Conversion
	{
		typedef Internal::ConversionHelper<T, U> ConversionType;

		enum { exist = sizeof(typename ConversionType::Small) == sizeof(ConversionType::Test(ConversionType::MakeT())) };
		enum { exist2way = exist && Conversion<U, T>::exist };
		enum { sametype = false };
	};
	template<typename T>
	struct Conversion<T, T>
	{
		enum { exist = true, exist2way = true, sametype = true };
	};
	template<typename T>
	struct Conversion<void, T>
	{
		enum { exist = false, exist2way = false, sametype = false };
	};
	template<typename T>
	struct Conversion<T, void>
	{
		enum { exist = false, exist2way = false, sametype = false };
	};
	template<>
	struct Conversion<void, void>
	{
		enum { exist = true, exist2way = true, sametype = true };
	};




	/************************************************************************
	*  
	*	SuperSubclass--判断两个class之间是否存在继承关系，当为private继承时不成立
	*	调用方式: SuperSubclass<B, D>::value		当B为D的公有继承父类或相同类型时返回true
	*	
	*
	***********************************************************************/
	template<typename T, typename U>
	struct SuperSubclass
	{
		// 对代码施加两次const或volatile时，后面那个修饰符会被忽略
		enum { value = Conversion<const volatile U *, const volatile T *>::exist &&
			Conversion<const volatile T *, const volatile void *>::sametype };

		enum { dontuse = sizeof(T) == sizeof(U) };
	};
	template<>
	struct SuperSubclass<void, void>
	{
		enum { value = false };
	};
	template<typename U>
	struct SuperSubclass<void, U>
	{
		enum { value = Conversion<const volatile U *, const volatile void *>::exist &&
			Conversion<const volatile void *, const volatile void *>::sametype };

		enum { dontuse = 0 == sizeof(U) };
	};
	template<typename T>
	struct SuperSubclass<T, void>
	{
		enum { value = Conversion<const volatile void *, const volatile T *>::exist &&
			Conversion<const volatile T *, const volatile void *>::sametype };

		enum { dontuse = sizeof(T) == 0 };
	};




	template<typename T, typename U>
	struct SuperSubclassStrict
	{
		enum { value = Conversion<const volatile U *, const volatile T *>::exist &&
			Conversion<const volatile T *, const volatile void *>::sametype &&
			Conversion<const volatile T *, const volatile U *>::sametype };

		enum { dontuse = sizeof(T) == sizeof(U) };
	};
	template<>
	struct SuperSubclassStrict<void, void>
	{
		enum { value = false };
	};
	template<typename U>
	struct SuperSubclassStrict<void, U>
	{
		enum { value = Conversion<const volatile U *, const volatile void *>::exist &&
			Conversion<const volatile void *, const volatile void *>::sametype &&
			Conversion<const volatile void *, const volatile U *>::sametype };

		enum { dontuse = 0 == sizeof(U) };
	};
	template<typename T>
	struct SuperSubclassStrict<T, void>
	{
		enum { value = Conversion<const volatile void *, const volatile T *>::exist &&
			Conversion<const volatile void *, const volatile void *>::sametype &&
			Conversion<const volatile T *, const volatile void *>::sametype };

		enum { dontuse = sizeof(T) == 0 };
	};

	#define SUPERSUBCLASS(T, U)	\
		SuperSubclass<T, U>::value
	#define SUPERSUBCLASS_STRICT(T, U)	\
		SuperSubclassStrict<T, U>::value



	/************************************************************************
	*  
	*	MustBePODOrVoid	: 如果一个类具有特殊的构造函数、拷贝构造函数、析构函数、赋值操作符函数，则不能作为联合的成员
	*	调用方式			: MustBePODOrVoid<int>()
	*	
	*
	***********************************************************************/

	namespace align
	{
		struct a1 { char s; };
		struct a2 { short s; };
		struct a4 { int s; };
		struct a8 { double s; };
		struct a16 { long double s; };
	}

	typedef align::a16 max_align;

	template<size_t N>
	struct TypeWithAligment
	{
		typedef align::a16 type;
	};

	template<> 
	struct TypeWithAligment<1>
	{
		typedef align::a1 type;
	};

	template<> 
	struct TypeWithAligment<2>
	{
		typedef align::a2 type;
	};

	template<> 
	struct TypeWithAligment<4>
	{
		typedef align::a4 type;
	};

	template<> 
	struct TypeWithAligment<8>
	{
		typedef align::a8 type;
	};

	template<> 
	struct TypeWithAligment<16>
	{
		typedef align::a16 type;
	};
}



#endif