#include "stdafx.h"
#include "ProbabilityEdge.h"

namespace User
{
	
	///<summary>
	/// コンストラクタ
	///</summary>
	ProbabilityEdge::ProbabilityEdge(Graph::node_id to, double probability) : Graph::BasicEdge(to)
	{
		set_probability(probability);
	}


	///<summary>
	/// デストラクタ
	///</summary>
	ProbabilityEdge::~ProbabilityEdge()
	{
	}


	///<summary>
	/// 確率値を設定します
	/// 値が不正な場合はinvalid_argumentをスローしfalseを返します
	/// ただし1.0より大きい値は例外を投げずに値を1.0に補正します
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
	/// 確率値を取得します
	///</summary>
	double ProbabilityEdge::probability() const
	{
		return _probability;
	}
}

