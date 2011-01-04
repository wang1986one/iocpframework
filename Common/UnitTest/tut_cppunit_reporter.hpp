
#ifndef TUT_CPPUNIT_REPORTER
#define TUT_CPPUNIT_REPORTER

#include "tut.hpp"



namespace tut
{

	/**
	* CppUnit TUT reporter
	*/
	class CPPUnitReporter 
		: public tut::Callback
	{
		std::vector<tut::TestResult> failed_tests_;
		std::vector<tut::TestResult> passed_tests_;
		const std::string filename_;
		std::auto_ptr<std::ostream> stream_;

	public:
		explicit CPPUnitReporter(const std::string &filename = "testResult.xml")
			: failed_tests_(),
			passed_tests_(),
			filename_(filename),
			stream_(new std::ofstream(filename_.c_str()))
		{
			if( !stream_->good()) 
				throw TutError("Cannot open output file `" + filename_ + "`");
		}

		explicit CPPUnitReporter(std::ostream &stream)
			: failed_tests_(),
			passed_tests_(),
			filename_(),
			stream_(&stream)
		{
		}

		~CPPUnitReporter()
		{
			if(filename_.empty())
			{
				stream_.release();
			}
		}

	private:
		CPPUnitReporter(const CPPUnitReporter &);
		CPPUnitReporter &operator=(const CPPUnitReporter &);

	public:
		void RunStart()
		{
			failed_tests_.clear();
			passed_tests_.clear();
		}

		void TestCompleted(const tut::TestResult& tr)
		{
			assert(tr.result_ != TestResult::dummy );
			if(  (tr.result_ == TestResult::ok) ||
				(tr.result_ == TestResult::skipped) )
			{
				passed_tests_.push_back(tr);
			}
			else
			{
				failed_tests_.push_back(tr);
			}
		}

		void RunCompleted()
		{
			int errors = 0;
			int failures = 0;
			std::string failure_type;
			std::string failure_msg;

			*stream_ << "<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\" ?>" << std::endl
				<< "<TestRun>" << std::endl;

			if( failed_tests_.size() > 0)
			{
				*stream_ << "  <FailedTests>" << std::endl;

				for (unsigned int i=0; i<failed_tests_.size(); i++)
				{
					switch (failed_tests_[i].result_)
					{
					case TestResult::fail:
						failure_type = "Assertion";
						failure_msg  = "";
						failures++;
						break;
					case TestResult::ex:
						failure_type = "Assertion";
						failure_msg  = "Thrown exception: " + failed_tests_[i].exceptionTypeid_ + '\n';
						failures++;
						break;
					case TestResult::warn:
						failure_type = "Assertion";
						failure_msg  = "Destructor failed\n";
						failures++;
						break;
					case TestResult::term:
						failure_type = "Error";
						failure_msg  = "Test application terminated abnormally\n";
						errors++;
						break;
					case TestResult::ex_ctor:
						failure_type = "Error";
						failure_msg  = "Constructor has thrown an exception: " + failed_tests_[i].exceptionTypeid_ + '\n';
						errors++;
						break;
					case TestResult::rethrown:
						failure_type = "Assertion";
						failure_msg  = "Child failed\n";
						failures++;
						break;
					default: // ok, skipped, dummy
						failure_type = "Error";
						failure_msg  = "Unknown testNum_ status, this should have never happened. "
							"You may just have found a bug in TUT, please report it immediately.\n";
						errors++;
						break;
					}

					*stream_ << "    <FailedTest id=\"" << failed_tests_[i].testNum_ << "\">" << std::endl
						<< "      <Name>" << Encode(failed_tests_[i].groupName_) + "::" + Encode(failed_tests_[i].name_) << "</Name>" << std::endl
						<< "      <FailureType>" << failure_type << "</FailureType>" << std::endl
						<< "      <Location>" << std::endl
						<< "        <File>Unknown</File>" << std::endl
						<< "        <Line>Unknown</Line>" << std::endl
						<< "      </Location>" << std::endl
						<< "      <Message>" << Encode(failure_msg + failed_tests_[i].msg_) << "</Message>" << std::endl
						<< "    </FailedTest>" << std::endl;
				}

				*stream_ << "  </FailedTests>" << std::endl;
			}

			/* *********************** passed tests ***************************** */
			if( passed_tests_.size() > 0) {
				*stream_ << "  <SuccessfulTests>" << std::endl;

				for (unsigned int i=0; i<passed_tests_.size(); i++)
				{
					*stream_ << "    <Test id=\"" << passed_tests_[i].testNum_ << "\">" << std::endl
						<< "      <Name>" << Encode(passed_tests_[i].groupName_) + "::" + Encode(passed_tests_[i].name_) << "</Name>" << std::endl
						<< "    </Test>" << std::endl;
				}

				*stream_ << "  </SuccessfulTests>" << std::endl;
			}

			/* *********************** statistics ***************************** */
			*stream_ << "  <Statistics>" << std::endl
				<< "    <Tests>" << (failed_tests_.size() + passed_tests_.size()) << "</Tests>" << std::endl
				<< "    <FailuresTotal>" << failed_tests_.size() << "</FailuresTotal>" << std::endl
				<< "    <Errors>" << errors << "</Errors>" << std::endl
				<< "    <Failures>" << failures << "</Failures>" << std::endl
				<< "  </Statistics>" << std::endl;

			/* *********************** footer ***************************** */
			*stream_ << "</TestRun>" << std::endl;
		}

		bool AllOK() const
		{
			return failed_tests_.empty();
		}

		/**
		* \brief Encodes text to XML
		* XML-reserved characters (e.g. "<") are encoded according to specification
		* @param text text to be encoded
		* @return encoded string
		*/
		static std::string Encode(const std::string & text)
		{
			std::string out;

			for (unsigned int i=0; i<text.length(); ++i) 
			{
				char c = text[i];
				switch (c) 
				{
				case '<':
					out += "&lt;";
					break;
				case '>':
					out += "&gt;";
					break;
				case '&':
					out += "&amp;";
					break;
				case '\'':
					out += "&apos;";
					break;
				case '"':
					out += "&quot;";
					break;
				default:
					out += c;
				}
			}

			return out;
		}
	};

}

#endif

