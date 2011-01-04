#ifndef __ULTILITY_NULL_HPP
#define __ULTILITY_NULL_HPP




namespace DataStructure
{
	// =========================================================================
	// class Nullv
	// �����ʾ��ָ��ֵ����˿������캯���Լ��������Ʋ����������ء�


	struct Nullv
	{
	public:
		Nullv()
		{}
	private:
		Nullv(const Nullv &);
		Nullv &operator=(const Nullv &);

	public:
		static Nullv Create()
		{
			return Nullv();
		}

		// Conversion
	public:
		template<typename T>
		operator T *() const
		{
			return 0;
		}

		// ֧�ֳ�Ա����
		template<typename T, typename C>
		operator T C::*() const
		{
			return 0;
		}

	public:
		template<typename T>
		bool Equal(const T &rhs) const
		{
			return rhs == 0;
		}

	private:
		// ����ȡ��ַ������
		void operator &() const;
	};


	template<typename T>
	inline bool operator==(const Nullv &lhs, const T &rhs)
	{
		return lhs.Equal(rhs);
	}
	template<typename T>
	inline bool operator==(const T &lhs, const Nullv &rhs)
	{
		return rhs.Equal(lhs);
	}
	template<typename T>
	inline bool operator!=(const Nullv &lhs, const T &rhs)
	{
		return !lhs.Equal(rhs);
	}
	template<typename T>
	inline bool operator!=(const T &lhs, const Nullv &rhs)
	{
		return !rhs.Equal(lhs);
	}
}




#endif