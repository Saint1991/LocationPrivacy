#pragma once

namespace Graph 
{
	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename NODE>
	WarshallFloyd<NODE>::WarshallFloyd() : RoutingMethod()
	{
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename NODE>
	WarshallFloyd<NODE>::~WarshallFloyd()
	{
	}


	///<summary>
	/// ���[�V�����t���C�h�@�ɂ�郋�[�e�B���O�e�[�u���̍쐬
	/// �Q�� (http://dai1741.github.io/maximum-algo-2012/docs/shortest-path/)
	/// nodes���������n����Ă��Ȃ���nullptr���Ԃ�
	///</summary>
	template <typename NODE>
	std::unique_ptr<const std::vector<const std::vector<node_id>>> 
		WarshallFloyd<NODE>::create_routing_table(std::shared_ptr<const Collection::IdentifiableCollection<std::shared_ptr<NODE const>>> nodes) const
	{

		//�t�B�[���h��node_collection�ւ̎Q�Ƃ��������� (�����v�Z distance���\�b�h�̂���)
		if (nodes == nullptr) return nullptr;
		node_collection = nodes;

		//�m�[�h���̎擾
		const node_id NODE_COUNT = node_collection->size();

		//�S�l���ŏ���NOWHERE�ŏ�����
		std::unique_ptr<std::vector<std::vector<node_id>>> routing_table = 
			std::make_unique<std::vector<std::vector<node_id>>>(NODE_COUNT, std::vector<node_id>(NODE_COUNT, NOWHERE));


		


		return std::move(routing_table);
	}
}
