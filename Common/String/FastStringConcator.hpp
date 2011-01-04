#ifndef __STRING_FAST_STRING_CONCATOR_HPP
#define __STRING_FAST_STRING_CONCATOR_HPP



namespace DataStructure
{

	class _FscSeed{};

	template<typename StringT>
	class _FastSeedT
		: public _FscSeed
	{
	public:
		typedef StringT		StringType;
	};


	template<
		typename StringT, 
		typename CharT = typename StringT::value_type, 
		typename CharTraitsT = CharTraits<CharT>
		>
	class FastStringConcator
	{
	public:
		typedef StringT					StringType;
		typedef CharT					char_type;
		typedef CharTraitsT				CharTraitsType;
		typedef size_t					size_type;

		typedef FastStringConcator<StringType, char_type, CharTraitsType>	ClassType;


	private:
		struct data;

		data m_lhs;
		data m_rhs;


	public:
		FastStringConcator(const StringType &lhs,	const StringType &rhs);
		FastStringConcator(const StringType &lhs,	const char_type rhs);
		FastStringConcator(const StringType &lhs,	const char_type rhs);

		FastStringConcator(const char_type *lhs,		const StringType &rhs);
		FastStringConcator(const char_type lhs,		const StringType &rhs);
		

		FastStringConcator(const ClassType &lhs,	const StringType &rhs);
		FastStringConcator(const ClassType &lhs,	const char_type *rhs);
		FastStringConcator(const ClassType &lhs,	const char_type rhs);

		FastStringConcator(const _FscSeed &lhs,		const StringType &rhs);

		FastStringConcator(const ClassType &lhs,	const ClassType &rhs);
		FastStringConcator(const StringType &lhs,	const ClassType &rhs);
		FastStringConcator(const char_type *lhs,		const ClassType &rhs);
		FastStringConcator(const char_type lhs,		const ClassType &rhs);

	private:
		FastStringConcator &operator=(const ClassType&);


	public:
		operator StringType() const;

	private:
		size_type _Length() const
		{
			return m_lhs.Length() + m_rhs.Length();
		}

		char_type *Write(char_type *pStr) const
		{
			return m_rhs.Write(m_lhs.Write(pStr));
		}


	private:
		friend struct data;

		struct data
		{
			enum DataType
			{
				seed,			// 参数为seed类型
				single,			// 参数为single类型
				cstring,		// 参数为cstring类型
				concat			// 参数为concat类型
			};

			// C-Style String
			struct CString
			{
				size_type m_len;
				const char_type *m_pStr;
			};

			union DataRef
			{
				CString cstring;
				char_type ch;
				const ClassType* concat; 
			};


			DataRef ref;
			const DataType type;


			data(const StringType &str)
				: type(cstring)
			{
				ref.cstring.m_len = str.Length();
				ref.cstring.m_pStr = str.data();
			}
			data(const char_type *pStr)
				: type(cstring)
			{
				ref.cstring.m_len = CharTraitsType::Length(pStr);
				ref.cstring.m_pStr = pStr;
			}


			//....未完！
		};

	};
}





#endif