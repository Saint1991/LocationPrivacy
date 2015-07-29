#include "stdafx.h"
#include "CppUnitTest.h"
#include "GeographicalCalculation.h"
#include "AngleUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilsTest
{

	///<summary>
	/// 距離系等の計算はhttp://japonyol.net/editor/distancia.htmlで算出できる距離をactualとしてテストを書いてます
	/// 各種計算方法でこれとは当然誤差が出るのでprecisionは値をみつつ，
	/// まぁこれくらいなら妥当だろう(計算的には間違っていないだろう)という範囲を設定しています・
	///</summary>
	TEST_CLASS(GeographycalCalculationTest)
	{
	private:
		const double precision = 1.0e-10;
	
	public:
		TEST_METHOD(lambert_dist1) 
		{
			Geography::LatLng from(34.8185, 135.5224);
			Geography::LatLng to(34.819, 135.5244);

			double expected = 193.0;
			double actual = Geography::GeographicalCalculation::lambert_distance(from, to);			

			Assert::AreEqual(expected, actual, 10);
		}

	};
}