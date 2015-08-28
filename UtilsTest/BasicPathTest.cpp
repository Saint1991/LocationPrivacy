
#include "stdafx.h"
#include "CppUnitTest.h"
#include "BasicPath.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilsTest
{
	TEST_CLASS(BasicPathTest)
	{
	public:

		TEST_METHOD(BasicPath_Constructor)
		{

		}

		TEST_METHOD(BasicPath_distance)
		{
			Graph::BasicPathData bpd(100.0);
			Graph::BasicPath bp(1L, std::make_shared<Graph::BasicPathData>(bpd));
			

		
		}

		TEST_METHOD(BasicPath_update_distance)
		{

		}
	};
}
