#ifndef __CONSTRAINTS_HPP
#define __CONSTRAINTS_HPP


namespace DataStructure
{
	// ֻ������������
	class NullType;

	// �ɱ��̳еĿ�����
	struct EmptyType
	{};


	/************************************************************************
	*  
	*	MustHaveBase	: ��D���͵�ָ�븳ֵ��B���͵�ָ�룬ConstraintsΪ��̬��Ա������ȷ��û���κ������ڸ���
	*	���÷�ʽ			: MustHaveBase<X *, XX *>() �� MustHaveBase_(&b, &a)
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
	*	MustBeSubcriptable	: ȷ������������[]��ʽ����
	*	���÷�ʽ				: MustBeSubcriptable<X>() �� MustBeSubcriptable_(x)
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
	*	MustBePOD	: ���һ�����������Ĺ��캯�����������캯����������������ֵ������������������Ϊ���ϵĳ�Ա
	*	���÷�ʽ		: MustBePOD<int>() �� MustBePOD_(x)
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
	*	MustBePODOrVoid	: ���һ�����������Ĺ��캯�����������캯����������������ֵ������������������Ϊ���ϵĳ�Ա
	*	���÷�ʽ			: MustBePODOrVoid<int>()
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
			// ���ﲻ����NULL
			return static_cast<pFuncType>(0);
		}
	};
	


	// ���ԡ��ϵ�
	#define BREAK_ASSERT(x)    do { if(!x) __asm { int 3 }; } while(0)




	/************************************************************************
	*  
	*	IsVoid		: �������
	*	���÷�ʽ		: IsVoid<void>::value
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
	*	SizeOf		: ����sizeof����������void����
	*	���÷�ʽ		: SizeOf<void>::value  Sizeof<int>::value
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
	*	STATIC_CHECK		: ������ȷ�����ʽΪ��
	*	���÷�ʽ				: STATIC_ASSERT(true, m)
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
	*	Int2Type		: ������ӳ��Ϊ���ͣ��ɱ����ڼ�������Ľ��ѡ��ͬ�ĺ������ﵽ��̬����
	*	���÷�ʽ			: Int2Type<isPolymorphics>
	*	��������			: ����Ҫ����ĳ�������ڳ�������һ����������ͬ�ĺ���
	*					  �б�Ҫ�ڱ�����ʵʩ��̬����
	*
	*	Type2Type		: ���ú������ػ��ƣ�ģ��templateƫ�ػ��������������ͱ��������ͱ���Ϣ
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
	*	Type2Int		: ����ת��������ֵ����Ҫ�������ڵ�һ��ȷ����ֵ
	*	���÷�ʽ			: Type2Int<int>		Type2Int< Type2Int<int> >
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
	*	Select--����boolֵѡ������
	*	���÷�ʽ: Select<flag, T, U>::value_type
	*	T �� U�������ͣ���flagΪtrueʱ������T����֮��ΪU
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
	*	IsSameType--�ж��Ƿ�Ϊͬһ����
	*	���÷�ʽ: IsSameType<A, B>::value
	*	T �� U�������ͣ���flagΪtrueʱ������T����֮��ΪU
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
	*	Conversion--ָ���������ͼ�Ĺ�ϵ
	*	���÷�ʽ: Conversion<T, U>::exist		�Ƿ���ڴ�T��U����ʽ����ת��
	*			 Conversion<T, U>::exist2way	�Ƿ����T��U��U��T������ת��
	*			 Conversion<T, U>::sametype		�Ƿ�Ϊ��ͬ������
	*
	***********************************************************************/
	namespace Internal
	{
		template<typename T, typename U>
		struct ConversionHelper
		{
			typedef char Small;
			struct Big { char dummy[2]; };
			static Big Test(...);			// ֻ�����Զ�����ת�������������ʱ���Ż�ѡ�д����ȼ���͵�ת����ʽ
			static Small Test(U);			// ����һ��U���Ͳ���������Small
			static T MakeT();				// �����˺����������T��Ĭ�Ϲ��캯��Ϊprivateʱ��sizeof(T())����ͨ��
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
	*	SuperSubclass--�ж�����class֮���Ƿ���ڼ̳й�ϵ����Ϊprivate�̳�ʱ������
	*	���÷�ʽ: SuperSubclass<B, D>::value		��BΪD�Ĺ��м̳и������ͬ����ʱ����true
	*	
	*
	***********************************************************************/
	template<typename T, typename U>
	struct SuperSubclass
	{
		// �Դ���ʩ������const��volatileʱ�������Ǹ����η��ᱻ����
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
	*	MustBePODOrVoid	: ���һ�����������Ĺ��캯�����������캯����������������ֵ������������������Ϊ���ϵĳ�Ա
	*	���÷�ʽ			: MustBePODOrVoid<int>()
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