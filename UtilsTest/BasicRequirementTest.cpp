
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
			Requirement::BasicRequirement requirement(20.0, 16U);
			Assert::AreEqual(20.0, requirement.required_anonymous_area);
			Assert::AreEqual(16U, requirement.dummy_num);
		}
	};
}
