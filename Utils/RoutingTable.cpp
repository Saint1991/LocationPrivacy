#include "stdafx.h"
#include "RoutingTable.h"

namespace Graph 
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	RoutingTable::RoutingTable(std::unique_ptr<std::vector<std::vector<node_id>>> table, std::unique_ptr<std::unordered_map<node_id, int>> conversion_map) 
		: table(std::move(table)), conversion_map(std::move(conversion_map))
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
		node_id next = table->at(from).at(to);
		return next;
	}

	///<summary>
	/// node_id�̌n��Ōo�H��Ԃ��D
	/// ���B�s�\�ȏꍇ�͍Ō����NOWHERE�ɂȂ�
	/// source == destination�̏ꍇ�͋�̃��X�g���Ԃ����
	///</summary>
	const std::list<node_id> RoutingTable::get_shortest_path(const node_id& source, const node_id& destination) const
	{
		std::list<node_id> shortest_path;
		if (source == destination) return shortest_path;

		node_id iter = source;
		while (iter != destination && iter != NOWHERE) {
			iter = get_next_node_of_shortest_path(iter, destination);
			shortest_path.push_back(iter);
		}
		return std::move(shortest_path);
	}
}

