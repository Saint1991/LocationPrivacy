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
		time(&now);
		return timestamp(now);
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


	///<summary>
	/// UNIX_TIMESTAMP������������ɕϊ�����
	///</summary>
	std::string TimeUtility::timestamp(time_t time)
	{
		tm tm_now;
		errno_t result = localtime_s(&tm_now, &time);

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

		timestamp << year << month << day << hour << min << sec;
		return timestamp.str();
	}


	///<summary>
	/// UNIXTIMESTAMP������t(2015/10/29��)���擾����
	///</summary>
	std::string TimeUtility::date(time_t time)
	{
		tm tm_now;
		errno_t result = localtime_s(&tm_now, &time);

		std::stringstream timestamp;
		int year = tm_now.tm_year + 1900;

		char month[3];
		sprintf_s(month, 3, "%02d", tm_now.tm_mon + 1);

		char day[3];
		sprintf_s(day, 3, "%02d", tm_now.tm_mday);

		timestamp << year << month << day;
		return timestamp.str();
	}

	


	///<summary>
	/// 2�̎����̎��ԊԊu��Ԃ�
	///</summary>
	int TimeUtility::interval(const std::string& time1, const std::string& time2)
	{
		return std::abs(convert_to_unixtimestamp(time1) - convert_to_unixtimestamp(time2));
	}


	///<summary>
	/// time����forward_time������������i�߂��^�C���X�^���v���擾����
	///</summary>
	std::string TimeUtility::forward(const std::string& time, time_t forward_time)
	{
		time_t newtime = convert_to_unixtimestamp(time) + forward_time;
		std::string newtime_str = timestamp(newtime);
		std::stringstream ret;
		ret << newtime_str.substr(0, 4) << "-" << newtime_str.substr(4, 2) << "-" << newtime_str.substr(6, 2) << " " << newtime_str.substr(8, 2) << ":" << newtime_str.substr(10, 2) << ":" << newtime_str.substr(12, 2);
		return ret.str();
	}


	///<summary>
	/// time_t����MySQL�̃^�C���X�^���v������ɕϊ�����
	///</summary>
	std::string TimeUtility::mysql_timestamp(time_t time)
	{
		tm tm_now;
		errno_t result = localtime_s(&tm_now, &time);

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

		timestamp << year << "-" << month << "-" << day << " " << hour << ":" << min << ":" << sec;
		return timestamp.str();
	}
}

