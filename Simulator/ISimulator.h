#pragma once
#include "BasicUser.h"
#include "BasicRequirement.h"


namespace Simulation
{

	///<summary>
	/// Simulator�̐��`
	/// ����ɍ��킹��Simulator���쐬����
	/// ���͂Ƃ��Ēn�}�CUser�̍s���C�v���p�����[�^��3��z�肵�č쐬���Ă��܂��D
	///</summary>
	template <typename MAP_TYPE, typename USER_TYPE, typename REQUIREMENT_TYPE>
	class ISimulator
	{
	
	static_assert(std::is_base_of<Requirement::BasicRequirement, REQUIREMENT_TYPE>::value, "template type REQUIREMENT_TYPE must be derived from BasicRequirement");
	
	protected:
		std::shared_ptr<MAP_TYPE const> map;
		std::shared_ptr<USER_TYPE const> user;
		std::list<std::shared_ptr<REQUIREMENT_TYPE const>> requirements;

	public:
		ISimulator();
		virtual ~ISimulator();

		virtual void build_map() = 0;
		virtual void create_user() = 0;
		virtual void make_requirement_list() = 0;
		virtual void run() = 0;
	};
}


