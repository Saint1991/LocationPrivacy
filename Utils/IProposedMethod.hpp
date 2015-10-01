
namespace Framework
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename MAP_TYPE, typename USER_TYPE, typename DUMMY_TYPE, typename REQUIREMENT_TYPE>
	IProposedMethod<MAP_TYPE, USER_TYPE, DUMMY_TYPE, REQUIREMENT_TYPE>::IProposedMethod(std::shared_ptr<MAP_TYPE const> map, std::shared_ptr<USER_TYPE const> user, std::shared_ptr<REQUIREMENT_TYPE const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager)
		: map(map), user(user), requirement(requirement)
	{
		size_t dummy_num = this->requirement->dummy_num;
		dummies = std::make_shared<std::vector<DUMMY_TYPE>>(dummy_num);
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename MAP_TYPE, typename USER_TYPE, typename DUMMY_TYPE, typename REQUIREMENT_TYPE>
	IProposedMethod<MAP_TYPE, USER_TYPE, DUMMY_TYPE, REQUIREMENT_TYPE>::~IProposedMethod()
	{
	}


	///<summary>
	/// �e�_�~�[�ɂ���execute_function�����s���郆�[�e�B���e�B
	///</summary>
	template <typename MAP_TYPE, typename USER_TYPE, typename DUMMY_TYPE, typename REQUIREMENT_TYPE>
	void IProposedMethod<MAP_TYPE, USER_TYPE, DUMMY_TYPE, REQUIREMENT_TYPE>::for_each_dummy(const std::function<void(int, std::shared_ptr<DUMMY_TYPE>)>& execute_function)
	{
		for (std::vector<std::shared_ptr<DUMMY_TYPE>>::iterator iter = dummies->begin(); iter != dummies->end(); iter++) {
			int dummy_id = (*iter)->get_id();
			execute_function(dummy_id, *iter);
		}
	}

	///<summary>
	/// ProposedMethod�̎��s
	///</summary>
	template <typename MAP_TYPE, typename USER_TYPE, typename DUMMY_TYPE, typename REQUIREMENT_TYPE>
	void IProposedMethod<MAP_TYPE, USER_TYPE, DUMMY_TYPE, REQUIREMENT_TYPE>::run() 
	{

		//�����Ŏ��s���Ԃ̌v�����J�n
		timer->start();

		//������
		initialize();

		//���������s����(�e�����̃_�~�[�ʒu���v�Z����)
		decide_dummy_positions();

		//�����Ōv�����I��
		timer->end();

		//�ݒ肵���_�~�[�C���[�U�̈ʒu�����MTC�Ȃǂ̕]���w�W���v�Z����
		evaluate();

		//���s���ԈȊO�̃G�N�X�|�[�g
		export_results();

		//�I������
		terminate();
	}
}

