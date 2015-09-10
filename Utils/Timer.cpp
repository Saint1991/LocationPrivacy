#include "stdafx.h"
#include "Timer.h"

namespace Time
{

	///<summary>
	/// コンストラクタ
	///</summary>
	Timer::Timer() : start_time(INVALID), end_time(INVALID)
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	Timer::~Timer()
	{
	}


	///<summary>
	///計測を開始する
	///</summary>
	void Timer::start()
	{
		time(&start_time);
	}


	///<summary>
	/// 計測を終了する
	///</summary>
	void Timer::end()
	{
		time(&end_time);
	}


	///<summary>
	/// 計測が終了しているかを返す
	///</summary>
	bool Timer::is_finished() const
	{
		return start_time != INVALID && end_time != INVALID;
	}


	///<summary>
	/// 経過時間[sec]を返す
	///</summary>
	time_t Timer::duration() const
	{
		if (start_time == INVALID || end_time == INVALID) return INVALID;
		return end_time - start_time;
	}


	///<summary>
	/// 経過時刻を単位付き文字列で返す
	///</summary>
	const std::string Timer::to_string() const
	{
		if (!is_finished()) return "NOT FINISHED";
		return std::to_string(duration()) + "[sec]";
	}

	#pragma region Export
	const std::string Timer::DURATION = "Duration";


	///<summary>
	/// Export用データの作成
	///</summary>
	std::unordered_map<std::string, std::string> Timer::get_export_data()  const
	{
		std::unordered_map<std::string, std::string> data = {
			{DURATION, std::to_string(duration())}
		};
		return data;
	}
	#pragma endregion
}

