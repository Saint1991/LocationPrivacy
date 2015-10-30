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
#include "TimeUtility.h"

int _tmain(int argc, _TCHAR* argv[])
{	

	constexpr int USER_ID = 1;
	constexpr double TESTSET_PROPORTION = 0.3;

	std::stringstream export_path;
	export_path << "C:/Users/Mizuno/Desktop/EvaluationResults/" << Time::TimeUtility::current_timestamp() << "/";

	//ここでISimulatorを実装したクラスを使い分ける
	std::unique_ptr<Simulation::BaseSimulator> simulator
		= std::make_unique<Simulation::PaisSimulator>(USER_ID, TESTSET_PROPORTION);
	simulator->prepare();
	simulator->run();
	simulator->evaluate();
	simulator->export_evaluation_result(export_path.str());
	return 0;
}

