#pragma once
#include "stdafx.h"
#include "BasicRequirement.h"
#include "LatLng.h"
#include "IProposedMethod.h"

namespace Simulation
{

	///<summary>
	/// Simulator�̐��`
	/// ����ɍ��킹��Simulator���쐬����
	/// ���͂Ƃ��Ēn�}�CUser�̍s���C�v���p�����[�^��3��z�肵�č쐬���Ă��܂��D
	///</summary>
	template <typename MAP_TYPE, typename USER_TYPE, typename DUMMY_TYPE, typename REQUIREMENT_TYPE, typename POSITION_TYPE = Geography::LatLng>
	class ISimulator
	{
	
	static_assert(std::is_base_of<Requirement::BasicRequirement, REQUIREMENT_TYPE>::value, "template type REQUIREMENT_TYPE must be derived from BasicRequirement");
	
	protected:
		std::shared_ptr<MAP_TYPE> map;
		std::shared_ptr<USER_TYPE> user;
		std::list<std::shared_ptr<REQUIREMENT_TYPE const>> requirements;
		std::list<std::shared_ptr<Framework::IProposedMethod<MAP_TYPE, USER_TYPE, DUMMY_TYPE, REQUIREMENT_TYPE, POSITION_TYPE>>> comparative_methods;
		std::shared_ptr<Time::TimeSlotManager> time_manager;

	public:
		ISimulator() {}
		virtual ~ISimulator() {}

		virtual void build_map() = 0;
		virtual void create_user(unsigned int user_id) = 0;
		virtual void set_comparative_methods() = 0;
		virtual void make_requirement_list() = 0;
		virtual void run() = 0;
	};
}


