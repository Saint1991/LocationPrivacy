#include "stdafx.h"
#include "HayashidaDbMap.h"

namespace Map {

	///<summary>
	/// �R���X�g���N�^
	/// Routing��WarshallFloyd��p����
	///</summary>
	HayashidaDbMap::HayashidaDbMap(
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
	HayashidaDbMap::~HayashidaDbMap()
	{
	}


	///<summary>
	/// �}�b�v���\������ (�\�����e�̎����͂��ꂼ��DbNodeCollectionFactory, DbPoiCollectionFactory�̒�)
	///</summary>
	void HayashidaDbMap::build_map(const Graph::Rectangle<Geography::LatLng>& boundary)
	{
		std::unique_ptr<DbNodeCollectionFactory> node_factory = std::make_unique<DbNodeCollectionFactory>(setting_file_path, db_name, node_table, node_connection_table);
		std::unique_ptr<DbPoiCollectionFactory> poi_factory = std::make_unique<DbPoiCollectionFactory>(setting_file_path, db_name, poi_table, poi_connection_table);

		node_collection = node_factory->create_static_node_collection(boundary);
		poi_collection = poi_factory->create_static_node_collection(boundary);
	}
}
