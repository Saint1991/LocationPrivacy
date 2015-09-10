#ifdef UTILS_EXPORTS
#define TIMER_API __declspec(dllexport)
#else
#define TIMER_API __declspec(dllimport)
#endif

#pragma once
#include "Serializable.h"
#include "FileExportable.h"

namespace Time
{



	///<summary>
	/// 実行速度計測用タイマークラス
	///</summary>
	class TIMER_API Timer : public Serializable, public IO::FileExportable
	{
	private:
		static const time_t INVALID = -1;
		time_t start_time;
		time_t end_time;

	public:
		static const std::string DURATION;

		Timer();
		virtual ~Timer();

		void start();
		void end();
		bool is_finished() const;
		time_t duration() const;
		const std::string to_string() const;
		std::unordered_map<std::string, std::string> get_export_data()  const;
	};
}


