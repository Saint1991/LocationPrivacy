#include "stdafx.h"
#include "KatoMethodRequirement.h"

namespace Requirement {
	
	///<summary>
	/// コンストラクタ
	///</summary>
	KatoMethodRequirement::KatoMethodRequirement()
	{
	}


	///<summary>
	/// コンストラクタ
	///</summary>
	KatoMethodRequirement::KatoMethodRequirement(double max_pause_time, double min_pause_time):max_pause_time(max_pause_time),min_pause_time(min_pause_time)
	{
	}

	///<summary>
	/// デストラクタ
	///</summary>
	KatoMethodRequirement::~KatoMethodRequirement()
	{
	}

}