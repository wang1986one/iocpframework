#ifndef TUT_CONSOLE_REPORTER
#define TUT_CONSOLE_REPORTER

#include "tut.hpp"



namespace
{

	std::ostream& operator<<(std::ostream& os, const tut::TestResult& tr)
	{
		switch(tr.result_)
		{
		case tut::TestResult::ok:
			os << '.';
			break;
		case tut::TestResult::fail:
			os << '[' << tr.testNum_ << "=F]";
			break;
		case tut::TestResult::ex_ctor:
			os << '[' << tr.testNum_ << "=C]";
			break;
		case tut::TestResult::ex:
			os << '[' << tr.testNum_ << "=X]";
			break;
		case tut::TestResult::warn:
			os << '[' << tr.testNum_ << "=W]";
			break;
		case tut::TestResult::term:
			os << '[' << tr.testNum_ << "=T]";
			break;
		case tut::TestResult::rethrown:
			os << '[' << tr.testNum_ << "=P]";
			break;
		case tut::TestResult::skipped:
			os << '[' << tr.testNum_ << "=S]";
			break;
		case tut::TestResult::dummy:
			throw tut::TutError("console reporter called for dummy testNum_ result_");
		}

		return os;
	}

} // end of namespace

namespace tut
{

	/**
	* Default TUT Callback handler.
	*/
	class ConsoleReporter 
		: public tut::Callback
	{
		typedef std::vector<tut::TestResult> NotPassList;

	private:
		std::string current_group;
		NotPassList not_passed;
		std::ostream& os;


	public:
		int ok_count;
		int exceptions_count;
		int failures_count;
		int terminations_count;
		int warnings_count;
		int skipped_count;

		ConsoleReporter()
			: current_group(),
			not_passed(),
			os(std::cout),
			ok_count(0),
			exceptions_count(0),
			failures_count(0),
			terminations_count(0),
			warnings_count(0),
			skipped_count(0)
		{
			_Init();
		}

		ConsoleReporter(std::ostream& out)
			: current_group(),
			not_passed(),
			os(out),
			ok_count(0),
			exceptions_count(0),
			failures_count(0),
			terminations_count(0),
			warnings_count(0),
			skipped_count(0)

		{
			_Init();
		}

	private:
		ConsoleReporter(const ConsoleReporter &);
		ConsoleReporter &operator=(const ConsoleReporter &);

	public:
		virtual void RunStart()
		{
			_Init();
		}

		virtual void TestCompleted(const tut::TestResult& tr)
		{
			if( tr.groupName_ != current_group)
			{
				os << std::endl << tr.groupName_ << ": " << std::flush;
				current_group = tr.groupName_;
			}

			os << tr << std::flush;

			// update global statistics
			switch (tr.result_) 
			{
			case TestResult::ok:
				ok_count++;
				break;
			case TestResult::fail:
			case TestResult::rethrown:
				failures_count++;
				break;
			case TestResult::ex:
			case TestResult::ex_ctor:
				exceptions_count++;
				break;
			case TestResult::warn:
				warnings_count++;
				break;
			case TestResult::term:
				terminations_count++;
				break;
			case TestResult::skipped:
				skipped_count++;
				break;
			case tut::TestResult::dummy:
				assert( (tr.result_ != tut::TestResult::dummy) && "Should never be called");
			} // switch

			if(  (tr.result_ != tut::TestResult::ok) &&
				(tr.result_ != tut::TestResult::skipped) )
			{
				not_passed.push_back(tr);
			}
		}

		virtual void RunCompleted()
		{
			os << std::endl;

			if( not_passed.size() > 0)
			{
				NotPassList::const_iterator i = not_passed.begin();
				while (i != not_passed.end())
				{
					tut::TestResult tr = *i;

					os << std::endl;

					os << "---> " << "groupName_: " << tr.groupName_
						<< ", testNum_: testNum_<" << tr.testNum_ << ">"
						<< (!tr.name_.empty() ? (std::string(" : ") + tr.name_) : std::string())
						<< std::endl;
					os << "     problem: ";

					switch(tr.result_)
					{
					case TestResult::rethrown:
						os << "assertion failed in child" << std::endl;
						break;
					case TestResult::fail:
						os << "assertion failed" << std::endl;
						break;
					case TestResult::ex:
					case TestResult::ex_ctor:
						os << "unexpected exception" << std::endl;
						if( tr.exceptionTypeid_ != "" )
						{
							os << "     exception typeid: "
								<< tr.exceptionTypeid_ << std::endl;
						}
						break;
					case TestResult::term:
						os << "would be terminated" << std::endl;
						break;
					case TestResult::warn:
						os << "testNum_ passed, but cleanup code (destructor) raised"
							" an exception" << std::endl;
						break;
					default:
						break;
					}

					if( !tr.msg_.empty())
					{
						if( tr.result_ == TestResult::fail)
						{
							os << "     failed assertion: `" << tr.msg_ << "`"
								<< std::endl;
						}
						else
						{
							os << "     msg_: `" << tr.msg_ << "`"
								<< std::endl;
						}
					}

					++i;
				}
			}

			os << std::endl;

			os << "tests summary:";
			if( terminations_count > 0)
			{
				os << " terminations:" << terminations_count;
			}
			if( exceptions_count > 0)
			{
				os << " exceptions:" << exceptions_count;
			}
			if( failures_count > 0)
			{
				os << " failures:" << failures_count;
			}
			if( warnings_count > 0)
			{
				os << " warnings:" << warnings_count;
			}

			os << " ok:" << ok_count;

			if(skipped_count > 0)
			{
				os << " skipped:" << skipped_count;
			}
			os << std::endl;
		}

		virtual bool AllOK() const
		{
			return not_passed.empty();
		}

	private:
		void _Init()
		{
			ok_count = 0;
			exceptions_count = 0;
			failures_count = 0;
			terminations_count = 0;
			warnings_count = 0;
			skipped_count = 0;
			not_passed.clear();
		}
	};

}

#endif
