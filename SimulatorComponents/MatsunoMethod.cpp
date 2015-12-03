#include "stdafx.h"
#include "MatsunoMethod.h"

namespace Method
{

	///<summary>
	/// コンストラクタ
	/// これにSimulatorで作成した各種入力への参照を渡す
	///</summary>
	MatsunoMethod::MatsunoMethod(std::shared_ptr<Map::BasicDbMap const> map, std::shared_ptr<User::BasicUser<Geography::LatLng>> user, std::shared_ptr<Requirement::BasicRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager)
		: Framework::IProposedMethod<Map::BasicDbMap, User::BasicUser<Geography::LatLng>, Entity::Dummy<Geography::LatLng>, Requirement::BasicRequirement, Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>>(map, user, requirement, time_manager)
	{
	}

	///<summary>
	/// デストラクタ
	///</summary>
	MatsunoMethod::~MatsunoMethod()
	{
	}


	///<summary>
	/// 初期化 (今回は特にやることはない)
	///</summary>
	void MatsunoMethod::initialize()
	{

	}

	///<summary>
	/// IDがidのダミーに対する共有地点(交差)の設定
	///</summary>
	void MatsunoMethod::set_crossing_points(Entity::entity_id id, std::shared_ptr<Entity::Dummy<Geography::LatLng>> dummy)
	{
		
		//交差回数が少ないエンティティを優先的に交差対象にして繰り返す
		std::vector<std::pair<Entity::entity_id, int>> entity_list_order_by_cross = entities->get_entity_id_list_order_by_cross_count();
		for (std::vector<std::pair<Entity::entity_id, int>>::const_iterator iter = entity_list_order_by_cross.begin(); iter != entity_list_order_by_cross.end(); iter++) {
			
			//未生成のダミーのものはスキップ
			if (iter->first > id) continue;

			//交差対象のID
			Entity::entity_id cross_target = iter->second;
			int target_phase = INVALID;

			//BasicUserはDummyを継承しているのでポインタ代入は可能
			std::shared_ptr<Entity::Dummy<Geography::LatLng>> target = cross_target == 0 ? entities->get_user() : entities->get_dummy_by_id(cross_target);
			std::vector<int> target_phases = target->find_cross_not_set_phases();
			
			//交差未設定の時刻からランダムに共有地点設定を試みる
			std::random_device device;
			std::mt19937_64 generator(device());
			std::shuffle(target_phases.begin(), target_phases.end(), generator);

			for (std::vector<int>::const_iterator target_phase = target_phases.begin(); target_phase != target_phases.end(); target_phase++) {
				if (*target_phase == 0) continue;


			}
		}


		
	}


	///<summary>
	/// IDがidのダミーに対する基準地点(匿名領域確保)の設定
	///</summary>
	void MatsunoMethod::set_expanding_points(Entity::entity_id id, std::shared_ptr<Entity::Dummy<Geography::LatLng>> dummy)
	{

	}


	///<summary>
	/// ここが提案手法の核になる部分
	///</summary>
	void MatsunoMethod::decide_dummy_positions()
	{
		entities->for_each_dummy([&](Entity::entity_id id, std::shared_ptr<Entity::Dummy<Geography::LatLng>> dummy) {
			set_crossing_points(id, dummy);
			set_expanding_points(id, dummy);
		});
	}
}

