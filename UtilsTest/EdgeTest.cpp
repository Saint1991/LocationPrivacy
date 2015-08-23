#include "stdafx.h"
#include "CppUnitTest.h"
#include "BasicPathData.h"
#include "Edge.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Graph;

namespace UtilsTest
{
	TEST_CLASS(EdgeTest)
	{
	public:
		const double PRECISION = 1.0E-8;

		TEST_METHOD(Edge_Constructor)
		{
			Edge<BasicPathData> path(2L, std::make_shared<BasicPathData>(2.3));
			std::shared_ptr<BasicPathData> data = path.get_data();
			std::shared_ptr<BasicPathData const> static_data = path.get_static_data();

			double actual_distance = static_data->get_distance();
			Assert::AreEqual(2L, path.get_to(), PRECISION);
			Assert::AreEqual(2.3, actual_distance);
		}
	};
}
