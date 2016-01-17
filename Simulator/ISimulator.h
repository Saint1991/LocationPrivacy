#pragma once
#include "stdafx.h"
#include "SemanticTrajectory.h"
#include "BasicRequirement.h"
#include "LatLng.h"
#include "IProposedMethod.h"
#include "Rectangle.h"

namespace Simulation
{

	///<summary>
	/// Simulator�̐��`
	/// ����ɍ��킹��Simulator���쐬����
	/// ���͂Ƃ��Ēn�}�CUser�̍s���C�v���p�����[�^��3��z�肵�č쐬���Ă��܂��D
	///</summary>
	template <typename MAP_TYPE, typename USER_TYPE, typename DUMMY_TYPE, typename REQUIREMENT_TYPE, typename POSITION_TYPE = Geography::LatLng, typename TRAJECTORY_TYPE = Graph::SemanticTrajectory<POSITION_TYPE>>
	class ISimulator
	{
	
	static_assert(std::is_base_of<Requirement::BasicRequirement, REQUIREMENT_TYPE>::value, "template type REQUIREMENT_TYPE must be derived from BasicRequirement");
	
	protected:
		std::string simulation_start_time;
		std::shared_ptr<MAP_TYPE> map;
		std::shared_ptr<std::vector<std::shared_ptr<TRAJECTORY_TYPE>>> user_trajectories;
		std::shared_ptr<USER_TYPE> user;
		std::list<std::shared_ptr<REQUIREMENT_TYPE const>> requirements;
		std::shared_ptr<Time::TimeSlotManager> time_manager; //��������Ȃ��Ȃ邩��

		virtual void build_map(const Graph::Rectangle<Geography::LatLng>& boundary) = 0;
		virtual void create_trajectories() = 0;
		virtual void make_requirement_list() = 0;

		virtual void export_evaluation_result(const REQUIREMENT_TYPE& requirement) = 0;

	public:
		ISimulator() : user_trajectories(std::make_shared<std::vector<std::shared_ptr<TRAJECTORY_TYPE>>>()) {}
		virtual ~ISimulator() {}

		virtual void prepare() = 0;
		virtual void run() = 0;
	};
}


