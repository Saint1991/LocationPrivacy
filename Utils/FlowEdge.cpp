#include "stdafx.h"
#include "FlowEdge.h"

namespace Graph
{


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	FlowEdge::FlowEdge(node_id id, double initial_flow) : Edge<double>(id, std::make_shared<double>(initial_flow))
	{
	}


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	FlowEdge::FlowEdge(node_id id, std::shared_ptr<double> initial_flow) : Edge<double>(id, initial_flow)
	{

	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	FlowEdge::~FlowEdge()
	{
	}


	///<summary>
	/// �ێ�����Flow�̗ʂ�amount�������₷
	///</summary>
	void FlowEdge::flow_in(double amount)
	{
		*data += amount;
	}

	///<summary>
	/// �ێ�����Flow�̗ʂ�amount�������炷
	///</summary>
	void FlowEdge::flow_out(double amount)
	{
		*data -= amount;
	}


	///<summary>
	/// Flow�̗ʂ��擾����
	///</summary>
	double FlowEdge::get_flow() const
	{
		return *data;
	}


	///<summary>
	/// Flow�l���Đݒ肷��
	///</summary>
	void FlowEdge::set_flow(double amount)
	{
		*data = amount;
	}
}

