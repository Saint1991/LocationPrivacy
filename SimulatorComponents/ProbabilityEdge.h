#pragma once
#include "BasicEdge.h"

namespace User
{
	///<summary>
	/// �J�ڊm����ێ�����G�b�W
	///</summary>
	class ProbabilityEdge : public Graph::BasicEdge
	{
	private:
		double _probability;
	public:
		bool set_probability(double probability);
		double probability() const;
		ProbabilityEdge(Graph::node_id to, double probability = 1.0);
		virtual ~ProbabilityEdge();
	};
}


