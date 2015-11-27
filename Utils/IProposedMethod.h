#pragma once
#include "stdafx.h"
#include "BasicRequirement.h"
#include "TimeSlotManager.h"
#include "Timer.h"
#include "FileExporter.h"
#include "EntityManager.h"
#include "SemanticTrajectory.h"
#include "Trajectory.h"

namespace Framework
{

	///<summary>
	/// 提案手法を実装する際のテンプレート
	/// これを継承したクラスを作成し，純粋仮想関数を実装する
	///</summary>
	template <typename MAP_TYPE, typename USER_TYPE, typename DUMMY_TYPE, typename REQUIREMENT_TYPE, typename POSITION_TYPE = Geography::LatLng, typename TRAJECTORY_TYPE = Graph::SemanticTrajectory<POSITION_TYPE>>
	class IProposedMethod
	{
	static_assert(std::is_base_of<Requirement::BasicRequirement, REQUIREMENT_TYPE>::value, "template type REQUIREMENT_TYPE must be derived from BasicRequirement");
	
	protected:
		std::shared_ptr<MAP_TYPE const> map;
		std::shared_ptr<REQUIREMENT_TYPE const> requirement;
		std::shared_ptr<Time::TimeSlotManager const> time_manager;
		std::shared_ptr<Entity::EntityManager<DUMMY_TYPE, USER_TYPE, POSITION_TYPE>> entities;
		std::shared_ptr<Time::Timer> timer;

		//このfunctionに各回が終わった際の処理を記述する
		std::function<void(std::shared_ptr<Entity::EntityManager<DUMMY_TYPE, USER_TYPE, POSITION_TYPE>>, std::shared_ptr<REQUIREMENT_TYPE const>, std::shared_ptr<Time::Timer>)> execution_callback = nullptr;

		///<summary>
		/// 入力の加工等の初期化処理
		///</summary>
		virtual void initialize() = 0;


		///<summary>
		/// 上記のフィールドに基づいて各フェーズにおけるダミーの位置を決定する
		///</summary>
		virtual void decide_dummy_positions() = 0;


		///<summary>
		/// 終了処理
		/// 登録されていれば，execution_callbackを呼び出す
		/// このコールバックで1トラジェクトリ分の評価やそのエクスポートを行う
		///</summary>
		void terminate();

	public:
		IProposedMethod(std::shared_ptr<MAP_TYPE const> map, std::shared_ptr<USER_TYPE> user, std::shared_ptr<REQUIREMENT_TYPE const> requirement, std::shared_ptr<Time::TimeSlotManager const> time_manager);
		virtual ~IProposedMethod();
		void set_execution_callback(const std::function<void(std::shared_ptr<Entity::EntityManager<DUMMY_TYPE, USER_TYPE, POSITION_TYPE>>, std::shared_ptr<REQUIREMENT_TYPE const>, std::shared_ptr<Time::Timer>)>& callback);
		void run();
	};


	class TrajectoryNotFoundException : public std::runtime_error
	{
	public:
		TrajectoryNotFoundException(const std::string& what) : std::runtime_error(what){	}
	};
}

#include "IProposedMethod.hpp"
