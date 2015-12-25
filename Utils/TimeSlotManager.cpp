#include "stdafx.h"
#include "TimeSlotManager.h"

namespace Time
{

	///<summary>
	/// コンストラクタ
	/// UNIX_TIMESTAMPの系列を渡して経過時刻のリストに変換する
	/// relative_to_firstをfalseにすると生のUNIX_TIMESTAMPの値をそのまま保持する
	///</summary>
	TimeSlotManager::TimeSlotManager(std::unique_ptr<std::vector<time_t>> timeslots, bool relative_to_first)
	{
		std::sort(timeslots->begin(), timeslots->end());
		if (!relative_to_first) this->timeslots = std::move(timeslots);
		else {
			this->timeslots = std::make_unique<std::vector<time_t>>(timeslots->size());
			if (timeslots->size() == 0) return;

			time_t first = timeslots->at(0);
			for (int i = 0; i < timeslots->size(); i++) {
				this->timeslots->at(i) = timeslots->at(i) - first;
			}
		}
	}


	///<summary>
	/// コンストラクタ
	/// 文字列のTIMESTAMPを利用する場合
	/// 不正な値は除去されるので注意
	///</summary>
	TimeSlotManager::TimeSlotManager(std::unique_ptr<std::vector<std::string>> timeslots, bool relative_to_first)
	{
		std::unique_ptr<std::vector<time_t>> unix_timeslots = std::make_unique<std::vector<time_t>>();
		for (int i = 0; i < timeslots->size(); i++) {
			time_t time = TimeUtility::convert_to_unixtimestamp(timeslots->at(i));
			unix_timeslots->push_back(time);
		}

		std::sort(unix_timeslots->begin(), unix_timeslots->end());
		if (!relative_to_first) this->timeslots = std::move(unix_timeslots);
		else {
			this->timeslots = std::make_unique<std::vector<time_t>>(unix_timeslots->size());
			if (this->timeslots->size() == 0) return;

			time_t first = unix_timeslots->at(0);
			for (int i = 0; i < unix_timeslots->size(); i++) {
				this->timeslots->at(i) = unix_timeslots->at(i) - first;
			}
		}
	}


	///<summary>
	/// デストラクタ
	///</summary>
	TimeSlotManager::~TimeSlotManager()
	{
	}

	///<summary>
	/// 各時刻についてexecute_functionを実行する
	/// 引数は時刻, 1つ前のPhaseからの経過時間，現在のPhase
	///</summary>
	void TimeSlotManager::for_each_time(const std::function<void(time_t, long, int)>& execute_function) const
	{
		if (timeslots->size() == 0) return;
		execute_function(timeslots->at(0), 0, 0);
		for (int phase = 1; phase < timeslots->size(); phase++) {
			long duration = timeslots->at(phase) - timeslots->at(phase - 1);
			execute_function(timeslots->at(phase), duration, phase);
		}
	}


	///<summary>
	/// 指定したPhaseにおける時刻を取得する
	///</summary>
	time_t TimeSlotManager::time_of_phase(int phase) const
	{
		return timeslots->at(phase);
	}


	///<summary>
	/// timeをもとにphaseを取得します
	/// 値がtimeのエントリが存在しない場合はINVALID=-1を返します
	///</summary>
	size_t TimeSlotManager::find_phase_of_time(time_t time) const
	{
		std::vector<time_t>::iterator iter = std::find(timeslots->begin(), timeslots->end(), time);
		size_t index = std::distance(timeslots->begin(), iter);
		if (index == timeslots->size()) return INVALID;
		return index;
	}


	///<summary>
	/// Phase数の合計を取得する
	///</summary>
	size_t TimeSlotManager::phase_count() const
	{
		return timeslots->size();
	}

	///<summary>
	/// 最終Phaseを取得する
	///</summary>
	size_t TimeSlotManager::last_phase() const
	{
		return phase_count() - 1;
	}
}

