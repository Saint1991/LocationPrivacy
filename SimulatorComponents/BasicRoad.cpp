#include "stdafx.h"
#include "BasicRoad.h"


namespace Map
{
	///<summary>
	/// コンストラクタ
	///</summary>
	BasicRoad::BasicRoad(Graph::node_id to, double distance) : Edge<Graph::BasicPathData>(to, std::make_shared<Graph::BasicPathData>(distance))
	{
	}

	///<summary>
	/// デストラクタ
	///</summary>
	BasicRoad::~BasicRoad()
	{
	}


	///<summary>
	/// 距離を取得
	///</summary>
	double BasicRoad::distance() const
	{
		return data->get_distance();
	}


	///<summary>
	/// 距離を更新
	///</summary>
	void BasicRoad::update_distance(const double& distance)
	{
		data->update_distance(distance);
	}
}

