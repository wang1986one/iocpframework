#ifndef __CONVERSION_EXPLICIT_CAST_HPP
#define __CONVERSION_EXPLICIT_CAST_HPP


namespace DataStructure
{


	// =========================================================================
	// class explicit_cast
	// 显式转换操作符，关键字explicit将两个转换操作符标记为不能被用于隐式转换


	// 针对内置类型泛化
	template<typename T>
	class explicit_cast
	{
	public:
		typedef T					value_type;
		typedef explicit_cast<T>	ClassType;

	private:
		T m_t;

	public:
		explicit_cast(const T t)
			: m_t(t)
		{}
		// 强迫T只能为POD类型
		~explicit_cast()
		{
			union MustBasicType
			{
				int i;
				T   t;
			};
		}

	public:
		operator value_type() const
		{
			return m_t;
		}
	};


	// 针对引用类型偏特化
	template<typename T>
	class explicit_cast<T &>
	{
	public:
		typedef T					value_type;
		typedef explicit_cast<T>	ClassType;

	public:
		explicit_cast(const T t)
			: m_t(t)
		{}

	private:
		explicit_cast(T &);

	private:
		operator value_type();
	};


	// 针对const引用偏特化
	template<typename T>
	class explicit_cast<const T &>
	{
	public:
		typedef T					value_type;
		typedef explicit_cast<T>	ClassType;

	private:
		const T &m_t;

	public:
		explicit_cast(const T &t)
			: m_t(t)
		{}

	public:
		operator const value_type&() const
		{
			return m_t;
		}
	};



	// 针对指针类型偏特化
	template<typename T>
	class explicit_cast<T *>
	{
	public:
		typedef T					value_type;
		typedef explicit_cast<T>	ClassType;

	private:
		T *m_t;

	public:
		explicit_cast(T *t)
			: m_t(t)
		{}

	public:
		operator value_type *()
		{
			return m_t;
		}
	};



	// 针对const指针类型偏特化
	template<typename T>
	class explicit_cast<const T *>
	{
	public:
		typedef T					value_type;
		typedef explicit_cast<T>	ClassType;

	private:
		const T *m_t;

	public:
		explicit_cast(const T *t)
			: m_t(t)
		{}

	public:
		operator const value_type *() const
		{
			return m_t;
		}
	};
}







#endif