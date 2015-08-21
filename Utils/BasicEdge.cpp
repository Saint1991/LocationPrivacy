#include "stdafx.h"
#include "BasicEdge.h"

namespace Graph 
{

	///<summary>
	/// コンストラクタ
	///</summary>
	BasicEdge::BasicEdge(const node_id& to) : _to(to)
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	BasicEdge::~BasicEdge()
	{
	}


	///<summary>
	/// 接続先を取得する
 	///</summary>
	node_id BasicEdge::get_to() const
	{
		return _to;
	}


	bool BasicEdge::operator==(const BasicEdge& e) const
	{
		return _to == e.get_to();
	}


	bool BasicEdge::operator!=(const BasicEdge& e) const
	{
		return _to != e.get_to();
	}
}

