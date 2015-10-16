#ifdef UTILS_EXPORTS
#define KATOMETHODREQUIREMENT_API __declspec(dllexport)
#else
#define KATOMETHODREQUIREMENT_API __declspec(dllimport)
#endif

#pragma once
#include "BasicRequirement.h"

namespace Requirement {

	///<summary>
	/// ������@�ɕK�v��Requirement
	///</summary>
	class KATOMETHODREQUIREMENT_API KatoMethodRequirement : public Requirement::BasicRequirement
	{
	public:
		KatoMethodRequirement();
		virtual ~KatoMethodRequirement();
	};
}
