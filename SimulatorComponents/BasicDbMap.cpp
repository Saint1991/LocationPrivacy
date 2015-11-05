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
		std::vector<std::shared_ptr<BasicPoi const>> ret;
		for (std::vector<std::shared_ptr<BasicPoi const>>::iterator iter = within_boundary.begin(); iter != within_boundary.end(); iter++) {
			if (*iter != nullptr && (*iter)->category_id() == category_id) ret.push_back(*iter);
		}
		return ret;
	}

	///<summary>
	/// �w�肵����ԓ��ɑ��݂���J�e�S��ID��category_id��POI�ꗗ���擾����
	///</summary>
	std::vector<std::shared_ptr<BasicPoi const>> BasicDbMap::find_pois_of_category_within_boundary(const Graph::Rectangle<>& boundary, const std::string& category_id) const
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
		int index = generator.uniform_distribution(0, within_boundary.size() - 1);
		return within_boundary.at(index);
	}

	///<summary>
	/// �w�肵���̈���ɑ��݂��C�J�e�S����category_id��POI���烉���_���Ɉ�I�����Ď擾����
	///</summary>
	std::shared_ptr<BasicPoi const> BasicDbMap::find_random_poi_within_boundary(const Graph::Rectangle<>& boundary, const std::string& category_id) const
	{
		Graph::box query_box(Graph::point(boundary.left, boundary.bottom), Graph::point(boundary.right, boundary.top));
		return find_random_poi_within_boundary(query_box, category_id);
	}
}




