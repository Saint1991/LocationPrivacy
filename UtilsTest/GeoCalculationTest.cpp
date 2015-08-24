#include "stdafx.h"
#include "CppUnitTest.h"
#include "GeoCalculation.h"
#include "LatLng.h"
#include "AngleUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Geography;

namespace UtilsTest
{
	TEST_CLASS(GeoCalculationTest)
	{
	public:

		//距離の許容誤差(絶対値でなく距離のオーダーに対して許容誤差を決める)
		//すなわち下記はexpectedが100000mなら1000mまで誤差を許容するという意味
		// (大きいようにも見えるが，Google Mapの距離がそもそも正解とは限らないというか，Google Mapはhaversineの方をベースにやってるっぽい)
		const double DISTANCE_PRECISION = 1.0E-2;

		//角度の許容誤差[°]
		const double ANGLE_PRECISION = 0.1;

		//西尾研から理工学図書館までの距離(m)
		//Google Mapの距離計算を使って得られる距離が正しい距離としてテストしてます
		TEST_METHOD(GeoCalculation_lambert_distance)
		{
			LatLng nishio_lab(34.818292, 135.522105);
			LatLng library(34.822295, 135.522794);
			double actual = GeoCalculation::lambert_distance(nishio_lab, library);
			double expected = 447.24;

			double error = expected - actual;
			std::string log = "Error Distance is " + std::to_string(error) + "[m]\n";
			Logger::WriteMessage(log.c_str());
			Assert::AreEqual(expected, actual, DISTANCE_PRECISION * expected);
		}

		//西尾研から梅田駅までの距離
		TEST_METHOD(GeoCalculation_lambert_distance2)
		{
			LatLng nishio_lab(34.818292, 135.522105);
			LatLng hankyu_umeda(34.705523, 135.498301);
			double actual = GeoCalculation::lambert_distance(nishio_lab, hankyu_umeda);
			double expected = 12720.0;

			double error = expected - actual;
			std::string log = "Error Distance is " + std::to_string(error) + "[m]\n";
			Logger::WriteMessage(log.c_str());
			Assert::AreEqual(expected, actual, DISTANCE_PRECISION * expected);
		}
		
		//西尾研から理工学図書館までの距離(m)
		TEST_METHOD(GeoCalculation_haversine_distance)
		{
			LatLng nishio_lab(34.818292, 135.522105);
			LatLng library(34.822295, 135.522794);
			double actual = GeoCalculation::haversine_distance(nishio_lab, library);
			double expected = 447.24;

			double error = expected - actual;
			std::string log = "Error Distance is " + std::to_string(error) + "[m]\n";
			Logger::WriteMessage(log.c_str());
			Assert::AreEqual(expected, actual, DISTANCE_PRECISION * expected);
		}

		//西尾研から梅田駅までの距離
		TEST_METHOD(GeoCalculation_haversine_distance2)
		{
			LatLng nishio_lab(34.818292, 135.522105);
			LatLng hankyu_umeda(34.705523, 135.498301);
			double actual = GeoCalculation::haversine_distance(nishio_lab, hankyu_umeda);
			double expected = 12720.0;

			double error = expected - actual;
			std::string log = "Error Distance is " + std::to_string(error) + "[m]\n";
			Logger::WriteMessage(log.c_str());
			Assert::AreEqual(expected, actual, DISTANCE_PRECISION * expected);
		}


		//西尾研から見た理工学図書館への方位角
		//国土地理院のサイト(http://vldb.gsi.go.jp/sokuchi/surveycalc/surveycalc/bl2stf.html)のが正しい角度としてテストしています
		TEST_METHOD(GeoCalculation_lambert_azimuth_angle)
		{
			LatLng nishio_lab(34.818292, 135.522105);
			LatLng library(34.822295, 135.522794);
			double actual = Math::AngleUtility::convert_to_degree(GeoCalculation::lambert_azimuth_angle(nishio_lab, library)); 
			double expected = 278.078125;

			double error = expected - actual;
			std::string log = "Error angle is " + std::to_string(error) + "[°]\n";
			Logger::WriteMessage(log.c_str());
			Assert::AreEqual(expected, actual, ANGLE_PRECISION);
		}

		//西尾研から見た理工学図書館への方位角
		//国土地理院のサイト(http://vldb.gsi.go.jp/sokuchi/surveycalc/surveycalc/bl2stf.html)のが正しい角度としてテストしています
		TEST_METHOD(GeoCalculation_lambert_azimuth_angle2)
		{
			LatLng nishio_lab(34.818292, 135.522105);
			LatLng hankyu_umeda(34.705523, 135.498301);
			double actual = Math::AngleUtility::convert_to_degree(GeoCalculation::lambert_azimuth_angle(nishio_lab, hankyu_umeda));
			double expected = 99.8886777;

			double error = expected - actual;
			std::string log = "Error angle is " + std::to_string(error) + "[°]\n";
			Logger::WriteMessage(log.c_str());
			Assert::AreEqual(expected, actual, ANGLE_PRECISION);
		}

		//西尾研から見たグランドの方位角
		TEST_METHOD(GeoCalculation_lambert_azimuth_angle3)
		{
			LatLng nishio_lab(34.818292, 135.522105);
			LatLng ground(34.816566, 135.523197);
			double actual = Math::AngleUtility::convert_to_degree(GeoCalculation::lambert_azimuth_angle(nishio_lab, ground));
			double expected = 62.4454194;

			double error = expected - actual;
			std::string log = "Error angle is " + std::to_string(error) + "[°]\n";
			Logger::WriteMessage(log.c_str());
			Assert::AreEqual(expected, actual, ANGLE_PRECISION);
		}

		TEST_METHOD(GeoCalculation_lambert_azimuth_angle4) 
		{
			LatLng nishio_lab(34.818292, 135.522105);
			LatLng dental_hospital (34.819728, 135.520848);
			double actual = Math::AngleUtility::convert_to_degree(GeoCalculation::lambert_azimuth_angle(nishio_lab, dental_hospital));
			double expected = 234.17522;

			double error = expected - actual;
			std::string log = "Error angle is " + std::to_string(error) + "[°]\n";
			Logger::WriteMessage(log.c_str());
			Assert::AreEqual(expected, actual, ANGLE_PRECISION);
		}
	};
}
