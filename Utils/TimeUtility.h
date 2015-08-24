#ifdef UTILS_EXPORTS
#define TIME_UTILITY_API __declspec(dllexport)
#else
#define TIME_UTILITY_API __declspec(dllimport)
#endif
#pragma once

namespace Time
{
	class TimeUtility
	{
	private:
		TimeUtility();
		~TimeUtility();

	public:
		TIME_UTILITY_API static const std::string current_timestamp();
	};
}


