#include "stdafx.h"
#include "MatsunoMethod.h"

namespace Method
{

	///<summary>
	/// コンストラクタ
	/// これにSimulatorで作成した各種入力への参照を渡す
	///</summary>
	MatsunoMethod::MatsunoMethod(std::shared_ptr<Map::BasicDbMap const> map, std::shared_ptr<User::BasicUser<Geography::LatLng>> user, std::shared_ptr<Requirement::BasicRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager)
		: Framework::IProposedMethod<Map::BasicDbMap, User::BasicUser<Geography::LatLng>, Entity::Dummy<Geography::LatLng>, Requirement::BasicRequirement>(map, user, requirement, time_manager)
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
		Entity::entity_id cross_target = entities->get_min_cross_entity_id();
		int target_phase = -1;

		//ユーザに対する交差
		if (cross_target == 0) {
			std::shared_ptr<User::BasicUser<Geography::LatLng> const> user = entities->get_user();
			target_phase = user->randomly_pick_cross_not_set_phase();
		}
		//ダミーに対する交差
		else {
			std::shared_ptr<Entity::Dummy<Geography::LatLng>> dummy = entities->get_dummy_by_id(cross_target);
			target_phase = dummy->randomly_pick_cross_not_set_phase();
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


	

	///<summary>
	/// 決定した位置を基にMTC等各種評価値を算出する
	///</summary>
	void MatsunoMethod::evaluate()
	{

	}


	///<summary>
	/// 結果のファイルへのエクスポート
	///</summary>
	void MatsunoMethod::export_results()
	{

	}


	///<summary>
	/// 終了処理 (今回はスマートポインタを利用しているので，特にやることはない)
	///</summary>
	void MatsunoMethod::terminate()
	{

	}
}

