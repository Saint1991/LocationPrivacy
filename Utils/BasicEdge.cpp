#include "stdafx.h"
#include "BasicEdge.h"

namespace Graph 
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	BasicEdge::BasicEdge(const node_id& to) : _to(to)
	{
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	BasicEdge::~BasicEdge()
	{
	}


	///<summary>
	/// �ڑ�����擾����
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

