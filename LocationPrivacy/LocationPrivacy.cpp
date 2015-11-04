// LocationPrivacy.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "ISimulator.h"

#include "HayashidaSimulator.h"


int _tmain(int argc, _TCHAR* argv[])
{	
	//������ISimulator�����������N���X���g��������
	std::unique_ptr<Simulation::ISimulator<Map::BasicDbMap, Entity::PauseMobileEntity<Geography::LatLng>, Entity::PauseMobileEntity<Geography::LatLng>, Requirement::KatoMethodRequirement>> simulator
		= std::make_unique<Simulation::HayashidaSimulator>();
	simulator->build_map();
	simulator->create_user(0);
	simulator->make_requirement_list();
	simulator->run();
	return 0;
}

