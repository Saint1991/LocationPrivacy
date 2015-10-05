#include "stdafx.h"
#include "EntityManager.h"

namespace Entity
{


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	EntityManager<DUMMY, USER, POSITION_TYPE>::EntityManager(std::shared_ptr<USER> user, int num_of_dummy, std::shared_ptr<Time::TimeSlotManager> timeslot) 
		: timeslot(timeslot), dummies(std::make_shared<std::vector<std::shared_ptr<DUMMY>>>(num_of_dummy))
	{
		for (int id = 1; id <= num_of_dummy; id++) {
			dummies->at(id) = std::make_shared<DUMMY>(id, timeslot);
		}
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	EntityManager<DUMMY, USER, POSITION_TYPE>::~EntityManager()
	{
	}


	///<summary>
	/// �_�~�[�𐶐�����
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	entity_id EntityManager<DUMMY, USER, POSITION_TYPE>::create_dummy()
	{
		entity_id new_id = dummies->size() + 1;
		dummies->push_back(std::make_shared<DUMMY>(new_id, timeslot));
		return new_id;
	}


	///<summary>
	/// 2�G���e�B�e�B�Ԃ̋��L�n�_��ݒ肷��
	/// ������ĂԂ��тɊe�_�~�[�̌����񐔂�+1�����̂Œ���
	/// �����ݒ�ɕ����_�̐ݒ肪�K�v�ȏꍇ�͈�x����������Ăяo���āC���Ƃ�set_position�̕�����������
	/// �_�~�[ID��ID=1����_�~�[�����̘A�ԂɂȂ��Ă���̂�z�肵�Ă���
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	void EntityManager<DUMMY, USER, POSITION_TYPE>::set_crossing_point(entity_id id1, entity_id id2, POSITION_TYPE position1, POSITION_TYPE position2, int phase)
	{
		std::shared_ptr<DUMMY> dummy1 = dummies->at(id1 - 1);
		std::shared_ptr<DUMMY> dummy2 = dummies->at(id2 - 1);
	}



}


