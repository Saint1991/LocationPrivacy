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
	/// デストラクタ
	///</summary>
	MapNode::~MapNode()
	{
	}
}

