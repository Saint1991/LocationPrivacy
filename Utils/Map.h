#pragma once
#include "NodeCollectionFactory.h"
#include "IdentifiableCollection.h"
#include "RoutingMethod.h"
#include "RoutingTable.h"

namespace Graph
{

	///<summary>
	/// �n�}�Ɋւ��钊�ۃN���X
	/// ��̓I�ȃ}�b�v�͂�����p�����č��
	/// NODE, PATH�ɂ͂��ꂼ��Node, Edge���p�������N���X���g����
	/// PATH�ɂ�BasicPathData���p�����Ă���f�[�^��ێ����Ă���Edge�����g���Ȃ��̂Œ���
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	class Map
	{

	protected:
		std::shared_ptr<const Collection::IdentifiableCollection<Graph::node_id, NODE>> node_collection;
		std::shared_ptr <const Collection::IdentifiableCollection<Graph::node_id, POI>> poi_collection;
		virtual void build_map() = 0;

		///<summary>
		/// �ŒZ�H���i�[���郋�[�e�B���O�e�[�u��
		/// Source����Destination�֍s���̂Ɏ��ɂǂ̃m�[�h�ɍs���K�v�����邩���i�[����D
		/// Map�̍\����initialize���Ăяo�����ۂɍs����
		///</summary>
		std::unique_ptr<RoutingTable const> routing_table;

	public:
		Map();
		virtual ~Map();

		void initialize(std::unique_ptr<RoutingMethod<NODE, PATH>> routing_method);

		node_id get_next_node_of_shortest_path(const node_id& from, const node_id& to) const;
		const std::list<node_id> get_shortest_path(const node_id& source, const node_id& destination) const;
		double calc_necessary_time(const node_id& from, const node_id& to, const double& avg_speed) const;
		bool is_reachable(const node_id& from, const node_id& to, const double& avg_speed, const double& time_limit) const;
		
		const std::list<node_id> get_connecting_nodes(const node_id& id) const;
		std::shared_ptr<NODE const> get_static_node(const node_id& id) const;
	};
}

#include "Map.hpp"
