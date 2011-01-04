#ifndef __FUNCTOR_HPP
#define __FUNCTOR_HPP



namespace DataStructure
{
	// Adaptable Unary Function
	template<typename Arg, typename Result>
	struct UnAryFunction
	{
		typedef Arg		ArgumentType;
		typedef Result  ResultType;
	};

	// Adaptable Binary Function
	template<typename Arg1, typename Arg2, typename Result>
	struct BinaryFunction
	{
		typedef Arg1	FirstArgumentType;
		typedef Arg2	SecondArgumentType;
		typedef Result	ResultType;
	};



	// 算数类仿函数
	template<typename T>
	struct Plus
		: public BinaryFunction<T, T, T>
	{
		T operator()(const T &x, const T &y) const
		{
			return x + y;
		}
	};

	template<typename T>
	struct Minus
		: public BinaryFunction<T, T, T>
	{
		T operator()(const T &x, const T &y) const
		{
			return x - y;
		}
	};

	template<typename T>
	struct Multiplies
		: public BinaryFunction<T, T, T>
	{
		T operator()(const T &x, const T &y) const
		{
			return x * y;
		}
	};

	template<typename T>
	struct Divides
		: public BinaryFunction<T, T, T>
	{
		T operator()(const T &x, const T &y) const
		{
			return x / y;
		}
	};

	template<typename T>
	struct Modules
		: public BinaryFunction<T, T, T>
	{
		T operator()(const T &x, const T &y) const
		{
			return x % y;
		}
	};

	template<typename T>
	struct Negate
		: public UnAryFunction<T, T>
	{
		T operator()(const T &x) const
		{
			return -x;
		}
	};



	// 关系运算类仿函数
	template<typename T>
	struct EqualTo
		: public BinaryFunction<T, T, bool>
	{
		bool operator()(const T &x, const T &y) const
		{
			return x == y;
		}
	};

	template<typename T>
	struct Greater
		: public BinaryFunction<T, T, bool>
	{
		bool operator()(const T &x, const T &y) const
		{
			return x > y;
		}
	};

	template<typename T>
	struct Less
		: public BinaryFunction<T, T, bool>
	{
		bool operator()(const T &x, const T &y) const
		{
			return x < y;
		}
	};

	template<typename T>
	struct GreaterEqual
		: public BinaryFunction<T, T, bool>
	{
		bool operator()(const T &x, const T &y) const
		{
			return x >= y;
		}
	};

	template<typename T>
	struct LessEqual
		: public BinaryFunction<T, T, bool>
	{
		bool operator()(const T &x, const T &y) const
		{
			return x <= y;
		}
	};


	// 逻辑运算类仿函数
	template<typename T>
	struct LogicalAnd
		: public BinaryFunction<T, T, bool>
	{
		bool operator()(const T &x, const T &y) const
		{
			return x && y;
		}
	};

	template<typename T>
	struct LogicalOr
		: public BinaryFunction<T, T, bool>
	{
		bool operator()(const T &x, const T &y) const
		{
			return x || y;
		}
	};

	template<typename T>
	struct LogicalNot
		: public UnAryFunction<T, bool>
	{
		bool operator()(const T &x) const
		{
			return !x;
		}
	};

	// 证同元素(identity element)
	// 运算op的证同元素是数值A若与钙元素做op运算，会得到A自己。
	// 加法的证同元素为0,乘法的证同元素为1
	template<typename T>
	inline T IdentityElement(Plus<T>)
	{
		return T(0);
	}
	template<typename T>
	inline T IdentityElement(Multiplies<T>)
	{
		return T(1);
	}


	// 证同(identity)、选择(select)、投射(project)
	// 都只是将其参数原封不动的传回，间接性是抽象化的重要工具

	// 证同函数--任何数值通过此函数后，都不会有任何改变
	template<typename T>
	struct Identity
		: public UnAryFunction<T, T>
	{
		const T &operator()(const T &x) const
		{
			return x;
		}
	};

	// 选择函数--接受一个Pair，传回其第一元素
	template<typename Pair>
	struct Select1st
		: public UnAryFunction<Pair, typename Pair::FirstType>
	{
		const typename Pair::FirstType &operator()(const Pair &x) const
		{
			return x.first;
		}
	};
	// 接收一个Pair，传回其第二个元素
	template<typename Pair>
	struct Select2nd
		: public UnAryFunction<Pair, typename Pair::SecondType>
	{
		const typename Pair::SecondType &operator()(const Pair &x) const
		{
			return x.second;
		}
	};

	// 投射函数--传回第一参数，忽略第二参数
	template<typename Arg1, typename Arg2>
	struct Project1st:
		public BinaryFunction<Arg1, Arg2, Arg1>
	{
		Arg1 operator()(const Arg1 &x, const Arg2 &) const
		{
			return x;
		}
	};
	template<typename Arg1, typename Arg2>
	struct Project2nd:
		public BinaryFunction<Arg1, Arg2, Arg1>
	{
		Arg2 operator()(const Arg1 &, const Arg2 &y) const
		{
			return y;
		}
	};


	// 将某个Adaptable Binary Function转换为Unary Function

	//----------------------------------------------------------------------------
	// class Binder1st

	template<typename Operation>
	class Binder1st
		: public UnAryFunction<typename Operation::SecondArgumentType, typename Operation::ResultType>
	{
	protected:
		Operation op;
		typename Operation::FirstArgumentType value;

	public:
		Binder1st(const Operation &x, const typename Operation::FirstArgumentType &y)
			: op(x)
			, value(y)
		{}

		typename Operation::ResultType operator()(const typename Operation::SecondArgumentType &x) const
		{
			return op(value, x);
		}
	};
	template<typename Operation, typename T>
	inline Binder1st<Operation> Bind1st(const Operation &op, const T &x)
	{
		typedef typename Operation::FirstArgumentType argType;
		return Binder1st<Operation>(op, argType(x));
	}



	//-------------------------------------------------------------------------------------------
	// class Binder2nd

	template<typename Operation>
	class Binder2nd
		: public UnAryFunction<typename Operation::FirstArgumentType, typename Operation::ResultType>
	{
	protected:
		Operation op;
		typename Operation::SecondArgumentType value;

	public:
		Binder2nd(const Operation &x, const typename Operation::SecondArgumentType &y)
			: op(x)
			, value(y)
		{}

		typename Operation::ResultType operator()(const typename Operation::FirstArgumentType &x) const
		{
			return op(x, value);
		}
	};
	template<typename Operation, typename T>
	inline Binder2nd<Operation> Bind2nd(const Operation &op, const T &x)
	{
		typedef typename Operation::SecondArgumentType argType;
		return Binder2nd<Operation>(op, argType(x));
	}



	//-------------------------------------------------
	// class UnaryCompose  

	template<typename OperatorT1, typename OperatorT2>
	class UnaryCompose
		: public UnAryFunction<typename OperatorT2::ArgumentType, typename OperatorT1::ResultType>
	{
	private:
		OperatorT1 m_func1;
		OperatorT2 m_func2;

	public:
		UnaryCompose(const OperatorT1 &x, const OperatorT2 &y)
			: m_func1(x)
			, m_func2(y)
		{}

		typename OperatorT1::ResultType operator()(const typename OperatorT2::ArgumentType &x) const
		{
			return m_func1(m_func2(x));
		}
	};


	//-------------------------------------------------
	// class BinaryCompose

	template<typename OperatorT1, typename OperatorT2, typename OperatorT3>
	class BinaryCompose
		: public UnAryFunction<typename OperatorT2::ArgumentType,
							   typename OperatorT1::ResultType>
	{
	private:
		OperatorT1 m_func1;
		OperatorT2 m_func2;
		OperatorT3 m_func3;

	public:
		BinaryCompose(const OperatorT1 &x, const OperatorT2 &y, const OperatorT3 &z)
			: m_func1(x)
			, m_func2(y)
			, m_func3(z)
		{}

		typename OperatorT1::ResultType operator()(const typename OperatorT2::ArgumentType &x) const
		{
			return m_func1(m_func2(x), m_func3(x));
		}
	};

	// UnaryCompose和BinaryCompose的帮助函数
	template<typename OperatorT1, typename OperatorT2>
	inline UnaryCompose<OperatorT1, OperatorT2> Compose1(const OperatorT1 &func1, const OperatorT2 &func2)
	{
		return UnaryCompose<OperatorT1, OperatorT2>(func1, func2);
	}
	
	template<typename OperatorT1, typename OperatorT2, typename OperatorT3>
	inline BinaryCompose<OperatorT1, OperatorT2, OperatorT3> Compose2(const OperatorT1 &func1, const OperatorT2 &func2, const OperatorT3 &func3)
	{
		return BinaryCompose<OperatorT1, OperatorT2, OperatorT3>(func1, func2, func3);
	}


	//-------------------------------------------------
	// class PointerUnaryFunction

	template<typename ArgT, typename ResultT>
	class PointerUnaryFunction
		: public UnAryFunction<ArgT, ResultT>
	{
	private:
		ResultT (*m_pFunc)(ArgT);

	public:
		PointerUnaryFunction()
		{}
		explicit PointerUnaryFunction(ResultT (*pFunc)(ArgT))
			: m_pFunc(pFunc)
		{}

		ResultT operator()(ArgT val) const
		{
			return m_pFunc(val);
		}
	};
	
	template<typename ArgT, typename ResultT>
	inline PointerUnaryFunction<ArgT, ResultT> PtrFunc(ResultT (*pFunc)(ArgT))
	{
		return PointerUnaryFunction<ArgT, ResultT>(pFunc);
	}


	//-------------------------------------------------
	// class PointerBinaryFunction

	template<typename ArgT1, typename ArgT2, typename ResultT>
	class PointerBinaryFunction
		: public BinaryFunction<ArgT1, ArgT2, ResultT>
	{
	private:
		ResultT (*m_pFunc)(ArgT1, ArgT2);

	public:
		PointerBinaryFunction()
		{}
		explicit PointerBinaryFunction(ResultT (*pFunc)(ArgT1, ArgT2))
			: m_pFunc(pFunc)
		{}

		ResultT operator()(ArgT1 val1, ArgT2 val2) const
		{
			return m_pFunc(val1, val2);
		}
	};

	template<typename ArgT1, typename ArgT2, typename ResultT>
	inline PointerBinaryFunction<ArgT1, ArgT2, ResultT> PtrFunc(ResultT (*pFunc)(ArgT1, ArgT2))
	{
		return PointerBinaryFunction<ArgT1, ArgT2, ResultT>(pFunc);
	}

	//-------------------------------------------------
	// class Pointer3Function

	template<typename FuncT, typename PtrT, typename ArgT1, typename ArgT2, typename ArgT3>
	class Pointer3Function
	{
	private:
		PtrT m_ptr;
		FuncT m_fn;
		ArgT1 m1;
		ArgT2 m2;
		ArgT3 m3;

	public:
		Pointer3Function(FuncT fn, PtrT ptr, ArgT1 i, ArgT2 j, ArgT3 k)
			: m_ptr(ptr)
			, m_fn(fn)
			, m1(i)
			, m2(j)
			, m3(k)
		{}

		void operator()() const
		{
			m_fn(m_ptr, m1, m2, m3);
		}
	};

	template<typename FuncT, typename PtrT, typename ArgT1, typename ArgT2, typename ArgT3>
	inline Pointer3Function<FuncT, PtrT, ArgT1, ArgT2, ArgT3> PtrFunc(FuncT func, PtrT p1, ArgT1 i, ArgT2 j, ArgT3 k)
	{
		return Pointer3Function<FuncT, PtrT, ArgT1, ArgT2, ArgT3>(func, p1, i, j, k);
	}
	


	//-------------------------------------------------
	// class MemFunT--普通成员函数
	
	template<typename ClassT, typename ResultT>
	class MemFunT
		: public UnAryFunction<ClassT *, ResultT>
	{
	public:
		typedef ResultT		ResultType;
		typedef ClassT*		ArgumentType;

	private:
		ResultT (ClassT::*m_pFun)();

	public:
		explicit MemFunT(ResultT (ClassT::*pFun)())
			: m_pFun(pFun)
		{}

		ResultT operator()(ClassT *p) const
		{
			return (p->*m_pFun)();
		}
	};
	
	//-------------------------------------------------
	// class ConstMemFunT--const成员函数

	template<typename ClassT, typename ResultT>
	class ConstMemFunT
		: public UnAryFunction<const ClassT *, ResultT>
	{
	private:
		ResultT (ClassT::*m_pFun)() const;

	public:
		explicit ConstMemFunT(ResultT (ClassT::*pFun)() const)
			: m_pFun(pFun)
		{}

		ResultT operator()(const ClassT *p) const
		{
			return (p->*m_pFun)();
		}
	};

	//-------------------------------------------------
	// class MemFunRefT--引用

	template<typename ClassT, typename ResultT>
	class MemFunRefT
		: public UnAryFunction<ClassT, ResultT>
	{
	private:
		ResultT (ClassT::*m_pFun)();

	public:
		explicit MemFunRefT(ResultT (ClassT::*pFun)())
			: m_pFun(pFun)
		{}

		ResultT operator()(ClassT &t) const
		{
			return (t.*m_pFun)();
		}
	};

	//-------------------------------------------------
	// class ConstMemFunRefT--引用 const 成员函数

	template<typename ClassT, typename ResultT>
	class ConstMemFunRefT
		: public UnAryFunction<ClassT, ResultT>
	{
	private:
		ResultT (ClassT::*m_pFun)() const;

	public:
		explicit ConstMemFunRefT(ResultT (ClassT::*pFun)() const)
			: m_pFun(pFun)
		{}

		ResultT operator()(ClassT &t) const
		{
			return (t.*m_pFun)();
		}
	};


	//-------------------------------------------------
	// class MemFunT--普通成员函数,有一个参数

	template<typename ClassT, typename ResultT, typename ArgT>
	class MemFun1T
		: public BinaryFunction<ClassT *, ArgT, ResultT>
	{
	private:
		ResultT (ClassT::*m_pFun)(ArgT);

	public:
		explicit MemFun1T(ResultT (ClassT::*pFun)(ArgT))
			: m_pFun(pFun)
		{}

		ResultT operator()(ClassT *p, ArgT val) const
		{
			return (p->*m_pFun)(val);
		}
	};

	//-------------------------------------------------
	// class ConstMemFunT--const成员函数,有一个参数

	template<typename ClassT, typename ResultT, typename ArgT>
	class ConstMemFun1T
		: public BinaryFunction<const ClassT *, ArgT, ResultT>
	{
	private:
		ResultT (ClassT::*m_pFun)(ArgT) const;

	public:
		explicit ConstMemFun1T(ResultT (ClassT::*pFun)(ArgT) const)
			: m_pFun(pFun)
		{}

		ResultT operator()(const ClassT *p, ArgT val) const
		{
			return (p->*m_pFun)(val);
		}
	};

	//-------------------------------------------------
	// class MemFunRefT--引用,有一个参数

	template<typename ClassT, typename ResultT, typename ArgT>
	class MemFunRef1T
		: public BinaryFunction<ClassT, ArgT, ResultT>
	{
	private:
		ResultT (ClassT::*m_pFun)(ArgT);

	public:
		explicit MemFunRef1T(ResultT (ClassT::*pFun)(ArgT))
			: m_pFun(pFun)
		{}

		ResultT operator()(ClassT &t, ArgT val) const
		{
			return (t.*m_pFun)(val);
		}
	};

	//-------------------------------------------------
	// class ConstMemFunRefT--引用 const 成员函数

	template<typename ClassT, typename ResultT, typename ArgT>
	class ConstMemFunRef1T
		: public BinaryFunction<ClassT, ArgT, ResultT>
	{
	private:
		ResultT (ClassT::*m_pFun)(ArgT) const;

	public:
		explicit ConstMemFunRef1T(ResultT (ClassT::*pFun)(ArgT) const)
			: m_pFun(pFun)
		{}

		ResultT operator()(ClassT &t, ArgT val) const
		{
			return (t.*m_pFun)(val);
		}
	};
	

	//----------------------------------------------------
	// function MemFun

	template<typename ResultT, typename ClassT>
	inline MemFunT<ClassT, ResultT> MemFun(ResultT (ClassT::*pFun)())
	{
		return MemFunT<ClassT, ResultT>(pFun);
	}
	template<typename ResultT, typename ClassT>
	inline ConstMemFunT<ClassT, ResultT> MemFun(ResultT (ClassT::*pFun)() const)
	{
		return ConstMemFunT<ClassT, ResultT>(pFun);
	}
	template<typename ResultT, typename ClassT>
	inline MemFunRefT<ClassT, ResultT> MemFunRef(ResultT (ClassT::*pFun)())
	{
		return MemFunRefT<ClassT, ResultT>(pFun);
	}
	template<typename ResultT, typename ClassT>
	inline ConstMemFunRefT<ClassT, ResultT> MemFunRef(ResultT (ClassT::*pFun)() const)
	{
		return ConstMemFunRefT<ClassT, ResultT>(pFun);
	}
	template<typename ResultT, typename ClassT, typename ArgT>
	inline MemFun1T<ClassT, ResultT, ArgT> MemFun(ResultT (ClassT::*pFun)(ArgT))
	{
		return MemFun1T<ClassT, ResultT, ArgT>(pFun);
	}
	template<typename ResultT, typename ClassT, typename ArgT>
	inline ConstMemFun1T<ClassT, ResultT, ArgT> MemFun(ResultT (ClassT::*pFun)(ArgT) const)
	{
		return ConstMemFun1T<ClassT, ResultT, ArgT>(pFun);
	}
	template<typename ResultT, typename ClassT, typename ArgT>
	inline MemFunRef1T<ClassT, ResultT, ArgT> MemFunRef(ResultT (ClassT::*pFun)(ArgT))
	{
		return MemFunRef1T<ClassT, ResultT, ArgT>(pFun);
	}
	template<typename ResultT, typename ClassT, typename ArgT>
	inline ConstMemFunRef1T<ClassT, ResultT, ArgT> MemFunRef(ResultT (ClassT::*pFun)(ArgT) const)
	{
		return ConstMemFunRef1T<ClassT, ResultT, ArgT>(pFun);
	}


	//----------------------------------------------------------------------------------
	// class MemFun3T

	template<typename ResultT, typename ClassT, typename ArgT1, typename ArgT2, typename Argt3>
	class MemFun3T
	{
	private:
		ResultT (ClassT::*m_Ptr)(ArgT1, ArgT2, Argt3);

	public:
		explicit MemFun3T(ResultT (ClassT::*_Pm)(ArgT1, ArgT2, Argt3))
			:m_Ptr(_Pm) 
		{}

		ResultT operator()(ClassT *_P, ArgT1 arg1, ArgT2 arg2, Argt3 arg3) const
		{
			return ((_P->*m_Ptr)(arg1,arg2,arg3));
		}
	};
	
	template<typename ResultT,typename ClassT,typename ArgT1,typename ArgT2,typename Argt3>
	inline MemFun3T<ResultT,ClassT,ArgT1,ArgT2,Argt3> MemFun(ResultT (ClassT::*_Pm)(ArgT1, ArgT2, Argt3))
	{
		return (MemFun3T<ResultT,ClassT,ArgT1,ArgT2,Argt3>(_Pm));
	}

	
}




#endif