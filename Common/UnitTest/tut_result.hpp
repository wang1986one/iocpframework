#ifndef TUT_RESULT_H_GUARD__
#define TUT_RESULT_H_GUARD__


#include "tut_config.hpp"




namespace tut
{

	// ----------------------------------------------
	// typeid

	template<typename T>
	inline std::string TypeName(const T& t)
	{
		return typeid(t).name();
	}





	/**
	* Return type of runned testNum_/testNum_ groupName_.
	*
	* For testNum_: contains result_ of testNum_ and, possible, msg_
	* for failure or exception.
	*/
	struct TestResult
	{
		/**
		* Test groupName_ name_.
		*/
		std::string groupName_;

		/**
		* Test number in group.
		*/
		int testNum_;

		/**
		* Test name_ (optional)
		*/
		std::string name_;

		/**
		* result_ of a testNum_ 
		*/
		enum ResultType
		{
			ok,       //< testNum_ finished successfully
			fail,     //< testNum_ failed with ensure() or fail() methods
			ex,       //< testNum_ throwed an exceptions
			warn,     //< testNum_ finished successfully, but testNum_ destructor throwed
			term,     //< testNum_ forced testNum_ application to terminate abnormally
			ex_ctor,  //< 
			rethrown, //< 
			skipped,  //< 
			dummy     //< 
		};

		ResultType result_;

		/**
		* Exception msg_ for failed testNum_.
		*/
		std::string msg_;
		std::string exceptionTypeid_;


	public:
		TestResult()
			: groupName_()
			, testNum_(0)
			, name_()
			, result_(ok)
			, msg_()
			, exceptionTypeid_()
		{}

		TestResult(const std::string& grp, int pos, const std::string& test_name, ResultType res)
			: groupName_(grp)
			, testNum_(pos)
			, name_(test_name)
			, result_(res)
			, msg_()
			, exceptionTypeid_()
		{}

		TestResult(const std::string& grp,int pos, const std::string& test_name, ResultType res, const std::exception& ex)
			: groupName_(grp)
			, testNum_(pos)
			, name_(test_name)
			, result_(res)
			, msg_(ex.what())
			, exceptionTypeid_(TypeName(ex))
		{}

		TestResult(const std::string& grp,int pos, const std::string& test_name, ResultType res, const std::string& ex_typeid, const std::string& msg)
			: groupName_(grp)
			, testNum_(pos)
			, name_(test_name)
			, result_(res)
			, msg_(msg)
			, exceptionTypeid_(ex_typeid)
		{}

		virtual ~TestResult()
		{}
	};

}

#endif
