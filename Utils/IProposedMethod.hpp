
namespace Framework
{

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename MAP_TYPE, typename USER_TYPE, typename DUMMY_TYPE, typename REQUIREMENT_TYPE>
	IProposedMethod<MAP_TYPE, USER_TYPE, DUMMY_TYPE, REQUIREMENT_TYPE>::IProposedMethod(std::shared_ptr<MAP_TYPE const> map, std::shared_ptr<USER_TYPE const> user, std::shared_ptr<REQUIREMENT_TYPE const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager)
		: map(map), user(user), requirement(requirement)
	{
		size_t dummy_num = this->requirement->dummy_num;
		dummies = std::make_shared<std::vector<DUMMY_TYPE>>(dummy_num);
	}


	///<summary>
	/// デストラクタ
	///</summary>
	template <typename MAP_TYPE, typename USER_TYPE, typename DUMMY_TYPE, typename REQUIREMENT_TYPE>
	IProposedMethod<MAP_TYPE, USER_TYPE, DUMMY_TYPE, REQUIREMENT_TYPE>::~IProposedMethod()
	{
	}


	///<summary>
	/// 各ダミーについてexecute_functionを実行するユーティリティ
	///</summary>
	template <typename MAP_TYPE, typename USER_TYPE, typename DUMMY_TYPE, typename REQUIREMENT_TYPE>
	void IProposedMethod<MAP_TYPE, USER_TYPE, DUMMY_TYPE, REQUIREMENT_TYPE>::for_each_dummy(const std::function<void(int, std::shared_ptr<DUMMY_TYPE>)>& execute_function)
	{
		for (std::vector<std::shared_ptr<DUMMY_TYPE>>::iterator iter = dummies->begin(); iter != dummies->end(); iter++) {
			int dummy_id = (*iter)->get_id();
			execute_function(dummy_id, *iter);
		}
	}

	///<summary>
	/// ProposedMethodの実行
	///</summary>
	template <typename MAP_TYPE, typename USER_TYPE, typename DUMMY_TYPE, typename REQUIREMENT_TYPE>
	void IProposedMethod<MAP_TYPE, USER_TYPE, DUMMY_TYPE, REQUIREMENT_TYPE>::run() 
	{

		//ここで実行時間の計測を開始
		timer->start();

		//初期化
		initialize();

		//ここが実行部分(各時刻のダミー位置を計算する)
		decide_dummy_positions();

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

