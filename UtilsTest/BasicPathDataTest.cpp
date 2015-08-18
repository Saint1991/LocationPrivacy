#include "stdafx.h"
#include "CppUnitTest.h"
#include "BasicPathData.h"
#include "Coordinate.h"
#include "LatLng.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Graph;

namespace UtilsTest
{
	TEST_CLASS(BasicPathDataTest)
	{
	public:

		const double precision = 0.1;

		TEST_METHOD(BasicPathData_Constructors)
		{
			BasicPathData data1(0.2);
		}
	};
}
