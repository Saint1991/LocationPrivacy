#include "stdafx.h"
#include "BasicPath.h"


namespace Graph
{
	///<summary>
	/// コンストラクタ
	///</summary>
	BasicPath::BasicPath(const node_id& to, std::shared_ptr<BasicPathData> data) : Edge<BasicPathData>(to, data)
	{
	}

	///<summary>
	/// デストラクタ
	///</summary>
	BasicPath::~BasicPath()
	{
	}


	///<summary>
	/// 距離を取得
	///</summary>
	const double BasicPath::distance() const
	{
		return data->get_distance();
	}

	
	///<summary>
	/// 距離を更新
	///</summary>
	void BasicPath::update_distance(const double& distance)
	{
		data->update_distance(distance);
	}
}

