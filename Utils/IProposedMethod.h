#pragma once
#include "stdafx.h"
#include "BasicRequirement.h"
#include "TimeSlotManager.h"
#include "Timer.h"
#include "FileExporter.h"

namespace Framework
{


	///<summary>
	/// 提案手法を実装する際のテンプレート
	/// これを継承したクラスを作成し，純粋仮想関数を実装する
	///</summary>
	template <typename MAP_TYPE, typename USER_TYPE, typename DUMMY_TYPE, typename REQUIREMENT_TYPE>
	class IProposedMethod
	{
	static_assert(std::is_base_of<Requirement::BasicRequirement, REQUIREMENT_TYPE>::value, "template type REQUIREMENT_TYPE must be derived from BasicRequirement");
	
	protected:
		std::shared_ptr<MAP_TYPE const> map;
		std::shared_ptr<USER_TYPE const> user;
		std::shared_ptr<REQUIREMENT_TYPE const> requirement;
		std::shared_ptr<Time::TimeSlotManager> time_manager;
		std::shared_ptr<std::vector<std::shared_ptr<DUMMY_TYPE>>> dummies;
		std::shared_ptr<Time::Timer> timer;
		

		///<summary>
		/// ここで入力の加工等の初期化処理を行う
		///</summary>
		virtual void initialize() = 0;


		///<summary>
		/// 上記のフィールドに基づいて各フェーズにおけるダミーの位置を決定する
		///</summary>
		virtual void decide_dummy_positions() = 0;


		///<summary>
		/// 決定したダミーの位置情報に基づいて，MTCや匿名領域の計算を行う
		///</summary>
		virtual void evaluate() = 0;


		///<summary>
		/// 計算した評価値のファイルへのエクスポートを行う
		///</summary>
		virtual void export_results() = 0;


		///<summary>
		/// 終了処理
		/// 今回はスマートポインタを用いているので解放するべきリソース等はほぼないはず
		///</summary>
		virtual void terminate() = 0;

		void for_each_dummy(const std::function<void(int, std::shared_ptr<DUMMY_TYPE>)>& execute_function);

	public:
		IProposedMethod(std::shared_ptr<MAP_TYPE const> map, std::shared_ptr<USER_TYPE const> user, std::shared_ptr<REQUIREMENT_TYPE const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager);
		virtual ~IProposedMethod();

		void run();
	};
}

#include "IProposedMethod.hpp"
