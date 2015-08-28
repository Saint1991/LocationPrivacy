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

		TEST_METHOD(Edge_update_data)
		{
			Edge<BasicPathData> path(2L, std::make_shared<BasicPathData>(1.0));
			path.update_data(std::make_shared<BasicPathData>(2.0));
			std::shared_ptr<BasicPathData> data = path.get_data();

			double dist = data->get_distance();
			Assert::AreEqual(2.0, dist, PRECISION);
		}

		TEST_METHOD(Edge_operators)
		{
			Edge<BasicPathData> path1(1L, std::make_shared<BasicPathData>(1.0));
			Edge<BasicPathData> path2(2L, std::make_shared<BasicPathData>(1.0));
			Edge<BasicPathData> path3(2L, std::make_shared<BasicPathData>(1.0));

			Assert::IsFalse(path1 == path2);
			Assert::IsTrue(path2 == path3);
			Assert::IsTrue(path1 != path2);
			Assert::IsFalse(path2 != path3);
		}

	};
}
