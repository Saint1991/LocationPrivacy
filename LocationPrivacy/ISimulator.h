#pragma once

namespace Simulation
{
	class ISimulator
	{
	protected:
		
	public:
		ISimulator() {}
		virtual ~ISimulator();

		virtual void build_map() = 0;
		virtual void create_user_movement() = 0;
		virtual void run() = 0;
		virtual void export_datas() = 0;
		virtual void terminate() = 0;
	};
}


