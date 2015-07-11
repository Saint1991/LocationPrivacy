#include "stdafx.h"
#include "CppUnitTest.h"
#include "LatLng.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilsTest
{
	TEST_CLASS(LatLngTest)
	{
	public:
		
		TEST_METHOD(constructor1)
		{
			Geography::LatLng ll(-90.0, 180.0);
		}

		TEST_METHOD(constructor2) 
		{
			try {
				Geography::LatLng ll(-90.1, 179.0);
				Assert::Fail();
			} 
			catch (std::invalid_argument e) {
				Logger::WriteMessage(e.what());
			}
		}

		TEST_METHOD(constructor3)
		{
			try {
				Geography::LatLng(-89.0, 181.0);
				Assert::Fail();
			}
			catch (std::invalid_argument e) {
				Logger::WriteMessage(e.what());
			}
		}

		

	};
}