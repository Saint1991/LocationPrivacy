
#include "stdafx.h"
#include "CppUnitTest.h"
#include "BasicPath.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilsTest
{
	TEST_CLASS(BasicPathTest)
	{
	public:
		const double PRECISION = 1.0E-10;
		TEST_METHOD(BasicPath_Constructor)
		{
			Graph::BasicPathData bpd(100.0);
			Graph::BasicPath bp(1L, std::make_shared<Graph::BasicPathData>(bpd));
			Assert::AreEqual(1L, bp.get_to(), PRECISION);
			Assert::AreEqual(100.0, bp.distance(), PRECISION);
		}

		TEST_METHOD(BasicPath_distance)
		{
			Graph::BasicPathData bpd(100.0);
			Graph::BasicPath bp(1L, std::make_shared<Graph::BasicPathData>(bpd));
			double dist = bp.distance();
			Assert::AreEqual(100.0, dist);

		}

		TEST_METHOD(BasicPath_update_distance)
		{
			Graph::BasicPathData bpd(100.0);
			Graph::BasicPath bp(1L, std::make_shared<Graph::BasicPathData>(bpd));
			bp.update_distance(50.0);
			Assert::AreEqual(50.0, bp.distance(), PRECISION);
		}
	};
}
