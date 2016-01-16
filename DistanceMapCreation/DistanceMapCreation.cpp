// DistanceMapCreation.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "BasicDbMap2.h"
#include "MySQLDb.h"
#include "Dijkstra.h"

//15���ȓ��ɓ��B�\��POI�Ԃ̋����}�b�v���쐬����
typedef std::pair<double, Graph::node_id> distance_info;
constexpr int NODE_NUM = 325946;
constexpr int NOWHERE = -1;
constexpr double WALKING_SPEED = 5.0 * 1000;
constexpr double MAX_INTERVAL = 0.25;
constexpr double DISTANCE_LIMIT = WALKING_SPEED * MAX_INTERVAL;

std::unordered_map<Graph::node_id, double> dijkstra(std::shared_ptr<Collection::IdentifiableCollection<Graph::node_id, Map::BasicMapNode> const> node_collection, std::shared_ptr<std::unordered_map<Graph::node_id, int>> index_map, std::shared_ptr<std::vector<Graph::node_id>> reverse_index_map,  Graph::node_id from)
{
	std::unordered_map<Graph::node_id, double> result;

	//������
	size_t node_count = node_collection->size();

	//�D��x���L���[�̐錾
	std::unique_ptr<std::priority_queue<distance_info, std::vector<distance_info>, std::greater<distance_info>>> queue
		= std::make_unique<std::priority_queue<distance_info, std::vector<distance_info>, std::greater<distance_info>>>();

	//�n�_����C���f�b�N�Xi�̃m�[�h�ɓ��B����ŒZ�o�H�ɂ�����i�̑O�̃m�[�h�̃C���f�b�N�X
	std::shared_ptr<std::vector<double>> distance_list = std::make_shared<std::vector<double>>(node_count, DBL_MAX);

	int index_from = index_map->at(from);
	distance_list->at(index_from) = 0.0;
	queue->push(std::make_pair(distance_list->at(index_from), from));

	//�������炪�{�v�Z
	while (!queue->empty()) {

		//�R�X�g���ŏ��̃m�[�h�����o��(�����ŋ������m�肷��)
		distance_info target = queue->top();
		double d = target.first;
		Graph::node_id v = target.second;
		result.insert(std::make_pair(v, d));
		queue->pop();

		//�����ŏ��R�X�g��distance_threshold�𒴂��Ă���ꍇ�͒T����ł��؂�
		if (d > DISTANCE_LIMIT) break;

		std::shared_ptr<Map::BasicMapNode const> node = node_collection->read_by_id(target.second);
		node->for_each_edge([&](std::shared_ptr<Map::BasicRoad const> edge) {
			Graph::node_id u = edge->get_to();
			double alt = d + edge->get_static_data()->get_distance();
			int index_u = index_map->at(u);
			double distance_u = distance_list->at(index_u);
			if (alt < distance_u) {
				distance_list->at(index_u) = alt;
				queue->push(std::make_pair(alt, u));
			}
		});
	}

	return result;
}

int main()
{
	constexpr char* DB_NAME = "map_tokyo_category_top_level";
	std::unique_ptr<Map::BasicDbMap2> map = std::make_unique<Map::BasicDbMap2>(std::make_shared<Graph::Dijkstra<Map::BasicMapNode, Map::BasicRoad>>(), "../settings/mydbsettings.xml", DB_NAME);
	map->load({ 35.9, 139.4, 35.5, 141.0 });
	std::shared_ptr<Collection::IdentifiableCollection<Graph::node_id, Map::BasicMapNode> const> node_collection = map->get_node_collection();

	std::shared_ptr<std::unordered_map<Graph::node_id, int>> index_map = std::make_unique<std::unordered_map<Graph::node_id, int>>();
	std::shared_ptr<std::vector<Graph::node_id>> reverse_index_map = std::make_unique<std::vector<Graph::node_id>>(node_collection->size());
	int index = 0;
	node_collection->foreach([&](std::shared_ptr<Map::BasicMapNode const> node) {
		Graph::node_id id = node->get_id();
		index_map->insert(std::make_pair(id, index));
		reverse_index_map->at(index++) = id;
	});

	std::unique_ptr<Db::IDbSettingsLoader> loader = std::make_unique<Db::DbSettingsFileLoader>("../settings/mydbsettings.xml");
	std::shared_ptr<Db::MySQLDb> db = std::make_unique<Db::MySQLDb>(std::move(loader));
	db->use("map_tokyo_category_top_level");
	
	std::string query = "INSERT INTO distances (id1, id2, distance) VALUES ";
	std::stringstream value_clause;
	
	for (int i = 28999; i <= NODE_NUM; i++) {
		std::cout << i << std::endl;
		std::unordered_map<Graph::node_id, double> result = dijkstra(node_collection, index_map, reverse_index_map, i);

		int count = 0;
		
		for (std::unordered_map<Graph::node_id, double>::const_iterator iter = result.begin(); iter != result.end(); ++iter) {
			value_clause << "(" << i << ", " << iter->first << ", " << iter->second << "),";
		}
		std::string execute_query = query + value_clause.str();
		execute_query.replace(execute_query.end() - 1, execute_query.end(), ";");
		//std::cout << execute_query << std::endl;
		db->execute(execute_query);
		value_clause.str("");
		value_clause.clear(std::stringstream::goodbit);
	}

    return 0;
}

