#include "stdafx.h"
#include "SemanticTrajectory.h"

namespace Graph
{

	#pragma region SemanticTrajectoryState

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	SemanticTrajectoryState<POSITION_TYPE>::SemanticTrajectoryState(time_t time, const category_id& category, std::shared_ptr<POSITION_TYPE const> position, const std::string& venue_name, const std::string& category_name)
		: TrajectoryState<POSITION_TYPE>(time, position, venue_name), category(category), category_name(category_name)
	{

	}

	///<summary>
	/// ファイル出力用データ
	///</summary>
	template <typename POSITION_TYPE>
	std::unordered_map<std::string, std::string> SemanticTrajectoryState<POSITION_TYPE>::SemanticTrajectoryState::get_export_data() const
	{
		std::unordered_map<std::string, std::string> ret = TrajectoryState<POSITION_TYPE>::get_export_data();
		ret.insert(std::make_pair(CATEGORY, category));
		ret.insert(std::make_pair(CATEGORY_NAME, category_name));
		return ret;
	}

	#pragma endregion SemanticTrajectoryState


	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	SemanticTrajectory<POSITION_TYPE>::SemanticTrajectory(std::shared_ptr<Time::TimeSlotManager const> timeslot) 
		: Trajectory<POSITION_TYPE>(timeslot), category_sequence(std::make_shared<Collection::Sequence<category_id>>(timeslot->phase_count())), category_names(std::make_shared<std::vector<std::string>>(timeslot->phase_count()))
	{
	}


	///<summary>
	/// コンストラクタ
 	///</summary>
	template <typename POSITION_TYPE>
	SemanticTrajectory<POSITION_TYPE>::SemanticTrajectory(
		std::shared_ptr<Time::TimeSlotManager const> timeslot, 
		std::shared_ptr<std::vector<Graph::MapNodeIndicator>> node_ids,
		std::shared_ptr<std::vector<std::shared_ptr<POSITION_TYPE const>>> positions, 
		std::shared_ptr<Collection::Sequence<category_id>> category_sequence,
		std::shared_ptr<std::vector<std::string>> venue_names, 
		std::shared_ptr<std::vector<std::string>> category_names)
		: Trajectory<POSITION_TYPE>(timeslot, node_ids, positions, venue_names), category_sequence(category_sequence), category_names(category_names)
	{

	}


	///<summary>
	/// デストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	SemanticTrajectory<POSITION_TYPE>::~SemanticTrajectory()
	{
	}


	///<summary>
	/// 指定したPhaseにおけるカテゴリを設定する
	///</summary>
	template <typename POSITION_TYPE>
	bool SemanticTrajectory<POSITION_TYPE>::set_category_of_phase(int phase, const std::string& category_id, const std::string& category_name)
	{
		if (phase < 0 || category_sequence->size() <= phase) return false;
		category_sequence->at(phase) = category_id;
		category_names->at(phase) = category_name;
	}



	///<summary>
	/// 指定した時刻におけるカテゴリを設定する
	///</summary>
	template <typename POSITION_TYPE>
	bool SemanticTrajectory<POSITION_TYPE>::set_category_at(time_t time, const std::string& category_id, const std::string& category_name)
	{
		int phase = timeslot->find_phase_of_time(time);
		return set_category_of_phase(phase, category_id, category_name);
	}


	///<summary>
	/// 指定したPhaseにおけるカテゴリIDを返す
	/// 不正なPhaseの場合は空文字列を返す
	///</summary>
	template <typename POSITION_TYPE>
	category_id SemanticTrajectory<POSITION_TYPE>::category_of_phase(int phase) const
	{
		if (phase < 0 || category_sequence->size() <= phase) return "";
		return category_sequence->at(phase);
	}


	///<summary>
	/// 指定した時刻におけるカテゴリIDを返す
	/// 不正な時刻の場合は空文字列を返す
	///</summary>
	template <typename POSITION_TYPE>
	category_id SemanticTrajectory<POSITION_TYPE>::category_at(time_t time) const
	{
		int phase = timeslot->find_phase_of_time(time);
		return category_of_phase(phase);
	}


	///<summary>
	/// PhaseにおけるSemanticTrajectoryStateを取得する
	///</summary>
	template <typename POSITION_TYPE>
	std::shared_ptr<Graph::SemanticTrajectoryState<POSITION_TYPE> const> SemanticTrajectory<POSITION_TYPE>::read_semantic_state_of_phase(int phase) const
	{
		time_t time = timeslot->time_of_phase(phase);
		std::shared_ptr<POSITION_TYPE const> position = positions->at(phase);
		std::string venue_name = venue_names->at(phase);
		std::string category_id = category_sequence->at(phase);
		std::string category_name = category_names->at(phase);
		return std::make_shared<SemanticTrajectoryState<POSITION_TYPE>>(time, category_id, position, venue_name, category_name);
	}


	///<summary>
	/// 各時刻における状態について繰り返し実行する
	///</summary>
	template <typename POSITION_TYPE>
	void SemanticTrajectory<POSITION_TYPE>::foreach(const std::function<void(int, time_t, std::shared_ptr<POSITION_TYPE const>, const category_id&)>& execute_function) const
	{
		timeslot->for_each_time([&](time_t time, long interval, int phase) {
			std::shared_ptr<POSITION_TYPE const> position = positions->at(phase);
			category_id category = category_sequence->at(phase);
			execute_function(phase, time, position, category);
		});
	}


	///<summary>
	///各phase，前Phaseからの経過時刻, Stateについてexecute_functionを実行する
	///</summary>
	template <typename POSITION_TYPE>
	void SemanticTrajectory<POSITION_TYPE>::foreach_state(const std::function<void(int, long, const Graph::MapNodeIndicator&, std::shared_ptr<SemanticTrajectoryState<POSITION_TYPE> const>)>& execute_function) const
	{
		timeslot->for_each_time([&](time_t time, long interval, int phase) {
			std::shared_ptr<POSITION_TYPE const> position = positions->at(phase);
			category_id category = category_sequence->at(phase);
			std::string venue_name = venue_names->at(phase);
			std::string category_name = category_names->at(phase);
			Graph::MapNodeIndicator visited_node = visited_node_ids->at(phase);
			execute_function(phase, interval, visited_node, std::make_shared<SemanticTrajectoryState<POSITION_TYPE> const>(time, category, position, venue_name, category_name));
		});
	}

	///<summary>
	/// from_phaseからto_phaseまでのカテゴリシークエンスを取り出す
	///</summary>
	template <typename POSITION_TYPE>
	Collection::Sequence<category_id> SemanticTrajectory<POSITION_TYPE>::get_category_sequence(int from_phase, int to_phase) const
	{
		if (to_phase == -1) to_phase = phase_count() - 1;
		return category_sequence->subsequence(from_phase, to_phase);
	}



	///<summary>
	/// ファイルエクスポート用トラジェクトリデータを取得する
	///</summary>
	template <typename POSITION_TYPE>
	std::list<std::shared_ptr<IO::FileExportable const>> SemanticTrajectory<POSITION_TYPE>::get_export_data() const
	{
		std::list<std::shared_ptr<IO::FileExportable const>> ret;
		timeslot->for_each_time([this, &ret](time_t time, long interval, int phase) {
			std::shared_ptr<POSITION_TYPE const> position = positions->at(phase);
			category_id category = category_sequence->at(phase);
			std::string venue_name = venue_names->at(phase);
			std::string category_name = category_names->at(phase);
			std::shared_ptr<IO::FileExportable const> data = std::make_shared<SemanticTrajectoryState<POSITION_TYPE> const>(time, category, position, venue_name, category_name);
			ret.push_back(data);
		});
		return ret;
	}

}
