#include "stdafx.h"
#include "BasicIntersection.h"

namespace Map
{

	///<summary>
	/// コンストラクタ
	///</summary>
	BasicIntersection::BasicIntersection(Graph::node_id id, Geography::LatLng position)  : BasicMapNode(id, position)
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	BasicIntersection::~BasicIntersection()
	{
	}
}

