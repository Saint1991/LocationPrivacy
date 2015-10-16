#ifdef UTILS_EXPORTS
#define KATOMETHODREQUIREMENT_API __declspec(dllexport)
#else
#define KATOMETHODREQUIREMENT_API __declspec(dllimport)
#endif

#pragma once
#include "BasicRequirement.h"

namespace Requirement {

	///<summary>
	/// ‰Á“¡Žè–@‚É•K—v‚ÈRequirement
	///</summary>
	class KATOMETHODREQUIREMENT_API KatoMethodRequirement : public Requirement::BasicRequirement
	{
	protected:
		double max_pause_time;
		double min_pause_time;

	public:
		KatoMethodRequirement();
		KatoMethodRequirement(double max_pause_time, double min_pause_time);
		virtual ~KatoMethodRequirement();
	};
}
