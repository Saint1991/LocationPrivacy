
namespace Graph
{

	template <typename NODE, typename EDGE>
	Dijkstra<NODE, EDGE>::Dijkstra()
	{
	}

	template <typename NODE, typename EDGE>
	Dijkstra<NODE, EDGE>::~Dijkstra()
	{
	}


	///<summary>
	/// Dijkstra�@�ɂ��2�_�Ԃ̍ŒZ�H�����߂�
	/// �s���Ȓl�̏ꍇ��(nullptr, DBL_MAX)���Ԃ�
	/// �Q�l(https://ja.wikipedia.org/wiki/�_�C�N�X�g���@)
	/// distance_threshold��ݒ肷��Ƌ���������ȏ�ɂȂ����ꍇ�̒T����ł��؂�܂�
	/// ���̏ꍇ��from����to�ւ̌o�H���������Ă��Ȃ����(nullptr, DBL_MAX)���Ԃ�܂�
	///</summary>
	template <typename NODE, typename EDGE>
	RouteInfo<EDGE> Dijkstra<NODE, EDGE>::shortest_path(std::shared_ptr<const Collection::IdentifiableCollection<Graph::node_id, NODE>> node_collection, Graph::node_id from, Graph::node_id to, double distance_threshold)
	{

		if (from == to) return RouteInfo<EDGE>(std::make_shared<std::vector<node_id>>(), 0.0);

		//������
		size_t node_count = node_collection->size();

		//�D��x���L���[�̐錾
		std::unique_ptr<std::priority_queue<distance_info, std::vector<distance_info>, std::greater<distance_info>>> queue
			= std::make_unique<std::priority_queue<distance_info, std::vector<distance_info>, std::greater<distance_info>>>();

		//�n�_����C���f�b�N�Xi�̃m�[�h�ɓ��B����ŒZ�o�H�ɂ�����i�̑O�̃m�[�h�̃C���f�b�N�X
		std::unique_ptr<std::vector<int>> previous = std::make_unique<std::vector<int>>(node_count, NOWHERE);
		std::shared_ptr<std::vector<double>> distance_map = std::make_shared<std::vector<double>>(node_count, DBL_MAX);
		std::unique_ptr<std::unordered_map<node_id, int>> index_map = std::make_unique<std::unordered_map<node_id, int>>();
		std::unique_ptr<std::vector<node_id>> reverse_index_map = std::make_unique<std::vector<node_id>>(node_count);

		int index = 0;
		node_collection->foreach([&](std::shared_ptr<NODE const> node) {
			node_id id = node->get_id();
			index_map->insert(std::make_pair(id, index));
			reverse_index_map->at(index++) = id;
		});
		int index_from = index_map->at(from);
		distance_map->at(index_from) = 0.0;
		queue->push(std::make_pair(distance_map->at(index_from), from));

		//�������炪�{�v�Z
		while (!queue->empty()) {

			//�R�X�g���ŏ��̃m�[�h�����o��
			distance_info target = queue->top();
			double d = target.first;
			node_id v = target.second;
			queue->pop();

			//�����ŏ��R�X�g��distance_threshold�𒴂��Ă���ꍇ�͒T����ł��؂�
			if (d > distance_threshold || v == to) break;
			
			std::shared_ptr<NODE const> node = node_collection->read_by_id(target.second);
			node->for_each_edge([&](std::shared_ptr<EDGE const> edge) {
				node_id u = edge->get_to();
				double alt = d + edge->get_static_data()->get_distance();
				int index_u = index_map->at(u);
				double distance_u = distance_map->at(index_u);
				if (alt < distance_u) {
					distance_map->at(index_u) = alt;
					queue->push(std::make_pair(alt, u));
					previous->at(index_u) = index_map->at(v);
				}
			});
		}

		//previous�����ǂ��Čo�H���쐬
		int index_to = index_map->at(to);
		double total_distance = distance_map->at(index_to);
		if (total_distance > distance_threshold) return RouteInfo<EDGE>(nullptr, DBL_MAX);

		std::shared_ptr<std::vector<node_id>> path = std::make_shared<std::vector<node_id>>();
		int current_index = index_to;
		while (current_index != NOWHERE) {
			path->push_back(reverse_index_map->at(current_index));
			current_index = previous->at(current_index);
		}
		if (path->back() != from || path->front() != to) return RouteInfo<EDGE>(nullptr, DBL_MAX);
		path->pop_back();
		std::reverse(path->begin(), path->end());
		
		return RouteInfo<EDGE>(path, total_distance);
	}
}


