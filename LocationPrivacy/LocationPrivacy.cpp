// LocationPrivacy.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "PaisSimulator.h"
#include "ISimulator.h"
#include "BasicDbMap.h"
#include "BasicUser.h"
#include "Dummy.h"
#include "BasicRequirement.h"
#include "SemanticTrajectory.h"

int _tmain(int argc, _TCHAR* argv[])
{	

	const int USER_ID = 1;

	//������ISimulator�����������N���X���g��������
	std::unique_ptr<Simulation::ISimulator<Map::BasicDbMap, User::BasicUser<>, Entity::Dummy<>, Requirement::BasicRequirement, Geography::LatLng, Graph::SemanticTrajectory<>>> simulator
		= std::make_unique<Simulation::PaisSimulator>();
	simulator->build_map();
	simulator->create_trajectories(USER_ID);
	simulator->create_user();
	simulator->make_requirement_list();
	simulator->run();
	return 0;
}

