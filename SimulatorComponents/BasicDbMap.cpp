#include "stdafx.h"
#include "BasicDbMap.h"

namespace Map
{

	///<summary>
	/// コンストラクタ
	/// RoutingはWarshallFloydを用いる
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
	/// デストラクタ
	///</summary>
	BasicDbMap::~BasicDbMap()
	{
	}


	///<summary>
	/// マップを構成する (構成内容の実装はそれぞれDbNodeCollectionFactory, DbPoiCollectionFactoryの中)
	///</summary>
	void BasicDbMap::build_map(const Graph::Rectangle<Geography::LatLng>& boundary)
	{
		std::unique_ptr<DbNodeCollectionFactory> node_factory = std::make_unique<DbNodeCollectionFactory>(setting_file_path, db_name, node_table, node_connection_table);
		std::unique_ptr<DbPoiCollectionFactory> poi_factory = std::make_unique<DbPoiCollectionFactory>(setting_file_path, db_name, poi_table, poi_connection_table);

		node_collection = node_factory->create_static_node_collection(boundary);
		poi_collection = poi_factory->create_static_node_collection(boundary);
	}


	///<summary>
	/// 指定した区間内に存在するカテゴリIDがcategory_idのPOI一覧を取得する
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
	/// 指定した区間内に存在するカテゴリIDがcategory_idのPOI一覧を取得する
	///</summary>
	std::vector<std::shared_ptr<BasicPoi const>> BasicDbMap::find_pois_of_category_within_boundary(const Graph::Rectangle<Geography::LatLng>& boundary, const std::string& category_id) const
	{
		Graph::box query_box(Graph::point(boundary.left, boundary.bottom), Graph::point(boundary.right, boundary.top));
		return find_pois_of_category_within_boundary(query_box, category_id);
	}


	///<summary>
	/// 指定した領域内に存在し，カテゴリがcategory_idのPOIからランダムに一つ選択して取得する
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
	/// 指定した領域内に存在し，カテゴリがcategory_idのPOIからランダムに一つ選択して取得する
	///</summary>
	std::shared_ptr<BasicPoi const> BasicDbMap::find_random_poi_within_boundary(const Graph::Rectangle<Geography::LatLng>& boundary, const std::string& category_id) const
	{
		Graph::box query_box(Graph::point(boundary.left, boundary.bottom), Graph::point(boundary.right, boundary.top));
		return find_random_poi_within_boundary(query_box, category_id);
	}


	///<summary>
	/// 基準点と移動可能な距離から，探索範囲を作成する
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
	/// point_basisを基準にcategory_sequenceとmove_distance_listの制約を満たす経路を全て取得する
	/// 経路の長さはcategory_sequenceの長さに依存
	///</summary>
	std::shared_ptr<std::vector<std::vector<Graph::MapNodeIndicator>>> BasicDbMap::find_reachable_trajectory(const Graph::MapNodeIndicator& point_basis, const Collection::Sequence<User::category_id> category_sequence, const std::vector<double> reachable_distance_list) const
	{
		std::shared_ptr<std::vector<std::vector<Graph::MapNodeIndicator>>> ret = nullptr;

		std::vector<double>::const_iterator reachable_distance = reachable_distance_list.begin();
		std::shared_ptr<BasicPoi const> current_poi = get_static_poi(point_basis.id());
		Geography::LatLng current_position = current_poi->get_point();
		
		for (Collection::Sequence<User::category_id>::const_iterator category = category_sequence.begin(); category != category_sequence.end() && reachable_distance != reachable_distance_list.end(); category++, reachable_distance++) {
			Graph::Rectangle<Geography::LatLng> search_boundary = create_reachable_rect(current_position, *reachable_distance);
			std::vector<std::shared_ptr<BasicPoi const>> pois = find_pois_of_category_within_boundary(search_boundary, *category);
		}

		return ret;
	}

	

	
}




