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


	///<summary>
	/// �����s��C���[�e�B���O���ʂ̏�����
	///</summary>
	template <typename NODE, typename EDGE>
	void WarshallFloyd<NODE, EDGE>::initialize(std::shared_ptr<const Collection::IdentifiableCollection<Graph::node_id, NODE>> node_collection)
	{
		//�m�[�h���̎擾
		node_count = node_collection->size();

		//�C���f�b�N�X�ƃm�[�hID�̕ϊ��}�b�v�̍쐬
		create_conversion_map(node_collection);

		//�����s��ƃ��[�e�B���O�e�[�u���̏�����
		//�����s��͑S�Ă̗v�f��NO_CONNECTION = DBL_MAX�ŏ������C���[�e�B���O�e�[�u���͑S�v�fNOWHERE = -1�ŏ�����
		distance_map = std::make_unique<std::vector<std::vector<double>>>(node_count, std::vector<double>(node_count, NO_CONNECTION));
		routing_table = std::make_unique<std::vector<std::vector<node_id>>>(node_count, std::vector<node_id>(node_count, NOWHERE));

		//node_collection���Q�Ƃ��ăm�[�h�Ԃ̋������i�[�C���g�ւ̋�����0�ɂ���
		//���g�ւ̑J�ڂ�SELF = -2�Ƃ���C���[�e�B���O�e�[�u���̓C���f�b�N�X�łȂ��m�[�h��ID���i�[����
		int node_index = 0;
		node_collection->foreach([&](std::shared_ptr<NODE const> node) {

			node->for_each_edge([&](std::shared_ptr<EDGE const> edge) {
				node_id id = edge->get_to();
				long index_to = conversion_map->at(id);
				double distance = edge->get_static_data()->get_distance();
				distance_map->at(node_index).at(index_to) = distance;
				routing_table->at(node_index).at(index_to) = id;
			});
			distance_map->at(node_index).at(node_index) = 0;
			routing_table->at(node_index).at(node_index) = SELF;
			node_index++;

		});
	}


	///<summary>
	/// �C���f�b�N�X�ƃm�[�hID�̕ϊ��}�b�v���쐬���� (�Ƃ肠���������ł͏������x��D�悵�ė������̕ϊ��}�b�v���쐬���Ă��邪�C
	/// ������������D�悷��Ȃ�Е����ɂ��ăC���f�b�N�X��T������悤�ɒ���
	///</summary>
	template <typename NODE, typename EDGE>
	void WarshallFloyd<NODE, EDGE>::create_conversion_map(std::shared_ptr<const Collection::IdentifiableCollection<Graph::node_id, NODE>> node_collection)
	{
		conversion_map = std::make_unique<std::unordered_map<node_id, int>>(node_count);
		reverse_conversion_map = std::make_unique<std::vector<node_id>>(node_count);
		long index = 0;
		node_collection->foreach([&](std::shared_ptr<NODE const> node) {
			node_id id = node->get_id();
			conversion_map->insert(std::make_pair(id, index));
			reverse_conversion_map->at(index) = id;
			index++;
		});
	}


	///<summary>
	/// ���[�V�����t���C�h�@�ɂ�郋�[�e�B���O�e�[�u���̍쐬
	/// �Q�� (http://dai1741.github.io/maximum-algo-2012/docs/shortest-path/)
	/// node_collection���������n����Ă��Ȃ���nullptr���Ԃ�
	///</summary>
	template <typename NODE, typename EDGE>
	std::unique_ptr<RoutingTable const>	WarshallFloyd<NODE, EDGE>::create_routing_table(std::shared_ptr<const Collection::IdentifiableCollection<Graph::node_id, NODE>> node_collection)
	{
		//�t�B�[���h��node_collection�ւ̎Q�Ƃ��������� (�����v�Z distance���\�b�h�̂���)
		if (node_collection == nullptr) return nullptr;

		//�e�t�B�[���h������������
		initialize(node_collection);

		//���[�V�����t���C�h�@�ōŒZ�H���m�肳���Ă���
		for (long source = 0; source < node_count; source++) {
			for (long destination = 0; destination < node_count; destination++) {
				for (long via = 0; via < node_count; via++) {
					
					double distance_src_dest = distance_map->at(source).at(destination);
					double distance_src_via_dest = distance_map->at(source).at(via) + distance_map->at(via).at(destination);
					if (distance_src_via_dest < distance_src_dest) {
						distance_map->at(source).at(destination) = distance_src_via_dest;
						routing_table->at(source).at(destination) = reverse_conversion_map->at(via);
					}

				}
			}
		}

		std::unique_ptr<RoutingTable const> ret = std::make_unique<RoutingTable const>(std::move(routing_table), std::move(distance_map), std::move(conversion_map));
		return std::move(ret);
	}

}
