#include "stdafx.h"
#include "RoutingTable.h"

namespace Graph 
{

	///<summary>
	/// �R���X�g���N�^
	/// routing_table�ɂ�[from�m�[�h�̃C���f�b�N�X][to�m�[�h�̃C���f�b�N�X] => ���s���ׂ��m�[�hID�̍s��
	/// shortest_distance_table�ɂ�[from�m�[�h�̃C���f�b�N�X][to�m�[�h�̃C���f�b�N�X] = �ŒZ���[�g�ɂ����鋗��
	/// conversion�e�[�u����[�m�[�hID] => �m�[�h�̃C���f�b�N�X
	/// ���i�[����Ă��邱�Ƃ�z�肵�Ă���
	///</summary>
	RoutingTable::RoutingTable(std::unique_ptr<std::vector<std::shared_ptr<std::vector<node_id>>>> routing_table, std::unique_ptr<std::vector<std::shared_ptr<std::vector<double>>>> shortest_distance_table, std::unique_ptr<std::unordered_map<node_id, int>> conversion_map)
		: routing_table(std::move(routing_table)), shortest_distance_table(std::move(shortest_distance_table)), conversion_map(std::move(conversion_map))
	{
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	RoutingTable::~RoutingTable()
	{
	}


	///<summary>
	/// from ���� to�֍ŒZ�H�ōs���Ƃ��Ɏ��ɐi�ނׂ��m�[�h��ID���擾
	/// �ڑ����Ă��Ȃ��ꍇ��NOWHERE���Ԃ�
	///</summary>
	node_id RoutingTable::get_next_node_of_shortest_path(const node_id& from, const node_id& to) const
	{
		long index_from = conversion_map->at(from);
		long index_to = conversion_map->at(to);
		node_id next = routing_table->at(index_from)->at(index_to);
		return next;
	}

	///<summary>
	/// node_id�̌n��Ōo�H��Ԃ��D
	/// ���B�s�\�ȏꍇ�͍Ō����NOWHERE�ɂȂ�
	/// source == destination�̏ꍇ�͋�̃��X�g���Ԃ����
	///</summary>
	const std::vector<node_id> RoutingTable::get_shortest_path(const node_id& source, const node_id& destination) const
	{
		std::vector<node_id> shortest_path;
		if (source == destination) return shortest_path;

		node_id iter = source;
		while (iter != destination && iter != NOWHERE) {
			iter = get_next_node_of_shortest_path(iter, destination);
			shortest_path.push_back(iter);
		}
		return std::move(shortest_path);
	}


	///<summary>
	/// from����to�֓��B����ŒZ�o�H�̋�����Ԃ�
	///</summary>
	double RoutingTable::shortest_distance(const node_id& from, const node_id& to) const
	{
		long index_from = conversion_map->at(from);
		long index_to = conversion_map->at(to);
		double shortest_distance = shortest_distance_table->at(index_from)->at(index_to);
		return shortest_distance;
	}



	///<summary>
	/// candidates�̂���from����ł��߂����̂�Ԃ��܂�
	/// candidates���s���ȏꍇ��NO_CONNECTION��Ԃ��܂�
	///</summary>
	node_id RoutingTable::get_nearest_from(const node_id& from, const std::vector<node_id>& candidates) const
	{
		node_id ret = NOWHERE;
		double min_distance = NO_CONNECTION;
		for (std::vector<node_id>::const_iterator iter = candidates.begin(); iter != candidates.end(); iter++) {
			double distance = shortest_distance(from, *iter);
			if (distance < min_distance) {
				min_distance = distance;
				ret = *iter;
			}
		}
		return ret;
	}
}

