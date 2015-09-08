
namespace Entity
{


	///<summary>
	/// �R���X�g���N�^
	/// ���[�U�̓����͓��͂Ƃ��ăR���X�g���N�^�ɓn��
	///</summary>
	template <typename STATE_TYPE>
	BasicUser<STATE_TYPE>::BasicUser(std::vector<std::shared_ptr<STATE_TYPE>> user_movement, entity_id id)
		: MobileEntity<Geography::LatLng, STATE_TYPE>(id), past_states(std::make_shared<std::vector<std::shared_ptr<STATE_TYPE>>>(user_movement))
	{
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename STATE_TYPE>
	BasicUser<STATE_TYPE>::~BasicUser()
	{
	}

}


