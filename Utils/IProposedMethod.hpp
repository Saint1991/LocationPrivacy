
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
		dummies = std::make_unique<std::vector<DUMMY_TYPE>>(dummy_num);
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename MAP_TYPE, typename USER_TYPE, typename DUMMY_TYPE, typename REQUIREMENT_TYPE>
	IProposedMethod<MAP_TYPE, USER_TYPE, DUMMY_TYPE, REQUIREMENT_TYPE>::~IProposedMethod()
	{
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
		time_manager->for_each_time([](time_t time, long interval, int phase) {
			for_each_phase(time, interval, phase);
		});

		//�����Ōv�����I��
		timer->end();

		//���s���ԈȊO�̃G�N�X�|�[�g
		export_results();

		//�I������
		terminate();
	}
}

