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


	///<summary>
	/// Timestamp�������unixtimestamp (time_t)�ɕϊ�����
	/// �s����Timestamp�̏ꍇ��INVALID = -1��Ԃ�
	///</summary>
	time_t TimeUtility::convert_to_unixtimestamp(std::string timestamp)
	{
		if (timestamp.length() != 19U) return INVALID;
		tm time;
		time.tm_year = std::stoi(timestamp.substr(0, 4)) - 1900;
		time.tm_mon = std::stoi(timestamp.substr(5, 2)) - 1;
		time.tm_mday = std::stoi(timestamp.substr(8, 2));
		time.tm_hour = std::stoi(timestamp.substr(11, 2));
		time.tm_min = std::stoi(timestamp.substr(14, 2));
		time.tm_sec = std::stoi(timestamp.substr(17, 2));
		time.tm_isdst = -1;
		return mktime(&time);
	}
}

