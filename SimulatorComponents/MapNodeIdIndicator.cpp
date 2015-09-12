#include "stdafx.h"
#include "MapNodeIdIndicator.h"

namespace Map
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	MapNodeIdIndicator::MapNodeIdIndicator(Graph::node_id id, NodeType type) : _id(id), _type(type)
	{
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	MapNodeIdIndicator::~MapNodeIdIndicator()
	{
	}


	///<summary>
	/// �w������ID�̎擾
	///</summary>
	Graph::node_id MapNodeIdIndicator::id() const
	{
		return _id;
	}


	///<summary>
	/// �w�������m�[�h�̎�ʂ��擾
	///</summary>
	NodeType MapNodeIdIndicator::type() const
	{
		return _type;
	}
}

