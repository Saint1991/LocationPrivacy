#pragma once
#include "stdafx.h"
#include "Edge.h"
#include "Node.h"
#include "IdentifiableCollection.h"

namespace Graph 
{

	//�Y���ӏ���������Ȃ��ꍇ�̒l(create���ɂ���ɊY������m�[�h�̓G�X�P�[�v����K�v����Ȃ̂Œ���!)
	const node_id NOWHERE = ULONG_MAX;

	///<summary>
	/// ���[�e�B���O�e�[�u���쐬�̃A���S���Y���p�C���^�[�t�F�[�X
	///</summary>
	template <typename NODE>
	class RoutingMethod
	{	
	public:
		virtual std::unique_ptr<const std::vector<const std::vector<node_id>>> 
			create_routing_table(std::shared_ptr<const Collection::IdentifiableCollection<std::shared_ptr<NODE const>>> node_collection) const = 0;
	};
}
