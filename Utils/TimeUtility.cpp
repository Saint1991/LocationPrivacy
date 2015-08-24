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
	/// ���ݎ����̃^�C���X�^���v��������擾����
	/// 20150824164900 <= 2015�N8��24��16��49��00�b
	///</summary>
	const std::string TimeUtility::current_timestamp()
	{
		time_t now;
		tm tm_now;
		time(&now);
		errno_t result = localtime_s(&tm_now, &now);
		
		std::stringstream timestamp;
		int year = tm_now.tm_year + 1900;
		int month = tm_now.tm_mon + 1;
		timestamp << year << month << tm_now.tm_mday << tm_now.tm_min << tm_now.tm_sec;
		return timestamp.str();
	}
}

