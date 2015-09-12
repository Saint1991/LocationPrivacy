
namespace Graph
{
	
	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	Map<NODE, POI, PATH>::Map()
		: node_collection(std::make_shared<const Collection::IdentifiableCollection<Graph::node_id, NODE>>())
	{		
	}

	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	Map<NODE, POI, PATH>::~Map()
	{
	}


	///<summary>
	/// Map�̏�����
	/// �ŏ��ɕK��������ĂԕK�v�����邱�Ƃɒ���
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	void Map<NODE, POI, PATH>::initialize(std::unique_ptr<RoutingMethod<NODE, PATH>> routing_method)
	{
		build_map();
		routing_table = routing_method->create_routing_table(node_collection);
	}

	///<summary>
	/// from ���� to�֍ŒZ�H�ōs���Ƃ��Ɏ��ɐi�ނׂ��m�[�h��ID���擾
	/// �ڑ����Ă��Ȃ��ꍇ��NOWHERE���Ԃ�
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	node_id Map<NODE, POI, PATH>::get_next_node_of_shortest_path(const node_id& from, const node_id& to) const
	{
		return routing_table->get_next_node_of_shortest_path(from, to);
	}


	///<summary>
	/// node_id�̌n��Ōo�H��Ԃ��D
	/// ���B�s�\�ȏꍇ��nullptr��Ԃ�
	/// source == destination�̏ꍇ�͋�̃��X�g���Ԃ���܂��D
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	const std::list<node_id> Map<NODE, POI, PATH>::get_shortest_path(const node_id& source, const node_id& destination) const
	{
		return routing_table->get_shortest_path(source, destination);
	}



	///<summary>
	/// from����to�܂ŕ��ϑ��xavg_speed[m/s]�ňړ������ۂ̏��v���Ԃ��v�Z���܂�
	/// ���B�s�\�ȏꍇ��NO_CONNECTION��Ԃ��܂��D
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	double Map<NODE, POI, PATH>::calc_necessary_time(const node_id& source, const node_id& destination, const double& avg_speed) const
	{
		double shortest_distance = routing_table->shortest_distance(source, destination);
		return shortest_distance / avg_speed;
	}

	///<summary>
	/// from����to�֕��ϑ��xspeed[m/s]�ňړ������Ƃ��̐�������time_limit[s]�̓��B�\���𒲂ׂ�
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	bool Map<NODE, POI, PATH>::is_reachable(const node_id& from, const node_id& to, const double& speed, const double& time_limit) const
	{
		return calc_necessary_time(from, to, speed) <= time_limit;
	}


	///<summary>
	/// �w�肵��ID�̃m�[�h�ɐڑ����Ă��邷�ׂẴm�[�h��Ԃ�
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	const std::list<node_id> Map<NODE, POI, PATH>::get_connecting_nodes(const node_id& id) const
	{
		std::shared_ptr<NODE const> node = node_collection->read_by_id(id);
		return node->get_connecting_node_list();
	}


	///<summary>
	/// �w�肵��ID�̃m�[�h��ύX�s�̏�ԂŎ擾����
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	std::shared_ptr<NODE const> Map<NODE, POI, PATH>::get_static_node(const node_id& id) const
	{
		return node_collection->read_by_id(id);
	}
}

