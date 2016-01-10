#ifdef UTILS_EXPORTS
#define TIME_UTILITY_API __declspec(dllexport)
#else
#define TIME_UTILITY_API __declspec(dllimport)
#endif
#pragma once

namespace Time
{

	const time_t INVALID = -1;

	class TimeUtility
	{
	private:
		TimeUtility();
		~TimeUtility();

	public:
		TIME_UTILITY_API static const std::string current_timestamp();
		TIME_UTILITY_API static time_t convert_to_unixtimestamp(std::string timestamp);
		TIME_UTILITY_API static std::string timestamp(time_t time);
		TIME_UTILITY_API static std::string date(time_t time);
		TIME_UTILITY_API static int interval(const std::string& time1, const std::string& time2);
		TIME_UTILITY_API static std::string forward(const std::string& time, time_t forward_time);
		TIME_UTILITY_API static std::string mysql_timestamp(time_t time);
	};
}


