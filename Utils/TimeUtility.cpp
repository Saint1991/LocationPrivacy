#include "stdafx.h"
#include "TimeUtility.h"

namespace Time
{

	TimeUtility::TimeUtility()
	{
	}

	TimeUtility::~TimeUtility()
	{
	}

	///<summary>
	/// 現在時刻のタイムスタンプ文字列を取得する
	/// 20150824164900 <= 2015年8月24日16時49分00秒
	///</summary>
	const std::string TimeUtility::current_timestamp()
	{
		time_t now;
		tm tm_now;
		time(&now);
		errno_t result = localtime_s(&tm_now, &now);
		
		std::stringstream timestamp;
		int year = tm_now.tm_year + 1900;
		
		char month[3];
		sprintf_s(month, 3, "%02d", tm_now.tm_mon + 1);
		
		char day[3];
		sprintf_s(day, 3, "%02d", tm_now.tm_mday);

		char hour[3];
		sprintf_s(hour, 3, "%02d", tm_now.tm_hour);

		char min[3];
		sprintf_s(min, 3, "%02d", tm_now.tm_min);

		char sec[3];
		sprintf_s(sec, 3, "%02d", tm_now.tm_sec);

		timestamp << year << month << day << hour <<min << sec;
		return timestamp.str();
	}
}

