#include "stdafx.h"
#include "BasicPath.h"


namespace Graph
{
	///<summary>
	/// �R���X�g���N�^
	///</summary>
	BasicPath::BasicPath(const node_id& to, std::shared_ptr<BasicPathData> data) : Edge<BasicPathData>(to, data)
	{
	}

	///<summary>
	/// �f�X�g���N�^
	///</summary>
	BasicPath::~BasicPath()
	{
	}


	///<summary>
	/// �������擾
	///</summary>
	const double BasicPath::distance() const
	{
		return data->get_distance();
	}

	
	///<summary>
	/// �������X�V
	///</summary>
	void BasicPath::update_distance(const double& distance)
	{
		data->update_distance(distance);
	}
}

