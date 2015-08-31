
#include "stdafx.h"
#include "CppUnitTest.h"
#include "BasicGeoNodeData.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilsTest
{
	TEST_CLASS(BasicGeoNodeDataTest)
	{
	public:
		const double PRECISION = 1.0E-10;

		TEST_METHOD(BasicGeoNodeData_Constructor1)
		{
			Geography::LatLng position(1.0,2.0);
			Geography::BasicGeoNodeData node_data(position);
			Assert::AreEqual(1.0, node_data.get_position().lat(), PRECISION);
			Assert::AreEqual(2.0, node_data.get_position().lng(), PRECISION);
		}

		TEST_METHOD(BasicGeoNodeData_Constructor2)
		{
			Geography::BasicGeoNodeData node_data(1.0, 2.0);
			Assert::AreEqual(1.0, node_data.get_position().lat(), PRECISION);
			Assert::AreEqual(2.0, node_data.get_position().lng(), PRECISION);
		}

	};
}
