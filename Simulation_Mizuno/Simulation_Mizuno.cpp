#include "stdafx.h"
#include "PaisSimulator.h"
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
	std::unique_ptr<Simulation::BaseSimulator> simulator	= std::make_unique<Simulation::PaisSimulator>();
	simulator->prepare();
	simulator->run();
	//simulator->evaluate();
	//simulator->export_evaluation_result(export_path.str());

	return 0;
}

