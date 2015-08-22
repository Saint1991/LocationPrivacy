#pragma once

namespace Graph 
{
	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename NODE, typename EDGE>
	WarshallFloyd<NODE, EDGE>::WarshallFloyd() : RoutingMethod()
	{
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename NODE, typename EDGE>
	WarshallFloyd<NODE, EDGE>::~WarshallFloyd()
	{
	}

	template <typename NODE, typename EDGE>
	void WarshallFloyd<NODE, EDGE>::initialize()
	{
		//�m�[�h���̎擾
		node_count = node_collection->size();

		//�C���f�b�N�X�ƃm�[�hID�̕ϊ��}�b�v�̍쐬
		create_conversion_map(node_collection);

		//�����s��̏�����
		initialize_distance_map(node_collection);
	}


	///<summary>
	/// �C���f�b�N�X�ƃm�[�hID�̕ϊ��}�b�v���쐬����
	///</summary>
	template <typename NODE, typename EDGE>
	void WarshallFloyd<NODE, EDGE>::create_conversion_map(std::shared_ptr<const Collection::IdentifiableCollection<NODE const>> node_collection)
	{
		conversion_map = std::make_unique<std::unordered_map<node_id, int>>(node_count);
		std::unique_ptr<std::vector<long>> id_list = node_collection->get_id_list();
		for (std::vector<node_id>::const_iterator iter = id_list->begin(), long index = 0; iter != id_list->end(); iter++, index++) {
			conversion_map->insert({*iter, index});
		}
	}


	///<summary>
	/// node_collection����ɋ����s�������������
	/// ���g���玩�g�ւ̋�����0�C���̗v�f��NO_CONNECTION = DBL_MAX�ŏ��������C
	/// ���̌�ڑ����Ă���m�[�h�Ԃ̋������i�[����
	/// EDGE�����f�[�^��BasicPathData���p�������N���X�ł��邱�Ƃ�z��
	///</summary>
	template <typename NODE, typename EDGE>
	void	WarshallFloyd<NODE, EDGE>::initialize_distance_map(std::shared_ptr<const Collection::IdentifiableCollection<NODE const>> node_collection)
	{
		//�ŏ��͑S�Ă̗v�f��NO_CONNECTION = DBL_MAX�ŏ���������
		distance_map = std::make_unique<std::vector<std::vector<double>>>(node_count, std::vector<double>(node_count, NO_CONNECTION));

		//node_collection���Q�Ƃ��ăm�[�h�Ԃ̋������i�[�C����Ɏ��g�ւ̋�����0�ɂ���
		int node_index = 0;
		node_collection->foreach([&](std::shared_ptr<NODE const> node) {

			distance_map->at(node_index)->at(node_index) = 0;
			
			node->for_each_edge([&](std::shared_ptr<EDGE const> edge) {
				long index_to =conversion_map->at(edge->get_to());
				double distance = edge->get_static_data()->get_distance();
				distance_map->at(node_index)->at(index_to) = distance;
			});
			node_index++;
		});
	}


	///<summary>
	/// ���[�V�����t���C�h�@�ɂ�郋�[�e�B���O�e�[�u���̍쐬
	/// �Q�� (http://dai1741.github.io/maximum-algo-2012/docs/shortest-path/)
	/// nodes���������n����Ă��Ȃ���nullptr���Ԃ�
	///</summary>
	template <typename NODE, typename EDGE>
	std::unique_ptr<RoutingTable const>	WarshallFloyd<NODE>::create_routing_table(std::shared_ptr<const Collection::IdentifiableCollection<NODE const>> node_collection) const
	{
		//�t�B�[���h��node_collection�ւ̎Q�Ƃ��������� (�����v�Z distance���\�b�h�̂���)
		if (node_collection == nullptr) return nullptr;

		//�e�t�B�[���h������������
		initialize();
		
		//�������烋�[�e�B���O�e�[�u���̍쐬���s���C�ŏ��͑S�v�fNOWHERE=-1�ŏ�����
		std::unique_ptr<std::vector<std::vector<node_id>>> routing_table = std::make_unique<std::vector<std::vector<node_id>>>(NODE_COUNT, std::vector<node_id>(NODE_COUNT, NOWHERE));





		return std::move(routing_table);
	}

}
