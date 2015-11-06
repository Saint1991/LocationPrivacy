#include "stdafx.h"
#include "BasicPoi.h"

namespace Map
{


	///<summary>
	/// コンストラクタ
	///</summary>
	BasicPoi::BasicPoi(Graph::node_id id, Geography::PoiData data) : Graph::Node<Geography::PoiData, BasicRoad>(id, std::make_shared<Geography::PoiData>(data))
	{

	}


	///<summary>
	/// コンストラクタ
	///</summary>
	BasicPoi::BasicPoi(Graph::node_id id, Geography::LatLng position, std::string venue_name, std::string category_id, std::string category_name) 
		: Graph::Node<Geography::PoiData, BasicRoad>(id, std::make_shared<Geography::PoiData>(venue_name, category_id, category_name, position))
	{

	}


	///<summary>
	/// コピーコンストラクタ
	///</summary>
	BasicPoi::BasicPoi(const BasicPoi& poi) : Graph::Node<Geography::PoiData, BasicRoad>(poi)
	{

	}


	///<summary>
	/// デストラクタ
	///</summary>
	BasicPoi::~BasicPoi()
	{
	}


	///<summary>
	/// 使用禁止のため
	///</summary>
	bool BasicPoi::connect_to(std::shared_ptr<BasicRoad> road)
	{
		return false;
	}

	///<summary>
	/// カテゴリIDの取得
	///</summary>
	const std::string BasicPoi::category_id() const
	{
		return data->get_category_id();
	}


	///<summary>
	/// カテゴリ名の取得
	///</summary>
	const std::string BasicPoi::category_name() const
	{
		return data->get_category_name();
	}


	///<summary>
	/// POI名の取得
	///</summary>
	const std::string BasicPoi::name() const
	{
		return data->get_venue_name();
	}


	///<summary>
	/// 緯度の取得 
	///</summary>
	double BasicPoi::lat() const
	{
		return data->get_position().lat();
	}


	///<summary>
	/// 経度の取得
	///</summary>
	double BasicPoi::lng() const
	{
		return data->get_position().lng();
	}


	///<summary>
	/// 接続関係を設定します
	/// 既存の接続関係があった場合はそれを削除して新しい値を設定します
	/// 片方でも接続がうまくいかなかった場合はfalseを返します
	///</summary>
	bool BasicPoi::set_relation(Graph::node_id id1, double distance_to1, Graph::node_id id2, double distance_to2)
	{
		edge_list->clear();
		bool is_succeed = true;
		is_succeed = is_succeed && Graph::Node<Geography::PoiData, BasicRoad>::connect_to(std::make_shared<BasicRoad>(id1, distance_to1));
		is_succeed = is_succeed && Graph::Node<Geography::PoiData, BasicRoad>::connect_to(std::make_shared<BasicRoad>(id2, distance_to2));
		return is_succeed;
	}
	
	///<summary>
	/// 接続関係を設定します
	/// 既存の接続関係があった場合はそれを削除して新しい値を設定します
	/// 片方でも接続がうまくいかなかった場合はfalseを返します
	///</summary>
	bool BasicPoi::set_relation(std::pair<Graph::node_id, double> relation1, std::pair<Graph::node_id, double> relation2)
	{
		return set_relation(relation1.first, relation1.second, relation2.first, relation2.second);
	}


	///<summary>
	/// どのNodeの間にあるかを取得します
	/// 隣接ノードのIDの組を返します
	///</summary>
	std::pair<Graph::node_id, Graph::node_id> BasicPoi::between() const
	{
		std::set<std::shared_ptr<Graph::BasicEdge>, std::function<bool(std::shared_ptr<Graph::BasicEdge>, std::shared_ptr<Graph::BasicEdge>)>>::const_iterator iter = edge_list->begin();
		Graph::node_id id1 = INVALID;
		Graph::node_id id2 = INVALID;
		
		if (edge_list->size() != 2) return std::pair<Graph::node_id, Graph::node_id>(id1, id2);
		id1 = (*iter)->get_to();
		id2 = (*(++iter))->get_to();
		return std::pair<Graph::node_id, Graph::node_id>(id1, id2);
	}


	///<summary>
	/// toまでの距離を取得する
	///</summary>
	double BasicPoi::distance_to(const Graph::node_id& to) const
	{
		std::shared_ptr<BasicRoad const> road = get_static_edge_to(to);
		if (road == nullptr) return id == to ? 0.0 : DBL_MAX;
		return road->distance();
	}
}
