
///<summary>
/// �R���X�g���N�^
///</summary>
template <typename GEONODEDATA, typename PATHDATA>
Geography::Map<GEONODEDATA, PATHDATA>::Map(std::unique_ptr<Graph::RoutingMethod<GEONODEDATA, PATHDATA>> routing_method) 
	: node_collection(std::make_shared<const Collection::IdentifiableCollection<std::shared_ptr<const Geography::GeoNode<GEONODEDATA, PATHDATA>>>>())
{
	build_map();
	routing_table = create_routing_table(routing_method);
}

///<summary>
/// �f�X�g���N�^
///</summary>
template <typename GEONODEDATA, typename PATHDATA>
Geography::Map<GEONODEDATA, PATHDATA>::~Map()
{
}


///<summary>
/// �ŒZ�H�̃��[�e�B���O�e�[�u�����\�z���܂��D
///</summary>
template <typename GEONODEDATA, typename PATHDATA>
void Geography::Map<GEONODEDATA, PATHDATA>::create_routing_table(std::unique_ptr<Graph::RoutingMethod<GEONODEDATA, PATHDATA>> routing_method)
{
	routing_table = std::move(routing_method->create_routing_table(node_collection));
}


///<summary>
/// from����to�܂ŕ��ϑ��xavg_speed�ňړ������ۂ̏��v���Ԃ��v�Z���܂�
/// ���B�s�\�ȏꍇ��NOWHERE��Ԃ��܂��D
///</summary>
template <typename GEONODEDATA, typename PATHDATA>
const unsigned long Geography::Map<GEONODEDATA, PATHDATA>::calc_necessary_time(const Graph::node_id& source, const Graph::node_id& destination, const double& avg_speed) const
{

	std::list<Graph::node_id> shortest_path = get_shortest_path();
	if (shortest_path == nullptr) return NOWHERE;

	Graph::node_id from = source;
	Graph::node_id to;
	unsigned long necessary_time = 0UL;
	for (std::list<Graph::node_id>::iterator iter = shortest_path.begin(); iter != shortest_path.end(); iter++) {

		//���[�e�B���O���X�g���玟�̃m�[�h��ID���擾
		to = *iter;

		//node�R���N�V��������from�ɊY������m�[�h���擾
		std::shared_ptr<const Geography::GeoNode<GEONODEDATA, PATHDATA>> target = node_collection->read_by_id(from);
		if (target == nullptr) return NOWHERE;

		//node������to�ւ̃G�b�W���擾
		std::shared_ptr<Graph::Edge<PATHDATA>> path = target->get_edge_to(to);
		if (path == nullptr) return NOWHERE;

		//edge�̋����𒲂ׂ� (GeoNode��PATHDATA��BasicPathData�̔h���N���X�ɍi���Ă���̂ŁC���̃L���X�g�͈��S�Ȃ͂�)
		std::shared_ptr<const Geography::BasicPathData> path_data = std::static_pointer_cast<Geography::BasicPathData>(path);
		double distance = path_data->get_distance();

		//���p���Ԃɉ��Z
		necessary_time += distance / avg_speed;

		//�T����i�߂�
		from = to;
	}

	return necessary_time;
}

///<summary>
/// from����to�֕��ϑ��xspeed�ňړ������Ƃ��̐�������time_limit[��]�̓��B�\���𒲂ׂ�
///</summary>
template <typename GEONODEDATA, typename PATHDATA>
const bool Geography::Map<GEONODEDATA, PATHDATA>::is_reachable(const Graph::node_id& from, const Graph::node_id& to, const double& speed, const unsigned int& time_limit) const
{
	return calc_necessary_time(from, to, speed) <= time_limit;
}


///<summary>
/// from ���� to�֍ŒZ�H�ōs���Ƃ��Ɏ��ɐi�ނׂ��m�[�h��ID���擾
///</summary>
template <typename GEONODEDATA, typename PATHDATA>
const Graph::node_id Geography::Map<GEONODEDATA, PATHDATA>::get_next_node_of_shortest_path(const Graph::node_id& from, const Graph::node_id& to) const
{
	return routing_table->at(from)->at(to);
}


///<summary>
/// node_id�̌n��Ōo�H��Ԃ��D
/// NOWHERE�ŒT�����I�������ꍇ��nullptr��Ԃ�
/// source == destination�̏ꍇ�͋�̃��X�g���Ԃ���܂��D
///</summary>
template <typename GEONODEDATA, typename PATHDATA>
const std::list<Graph::node_id> Geography::Map<GEONODEDATA, PATHDATA>::get_shortest_path(const Graph::node_id& source, const Graph::node_id& destination) const
{
	std::list<Graph::node_id> path;
	if (source == destination) return path;

	Graph::node_id iter = source;
	do {
		iter = get_next_node_of_shortest_path(iter, destination);
		if (iter == NOWHERE) break;
		path.push_back(iter);
	} while (iter != destination && iter != NOWHERE)

		//NOWHERE�ŏI�������ꍇ�͌o�H�Ȃ��Ƃ���nullptr��Ԃ�
		if (path.back() != destination) return nullptr;
	return path;
}


///<summary>
/// �w�肵��ID�̃m�[�h�ɐڑ����Ă��邷�ׂẴm�[�h��Ԃ�
///</summary>
template <typename GEONODEDATA, typename PATHDATA>
const std::list<Graph::node_id> Geography::Map<GEONODEDATA, PATHDATA>::get_connecting_nodes(const Graph::node_id& id) const
{
	std::shared_ptr<Geography::GeoNode<GEONODEDATA, PATHDATA> const> node = node_collection->read_by_id(id);
	return node->get_connecting_node_list();
}


///<summary>
/// �w�肵��ID�̃m�[�h��ύX�s�̏�ԂŎ擾����
///</summary>
template <typename GEONODEDATA, typename PATHDATA>
std::shared_ptr<const Geography::GeoNode<GEONODEDATA, PATHDATA>> Geography::Map<GEONODEDATA, PATHDATA>::get_static_node(const Graph::node_id& id) const
{
	return node_collection->read_by_id(id);
}


///<summary>
/// �w�肵��ID�̃m�[�h���擾����
///</summary>
template <typename GEONODEDATA, typename PATHDATA>
std::shared_ptr<Geography::GeoNode<GEONODEDATA, PATHDATA>> Geography::Map<GEONODEDATA, PATHDATA>::get_node(const Graph::node_id& id)
{
	return node_collection->get_by_id(id);
}
