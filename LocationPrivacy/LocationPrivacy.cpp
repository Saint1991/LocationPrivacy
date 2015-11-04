// LocationPrivacy.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "ISimulator.h"

#include "HayashidaSimulator.h"


int _tmain(int argc, _TCHAR* argv[])
{	
	//ここでISimulatorを実装したクラスを使い分ける
	std::unique_ptr<Simulation::ISimulator<Map::BasicDbMap, Entity::PauseMobileEntity<Geography::LatLng>, Entity::PauseMobileEntity<Geography::LatLng>, Requirement::KatoMethodRequirement>> simulator
		= std::make_unique<Simulation::HayashidaSimulator>();
	simulator->build_map();
	simulator->create_user(0);
	simulator->make_requirement_list();
	simulator->run();
	return 0;
}

