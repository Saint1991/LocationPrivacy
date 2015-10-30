// LocationPrivacy.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
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

	constexpr int USER_ID = 1;
	constexpr double TESTSET_PROPORTION = 0.3;

	//ここでISimulatorを実装したクラスを使い分ける
	std::unique_ptr<Simulation::BaseSimulator> simulator
		= std::make_unique<Simulation::PaisSimulator>(TESTSET_PROPORTION);
	simulator->build_map();
	simulator->create_trajectories(USER_ID);

	simulator->make_requirement_list();
	simulator->run();
	simulator->evaluate();
	simulator->export_evaluation_result();
	return 0;
}

