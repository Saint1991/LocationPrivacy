#include "stdafx.h"
#include "StateResolver.h"

namespace Observer
{


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	StateResolver::StateResolver(std::shared_ptr<Map::BasicDbMap> map) : map(map)
	{

	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	StateResolver::~StateResolver()
	{

	}

	///<summary>
	/// �m�[�h�Ɗϑ����ꂽ�G���e�B�e�B���̕ω���񂩂�J�ڊm���������ăG�b�W�̒l���X�V
	///</summary>
	void StateResolver::resolve(std::vector<std::shared_ptr<ObservedTrajectoryNode>>& current_state_nodes, std::vector<std::shared_ptr<ObservedTrajectoryNode>>& next_state_nodes)
	{

	}
}

