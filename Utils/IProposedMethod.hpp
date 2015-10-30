
namespace Framework
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename MAP_TYPE, typename USER_TYPE, typename DUMMY_TYPE, typename REQUIREMENT_TYPE, typename POSITION_TYPE>
	IProposedMethod<MAP_TYPE, USER_TYPE, DUMMY_TYPE, REQUIREMENT_TYPE, POSITION_TYPE>::IProposedMethod (
		std::shared_ptr<MAP_TYPE const> map, std::shared_ptr<USER_TYPE> user, std::shared_ptr<REQUIREMENT_TYPE const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager
	) : map(map), requirement(requirement)
	{
		size_t dummy_num = this->requirement->dummy_num;
		entities = std::make_shared<Entity::EntityManager<DUMMY_TYPE, USER_TYPE, POSITION_TYPE>>(user, dummy_num, time_manager);
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename MAP_TYPE, typename USER_TYPE, typename DUMMY_TYPE, typename REQUIREMENT_TYPE, typename POSITION_TYPE>
	IProposedMethod<MAP_TYPE, USER_TYPE, DUMMY_TYPE, REQUIREMENT_TYPE, POSITION_TYPE>::~IProposedMethod()
	{
	}

	///<summary>
	/// ProposedMethod�̎��s
	///</summary>
	template <typename MAP_TYPE, typename USER_TYPE, typename DUMMY_TYPE, typename REQUIREMENT_TYPE, typename POSITION_TYPE>
	void IProposedMethod<MAP_TYPE, USER_TYPE, DUMMY_TYPE, REQUIREMENT_TYPE, POSITION_TYPE>::run() 
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


	///<summary>
	/// ��@�̎��s�I�����̃R�[���o�b�N��o�^����
	///</summary>
	template <typename MAP_TYPE, typename USER_TYPE, typename DUMMY_TYPE, typename REQUIREMENT_TYPE, typename POSITION_TYPE>
	void IProposedMethod<MAP_TYPE, USER_TYPE, DUMMY_TYPE, REQUIREMENT_TYPE, POSITION_TYPE>::set_execution_callback(const std::function<void(std::shared_ptr<Entity::EntityManager<DUMMY_TYPE, USER_TYPE, POSITION_TYPE>>, Time::Timer)>& callback)
	{
		execution_callback = callback;
	}


	///<summary>
	/// �R�[���o�b�N��o�^����
	///</summary>
	template <typename MAP_TYPE, typename USER_TYPE, typename DUMMY_TYPE, typename REQUIREMENT_TYPE, typename POSITION_TYPE>
	void IProposedMethod<MAP_TYPE, USER_TYPE, DUMMY_TYPE, REQUIREMENT_TYPE, POSITION_TYPE>::terminate()
	{
		if (execution_callback != nullptr) execution_callback(entities, timer);
	}

}

