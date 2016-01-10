#ifdef SIMULATORCOMPONENTS_EXPORTS
#define USER_TRAJECTORY_CONVERTER_API __declspec(dllexport)
#else
#define USER_TRAJECTORY_CONVERTER_API __declspec(dllimport)
#endif
#pragma once
#include "stdafx.h"
#include "Trajectory.h"
#include "SemanticTrajectory.h"
#include "BasicDbMap.h"
namespace User
{

	///<summary>
	/// �g���W�F�N�g����ϊ����ďo�͂��邽�߂̃N���X
	/// TRAJECTORY_TYPE�Ɏw��ł���̂�Trajectory<LatLng>�̔h���N���X�̂�
	/// map�ɂ�BasicDbMap�̔h���N���X�Ȃ牽�ł��g����
	///</summary>
	class USER_TRAJECTORY_CONVERTER_API UserTrajectoryConverter
	{
	protected:
		std::shared_ptr<Map::BasicDbMap const> map;
		std::pair<int, int> get_phase_range_of_longest_walking_trajectory(std::shared_ptr<Graph::Trajectory<Geography::LatLng> const> original_trajectory, double max_speed) const;

	public:
		UserTrajectoryConverter(std::shared_ptr<Map::BasicDbMap const> map);
		~UserTrajectoryConverter();

		std::shared_ptr<Graph::Trajectory<Geography::LatLng>> extract_walking_trajectory(std::shared_ptr<Graph::Trajectory<Geography::LatLng> const> original_trajectory, size_t min_trajectory_length, double max_speed) const;
		std::shared_ptr<Graph::SemanticTrajectory<Geography::LatLng>> extract_walking_semantic_trajectory(std::shared_ptr<Graph::SemanticTrajectory<Geography::LatLng> const> original_trajectory, size_t min_trajectory_length, double max_speed) const;
		std::shared_ptr<Graph::SemanticTrajectory<Geography::LatLng>> convert_to_walking_semantic_trajectory(std::shared_ptr<Graph::SemanticTrajectory<Geography::LatLng> const> original_trajectory, double max_speed) const;
	};

}



