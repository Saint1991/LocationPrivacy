#include "stdafx.h"
#include "BasicObserver.h"

namespace Observer
{


	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename TRAJECTORY_TYPE, typename DUMMY_TYPE, typename USER_TYPE>
	BasicObserver<TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>::BasicObserver (
		std::shared_ptr<Map::BasicDbMap const> map,
		std::shared_ptr<Entity::EntityManager<Geography::LatLng, TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE> const> entities,
		double move_speed,
		const std::function<bool(std::shared_ptr<Map::BasicDbMap const>, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, double, long)>& cross_rule
	) : module(std::make_unique<Evaluate::CrossJudgementModule<Map::BasicDbMap, Geography::LatLng, TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>>(map, entities, move_speed, cross_rule)), entities(entities), map(map)
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	template <typename TRAJECTORY_TYPE, typename DUMMY_TYPE, typename USER_TYPE>
	BasicObserver<TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>::~BasicObserver()
	{
	}


	///<summary>
	/// トラジェクトリとその交差関係を基に観測されるすべてのトラジェクトリモデルを作成する
	///</summary>
	template <typename TRAJECTORY_TYPE, typename DUMMY_TYPE, typename USER_TYPE>
	std::shared_ptr<ObservedTrajectoryStructure> BasicObserver<TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>::create_observed_trajectory_structure() const
	{
		std::shared_ptr<ObservedTrajectoryStructure> trajectory_structure = std::make_shared<ObservedTrajectoryStructure>();
		ObservedTrajectoryStructure::base_iterator iter = trajectory_structure->root<ObservedTrajectoryStructure::base_iterator>();

		//まずは交差を考えずに個々の可観測なトラジェクトリ情報を作成する
		std::shared_ptr<Time::TimeSlotManager const> timeslot = entities->read_timeslot();
		timeslot->for_each_time([&](time_t time, long interval, int phase) {

			int depth = phase + 1;

			for (Entity::entity_id id = 0; id <= entities->get_dummy_count(); id++) {

				//着目しているエンティティに応じて親ノードを更新
				std::shared_ptr<Entity::MobileEntity<Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>> const> entity = entities->read_entity_by_id(id);
				if (phase >= 1) {
					Graph::MapNodeIndicator previous_node = entity->read_node_pos_info_of_phase(phase - 1).first;
					iter = trajectory_structure->find_node(previous_node, phase - 1);
				}
				else if (phase == 0) iter->count_up();

				//次の訪問地点をエンティティ数1で子ノードとして接続する
				//既存のノードの場合はエンティティ数をカウントアップする
				Graph::MapNodeIndicator current_node = entity->read_node_pos_info_of_phase(phase).first;
				ObservedTrajectoryStructure::base_iterator child = iter.find_child_if([&current_node](std::shared_ptr<ObservedTrajectoryNode const> node) {
					return *node->data == current_node;
				});
				if (*child == nullptr) {
					std::shared_ptr<ObservedTrajectoryNode> new_node = std::make_shared<ObservedTrajectoryNode>(trajectory_structure->node_count(), depth, std::make_shared<Graph::MapNodeIndicator>(current_node));
					iter.add_child(new_node);
				}
				else {
					child->count_up();
				}
			}
		});

		//交差情報を元に取りうるトラジェクトリの接続関係を追加する
		for (Entity::entity_id id = 0; id <= entities->get_dummy_count(); id++) {

			//着目するエンティティとその交差情報
			std::shared_ptr<Entity::MobileEntity<Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>> const> entity = entities->read_entity_by_id(id);
			std::vector<Evaluate::CrossInfo> cross_infos = module->get_all_cross_info_of_entity(id);

			//交差相手の辿ったノードへのパスがない場合はそれを作成する
			for (std::vector<Evaluate::CrossInfo>::const_iterator cross_info = cross_infos.begin(); cross_info != cross_infos.end(); cross_info++) {

				//交差を作成するtrajectory_nodeを取得する
				int cross_phase = cross_info->phase;
				Graph::MapNodeIndicator cross_node = entity->read_node_pos_info_of_phase(cross_phase).first;
				ObservedTrajectoryStructure::base_iterator iter = trajectory_structure->find_node(cross_node, cross_info->phase);

				//交差相手のノードを取得しtrajectory_nodeにエッジを追加
				for (std::vector<Entity::entity_id>::const_iterator target_entity_id = cross_info->crossing_entities.begin(); target_entity_id != cross_info->crossing_entities.end(); target_entity_id++) {

					//エッジを張る先のノードのIDを取得(交差相手のcross_phase + 1の訪問場所)
					Graph::MapNodeIndicator next_node = entities->read_entity_by_id(id)->read_node_pos_info_of_phase(cross_phase + 1).first;
					Graph::node_id next_node_id = trajectory_structure->find_node_id(next_node, cross_phase + 1);

					if (!iter->is_connecting_to(next_node_id)) iter->connect_to(std::make_shared<Graph::BasicEdge>(next_node_id));
				}
			}
		}
		return trajectory_structure;
	}
}

