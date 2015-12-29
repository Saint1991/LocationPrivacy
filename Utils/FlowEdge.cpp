#include "stdafx.h"
#include "FlowEdge.h"

namespace Graph
{


	///<summary>
	/// コンストラクタ
	///</summary>
	FlowEdge::FlowEdge(node_id id, double initial_flow) : Edge<double>(id, std::make_shared<double>(initial_flow))
	{
	}


	///<summary>
	/// コンストラクタ
	///</summary>
	FlowEdge::FlowEdge(node_id id, std::shared_ptr<double> initial_flow) : Edge<double>(id, initial_flow)
	{

	}


	///<summary>
	/// デストラクタ
	///</summary>
	FlowEdge::~FlowEdge()
	{
	}


	///<summary>
	/// 保持するFlowの量をamountだけ増やす
	///</summary>
	void FlowEdge::flow_in(double amount)
	{
		*data += amount;
	}

	///<summary>
	/// 保持するFlowの量をamountだけ減らす
	///</summary>
	void FlowEdge::flow_out(double amount)
	{
		*data -= amount;
	}


	///<summary>
	/// Flowの量を取得する
	///</summary>
	double FlowEdge::get_flow() const
	{
		return *data;
	}


	///<summary>
	/// Flow値を再設定する
	///</summary>
	void FlowEdge::set_flow(double amount)
	{
		*data = amount;
	}
}

