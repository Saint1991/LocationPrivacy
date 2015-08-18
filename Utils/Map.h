#ifdef UTILS_EXPORTS
#define MAP_API __declspec(dllexport)
#else
#define MAP_API __declspec(dllimport)
#endif

#pragma once
#include "NodeCollectionFactory.h"
#include "IdentifiableCollection.h"
#include "RoutingMethod.h"

namespace Geography 
{

	//�Y���ӏ���������Ȃ��ꍇ�̒l(create���ɂ���ɊY������m�[�h�̓G�X�P�[�v����K�v����Ȃ̂Œ���!)
	const Graph::node_id NOWHERE = ULONG_MAX;

	///<summary>
	/// �n�}�Ɋւ��钊�ۃN���X
	/// ��̓I�ȃ}�b�v�͂�����p�����č��
	/// ID ULONG_MAX�͗\��ς݂Ŏg���Ȃ��̂Œ���
	///</summary>
	/*template <typename GEONODEDATA, typename PATHDATA>
	class Map
	{
		
	protected:
		std::shared_ptr<const Collection::IdentifiableCollection<std::shared_ptr<const Geography::GeoNode<GEONODEDATA, PATHDATA>>>> node_collection;
		virtual void build_map() = 0;
		virtual void create_routing_table(std::unique_ptr<Graph::RoutingMethod<GEONODEDATA, PATHDATA>> routing_method);


		///<summary>
		/// �ŒZ�H���i�[���郋�[�e�B���O�e�[�u��
		/// Source����Destination�֍s���̂Ɏ��ɂǂ̃m�[�h�ɍs���K�v�����邩���i�[����D
		///</summary>
		std::unique_ptr<const std::vector<const std::vector<Graph::node_id>>> routing_table;

	public:
		Map(std::unique_ptr<Graph::RoutingMethod<GEONODEDATA, PATHDATA>> routing_method);
		virtual ~Map();

		const unsigned long calc_necessary_time(const Graph::node_id& from, const Graph::node_id& to, const double& avg_speed) const;
		const bool is_reachable(const Graph::node_id& from, const Graph::node_id& to, const double& avg_speed, const unsigned int& time_limit) const;
		const Graph::node_id get_next_node_of_shortest_path(const Graph::node_id& from, const Graph::node_id& to) const;
		const std::list<Graph::node_id> get_shortest_path(const Graph::node_id& from, const Graph::node_id& to) const;
		const std::list<Graph::node_id> get_connecting_nodes(const Graph::node_id& id) const;
		std::shared_ptr<const Geography::GeoNode<GEONODEDATA, PATHDATA>> get_static_node(const Graph::node_id& id) const;
		std::shared_ptr<Geography::GeoNode<GEONODEDATA, PATHDATA>> get_node(const Graph::node_id& id);
	};
	
	#include "Map.hpp"
	*/
}


