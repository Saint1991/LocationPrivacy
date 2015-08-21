
namespace Graph
{
	///<summary>
	/// 2�̃m�[�h�Ԃ̋������擾����
	/// from == to�̏ꍇ��0���Ԃ�C�ڑ����Ă��Ȃ��ꍇ��NO_CONNECTION���Ԃ�
	/// NODE�ɂ�BasicPathData�̔h���N���X���G�b�W�Ɏ���Node�N���X���p�������N���X�������ĂȂ��̂Œ���
	///</summary>
	template <typename NODE>
	double RoutingMethod<NODE>::distance(const node_id& from, const node_id& to) 
	{
		if (from == to) return 0;

		std::shared_ptr<NODE const> node = node_collection->read_by_id(from);
		if (node == nullptr) return NO_CONNECTION;

		auto edge = node->get_static_edge_to(to);
		if (edge == nullptr) return NO_CONNECTION;

		auto path_data = path->get_static_data();
		if (path_data == nullptr) return NO_CONNECTIOIN;

		return path_data->get_distance();
	}

	template <typename NODE>
	void create_distance_map(std::shared_ptr<const Collection::IdentifiableCollection<std::shared_ptr<NODE const>>> node_collectioin)
	{
		distance_map = std::make_unique<std::vector<std::vector<double>>>();
	}

	///<summary>
	/// node_collection��ݒ肷��
	///</summary>
	template <typename NODE>
	void RoutingMethod<NODE>::set_node_collection(std::shared_ptr<const Collection::IdentifiableCollection<std::shared_ptr<NODE const>> nodes)
	{
		node_collection = nodes;
		create_distance_map();
	}
}
