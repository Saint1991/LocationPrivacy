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
	/// �R���X�g���N�^
	///</summary>
	BasicRoad::BasicRoad(Graph::node_id to, std::shared_ptr<Graph::BasicPathData> data) : Edge<Graph::BasicPathData>(to, data)
	{
	}

	///<summary>
	/// �R�s�[�R���X�g���N�^
	///</summary>
	BasicRoad::BasicRoad(const BasicRoad& road) : Edge<Graph::BasicPathData>(road.get_to(), std::make_shared<Graph::BasicPathData>(road.data->get_distance()))
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

