#pragma once

namespace Requirement
{

	///<summary>
	/// �V�~�����[�V�����̃p�����[�^
	///</summary>
	struct BasicRequirement
	{
		unsigned int simulation_time;
		unsigned int service_interval;
		double required_anonymous_area;
		size_t dummy_num;
		
		BasicRequirement();
		BasicRequirement(unsigned int simulation_time, unsigned int service_interval, double required_anonymous_area, size_t dummy_num);
		~BasicRequirement();
	};
}


