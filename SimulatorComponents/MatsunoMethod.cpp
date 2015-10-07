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
	/// ここが提案手法の核になる部分
	///</summary>
	void MatsunoMethod::decide_dummy_positions()
	{

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

