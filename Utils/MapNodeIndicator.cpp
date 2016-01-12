#include "stdafx.h"
#include "MapNodeIndicator.h"

namespace Graph
{

	
	///<summary>
	/// コンストラクタ
	///</summary>
	MapNodeIndicator::MapNodeIndicator() : _id(INVALID), _type(NodeType::INVALID)
	{

	}

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

	#pragma region Export
	///<summary>
	/// ファイル出力向けのハッシュデータを作成します．
	///</summary>
	std::unordered_map<std::string, std::string> MapNodeIndicator::get_export_data() const
	{
		std::unordered_map<std::string, std::string> ret = {
			{ NODE_ID, std::to_string(_id) },
			{ NODE_TYPE, std::to_string(_type) }
		};
		return ret;
	}

	#pragma endregion Export関係
}

