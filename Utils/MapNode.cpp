#include "stdafx.h"
#include "MapNode.h"


namespace Geography 
{
	
	///<summary>
	/// �R���X�g���N�^
	///</summary>
	MapNode::MapNode(const Graph::node_id& id, std::shared_ptr<LatLng> position) : Graph::BasicNode<LatLng>(id, position)
	{
	}



	///<summary>
	/// �f�X�g���N�^
	///</summary>
	MapNode::~MapNode()
	{
	}
}

