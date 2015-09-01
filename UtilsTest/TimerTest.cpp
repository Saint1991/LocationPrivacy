
#include "stdafx.h"
#include "CppUnitTest.h"
#include "Timer.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilsTest
{
	TEST_CLASS(TimerTest)
	{
	public:
		TEST_METHOD(Timer_start_and_end)
		{
			time_t start_time = 0;
			Time::Timer timer(start_time);
		}
		
		TEST_METHOD(Timer_is_finished)
		{

		}
		
		TEST_METHOD(Timer_duration)
		{

		}
		
		TEST_METHOD(Timer_to_string)
		{

		}
	};
}
