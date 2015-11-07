#pragma once
#include "NodeCollectionFactory.h"
#include "LatLng.h"
#include "IdentifiableCollection.h"
#include "MapNodeIndicator.h"
#include "boost\geometry\index\rtree.hpp"
#include "Rectangle.h"
#include "RoutingClient.h"
#include "IRoutingModule.h"

namespace Graph
{

	namespace geometry = boost::geometry;
	namespace model = geometry::model;
	namespace index = geometry::index;
	
	typedef model::point<double, 2, boost::geometry::cs::cartesian> point;
	typedef model::box<point> box;
	
	///<summary>
	/// 地図に関する抽象クラス
	/// 具体的なマップはこれを継承して作る
	/// NODE, PATHにはそれぞれNode, Edgeを継承したクラスが使える
	/// PATHにはBasicPathDataを継承しているデータを保持しているEdgeしか使えないので注意
	/// とりあえずR-Treeの座標は緯度，経度で固定している
	/// Mapの構築はload(Rectangle boundary)を読んだタイミングで始まる
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	class Map
	{

		#pragma region RTree
		typedef std::shared_ptr<POI const> rtree_value;
		class MyPoiAdapter
		{
		public:
			typedef point result_type;
			point operator()(rtree_value val) const
			{
				double x = val->data->get_position().x();
				double y = val->data->get_position().y();
				return point(x, y);
			}
		};
		typedef index::rtree<rtree_value, index::quadratic<16>, MyPoiAdapter> rtree;
		#pragma endregion

	protected:
		std::shared_ptr<const Collection::IdentifiableCollection<Graph::node_id, NODE>> node_collection;
		std::shared_ptr<const Collection::IdentifiableCollection<Graph::node_id, POI>> poi_collection;
		std::unique_ptr<rtree> rtree_index;
		std::unique_ptr<RoutingClient<NODE, PATH>> routing_client;
		std::shared_ptr<IRoutingModule<NODE, PATH>> routing_method;

		virtual void build_map(const Graph::Rectangle<Geography::LatLng>& boundary) = 0;
		void build_rtree_index();

	public:
		typedef std::pair<std::vector<MapNodeIndicator>, double> shortest_path_info;
		
		Map(std::shared_ptr<IRoutingModule<NODE, PATH>> routing_method);
		virtual ~Map();

		void load(const Graph::Rectangle<Geography::LatLng>& boundary);


		shortest_path_info get_shortest_path_info(const MapNodeIndicator& from, const MapNodeIndicator& to, double distance_threshold = DBL_MAX) const;
		double shortest_distance(const MapNodeIndicator& from, const MapNodeIndicator& to, double distance_threshold = DBL_MAX) const;
		RouteInfo<PATH> get_shortest_route_info_between_intersections(const node_id& from, const node_id& to, double distance_threshold = DBL_MAX) const;
		std::vector<MapNodeIndicator> get_shortest_path(const MapNodeIndicator& source, const MapNodeIndicator& destination, double distance_threshold = DBL_MAX) const;
		double calc_necessary_time(const MapNodeIndicator& from, const MapNodeIndicator& to, const double& avg_speed, double distance_threshold = DBL_MAX) const;
		bool is_reachable(const MapNodeIndicator& from, const MapNodeIndicator& to, const double& avg_speed, const double& time_limit) const;
		


		virtual std::vector<std::shared_ptr<POI const>> find_pois_within_boundary(const box& boundary) const;
		virtual std::vector<std::shared_ptr<POI const>> find_pois_within_boundary(const Graph::Rectangle<>& boundary) const;
		virtual std::vector<std::shared_ptr<POI const>> find_pois_within_boundary(const Graph::Rectangle<Geography::LatLng>& boundary) const;

		std::shared_ptr<NODE const> get_static_node(const node_id& id) const;
		std::shared_ptr<POI const> get_static_poi(const node_id& poi_id) const;
	};
}

#include "Map.hpp"
