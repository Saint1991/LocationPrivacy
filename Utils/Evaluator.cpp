#include "stdafx.h"
#include "Evaluator.h"

namespace Evaluation
{


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	Evaluator::Evaluator() : modules()
	{
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	Evaluator::~Evaluator()
	{
	}


	///<summary>
	/// �]���w�W���v�Z����results�Ɋi�[����
	///</summary>
	void Evaluator::evaluate()
	{
		for (std::vector<std::shared_ptr<IEvaluationModule>>::const_iterator iter = modules.begin(); iter != modules.end(); iter++) {
			results.insert((*iter)->calculate_evaluation_metric());
		}
	}


	///<summary>
	/// EvaluationModule��ǉ�����
	///</summary>
	void Evaluator::add_module(std::shared_ptr<IEvaluationModule> module)
	{
		modules.push_back(module);
	}


	///<summary>
	/// Module���X�g��ǉ�����
	///</summary>
	void Evaluator::add_modules(const std::vector<std::shared_ptr<IEvaluationModule>>& module_list)
	{
		modules.insert(modules.end(), module_list.begin(), module_list.end());
	}


	///<summary>
	/// �]���w�W�v�Z���ʂ��擾
	///</summary>
	const std::unordered_map<metric_name, metric_value> Evaluator::get_results() const
	{
		return results;
	}
}

