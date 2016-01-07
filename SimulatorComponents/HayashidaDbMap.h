#ifdef SIMULATORCOMPONENTS_EXPORTS
#define HAYASHIDA_DB_MAP_API __declspec(dllexport)
#else
#define HAYASHIDA_DB_MAP_API __declspec(dllimport)
#endif

#pragma once
#include "BasicDbMap.h"
#include "BasicMapNode.h"
#include "BasicPoi.h"
#include "BasicRoad.h"
#include "DbNodeCollectionFactory.h"
#include "DbPoiCollectionFactory.h"
#include "Probability.h"
#include "Rectangle.h"
#include "IRoutingModule.h"
#include "LatLng.h"

namespace Map
{
	namespace geometry = boost::geometry;
	namespace model = geometry::model;
	namespace index = geometry::index;

	typedef model::point<double, 2, boost::geometry::cs::cartesian> point;
	typedef model::box<point> box;

	class HAYASHIDA_DB_MAP_API HayashidaDbMap : public BasicDbMap
	{

		#pragma region RTree
		typedef std::shared_ptr<BasicMapNode const> rtree_value;
		class MyNodeAdapter
		{
		public:
			typedef point result_type;
			point operator()(rtree_value val) const
			{
				double x = val->data->lng();
				double y = val->data->lat();
				return point(x, y);
			}
		};
		typedef index::rtree<rtree_value, index::quadratic<16>, MyNodeAdapter> rtree;
		#pragma endregion

	protected:
		std::unique_ptr<rtree> rtree_index_of_node;

	public:
		typedef std::pair<std::vector<Graph::MapNodeIndicator>, double> path_info;
		typedef std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>> node_pos_info;

		HayashidaDbMap(std::shared_ptr<Graph::IRoutingModule<BasicMapNode, BasicRoad>> routing_method,
			const std::string& setting_file_path,
			const std::string& db_name,
			const std::string& node_table = "nodes",
			const std::string& node_connection_table = "node_connections",
			const std::string& poi_table = "pois",
			const std::string& poi_connection_table = "poi_connections"
			);
		virtual ~HayashidaDbMap();
		
		std::shared_ptr<Graph::MapNodeIndicator> get_nearest_node_of_now_position(Geography::LatLng& now_pos) const;
		path_info search_random_path(const Graph::MapNodeIndicator& from, const Graph::MapNodeIndicator& to, double distance_threshold);
		path_info get_random_path_info(const Graph::MapNodeIndicator& from, const Graph::MapNodeIndicator& to, double distance_threshold = DBL_MAX) const;
		void build_rtree_index_of_node();

		std::vector<std::shared_ptr<BasicMapNode const>> find_nodes_within_boundary(const box& boundary) const;
		std::vector<std::shared_ptr<BasicMapNode const>> find_nodes_within_boundary(const Graph::Rectangle<>& boundary) const;
		std::vector<std::shared_ptr<BasicMapNode const>> find_nodes_within_boundary(const Graph::Rectangle<Geography::LatLng>& boundary) const;
		//bool is_reachable(const Graph::MapNodeIndicator& from, const Graph::MapNodeIndicator& to, const double& avg_speed, const double& time_limit) const;
	};

}