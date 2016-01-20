#ifdef SIMULATORCOMPONENTS_EXPORTS
#define BASIC_DB_MAP_API __declspec(dllexport)
#else
#define BASIC_DB_MAP_API __declspec(dllimport)
#endif

#pragma once
#include "Map.h"
#include "PreferenceTreeNode.h"
#include "BasicMapNode.h"
#include "BasicPoi.h"
#include "BasicRoad.h"
#include "DbNodeCollectionFactory.h"
#include "DbPoiCollectionFactory.h"
#include "Probability.h"
#include "Rectangle.h"
#include "IRoutingModule.h"
#include "Sequence.h"
#include "SequentialTree.h"
#include "DbDistanceMap.h"

namespace Map
{

	///<summary>
	/// DBから読み出して基本的なMapを構成するためのクラス
	/// Tableのフォーマットはそれぞれ
	/// nodes (id, latitude, longitude)
	/// node_connections (id1, id2, distance)で
	/// ※node_connectionsに格納されているコネクションを双方向で構成する
	/// pois (id, latitude, longitude, category_id, category_name, venue_name)
	/// poi_connections (id, to1, to2, distance1, distance2)
	/// を想定
	///</summary>
	class BASIC_DB_MAP_API BasicDbMap : public Graph::Map<BasicMapNode, BasicPoi, BasicRoad>
	{
		
	protected:
		std::string setting_file_path;
		std::string db_name;
		std::string node_table;
		std::string node_connection_table;
		std::string poi_table;
		std::string poi_connection_table;

		virtual void build_map(const Graph::Rectangle<Geography::LatLng>& boundary);

	public:
		BasicDbMap (
			std::shared_ptr<Graph::IRoutingModule<BasicMapNode, BasicRoad>> routing_method,
			const std::string& setting_file_path,
			const std::string& db_name,
			const std::string& node_table = "nodes",
			const std::string& node_connection_table = "node_connections",
			const std::string& poi_table = "pois",
			const std::string& poi_connection_table = "poi_connections"
		);
		virtual ~BasicDbMap();

		std::vector<std::shared_ptr<BasicPoi const>> find_pois_of_category_within_boundary(const Graph::box& boundary, const std::string& category_id) const;
		std::vector<std::shared_ptr<BasicPoi const>> find_pois_of_category_within_boundary(const Graph::Rectangle<Geography::LatLng>& boundary, const std::string& category_id) const;

		std::shared_ptr<BasicPoi const> find_random_poi_within_boundary(const Graph::box& boundary, const std::string& category_id) const;
		std::shared_ptr<BasicPoi const> find_random_poi_within_boundary(const Graph::Rectangle<Geography::LatLng>& boundary, const std::string& category_id) const;

		std::shared_ptr<std::vector<Collection::Sequence<Graph::MapNodeIndicator>>> find_reachable_trajectories(const Graph::MapNodeIndicator& point_basis, const Collection::Sequence<User::category_id> category_sequence, const std::vector<double> reachable_distance_list, int poi_limit_for_each_phase = 8) const;
		Collection::Sequence<User::category_id> convert_to_category_sequence(const std::vector<Graph::MapNodeIndicator>& trajectory) const;
		static Graph::Rectangle<Geography::LatLng> create_reachable_rect(const Geography::LatLng& center, double reachable_distance);
	};

}

