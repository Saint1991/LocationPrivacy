#pragma once
#include "stdafx.h"
#include "Edge.h"
#include "Node.h"
#include "IdentifiableCollection.h"
#include "RoutingTable.h"

namespace Graph 
{

	///<summary>
	/// ���[�e�B���O�e�[�u���쐬�̃A���S���Y���p�C���^�[�t�F�[�X
	/// NODE�ɂ�BasicPathData�̔h���N���X���G�b�W�Ɏ���Node�N���X���p�������N���X�������ĂȂ��̂Œ���
	///</summary>
	template <typename NODE, typename EDGE>
	class RoutingMethod
	{		
	public:
		virtual std::unique_ptr<RoutingTable const> 
			create_routing_table(std::shared_ptr<const Collection::IdentifiableCollection<NODE const>> node_collection) const = 0;
	};
}