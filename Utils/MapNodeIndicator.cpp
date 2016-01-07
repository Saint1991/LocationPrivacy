#include "stdafx.h"
#include "MapNodeIndicator.h"

namespace Graph
{

	
	///<summary>
	/// �R���X�g���N�^
	///</summary>
	MapNodeIndicator::MapNodeIndicator() : _id(INVALID), _type(NodeType::INVALID)
	{

	}

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	MapNodeIndicator::MapNodeIndicator(Graph::node_id id, NodeType type) : _id(id), _type(type)
	{
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	MapNodeIndicator::~MapNodeIndicator()
	{
	}


	///<summary>
	/// �w������ID�̎擾
	///</summary>
	Graph::node_id MapNodeIndicator::id() const
	{
		return _id;
	}


	///<summary>
	/// �w�������m�[�h�̎�ʂ��擾
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
	/// �t�@�C���o�͌����̃n�b�V���f�[�^���쐬���܂��D
	///</summary>
	std::unordered_map<std::string, std::string> MapNodeIndicator::get_export_data() const
	{
		std::unordered_map<std::string, std::string> ret = {
			{ NODE_ID, std::to_string(_id) },
			{ NODE_TYPE, std::to_string(_type) }
		};
		return ret;
	}

	#pragma endregion Export�֌W
}

