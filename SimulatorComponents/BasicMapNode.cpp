#include "stdafx.h"
#include "BasicMapNode.h"

namespace Map
{

	///<summary>
	/// コンストラクタ

	///</summary>
	BasicMapNode::BasicMapNode(Graph::node_id id, Geography::LatLng position) 
		: Graph::Node<Geography::LatLng, BasicRoad>(id, std::make_shared<Geography::LatLng>(position))
	{
	}


	///<summary>
	/// コピーコンストラクタ
	///</summary>
	BasicMapNode::BasicMapNode(const BasicMapNode& node) : Graph::Node<Geography::LatLng, BasicRoad>(node)
	{
	}

	///<summary>
	/// デストラクタ
	///</summary>
	BasicMapNode::~BasicMapNode()
	{
	}


	///<summary>
	/// 緯度を取得
	///</summary>
	double BasicMapNode::lat() const
	{
		return data->lat();
	}

	///<summary>
	/// 経度を取得
	///</summary>
	double BasicMapNode::lng() const
	{
		return data->lng();
	}


	///<summary>
	/// LatLngを取得する
	///</summary>
	Geography::LatLng BasicMapNode::get_point() const
	{
		return *data;
	}

	///<summary>
	/// toに対して距離distanceのエッジを作成
	///</summary>
	bool BasicMapNode::connect_to(Graph::node_id to, double distance)
	{
		return Graph::Node<Geography::LatLng, BasicRoad>::connect_to(std::make_shared<BasicRoad>(to, distance));
	}

	///<summary>
	/// toに対して距離distanceのエッジを作成
	///</summary>
	bool BasicMapNode::connect_to(std::shared_ptr<BasicRoad> road)
	{
		return Graph::Node<Geography::LatLng, BasicRoad>::connect_to(road);
	}


	///<summary>
	/// toのノードまでの距離を取得します
	/// ただし隣接していない場合はLONG_MAXを返します
	///</summary>
	double BasicMapNode::distance_to_next_node(Graph::node_id to) const
	{
		std::shared_ptr<BasicRoad const> road = get_static_edge_to(to);
		if (road == nullptr) return LONG_MAX;
		return road->distance();
	}
}

