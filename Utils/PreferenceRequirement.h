#ifdef UTILS_EXPORTS
#define PREFERENCE_REQUIREMENT_API __declspec(dllexport)
#else
#define PREFERENCE_REQUIREMENT_API __declspec(dllimport)
#endif

#pragma once
#include "BasicRequirement.h"

namespace Requirement
{
	struct PREFERENCE_REQUIREMENT_API PreferenceRequirement : public BasicRequirement
	{
		double required_preference_conservation;

		PreferenceRequirement();
		PreferenceRequirement(double required_anonymous_area, size_t dummy_num, double required_preference_conservation, double average_speed = 1.5, double speed_range = 0.5);
		~PreferenceRequirement();
	};
}



