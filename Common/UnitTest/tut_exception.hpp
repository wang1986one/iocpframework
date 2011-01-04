#ifndef TUT_EXCEPTION_H_GUARD__
#define TUT_EXCEPTION_H_GUARD__



#include <stdexcept>
#include "tut_result.hpp"


namespace tut
{

	// -------------------------------------------------------------------
	// class TUTError

	// The base for all TUT exceptions.

	struct TutError 
		: public std::exception
	{
		explicit TutError(const std::string& msg)
			: err_msg(msg)
		{}

		virtual TestResult::ResultType result() const
		{
			return TestResult::ex;
		}

		virtual std::string type() const
		{
			return "tut::TutError";
		}

		const char* what() const throw()
		{
			return err_msg.c_str();
		}

		~TutError() throw()
		{
		}

	private:
		TutError &operator=(const TutError &);

		const std::string err_msg;
	};


	// -------------------------------------------------------------------
	// class NoSuchGroup
	
	// Group not found exception.

	struct NoSuchGroup 
		: public TutError
	{
		explicit NoSuchGroup(const std::string& grp)
			: TutError(grp)
		{}

		virtual std::string type() const
		{
			return "tut::NoSuchGroup";
		}

		~NoSuchGroup() throw()
		{
		}
	};

	// -------------------------------------------------------------------
	// class NoSuchTest

	// Test not found exception.

	struct NoSuchTest 
		: public TutError
	{
		explicit NoSuchTest(const std::string& grp)
			: TutError(grp)
		{}

		virtual std::string type() const
		{
			return "tut::NoSuchTest";
		}

		~NoSuchTest() throw()
		{
		}
	};

	
	// -------------------------------------------------------------------
	// class BadCtor

	/* Internal exception to be throwed when
	* test constructor has failed.
	*/
	struct BadCtor 
		: public TutError
	{
		explicit BadCtor(const std::string& msg)
			: TutError(msg)
		{}

		TestResult::ResultType result() const
		{
			return TestResult::ex_ctor;
		}

		virtual std::string type() const
		{
			return "tut::BadCtor";
		}

		~BadCtor() throw()
		{
		}
	};


	// -------------------------------------------------------------------
	// class Failure

	// Exception to be throwed when ensure() fails or fail() called.

	struct Failure 
		: public TutError
	{
		explicit Failure(const std::string& msg)
			: TutError(msg)
		{}

		TestResult::ResultType result() const
		{
			return TestResult::fail;
		}

		virtual std::string type() const
		{
			return "tut::Failure";
		}

		~Failure() throw()
		{
		}
	};

	// -------------------------------------------------------------------
	// class Warning
	
	// Exception to be throwed when test desctructor throwed an exception.

	struct Warning 
		: public TutError
	{
		explicit Warning(const std::string& msg)
			: TutError(msg)
		{}

		TestResult::ResultType result() const
		{
			return TestResult::warn;
		}

		virtual std::string type() const
		{
			return "tut::Warning";
		}

		~Warning() throw()
		{
		}
	};

	
	// -------------------------------------------------------------------
	// class SEH
	
	// Exception to be throwed when test issued SEH (Win32)
	
	struct SEH : public TutError
	{
		explicit SEH(const std::string& msg)
			: TutError(msg)
		{}

		virtual TestResult::ResultType result() const
		{
			return TestResult::term;
		}

		virtual std::string type() const
		{
			return "tut::SEH";
		}

		~SEH() throw()
		{
		}
	};


	// -------------------------------------------------------------------
	// class Rethrown
	
	// Exception to be throwed when child processes fail.

	struct Rethrown 
		: public Failure
	{
		explicit Rethrown(const TestResult &result)
			: Failure(result.msg_), tr(result)
		{}

		virtual TestResult::ResultType result() const
		{
			return TestResult::rethrown;
		}

		virtual std::string type() const
		{
			return "tut::Rethrown";
		}

		~Rethrown() throw()
		{
		}

		const TestResult tr;
	};

	
	// -------------------------------------------------------------------
	// class Skipped

	struct Skipped : public TutError
	{
		explicit Skipped(const std::string& msg)
			: TutError(msg)
		{}

		virtual TestResult::ResultType result() const
		{
			return TestResult::skipped;
		}

		virtual std::string type() const
		{
			return "tut::Skipped";
		}

		~Skipped() throw()
		{
		}
	};

}

#endif
