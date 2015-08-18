#ifdef UTILS_EXPORTS
#define BASIC_PATH_DATA_API __declspec(dllexport)
#else
#define BASIC_PATH_DATA_API __declspec(dllimport)
#endif

#pragma once
#include "Coordinate.h"
#include "LatLng.h"

namespace Graph
{


	///<summary>
	/// ���������f�[�^
	/// �g���R���X�g���N�^�ɂ���Ē������W���ܓx�o�x�����g�������ċ����v�Z�ł���
	///</summary>
	class BASIC_PATH_DATA_API BasicPathData
	{
	protected:
		double distance;

	public:
		BasicPathData(const double distance);
		BasicPathData(const Coordinate& c1, const Coordinate& c2);
		BasicPathData(const Geography::LatLng& ll1, const Geography::LatLng& ll2);
		~BasicPathData();
		
		const double get_distance() const;
		void update_distance(double distance);
	};
}


