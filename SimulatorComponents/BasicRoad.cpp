#include "stdafx.h"
#include "BasicRoad.h"


namespace Map
{
	///<summary>
	/// �R���X�g���N�^
	///</summary>
	BasicRoad::BasicRoad(Graph::node_id to, double distance) : Edge<Graph::BasicPathData>(to, std::make_shared<Graph::BasicPathData>(distance))
	{
	}

	///<summary>
	/// �f�X�g���N�^
	///</summary>
	BasicRoad::~BasicRoad()
	{
	}


	///<summary>
	/// �������擾
	///</summary>
	double BasicRoad::distance() const
	{
		return data->get_distance();
	}


	///<summary>
	/// �������X�V
	///</summary>
	void BasicRoad::update_distance(const double& distance)
	{
		data->update_distance(distance);
	}
}

