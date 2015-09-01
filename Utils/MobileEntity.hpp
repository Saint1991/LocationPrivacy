
namespace Entity
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename POSITION_TYPE, typename STATE_TYPE>
	MobileEntity<POSITION_TYPE, STATE_TYPE>::MobileEntity(entity_id id) : Identifiable<entity_id>(id), past_states(std::make_shared<std::vector<std::shared_ptr<STATE_TYPE>>>())
	{
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename POSITION_TYPE, typename STATE_TYPE>
	MobileEntity<POSITION_TYPE, STATE_TYPE>::~MobileEntity()
	{
	}


	///<summary>
	/// �^����ꂽstate��ۑ�����
	///</summary>
	template <typename POSITION_TYPE, typename STATE_TYPE>
	void MobileEntity<POSITION_TYPE, STATE_TYPE>::save_state(std::shared_ptr<STATE_TYPE> state)
	{
		past_states->push_back(state);
	}

	///<summary>
	/// ���݈ʒu��Ԃ��܂�
	/// ���݂��Ȃ��ꍇ��nullptr���Ԃ�܂�
	///</summary>
	template <typename POSITION_TYPE, typename STATE_TYPE>
	const std::shared_ptr<POSITION_TYPE const> MobileEntity<POSITION_TYPE, STATE_TYPE>::current_position() const
	{
		if (past_states->size() == 0) return nullptr;
		return past_states->back()->read_position();
	}


	///<summary>
	/// �ߋ�count�X�e�b�v�O����̈ʒu��Ԃ�
	///</summary>
	template <typename POSITION_TYPE, typename STATE_TYPE>
	const std::vector<std::shared_ptr<POSITION_TYPE const>> MobileEntity<POSITION_TYPE, STATE_TYPE>::previous_positions(unsigned int count) const
	{
		std::vector<std::shared_ptr<POSITION_TYPE const>> ret(count);

		unsigned int index = 0;
		for (std::vector<std::shared_ptr<STATE_TYPE const>>::const_iterator iter = past_states->end() - count; iter != past_states->end(); iter++) {
			ret.at(index++) = iter->read_position();
		}

		return ret;
	}
}

