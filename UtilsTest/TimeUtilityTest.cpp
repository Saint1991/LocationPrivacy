
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
			Assert::AreEqual(14U, timestamp.length());
			Logger::WriteMessage(timestamp.c_str());
		}
		
	};
}
