#pragma once
#include "stdafx.h"

namespace Evaluation
{

	typedef std::string metric_name;
	typedef double metric_value;

	///<summary>
	/// 評価指標計算のためのインタフェース
	///</summary>
	class IEvaluationModule 
	{
	public:		
		const metric_name name;
		IEvaluationModule(const metric_name& name) : name(name) {}
		virtual std::pair<metric_name, metric_value> calculate_evaluation_metric() const = 0;
	};
}