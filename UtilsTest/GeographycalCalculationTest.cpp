#include "stdafx.h"
#include "CppUnitTest.h"
#include "GeographicalCalculation.h"
#include "AngleUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilsTest
{

	///<summary>
	/// �����n���̌v�Z��http://japonyol.net/editor/distancia.html�ŎZ�o�ł��鋗����actual�Ƃ��ăe�X�g�������Ă܂�
	/// �e��v�Z���@�ł���Ƃ͓��R�덷���o��̂�precision�͒l���݂C
	/// �܂����ꂭ�炢�Ȃ�Ó����낤(�v�Z�I�ɂ͊Ԉ���Ă��Ȃ����낤)�Ƃ����͈͂�ݒ肵�Ă��܂��E
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