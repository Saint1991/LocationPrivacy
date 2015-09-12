#include "stdafx.h"
#include "MapNodeIdIndicator.h"

namespace Map
{

	///<summary>
	/// コンストラクタ
	///</summary>
	MapNodeIdIndicator::MapNodeIdIndicator(Graph::node_id id, NodeType type) : _id(id), _type(type)
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	MapNodeIdIndicator::~MapNodeIdIndicator()
	{
	}


	///<summary>
	/// 指し示すIDの取得
	///</summary>
	Graph::node_id MapNodeIdIndicator::id() const
	{
		return _id;
	}


	///<summary>
	/// 指し示すノードの種別を取得
	///</summary>
	NodeType MapNodeIdIndicator::type() const
	{
		return _type;
	}
}

