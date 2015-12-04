#include "stdafx.h"
#include "BasicDbMap.h"

namespace Map
{

	///<summary>
	/// �R���X�g���N�^
	/// Routing��WarshallFloyd��p����
	///</summary>
	BasicDbMap::BasicDbMap (
		std::shared_ptr<Graph::IRoutingModule<BasicMapNode, BasicRoad>> routing_method, 
		const std::string& setting_file_path,
		const std::string& db_name,
		const std::string& node_table,
		const std::string& node_connection_table,
		const std::string& poi_table,
		const std::string& poi_connection_table)
		: Graph::Map<BasicMapNode, BasicPoi, BasicRoad>(std::move(routing_method)), 
		setting_file_path(setting_file_path),
		db_name(db_name),
		node_table(node_table),
		node_connection_table(node_connection_table),
		poi_table(poi_table),
		poi_connection_table(poi_connection_table)
	{
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	BasicDbMap::~BasicDbMap()
	{
	}


	///<summary>
	/// �}�b�v���\������ (�\�����e�̎����͂��ꂼ��DbNodeCollectionFactory, DbPoiCollectionFactory�̒�)
	///</summary>
	void BasicDbMap::build_map(const Graph::Rectangle<Geography::LatLng>& boundary)
	{
		std::unique_ptr<DbNodeCollectionFactory> node_factory = std::make_unique<DbNodeCollectionFactory>(setting_file_path, db_name, node_table, node_connection_table);
		std::unique_ptr<DbPoiCollectionFactory> poi_factory = std::make_unique<DbPoiCollectionFactory>(setting_file_path, db_name, poi_table, poi_connection_table);

		node_collection = node_factory->create_static_node_collection(boundary);
		poi_collection = poi_factory->create_static_node_collection(boundary);
	}


	///<summary>
	/// �w�肵����ԓ��ɑ��݂���J�e�S��ID��category_id��POI�ꗗ���擾����
	///</summary>
	std::vector<std::shared_ptr<BasicPoi const>> BasicDbMap::find_pois_of_category_within_boundary(const Graph::box& boundary, const std::string& category_id) const
	{
		std::vector<std::shared_ptr<BasicPoi const>> within_boundary = Graph::Map<BasicMapNode, BasicPoi, BasicRoad>::find_pois_within_boundary(boundary);
		if (category_id == User::ANY) return within_boundary;
		
		std::vector<std::shared_ptr<BasicPoi const>> ret;
		for (std::vector<std::shared_ptr<BasicPoi const>>::iterator iter = within_boundary.begin(); iter != within_boundary.end(); iter++) {
			if (*iter != nullptr && (*iter)->category_id() == category_id) ret.push_back(*iter);
		}
		return ret;
	}

	///<summary>
	/// �w�肵����ԓ��ɑ��݂���J�e�S��ID��category_id��POI�ꗗ���擾����
	///</summary>
	std::vector<std::shared_ptr<BasicPoi const>> BasicDbMap::find_pois_of_category_within_boundary(const Graph::Rectangle<Geography::LatLng>& boundary, const std::string& category_id) const
	{
		Graph::box query_box(Graph::point(boundary.left, boundary.bottom), Graph::point(boundary.right, boundary.top));
		return find_pois_of_category_within_boundary(query_box, category_id);
	}


	///<summary>
	/// �w�肵���̈���ɑ��݂��C�J�e�S����category_id��POI���烉���_���Ɉ�I�����Ď擾����
	///</summary>
	std::shared_ptr<BasicPoi const> BasicDbMap::find_random_poi_within_boundary(const Graph::box& boundary, const std::string& category_id) const
	{
		std::vector<std::shared_ptr<BasicPoi const>> within_boundary = find_pois_of_category_within_boundary(boundary, category_id);
		Math::Probability generator;
		if (within_boundary.size() == 0) return nullptr;
		int index = generator.uniform_distribution(0, within_boundary.size() - 1);
		return within_boundary.at(index);
	}

	///<summary>
	/// �w�肵���̈���ɑ��݂��C�J�e�S����category_id��POI���烉���_���Ɉ�I�����Ď擾����
	///</summary>
	std::shared_ptr<BasicPoi const> BasicDbMap::find_random_poi_within_boundary(const Graph::Rectangle<Geography::LatLng>& boundary, const std::string& category_id) const
	{
		Graph::box query_box(Graph::point(boundary.left, boundary.bottom), Graph::point(boundary.right, boundary.top));
		return find_random_poi_within_boundary(query_box, category_id);
	}


	///<summary>
	/// ��_�ƈړ��\�ȋ�������C�T���͈͂��쐬����
	///</summary>
	Graph::Rectangle<Geography::LatLng> create_reachable_rect(const Geography::LatLng& center, double reachable_distance)
	{
		double top = center.lat() + 0.000007 * reachable_distance;
		double left = center.lng() - 0.000009 * reachable_distance;
		double bottom = center.lat() - 0.000007 * reachable_distance;
		double right = center.lng() + 0.000009 * reachable_distance;
		return Graph::Rectangle<Geography::LatLng>(top, left, bottom, right);
	}

	///<summary>
	/// point_basis�����category_sequence��move_distance_list�̐���𖞂����o�H��S�Ď擾����
	/// �o�H�̒�����category_sequence�̒����Ɉˑ�
	///</summary>
	std::shared_ptr<std::vector<Collection::Sequence<Graph::MapNodeIndicator>>> BasicDbMap::find_reachable_trajectory(const Graph::MapNodeIndicator& point_basis, const Collection::Sequence<User::category_id> category_sequence, const std::vector<double> reachable_distance_list) const
	{
		std::shared_ptr<std::vector<Collection::Sequence<Graph::MapNodeIndicator>>> ret = std::make_shared<std::vector<Collection::Sequence<Graph::MapNodeIndicator>>>();

		//���݂̖K��n�_
		std::shared_ptr<BasicPoi const> current_poi = get_static_poi(point_basis.id());

		//���݂̃m�[�h���w���C�e���[�^
		Graph::node_id current_node_id = 1;
		std::unique_ptr<Graph::SequentialTree<Graph::SequentialTreeNode<Graph::node_id>, Graph::node_id>> trajectory_tree = std::make_unique<Graph::SequentialTree<Graph::SequentialTreeNode<Graph::node_id>, Graph::node_id>>();
		Graph::BaseIterator<Graph::SequentialTreeNode<Graph::node_id>, Graph::node_id, Graph::BasicEdge> current_node = trajectory_tree->root<Graph::BaseIterator<Graph::SequentialTreeNode<Graph::node_id>, Graph::node_id, Graph::BasicEdge>>();
		current_node.add_child(std::make_shared<Graph::SequentialTreeNode<Graph::node_id>>(current_node_id++,0,  1, std::make_shared<Graph::node_id>(point_basis.id())));

		while (current_node->get_connecting_node_list().size() != 0 && current_node->get_depth() < reachable_distance_list.size()) {
			std::vector<Graph::node_id> children = current_node->get_connecting_node_list();
			for (std::vector<Graph::node_id>::const_iterator current_child = children.begin(); current_child != children.end(); current_child++) {
				current_node = trajectory_tree->get_iter_by_id<Graph::BaseIterator<Graph::SequentialTreeNode<Graph::node_id>, Graph::node_id, Graph::BasicEdge>>(*current_child);
				current_poi = get_static_poi(*current_node->data);
				
				//���B�\��POI�̒T��
				Graph::Rectangle<Geography::LatLng> search_boundary = create_reachable_rect(current_poi->get_point(), reachable_distance_list.at(current_node->get_depth() - 1));
				std::vector<std::shared_ptr<BasicPoi const>> pois = find_pois_of_category_within_boundary(search_boundary, category_sequence.at(current_node->get_depth() - 1));
				for (std::vector<std::shared_ptr<BasicPoi const>>::const_iterator poi = pois.begin(); poi != pois.end(); poi++) {
					if (shortest_distance(Graph::MapNodeIndicator(current_poi->get_id()), Graph::MapNodeIndicator((*poi)->get_id())) <= reachable_distance_list.at(current_node->get_depth() - 1)) {
						current_node.add_child(std::make_shared<Graph::SequentialTreeNode<Graph::node_id>>(current_node_id++, current_node->get_id(), current_node->get_depth() + 1, std::make_shared<Graph::node_id>((*poi)->get_id())));
					}
				}
			}
		}
		
		trajectory_tree->for_each_sequence([&ret, &reachable_distance_list](const Collection::Sequence<std::shared_ptr<Graph::SequentialTreeNode<Graph::node_id> const>>& sequence) {
			if (sequence.size() == reachable_distance_list.size() + 1) {
				Collection::Sequence<Graph::MapNodeIndicator> trajectory(sequence.size());
				int index = 0;
				for (Collection::Sequence<std::shared_ptr<Graph::SequentialTreeNode<Graph::node_id> const>>::const_iterator iter = sequence.begin(); iter != sequence.end(); iter++, index++) {
					trajectory.at(index) = Graph::MapNodeIndicator(*(*iter)->data);
				}
				ret->push_back(trajectory);
			}
		});
		return ret;
	}
	
}




