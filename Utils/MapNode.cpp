#include "stdafx.h"
#include "MapNode.h"


namespace Geography 
{
	
	///<summary>
	/// コンストラクタ
	///</summary>
	MapNode::MapNode(const Graph::node_id& id, std::shared_ptr<LatLng> position) : Graph::BasicNode<LatLng>(id, position)
	{
	}


	///<summary>
	/// コピーコンストラクタ
	///</summary>
	MapNode::MapNode(const MapNode& node) : BasicNode<LatLng>(node)
	{
	}

	///<summary>
	/// デストラクタ
	///</summary>
	MapNode::~MapNode()
	{
	}

	
	///<summary>
	/// ノード間の距離計算(m)
	///</summary>
	double Geography::distance(const MapNode& node1, const MapNode& node2) 
	{
		return GeoCalculation::lambert_distance(*node1.data, *node2.data);
	}

	
	///<summary>
	/// ノードfromから見たノードtoの方位角の計算
	/// (東0度，南90度，西180度，北270度，単位はラジアン)
	///</summary>
	double Geography::azimuth_angle(const MapNode& from, const MapNode& to)
	{
		return GeoCalculation::lambert_azimuth_angle(*from.data, *to.data);
	}
}

