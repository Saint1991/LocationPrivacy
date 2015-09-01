#pragma once

namespace Simulation
{
	class ISimulator
	{
	protected:
		
	public:
		ISimulator() {}
		virtual ~ISimulator();

		virtual void initialize() = 0;
		virtual void run() = 0;
		virtual void export_datas() = 0;
		virtual void terminate() = 0;
	};
}


