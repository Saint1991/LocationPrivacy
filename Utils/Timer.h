#ifdef UTILS_EXPORTS
#define TIMER_API __declspec(dllexport)
#else
#define TIMER_API __declspec(dllimport)
#endif

#pragma once
#include "Serializable.h"

namespace Time
{



	///<summary>
	/// 実行速度計測用タイマークラス
	///</summary>
	class Timer : public Serializable
	{
	private:
		static const time_t INVALID = -1;
		time_t start_time;
		time_t end_time;

	public:
		Timer();
		virtual ~Timer();

		void start();
		void end();
		bool is_finished() const;
		time_t duration() const;
		const std::string to_string() const;
	};
}


