#include "stdafx.h"
#include "DeimSimulator.h"
#include "ISimulator.h"
#include "BasicDbMap.h"
#include "BasicUser.h"
#include "Dummy.h"
#include "BasicRequirement.h"
#include "SemanticTrajectory.h"
#include "TimeUtility.h"
#include "Rectangle.h"


int _tmain(int argc, _TCHAR* argv[])
{

	//‚±‚±‚ÅISimulator‚ðŽÀ‘•‚µ‚½ƒNƒ‰ƒX‚ðŽg‚¢•ª‚¯‚é
	std::unique_ptr<Simulation::BaseSimulator> simulator	= std::make_unique<Simulation::DeimSimulator>();
	simulator->prepare();
	simulator->run();
	simulator->evaluate();
	simulator->export_evaluation_result();

	return 0;
}

