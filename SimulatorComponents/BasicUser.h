#ifdef SIMULATORCOMPONENTS_EXPORTS
#define BASIC_USER_API __declspec(dllexport)
#else
#define BASIC_USER_API __declspec(dllimport)
#endif

#pragma once
#include "Coordinate.h"
#include "LatLng.h"
#include "Dummy.h"
#include "PreferenceTree.h"

namespace User
{

	///<summary>
	/// ���[�U��\���N���X
	/// �n�D�̖؂͍X�V���K�X�K�v�Ȃ̂�public�ɂ��Ă���
	///</summary>
	template <typename POSITION_TYPE = Geography::LatLng>
	class BASIC_USER_API BasicUser : public Entity::Dummy<POSITION_TYPE>
	{
	public:
		const std::shared_ptr<PreferenceTree> preference;

		BasicUser(std::shared_ptr<Time::TimeSlotManager const> timeslot, std::shared_ptr<PreferenceTree> preference_tree, Entity::entity_id id = 0);
		~BasicUser();
	};

	template class BasicUser<Graph::Coordinate>;
	template class BasicUser<Geography::LatLng>;
}


