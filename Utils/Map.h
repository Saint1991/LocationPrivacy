#ifdef UTILS_EXPORTS
#define MAP_API __declspec(dllexport)
#else
#define MAP_API __declspec(dllimport)
#endif

#pragma once
#include "NodeCollectionFactory.h"
#include "IdentifiableCollection.h"
#include "RoutingMethod.h"

namespace Graph
{

	///<summary>
	/// �n�}�Ɋւ��钊�ۃN���X
	/// ��̓I�ȃ}�b�v�͂�����p�����č��
	/// NODE, PATH�ɂ͂��ꂼ��Node, Edge���p�������N���X���g����
	/// PATH�ɂ�BasicPathData���p�����Ă���f�[�^��ێ����Ă���Edge�����g���Ȃ��̂Œ���
	/// ID ULONG_MAX�̃m�[�h������Ȃ��悤�ɒ���!!

	///</summary>
	template <typename NODE, typename PATH>
	class MAP_API Map
	{

	protected:
		std::shared_ptr<const Collection::IdentifiableCollection<std::shared_ptr<NODE const>>> node_collection;
		virtual void build_map() = 0;

		///<summary>
		/// �ŒZ�H���i�[���郋�[�e�B���O�e�[�u��
		/// Source����Destination�֍s���̂Ɏ��ɂǂ̃m�[�h�ɍs���K�v�����邩���i�[����D
		///</summary>
		std::unique_ptr<const std::vector<const std::vector<node_id>>> routing_table;

	public:
		Map(std::unique_ptr<RoutingMethod<NODE>> routing_method);
		virtual ~Map();

		node_id get_next_node_of_shortest_path(const node_id& from, const node_id& to) const;
		const std::list<node_id> get_shortest_path(const node_id& source, const node_id& destination) const;
		unsigned long calc_necessary_time(const node_id& from, const node_id& to, const double& avg_speed) const;
		bool is_reachable(const node_id& from, const node_id& to, const double& avg_speed, const unsigned int& time_limit) const;
		
		
		const std::list<node_id> get_connecting_nodes(const node_id& id) const;
		std::shared_ptr<const Geography::GeoNode<NODE, PATH>> get_static_node(const node_id& id) const;
		std::shared_ptr<Geography::GeoNode<NODE, PATH>> get_node(const node_id& id);
	};
}

#include "Map.hpp"
