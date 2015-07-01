#ifdef UTILS_EXPORTS
#define ANGLE_UTILITY_API __declspec(dllexport)
#else
#define ANGLE_UTILITY_API __declspec(dllimport)
#endif

#pragma once

namespace Math {
	class  AngleUtility
	{
	
	private:
		AngleUtility();
		virtual ~AngleUtility();

	public:
		ANGLE_UTILITY_API static double convert_to_radian(double angle_degree);
		ANGLE_UTILITY_API static double convert_to_degree(double angle_radian);
	};
}


