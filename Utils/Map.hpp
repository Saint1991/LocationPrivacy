
namespace Graph
{
	
	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename NODE, typename PATH>
	Map<NODE, PATH>::Map(std::unique_ptr<RoutingMethod<NODE>> routing_method)
		: node_collection(std::make_shared<const Collection::IdentifiableCollection<std::shared_ptr<NODE>>>())
	{
		build_map();
		routing_table = routing_method->create_routing_table(node_collection);
	}

	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename NODE, typename PATH>
	Map<NODE, PATH>::~Map()
	{
	}

	///<summary>
	/// from ���� to�֍ŒZ�H�ōs���Ƃ��Ɏ��ɐi�ނׂ��m�[�h��ID���擾
	///</summary>
	template <typename NODE, typename PATH>
	const node_id Map<NODE, PATH>::get_next_node_of_shortest_path(const node_id& from, const node_id& to) const
	{
		return routing_table->at(from)->at(to);
	}


	///<summary>
	/// node_id�̌n��Ōo�H��Ԃ��D
	/// ���B�s�\�ȏꍇ��nullptr��Ԃ�
	/// source == destination�̏ꍇ�͋�̃��X�g���Ԃ���܂��D
	///</summary>
	template <typename NODE, typename PATH>
	const std::list<node_id> Map<NODE, PATH>::get_shortest_path(const node_id& source, const node_id& destination) const
	{
		std::list<node_id> path;
		if (source == destination) return path;

		node_id iter = source;
		while (iter != destination && iter != NOWHERE) {
			iter = get_next_node_of_shortest_path(iter, destination);
			path.push_back(iter);
		} 

		//path�̍Ō����destination�ɂȂ��Ă��Ȃ��ꍇ�͌o�H���������Ă��Ȃ��̂�nullptr��Ԃ�
		if (path.back() != destination) return nullptr;
		return path;
	}



	///<summary>
	/// from����to�܂ŕ��ϑ��xavg_speed[m/s]�ňړ������ۂ̏��v���Ԃ��v�Z���܂�
	/// ���B�s�\�ȏꍇ��NOWHERE��Ԃ��܂��D
	///</summary>
	template <typename NODE, typename PATH>
	const unsigned long Map<NODE, PATH>::calc_necessary_time(const node_id& source, const node_id& destination, const double& avg_speed) const
	{

		const std::list<node_id> shortest_path = get_shortest_path();
		if (shortest_path == nullptr) return NOWHERE;

		node_id from = source;
		node_id to;
		unsigned long necessary_time = 0UL;
		for (std::list<node_id>::const_iterator iter = shortest_path.begin(); iter != shortest_path.end(); iter++) {

			//���[�e�B���O���X�g���玟�̃m�[�h��ID���擾
			to = *iter;

			//node�R���N�V��������from�ɊY������m�[�h���擾
			std::shared_ptr<NODE const> target = node_collection->read_by_id(from);
			if (target == nullptr) return NOWHERE;

			//node������to�ւ̃G�b�W���擾
			std::shared_ptr<PATH> path = target->get_edge_to(to);
			if (path == nullptr) return NOWHERE;

			//edge�̋����𒲂ׂ�
			auto path_data = path->get_static_data();
			if (path_data == nullptr) return NOWHERE;
			double distance = path_data->get_distance();

			//���p���Ԃɉ��Z
			necessary_time += distance / avg_speed;

			//�T����i�߂�
			from = to;
		}

		return necessary_time;
	}

	///<summary>
	/// from����to�֕��ϑ��xspeed[m/s]�ňړ������Ƃ��̐�������time_limit[s]�̓��B�\���𒲂ׂ�
	///</summary>
	template <typename NODE, typename PATH>
	const bool Map<NODE, PATH>::is_reachable(const node_id& from, const node_id& to, const double& speed, const unsigned int& time_limit) const
	{
		return calc_necessary_time(from, to, speed) <= time_limit;
	}


	///<summary>
	/// �w�肵��ID�̃m�[�h�ɐڑ����Ă��邷�ׂẴm�[�h��Ԃ�
	///</summary>
	template <typename NODE, typename PATH>
	const std::list<node_id> Map<NODE, PATH>::get_connecting_nodes(const node_id& id) const
	{
		std::shared_ptr<NODE const> node = node_collection->read_by_id(id);
		return node->get_connecting_node_list();
	}


	///<summary>
	/// �w�肵��ID�̃m�[�h��ύX�s�̏�ԂŎ擾����
	///</summary>
	template <typename NODE, typename PATH>
	std::shared_ptr<NODE const> Map<NODE, PATH>::get_static_node(const node_id& id) const
	{
		return node_collection->read_by_id(id);
	}


	///<summary>
	/// �w�肵��ID�̃m�[�h���擾����
	///</summary>
	template <typename NODE, typename PATH>
	std::shared_ptr<NODE> Map<NODE, PATH>::get_node(const node_id& id)
	{
		return node_collection->get_by_id(id);
	}
}

