#include "stdafx.h"
#include "MapNodeIndicator.h"

namespace Graph
{

	///<summary>
	/// コンストラクタ
	///</summary>
	MapNodeIndicator::MapNodeIndicator(Graph::node_id id, NodeType type) : _id(id), _type(type)
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	MapNodeIndicator::~MapNodeIndicator()
	{
	}


	///<summary>
	/// 指し示すIDの取得
	///</summary>
	Graph::node_id MapNodeIndicator::id() const
	{
		return _id;
	}


	///<summary>
	/// 指し示すノードの種別を取得
	///</summary>
	NodeType MapNodeIndicator::type() const
	{
		return _type;
	}


	bool MapNodeIndicator:: operator==(const MapNodeIndicator& id) const
	{
		return _id == id.id() && _type == id.type();
	}

	bool MapNodeIndicator::operator!=(const MapNodeIndicator& id) const
	{
		return _id != id.id() || _type != id.type();
	}
}

