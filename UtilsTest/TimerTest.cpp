
#include "stdafx.h"
#include "CppUnitTest.h"
#include "Timer.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilsTest
{
	TEST_CLASS(TimerTest)
	{
	public:
		TEST_METHOD(Timer_Constructor)
		{
			Time::Timer time;
			Assert::IsFalse(time.is_finished());
			time.start();
			Assert::IsFalse(time.is_finished());
			time.end();
			Assert::IsTrue(time.is_finished());

			std::string message = "duration:" + std::to_string(time.duration());
			Logger::WriteMessage(message.c_str());
		}
		
	};
}
