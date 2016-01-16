
#include "stdafx.h"
#include "CppUnitTest.h"
#include "TimeUtility.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilisTest
{
	TEST_CLASS(TimeUtilityTest)
	{
	public:
		
		TEST_METHOD(TimeUlitity_current_timestamp)
		{
			std::string timestamp = Time::TimeUtility::current_timestamp();
			Assert::AreEqual(14U, (unsigned int)timestamp.length());
			Logger::WriteMessage(timestamp.c_str());
		}

		TEST_METHOD(TimeUtility_convert_to_unixtimestamp)
		{
			std::string timestamp = "2012-04-03 09:22:04";
			long actual = Time::TimeUtility::convert_to_unixtimestamp(timestamp);
			long expected = 1333412524;
			Assert::AreEqual(expected, actual);

			timestamp = "2324242/4232/323";
			actual = Time::TimeUtility::convert_to_unixtimestamp(timestamp);
			Assert::AreEqual(-1L, actual);
		}
		
	};
}
