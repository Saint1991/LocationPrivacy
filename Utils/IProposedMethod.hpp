
namespace Framework
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename MAP_TYPE, typename USER_TYPE, typename DUMMY_TYPE, typename REQUIREMENT_TYPE, typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	IProposedMethod<MAP_TYPE, USER_TYPE, DUMMY_TYPE, REQUIREMENT_TYPE, POSITION_TYPE, TRAJECTORY_TYPE>::IProposedMethod (
		std::shared_ptr<MAP_TYPE const> map, std::shared_ptr<USER_TYPE> user, std::shared_ptr<REQUIREMENT_TYPE const> requirement, std::shared_ptr<Time::TimeSlotManager const> time_manager
	) : map(map), requirement(requirement),timer(std::make_shared<Time::Timer>()), time_manager(time_manager)
	{
		 size_t dummy_num = this->requirement->dummy_num;
		entities = std::make_shared<Entity::EntityManager<POSITION_TYPE, TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>>(user, dummy_num, time_manager);
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename MAP_TYPE, typename USER_TYPE, typename DUMMY_TYPE, typename REQUIREMENT_TYPE, typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	IProposedMethod<MAP_TYPE, USER_TYPE, DUMMY_TYPE, REQUIREMENT_TYPE, POSITION_TYPE, TRAJECTORY_TYPE>::~IProposedMethod()
	{
	}

	///<summary>
	/// ProposedMethod�̎��s
	///</summary>
	template <typename MAP_TYPE, typename USER_TYPE, typename DUMMY_TYPE, typename REQUIREMENT_TYPE, typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void IProposedMethod<MAP_TYPE, USER_TYPE, DUMMY_TYPE, REQUIREMENT_TYPE, POSITION_TYPE, TRAJECTORY_TYPE>::run() 
	{

		//�����Ŏ��s���Ԃ̌v�����J�n
		timer->start();

		//������
		initialize();

		//���������s����(�e�����̃_�~�[�ʒu���v�Z����)
		decide_dummy_positions();

		//�����Ōv�����I��
		timer->end();

		//�I������
		terminate();
	}


	///<summary>
	/// ��@�̎��s�I�����̃R�[���o�b�N��o�^����
	///</summary>
	template <typename MAP_TYPE, typename USER_TYPE, typename DUMMY_TYPE, typename REQUIREMENT_TYPE, typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void IProposedMethod<MAP_TYPE, USER_TYPE, DUMMY_TYPE, REQUIREMENT_TYPE, POSITION_TYPE, TRAJECTORY_TYPE>::set_execution_callback(const std::function<void(std::shared_ptr<Entity::EntityManager<POSITION_TYPE, TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>>, std::shared_ptr<REQUIREMENT_TYPE const> requirement, std::shared_ptr<Time::Timer>)>& callback)
	{
		execution_callback = callback;
	}


	///<summary>
	/// �R�[���o�b�N�����s����
	///</summary>
	template <typename MAP_TYPE, typename USER_TYPE, typename DUMMY_TYPE, typename REQUIREMENT_TYPE, typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void IProposedMethod<MAP_TYPE, USER_TYPE, DUMMY_TYPE, REQUIREMENT_TYPE, POSITION_TYPE, TRAJECTORY_TYPE>::terminate()
	{
		if (execution_callback != nullptr) execution_callback(entities, requirement, timer);
		entities->get_user()->clear_cross_info();
	}

}

