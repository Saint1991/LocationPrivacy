#pragma once
#include "NodeCollectionFactory.h"
#include "LatLng.h"
#include "IdentifiableCollection.h"
#include "RoutingMethod.h"
#include "RoutingTable.h"
#include "MapNodeIndicator.h"
#include "boost\geometry\index\rtree.hpp"
#include "Rectangle.h"

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
		std::unique_ptr<RoutingMethod<NODE, PATH>> routing_method;
		std::shared_ptr<const Collection::IdentifiableCollection<Graph::node_id, NODE>> node_collection;
		std::shared_ptr<const Collection::IdentifiableCollection<Graph::node_id, POI>> poi_collection;
		std::unique_ptr<rtree> rtree_index;
		
		virtual void build_map(const Graph::Rectangle<Geography::LatLng>& boundary) = 0;
		void build_rtree_index();

		///<summary>
		/// 最短路を格納するルーティングテーブル
		/// SourceからDestinationへ行くのに次にどのノードに行く必要があるかを格納する．
		/// Mapの構成はinitializeを呼び出した際に行われる
		///</summary>
		std::unique_ptr<RoutingTable const> routing_table;

	private:
		std::pair<node_id, node_id> get_intersection_ends_of_shortest_path(const MapNodeIndicator& from, const MapNodeIndicator& to) const;

	public:
		Map(std::unique_ptr<RoutingMethod<NODE, PATH>> routing_method);
		virtual ~Map();

		void load(const Graph::Rectangle<Geography::LatLng>& boundary);

		double shortest_distance(const MapNodeIndicator& from, const MapNodeIndicator& to) const;
		const std::vector<MapNodeIndicator> get_shortest_path(const MapNodeIndicator& source, const MapNodeIndicator& destination) const;
		double calc_necessary_time(const MapNodeIndicator& from, const MapNodeIndicator& to, const double& avg_speed) const;
		bool is_reachable(const MapNodeIndicator& from, const MapNodeIndicator& to, const double& avg_speed, const double& time_limit) const;
		
		virtual std::vector<std::shared_ptr<POI const>> find_pois_within_boundary(const box& boundary) const;
		virtual std::vector<std::shared_ptr<POI const>> find_pois_within_boundary(const Graph::Rectangle<>& boundary) const;
		virtual std::vector<std::shared_ptr<POI const>> find_pois_within_boundary(const Graph::Rectangle<Geography::LatLng>& boundary) const;

		std::shared_ptr<NODE const> get_static_node(const node_id& id) const;
		std::shared_ptr<POI const> get_static_poi(const node_id& poi_id) const;
	};
}

#include "Map.hpp"
