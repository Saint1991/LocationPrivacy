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

		//�����̋��e�덷(��Βl�łȂ������̃I�[�_�[�ɑ΂��ċ��e�덷�����߂�)
		//���Ȃ킿���L��expected��100000m�Ȃ�1000m�܂Ō덷�����e����Ƃ����Ӗ�
		// (�傫���悤�ɂ������邪�CGoogle Map�̋������������������Ƃ͌���Ȃ��Ƃ������CGoogle Map��haversine�̕����x�[�X�ɂ���Ă���ۂ�)
		const double DISTANCE_PRECISION = 1.0E-2;

		//�p�x�̋��e�덷[��]
		const double ANGLE_PRECISION = 0.1;

		//���������痝�H�w�}���ق܂ł̋���(m)
		//Google Map�̋����v�Z���g���ē����鋗���������������Ƃ��ăe�X�g���Ă܂�
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

		//����������~�c�w�܂ł̋���
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
		
		//���������痝�H�w�}���ق܂ł̋���(m)
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

		//����������~�c�w�܂ł̋���
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


		//���������猩�����H�w�}���قւ̕��ʊp
		//���y�n���@�̃T�C�g(http://vldb.gsi.go.jp/sokuchi/surveycalc/surveycalc/bl2stf.html)�̂��������p�x�Ƃ��ăe�X�g���Ă��܂�
		TEST_METHOD(GeoCalculation_lambert_azimuth_angle)
		{
			LatLng nishio_lab(34.818292, 135.522105);
			LatLng library(34.822295, 135.522794);
			double actual = Math::AngleUtility::convert_to_degree(GeoCalculation::lambert_azimuth_angle(nishio_lab, library)); 
			double expected = 278.078125;

			double error = expected - actual;
			std::string log = "Error angle is " + std::to_string(error) + "[��]\n";
			Logger::WriteMessage(log.c_str());
			Assert::AreEqual(expected, actual, ANGLE_PRECISION);
		}

		//���������猩�����H�w�}���قւ̕��ʊp
		//���y�n���@�̃T�C�g(http://vldb.gsi.go.jp/sokuchi/surveycalc/surveycalc/bl2stf.html)�̂��������p�x�Ƃ��ăe�X�g���Ă��܂�
		TEST_METHOD(GeoCalculation_lambert_azimuth_angle2)
		{
			LatLng nishio_lab(34.818292, 135.522105);
			LatLng hankyu_umeda(34.705523, 135.498301);
			double actual = Math::AngleUtility::convert_to_degree(GeoCalculation::lambert_azimuth_angle(nishio_lab, hankyu_umeda));
			double expected = 99.8886777;

			double error = expected - actual;
			std::string log = "Error angle is " + std::to_string(error) + "[��]\n";
			Logger::WriteMessage(log.c_str());
			Assert::AreEqual(expected, actual, ANGLE_PRECISION);
		}

		//���������猩���O�����h�̕��ʊp
		TEST_METHOD(GeoCalculation_lambert_azimuth_angle3)
		{
			LatLng nishio_lab(34.818292, 135.522105);
			LatLng ground(34.816566, 135.523197);
			double actual = Math::AngleUtility::convert_to_degree(GeoCalculation::lambert_azimuth_angle(nishio_lab, ground));
			double expected = 62.4454194;

			double error = expected - actual;
			std::string log = "Error angle is " + std::to_string(error) + "[��]\n";
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
			std::string log = "Error angle is " + std::to_string(error) + "[��]\n";
			Logger::WriteMessage(log.c_str());
			Assert::AreEqual(expected, actual, ANGLE_PRECISION);
		}
	};
}