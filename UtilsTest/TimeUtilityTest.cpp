
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
			Logger::WriteMessage(timestamp.c_str());
		}
		
	};
}
