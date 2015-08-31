
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
			Graph::BasicPathData path_data(100.0);
			Graph::BasicPath path(1L, std::make_shared<Graph::BasicPathData>(path_data));
			Assert::AreEqual(1L, path.get_to(), PRECISION);
			Assert::AreEqual(100.0, path.distance(), PRECISION);
		}

		TEST_METHOD(BasicPath_distance)
		{
			Graph::BasicPathData path_data(100.0);
			Graph::BasicPath path(1L, std::make_shared<Graph::BasicPathData>(path_data));
			double dist = path.distance();
			Assert::AreEqual(100.0, dist);
		}

		TEST_METHOD(BasicPath_update_distance)
		{
			Graph::BasicPathData path_data(100.0);
			Graph::BasicPath path(1L, std::make_shared<Graph::BasicPathData>(path_data));
			path.update_distance(50.0);
			Assert::AreEqual(50.0, path.distance(), PRECISION);
		}
	};
}
