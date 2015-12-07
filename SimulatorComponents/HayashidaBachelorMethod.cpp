#include "stdafx.h"
#include "HayashidaBachelorMethod.h"

namespace Method
{

	///<summary>
	/// コンストラクタ
	/// これにSimulatorで作成した各種入力への参照を渡す
	///</summary>
	HayashidaBachelorMethod::HayashidaBachelorMethod(std::shared_ptr<Map::BasicDbMap const> map, std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> user, std::shared_ptr<Requirement::KatoMethodRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager)
		: KatoMasterMethod(map, user, requirement, time_manager),
		predicted_user(nullptr)
	{
	}

	///<summary>
	/// デストラクタ
	///</summary>
	HayashidaBachelorMethod::~HayashidaBachelorMethod()
	{
	}

	///<summary>
	/// 初期化 (今回は特にやることはない)
	///</summary>
	void HayashidaBachelorMethod::initialize()
	{
		//ユーザの動きの変更→新しく作る．
	}


	///<summary>
	/// ここが提案手法の核になる部分.ダミーの行動を修正する
	///</summary>
	void HayashidaBachelorMethod::adjust_dummy_positions()
	{
		
	}


	///<summary>
	/// 決定した位置を基にMTC等各種評価値を算出する
	///</summary>
	void HayashidaBachelorMethod::evaluate()
	{

	}


	///<summary>
	/// 結果のファイルへのエクスポート
	///</summary>
	void HayashidaBachelorMethod::export_results()
	{

	}


	///<summary>
	/// 終了処理 (今回はスマートポインタを利用しているので，特にやることはない)
	///</summary>
	void HayashidaBachelorMethod::terminate()
	{

	}


	void HayashidaBachelorMethod::run()
	{
		//ここで実行時間の計測を開始
		timer->start();

		//初期化
		initialize();

		//ここが実行部分(加藤さん卒論手法[Kato 13])
		decide_dummy_positions();

		//ここでユーザの行動の予測やダミーの行動を修正する(林田さん卒論手法[Hayashida 14])
		adjust_dummy_positions();

		//ここで計測を終了
		timer->end();

		//設定したダミー，ユーザの位置を基にMTCなどの評価指標を計算する
		evaluate();

		//実行時間以外のエクスポート
		export_results();

		//終了処理
		terminate();
	}


}
