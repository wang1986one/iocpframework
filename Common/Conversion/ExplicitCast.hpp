#ifndef __CONVERSION_EXPLICIT_CAST_HPP
#define __CONVERSION_EXPLICIT_CAST_HPP


namespace DataStructure
{


	// =========================================================================
	// class explicit_cast
	// ��ʽת�����������ؼ���explicit������ת�����������Ϊ���ܱ�������ʽת��


	// ����������ͷ���
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
		// ǿ��Tֻ��ΪPOD����
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


	// �����������ƫ�ػ�
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


	// ���const����ƫ�ػ�
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



	// ���ָ������ƫ�ػ�
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



	// ���constָ������ƫ�ػ�
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