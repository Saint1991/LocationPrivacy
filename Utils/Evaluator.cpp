#include "stdafx.h"
#include "Evaluator.h"

namespace Evaluation
{


	///<summary>
	/// コンストラクタ
	///</summary>
	Evaluator::Evaluator() : modules()
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	Evaluator::~Evaluator()
	{
	}


	///<summary>
	/// 評価指標を計算してresultsに格納する
	///</summary>
	void Evaluator::evaluate()
	{
		for (std::vector<std::shared_ptr<IEvaluationModule>>::const_iterator iter = modules.begin(); iter != modules.end(); iter++) {
			results.insert((*iter)->calculate_evaluation_metric());
		}
	}


	///<summary>
	/// EvaluationModuleを追加する
	///</summary>
	void Evaluator::add_module(std::shared_ptr<IEvaluationModule> module)
	{
		modules.push_back(module);
	}


	///<summary>
	/// Moduleリストを追加する
	///</summary>
	void Evaluator::add_modules(const std::vector<std::shared_ptr<IEvaluationModule>>& module_list)
	{
		modules.insert(modules.end(), module_list.begin(), module_list.end());
	}


	///<summary>
	/// 評価指標計算結果を取得
	///</summary>
	const std::unordered_map<metric_name, metric_value> Evaluator::get_results() const
	{
		return results;
	}
}

