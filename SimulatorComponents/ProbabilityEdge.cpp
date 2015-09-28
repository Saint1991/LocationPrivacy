#include "stdafx.h"
#include "ProbabilityEdge.h"

namespace User
{
	
	///<summary>
	/// �R���X�g���N�^
	///</summary>
	ProbabilityEdge::ProbabilityEdge(Graph::node_id to, double probability) : Graph::BasicEdge(to)
	{
		set_probability(probability);
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	ProbabilityEdge::~ProbabilityEdge()
	{
	}


	///<summary>
	/// �m���l��ݒ肵�܂�
	/// �l���s���ȏꍇ��invalid_argument���X���[��false��Ԃ��܂�
	/// ������1.0���傫���l�͗�O�𓊂����ɒl��1.0�ɕ␳���܂�
	///</summary>
	bool ProbabilityEdge::set_probability(double probability)
	{
		if (0.0 <= probability) {
			_probability = probability > 1.0 ? 1.0 : probability;
			return true;
		}
		throw std::invalid_argument("Probability Must be in the range [0, 1]");
		_probability = 1.0;
		return false;
	}


	///<summary>
	/// �m���l���擾���܂�
	///</summary>
	double ProbabilityEdge::probability() const
	{
		return _probability;
	}
}

