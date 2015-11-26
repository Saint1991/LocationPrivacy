
#include "stdafx.h"
#include "CppUnitTest.h"
#include "BasicRequirement.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilsTest
{
	TEST_CLASS(BasicRequirementTest)
	{
	public:

		TEST_METHOD(BasicRequirement_Constructor)
		{
			Requirement::BasicRequirement requirement(20.0, 16U, 2.0, 5.0);
			Assert::AreEqual(20.0, requirement.required_anonymous_area);
			Assert::AreEqual(16U, requirement.dummy_num);
			Assert::AreEqual(2.0, requirement.average_speed_of_dummy, 1.0E-8);
			Assert::AreEqual(5.0, requirement.speed_range_of_dummy, 1.0E-8);
		}

		TEST_METHOD(BasicRequirement_Constructor2)
		{
			Requirement::BasicRequirement requirement(20.0, 16U);
			Assert::AreEqual(20.0, requirement.required_anonymous_area);
			Assert::AreEqual(16U, requirement.dummy_num);
			Assert::AreEqual(1.5, requirement.average_speed_of_dummy, 1.0E-8);
			Assert::AreEqual(0.5, requirement.speed_range_of_dummy, 1.0E-8);
		}
	};
}
